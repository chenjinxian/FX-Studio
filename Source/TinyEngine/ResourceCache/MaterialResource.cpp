#include "MaterialResource.h"
#include "XmlResource.h"
#include "../AppFramework/BaseGameApp.h"
#include <d3dcompiler.h>

HlslResourceExtraData::HlslResourceExtraData()
	: m_pD3DX11Effect(nullptr),
	m_pEffect(nullptr)
{

}

HlslResourceExtraData::~HlslResourceExtraData()
{
	SAFE_RELEASE(m_pD3DX11Effect);
	SAFE_DELETE(m_pEffect);
}

bool FxSourceEffectResourceLoader::VLoadResource(char *rawBuffer, uint32_t rawSize, shared_ptr<ResHandle> handle)
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

bool FxObjectEffectResourceLoader::VLoadResource(char *rawBuffer, uint32_t rawSize, shared_ptr<ResHandle> handle)
{
	BaseGameApp::Renderer renderer = g_pApp->GetRendererType();
	if (renderer == BaseGameApp::Renderer_D3D11)
	{
		shared_ptr<HlslResourceExtraData> extra = shared_ptr<HlslResourceExtraData>(DEBUG_NEW HlslResourceExtraData());

		if (g_pApp->GetRendererAPI()->VCreateShaderFromMemory(rawBuffer, rawSize, extra))
		{
			handle->SetExtraData(extra);
			return true;
		}
	}

	DEBUG_ASSERT(0 && "Unsupported Renderer in TextureResourceLoader::VLoadResource");
	return false;
}

shared_ptr<IResourceLoader> CreateFxSourceEffectResourceLoader()
{
	return shared_ptr<IResourceLoader>(DEBUG_NEW FxSourceEffectResourceLoader());
}

shared_ptr<IResourceLoader> CreateFxObjectEffectResourceLoader()
{
	return shared_ptr<IResourceLoader>(DEBUG_NEW FxObjectEffectResourceLoader());
}

bool MaterialResourceLoader::VLoadResource(char *rawBuffer, uint32_t rawSize, shared_ptr<ResHandle> handle)
{
	if (rawBuffer == nullptr || rawSize == 0)
		return false;

	shared_ptr<XmlResourceExtraData> pExtraData = shared_ptr<XmlResourceExtraData>(DEBUG_NEW XmlResourceExtraData());
	pExtraData->ParseXml(rawBuffer, rawSize);

	handle->SetExtraData(shared_ptr<XmlResourceExtraData>(pExtraData));

	return true;
}

tinyxml2::XMLElement* MaterialResourceLoader::LoadAndReturnRootXmlElement(const std::string& resourceString)
{
	Resource resource(resourceString);
	shared_ptr<ResHandle> pResourceHandle = g_pApp->GetResCache()->GetHandle(&resource);
	shared_ptr<XmlResourceExtraData> pExtraData = static_pointer_cast<XmlResourceExtraData>(pResourceHandle->GetExtraData());
	if (pExtraData != nullptr)
	{
		return pExtraData->GetRoot();
	}
	else
	{
		return nullptr;
	}
}

shared_ptr<IResourceLoader> CreateMaterialResourceLoader()
{
	return shared_ptr<IResourceLoader>(DEBUG_NEW MaterialResourceLoader());
}
