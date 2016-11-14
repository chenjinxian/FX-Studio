#include "TextureResourceLoader.h"
#include "../AppFramework/BaseGameApp.h"
#include "DDSTextureLoader.h"
#include "WICTextureLoader.h"

D3DTextureResourceExtraData11::D3DTextureResourceExtraData11()
	: m_pTexture(nullptr)
{
}

D3DTextureResourceExtraData11::~D3DTextureResourceExtraData11()
{
	SAFE_RELEASE(m_pTexture);
}

bool DdsResourceLoader::VLoadResource(char *rawBuffer, uint32_t rawSize, shared_ptr<ResHandle> handle)
{
// 	BaseGameApp::Renderer renderer = BaseGameApp::GetRendererImpl();
// 	if (renderer == BaseGameApp::Renderer_D3D11)
// 	{
// 		shared_ptr<D3DTextureResourceExtraData11> extra = shared_ptr<D3DTextureResourceExtraData11>(DEBUG_NEW D3DTextureResourceExtraData11());
// 
// 		if (FAILED(CreateDDSTextureFromMemoryEx(
// 			DXUTGetD3D11Device(), DXUTGetD3D11DeviceContext(), (uint8_t*)rawBuffer, rawSize, 0,
// 			D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0, true, nullptr, &extra->m_pTexture)))
// 			return false;
// 
// 		handle->SetExtra(shared_ptr<D3DTextureResourceExtraData11>(extra));
// 		return true;
// 	}

	DEBUG_ASSERT(0 && "Unsupported Renderer in TextureResourceLoader::VLoadResource");
	return false;
}

bool WicResourceLoader::VLoadResource(char *rawBuffer, uint32_t rawSize, shared_ptr<ResHandle> handle)
{
// 	BaseGameApp::Renderer renderer = BaseGameApp::GetRendererImpl();
// 	if (renderer == BaseGameApp::Renderer_D3D11)
// 	{
// 		shared_ptr<D3DTextureResourceExtraData11> extra =
// 			shared_ptr<D3DTextureResourceExtraData11>(DEBUG_NEW D3DTextureResourceExtraData11());
// 
// 		if (FAILED(CreateWICTextureFromMemoryEx(
// 			DXUTGetD3D11Device(), DXUTGetD3D11DeviceContext(), (uint8_t*)rawBuffer, rawSize, 0,
// 			D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0, true, nullptr, &extra->m_pTexture)))
// 			return false;
// 
// 		handle->SetExtra(shared_ptr<D3DTextureResourceExtraData11>(extra));
// 		return true;
// 	}

	DEBUG_ASSERT(0 && "Unsupported Renderer in TextureResourceLoader::VLoadResource");
	return false;
}

class JpgResourceLoader : public WicResourceLoader
{
public:
	virtual std::string VGetPattern() { return "*.jpg"; }
};

class PngResourceLoader : public WicResourceLoader
{
public:
	virtual std::string VGetPattern() { return "*.png"; }
};

class BmpResourceLoader : public WicResourceLoader
{
public:
	virtual std::string VGetPattern() { return "*.bmp"; }
};

class TiffResourceLoader : public WicResourceLoader
{
public:
	virtual std::string VGetPattern() { return "*.tiff"; }
};

shared_ptr<IResourceLoader> CreateDdsResourceLoader()
{
	return shared_ptr<IResourceLoader>(DEBUG_NEW DdsResourceLoader());
}

shared_ptr<IResourceLoader> CreateJpgResourceLoader()
{
	return shared_ptr<IResourceLoader>(DEBUG_NEW JpgResourceLoader());
}

shared_ptr<IResourceLoader> CreatePngResourceLoader()
{
	return shared_ptr<IResourceLoader>(DEBUG_NEW PngResourceLoader());
}

shared_ptr<IResourceLoader> CreateBmpResourceLoader()
{
	return shared_ptr<IResourceLoader>(DEBUG_NEW BmpResourceLoader());
}

shared_ptr<IResourceLoader> CreateTiffResourceLoader()
{
	return shared_ptr<IResourceLoader>(DEBUG_NEW TiffResourceLoader());
}
