#include "SceneNode.h"
#include "CameraNode.h"
#include "Scene.h"
#include "ModelImporter.h"
#include "Material.h"
#include "../Actors/Actor.h"
#include "../Actors/RenderComponent.h"
#include "../Actors/TransformComponent.h"
#include "../ResourceCache/ResCache.h"
#include "../ResourceCache/TextureResource.h"
#include "../ResourceCache/ShaderResource.h"
#include "../AppFramework/BaseGameApp.h"
#include "../AppFramework/BaseGameLogic.h"
#include <DirectXColors.h>

SceneNodeProperties::SceneNodeProperties()
	: m_ActorId(INVALID_ACTOR_ID),
	m_RenderPass(RenderPass_0),
	m_worldMatrix(Matrix::Identity)
{

}

SceneNode::SceneNode(ActorId actorId, WeakBaseRenderComponentPtr renderComponent, RenderPass renderPass, const Matrix& worldMatrix)
	: m_pParent(nullptr),
	m_pRenderComponent(renderComponent)
{
	m_Properties.m_ActorId = actorId;
	m_Properties.m_ActorName = (renderComponent != nullptr) ? renderComponent->VGetComponentName() : "SceneNode";
	m_Properties.m_RenderPass = renderPass;
	VSetTransform(worldMatrix);
	m_Properties.m_AABox.Center = Vector3(0.0f, 0.0f, 0.0f);
	m_Properties.m_AABox.Extents = Vector3(0.0f, 0.0f, 0.0f);
}

SceneNode::~SceneNode()
{
}

void SceneNode::VSetTransform(const Matrix& worldMatrix)
{
	m_Properties.m_worldMatrix = worldMatrix;
}

HRESULT SceneNode::VOnInitSceneNode(Scene* pScene)
{
	Color color = (m_pRenderComponent) ? m_pRenderComponent->GetColor() : Color(Colors::White.f);

	for (auto& child : m_Children)
	{
		child->VOnInitSceneNode(pScene);
	}
	return S_OK;
}

HRESULT SceneNode::VOnDeleteSceneNode(Scene *pScene)
{
	for (auto& child : m_Children)
	{
		child->VOnDeleteSceneNode(pScene);
	}
	return S_OK;
}

HRESULT SceneNode::VOnUpdate(Scene* pScene, const GameTime& gameTime)
{
	for (auto& child : m_Children)
	{
		child->VOnUpdate(pScene, gameTime);
	}
	return S_OK;
}

HRESULT SceneNode::VPreRender(Scene* pScene)
{
	StrongActorPtr pActor = MakeStrongPtr(g_pApp->GetGameLogic()->VGetActor(m_Properties.GetActorId()));
	if (pActor != nullptr)
	{
		shared_ptr<TransformComponent> pTransform = MakeStrongPtr(pActor->GetComponent<TransformComponent>(TransformComponent::m_Name));
		if (pTransform)
		{
			m_Properties.m_worldMatrix = pTransform->GetTransform();
		}
	}

	pScene->PushAndSetMatrix(m_Properties.m_worldMatrix);

	return S_OK;
}

HRESULT SceneNode::VRender(Scene* pScene, const GameTime& gameTime)
{
	return S_OK;
}

HRESULT SceneNode::VRenderChildren(Scene* pScene, const GameTime& gameTime)
{
	for (auto& child : m_Children)
	{
		if (child->VPreRender(pScene) == S_OK)
		{
			if (child->VIsVisible(pScene))
			{
				child->VRender(pScene, gameTime);
				child->VRenderChildren(pScene, gameTime);
			}
		}
		child->VPostRender(pScene);
	}

	return S_OK;
}

HRESULT SceneNode::VPostRender(Scene* pScene)
{
	pScene->PopMatrix();
	return S_OK;
}

bool SceneNode::VIsVisible(Scene* pScene) const
{
	return true;
}

bool SceneNode::VAddChild(shared_ptr<ISceneNode> child)
{
	m_Children.push_back(child);
	shared_ptr<SceneNode> kid = static_pointer_cast<SceneNode>(child);
	kid->m_pParent = this;
	
	return true;
}

bool SceneNode::VRemoveChild(ActorId actorId)
{
	for (SceneNodeList::iterator i = m_Children.begin(); i != m_Children.end(); ++i)
	{
		const SceneNodeProperties* pProps = (*i)->VGet();
		if (pProps->GetActorId() != INVALID_ACTOR_ID && actorId == pProps->GetActorId())
		{
			i = m_Children.erase(i);
			return true;
		}
	}
	return false;
}

void SceneNode::VPick(Scene* pScene, int cursorX, int cursorY)
{
	for (auto& child : m_Children)
	{
		child->VPick(pScene, cursorX, cursorY);
	}
}

void SceneNode::SetBoundingBox(const std::vector<Vector3>& postions)
{
	BoundingBox::CreateFromPoints(m_Properties.m_AABox, postions.size(), &postions.front(), sizeof(Vector3));
}

RootNode::RootNode()
	: SceneNode(INVALID_ACTOR_ID, WeakBaseRenderComponentPtr(), RenderPass_0)
{
	m_Children.reserve(RenderPass_Last);

	shared_ptr<SceneNode> staticGroup(DEBUG_NEW SceneNode(INVALID_ACTOR_ID, WeakBaseRenderComponentPtr(), RenderPass_Static));
	m_Children.push_back(staticGroup);

	shared_ptr<SceneNode> actorGroup(DEBUG_NEW SceneNode(INVALID_ACTOR_ID, WeakBaseRenderComponentPtr(), RenderPass_Actor));
	m_Children.push_back(actorGroup);

	shared_ptr<SceneNode> skyGroup(DEBUG_NEW SceneNode(INVALID_ACTOR_ID, WeakBaseRenderComponentPtr(), RenderPass_Sky));
	m_Children.push_back(skyGroup);

	shared_ptr<SceneNode> debugGroup(DEBUG_NEW SceneNode(INVALID_ACTOR_ID, WeakBaseRenderComponentPtr(), RenderPass_Debug));
	m_Children.push_back(debugGroup);

	shared_ptr<SceneNode> invisibleGroup(DEBUG_NEW SceneNode(INVALID_ACTOR_ID, WeakBaseRenderComponentPtr(), RenderPass_NotRendered));
	m_Children.push_back(invisibleGroup);
}

RootNode::~RootNode()
{

}

