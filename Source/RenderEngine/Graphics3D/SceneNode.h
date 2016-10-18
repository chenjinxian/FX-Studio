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

	virtual HRESULT VOnUpdate(Scene* pScene, double fTime, float fElapsedTime) override;
	virtual HRESULT VOnRestore(Scene* pScene) override;
	virtual HRESULT VOnLostDevice(Scene *pScene) override;
	virtual LRESULT CALLBACK VOnMsgProc(Scene *pScene, AppMsg msg) override;

	virtual HRESULT VPreRender(Scene* pScene) override;
	virtual HRESULT VRender(Scene* pScene, double fTime, float fElapsedTime) override;
	virtual HRESULT VRenderChildren(Scene* pScene, double fTime, float fElapsedTime) override;
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

	virtual HRESULT VRenderChildren(Scene* pScene, double fTime, float fElapsedTime);
	virtual bool VAddChild(shared_ptr<ISceneNode> child);
	virtual bool VRemoveChild(ActorId actorId);
	virtual bool VIsVisible(Scene* pScene) const { return true; }
};

class CameraNode : public SceneNode
{
public:
	CameraNode();
	~CameraNode();

	virtual HRESULT VOnUpdate(Scene* pScene, double fTime, float fElapsedTime);
	virtual HRESULT VRender(Scene* pScene, double fTime, float fElapsedTime);
	virtual HRESULT VOnRestore(Scene* pScene);
	virtual bool VIsVisible(Scene* pScene) const { return true; }
	virtual LRESULT CALLBACK VOnMsgProc(Scene *pScene, AppMsg msg);

	Matrix GetViewMatrix() { return m_ModelViewer.GetViewMatrix(); }
	Matrix GetProjectMatrix() { return m_ModelViewer.GetProjMatrix(); }

private:
	CModelViewerCamera m_ModelViewer;
};

typedef struct _VertexPositionColor
{
	XMFLOAT4 Position;
	XMFLOAT4 Color;

	_VertexPositionColor() { }

	_VertexPositionColor(const XMFLOAT4& position, const XMFLOAT4& color)
		: Position(position), Color(color) { }
} VertexPositionColor;

class GridNode : public SceneNode
{
public:
	GridNode(ActorId actorId, WeakBaseRenderComponentPtr renderComponent);
	virtual ~GridNode();

	virtual HRESULT VOnRestore(Scene* pScene);
	virtual HRESULT VRender(Scene* pScene, double fTime, float fElapsedTime);
	virtual HRESULT VOnUpdate(Scene* pScene, double fTime, float fElapsedTime);

private:
	ID3DX11Effect* mEffect;
	ID3DX11EffectTechnique* mTechnique;
	ID3DX11EffectPass* mPass;
	ID3DX11EffectMatrixVariable* mWvpVariable;

	ID3D11InputLayout* mInputLayout;
	ID3D11Buffer* mVertexBuffer;

	Vector3 mPosition;
	Matrix mWorldMatrix;
	UINT mSize;
	UINT mScale;
	Color mColor;
};