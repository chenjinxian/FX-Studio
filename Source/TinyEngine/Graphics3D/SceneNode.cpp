#include "SceneNode.h"
#include "CameraNode.h"
#include "Scene.h"
#include "Model.h"
#include "Material.h"
#include "../Actors/Actor.h"
#include "../Actors/RenderComponent.h"
#include "../Actors/TransformComponent.h"
#include "../ResourceCache/ResCache.h"
#include "../ResourceCache/TextureResource.h"
#include "../ResourceCache/MaterialResource.h"
#include "../ResourceCache/XmlResource.h"
#include "../AppFramework/BaseGameApp.h"
#include "../AppFramework/BaseGameLogic.h"
#include "boost/lexical_cast.hpp"
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

	BaseGameLogic* pGameLogic = g_pApp->GetGameLogic();
	if (pGameLogic == nullptr)
		return;

	StrongActorPtr pActor = MakeStrongPtr(pGameLogic->VGetActor(m_Properties.GetActorId()));
	if (pActor != nullptr)
	{
		shared_ptr<TransformComponent> pTransform = MakeStrongPtr(pActor->GetComponent<TransformComponent>(TransformComponent::m_Name));
		if (pTransform != nullptr)
		{
			pTransform->SetTransform(worldMatrix);
		}
	}
}