HRESULT RootNode::VRenderChildren(Scene* pScene, const GameTime& gameTime)
{
	for (int pass = RenderPass_0; pass < RenderPass_Last; ++pass)
	{
		switch (pass)
		{
		case RenderPass_Static:
		case RenderPass_Actor:
		{
			m_Children[pass]->VRenderChildren(pScene, gameTime);
			break;
		}

		case RenderPass_Sky:
		{
			shared_ptr<IRenderState> skyPass = pScene->GetRenderder()->VPrepareSkyBoxPass();
			m_Children[pass]->VRenderChildren(pScene, gameTime);
			break;
		}
		case  RenderPass_Debug:
		{
			m_Children[pass]->VRenderChildren(pScene, gameTime);
			break;
		}
		}
	}

	return S_OK;
}

bool RootNode::VAddChild(shared_ptr<ISceneNode> child)
{
	RenderPass pass = child->VGet()->GetRenderPass();
	if ((unsigned)pass >= m_Children.size() || !m_Children[pass])
	{
		DEBUG_ASSERT(0 && _T("There is no such render pass"));
		return false;
	}

	return m_Children[pass]->VAddChild(child);
}

bool RootNode::VRemoveChild(ActorId actorId)
{
	bool anythingRemoved = false;
	for (int i = RenderPass_0; i < RenderPass_Last; ++i)
	{
		if (m_Children[i]->VRemoveChild(actorId))
		{
			anythingRemoved = true;
		}
	}
	return anythingRemoved;
}

void RootNode::VPick(Scene* pScene, int cursorX, int cursorY)
{
	if (m_Children.size() > RenderPass_Actor)
	{
		m_Children[RenderPass_Actor]->VPick(pScene, cursorX, cursorY);
	}
}

GridNode::GridNode(ActorId actorId, WeakBaseRenderComponentPtr renderComponent)
	: SceneNode(actorId, renderComponent, RenderPass_Static),
	m_pEffect(nullptr),
	m_pCurrentPass(nullptr),
	m_pVertexBuffer(nullptr),
	m_pIndexBuffer(nullptr),
	m_VertexCount(0),
	m_IndexCount(0)
{
	GridRenderComponent* pGridRender = static_cast<GridRenderComponent*>(m_pRenderComponent);
	if (pGridRender != nullptr)
	{
		m_TextureName = pGridRender->GetTextureName();
		m_GridSize = pGridRender->GetGridSize();
		m_TicksInterval = pGridRender->GetTicksInterval();
	}

	Resource effectRes("Effects\\Grid.fx");
	shared_ptr<ResHandle> pEffectResHandle = g_pApp->GetResCache()->GetHandle(&effectRes);
	if (pEffectResHandle != nullptr)
	{
		shared_ptr<HlslResourceExtraData> extra = static_pointer_cast<HlslResourceExtraData>(pEffectResHandle->GetExtraData());
		if (extra != nullptr)
		{
			m_pEffect = extra->GetEffect();
		}
	}

	InitGridVertex();
}

GridNode::~GridNode()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
}

void GridNode::InitGridVertex()
{
	Technique* pCurrentTechnique = m_pEffect->GetTechniquesByName().at("main11");
	if (pCurrentTechnique == nullptr)
	{
		DEBUG_ERROR(std::string("technique is not exist: ") + "main11");
	}
	m_pCurrentPass = pCurrentTechnique->GetPassesByName().at("p0");
	if (m_pCurrentPass == nullptr)
	{
		DEBUG_ERROR(std::string("technique is not exist: ") + "p0");
	}

	Vector2 halfSize = m_GridSize * 0.5f;

	uint16_t rows = static_cast<uint16_t>(m_GridSize.x / m_TicksInterval);
	uint16_t columns = static_cast<uint16_t>(m_GridSize.y / m_TicksInterval);
	m_VertexCount = (rows + 1) * (columns + 1);
	m_IndexCount = rows * columns * 2 * 3;

	std::vector<VertexPositionTexture> vertices;
	vertices.resize(m_VertexCount);

	for (int i = 0; i < (rows + 1); i++)
	{
		for (int j = 0; j < (columns + 1); j++)
		{
			float x = (float)i - halfSize.x;
			float y = halfSize.y - (float)j;

			vertices[i * (columns + 1) + j] = VertexPositionTexture(Vector4(x, 0.0f, y, 1.0f), Vector2(x, y));
		}
	}

	std::vector<uint16_t> indices;
	indices.resize(m_IndexCount);

	uint16_t k = 0;
	for (uint16_t i = 0; i < rows; i++)
	{
		for (uint16_t j = 0; j < columns; j++)
		{
			indices[k] = i * (columns + 1) + j;
			indices[k + 1] = i * (columns + 1) + j + 1;
			indices[k + 2] = (i + 1) * (columns + 1) + j;

			indices[k + 3] = (i + 1) * (columns + 1) + j;
			indices[k + 4] = i * (columns + 1) + j + 1;
			indices[k + 5] = (i + 1) * (columns + 1) + j + 1;

			k += 6;
		}
	}

	m_pCurrentPass->CreateVertexBuffer(&vertices.front(), vertices.size() * sizeof(VertexPositionTexture), &m_pVertexBuffer);
	m_pCurrentPass->CreateIndexBuffer(&indices.front(), indices.size() * sizeof(uint16_t), &m_pIndexBuffer);
}

HRESULT GridNode::VOnInitSceneNode(Scene* pScene)
{
	return S_OK;
}

HRESULT GridNode::VOnDeleteSceneNode(Scene *pScene)
{
	return S_OK;
}

HRESULT GridNode::VRender(Scene* pScene, const GameTime& gameTime)
{
	const std::vector<Variable*>& variables = m_pEffect->GetVariables();
	for (auto variable : variables)
	{
		if (variable->GetVariableSemantic() == "worldviewprojection")
		{
			if (variable->GetVariableType() == "float4x4")
			{
				const XMMATRIX& wvp = pScene->GetCamera()->GetWorldViewProjection(pScene);
				variable->SetMatrix(wvp);
			}
		}
		else if (variable->GetVariableType() == "Texture2D")
		{
			Resource resource(m_TextureName);
			shared_ptr<ResHandle> pTextureRes = g_pApp->GetResCache()->GetHandle(&resource);
			if (pTextureRes != nullptr)
			{
				shared_ptr<D3D11TextureResourceExtraData> extra =
					static_pointer_cast<D3D11TextureResourceExtraData>(pTextureRes->GetExtraData());
				if (extra != nullptr)
				{
					variable->SetResource(extra->GetTexture());
				}
			}
		}
	}

	pScene->GetRenderder()->VInputSetup(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, m_pCurrentPass->GetInputLayout());
	uint32_t stride = m_pCurrentPass->GetVertexSize();
	uint32_t offset = 0;
	pScene->GetRenderder()->VSetVertexBuffers(m_pVertexBuffer, &stride, &offset);
	pScene->GetRenderder()->VSetIndexBuffer(m_pIndexBuffer, IRenderer::Format_uint16, 0);
	pScene->GetRenderder()->VDrawMesh(m_IndexCount, 0, 0, m_pCurrentPass->GetEffectPass());

	return S_OK;
}

