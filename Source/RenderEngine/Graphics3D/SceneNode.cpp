#include "SceneNode.h"

SceneNode::SceneNode()
{
}


SceneNode::~SceneNode()
{
}

void SceneNode::VOnUpdate(Scene* pScene, uint32_t elapsedMs)
{

}

void SceneNode::VOnRestore(Scene* pScene)
{

}

void SceneNode::VPreRender(Scene* pScene)
{

}

void SceneNode::VRender(Scene* pScene)
{

}

void SceneNode::VRenderChildren(Scene* pScene)
{

}

void SceneNode::VPostRender(Scene* pScene)
{

}

bool SceneNode::VIsVisible(Scene* pScene) const
{
	return true;
}

void SceneNode::VAddChild(shared_ptr<ISceneNode> child)
{

}

void SceneNode::VRemoveChild(ActorId actorId)
{

}

RootNode::RootNode()
{

}

RootNode::~RootNode()
{

}

void RootNode::VRenderChildren(Scene* pScene)
{

}

void RootNode::VAddChild(shared_ptr<ISceneNode> child)
{

}

void RootNode::VRemoveChild(ActorId actorId)
{

}

CameraNode::CameraNode()
{

}

CameraNode::~CameraNode()
{

}

void CameraNode::VRender(Scene* pScene)
{

}

void CameraNode::VOnRestore(Scene* pScene)
{

}
