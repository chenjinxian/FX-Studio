#include "LightNode.h"
#include "../Actors/RenderComponent.h"

LightNode::LightNode(ActorId actorId, WeakBaseRenderComponentPtr renderComponent, RenderPass renderPass, const Matrix& mat)
	:SceneNode(actorId, renderComponent, renderPass)
{

}

HRESULT DirectionalLightNode::VOnRestore(Scene* pScene)
{
	return S_OK;
}

HRESULT DirectionalLightNode::VOnUpdate(Scene* pScene, double fTime, float fElapsedTime)
{
	return S_OK;
}
