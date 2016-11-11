#include "D3DSdkMesh.h"
#include "SceneNode.h"
#include "CameraNode.h"
#include "D3DRenderer.h"
#include "TextureResourceLoader.h"
#include "../Actors/RenderComponent.h"
#include "../ResourceCache/ResCache.h"
#include "../AppFramework/RenderEngineApp.h"
#include <SDKmisc.h>


shared_ptr<IResourceLoader> CreateSdkMeshResourceLoader()
{
	return shared_ptr<IResourceLoader>(GCC_NEW SdkMeshResourceLoader());
}

uint32_t SdkMeshResourceLoader::VGetLoadedResourceSize(char *rawBuffer, uint32_t rawSize)
{
	return rawSize;
}

bool SdkMeshResourceLoader::VLoadResource(char *rawBuffer, uint32_t rawSize, shared_ptr<ResHandle> handle)
{
	RenderEngineApp::Renderer renderer = RenderEngineApp::GetRendererImpl();
	if (renderer == RenderEngineApp::Renderer_D3D11)
	{
		shared_ptr<D3DSdkMeshResourceExtraData11> extra =
			shared_ptr<D3DSdkMeshResourceExtraData11>(GCC_NEW D3DSdkMeshResourceExtraData11());

		if (SUCCEEDED(extra->m_Mesh11.Create(DXUTGetD3D11Device(), (BYTE*)rawBuffer, rawSize, true, nullptr, false)))
		{
			handle->SetExtra(shared_ptr<D3DSdkMeshResourceExtraData11>(extra));
		}

		return true;
	}

	DEBUG_ASSERT(0 && "Unsupported Renderer in SdkMeshResourceLoader::VLoadResource");
	return false;
}

D3DShaderMeshNode11::D3DShaderMeshNode11(
	ActorId actorId, WeakBaseRenderComponentPtr renderComponent, RenderPass renderPass, const Matrix& mat)
	: SceneNode(actorId, renderComponent, renderPass)
{
	ModelRenderComponent* pMeshRender = static_cast<ModelRenderComponent*>(m_pRenderComponent);
	if (pMeshRender != nullptr)
	{
		m_SdkMeshName = pMeshRender->GetModelName();
		m_TextureName = pMeshRender->GetTextureName();
		m_EffectName = pMeshRender->GetEffectName();
	}

	g_pEffect = nullptr;
	g_pVertexLayout = nullptr;
	g_pTechnique = nullptr;
	g_ptxDiffuseVariable = nullptr;
	g_pWorldVariable = nullptr;
	g_pViewVariable = nullptr;
	g_pProjectionVariable = nullptr;
	g_pWavinessVariable = nullptr;
	g_pTimeVariable = nullptr;

	auto pd3dImmediateContext = DXUTGetD3D11DeviceContext();
	auto pd3dDevice = DXUTGetD3D11Device();

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	dwShaderFlags |= D3DCOMPILE_DEBUG;
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
	Resource resource(m_EffectName);
	shared_ptr<ResHandle> pResourceHandle = g_pApp->m_pResCache->GetHandle(&resource);
	if (pResourceHandle)
	{
		D3DX11CompileEffectFromMemory(
			pResourceHandle->Buffer(), pResourceHandle->Size(), m_EffectName.c_str(),
			nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, dwShaderFlags, 0, pd3dDevice, &g_pEffect, nullptr);
// 		D3DX11CreateEffectFromMemory(pResourceHandle->Buffer(), pResourceHandle->Size(), 0, pd3dDevice, &g_pEffect, m_EffectName.c_str());
	}

	// Obtain the technique
	g_pTechnique = g_pEffect->GetTechniqueByName("Render");

	// Obtain the variables
	g_ptxDiffuseVariable = g_pEffect->GetVariableByName("g_txDiffuse")->AsShaderResource();
	g_pWorldVariable = g_pEffect->GetVariableByName("World")->AsMatrix();
	g_pViewVariable = g_pEffect->GetVariableByName("View")->AsMatrix();
	g_pProjectionVariable = g_pEffect->GetVariableByName("Projection")->AsMatrix();
	g_pWavinessVariable = g_pEffect->GetVariableByName("Waviness")->AsScalar();
	g_pTimeVariable = g_pEffect->GetVariableByName("Time")->AsScalar();


	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	// Create the input layout
	D3DX11_PASS_DESC PassDesc;
	g_pTechnique->GetPassByIndex(0)->GetDesc(&PassDesc);
	pd3dDevice->CreateInputLayout(layout, numElements, PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize, &g_pVertexLayout);

	pd3dImmediateContext->IASetInputLayout(g_pVertexLayout);

	g_World = Matrix::Identity;
}