HRESULT GridNode::VOnUpdate(Scene* pScene, const GameTime& gameTime)
{
	return S_OK;
}

const std::string GeometryNode::m_Cube = "Cube";
const std::string GeometryNode::m_Sphere = "Sphere";
const std::string GeometryNode::m_Cylinder = "Cylinder";
const std::string GeometryNode::m_Teapot = "Teapot";
const std::string GeometryNode::m_Plane = "Plane";

GeometryNode::GeometryNode(ActorType actorType, ActorId actorId, WeakBaseRenderComponentPtr renderComponent, RenderPass renderPass, const Matrix& worldMatrix)
	: SceneNode(actorId, renderComponent, renderPass),
	m_pEffect(nullptr),
	m_pCurrentPass(nullptr),
	m_pVertexBuffer(nullptr),
	m_pIndexBuffer(nullptr),
	m_Mesh(nullptr)
{
	GeometryRenderComponent* pGeometryRender = static_cast<GeometryRenderComponent*>(m_pRenderComponent);
	if (pGeometryRender != nullptr)
	{
		m_TextureName = pGeometryRender->GetTextureName();
		m_EffectName = pGeometryRender->GetEffectName();
		m_CurrentTechnique = pGeometryRender->GetCurrentTechniqueName();
		m_CurrentPass = pGeometryRender->GetCurrentPassName();
	}

	Resource effectRes(m_EffectName);
	shared_ptr<ResHandle> pEffectResHandle = g_pApp->GetResCache()->GetHandle(&effectRes);
	if (pEffectResHandle != nullptr)
	{
		shared_ptr<HlslResourceExtraData> extra = static_pointer_cast<HlslResourceExtraData>(pEffectResHandle->GetExtraData());
		if (extra != nullptr)
		{
			m_pEffect = extra->GetEffect();
		}
	}

	if (m_pEffect == nullptr)
	{
		DEBUG_ERROR("effect is not exist or valid: " + m_EffectName);
	}

	Technique* pCurrentTechnique = m_pEffect->GetTechniquesByName().at(m_CurrentTechnique);
	if (pCurrentTechnique == nullptr)
	{
		DEBUG_ERROR("technique is not exist: " + m_CurrentTechnique);
	}
	m_pCurrentPass = pCurrentTechnique->GetPassesByName().at(m_CurrentPass);
	if (m_pCurrentPass == nullptr)
	{
		DEBUG_ERROR("technique is not exist: " + m_CurrentTechnique);
	}

	if (actorType == m_Cube)
	{
		CreateCube();
	}
	if (actorType == m_Sphere)
	{
		CreateSphere();
	}
	else if (actorType == m_Cylinder)
	{
		CreateCylinder();
	}
	else if (actorType == m_Teapot)
	{
		CreateTeapot();
	}
	else if (actorType == m_Plane)
	{
		CreatePlane();
	}
	else
	{

	}


	m_pCurrentPass->CreateVertexBuffer(m_Mesh.get(), &m_pVertexBuffer);
	m_pCurrentPass->CreateIndexBuffer(m_Mesh.get(), &m_pIndexBuffer);

	SetBoundingBox(m_Mesh->GetVertices());
}

GeometryNode::~GeometryNode()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
}

HRESULT GeometryNode::VOnInitSceneNode(Scene* pScene)
{
	return S_OK;
}

HRESULT GeometryNode::VOnDeleteSceneNode(Scene *pScene)
{
	return S_OK;
}

HRESULT GeometryNode::VOnUpdate(Scene* pScene, const GameTime& gameTime)
{
	return S_OK;
}

HRESULT GeometryNode::VRender(Scene* pScene, const GameTime& gameTime)
{
	const std::vector<Variable*>& variables = m_pEffect->GetVariables();
	for (auto variable : variables)
	{
		if (variable->GetVariableSemantic() == "worldviewprojection")
		{
			if (variable->GetVariableType() == "float4x4")
			{
				const XMMATRIX& wvp = pScene->GetCamera()->GetWorldViewProjection(pScene);
				variable->SetMatrix(wvp);
			}
		}
		else if (variable->GetVariableType() == "Texture2D")
		{
			Resource resource(m_TextureName);
			shared_ptr<ResHandle> pTextureRes = g_pApp->GetResCache()->GetHandle(&resource);
			if (pTextureRes != nullptr)
			{
				shared_ptr<D3D11TextureResourceExtraData> extra =
					static_pointer_cast<D3D11TextureResourceExtraData>(pTextureRes->GetExtraData());
				if (extra != nullptr)
				{
					variable->SetResource(extra->GetTexture());
				}
			}
		}
	}

	pScene->GetRenderder()->VInputSetup(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, m_pCurrentPass->GetInputLayout());
	uint32_t stride = m_pCurrentPass->GetVertexSize();
	uint32_t offset = 0;
	pScene->GetRenderder()->VSetVertexBuffers(m_pVertexBuffer, &stride, &offset);
	pScene->GetRenderder()->VSetIndexBuffer(m_pIndexBuffer, IRenderer::Format_uint32, 0);
	pScene->GetRenderder()->VDrawMesh(m_IndexCount, 0, 0, m_pCurrentPass->GetEffectPass());

	return S_OK;
}

void GeometryNode::VPick(Scene* pScene, int cursorX, int cursorY)
{
	const Matrix& projectMat = pScene->GetCamera()->GetProjectMatrix();
	float viewX = (2.0f * cursorX / g_pApp->GetGameConfig().m_ScreenWidth - 1.0f) / projectMat.m[0][0];
	float viewY = (1.0f - 2.0f * cursorY / g_pApp->GetGameConfig().m_ScreenHeight) / projectMat.m[1][1];

	Matrix toLocal = (m_Properties.GetWorldMatrix() * pScene->GetCamera()->GetViewMatrix()).Invert();
	Vector3 rayPostition = toLocal.Translation();
	//use right-hand coordinates, z should be -1
	Vector3 rayDir = Vector3::TransformNormal(Vector3(viewX, viewY, -1.0f), toLocal);
	rayDir.Normalize();

	Ray ray(rayPostition, rayDir);

	float distance = 0.0f;
	if (ray.Intersects(m_Properties.GetBoundingBox(), distance))
	{
		if (distance < pScene->GetPickDistance())
		{
			pScene->SetPickDistance(distance);
			for (uint32_t i = 0; i < m_IndexCount; i += 3)
			{
				const std::vector<Vector3>& vertices = m_Mesh->GetVertices();
				const std::vector<uint32_t>& indices = m_Mesh->GetIndices();
				Vector3 tri0 = vertices.at(indices[i]);
				Vector3 tri1 = vertices.at(indices[i + 1]);
				Vector3 tri2 = vertices.at(indices[i + 2]);

				float distance = 0.0f;
				if (ray.Intersects(tri0, tri1, tri2, distance))
				{
					pScene->SetPickedActor(m_Properties.GetActorId());
				}
			}
		}
	}
}

