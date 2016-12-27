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
#include "GeometricPrimitive.h"
#include <DirectXColors.h>

SceneNodeProperties::SceneNodeProperties()
	: m_ActorId(INVALID_ACTOR_ID),
	m_RenderPass(RenderPass_0),
	m_worldMatrix(Matrix::Identity)
{

}

SceneNode::SceneNode(ActorId actorId, WeakBaseRenderComponentPtr renderComponent, RenderPass renderPass, const Matrix& worldMatrix)
	: m_pParent(nullptr), m_pRenderComponent(renderComponent)
{
	m_Properties.m_ActorId = actorId;
	m_Properties.m_ActorName = (renderComponent != nullptr) ? renderComponent->VGetComponentName() : "SceneNode";
	m_Properties.m_RenderPass = renderPass;
	VSetTransform(worldMatrix);
	SetRadius(0);
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

RootNode::RootNode() : SceneNode(INVALID_ACTOR_ID, WeakBaseRenderComponentPtr(), RenderPass_0)
{
	m_Children.reserve(RenderPass_Last);

	shared_ptr<SceneNode> staticGroup(DEBUG_NEW SceneNode(INVALID_ACTOR_ID, WeakBaseRenderComponentPtr(), RenderPass_Static));
	m_Children.push_back(staticGroup);

	shared_ptr<SceneNode> actorGroup(DEBUG_NEW SceneNode(INVALID_ACTOR_ID, WeakBaseRenderComponentPtr(), RenderPass_Actor));
	m_Children.push_back(actorGroup);

	shared_ptr<SceneNode> skyGroup(DEBUG_NEW SceneNode(INVALID_ACTOR_ID, WeakBaseRenderComponentPtr(), RenderPass_Sky));
	m_Children.push_back(skyGroup);

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
			m_Children[pass]->VRenderChildren(pScene, gameTime);
			break;

		case RenderPass_Sky:
		{
// 			shared_ptr<IRenderState> skyPass = pScene->GetRenderder()->
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

GridNode::GridNode(ActorId actorId, WeakBaseRenderComponentPtr renderComponent)
	: SceneNode(actorId, renderComponent, RenderPass_0),
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

	uint32_t rows = static_cast<uint32_t>(m_GridSize.x / m_TicksInterval);
	uint32_t columns = static_cast<uint32_t>(m_GridSize.y / m_TicksInterval);
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

	std::vector<uint32_t> indices;
	indices.resize(m_IndexCount);

	uint32_t k = 0;
	for (uint32_t i = 0; i < rows; i++)
	{
		for (uint32_t j = 0; j < columns; j++)
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
	m_pCurrentPass->CreateIndexBuffer(&indices.front(), indices.size() * sizeof(uint32_t), &m_pIndexBuffer);
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

	g_pApp->GetRendererAPI()->VInputSetup(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, m_pCurrentPass->GetInputLayout());
	g_pApp->GetRendererAPI()->VDrawMesh(
		m_pCurrentPass->GetVertexSize(), m_pVertexBuffer, m_VertexCount, m_pIndexBuffer, m_IndexCount, m_pCurrentPass->GetEffectPass());

	return S_OK;
}

HRESULT GridNode::VOnUpdate(Scene* pScene, const GameTime& gameTime)
{
	return S_OK;
}

const std::string GeometryNode::m_Sphere = "Sphere";
const std::string GeometryNode::m_Cylinder = "Cylinder";
const std::string GeometryNode::m_Teapot = "Teapot";

GeometryNode::GeometryNode(ActorType actorType, ActorId actorId, WeakBaseRenderComponentPtr renderComponent, RenderPass renderPass, const Matrix& worldMatrix)
	: SceneNode(actorId, renderComponent, renderPass),
	m_pEffect(nullptr),
	m_pCurrentPass(nullptr),
	m_pVertexBuffer(nullptr),
	m_pIndexBuffer(nullptr),
	m_IndexCount(0)
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
	else
	{

	}
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

	g_pApp->GetRendererAPI()->VInputSetup(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, m_pCurrentPass->GetInputLayout());
	g_pApp->GetRendererAPI()->VDrawMesh(
		m_pCurrentPass->GetVertexSize(), m_pVertexBuffer, 0, m_pIndexBuffer, m_IndexCount, m_pCurrentPass->GetEffectPass());

	return S_OK;
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
		GeometricPrimitive::CreateSphere(vertices, indices, diameter, tessellation, useRHcoords);

		std::unique_ptr<Mesh> mesh(new Mesh(vertices, indices));
		m_pCurrentPass->CreateVertexBuffer(mesh.get(), &m_pVertexBuffer);
		m_pCurrentPass->CreateIndexBuffer(mesh.get(), &m_pIndexBuffer);
		m_IndexCount = indices.size();
	}
}

void GeometryNode::CreateCylinder()
{

}

void GeometryNode::CreateTeapot()
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

	g_pApp->GetRendererAPI()->VInputSetup(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, m_pCurrentPass->GetInputLayout());

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

		g_pApp->GetRendererAPI()->VDrawMesh(
			m_pCurrentPass->GetVertexSize(), m_pVertexBuffers[i], 0, m_pIndexBuffers[i], m_IndexCounts[i], m_pCurrentPass->GetEffectPass());
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

	g_pApp->GetRendererAPI()->VInputSetup(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, m_pCurrentPass->GetInputLayout());
	g_pApp->GetRendererAPI()->VDrawMesh(
		m_pCurrentPass->GetVertexSize(), m_pVertexBuffer, 0, m_pIndexBuffer, m_IndexCount, m_pCurrentPass->GetEffectPass());

	return S_OK;
}


HRESULT SkyboxNode::VOnUpdate(Scene* pScene, const GameTime& gameTime)
{
	return S_OK;
}
