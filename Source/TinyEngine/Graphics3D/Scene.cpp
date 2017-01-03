#include "Scene.h"
#include "SceneNode.h"
#include "CameraNode.h"
#include "../EventManager/EventManager.h"
#include "../EventManager/Events.h"
#include "../AppFramework/BaseGameApp.h"

Scene::Scene(shared_ptr<IRenderer> pRenderer)
	: m_pRenderer(pRenderer),
	m_pRootNode(nullptr),
	m_pCamera(nullptr),
	m_pDebugNode(nullptr),
	m_MatrixStack(),
	m_ActorMap(),
	m_PickedActor(INVALID_ACTOR_ID),
	m_PickDistance(FLT_MAX)
{
	m_pRootNode.reset(DEBUG_NEW RootNode());
	m_pDebugNode.reset(DEBUG_NEW DebugAssistNode());
	AddChild(INVALID_ACTOR_ID, m_pDebugNode);

	IEventManager* pEventMgr = IEventManager::Get();
	pEventMgr->VAddListener(
		boost::bind(&Scene::NewRenderComponentDelegate, this, _1), EvtData_New_Render_Component::sk_EventType);
	pEventMgr->VAddListener(
		boost::bind(&Scene::DestroyActorDelegate, this, _1), EvtData_Destroy_Actor::sk_EventType);
	pEventMgr->VAddListener(
		boost::bind(&Scene::MoveActorDelegate, this, _1), EvtData_Move_Actor::sk_EventType);
	pEventMgr->VAddListener(
		boost::bind(&Scene::ModifiedRenderComponentDelegate, this, _1), EvtData_Modified_Render_Component::sk_EventType);
}

Scene::~Scene()
{
	IEventManager* pEventMgr = IEventManager::Get();
	pEventMgr->VRemoveListener(
		boost::bind(&Scene::NewRenderComponentDelegate, this, _1), EvtData_New_Render_Component::sk_EventType);
	pEventMgr->VRemoveListener(
		boost::bind(&Scene::DestroyActorDelegate, this, _1), EvtData_Destroy_Actor::sk_EventType);
	pEventMgr->VRemoveListener(
		boost::bind(&Scene::MoveActorDelegate, this, _1), EvtData_Move_Actor::sk_EventType);
	pEventMgr->VRemoveListener(
		boost::bind(&Scene::ModifiedRenderComponentDelegate, this, _1), EvtData_Modified_Render_Component::sk_EventType);
}

HRESULT Scene::OnUpdate(const GameTime& gameTime)
{
	if (m_pRootNode != nullptr)
	{
		return m_pRootNode->VOnUpdate(this, gameTime);
	}

	return S_OK;
}

HRESULT Scene::OnRender(const GameTime& gameTime)
{
	if (m_pRootNode != nullptr && m_pCamera != nullptr)
	{
		m_pCamera->SetViewTransform(this);

		if (m_pRootNode->VPreRender(this) == S_OK)
		{
			m_pRootNode->VRender(this, gameTime);
			m_pRootNode->VRenderChildren(this, gameTime);
			m_pRootNode->VPostRender(this);
		}

		RenderAlphaPass();
	}

	return S_OK;
}

HRESULT Scene::OnInitScene()
{
	if (!m_pRootNode)
		return S_OK;

// 	HRESULT hr;
// 	V_RETURN(m_pRenderer->VOnRestore());

	return m_pRootNode->VOnInitSceneNode(this);
}

HRESULT Scene::OnDeleteScene()
{
	if (m_pRootNode != nullptr)
	{
		return m_pRootNode->VOnDeleteSceneNode(this);
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
	shared_ptr<EvtData_New_Render_Component> pCastEventData = static_pointer_cast<EvtData_New_Render_Component>(pEventData);

	ActorId actorId = pCastEventData->GetActorId();
	shared_ptr<SceneNode> pSceneNode(pCastEventData->GetSceneNode());
	if (pSceneNode != nullptr)
	{
		if (FAILED(pSceneNode->VOnInitSceneNode(this)))
		{
// 			std::string error = "Failed to restore scene node to the scene for actorid " + boost::lex(actorId);
// 			DEBUG_ERROR(error);
			return;
		}

		AddChild(actorId, pSceneNode);
	}
}

void Scene::ModifiedRenderComponentDelegate(IEventDataPtr pEventData)
{

}

void Scene::DestroyActorDelegate(IEventDataPtr pEventData)
{
	shared_ptr<EvtData_Destroy_Actor> pCastEventData = static_pointer_cast<EvtData_Destroy_Actor>(pEventData);
	RemoveChild(pCastEventData->GetActorId());
}

void Scene::MoveActorDelegate(IEventDataPtr pEventData)
{
	shared_ptr<EvtData_Move_Actor> pCastEventData = static_pointer_cast<EvtData_Move_Actor>(pEventData);

	ActorId id = pCastEventData->GetActorId();
	Matrix transform = pCastEventData->GetMatrix();

	shared_ptr<ISceneNode> pNode = FindActor(id);
	if (pNode)
	{
// 		pNode->VSetTransform(transform);
	}
}

void Scene::SetTransformType(int type)
{
	if (m_pDebugNode != nullptr)
	{
		m_pDebugNode->SetTransformType((DebugAssistNode::TransformType)type);
	}
}

ActorId Scene::PickActor(int cursorX, int cursorY)
{
	m_PickedActor = INVALID_ACTOR_ID;
	m_PickDistance = FLT_MAX;
	m_pRootNode->VPick(this, cursorX, cursorY);
	m_pDebugNode->SetVisible(m_PickedActor != INVALID_ACTOR_ID);
	return m_PickedActor;
}

void Scene::PointMove(const Vector2 &pos)
{
	if (m_pDebugNode != nullptr)
	{
		m_pDebugNode->UpdatePointer(pos);
	}
}