void GeometryNode::CreateCube()
{
	CubeRenderComponent* pMeshRender = static_cast<CubeRenderComponent*>(m_pRenderComponent);
	if (pMeshRender != nullptr)
	{
		float size = pMeshRender->GetSize();
		bool useRHcoords = pMeshRender->UseRHcoords();

		std::vector<VertexPositionNormalTexture> vertices;
		std::vector<uint16_t> indices;
		GeometricPrimitive::CreateCube(vertices, indices, size, useRHcoords);
		m_IndexCount = indices.size();
		m_Mesh = unique_ptr<Mesh>(new Mesh(vertices, indices));
	}
}

void GeometryNode::CreateSphere()
{
	SphereRenderComponent* pMeshRender = static_cast<SphereRenderComponent*>(m_pRenderComponent);
	if (pMeshRender != nullptr)
	{
		float diameter = pMeshRender->GetDiameter();
		uint32_t tessellation = pMeshRender->GetTessellation();
		bool useRHcoords = pMeshRender->UseRHcoords();

		std::vector<VertexPositionNormalTexture> vertices;
		std::vector<uint16_t> indices;
		GeometricPrimitive::CreateGeoSphere(vertices, indices, diameter, tessellation, false);
		m_IndexCount = indices.size();
		m_Mesh = unique_ptr<Mesh>(new Mesh(vertices, indices));
	}
}

void GeometryNode::CreateCylinder()
{
	CylinderRenderComponent* pMeshRender = static_cast<CylinderRenderComponent*>(m_pRenderComponent);
	if (pMeshRender != nullptr)
	{
		float height = pMeshRender->GetHeight();
		float diameter = pMeshRender->GetDiameter();
		uint32_t tessellation = pMeshRender->GetTessellation();
		bool useRHcoords = pMeshRender->UseRHcoords();

		std::vector<VertexPositionNormalTexture> vertices;
		std::vector<uint16_t> indices;
		GeometricPrimitive::CreateCylinder(vertices, indices, height, diameter, tessellation, useRHcoords);
		m_IndexCount = indices.size();
		m_Mesh = unique_ptr<Mesh>(new Mesh(vertices, indices));
	}
}

void GeometryNode::CreateTeapot()
{
	TeapotRenderComponent* pMeshRender = static_cast<TeapotRenderComponent*>(m_pRenderComponent);
	if (pMeshRender != nullptr)
	{
		float size = pMeshRender->GetSize();
		uint32_t tessellation = pMeshRender->GetTessellation();
		bool useRHcoords = pMeshRender->UseRHcoords();

		std::vector<VertexPositionNormalTexture> vertices;
		std::vector<uint16_t> indices;
		GeometricPrimitive::CreateTeapot(vertices, indices, size, tessellation, useRHcoords);
		m_IndexCount = indices.size();
		m_Mesh = unique_ptr<Mesh>(new Mesh(vertices, indices));
	}
}

void GeometryNode::CreatePlane()
{

}

ModelNode::ModelNode(
	ActorId actorId, WeakBaseRenderComponentPtr renderComponent, RenderPass renderPass, const Matrix& worldMatrix)
	: SceneNode(actorId, renderComponent, renderPass, worldMatrix),
	m_pEffect(nullptr),
	m_pCurrentPass(nullptr),
	m_WorldMatrix(worldMatrix)
{
	ModelRenderComponent* pMeshRender = static_cast<ModelRenderComponent*>(m_pRenderComponent);
	if (pMeshRender != nullptr)
	{
		m_ModelName = pMeshRender->GetModelName();
		m_TextureNames = pMeshRender->GetTextureName();
		m_EffectName = pMeshRender->GetEffectName();
		m_CurrentTechnique = pMeshRender->GetCurrentTechniqueName();
		m_CurrentPass = pMeshRender->GetCurrentPassName();
	}

	Resource effectRes(m_EffectName);
	shared_ptr<ResHandle> pEffectResHandle = g_pApp->GetResCache()->GetHandle(&effectRes);
	if (pEffectResHandle != nullptr)
	{
		shared_ptr<HlslResourceExtraData> extra = static_pointer_cast<HlslResourceExtraData>(pEffectResHandle->GetExtraData());
		if (extra != nullptr)
		{
			m_pEffect = extra->GetEffect();
		}
	}

	Resource modelRes(m_ModelName);
	shared_ptr<ResHandle> pModelResHandle = g_pApp->GetResCache()->GetHandle(&modelRes);
	std::unique_ptr<Model> model(new Model(pModelResHandle->Buffer(), pModelResHandle->Size(), true));

	Technique* pCurrentTechnique = m_pEffect->GetTechniquesByName().at(m_CurrentTechnique);
	if (pCurrentTechnique == nullptr)
	{
		DEBUG_ERROR("technique is not exist: " + m_CurrentTechnique);
	}
	m_pCurrentPass = pCurrentTechnique->GetPassesByName().at(m_CurrentPass);
	if (m_pCurrentPass == nullptr)
	{
		DEBUG_ERROR("technique is not exist: " + m_CurrentTechnique);
	}

	uint32_t meshSize = model->GetMeshes().size();
	m_pVertexBuffers.resize(meshSize);
	m_pIndexBuffers.resize(meshSize);
	m_IndexCounts.resize(meshSize);
	m_pTextures.resize(meshSize);
	for (uint32_t i = 0; i < meshSize; i++)
	{
		auto mesh = model->GetMeshes().at(i);
		ID3D11Buffer* pVertexBuffer = nullptr;
		ID3D11Buffer* pIndexBuffer = nullptr;
		m_pCurrentPass->CreateVertexBuffer(mesh, &pVertexBuffer);
		m_pCurrentPass->CreateIndexBuffer(mesh, &pIndexBuffer);

		m_pVertexBuffers[i] = pVertexBuffer;
		m_pIndexBuffers[i] = pIndexBuffer;
		m_IndexCounts[i] = mesh->GetIndices().size();

		if (m_TextureNames.size() > i)
		{
			Resource resource(m_TextureNames[i]);
			shared_ptr<ResHandle> pTextureRes = g_pApp->GetResCache()->GetHandle(&resource);
			if (pTextureRes != nullptr)
			{
				shared_ptr<D3D11TextureResourceExtraData> extra =
					static_pointer_cast<D3D11TextureResourceExtraData>(pTextureRes->GetExtraData());
				if (extra != nullptr)
				{
					m_pTextures[i] = extra->GetTexture();
				}
			}
		}
	}
}

