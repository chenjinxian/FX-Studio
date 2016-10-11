#include "Scene.h"
#include "SceneNode.h"
#include "../EventManager/EventManager.h"
#include "../EventManager/Events.h"

Scene::Scene(shared_ptr<IRenderer> pRenderer) : m_pRenderer(pRenderer), m_pRootNode(nullptr), m_pCamera(nullptr)
{
	IEventManager* pEventMgr = IEventManager::Get();
	pEventMgr->VAddListener(
		boost::bind(&Scene::NewRenderComponentDelegate, this, std::placeholders::_1), EvtData_New_Render_Component::sk_EventType);
	pEventMgr->VAddListener(
		boost::bind(&Scene::DestroyActorDelegate, this, std::placeholders::_1), EvtData_Destroy_Actor::sk_EventType);
	pEventMgr->VAddListener(
		boost::bind(&Scene::MoveActorDelegate, this, std::placeholders::_1), EvtData_Move_Actor::sk_EventType);
	pEventMgr->VAddListener(
		boost::bind(&Scene::ModifiedRenderComponentDelegate, this, std::placeholders::_1), EvtData_Modified_Render_Component::sk_EventType);
}

Scene::~Scene()
{
	IEventManager* pEventMgr = IEventManager::Get();
	pEventMgr->VRemoveListener(
		boost::bind(&Scene::NewRenderComponentDelegate, this, std::placeholders::_1), EvtData_New_Render_Component::sk_EventType);
	pEventMgr->VRemoveListener(
		boost::bind(&Scene::DestroyActorDelegate, this, std::placeholders::_1), EvtData_Destroy_Actor::sk_EventType);
	pEventMgr->VRemoveListener(
		boost::bind(&Scene::MoveActorDelegate, this, std::placeholders::_1), EvtData_Move_Actor::sk_EventType);
	pEventMgr->VRemoveListener(
		boost::bind(&Scene::ModifiedRenderComponentDelegate, this, std::placeholders::_1), EvtData_Modified_Render_Component::sk_EventType);
}

HRESULT Scene::OnUpdate(uint32_t deltaMs)
{
	if (m_pRootNode != nullptr)
	{
		static uint32_t lastTime = GetTickCount();
		uint32_t elapsedTime = 0;
		uint32_t currentTime = GetTickCount();
		elapsedTime = currentTime - lastTime;
		lastTime = currentTime;

		return m_pRootNode->VOnUpdate(this, elapsedTime);
	}

	return S_OK;
}

HRESULT Scene::OnRender()
{
	if (m_pRootNode != nullptr && m_pCamera != nullptr)
	{
		if (m_pRootNode->VPreRender(this) == S_OK)
		{
			m_pRootNode->VRender(this);
			m_pRootNode->VRenderChildren(this);
			m_pRootNode->VPostRender(this);
		}
	}

	return S_OK;
}

HRESULT Scene::OnRestore()
{
	if (!m_pRootNode)
		return S_OK;

	HRESULT hr;
	V_RETURN(m_pRenderer->VOnRestore());

	return m_pRootNode->VOnRestore(this);
}

HRESULT Scene::OnLostDevice()
{
	if (m_pRootNode != nullptr)
	{
		return m_pRootNode->VOnLostDevice(this);
	}
	return S_OK;
}

shared_ptr<ISceneNode> Scene::FindActor(ActorId actorId)
{
	auto actor = m_ActorMap.find(actorId);
	if (actor != m_ActorMap.end())
	{
		return actor->second;
	}
	else
	{
		return shared_ptr<ISceneNode>();
	}
}

bool Scene::AddChild(ActorId actorId, shared_ptr<ISceneNode> pChild)
{
	if (actorId != INVALID_ACTOR_ID)
	{
		m_ActorMap[actorId] = pChild;
	}

	return m_pRootNode->VAddChild(pChild);
}

bool Scene::RemoveChild(ActorId actorId)
{
	if (actorId != INVALID_ACTOR_ID)
	{
		m_ActorMap.erase(actorId);
		return m_pRootNode->VRemoveChild(actorId);
	}

	return false;
}

void Scene::NewRenderComponentDelegate(IEventDataPtr pEventData)
{

}

void Scene::ModifiedRenderComponentDelegate(IEventDataPtr pEventData)
{

}

void Scene::DestroyActorDelegate(IEventDataPtr pEventData)
{

}

void Scene::MoveActorDelegate(IEventDataPtr pEventData)
{

}
