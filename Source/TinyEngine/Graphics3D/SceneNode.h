#pragma once
#include "../TinyEngineBase.h"
#include "../TinyEngineInterface.h"
#include "d3dx11effect.h"
#include "GeometricPrimitive.h"

class SceneNode;
class Scene;
class RayCast;
class MovementController;
class IResourceExtraData;
class ActorComponent;
class BaseRenderComponent;
class Effect;
class Pass;
class Mesh;
class Model;

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
	const BoundingBox& GetBoundingBox() const { return m_AABox; }
	const BoundingSphere& GetBoundingSphere() const { return m_Sphere; }
	RenderPass GetRenderPass() const { return m_RenderPass; }

protected:
	ActorId m_ActorId;
	std::string m_ActorName;
	Matrix m_worldMatrix;
	BoundingBox m_AABox;
	BoundingSphere m_Sphere;
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

	virtual void VPick(Scene* pScene, int cursorX, int cursorY) override;

protected:
	void SetBoundingBox(const BoundingBox& aabb);
	void SetBoundingSphere(const BoundingSphere& sphere);

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
	virtual void VPick(Scene* pScene, int cursorX, int cursorY) override;
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
	typedef struct _VertexPositionColor
	{
		Vector4 position;
		Color color;

		_VertexPositionColor() { }

		_VertexPositionColor(const Vector4& position, const Color& color)
			: position(position), color(color) { }
	} VertexPositionColor;

	void InitGridVertex();

	Effect* m_pEffect;
	Pass* m_pCurrentPass;
	ID3D11Buffer* m_pVertexBuffer;
	uint32_t m_VertexCount;

	uint32_t m_GridSize;
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
	virtual void VPick(Scene* pScene, int cursorX, int cursorY) override;

private:
	void CreateCube();
	void CreateSphere();
	void CreateTorus();
	void CreateTeapot();
	void CreatePlane();

	Effect* m_pEffect;
	Pass* m_pCurrentPass;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	uint32_t m_IndexCount;
	std::unique_ptr<Mesh> m_Mesh;

	std::string m_MaterialName;

	static const std::string m_Cube;
	static const std::string m_Sphere;
	static const std::string m_Torus;
	static const std::string m_Teapot;
	static const std::string m_Plane;
};