ModelNode::~ModelNode()
{
	for (auto pVertexBuffer : m_pVertexBuffers)
	{
		SAFE_RELEASE(pVertexBuffer);
	}
	m_pVertexBuffers.clear();

	for (auto pIndexBuffer : m_pIndexBuffers)
	{
		SAFE_RELEASE(pIndexBuffer);
	}
	m_pIndexBuffers.clear();
}

HRESULT ModelNode::VOnInitSceneNode(Scene *pScene)
{
	return S_OK;
}

HRESULT ModelNode::VOnDeleteSceneNode(Scene* pScene)
{
	return S_OK;
}

HRESULT ModelNode::VOnUpdate(Scene* pScene, const GameTime& gameTime)
{
	return S_OK;
}

HRESULT ModelNode::VRender(Scene* pScene, const GameTime& gameTime)
{
	const std::vector<Variable*>& variables = m_pEffect->GetVariables();
	for (auto variable : variables)
	{
		const std::string& semantic = variable->GetVariableSemantic();
		if (semantic == "worldviewprojection")
		{
			const std::string& type = variable->GetVariableType();
			if (type == "float4x4")
			{
				const XMMATRIX& wvp = pScene->GetCamera()->GetWorldViewProjection(pScene);
				variable->SetMatrix(wvp);
			}
		}
	}

	pScene->GetRenderder()->VInputSetup(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, m_pCurrentPass->GetInputLayout());

	for (uint32_t i = 0, count = m_IndexCounts.size(); i < count; i++)
	{
		for (auto variable : variables)
		{
			const std::string& type = variable->GetVariableType();
			if (type == "Texture2D")
			{
				variable->SetResource(m_pTextures[i]);
				break;
			}
		}

		uint32_t stride = m_pCurrentPass->GetVertexSize();
		uint32_t offset = 0;
		pScene->GetRenderder()->VSetVertexBuffers(m_pVertexBuffers[i], &stride, &offset);
		pScene->GetRenderder()->VSetIndexBuffer(m_pIndexBuffers[i], IRenderer::Format_uint32, 0);
		pScene->GetRenderder()->VDrawMesh(m_IndexCounts[i], 0, 0, m_pCurrentPass->GetEffectPass());
	}

	return S_OK;
}

SkyboxNode::SkyboxNode(ActorId actorId, WeakBaseRenderComponentPtr renderComponent, RenderPass renderPass)
	: SceneNode(actorId, renderComponent, renderPass),
	m_pEffect(nullptr),
	m_pCurrentPass(nullptr),
	m_pVertexBuffer(nullptr),
	m_pIndexBuffer(nullptr),
	m_IndexCount(0),
	m_ScaleMatrix(Matrix::CreateScale(500.0f))
{
	SkyboxRenderComponent* pSkyboxRender = static_cast<SkyboxRenderComponent*>(m_pRenderComponent);
	if (pSkyboxRender != nullptr)
	{
		m_TextureName = pSkyboxRender->GetTextureName();
	}

	Resource effectRes("Effects\\Skybox.fx");
	shared_ptr<ResHandle> pEffectResHandle = g_pApp->GetResCache()->GetHandle(&effectRes);
	if (pEffectResHandle != nullptr)
	{
		shared_ptr<HlslResourceExtraData> extra = static_pointer_cast<HlslResourceExtraData>(pEffectResHandle->GetExtraData());
		if (extra != nullptr)
		{
			m_pEffect = extra->GetEffect();
		}
	}

	Resource modelRes("Models\\Sphere.obj");
	shared_ptr<ResHandle> pModelResHandle = g_pApp->GetResCache()->GetHandle(&modelRes);
	std::unique_ptr<Model> model(new Model(pModelResHandle->Buffer(), pModelResHandle->Size(), true));

	Technique* pCurrentTechnique = m_pEffect->GetTechniquesByName().at("main11");
	if (pCurrentTechnique == nullptr)
	{
		DEBUG_ERROR(std::string("technique is not exist: ") + "main11");
	}
	m_pCurrentPass = pCurrentTechnique->GetPassesByName().at("p0");
	if (m_pCurrentPass == nullptr)
	{
		DEBUG_ERROR(std::string("technique is not exist: ") + "p0");
	}

	Mesh* mesh = model->GetMeshes().at(0);
	m_pCurrentPass->CreateVertexBuffer(mesh, &m_pVertexBuffer);
	m_pCurrentPass->CreateIndexBuffer(mesh, &m_pIndexBuffer);
	m_IndexCount = mesh->GetIndices().size();
}

SkyboxNode::~SkyboxNode()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
}

bool SkyboxNode::VIsVisible(Scene* pScene)
{
	return true;
}

HRESULT SkyboxNode::VPreRender(Scene* pScene)
{
	m_ScaleMatrix.Translation(pScene->GetCamera()->GetPosition());
	VSetTransform(m_ScaleMatrix);

	return SceneNode::VPreRender(pScene);
}

HRESULT SkyboxNode::VOnInitSceneNode(Scene* pScene)
{
	return S_OK;
}

HRESULT SkyboxNode::VOnDeleteSceneNode(Scene *pScene)
{
	return S_OK;
}

