#include "Skybox.h"
#include "Scene.h"
#include "CameraNode.h"
#include "ModelImporter.h"
#include "../Actors/RenderComponent.h"
#include "../ResourceCache/ResCache.h"
#include "../ResourceCache/TextureResource.h"
#include "../ResourceCache/ShaderResource.h"
#include "../AppFramework/BaseGameApp.h"

SkyboxNode::SkyboxNode(ActorId actorId, WeakBaseRenderComponentPtr renderComponent, RenderPass renderPass)
	: SceneNode(actorId, renderComponent, renderPass),
	m_pEffect(nullptr),
	m_pCurrentPass(nullptr),
	m_pVertexBuffer(nullptr),
	m_pIndexBuffer(nullptr),
	m_IndexCount(0),
	m_pTextures(),
	m_ScaleMatrix(Matrix::CreateScale(100.0f))
{
	SkyboxRenderComponent* pSkyboxRender = static_cast<SkyboxRenderComponent*>(m_pRenderComponent);
	if (pSkyboxRender != nullptr)
	{
		m_TextureNames = pSkyboxRender->GetTextureName();
		m_EffectName = pSkyboxRender->GetEffectName();
		m_CurrentTechnique = pSkyboxRender->GetCurrentTechniqueName();
		m_CurrentPass = pSkyboxRender->GetCurrentPassName();
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

	Resource modelRes("Models\\Sphere.obj");
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

	Mesh* mesh = model->GetMeshes().at(0);
	m_pCurrentPass->CreateVertexBuffer(mesh, &m_pVertexBuffer);
	m_pCurrentPass->CreateIndexBuffer(mesh, &m_pIndexBuffer);
	m_IndexCount = mesh->GetIndices().size();

	m_pTextures.reserve(m_TextureNames.size());
	for (auto& texture : m_TextureNames)
	{
		Resource resource(texture);
		shared_ptr<ResHandle> pTextureRes = g_pApp->GetResCache()->GetHandle(&resource);
		if (pTextureRes != nullptr)
		{
			shared_ptr<D3D11TextureResourceExtraData> extra =
				static_pointer_cast<D3D11TextureResourceExtraData>(pTextureRes->GetExtraData());
			if (extra != nullptr)
			{
				m_pTextures.push_back(extra->GetTexture());
			}
		}
	}
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
		const std::string& semantic = variable->GetVariableSemantic();
		if (semantic == "worldviewprojection")
		{
			if (variable->GetVariableType() == "float4x4")
			{
				const XMMATRIX& wvp = pScene->GetCamera()->GetWorldViewProjection(pScene);
				variable->SetMatrix(wvp);
			}
		}
	}

	g_pApp->GetRendererAPI()->VInputSetup(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, m_pCurrentPass->GetInputLayout());

	for (auto variable : variables)
	{
		if (variable->GetVariableType() == "TextureCube")
		{
			variable->SetResource(m_pTextures[0]);
			break;
		}
	}

	g_pApp->GetRendererAPI()->VDrawMeshe(
		m_pCurrentPass->GetVertexSize(), m_pVertexBuffer, m_pIndexBuffer, m_IndexCount, m_pCurrentPass->GetEffectPass());

	return S_OK;
}


HRESULT SkyboxNode::VOnUpdate(Scene* pScene, const GameTime& gameTime)
{
	return S_OK;
}
