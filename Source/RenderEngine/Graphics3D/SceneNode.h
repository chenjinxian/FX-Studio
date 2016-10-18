#pragma once
#include "../RenderEngineBase.h"
#include "../RenderEngineInterface.h"
#include "d3dx11effect.h"

class SceneNode;
class Scene;
class RayCast;
class MovementController;
class IResourceExtraData;
class ActorComponent;
class BaseRenderComponent;

typedef BaseRenderComponent* WeakBaseRenderComponentPtr;

class SceneNodeProperties
{
	friend class SceneNode;

public:
	SceneNodeProperties() : m_ActorId(INVALID_ACTOR_ID), m_RenderPass(RenderPass_0) {}
	~SceneNodeProperties() {}

	ActorId GetActorId() const { return m_ActorId; }
	const std::string& GetActorName() const { return m_ActorName; }
	RenderPass GetRenderPass() const { return m_RenderPass; }

protected:
	ActorId m_ActorId;
	std::string m_ActorName;
	RenderPass m_RenderPass;
};

typedef std::vector<shared_ptr<ISceneNode> > SceneNodeList;

class SceneNode : public ISceneNode
{
	friend class Scene;

public:
	SceneNode(ActorId actorId, WeakBaseRenderComponentPtr renderComponent, RenderPass renderPass);
	virtual ~SceneNode();

	virtual const SceneNodeProperties* VGet() const override { return &m_Properties; }

	virtual HRESULT VOnUpdate(Scene* pScene, float fElapsedTime) override;
	virtual HRESULT VOnRestore(Scene* pScene) override;
	virtual HRESULT VOnLostDevice(Scene *pScene);

	virtual HRESULT VPreRender(Scene* pScene) override;
	virtual HRESULT VRender(Scene* pScene) override;
	virtual HRESULT VRenderChildren(Scene* pScene) override;
	virtual HRESULT VPostRender(Scene* pScene) override;
	virtual bool VIsVisible(Scene* pScene) const override;

	virtual bool VAddChild(shared_ptr<ISceneNode> child) override;
	virtual bool VRemoveChild(ActorId actorId) override;

protected:
	SceneNodeList m_Children;
	SceneNode* m_pParent;
	SceneNodeProperties m_Properties;
	WeakBaseRenderComponentPtr m_pRenderComponent;
};

class RootNode : public SceneNode
{
public:
	RootNode();
	~RootNode();

	virtual HRESULT VRenderChildren(Scene* pScene);
	virtual bool VAddChild(shared_ptr<ISceneNode> child);
	virtual bool VRemoveChild(ActorId actorId);
	virtual bool VIsVisible(Scene* pScene) const { return true; }
};

class CameraNode : public SceneNode
{
public:
	CameraNode();
	~CameraNode();

	virtual HRESULT VOnUpdate(Scene* pScene, float fElapsedTime);
	virtual HRESULT VRender(Scene* pScene);
	virtual HRESULT VOnRestore(Scene* pScene);
	virtual bool VIsVisible(Scene* pScene) const { return true; }

	Matrix GetViewMatrix() { return m_ModelViewer.GetViewMatrix(); }
	Matrix GetProjectMatrix() { return m_ModelViewer.GetProjMatrix(); }

private:
	CModelViewerCamera m_ModelViewer;
};

class GridNode : public SceneNode
{
public:
	GridNode(ActorId actorId, WeakBaseRenderComponentPtr renderComponent);
	virtual ~GridNode();

	virtual HRESULT VOnRestore(Scene* pScene);
	virtual HRESULT VRender(Scene* pScene);
	virtual HRESULT VOnUpdate(Scene* pScene, float fElapsedTime);

private:
	XMMATRIX                            g_World;
	float                               g_fModelWaviness = 0.0f;
	bool                                g_bSpinning;
	ID3DX11Effect*                      g_pEffect;
	ID3D11InputLayout*                  g_pVertexLayout;
	ID3DX11EffectTechnique*             g_pTechnique;
	CDXUTSDKMesh                        g_Mesh;
	ID3DX11EffectShaderResourceVariable* g_ptxDiffuseVariable;
	ID3DX11EffectMatrixVariable*        g_pWorldVariable;
	ID3DX11EffectMatrixVariable*        g_pViewVariable;
	ID3DX11EffectMatrixVariable*        g_pProjectionVariable;
	ID3DX11EffectScalarVariable*        g_pWavinessVariable;
	ID3DX11EffectScalarVariable*        g_pTimeVariable;
};