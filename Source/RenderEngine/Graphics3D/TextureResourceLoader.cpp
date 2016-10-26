#include "TextureResourceLoader.h"
#include "../AppFramework/RenderEngineApp.h"

class DdsResourceLoader : public TextureResourceLoader
{
public:
	virtual std::string VGetPattern() { return "*.dds"; }
};

shared_ptr<IResourceLoader> CreateDDSResourceLoader()
{
	return shared_ptr<IResourceLoader>(GCC_NEW DdsResourceLoader());
}

class JpgResourceLoader : public TextureResourceLoader
{
public:
	virtual std::string VGetPattern() { return "*.jpg"; }
};

shared_ptr<IResourceLoader> CreateJPGResourceLoader()
{
	return shared_ptr<IResourceLoader>(GCC_NEW JpgResourceLoader());
}

D3DTextureResourceExtraData11::D3DTextureResourceExtraData11()
: m_pTexture(NULL), m_pSamplerLinear(NULL)
{	
}

unsigned int TextureResourceLoader::VGetLoadedResourceSize(char *rawBuffer, unsigned int rawSize)
{
	return 0;
}

bool TextureResourceLoader::VLoadResource(char *rawBuffer, unsigned int rawSize, shared_ptr<ResHandle> handle)
{
	RenderEngineApp::Renderer renderer = RenderEngineApp::GetRendererImpl();
	if (renderer == RenderEngineApp::Renderer_D3D11)
	{
		shared_ptr<D3DTextureResourceExtraData11> extra = shared_ptr<D3DTextureResourceExtraData11>(GCC_NEW D3DTextureResourceExtraData11());

		CreateDDSTextureFromMemory();
		CreateWICTextureFromMemory()
// 		if ( FAILED (D3DX11CreateShaderResourceViewFromMemory( DXUTGetD3D11Device(), rawBuffer, rawSize, NULL, NULL, &extra->m_pTexture, NULL ) ) )
// 			return false;

		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory( &sampDesc, sizeof(sampDesc) );
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
		if( FAILED( DXUTGetD3D11Device()->CreateSamplerState( &sampDesc, &extra->m_pSamplerLinear ) ) ) 
			return false;

		handle->SetExtra(shared_ptr<D3DTextureResourceExtraData11>(extra));
		return true;
	}

	GCC_ASSERT(0 && "Unsupported Renderer in TextureResourceLoader::VLoadResource");
	return false;
}