HRESULT SceneNode::VOnInitSceneNode(Scene* pScene)
{
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
		if (pTransform != nullptr)
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

void SceneNode::SetBoundingBox(const BoundingBox& aabb)
{
	m_Properties.m_AABox = aabb;
}

void SceneNode::SetBoundingSphere(const BoundingSphere& sphere)
{
	m_Properties.m_Sphere = sphere;
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
	m_VertexCount(0),
	m_GridSize(10.0f),
	m_TicksInterval(0.1f)
{
	GridRenderComponent* pGridRender = static_cast<GridRenderComponent*>(m_pRenderComponent);
	if (pGridRender != nullptr)
	{
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

	uint32_t ticksCount = m_GridSize / m_TicksInterval;
	m_VertexCount = (ticksCount * 2 + 1) * 4;
	std::vector<VertexPositionColor> vertices;
	vertices.reserve(m_VertexCount);

	// X-Axes line
	vertices.push_back(VertexPositionColor(Vector4(-m_GridSize, 0, 0, 1.0f), Color(1.0f, 0.0f, 0.0f)));
	vertices.push_back(VertexPositionColor(Vector4(m_GridSize, 0, 0, 1.0f), Color(1.0f, 0.0f, 0.0f)));

	// Z-Axes line
	vertices.push_back(VertexPositionColor(Vector4(0, 0, -m_GridSize, 1.0f), Color(0.0f, 0.0f, 1.0f)));
	vertices.push_back(VertexPositionColor(Vector4(0, 0, m_GridSize, 1.0f), Color(0.0f, 0.0f, 1.0f)));

	Color color(0.961f, 0.871f, 0.702f, 1.0f);
	for (uint32_t i = 1; i <= ticksCount; i++)
	{
		// Vertical line
		vertices.push_back(VertexPositionColor(Vector4(-m_TicksInterval * i, 0, -m_GridSize, 1.0f), color));
		vertices.push_back(VertexPositionColor(Vector4(-m_TicksInterval * i, 0, m_GridSize, 1.0f), color));
		vertices.push_back(VertexPositionColor(Vector4(m_TicksInterval * i, 0, -m_GridSize, 1.0f), color));
		vertices.push_back(VertexPositionColor(Vector4(m_TicksInterval * i, 0, m_GridSize, 1.0f), color));

		// Horizontal line
		vertices.push_back(VertexPositionColor(Vector4(-m_GridSize, 0, m_TicksInterval * i, 1.0f), color));
		vertices.push_back(VertexPositionColor(Vector4(m_GridSize, 0, m_TicksInterval * i, 1.0f), color));
		vertices.push_back(VertexPositionColor(Vector4(-m_GridSize, 0, -m_TicksInterval * i, 1.0f), color));
		vertices.push_back(VertexPositionColor(Vector4(m_GridSize, 0, -m_TicksInterval * i, 1.0f), color));
	}

	m_pCurrentPass->CreateVertexBuffer(&vertices.front(), vertices.size() * sizeof(VertexPositionColor), &m_pVertexBuffer);
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
	}

	pScene->GetRenderder()->VInputSetup(D3D11_PRIMITIVE_TOPOLOGY_LINELIST, m_pCurrentPass->GetInputLayout());
	uint32_t stride = m_pCurrentPass->GetVertexSize();
	uint32_t offset = 0;
	pScene->GetRenderder()->VSetVertexBuffers(m_pVertexBuffer, &stride, &offset);
	pScene->GetRenderder()->VDrawMesh(0, 0, m_VertexCount, m_pCurrentPass->GetEffectPass());

	return S_OK;
}

HRESULT GridNode::VOnUpdate(Scene* pScene, const GameTime& gameTime)
{
	return S_OK;
}

const std::string GeometryNode::m_Sphere = "Sphere";
const std::string GeometryNode::m_Torus = "Torus";
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
	if (actorType == m_Sphere)
	{
		CreateSphere();
	}
	else if (actorType == m_Torus)
	{
		CreateTorus();
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

	SetBoundingBox(m_Mesh->GetBoundingBox());
}

GeometryNode::~GeometryNode()
{
	VOnDeleteSceneNode(nullptr);
}

HRESULT GeometryNode::VOnInitSceneNode(Scene* pScene)
{
	VOnDeleteSceneNode(pScene);

	GeometryRenderComponent* pGeometryRender = static_cast<GeometryRenderComponent*>(m_pRenderComponent);
	if (pGeometryRender != nullptr)
	{
		m_MaterialName = pGeometryRender->GetMaterialName();
	}

	const tinyxml2::XMLElement* rootNode = nullptr;

	Resource materialRes(m_MaterialName);
	shared_ptr<ResHandle> pMaterialResHandle = g_pApp->GetResCache()->GetHandle(&materialRes);
	if (pMaterialResHandle != nullptr)
	{
		shared_ptr<XmlResourceExtraData> extra = static_pointer_cast<XmlResourceExtraData>(pMaterialResHandle->GetExtraData());
		if (extra != nullptr)
		{
			rootNode = extra->GetRoot();
		}
	}

	if (nullptr == rootNode)
	{
		DEBUG_ERROR("Material is not exist or valid: " + m_MaterialName);
		return S_FALSE;
	}

	std::string effectName = rootNode->Attribute("object");
	Resource effectRes(effectName);
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
		DEBUG_ERROR("effect is not exist or valid: " + effectName);
		return S_FALSE;
	}

	const tinyxml2::XMLElement* pTechniques = rootNode->FirstChildElement("Techniques");
	if (pTechniques == nullptr)
	{
		DEBUG_ERROR("effect xml is invalid");
	}

	for (const tinyxml2::XMLElement* pNode = pTechniques->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
	{
		if (pNode->BoolAttribute("checked"))
		{
			std::string techniqueName = pNode->Attribute("name");
			Technique* pCurrentTechnique = m_pEffect->GetTechniquesByName().at(techniqueName);
			if (pCurrentTechnique == nullptr)
			{
				DEBUG_ERROR("technique is not exist: " + techniqueName);
			}

			m_pCurrentPass = pCurrentTechnique->GetPasses().at(0);
			if (m_pCurrentPass == nullptr)
			{
				DEBUG_ERROR("technique is not exist: " + techniqueName);
			}

			m_pCurrentPass->CreateVertexBuffer(m_Mesh.get(), &m_pVertexBuffer);
			m_pCurrentPass->CreateIndexBuffer(m_Mesh.get(), &m_pIndexBuffer);
			return S_OK;
		}
	}

	return S_FALSE;
}

HRESULT GeometryNode::VOnDeleteSceneNode(Scene *pScene)
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);

	return S_OK;
}

HRESULT GeometryNode::VOnUpdate(Scene* pScene, const GameTime& gameTime)
{
	return S_OK;
}

