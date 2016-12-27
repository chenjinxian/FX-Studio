#pragma once
#include "../TinyEngineBase.h"
#include "../TinyEngineInterface.h"
#include "d3dx11effect.h"

class SceneNode;
class Scene;
class RayCast;
class MovementController;
class IResourceExtraData;
class ActorComponent;
class BaseRenderComponent;
class Effect;
class Pass;

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
	Vector3 GetPosition() const { return m_worldMatrix.Translation(); }
	const Matrix& GetWorldMatrix() const { return m_worldMatrix; }
	RenderPass GetRenderPass() const { return m_RenderPass; }

protected:
	ActorId m_ActorId;
	std::string m_ActorName;
	Matrix m_worldMatrix;
	RenderPass m_RenderPass;
};

typedef std::vector<shared_ptr<ISceneNode> > SceneNodeList;

class SceneNode : public ISceneNode, public boost::noncopyable
{
	friend class Scene;

public:
	SceneNode(ActorId actorId, WeakBaseRenderComponentPtr renderComponent,
		RenderPass renderPass, const Matrix& worldMatrix = Matrix::Identity);
	virtual ~SceneNode();

	virtual const SceneNodeProperties* VGet() const override { return &m_Properties; }
	virtual void VSetTransform(const Matrix& worldMatrix);

	virtual HRESULT VOnUpdate(Scene* pScene, const GameTime& gameTime) override;
	virtual HRESULT VOnInitSceneNode(Scene* pScene) override;
	virtual HRESULT VOnDeleteSceneNode(Scene *pScene) override;

	virtual HRESULT VPreRender(Scene* pScene) override;
	virtual HRESULT VRender(Scene* pScene, const GameTime& gameTime) override;
	virtual HRESULT VRenderChildren(Scene* pScene, const GameTime& gameTime) override;
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

	virtual HRESULT VRenderChildren(Scene* pScene, const GameTime& gameTime);
	virtual bool VAddChild(shared_ptr<ISceneNode> child);
	virtual bool VRemoveChild(ActorId actorId);
	virtual bool VIsVisible(Scene* pScene) const { return true; }
};

class GridNode : public SceneNode
{
public:
	GridNode(ActorId actorId, WeakBaseRenderComponentPtr renderComponent);
	virtual ~GridNode();

	virtual HRESULT VOnInitSceneNode(Scene* pScene) override;
	virtual HRESULT VOnDeleteSceneNode(Scene *pScene) override;
	virtual HRESULT VRender(Scene* pScene, const GameTime& gameTime) override;
	virtual HRESULT VOnUpdate(Scene* pScene, const GameTime& gameTime) override;

private:
	typedef struct _VertexPositionTexture
	{
		Vector4 position;
		Vector2 texture;

		_VertexPositionTexture() { }

		_VertexPositionTexture(const Vector4& position, const Vector2& texture)
			: position(position), texture(texture) { }
	} VertexPositionTexture;

	void InitGridVertex();

	Effect* m_pEffect;
	Pass* m_pCurrentPass;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	uint32_t m_VertexCount;
	uint32_t m_IndexCount;

	std::string m_TextureName;
	Vector2 m_GridSize;
	float m_TicksInterval;
};

class GeometryNode : public SceneNode
{
public:
	GeometryNode(ActorType actorType, ActorId actorId, WeakBaseRenderComponentPtr renderComponent, RenderPass renderPass, const Matrix& worldMatrix);
	~GeometryNode();

	virtual HRESULT VOnInitSceneNode(Scene* pScene) override;
	virtual HRESULT VOnDeleteSceneNode(Scene *pScene) override;
	virtual HRESULT VOnUpdate(Scene* pScene, const GameTime& gameTime) override;
	virtual HRESULT VRender(Scene* pScene, const GameTime& gameTime) override;

private:
	void CreateSphere();
	void CreateCylinder();
	void CreateTeapot();

	Effect* m_pEffect;
	Pass* m_pCurrentPass;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	uint32_t m_IndexCount;

	std::string m_TextureName;
	std::string m_EffectName;
	std::string m_CurrentTechnique;
	std::string m_CurrentPass;

	static const std::string m_Sphere;
	static const std::string m_Cylinder;
	static const std::string m_Teapot;
};

class ModelNode : public SceneNode
{
public:
	ModelNode(ActorId actorId, WeakBaseRenderComponentPtr renderComponent, RenderPass renderPass, const Matrix& worldMatrix);
	~ModelNode();

	virtual HRESULT VOnInitSceneNode(Scene* pScene) override;
	virtual HRESULT VOnDeleteSceneNode(Scene *pScene) override;
	virtual HRESULT VOnUpdate(Scene* pScene, const GameTime& gameTime) override;
	virtual HRESULT VRender(Scene* pScene, const GameTime& gameTime) override;

private:
	Effect* m_pEffect;
	Pass* m_pCurrentPass;
	std::vector<ID3D11Buffer*> m_pVertexBuffers;
	std::vector<ID3D11Buffer*> m_pIndexBuffers;
	std::vector<uint32_t> m_IndexCounts;
	std::vector<ID3D11ShaderResourceView*> m_pTextures;
	Matrix m_WorldMatrix;

	std::string m_ModelName;
	std::vector<std::string> m_TextureNames;
	std::string m_EffectName;
	std::string m_CurrentTechnique;
	std::string m_CurrentPass;
};

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
