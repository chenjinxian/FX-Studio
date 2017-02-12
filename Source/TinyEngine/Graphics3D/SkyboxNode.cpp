#include "SkyboxNode.h"
#include "Scene.h"
#include "CameraNode.h"
#include "../Actors/RenderComponent.h"
#include "../ResourceCache/MaterialResource.h"
#include "../ResourceCache/TextureResource.h"
#include "../AppFramework/BaseGameApp.h"

SkyboxNode::SkyboxNode(ActorId actorId, WeakBaseRenderComponentPtr renderComponent, RenderPass renderPass)
	: SceneNode(actorId, renderComponent, renderPass),
	m_pEffect(nullptr),
	m_pCurrentPass(nullptr),
	m_pVertexBuffer(nullptr),
	m_pIndexBuffer(nullptr),
	m_IndexCount(0),
	m_ScaleMatrix(Matrix::CreateScale(10000.0f))
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

	std::vector<VertexPositionNormalTexture> vertices;
	std::vector<uint16_t> indices;
	GeometricPrimitive::CreateSphere(vertices, indices);

	std::vector<Vector3> points;
	points.reserve(vertices.size());
	for (auto vertex : vertices)
	{
		points.push_back(vertex.position);
	}

	m_pCurrentPass->CreateVertexBuffer(&points.front(), points.size() * sizeof(Vector3), &m_pVertexBuffer);
	m_pCurrentPass->CreateIndexBuffer(&indices.front(), indices.size() * sizeof(uint16_t), &m_pIndexBuffer);
	m_IndexCount = indices.size();
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
	pScene->GetRenderder()->VSetIndexBuffer(m_pIndexBuffer, IRenderer::Format_uint16, 0);
	pScene->GetRenderder()->VDrawMesh(m_IndexCount, 0, 0, m_pCurrentPass->GetEffectPass());

	return S_OK;
}


HRESULT SkyboxNode::VOnUpdate(Scene* pScene, const GameTime& gameTime)
{
	return S_OK;
}
