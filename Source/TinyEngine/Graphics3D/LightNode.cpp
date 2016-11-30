#include "LightNode.h"
#include "CameraNode.h"
#include "BasicMaterial.h"
#include "ModelImporter.h"
#include "../Actors/RenderComponent.h"
#include "../ResourceCache/ResCache.h"
#include "../AppFramework/BaseGameApp.h"

LightNode::LightNode(
	ActorId actorId, WeakBaseRenderComponentPtr renderComponent, RenderPass renderPass, const Matrix& mat)
	: SceneNode(actorId, renderComponent, renderPass),
	m_pEffect(nullptr),
	m_pMaterial(nullptr),
	m_pVertexBuffer(nullptr),
	m_pIndexBuffer(nullptr),
	m_WorldMatrix(Matrix::Identity),
	m_ScaleMatrix(Matrix::Identity)
{
	LightRenderComponent* pModelRender = static_cast<LightRenderComponent*>(renderComponent);
	if (pModelRender != nullptr)
	{
		m_ModelName = pModelRender->GetModelName();
	}

	Resource modelRes(m_ModelName);
	shared_ptr<ResHandle> pModelResHandle = g_pApp->GetResCache()->GetHandle(&modelRes);
	std::unique_ptr<Model> model(new Model(pModelResHandle->Buffer(), pModelResHandle->Size(), true));

// 	Resource effectRes(m_EffectName);
// 	shared_ptr<ResHandle> pEffectResHandle = g_pApp->GetResCache()->GetHandle(&effectRes);
// 	m_pEffect = new Effect();
// 	m_pEffect->CompileFromMemory(pEffectResHandle->Buffer(), pEffectResHandle->Size());
	m_pEffect = DEBUG_NEW Effect();
	m_pEffect->LoadCompiledEffect(L"Assets\\Effects\\BasicEffect.cso");

	m_pMaterial = DEBUG_NEW BasicMaterial();
	m_pMaterial->Initialize(m_pEffect);

	Mesh* mesh = model->GetMeshes().at(0);
	m_pMaterial->CreateVertexBuffer(mesh, &m_pVertexBuffer);
	mesh->CreateIndexBuffer(&m_pIndexBuffer);
	m_IndexCount = mesh->GetIndices().size();
}

LightNode::~LightNode()
{
	SAFE_DELETE(m_pMaterial);
	SAFE_DELETE(m_pEffect);
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
}

HRESULT LightNode::VOnInitSceneNode(Scene* pScene)
{
	return S_OK;
}

HRESULT LightNode::VOnDeleteSceneNode(Scene *pScene)
{
	return S_OK;
}

HRESULT LightNode::VRender(Scene* pScene, const GameTime& gameTime)
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
// 	XMMATRIX wvp = pScene->GetCamera()->GetWorldViewProjection(pScene);
// 	m_pMaterial->GetWorldViewProjection() << wvp;
// 
// 	pass->Apply(0);
// 
// 	if (mDisplayWireframe)
// 	{
// 		mGame->Direct3DDeviceContext()->RSSetState(RasterizerStates::Wireframe);
// 		direct3DDeviceContext->DrawIndexed(mIndexCount, 0, 0);
// 		mGame->Direct3DDeviceContext()->RSSetState(nullptr);
// 	}
// 	direct3DDeviceContext->DrawIndexed(m_IndexCount, 0, 0);

	return S_OK;
}

DirectionalLightNode::DirectionalLightNode(
	ActorId actorId, WeakBaseRenderComponentPtr renderComponent, RenderPass renderPass, const Matrix& mat)
	: LightNode(actorId, renderComponent, renderPass, mat)
{

}

DirectionalLightNode::~DirectionalLightNode()
{

}

HRESULT DirectionalLightNode::VOnUpdate(Scene* pScene, const GameTime& gameTime)
{
	return S_OK;
}

PointLightNode::PointLightNode(
	ActorId actorId, WeakBaseRenderComponentPtr renderComponent, RenderPass renderPass, const Matrix& mat)
	: LightNode(actorId, renderComponent, renderPass, mat)
{

}

PointLightNode::~PointLightNode()
{

}

HRESULT PointLightNode::VOnUpdate(Scene* pScene, const GameTime& gameTime)
{
	return S_OK;
}

SpotLightNode::SpotLightNode(
	ActorId actorId, WeakBaseRenderComponentPtr renderComponent, RenderPass renderPass, const Matrix& mat)
	: DirectionalLightNode(actorId, renderComponent, renderPass, mat),
	PointLightNode(actorId, renderComponent, renderPass, mat),
	LightNode(actorId, renderComponent, renderPass, mat)
{

}

SpotLightNode::~SpotLightNode()
{

}

HRESULT SpotLightNode::VOnUpdate(Scene* pScene, const GameTime& gameTime)
{
	return S_OK;
}
