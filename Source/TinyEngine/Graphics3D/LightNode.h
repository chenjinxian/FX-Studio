#pragma once
#include "Scene.h"
#include "SceneNode.h"

class LightNode : public SceneNode
{
public:
	LightNode(ActorId actorId, WeakBaseRenderComponentPtr renderComponent, RenderPass renderPass, const Matrix& mat);
	virtual ~LightNode();

	virtual HRESULT VOnInitSceneNode(Scene* pScene) override;
	virtual HRESULT VOnDeleteSceneNode(Scene *pScene) override;
	virtual HRESULT VRender(Scene* pScene, const GameTime& gameTime) override;

protected:
	Effect* m_pEffect;
	class BasicMaterial* m_pMaterial;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	uint64_t m_IndexCount;
	Matrix m_WorldMatrix;
	Matrix m_ScaleMatrix;

	std::string m_ModelName;
};

class DirectionalLightNode : virtual public LightNode
{
public:
	DirectionalLightNode(ActorId actorId, WeakBaseRenderComponentPtr renderComponent, RenderPass renderPass, const Matrix& mat);
	virtual ~DirectionalLightNode();

	virtual HRESULT VOnUpdate(Scene* pScene, const GameTime& gameTime) override;
};

class PointLightNode : virtual public LightNode
{
public:
	PointLightNode(ActorId actorId, WeakBaseRenderComponentPtr renderComponent, RenderPass renderPass, const Matrix& mat);
	virtual ~PointLightNode();

	virtual HRESULT VOnUpdate(Scene* pScene, const GameTime& gameTime) override;
};

class SpotLightNode : public DirectionalLightNode, public PointLightNode
{
public:
	SpotLightNode(ActorId actorId, WeakBaseRenderComponentPtr renderComponent, RenderPass renderPass, const Matrix& mat);
	virtual ~SpotLightNode();

	virtual HRESULT VOnUpdate(Scene* pScene, const GameTime& gameTime) override;
};


// class LightManager
// {
// 	friend class Scene;
// 
// protected:
// 	Lights					m_Lights;
// 	Vec4					m_vLightDir[MAXIMUM_LIGHTS_SUPPORTED];
// 	Color					m_vLightDiffuse[MAXIMUM_LIGHTS_SUPPORTED];
// 	Vec4					m_vLightAmbient;
// public:
// 	void CalcLighting(Scene *pScene);
// 	void CalcLighting(ConstantBuffer_Lighting* pLighting, SceneNode *pNode);
// 	int GetLightCount(const SceneNode *node) { return m_Lights.size(); }
// 	const Vec4 *GetLightAmbient(const SceneNode *node) { return &m_vLightAmbient; }
// 	const Vec4 *GetLightDirection(const SceneNode *node) { return m_vLightDir; }
// 	const Color *GetLightDiffuse(const SceneNode *node) { return m_vLightDiffuse; }
// };
