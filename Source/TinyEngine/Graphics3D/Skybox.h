#pragma once
#include "Material.h"
#include "SceneNode.h"

class Scene;

class SkyboxNode : public SceneNode
{
public:
	SkyboxNode(ActorId actorId, WeakBaseRenderComponentPtr renderComponent, RenderPass renderPass);
	virtual ~SkyboxNode();

	virtual bool VIsVisible(Scene* pScene);
	virtual HRESULT VPreRender(Scene* pScene) override;

	virtual HRESULT VOnInitSceneNode(Scene* pScene) override;
	virtual HRESULT VOnDeleteSceneNode(Scene *pScene) override;
	virtual HRESULT VRender(Scene* pScene, const GameTime& gameTime) override;
	virtual HRESULT VOnUpdate(Scene* pScene, const GameTime& gameTime) override;

private:
	Effect* m_pEffect;
	Pass* m_pCurrentPass;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	uint32_t m_IndexCount;
	Matrix m_ScaleMatrix;

	std::string m_TextureName;
};