D3DShaderMeshNode11::~D3DShaderMeshNode11()
{

}

HRESULT D3DShaderMeshNode11::VOnRestore(Scene *pScene)
{
	HRESULT hr;

	V_RETURN(SceneNode::VOnRestore(pScene));

	// 	V_RETURN (m_VertexShader.OnRestore(pScene) );
	// 	V_RETURN (m_PixelShader.OnRestore(pScene) );


	return S_OK;
}

HRESULT D3DShaderMeshNode11::VOnDestoryDevice(Scene* pScene)
{
	SAFE_RELEASE(g_ptxDiffuseVariable);
	SAFE_RELEASE(g_pWorldVariable);
	SAFE_RELEASE(g_pViewVariable);
	SAFE_RELEASE(g_pProjectionVariable);
	SAFE_RELEASE(g_pWavinessVariable);
	SAFE_RELEASE(g_pVertexLayout);
	SAFE_RELEASE(g_pTechnique);
	SAFE_RELEASE(g_pEffect);
	
	return S_OK;
}

HRESULT D3DShaderMeshNode11::VOnUpdate(Scene* pScene, double fTime, float fElapsedTime)
{
	g_World = Matrix::CreateRotationX(XMConvertToRadians(-90.0f)) *
		Matrix::CreateRotationY(60.0f * XMConvertToRadians((float)fTime));

	return S_OK;
}

HRESULT D3DShaderMeshNode11::VRender(Scene *pScene, double fTime, float fElapsedTime)
{
	auto pRTV = DXUTGetD3D11RenderTargetView();
	auto pd3dImmediateContext = DXUTGetD3D11DeviceContext();
	XMMATRIX mView = pScene->GetCamera()->GetViewMatrix();
	XMMATRIX mProj = pScene->GetCamera()->GetProjectMatrix();
	XMMATRIX mWorldViewProjection = g_World * mView * mProj;

	//
	// Update variables that change once per frame
	//
	g_pProjectionVariable->SetMatrix((float*)&mProj);
	g_pViewVariable->SetMatrix((float*)&mView);
	g_pWorldVariable->SetMatrix((float*)&g_World);
	g_pTimeVariable->SetFloat((float)fTime);

	//
	// Set the Vertex Layout
	//
	pd3dImmediateContext->IASetInputLayout(g_pVertexLayout);


	Resource resource(m_SdkMeshName);
	shared_ptr<ResHandle> pResourceHandle = g_pApp->m_pResCache->GetHandle(&resource);
	if (pResourceHandle == nullptr)
	{
		return S_FALSE;
	}
	shared_ptr<D3DSdkMeshResourceExtraData11> extra = static_pointer_cast<D3DSdkMeshResourceExtraData11>(pResourceHandle->GetExtra());
	if (extra == nullptr)
	{
		return S_FALSE;
	}

	UINT Strides[1];
	UINT Offsets[1];
	ID3D11Buffer* pVB[1];
	pVB[0] = extra->m_Mesh11.GetVB11(0, 0);
	Strides[0] = (UINT)extra->m_Mesh11.GetVertexStride(0, 0);
	Offsets[0] = 0;
	pd3dImmediateContext->IASetVertexBuffers(0, 1, pVB, Strides, Offsets);
	pd3dImmediateContext->IASetIndexBuffer(extra->m_Mesh11.GetIB11(0), extra->m_Mesh11.GetIBFormat11(0), 0);

	D3DX11_TECHNIQUE_DESC techDesc;
	HRESULT hr;
	VTrace(g_pTechnique->GetDesc(&techDesc));

	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		for (UINT subset = 0; subset < extra->m_Mesh11.GetNumSubsets(0); ++subset)
		{
			auto pSubset = extra->m_Mesh11.GetSubset(0, subset);

			auto PrimType = extra->m_Mesh11.GetPrimitiveType11((SDKMESH_PRIMITIVE_TYPE)pSubset->PrimitiveType);
			pd3dImmediateContext->IASetPrimitiveTopology(PrimType);

			Resource resource(m_TextureName);
			shared_ptr<ResHandle> texture = g_pApp->m_pResCache->GetHandle(&resource);
			if (texture != nullptr)
			{
				shared_ptr<D3DTextureResourceExtraData11> extra = static_pointer_cast<D3DTextureResourceExtraData11>(texture->GetExtra());
				if (extra != nullptr)
				{
					g_ptxDiffuseVariable->SetResource(extra->GetTexture());
				}
			}

			g_pTechnique->GetPassByIndex(p)->Apply(0, pd3dImmediateContext);
			pd3dImmediateContext->DrawIndexed((UINT)pSubset->IndexCount, 0, (UINT)pSubset->VertexStart);
		}
	}

	return S_OK;
}