HRESULT GeometryNode::VRender(Scene* pScene, const GameTime& gameTime)
{
	const tinyxml2::XMLElement* rootNode = nullptr;

	Resource materialRes(m_MaterialName);
	shared_ptr<ResHandle> pMaterialResHandle = g_pApp->GetResCache()->GetHandle(&materialRes);
	if (pMaterialResHandle != nullptr)
	{
		shared_ptr<XmlResourceExtraData> extra = static_pointer_cast<XmlResourceExtraData>(pMaterialResHandle->GetExtraData());
		if (extra != nullptr)
		{
			rootNode = extra->GetRoot();
		}
	}
	if (nullptr == rootNode)
		return S_FALSE;

	const tinyxml2::XMLElement* pVariables = rootNode->FirstChildElement("Variables");
	DEBUG_ASSERT(pVariables != nullptr);

	const std::map<std::string, Variable*>& variables = m_pEffect->GetVariablesByName();
	for (const tinyxml2::XMLElement* pNode = pVariables->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
	{
		Variable* variable = variables.at(pNode->Name());
		std::string semantic = variable->GetVariableSemantic();
		std::string type = variable->GetVariableType();

		if (semantic == "worldviewprojection")
		{
			if (type == "float4x4")
			{
				const Matrix& wvp = pScene->GetCamera()->GetWorldViewProjection(pScene);
				variable->SetMatrix(wvp);
			}
		}
		else if (semantic == "worldinversetranspose")
		{
			if (type == "float4x4")
			{
				Matrix& worldIT = pScene->GetTopMatrix().Invert().Transpose();
				variable->SetMatrix(worldIT);
			}
		}
		else if (semantic == "world")
		{
			if (type == "float4x4")
			{
				const Matrix& world = pScene->GetTopMatrix();
				variable->SetMatrix(world);
			}
		}
		else if (semantic == "viewinverse")
		{
			if (type == "float4x4")
			{
				Matrix& viewI = pScene->GetCamera()->GetViewMatrix().Invert();
				variable->SetMatrix(viewI);
			}
		}
		else if (semantic == "time")
		{
			variable->SetFloat(gameTime.GetTotalTime());
		}
		else if (type == "float4")
		{
			std::stringstream ss(pNode->GetText());
			Vector4 value;
			ss >> value.x >> value.y >> value.z >> value.w;
			variable->SetVector(value);
		}
		else if (type == "float3")
		{
			std::stringstream ss(pNode->GetText());
			Vector3 value;
			ss >> value.x >> value.y >> value.z;
			variable->SetVector(value);
		}
		else if (type == "float2")
		{
			std::stringstream ss(pNode->GetText());
			Vector2 value;
			ss >> value.x >> value.y;
			variable->SetVector(value);
		}
		else if (type == "float")
		{
			switch (variable->GetElementsCount())
			{
			case 0:
			{
				variable->SetFloat(boost::lexical_cast<float>(pNode->GetText()));
				break;
			}
			case 2:
			{
				std::stringstream ss(pNode->GetText());
				std::vector<float> value(2);
				ss >> value[0] >> value[1];
				variable->SetFloatArray(value);
				break;
			}
			case 3:
			{
				std::stringstream ss(pNode->GetText());
				std::vector<float> value(3);
				ss >> value[0] >> value[1] >> value[2];
				variable->SetFloatArray(value);
				break;
			}
			case 4:
			{
				std::stringstream ss(pNode->GetText());
				std::vector<float> value(4);
				ss >> value[0] >> value[1] >> value[2] >> value[3];
				variable->SetFloatArray(value);
				break;
			}
			default:
				break;
			}
		}
		else if (type == "int")
		{
			variable->SetInt(boost::lexical_cast<int>(pNode->GetText()));
		}
		else if (type == "Texture1D" || type == "Texture2D" || type == "Texture3D" || type == "TextureCube")
		{
			const char* resourName = pNode->Attribute("resourcename");
			if (resourName != nullptr)
			{
				std::string textureName = std::string("Textures\\") + resourName;
				Resource resource(textureName);
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
	}

	if (m_pCurrentPass->HasHullShader())
	{
		// D3D_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST start from 33 (32+1)
		pScene->GetRenderder()->VInputSetup(
			(D3D_PRIMITIVE_TOPOLOGY)(m_pCurrentPass->GetTessellationPrimitive() + 32), m_pCurrentPass->GetInputLayout());
	}
	else
	{
		pScene->GetRenderder()->VInputSetup(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, m_pCurrentPass->GetInputLayout());
	}
	uint32_t stride = m_pCurrentPass->GetVertexSize();
	uint32_t offset = 0;
	pScene->GetRenderder()->VSetVertexBuffers(m_pVertexBuffer, &stride, &offset);
	pScene->GetRenderder()->VSetIndexBuffer(m_pIndexBuffer, IRenderer::Format_uint32, 0);
	pScene->GetRenderder()->VDrawMesh(m_IndexCount, 0, 0, m_pCurrentPass->GetEffectPass());

	pScene->GetRenderder()->VResetShader(
		m_pCurrentPass->HasGeometryShader(), m_pCurrentPass->HasHullShader(), m_pCurrentPass->HasDomainShader());
	return S_OK;
}

void GeometryNode::VPick(Scene* pScene, int cursorX, int cursorY)
{
	const Matrix& projectMat = pScene->GetCamera()->GetProjectMatrix();
	float viewX = (2.0f * cursorX / g_pApp->GetGameConfig().m_ScreenWidth - 1.0f) / projectMat.m[0][0];
	float viewY = (1.0f - 2.0f * cursorY / g_pApp->GetGameConfig().m_ScreenHeight) / projectMat.m[1][1];

	Matrix toLocal = (m_Properties.GetWorldMatrix() * pScene->GetCamera()->GetViewMatrix()).Invert();
	Vector3 rayPos = toLocal.Translation();
	//use right-hand coordinates, z should be -1
	Vector3 rayDir = Vector3::TransformNormal(Vector3(viewX, viewY, -1.0f), toLocal);
	rayDir.Normalize();

	Ray ray(rayPos, rayDir);

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
					pScene->SetPickedActor(m_Properties.GetActorId(), 0);
					break;
				}
			}
		}
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
		GeometricPrimitive::CreateGeoSphere(vertices, indices, diameter, tessellation, useRHcoords);
		m_IndexCount = indices.size();
		m_Mesh = unique_ptr<Mesh>(DEBUG_NEW Mesh(vertices, indices));
	}
}

