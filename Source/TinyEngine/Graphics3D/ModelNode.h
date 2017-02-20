#pragma once
#include "SceneNode.h"

class Scene;

class ModelNode : public SceneNode
{
public:
	ModelNode(ActorId actorId, WeakBaseRenderComponentPtr renderComponent, RenderPass renderPass, const Matrix& worldMatrix);
	~ModelNode();

	virtual HRESULT VOnInitSceneNode(Scene* pScene) override;
	virtual HRESULT VOnDeleteSceneNode(Scene *pScene) override;
	virtual HRESULT VOnUpdate(Scene* pScene, const GameTime& gameTime) override;
	virtual HRESULT VRender(Scene* pScene, const GameTime& gameTime) override;
	virtual void VPick(Scene* pScene, int cursorX, int cursorY) override;

private:
	std::vector<Effect*> m_pEffects;
	std::vector<Pass*> m_pPasses;
	std::vector<ID3D11Buffer*> m_pVertexBuffers;
	std::vector<ID3D11Buffer*> m_pIndexBuffers;
	std::vector<uint32_t> m_IndexCounts;
	std::unique_ptr<Model> m_pModel;

	std::string m_ModelName;
	std::vector<std::string> m_MaterialNames;
};
