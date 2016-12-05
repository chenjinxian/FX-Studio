#include "ShaderResource.h"
#include "../AppFramework/BaseGameApp.h"
#include <d3dcompiler.h>

HlslResourceExtraData::HlslResourceExtraData()
	: m_pEffect(nullptr)
{

}

HlslResourceExtraData::~HlslResourceExtraData()
{

}

bool FxEffectResourceLoader::VLoadResource(char *rawBuffer, uint32_t rawSize, shared_ptr<ResHandle> handle)
{
	BaseGameApp::Renderer renderer = g_pApp->GetRendererType();
	if (renderer == BaseGameApp::Renderer_D3D11)
	{
		shared_ptr<HlslResourceExtraData> extra = shared_ptr<HlslResourceExtraData>(DEBUG_NEW HlslResourceExtraData());

		if (g_pApp->GetRendererAPI()->VCompileShaderFromMemory(rawBuffer, rawSize, extra))
		{
			handle->SetExtraData(extra);
			return true;
		}
	}

	DEBUG_ASSERT(0 && "Unsupported Renderer in TextureResourceLoader::VLoadResource");
	return false;
}

bool CsoEffectResourceLoader::VLoadResource(char *rawBuffer, uint32_t rawSize, shared_ptr<ResHandle> handle)
{
	return true;
}

shared_ptr<IResourceLoader> CreateFxEffectResourceLoader()
{
	return shared_ptr<IResourceLoader>(DEBUG_NEW FxEffectResourceLoader());
}

shared_ptr<IResourceLoader> CreateCsoEffectResourceLoader()
{
	return shared_ptr<IResourceLoader>(DEBUG_NEW CsoEffectResourceLoader());
}
