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

// 	SetRadius(CalcBoundingSphere(&extra->m_Mesh11));

	return S_OK;
}

HRESULT D3DShaderMeshNode11::VRender(Scene *pScene)
{
// 	HRESULT hr;
// 
// 	V_RETURN (m_VertexShader.SetupRender(pScene, this) );
// 	V_RETURN (m_PixelShader.SetupRender(pScene, this) );

	Resource resource(m_MeshFileName);
	shared_ptr<ResHandle> pResourceHandle = g_pApp->m_pResCache->GetHandle(&resource);  	
	shared_ptr<D3DSdkMeshResourceExtraData11> extra = static_pointer_cast<D3DSdkMeshResourceExtraData11>(pResourceHandle->GetExtra());

	UINT Strides[1];
	UINT Offsets[1];
	ID3D11Buffer* pVB[1];
	pVB[0] = extra->m_Mesh11.GetVB11( 0, 0 );
	Strides[0] = ( UINT )extra->m_Mesh11.GetVertexStride( 0, 0 );
	Offsets[0] = 0;
	DXUTGetD3D11DeviceContext()->IASetVertexBuffers( 0, 1, pVB, Strides, Offsets );
	DXUTGetD3D11DeviceContext()->IASetIndexBuffer( extra->m_Mesh11.GetIB11( 0 ), extra->m_Mesh11.GetIBFormat11( 0 ), 0 );

	D3D11_PRIMITIVE_TOPOLOGY PrimType;
	for( UINT subset = 0; subset < extra->m_Mesh11.GetNumSubsets( 0 ); ++subset )
	{
		SDKMESH_SUBSET *pSubset = extra->m_Mesh11.GetSubset( 0, subset );

		PrimType = CDXUTSDKMesh::GetPrimitiveType11( ( SDKMESH_PRIMITIVE_TYPE )pSubset->PrimitiveType );
		DXUTGetD3D11DeviceContext()->IASetPrimitiveTopology( PrimType );
 
		ID3D11ShaderResourceView* pDiffuseRV = extra->m_Mesh11.GetMaterial( pSubset->MaterialID )->pDiffuseRV11;
		DXUTGetD3D11DeviceContext()->PSSetShaderResources( 0, 1, &pDiffuseRV );

		DXUTGetD3D11DeviceContext()->DrawIndexed( ( UINT )pSubset->IndexCount, 0, ( UINT )pSubset->VertexStart );
	}
	return S_OK;
}

float D3DShaderMeshNode11::CalcBoundingSphere(CDXUTSDKMesh *mesh11)
{
	float radius = 0.0f;
	for( UINT subset = 0; subset < mesh11->GetNumSubsets( 0 ); ++subset )
	{
		Vector3 extents = mesh11->GetMeshBBoxExtents(subset);
		extents.x = abs(extents.x);
		extents.y = abs(extents.y);
		extents.z = abs(extents.z);
		radius = (radius > extents.x) ? radius : extents.x;
		radius = (radius > extents.y) ? radius : extents.y;
		radius = (radius > extents.z) ? radius : extents.z;
	}
	return radius;
}
