#pragma once
#include "D3DRenderer.h"
#include "Scene.h"
#include "SceneNode.h"

class LightNode : public SceneNode
{
public:
	LightNode(ActorId actorId, WeakBaseRenderComponentPtr renderComponent, RenderPass renderPass, const Matrix& mat);

protected:
	std::string m_ModelName;
};

class DirectionalLightNode : public LightNode
{
public:
	DirectionalLightNode(ActorId actorId, WeakBaseRenderComponentPtr renderComponent, RenderPass renderPass, const Matrix& mat)
		: LightNode(actorId, renderComponent, renderPass, mat) { }

	virtual HRESULT VOnRestore(Scene* pScene) override;
	virtual HRESULT VOnUpdate(Scene* pScene, double fTime, float fElapsedTime) override;
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
