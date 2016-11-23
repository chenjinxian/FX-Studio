#include "BaseGameLogic.h"
#include "BaseGameApp.h"
#include "../Actors/ActorFactory.h"
#include "../Actors/Actor.h"
#include "../ResourceCache/XmlResource.h"
#include "../EventManager/Events.h"
#include "../UserInterface/HumanView.h"
#include "boost/lexical_cast.hpp"

BaseGameLogic::BaseGameLogic()
	: m_Actors(),
	m_LastActorId(0),
	m_GameState(BGS_Initializing),
	m_GameViews(),
	m_pActorFactory(nullptr),
	m_HumanPlayersAttached(0),
	m_HumanGamesLoaded(0),
	m_AIPlayersAttached(0),
	m_IsProxy(false),
	m_RemotePlayerId(0),
	m_IsRenderDiagnostics(false),
	m_pPhysics(nullptr)
{
// 	RegisterEngineScriptEvents();
// 	IEventManager::Get()->VAddListener(
// 		boost::bind(&BaseGameLogic::RequestDestroyActorDelegate, this, std::placeholders::_1), EvtData_Request_Destroy_Actor::sk_EventType);
}

BaseGameLogic::~BaseGameLogic()
{
	while (!m_GameViews.empty())
	{
		m_GameViews.pop_front();
	}

	SAFE_DELETE(m_pActorFactory);

	for (auto& actor : m_Actors)
	{
		actor.second->Destroy();
	}
	m_Actors.clear();
}

bool BaseGameLogic::Init()
{
	m_pActorFactory = VCreateActorFactory();

	return true;
}

std::string BaseGameLogic::GetActorXml(ActorId id)
{
	StrongActorPtr pActor = MakeStrongPtr(VGetActor(id));
	if (pActor)
		return pActor->ToXml();
	else
		DEBUG_ERROR("Couldn't find actor: " + boost::lexical_cast<std::string>(id));

	return std::string();
}

