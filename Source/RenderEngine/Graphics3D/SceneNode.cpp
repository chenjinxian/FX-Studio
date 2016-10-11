#include "SceneNode.h"
#include "../Actors/RenderComponent.h"

SceneNode::SceneNode(ActorId actorId, WeakBaseRenderComponentPtr renderComponent, RenderPass renderPass)
	: m_pParent(nullptr), m_pRenderComponent(renderComponent)
{
	m_Properties.m_ActorId = actorId;
	m_Properties.m_ActorName = (renderComponent != nullptr) ? renderComponent->VGetName() : "SceneNode";
}

SceneNode::~SceneNode()
{
}

HRESULT SceneNode::VOnUpdate(Scene* pScene, uint32_t elapsedMs)
{
	return S_OK;
}

HRESULT SceneNode::VOnRestore(Scene* pScene)
{
	return S_OK;
}

HRESULT SceneNode::VOnLostDevice(Scene *pScene)
{
	return S_OK;
}

HRESULT SceneNode::VPreRender(Scene* pScene)
{
	return S_OK;
}

HRESULT SceneNode::VRender(Scene* pScene)
{
	return S_OK;
}

HRESULT SceneNode::VRenderChildren(Scene* pScene)
{
	return S_OK;
}

HRESULT SceneNode::VPostRender(Scene* pScene)
{
	return S_OK;
}

bool SceneNode::VIsVisible(Scene* pScene) const
{
	return true;
}

bool SceneNode::VAddChild(shared_ptr<ISceneNode> child)
{
	return true;
}

bool SceneNode::VRemoveChild(ActorId actorId)
{
	return true;
}

RootNode::RootNode() : SceneNode(INVALID_ACTOR_ID, WeakBaseRenderComponentPtr(), RenderPass_0)
{

}

RootNode::~RootNode()
{

}

HRESULT RootNode::VRenderChildren(Scene* pScene)
{
	return S_OK;
}

bool RootNode::VAddChild(shared_ptr<ISceneNode> child)
{
	return true;
}

bool RootNode::VRemoveChild(ActorId actorId)
{
	return true;
}

CameraNode::CameraNode() : SceneNode(INVALID_ACTOR_ID, WeakBaseRenderComponentPtr(), RenderPass_0)
{

}

CameraNode::~CameraNode()
{

}

HRESULT CameraNode::VRender(Scene* pScene)
{
	return S_OK;
}

HRESULT CameraNode::VOnRestore(Scene* pScene)
{
	return S_OK;
}
