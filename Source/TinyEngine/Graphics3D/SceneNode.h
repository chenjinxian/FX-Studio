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
	RenderPass GetRenderPass() const { return m_RenderPass; }

protected:
	ActorId m_ActorId;
	std::string m_ActorName;
	Matrix m_worldMatrix;
	BoundingBox m_AABox;
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
	void SetBoundingBox(const std::vector<Vector3>& postions);

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
	virtual void VPick(Scene* pScene, int cursorX, int cursorY) override;

private:
	void CreateCube();
	void CreateSphere();
	void CreateCylinder();
	void CreateTeapot();
	void CreatePlane();

	Effect* m_pEffect;
	Pass* m_pCurrentPass;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	uint32_t m_IndexCount;
	std::unique_ptr<Mesh> m_Mesh;

	std::string m_TextureName;
	std::string m_EffectName;
	std::string m_CurrentTechnique;
	std::string m_CurrentPass;

	static const std::string m_Cube;
	static const std::string m_Sphere;
	static const std::string m_Cylinder;
	static const std::string m_Teapot;
	static const std::string m_Plane;
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
	virtual void VPick(Scene* pScene, int cursorX, int cursorY) {}

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

class DebugAssistNode : public SceneNode
{
public:
	DebugAssistNode();
	~DebugAssistNode();

	virtual HRESULT VOnInitSceneNode(Scene* pScene) override;
	virtual HRESULT VOnDeleteSceneNode(Scene *pScene) override;
	virtual HRESULT VOnUpdate(Scene* pScene, const GameTime& gameTime) override;
	virtual HRESULT VRender(Scene* pScene, const GameTime& gameTime) override;
	virtual bool VIsVisible(Scene* pScene) const override;

	enum TransformType
	{
		TT_None = 0,
		TT_Translation,
		TT_Rotation,
		TT_Scale
	};

	enum PickedTransform
	{
		PT_None = 0,
		PT_TranslateX,
		PT_TranslateY,
		PT_TranslateZ,
		PT_RotateX,
		PT_RotateY,
		PT_RotateZ,
		PT_ScaleX,
		PT_ScaleY,
		PT_ScaleZ
	};

	void SetTransformType(TransformType transform) { m_Transform = transform; }
	void SetVisible(bool visible) { m_IsVisible = visible; }
	void UpdatePointer(const Vector2 &pos) { m_MousePos = pos; m_PickedTransform = PT_None; }
	PickedTransform GetPickedTransform() { return m_PickedTransform; }

private:
	HRESULT RenderBoundingBox(Scene* pScene, const BoundingBox& aaBox, const Matrix& world);
	HRESULT RenderTranslateAxes(Scene* pScene, const BoundingBox& aaBox, const Matrix& world);
	HRESULT RenderRotateRings(Scene* pScene, const BoundingBox& aaBox, const Matrix& world);
	HRESULT RenderScaleAxes(Scene* pScene, const BoundingBox& aaBox, const Matrix& world);

	void CreateAABox(std::vector<Vector3>& vertices, std::vector<uint16_t>& indices);
	void AddVertex(std::vector<Vector3>& vertices, const std::vector<struct VertexPositionNormalTexture>& inputVertices);
	void CreateGeometryBuffers();

	bool IsXAxisPicked(Scene* pScene, const Matrix& world);

	Effect* m_pEffect;
	Pass* m_pCurrentPass;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;

	int32_t m_AABoxVertexOffset;
	int32_t m_CylinderVertexOffset;
	int32_t m_ConeVertexOffset;
	int32_t m_TorusVertexOffset;
	int32_t m_CubeVertexOffset;

	uint32_t m_AABoxIndexOffset;
	uint32_t m_CylinderIndexOffset;
	uint32_t m_ConeIndexOffset;
	uint32_t m_TorusIndexOffset;
	uint32_t m_CubeIndexOffset;

	uint32_t m_AABoxIndexCount;
	uint32_t m_CylinderIndexCount;
	uint32_t m_ConeIndexCount;
	uint32_t m_TorusIndexCount;
	uint32_t m_CubeIndexCount;

	TransformType m_Transform;
	Vector2 m_MousePos;
	PickedTransform m_PickedTransform;
	bool m_IsVisible;
};
