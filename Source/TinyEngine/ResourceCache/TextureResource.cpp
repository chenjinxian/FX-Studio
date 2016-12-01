#include "TextureResource.h"
#include "../AppFramework/BaseGameApp.h"

D3D11TextureResourceExtraData::D3D11TextureResourceExtraData()
	: m_pTexture(nullptr)
{
}

D3D11TextureResourceExtraData::~D3D11TextureResourceExtraData()
{
	SAFE_RELEASE(m_pTexture);
}

bool DdsResourceLoader::VLoadResource(char *rawBuffer, uint32_t rawSize, shared_ptr<ResHandle> handle)
{
	BaseGameApp::Renderer renderer = g_pApp->GetRendererType();
	if (renderer == BaseGameApp::Renderer_D3D11)
	{
		shared_ptr<D3D11TextureResourceExtraData> extra =
			shared_ptr<D3D11TextureResourceExtraData>(DEBUG_NEW D3D11TextureResourceExtraData());

		if (g_pApp->GetRendererAPI()->VCreateDDSTextureResoure(rawBuffer, rawSize, extra))
		{
			handle->SetExtraData(extra);
			return true;
		}
	}

	DEBUG_ASSERT(0 && "Unsupported Renderer in TextureResourceLoader::VLoadResource");
	return false;
}

bool WicResourceLoader::VLoadResource(char *rawBuffer, uint32_t rawSize, shared_ptr<ResHandle> handle)
{
	BaseGameApp::Renderer renderer = g_pApp->GetRendererType();
	if (renderer == BaseGameApp::Renderer_D3D11)
	{
		shared_ptr<D3D11TextureResourceExtraData> extra =
			shared_ptr<D3D11TextureResourceExtraData>(DEBUG_NEW D3D11TextureResourceExtraData());

		if (g_pApp->GetRendererAPI()->VCreateWICTextureResoure(rawBuffer, rawSize, extra))
		{
			handle->SetExtraData(extra);
			return true;
		}
	}

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