void GeometryNode::CreateTorus()
{
	TorusRenderComponent* pMeshRender = static_cast<TorusRenderComponent*>(m_pRenderComponent);
	if (pMeshRender != nullptr)
	{
		float thickness = pMeshRender->GetThickness();
		float diameter = pMeshRender->GetDiameter();
		uint32_t tessellation = pMeshRender->GetTessellation();
		bool useRHcoords = pMeshRender->UseRHcoords();

		std::vector<VertexPositionNormalTexture> vertices;
		std::vector<uint16_t> indices;
		GeometricPrimitive::CreateTorus(vertices, indices, diameter, thickness, tessellation, useRHcoords);
		m_IndexCount = indices.size();
		m_Mesh = unique_ptr<Mesh>(DEBUG_NEW Mesh(vertices, indices));
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
		m_Mesh = unique_ptr<Mesh>(DEBUG_NEW Mesh(vertices, indices));
	}
}

void GeometryNode::CreatePlane()
{
	PlaneRenderComponent* pMeshRender = static_cast<PlaneRenderComponent*>(m_pRenderComponent);
	if (pMeshRender != nullptr)
	{
		float size = pMeshRender->GetSize() / 2.0f;
		std::vector<VertexPositionNormalTexture> vertices(4);
		vertices[0] = VertexPositionNormalTexture(XMFLOAT3(-size, 0.0f, size), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f));
		vertices[1] = VertexPositionNormalTexture(XMFLOAT3(-size, 0.0f, -size), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f));
		vertices[2] = VertexPositionNormalTexture(XMFLOAT3(size, 0.0f, -size), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f));
		vertices[3] = VertexPositionNormalTexture(XMFLOAT3(size, 0.0f, size), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f));
		std::vector<uint16_t> indices = {0, 1, 2, 0, 2, 3 };
		m_IndexCount = indices.size();
		m_Mesh = unique_ptr<Mesh>(DEBUG_NEW Mesh(vertices, indices));
	}
}