HRESULT SkyboxNode::VRender(Scene* pScene, const GameTime& gameTime)
{
	const std::vector<Variable*>& variables = m_pEffect->GetVariables();
	for (auto variable : variables)
	{
		if (variable->GetVariableSemantic() == "worldviewprojection")
		{
			if (variable->GetVariableType() == "float4x4")
			{
				const XMMATRIX& wvp = pScene->GetCamera()->GetWorldViewProjection(pScene);
				variable->SetMatrix(wvp);
			}
		}
		else if (variable->GetVariableType() == "TextureCube")
		{
			Resource resource(m_TextureName);
			shared_ptr<ResHandle> pTextureRes = g_pApp->GetResCache()->GetHandle(&resource);
			if (pTextureRes != nullptr)
			{
				shared_ptr<D3D11TextureResourceExtraData> extra =
					static_pointer_cast<D3D11TextureResourceExtraData>(pTextureRes->GetExtraData());
				if (extra != nullptr)
				{
					variable->SetResource(extra->GetTexture());
				}
			}
		}
	}

	pScene->GetRenderder()->VInputSetup(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, m_pCurrentPass->GetInputLayout());
	uint32_t stride = m_pCurrentPass->GetVertexSize();
	uint32_t offset = 0;
	pScene->GetRenderder()->VSetVertexBuffers(m_pVertexBuffer, &stride, &offset);
	pScene->GetRenderder()->VSetIndexBuffer(m_pIndexBuffer, IRenderer::Format_uint32, 0);
	pScene->GetRenderder()->VDrawMesh(m_IndexCount, 0, 0, m_pCurrentPass->GetEffectPass());

	return S_OK;
}


HRESULT SkyboxNode::VOnUpdate(Scene* pScene, const GameTime& gameTime)
{
	return S_OK;
}

DebugAssistNode::DebugAssistNode()
	: SceneNode(INVALID_ACTOR_ID, nullptr, RenderPass_Debug, Matrix::CreateTranslation(0.0f, 0.5f, 0.0f)),
	m_pEffect(nullptr),
	m_pCurrentPass(nullptr),
	m_pVertexBuffer(nullptr),
	m_pIndexBuffer(nullptr),
	m_Transform(TT_None),
	m_IsVisible(false)
{

}

DebugAssistNode::~DebugAssistNode()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
}

HRESULT DebugAssistNode::VOnInitSceneNode(Scene* pScene)
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);

	Resource effectRes("Effects\\DebugAssist.fx");
	shared_ptr<ResHandle> pEffectResHandle = g_pApp->GetResCache()->GetHandle(&effectRes);
	if (pEffectResHandle == nullptr)
	{
		return S_FALSE;
	}
	shared_ptr<HlslResourceExtraData> extra = static_pointer_cast<HlslResourceExtraData>(pEffectResHandle->GetExtraData());
	if (extra == nullptr)
	{
		return S_FALSE;
	}

	m_pEffect = extra->GetEffect();

	Technique* pCurrentTechnique = m_pEffect->GetTechniquesByName().at("main11");
	if (pCurrentTechnique == nullptr)
	{
		DEBUG_ERROR(std::string("technique is not exist: ") + "main11");
		return S_FALSE;
	}
	m_pCurrentPass = pCurrentTechnique->GetPassesByName().at("p0");
	if (m_pCurrentPass == nullptr)
	{
		DEBUG_ERROR(std::string("technique is not exist: ") + "p0");
		return S_FALSE;
	}

	CreateGeometryBuffers();

	return S_OK;
}

HRESULT DebugAssistNode::VOnDeleteSceneNode(Scene *pScene)
{
	return S_OK;
}

HRESULT DebugAssistNode::VOnUpdate(Scene* pScene, const GameTime& gameTime)
{
	return S_OK;
}

HRESULT DebugAssistNode::VRender(Scene* pScene, const GameTime& gameTime)
{
	shared_ptr<ISceneNode> pPickedNode = pScene->FindActor(pScene->GetPickedActor());
	if (pPickedNode == nullptr)
	{
		return S_OK;
	}

	uint32_t stride = m_pCurrentPass->GetVertexSize();
	uint32_t offset = 0;
	pScene->GetRenderder()->VSetVertexBuffers(m_pVertexBuffer, &stride, &offset);
	pScene->GetRenderder()->VSetIndexBuffer(m_pIndexBuffer, IRenderer::Format_uint16, 0);

	pScene->GetRenderder()->VInputSetup(D3D11_PRIMITIVE_TOPOLOGY_LINELIST, m_pCurrentPass->GetInputLayout());
	RenderBoundingBox(pScene, pPickedNode->VGet()->GetBoundingBox(), pPickedNode->VGet()->GetWorldMatrix());

	shared_ptr<IRenderState> debugPass = pScene->GetRenderder()->VPrepareDebugPass();

	pScene->GetRenderder()->VInputSetup(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, m_pCurrentPass->GetInputLayout());
	switch (m_Transform)
	{
	case DebugAssistNode::TT_None:
		break;
	case DebugAssistNode::TT_Translation:
		RenderTranslateAxes(pScene, pPickedNode->VGet()->GetBoundingBox(), pPickedNode->VGet()->GetWorldMatrix());
		break;
	case DebugAssistNode::TT_Rotation:
		RenderRotateRings(pScene, pPickedNode->VGet()->GetBoundingBox(), pPickedNode->VGet()->GetWorldMatrix());
		break;
	case DebugAssistNode::TT_Scale:
		RenderScaleAxes(pScene, pPickedNode->VGet()->GetBoundingBox(), pPickedNode->VGet()->GetWorldMatrix());
		break;
	default:
		break;
	}

	return S_OK;
}

bool DebugAssistNode::VIsVisible(Scene* pScene) const
{
	return pScene->GetPickedActor() != INVALID_ACTOR_ID;
}

HRESULT DebugAssistNode::RenderBoundingBox(Scene* pScene, const BoundingBox& aaBox, const Matrix& world)
{
	Variable* variable = m_pEffect->GetVariablesByName().at("WorldViewProjection");
	Matrix boxWorld = world * Matrix::CreateScale(aaBox.Extents * 2.0f);
	boxWorld.Translation(world.Translation());
	boxWorld = boxWorld * Matrix::CreateTranslation(aaBox.Center);
	const XMMATRIX& wvp = boxWorld * pScene->GetCamera()->GetViewMatrix() * pScene->GetCamera()->GetProjectMatrix();
	variable->SetMatrix(wvp);

	Variable* ambientColor = m_pEffect->GetVariablesByName().at("AmbientColor");
	ambientColor->SetVector(Color(Colors::LightSkyBlue.f));

	pScene->GetRenderder()->VDrawMesh(m_AABoxIndexCount, m_AABoxIndexOffset, m_AABoxVertexOffset, m_pCurrentPass->GetEffectPass());

	return S_OK;
}

