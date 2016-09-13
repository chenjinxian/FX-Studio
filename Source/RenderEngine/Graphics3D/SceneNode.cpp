#include "SceneNode.h"



SceneNode::SceneNode()
{
}


SceneNode::~SceneNode()
{
}

void SceneNode::VSetTransform(const Matrix& toWorld, const Matrix& fromWorld)
{
	m_Property.m_ToWorld = toWorld;
	m_Property.m_FromWorld = fromWorld;
}

HRESULT SceneNode::VOnUpdate(Scene* pScene, DWORD const elapsedMs)
{
	return S_OK;
}

HRESULT SceneNode::VOnRestore(Scene* pScene)
{
	return S_OK;
}

HRESULT SceneNode::VOnLostDevice(Scene* pScene)
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

bool SceneNode::VAddChild(shared_ptr<ISceneNode> kid)
{
	return true;
}

bool SceneNode::VRemoveChild(ActorId id)
{
	return true;
}

bool SceneNode::VIsVisible(Scene* pScene) const
{
	return true;
}

HRESULT SceneNode::VPick(Scene* pScene, RayCast *pRayCast)
{
	return S_OK;
}
