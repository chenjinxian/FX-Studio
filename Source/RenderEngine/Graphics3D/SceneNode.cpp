#include "SceneNode.h"
#include "Scene.h"
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
	m_Children.reserve(RenderPass_Last);

	shared_ptr<SceneNode> staticGroup(GCC_NEW SceneNode(INVALID_ACTOR_ID, WeakBaseRenderComponentPtr(), RenderPass_Static));
	m_Children.push_back(staticGroup);

	shared_ptr<SceneNode> actorGroup(GCC_NEW SceneNode(INVALID_ACTOR_ID, WeakBaseRenderComponentPtr(), RenderPass_Actor));
	m_Children.push_back(actorGroup);

	shared_ptr<SceneNode> skyGroup(GCC_NEW SceneNode(INVALID_ACTOR_ID, WeakBaseRenderComponentPtr(), RenderPass_Sky));
	m_Children.push_back(skyGroup);

	shared_ptr<SceneNode> invisibleGroup(GCC_NEW SceneNode(INVALID_ACTOR_ID, WeakBaseRenderComponentPtr(), RenderPass_NotRendered));
	m_Children.push_back(invisibleGroup);
}

RootNode::~RootNode()
{

}

HRESULT RootNode::VRenderChildren(Scene* pScene)
{
	for (int pass = RenderPass_0; pass < RenderPass_Last; ++pass)
	{
		switch (pass)
		{
		case RenderPass_Static:
		case RenderPass_Actor:
			m_Children[pass]->VRenderChildren(pScene);
			break;

		case RenderPass_Sky:
		{
// 			shared_ptr<IRenderState> skyPass = pScene->GetRenderder()->
			m_Children[pass]->VRenderChildren(pScene);
			break;
		}
		}
	}

	return S_OK;
}

bool RootNode::VAddChild(shared_ptr<ISceneNode> child)
{
	RenderPass pass = child->VGet()->GetRenderPass();
	if ((unsigned)pass >= m_Children.size() || !m_Children[pass])
	{
		GCC_ASSERT(0 && _T("There is no such render pass"));
		return false;
	}

	return m_Children[pass]->VAddChild(child);
}

bool RootNode::VRemoveChild(ActorId actorId)
{
	bool anythingRemoved = false;
	for (int i = RenderPass_0; i < RenderPass_Last; ++i)
	{
		if (m_Children[i]->VRemoveChild(actorId))
		{
			anythingRemoved = true;
		}
	}
	return anythingRemoved;
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

GridNode::GridNode(ActorId actorId, WeakBaseRenderComponentPtr renderComponent)
	: SceneNode(actorId, renderComponent, RenderPass_0)
{

}

GridNode::~GridNode()
{

}

HRESULT GridNode::VOnRestore(Scene* pScene)
{
	HRESULT hr;

	V_RETURN(SceneNode::VOnRestore(pScene));

	return hr;
}

HRESULT GridNode::VRender(Scene* pScene)
{
	return S_OK;
}

HRESULT GridNode::VOnUpdate(Scene* pScene, uint32_t elapsedMs)
{
	return S_OK;
}
