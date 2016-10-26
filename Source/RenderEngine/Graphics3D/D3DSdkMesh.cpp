#include "D3DSdkMesh.h"
#include "SceneNode.h"
#include "D3DRenderer.h"
#include "../ResourceCache/ResCache.h"
#include "../AppFramework/RenderEngineApp.h"
#include <SDKmisc.h>


shared_ptr<IResourceLoader> CreateSdkMeshResourceLoader()
{
	return shared_ptr<IResourceLoader>(GCC_NEW SdkMeshResourceLoader());
}

unsigned int SdkMeshResourceLoader::VGetLoadedResourceSize(char *rawBuffer, unsigned int rawSize)
{
	return rawSize;
}

bool SdkMeshResourceLoader::VLoadResource(char *rawBuffer, unsigned int rawSize, shared_ptr<ResHandle> handle)
{
	RenderEngineApp::Renderer renderer = RenderEngineApp::GetRendererImpl();
	if (renderer == RenderEngineApp::Renderer_D3D11)
	{
		shared_ptr<D3DSdkMeshResourceExtraData11> extra =
			shared_ptr<D3DSdkMeshResourceExtraData11>(GCC_NEW D3DSdkMeshResourceExtraData11());

		if (SUCCEEDED ( extra->m_Mesh11.Create( DXUTGetD3D11Device(), (BYTE *)rawBuffer, (UINT)rawSize, true ) ) )
		{
			handle->SetExtra(shared_ptr<D3DSdkMeshResourceExtraData11>(extra));
		}

		return true;
	}

	GCC_ASSERT(0 && "Unsupported Renderer in SdkMeshResourceLoader::VLoadResource");
	return false;
}

D3DShaderMeshNode11::D3DShaderMeshNode11(ActorId actorId, WeakBaseRenderComponentPtr renderComponent,
	std::string meshFileName, RenderPass renderPass, const Matrix& mat)
	: SceneNode(actorId, renderComponent, renderPass), m_MeshFileName(meshFileName)
{
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
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;

	// Disable optimizations to further improve shader debugging
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	WCHAR str[MAX_PATH];
	DXUTFindDXSDKMediaFileCch(str, MAX_PATH, L"Tutorial11.fx");

	D3DX11CompileEffectFromFile(str, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, dwShaderFlags, 0, pd3dDevice, &g_pEffect, nullptr);

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

	// Set the input layout
	pd3dImmediateContext->IASetInputLayout(g_pVertexLayout);

	// Initialize the world matrices
	g_World = Matrix::CreateTranslation(1.0f, 1.0f, 800.0f);
}

HRESULT D3DShaderMeshNode11::VOnRestore(Scene *pScene)
{
	HRESULT hr;

	V_RETURN(SceneNode::VOnRestore(pScene) );

// 	V_RETURN (m_VertexShader.OnRestore(pScene) );
// 	V_RETURN (m_PixelShader.OnRestore(pScene) );

	Resource resource(m_MeshFileName);
	shared_ptr<ResHandle> pResourceHandle = g_pApp->m_pResCache->GetHandle(&resource);  	
	shared_ptr<D3DSdkMeshResourceExtraData11> extra = static_pointer_cast<D3DSdkMeshResourceExtraData11>(pResourceHandle->GetExtra());
	g_Mesh = extra->m_Mesh11;

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

	//
	// Render the mesh
	//
	UINT Strides[1];
	UINT Offsets[1];
	ID3D11Buffer* pVB[1];
	pVB[0] = g_Mesh.GetVB11(0, 0);
	Strides[0] = (UINT)g_Mesh.GetVertexStride(0, 0);
	Offsets[0] = 0;
	pd3dImmediateContext->IASetVertexBuffers(0, 1, pVB, Strides, Offsets);
	pd3dImmediateContext->IASetIndexBuffer(g_Mesh.GetIB11(0), g_Mesh.GetIBFormat11(0), 0);

	D3DX11_TECHNIQUE_DESC techDesc;
	HRESULT hr;
	VTrace(g_pTechnique->GetDesc(&techDesc));

	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		for (UINT subset = 0; subset < g_Mesh.GetNumSubsets(0); ++subset)
		{
			auto pSubset = g_Mesh.GetSubset(0, subset);

			auto PrimType = g_Mesh.GetPrimitiveType11((SDKMESH_PRIMITIVE_TYPE)pSubset->PrimitiveType);
			pd3dImmediateContext->IASetPrimitiveTopology(PrimType);

			auto pDiffuseRV = g_Mesh.GetMaterial(pSubset->MaterialID)->pDiffuseRV11;
			g_ptxDiffuseVariable->SetResource(pDiffuseRV);

			g_pTechnique->GetPassByIndex(p)->Apply(0, pd3dImmediateContext);
			pd3dImmediateContext->DrawIndexed((UINT)pSubset->IndexCount, 0, (UINT)pSubset->VertexStart);
		}
	}

	return S_OK;
}
