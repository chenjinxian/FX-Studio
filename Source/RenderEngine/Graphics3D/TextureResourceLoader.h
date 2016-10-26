#pragma once
#include "../RenderEngineBase.h"
#include "../RenderEngineInterface.h"
#include "../ResourceCache/ResCache.h"

class D3DTextureResourceExtraData11 : public IResourceExtraData
{
	friend class TextureResourceLoader;

public:
	D3DTextureResourceExtraData11();
	virtual ~D3DTextureResourceExtraData11() { SAFE_RELEASE(m_pTexture); SAFE_RELEASE(m_pSamplerLinear); }
	virtual std::string VToString() { return "D3DTextureResourceExtraData11"; }

	ID3D11ShaderResourceView * const *GetTexture() { return &m_pTexture; }
	ID3D11SamplerState * const *GetSampler() { return &m_pSamplerLinear; }

protected:
	ID3D11ShaderResourceView *m_pTexture;			
	ID3D11SamplerState* m_pSamplerLinear;
};

class TextureResourceLoader : public IResourceLoader
{
public:
	virtual bool VUseRawFile() { return false; }
	virtual bool VDiscardRawBufferAfterLoad() { return true; }
	virtual unsigned int VGetLoadedResourceSize(char *rawBuffer, unsigned int rawSize);
	virtual bool VLoadResource(char *rawBuffer, unsigned int rawSize, shared_ptr<ResHandle> handle);
};
