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

class SceneNode : public ISceneNode, public boost::noncopyable
{
	friend class Scene;

public:
	SceneNode(ActorId actorId, WeakBaseRenderComponentPtr renderComponent, RenderPass renderPass);
	virtual ~SceneNode();

	virtual const SceneNodeProperties* VGet() const override { return &m_Properties; }

	virtual HRESULT VOnUpdate(Scene* pScene, double fTime, float fElapsedTime) override;
	virtual HRESULT VOnRestore(Scene* pScene) override;
	virtual HRESULT VOnDestoryDevice(Scene *pScene) override;

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
	virtual HRESULT VOnDestoryDevice(Scene *pScene);

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

class Effect;
class TextureMappingMaterial;
class ModelNode : public SceneNode
{
public:
	ModelNode(ActorId actorId, WeakBaseRenderComponentPtr renderComponent, RenderPass renderPass, const Matrix& mat);

	~ModelNode();

	virtual HRESULT VOnRestore(Scene *pScene);
	virtual HRESULT VOnDestoryDevice(Scene* pScene) override;
	virtual HRESULT VOnUpdate(Scene* pScene, double fTime, float fElapsedTime) override;
	virtual HRESULT VRender(Scene* pScene, double fTime, float fElapsedTime) override;

private:
	Effect* m_pEffect;
	TextureMappingMaterial* m_pTextureMappingMaterial;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	UINT m_IndexCount;
	Matrix m_WorldMatrix;

	std::string m_ModelName;
	std::string m_TextureName;
	std::string m_EffectName;
};