bool BaseGameLogic::VLoadGame(const std::string& projectXml)
{
	tinyxml2::XMLElement* pRoot = XmlResourceLoader::LoadAndReturnRootXmlElement(projectXml.c_str());
	if (!pRoot)
	{
		DEBUG_ERROR("Failed to find level resource file: " + projectXml);
		return false;
	}

	const char* preLoadScript = NULL;
	const char* postLoadScript = NULL;

	tinyxml2::XMLElement* pScriptElement = pRoot->FirstChildElement("Script");
	if (pScriptElement)
	{
		preLoadScript = pScriptElement->Attribute("preLoad");
		postLoadScript = pScriptElement->Attribute("postLoad");
	}

	if (preLoadScript)
	{
		Resource resource(preLoadScript);
		shared_ptr<ResHandle> pResourceHandle = g_pApp->m_pResCache->GetHandle(&resource);
	}

	tinyxml2::XMLElement* pActorsNode = pRoot->FirstChildElement("StaticActors");
	if (pActorsNode)
	{
		for (tinyxml2::XMLElement* pNode = pActorsNode->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
		{
			StrongActorPtr pActor = VCreateActor(pNode);
			if (pActor)
			{
				shared_ptr<EvtData_New_Actor> pNewActorEvent(DEBUG_NEW EvtData_New_Actor(pActor->GetActorId()));
				IEventManager::Get()->VQueueEvent(pNewActorEvent);
			}
		}
	}

	for (auto& gameView : m_GameViews)
	{
		if (gameView->VGetType() == GameView_Human)
		{
			shared_ptr<HumanView> pHumanView = static_pointer_cast<HumanView, IGameView>(gameView);
			pHumanView->LoadGame(pRoot);
		}
	}

	if (!VLoadGameDelegate(pRoot))
		return false;

	if (postLoadScript)
	{
		Resource resource(postLoadScript);
		shared_ptr<ResHandle> pResourceHandle = g_pApp->m_pResCache->GetHandle(&resource);
	}

	if (m_IsProxy)
	{
		shared_ptr<EvtData_Remote_Environment_Loaded> pNewGameEvent(DEBUG_NEW EvtData_Remote_Environment_Loaded);
		IEventManager::Get()->VTriggerEvent(pNewGameEvent);
	}
	else
	{
		shared_ptr<EvtData_Environment_Loaded> pNewGameEvent(DEBUG_NEW EvtData_Environment_Loaded);
		IEventManager::Get()->VTriggerEvent(pNewGameEvent);
	}
	return true;
}

void BaseGameLogic::VSetProxy()
{
	m_IsProxy = true;

	IEventManager::Get()->VAddListener(
		boost::bind(&BaseGameLogic::RequestNewActorDelegate, this, _1), EvtData_Request_New_Actor::sk_EventType);

// 	m_pPhysics.reset(CreateNullPhysics());
}

void BaseGameLogic::VAddView(shared_ptr<IGameView> pView, ActorId actorId)
{
	uint64_t viewId = m_GameViews.size();
	m_GameViews.push_back(pView);
	pView->VOnAttach(viewId, actorId);
	pView->VOnInitGameViews();
}

void BaseGameLogic::VRemoveView(shared_ptr<IGameView> pView)
{
	m_GameViews.remove(pView);
}

StrongActorPtr BaseGameLogic::VCreateActor(
	tinyxml2::XMLElement *pActorRoot, const Matrix& initialTransform, ActorId serversActorId)
{
	DEBUG_ASSERT(m_pActorFactory != nullptr);
	DEBUG_ASSERT(pActorRoot != nullptr);

	if (!m_IsProxy && serversActorId != INVALID_ACTOR_ID)
		return StrongActorPtr();

	if (m_IsProxy && serversActorId == INVALID_ACTOR_ID)
		return StrongActorPtr();

	StrongActorPtr pActor = m_pActorFactory->CreateActor(pActorRoot, initialTransform, serversActorId);
	if (pActor)
	{
		m_Actors.insert(std::make_pair(pActor->GetActorId(), pActor));
		if (!m_IsProxy && (m_GameState == BGS_SpawningPlayersActors || m_GameState == BGS_Running))
		{
// 			shared_ptr<EvtData_Request_New_Actor> pNewActor(DEBUG_NEW EvtData_Request_New_Actor(actorResource, initialTransform, pActor->GetId()));
// 			IEventManager::Get()->VTriggerEvent(pNewActor);
		}
		return pActor;
	}
	else
	{
		return StrongActorPtr();
	}
}

void BaseGameLogic::VDestroyActor(ActorId actorId)
{
	shared_ptr<EvtData_Destroy_Actor> pEvent(DEBUG_NEW EvtData_Destroy_Actor(actorId));
	IEventManager::Get()->VTriggerEvent(pEvent);

	auto findIt = m_Actors.find(actorId);
	if (findIt != m_Actors.end())
	{
		findIt->second->Destroy();
		m_Actors.erase(findIt);
	}
}

WeakActorPtr BaseGameLogic::VGetActor(ActorId actorId)
{
	auto findIt = m_Actors.find(actorId);
	if (findIt != m_Actors.end())
		return findIt->second;
	return WeakActorPtr();
}

void BaseGameLogic::VMoveActor(ActorId actorId, const Matrix& mat)
{
	 
}

void BaseGameLogic::VModifyActor(const ActorId actorId, tinyxml2::XMLElement* overrides)
{
	DEBUG_ASSERT(m_pActorFactory);
	if (!m_pActorFactory)
		return;

	auto findIt = m_Actors.find(actorId);
	if (findIt != m_Actors.end())
	{
		m_pActorFactory->ModifyActor(findIt->second, overrides);
	}
}

ActorFactory* BaseGameLogic::VCreateActorFactory()
{
	return DEBUG_NEW ActorFactory();
}

void BaseGameLogic::VOnUpdate(const GameTime& gameTime)
{
	switch (m_GameState)
	{
	case BGS_Initializing:
		VChangeState(BGS_MainMenu);
		break;

	case BGS_MainMenu:
		break;

	case BGS_LoadingGameEnvironment:
		break;

	case BGS_WaitingForPlayersToLoadEnvironment:
		VChangeState(BGS_SpawningPlayersActors);
		break;

	case BGS_SpawningPlayersActors:
		VChangeState(BGS_Running);
		break;

	case BGS_WaitingForPlayers:
		if (!g_pApp->m_Config.m_Project.empty())
		{
			VChangeState(BGS_LoadingGameEnvironment);
		}
		break;

	case BGS_Running:
// 		m_pProcessManager->UpdateProcesses(gameTime);

		if (m_pPhysics && !m_IsProxy)
		{
			m_pPhysics->VOnUpdate(gameTime);
			m_pPhysics->VSyncVisibleScene();
		}

		break;

	default:
		DEBUG_ERROR("Unrecognized state.");
	}

	for (auto& gameView : m_GameViews)
	{
		gameView->VOnUpdate(gameTime);
	}

	for (auto& actor : m_Actors)
	{
		actor.second->Update(gameTime);
	}
}

void BaseGameLogic::VChangeState(BaseGameState gameState)
{
	if (gameState == BGS_WaitingForPlayers)
	{
		m_GameViews.pop_front();

		VSetProxy();

		if (!g_pApp->AttachAsClient())
		{
			VChangeState(BGS_MainMenu);
			return;
		}
	}
	else if (gameState == BGS_LoadingGameEnvironment)
	{
		m_GameState = gameState;
		if (!g_pApp->VLoadGame())
		{
			DEBUG_ERROR("The game failed to load.");
			g_pApp->AbortGame();
		}
		else
		{
			VChangeState(BGS_WaitingForPlayersToLoadEnvironment);
			return;
		}
	}

	m_GameState = gameState;
}

void BaseGameLogic::VRenderDiagnostics()
{
	if (m_IsRenderDiagnostics)
	{
// 		m_pPhysics->VRenderDiagnostics();
	}
}

void BaseGameLogic::RequestDestroyActorDelegate(IEventDataPtr pEventData)
{
// 	shared_ptr<EvtData_Request_Destroy_Actor> pCastEventData = static_pointer_cast<EvtData_Request_Destroy_Actor>(pEventData);
// 	VDestroyActor(pCastEventData->GetActorId());
}

void BaseGameLogic::MoveActorDelegate(IEventDataPtr pEventData)
{
	shared_ptr<EvtData_Move_Actor> pCastEventData = static_pointer_cast<EvtData_Move_Actor>(pEventData);
	VMoveActor(pCastEventData->GetActorId(), pCastEventData->GetMatrix());
}

void BaseGameLogic::RequestNewActorDelegate(IEventDataPtr pEventData)
{
	DEBUG_ASSERT(m_IsProxy);
	if (!m_IsProxy)
		return;

// 	shared_ptr<EvtData_Request_New_Actor> pCastEventData = static_pointer_cast<EvtData_Request_New_Actor>(pEventData);
// 
// 	StrongActorPtr pActor = VCreateActor(pCastEventData->GetActorResource(), NULL, pCastEventData->GetInitialTransform(), pCastEventData->GetServerActorId());
// 	if (pActor)
// 	{
// 		shared_ptr<EvtData_New_Actor> pNewActorEvent(DEBUG_NEW EvtData_New_Actor(pActor->GetId(), pCastEventData->GetViewId()));
// 		IEventManager::Get()->VQueueEvent(pNewActorEvent);
// 	}
}
