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

enum AlphaType
{
	AlphaType_Opaque,
	AlphaType_Texture,
	AlphaType_Material,
	AlphaType_Vertex
};

class SceneNodeProperties
{
	friend class SceneNode;

public:
	SceneNodeProperties();
	~SceneNodeProperties() {}

	ActorId GetActorId() const { return m_ActorId; }
	const std::string& GetActorName() const { return m_ActorName; }
	const Vector3& GetPosition() const { return m_Position; }
	const Vector3& GetDirection() const { return m_Direction; }
	const Vector3& GetUp() const { return m_Up; }
	const Vector3& GetRight() const { return m_Right; }
	const Matrix& GetRotation() const { return m_Rotation; }
	RenderPass GetRenderPass() const { return m_RenderPass; }

protected:
	ActorId m_ActorId;
	std::string m_ActorName;
	Vector3 m_Position;
	Vector3 m_Direction;
	Vector3 m_Up;
	Vector3 m_Right;
	Matrix m_Rotation;
	RenderPass m_RenderPass;
};

typedef std::vector<shared_ptr<ISceneNode> > SceneNodeList;

class SceneNode : public ISceneNode, public boost::noncopyable
{
	friend class Scene;

public:
	SceneNode(ActorId actorId, WeakBaseRenderComponentPtr renderComponent, RenderPass renderPass,
		const Matrix& orientation = Matrix::Identity, const Matrix& rotation = Matrix::Identity);
	virtual ~SceneNode();

	virtual const SceneNodeProperties* VGet() const override { return &m_Properties; }
	virtual void VSetTransform(const Vector3& position, const Matrix& rotation);

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

	void SetRadius(float radius) { }

protected:
	SceneNodeList m_Children;
	SceneNode* m_pParent;
	WeakBaseRenderComponentPtr m_pRenderComponent;
	SceneNodeProperties m_Properties;
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

class GridNode : public SceneNode
{
public:
	GridNode(ActorId actorId, WeakBaseRenderComponentPtr renderComponent, const Matrix& worldMatrix);
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
	ModelNode(ActorId actorId, WeakBaseRenderComponentPtr renderComponent, RenderPass renderPass, const Matrix& worldMatrix);

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
