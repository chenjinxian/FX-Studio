#include "Skybox.h"
#include "ModelImporter.h"
#include "Scene.h"
#include "CameraNode.h"
#include "DDSTextureLoader.h"

SkyboxNode::SkyboxNode(const std::wstring& textureFile)
	: SceneNode(INVALID_ACTOR_ID, WeakBaseRenderComponentPtr(), RenderPass_Sky),
	m_pEffect(nullptr),
	m_pCubeMapShaderResourceView(nullptr),
	m_pVertexBuffer(nullptr),
	m_pIndexBuffer(nullptr),
	m_WorldMatrix(Matrix::CreateScale(100.0f)),
	m_ScaleMatrix(Matrix::Identity),
	m_TextureFile(textureFile),
	m_IndexCount(0),
	m_IsActive(true)
{
	m_ScaleMatrix = Matrix::CreateScale(100.0f);
}

SkyboxNode::~SkyboxNode()
{
	Reset();
}

HRESULT SkyboxNode::VPreRender(Scene* pScene)
{
	return SceneNode::VPreRender(pScene);
}

bool SkyboxNode::VIsVisible(Scene* pScene)
{
	return m_IsActive;
}

HRESULT SkyboxNode::VOnInitSceneNode(Scene* pScene)
{
// 	Reset();
// 
// 	std::unique_ptr<Model> model(new Model(std::string("Assets\\Models\\Sphere.obj"), true));
// 
// 	m_pEffect = new Effect();
// 	m_pEffect->LoadCompiledEffect(L"Assets\\Effects\\Skybox.cso");
// 
// 	m_pMaterial = new SkyboxMaterial();
// 	m_pMaterial->Initialize(m_pEffect);
// 
// 	Mesh* pMesh = model->GetMeshes().at(0);
// 	m_pMaterial->CreateVertexBuffer(pMesh, &m_pVertexBuffer);
// 	pMesh->CreateIndexBuffer(&m_pIndexBuffer);
// 	m_IndexCount = pMesh->GetIndices().size();
// 
// 	HRESULT hr = CreateDDSTextureFromFileEx(
// 		DXUTGetD3D11Device(), DXUTGetD3D11DeviceContext(), m_TextureFile.c_str(), 0,
// 		D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0, true, nullptr, &m_pCubeMapShaderResourceView);
// 	if (FAILED(hr))
// 	{
// 		DEBUG_ERROR("CreateDDSTextureFromFile() failed.");
// 	}
// 
// 	return hr;

	return S_OK;
}

HRESULT SkyboxNode::VOnDeleteSceneNode(Scene *pScene)
{
	return S_OK;
}

HRESULT SkyboxNode::VRender(Scene* pScene, const GameTime& gameTime)
{
// 	ID3D11DeviceContext* direct3DDeviceContext = DXUTGetD3D11DeviceContext();
// 	direct3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
// 
// 	Pass* pass = m_pMaterial->GetCurrentTechnique()->GetPasses().at(0);
// 	ID3D11InputLayout* inputLayout = m_pMaterial->GetInputLayouts().at(pass);
// 	direct3DDeviceContext->IASetInputLayout(inputLayout);
// 
// 	UINT stride = m_pMaterial->VertexSize();
// 	UINT offset = 0;
// 	direct3DDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
// 	direct3DDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
// 
// 	XMMATRIX wvp = XMMATRIX(m_WorldMatrix * pScene->GetCamera()->GetViewMatrix() * pScene->GetCamera()->GetProjectMatrix());
// 	m_pMaterial->WorldViewProjection() << wvp;
// 	m_pMaterial->SkyboxTexture() << m_pCubeMapShaderResourceView;
// 
// 	pass->Apply(0);
// 
// 	direct3DDeviceContext->DrawIndexed(m_IndexCount, 0, 0);

	return S_OK;
}


HRESULT SkyboxNode::VOnUpdate(Scene* pScene, const GameTime& gameTime)
{
	Vector3 cameraPosition = pScene->GetCamera()->GetPosition();
	m_WorldMatrix.Translation(cameraPosition);
	return S_OK;
}

void SkyboxNode::Reset()
{
	SAFE_RELEASE(m_pCubeMapShaderResourceView);
	SAFE_DELETE(m_pEffect);
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
}
