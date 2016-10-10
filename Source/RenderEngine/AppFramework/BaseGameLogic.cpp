#include "BaseGameLogic.h"
#include "RenderEngineApp.h"
#include "HumanView.h"
#include "../Actors/ActorFactory.h"
#include "../Actors/Actor.h"
#include "../ResourceCache/XmlResource.h"
#include "../EventManager/Events.h"

BaseGameLogic::BaseGameLogic(RenderEngineApp* pApp) : m_pApp(pApp), m_pActorFactory(nullptr)
{
}

BaseGameLogic::~BaseGameLogic()
{
	while (!m_GameViews.empty())
	{
		m_GameViews.pop_front();
	}

	SAFE_DELETE(m_pActorFactory);

	for (auto actor : m_Actors)
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

void BaseGameLogic::VOnUpdate(float totalTime, float elapsedTime)
{
	int deltaMs = int(elapsedTime * 1000.0f);
	for (auto view : m_GameViews)
	{
		view->VOnUpdate(deltaMs);
	}

	for (auto actor : m_Actors)
	{
		actor.second->Update(deltaMs);
	}
}

bool BaseGameLogic::VLoadGame(const std::string& projectXml)
{
	TiXmlElement* pRoot = XmlResourceLoader::LoadAndReturnRootXmlElement(m_pApp->m_pResCache, projectXml.c_str());
	if (!pRoot)
	{
		GCC_ERROR("Failed to find level resource file: " + projectXml);
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
		shared_ptr<ResHandle> pResourceHandle = m_pApp->m_pResCache->GetHandle(&resource);
	}

	TiXmlElement* pActorsNode = pRoot->FirstChildElement("StaticActors");
	if (pActorsNode)
	{
		for (TiXmlElement* pNode = pActorsNode->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
		{
			const char* actorResource = pNode->Attribute("resource");

			StrongActorPtr pActor = VCreateActor(actorResource, pNode, Matrix());
			if (pActor)
			{
				shared_ptr<EvtData_New_Actor> pNewActorEvent(GCC_NEW EvtData_New_Actor(pActor->GetId()));
				IEventManager::Get()->VQueueEvent(pNewActorEvent);
			}
		}
	}

	for (auto gameView : m_GameViews)
	{
		shared_ptr<HumanView> pHumanView = static_pointer_cast<HumanView, IGameView>(gameView);
		pHumanView->LoadGame(pRoot);
	}

	if (!VLoadGameDelegate(pRoot))
		return false;

	if (postLoadScript)
	{
		Resource resource(postLoadScript);
		shared_ptr<ResHandle> pResourceHandle = m_pApp->m_pResCache->GetHandle(&resource);  // this actually loads the XML file from the zip file
	}

	shared_ptr<EvtData_Environment_Loaded> pNewGameEvent(GCC_NEW EvtData_Environment_Loaded);
	IEventManager::Get()->VTriggerEvent(pNewGameEvent);
	return true;
}

void BaseGameLogic::VAddView(shared_ptr<IGameView> pView, ActorId actorId)
{
	uint32_t viewId = m_GameViews.size();
	m_GameViews.push_back(pView);
	pView->VOnAttach(viewId, actorId);
	pView->VOnRestore();
}

void BaseGameLogic::VRemoveView(shared_ptr<IGameView> pView)
{
	m_GameViews.remove(pView);
}

StrongActorPtr BaseGameLogic::VCreateActor(
	const std::string& actorResource, TiXmlElement *overrides, const Matrix& initialTransform)
{
	GCC_ASSERT(m_pActorFactory);

	StrongActorPtr pActor = m_pActorFactory->CreateActor(actorResource.c_str(), overrides, initialTransform);
	if (pActor)
	{
		m_Actors.insert(std::make_pair(pActor->GetId(), pActor));
		return pActor;
	}
	else
	{
		return StrongActorPtr();
	}
}

void BaseGameLogic::VDestroyActor(ActorId actorId)
{
	shared_ptr<EvtData_Destroy_Actor> pEvent(GCC_NEW EvtData_Destroy_Actor(actorId));
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

ActorFactory* BaseGameLogic::VCreateActorFactory()
{
	return GCC_NEW ActorFactory(m_pApp);
}