HRESULT DebugAssistNode::RenderTranslateAxes(Scene* pScene, const BoundingBox& aaBox, const Matrix& world)
{
	Variable* variable = m_pEffect->GetVariablesByName().at("WorldViewProjection");
	Vector3 cameraPos = pScene->GetCamera()->GetPosition();
	Matrix aixsWorld = world * Matrix::CreateScale(Vector3::Distance(cameraPos, world.Translation()) * 0.2f);
	aixsWorld.Translation(world.Translation());
	aixsWorld = aixsWorld * Matrix::CreateTranslation(aaBox.Center);
	const XMMATRIX& wvp = aixsWorld * pScene->GetCamera()->GetViewMatrix() * pScene->GetCamera()->GetProjectMatrix();

	Variable* ambientColor = m_pEffect->GetVariablesByName().at("AmbientColor");
	ambientColor->SetVector(Color(1.0f, 0.0f, 0.0f));
	variable->SetMatrix(Matrix::CreateRotationZ(XMConvertToRadians(-90)) * Matrix::CreateTranslation(Vector3(0.3f, 0.0f, 0.0f)) * wvp);
	pScene->GetRenderder()->VDrawMesh(m_CylinderIndexCount, m_CylinderIndexOffset, m_CylinderVertexOffset, m_pCurrentPass->GetEffectPass());
	variable->SetMatrix(Matrix::CreateRotationZ(XMConvertToRadians(-90)) * Matrix::CreateTranslation(Vector3(0.6f, 0.0f, 0.0f)) * wvp);
	pScene->GetRenderder()->VDrawMesh(
		m_ConeIndexCount, m_ConeIndexOffset, m_ConeVertexOffset, m_pCurrentPass->GetEffectPass());

	ambientColor->SetVector(Color(0.0f, 1.0f, 0.0f));
	variable->SetMatrix(Matrix::CreateTranslation(Vector3(0.0f, 0.3f, 0.0f)) * wvp);
	pScene->GetRenderder()->VDrawMesh(
		m_CylinderIndexCount, m_CylinderIndexOffset, m_CylinderVertexOffset, m_pCurrentPass->GetEffectPass());
	variable->SetMatrix(Matrix::CreateTranslation(Vector3(0.0f, 0.6f, 0.0f)) * wvp);
	pScene->GetRenderder()->VDrawMesh(
		m_ConeIndexCount, m_ConeIndexOffset, m_ConeVertexOffset, m_pCurrentPass->GetEffectPass());

	ambientColor->SetVector(Color(0.0f, 0.0f, 1.0f));
	variable->SetMatrix(Matrix::CreateRotationX(XMConvertToRadians(90)) * Matrix::CreateTranslation(Vector3(0.0f, 0.0f, 0.3f)) * wvp);
	pScene->GetRenderder()->VDrawMesh(
		m_CylinderIndexCount, m_CylinderIndexOffset, m_CylinderVertexOffset, m_pCurrentPass->GetEffectPass());
	variable->SetMatrix(Matrix::CreateRotationX(XMConvertToRadians(90)) * Matrix::CreateTranslation(Vector3(0.0f, 0.0f, 0.6f)) * wvp);
	pScene->GetRenderder()->VDrawMesh(
		m_ConeIndexCount, m_ConeIndexOffset, m_ConeVertexOffset, m_pCurrentPass->GetEffectPass());
	return S_OK;
}

HRESULT DebugAssistNode::RenderRotateRings(Scene* pScene, const BoundingBox& aaBox, const Matrix& world)
{
	Variable* variable = m_pEffect->GetVariablesByName().at("WorldViewProjection");
	Vector3 cameraPos = pScene->GetCamera()->GetPosition();
	Matrix ringWorld = world * world * Matrix::CreateScale(Vector3::Distance(cameraPos, world.Translation()) * 0.2f);
	ringWorld.Translation(world.Translation());
	ringWorld = ringWorld * Matrix::CreateTranslation(aaBox.Center);
	const XMMATRIX& wvp = ringWorld * pScene->GetCamera()->GetViewMatrix() * pScene->GetCamera()->GetProjectMatrix();

	variable->SetMatrix(Matrix::CreateRotationZ(XMConvertToRadians(-90)) * wvp);
	Variable* ambientColor = m_pEffect->GetVariablesByName().at("AmbientColor");
	ambientColor->SetVector(Color(1.0f, 0.0f, 0.0f));
	pScene->GetRenderder()->VDrawMesh(
		m_TorusIndexCount, m_TorusIndexOffset, m_TorusVertexOffset, m_pCurrentPass->GetEffectPass());

	variable->SetMatrix(wvp);
	ambientColor->SetVector(Color(0.0f, 1.0f, 0.0f));

	pScene->GetRenderder()->VDrawMesh(
		m_TorusIndexCount, m_TorusIndexOffset, m_TorusVertexOffset, m_pCurrentPass->GetEffectPass());

	variable->SetMatrix(Matrix::CreateRotationX(XMConvertToRadians(90)) * wvp);
	ambientColor->SetVector(Color(0.0f, 0.0f, 1.0f));

	pScene->GetRenderder()->VDrawMesh(
		m_TorusIndexCount, m_TorusIndexOffset, m_TorusVertexOffset, m_pCurrentPass->GetEffectPass());

	return S_OK;
}

HRESULT DebugAssistNode::RenderScaleAxes(Scene* pScene, const BoundingBox& aaBox, const Matrix& world)
{
	Variable* variable = m_pEffect->GetVariablesByName().at("WorldViewProjection");
	Vector3 cameraPos = pScene->GetCamera()->GetPosition();
	Matrix aixsWorld = world * Matrix::CreateScale(Vector3::Distance(cameraPos, world.Translation()) * 0.2f);
	aixsWorld.Translation(world.Translation());
	aixsWorld = aixsWorld * Matrix::CreateTranslation(aaBox.Center);
	const XMMATRIX& wvp = aixsWorld * pScene->GetCamera()->GetViewMatrix() * pScene->GetCamera()->GetProjectMatrix();

	Variable* ambientColor = m_pEffect->GetVariablesByName().at("AmbientColor");
	ambientColor->SetVector(Color(1.0f, 0.0f, 0.0f));
	variable->SetMatrix(Matrix::CreateRotationZ(XMConvertToRadians(-90)) * Matrix::CreateTranslation(Vector3(0.3f, 0.0f, 0.0f)) * wvp);
	pScene->GetRenderder()->VDrawMesh(m_CylinderIndexCount, m_CylinderIndexOffset, m_CylinderVertexOffset, m_pCurrentPass->GetEffectPass());
	variable->SetMatrix(Matrix::CreateRotationZ(XMConvertToRadians(-90)) * Matrix::CreateTranslation(Vector3(0.6f, 0.0f, 0.0f)) * wvp);
	pScene->GetRenderder()->VDrawMesh(
		m_CubeIndexCount, m_CubeIndexOffset, m_CubeVertexOffset, m_pCurrentPass->GetEffectPass());

	ambientColor->SetVector(Color(0.0f, 1.0f, 0.0f));
	variable->SetMatrix(Matrix::CreateTranslation(Vector3(0.0f, 0.3f, 0.0f)) * wvp);
	pScene->GetRenderder()->VDrawMesh(
		m_CylinderIndexCount, m_CylinderIndexOffset, m_CylinderVertexOffset, m_pCurrentPass->GetEffectPass());
	variable->SetMatrix(Matrix::CreateTranslation(Vector3(0.0f, 0.6f, 0.0f)) * wvp);
	pScene->GetRenderder()->VDrawMesh(
		m_CubeIndexCount, m_CubeIndexOffset, m_CubeVertexOffset, m_pCurrentPass->GetEffectPass());

	ambientColor->SetVector(Color(0.0f, 0.0f, 1.0f));
	variable->SetMatrix(Matrix::CreateRotationX(XMConvertToRadians(90)) * Matrix::CreateTranslation(Vector3(0.0f, 0.0f, 0.3f)) * wvp);
	pScene->GetRenderder()->VDrawMesh(
		m_CylinderIndexCount, m_CylinderIndexOffset, m_CylinderVertexOffset, m_pCurrentPass->GetEffectPass());
	variable->SetMatrix(Matrix::CreateRotationX(XMConvertToRadians(90)) * Matrix::CreateTranslation(Vector3(0.0f, 0.0f, 0.6f)) * wvp);
	pScene->GetRenderder()->VDrawMesh(
		m_CubeIndexCount, m_CubeIndexOffset, m_CubeVertexOffset, m_pCurrentPass->GetEffectPass());
	return S_OK;
}

