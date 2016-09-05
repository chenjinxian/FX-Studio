#include "BaseGameLogic.h"
#include "../ResourceCache/XmlResource.h"
#include "../Actors/Actor.h"
#include "../Actors/ActorFactory.h"

BaseGameLogic::BaseGameLogic(RenderEngineApp* pApp)
	: m_pApp(pApp), m_LifeTime(.0f), m_LastActorId(0), m_State(BGS_Initializing), m_HumanGamesLoaded(0),
	m_pActorFactory(nullptr), m_pProcessManager(GCC_NEW ProcessManager), m_pLevelManager(nullptr)
{
}

BaseGameLogic::~BaseGameLogic()
{
	while (!m_GameViews.empty())
		m_GameViews.pop_front();

	SAFE_DELETE(m_pLevelManager);
	SAFE_DELETE(m_pProcessManager);
	SAFE_DELETE(m_pActorFactory);

	for (auto& actor : m_Actors)
	{
// 		actor->second->Destory();
	}
	m_Actors.clear();
}

bool BaseGameLogic::Init()
{
	m_pActorFactory = VCreateActorFactory();
	return true;
}

ActorId BaseGameLogic::GetNewActorID()
{
	return ++m_LastActorId;
}

void BaseGameLogic::VAddView(shared_ptr<IGameView> pView, ActorId actorId /*= INVALID_ACTOR_ID*/)
{
	int viewId = static_cast<int>(m_GameViews.size());
	m_GameViews.push_back(pView);
	pView->VOnAttach(viewId, actorId);
	pView->VOnRestore();
}

void BaseGameLogic::VRemoveView(shared_ptr<IGameView> pView)
{
	m_GameViews.remove(pView);
}

StrongActorPtr BaseGameLogic::VCreateActor(const std::string &actorResource, TiXmlElement *overrides, const Matrix *initialTransform /*= NULL*/, const ActorId serversActorId /*= INVALID_ACTOR_ID*/)
{
	GCC_ASSERT(m_pActorFactory);
	if (serversActorId != INVALID_ACTOR_ID)
		return StrongActorPtr();

	StrongActorPtr pActor = m_pActorFactory->CreateActor(actorResource.c_str(), overrides, initialTransform, serversActorId);
	if (pActor)
	{
		m_Actors.insert(std::make_pair(pActor->GetId(), pActor));
		if (m_State == BGS_Running)
		{
// 			shared_ptr<EvtData_Request_New_Actor> pNewActor(GCC_NEW EvtData_Request_New_Actor(actorResource, initialTransform, pActor->GetId()));
// 			IEventManager::Get()->VTriggerEvent(pNewActor);
		}
		return pActor;
	}
	else
	{
		// FUTURE WORK: Log error: couldn't create actor
		return StrongActorPtr();
	}
}

void BaseGameLogic::VDestroyActor(const ActorId actorId)
{

}

WeakActorPtr BaseGameLogic::VGetActor(const ActorId actorId)
{
	ActorMap::iterator findIt = m_Actors.find(actorId);
	if (findIt != m_Actors.end())
		return findIt->second;
	return WeakActorPtr();
}

void BaseGameLogic::VModifyActor(const ActorId actorId, TiXmlElement *overrides)
{

}

void BaseGameLogic::VMoveActor(const ActorId actorId, Matrix const &mat)
{

}

bool BaseGameLogic::VLoadGame(const char* levelResource)
{
	TiXmlElement* pRoot = XmlResourceLoader::LoadAndReturnRootXmlElement(m_pApp->GetResCache(), levelResource);
	if (!pRoot)
	{
		GCC_ERROR("Failed to find level resource file: " + std::string(levelResource));
		return false;
	}

	const char* preLoadScript = NULL;
	const char* postLoadScript = NULL;
	TiXmlElement* pScriptElement = pRoot->FirstChildElement("Script");
	if (pScriptElement)
	{
		preLoadScript = pScriptElement->Attribute("preLoad");
		postLoadScript = pScriptElement->Attribute("postLoad");
	}

	if (preLoadScript)
	{
		Resource resource(preLoadScript);
		shared_ptr<ResHandle> pResourceHandle = m_pApp->GetResCache()->GetHandle(&resource);
	}

	TiXmlElement* pActorsNode = pRoot->FirstChildElement("StaticActors");
	if (pActorsNode)
	{
		for (TiXmlElement* pNode = pActorsNode->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
		{
			const char* actorResource = pNode->Attribute("resource");

			StrongActorPtr pActor = VCreateActor(actorResource, pNode);
			if (pActor)
			{
// 				shared_ptr<EvtData_New_Actor> pNewActorEvent(GCC_NEW EvtData_New_Actor(pActor->GetId()));
// 				IEventManager::Get()->VQueueEvent(pNewActorEvent);
			}
		}
	}

	for (auto it : m_GameViews)
	{
		if (it->VGetType() == GameView_Human)
		{
// 			shared_ptr<HumanView> pHumanView = static_pointer_cast<HumanView, IGameView>(pView);
// 			pHumanView->LoadGame(pRoot);
		}
	}

	if (!VLoadGameDelegate(pRoot))
		return false;

	if (postLoadScript)
	{
		Resource resource(postLoadScript);
		shared_ptr<ResHandle> pResourceHandle = m_pApp->GetResCache()->GetHandle(&resource);  // this actually loads the XML file from the zip file
	}

// 	shared_ptr<EvtData_Environment_Loaded> pNewGameEvent(GCC_NEW EvtData_Environment_Loaded);
// 	IEventManager::Get()->VTriggerEvent(pNewGameEvent);
	return true;
}

void BaseGameLogic::VChangeState(enum BaseGameState newState)
{
	if (newState == BGS_LoadingGameEnvironment)
	{
		m_State = newState;
		if (!m_pApp->VLoadGame())
		{
			GCC_ERROR("The game failed to load.");
// 			m_pApp->AbortGame();
		}
		else
		{
			return;
		}
	}

	m_State = newState;
}

void BaseGameLogic::VOnUpdate(float time, float elapsedTime)
{
	int deltaMilliseconds = int(elapsedTime * 1000.0f);
	m_LifeTime += elapsedTime;

	switch (m_State)
	{
	case BGS_Initializing:
// 		VChangeState(BGS_MainMenu);
		break;
	case BGS_Running:
		m_pProcessManager->UpdateProcesses(deltaMilliseconds);
		break;
	default:
		GCC_ERROR("Unrecognized state.");
	}

	for (auto& view : m_GameViews)
	{
		view->VOnUpdate(deltaMilliseconds);
	}

	for (auto& actor : m_Actors)
	{
// 		actor.second->Update(deltaMilliseconds);
	}
}

ActorFactory* BaseGameLogic::VCreateActorFactory(void)
{
	return GCC_NEW ActorFactory;
}
