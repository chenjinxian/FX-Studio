#pragma once
#include "ResCache.h"

class D3D11TextureResourceExtraData : public IResourceExtraData
{
	friend class D3D11Renderer;

public:
	D3D11TextureResourceExtraData();
	virtual ~D3D11TextureResourceExtraData();

	virtual std::string VToString() { return "D3D11TextureResourceExtraData"; }
	ID3D11ShaderResourceView* GetTexture() const { return m_pTexture; }

protected:
	ID3D11ShaderResourceView *m_pTexture;
};

class TextureResourceLoader : public IResourceLoader
{
public:
	virtual bool VUseRawFile() override { return false; }
	virtual bool VDiscardRawBufferAfterLoad() override { return true; }
	virtual uint32_t VGetLoadedResourceSize(char *rawBuffer, uint32_t rawSize) override { return 0; }
	virtual bool VLoadResource(char *rawBuffer, uint32_t rawSize, shared_ptr<ResHandle> handle) override { return true; }
};

class DdsResourceLoader : public TextureResourceLoader
{
public:
	virtual std::string VGetPattern() override { return "*.dds"; }
	virtual bool VLoadResource(char *rawBuffer, uint32_t rawSize, shared_ptr<ResHandle> handle) override;
};

class WicResourceLoader : public TextureResourceLoader
{
public:
	virtual bool VLoadResource(char *rawBuffer, uint32_t rawSize, shared_ptr<ResHandle> handle) override;
};