void DebugAssistNode::CreateAABox(std::vector<Vector3>& vertices, std::vector<uint16_t>& indices)
{
	vertices.clear();
	vertices.reserve(8);
	Color color(Colors::LightSkyBlue.f);

	vertices.push_back(Vector3(-0.5f, -0.5f, -0.5f));
	vertices.push_back(Vector3(0.5f, -0.5f, -0.5f));
	vertices.push_back(Vector3(0.5f, -0.5f, 0.5f));
	vertices.push_back(Vector3(-0.5f, -0.5f, 0.5f));
	vertices.push_back(Vector3(-0.5f, 0.5f, -0.5f));
	vertices.push_back(Vector3(0.5f, 0.5f, -0.5f));
	vertices.push_back(Vector3(0.5f, 0.5f, 0.5f));
	vertices.push_back(Vector3(-0.5f, 0.5f, 0.5f));

	uint16_t arrayIndices[] = { 0, 1, 1, 2, 2, 3, 3, 0, 4, 5, 5, 6, 6, 7, 7, 4, 0, 4, 1, 5, 2, 6, 3, 7 };
	indices.clear();
	indices.resize(ARRAYSIZE(arrayIndices));
	memcpy_s(&indices.front(), sizeof(arrayIndices), arrayIndices, sizeof(arrayIndices));
}

void DebugAssistNode::AddVertex(std::vector<Vector3>& vertices, const std::vector<struct VertexPositionNormalTexture>& inputVertices)
{
	for (auto& vertex : inputVertices)
	{
		vertices.push_back(vertex.position);
	}
}

void DebugAssistNode::CreateGeometryBuffers()
{
	std::vector<Vector3> boxVertices;
	std::vector<uint16_t> boxIndices;
	CreateAABox(boxVertices, boxIndices);

	std::vector<VertexPositionNormalTexture> cylinderVertices;
	std::vector<uint16_t> cylinderIndices;
	GeometricPrimitive::CreateCylinder(cylinderVertices, cylinderIndices, 0.6f, 0.008f);

	std::vector<VertexPositionNormalTexture> coneVertices;
	std::vector<uint16_t> coneIndices;
	GeometricPrimitive::CreateCone(coneVertices, coneIndices, 0.06f, 0.1f);

	std::vector<VertexPositionNormalTexture> torusVertices;
	std::vector<uint16_t> torusIndices;
	GeometricPrimitive::CreateTorus(torusVertices, torusIndices, 1.0f, 0.008f);

	std::vector<VertexPositionNormalTexture> cubeVertices;
	std::vector<uint16_t> cubeIndices;
	GeometricPrimitive::CreateCube(cubeVertices, cubeIndices, 0.06f);

	m_AABoxVertexOffset = 0;
	m_CylinderVertexOffset = boxVertices.size();
	m_ConeVertexOffset = m_CylinderVertexOffset + cylinderVertices.size();
	m_TorusVertexOffset = m_ConeVertexOffset + coneVertices.size();
	m_CubeVertexOffset = m_TorusVertexOffset + torusVertices.size();

	m_AABoxIndexCount = boxIndices.size();
	m_CylinderIndexCount = cylinderIndices.size();
	m_ConeIndexCount = coneIndices.size();
	m_TorusIndexCount = torusIndices.size();
	m_CubeIndexCount = cubeIndices.size();

	m_AABoxIndexOffset = 0;
	m_CylinderIndexOffset = m_AABoxIndexCount;
	m_ConeIndexOffset = m_CylinderIndexOffset + m_CylinderIndexCount;
	m_TorusIndexOffset = m_ConeIndexOffset + m_ConeIndexCount;
	m_CubeIndexOffset = m_TorusIndexOffset + m_TorusIndexCount;

	uint32_t vertexCount = 
		boxVertices.size() + cylinderVertices.size() + coneVertices.size() + torusVertices.size() + cubeVertices.size();
	std::vector<Vector3> vertices;
	vertices.reserve(vertexCount);

	vertices.insert(vertices.end(), boxVertices.begin(), boxVertices.end());

	AddVertex(vertices, cylinderVertices);
	AddVertex(vertices, coneVertices);
	AddVertex(vertices, torusVertices);
	AddVertex(vertices, cubeVertices);

	std::vector<uint16_t> indices;
	indices.insert(indices.end(), boxIndices.begin(), boxIndices.end());
	indices.insert(indices.end(), cylinderIndices.begin(), cylinderIndices.end());
	indices.insert(indices.end(), coneIndices.begin(), coneIndices.end());
	indices.insert(indices.end(), torusIndices.begin(), torusIndices.end());
	indices.insert(indices.end(), cubeIndices.begin(), cubeIndices.end());

	m_pCurrentPass->CreateVertexBuffer(&vertices.front(), vertices.size() * sizeof(Vector3), &m_pVertexBuffer);
	m_pCurrentPass->CreateIndexBuffer(&indices.front(), indices.size() * sizeof(uint16_t), &m_pIndexBuffer);
}
