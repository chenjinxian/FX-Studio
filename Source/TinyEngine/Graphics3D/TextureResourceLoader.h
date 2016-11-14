#pragma once
#include "../ResourceCache/ResCache.h"

shared_ptr<IResourceLoader> CreateDdsResourceLoader();
shared_ptr<IResourceLoader> CreateJpgResourceLoader();
shared_ptr<IResourceLoader> CreatePngResourceLoader();
shared_ptr<IResourceLoader> CreateBmpResourceLoader();
shared_ptr<IResourceLoader> CreateTiffResourceLoader();

class D3DTextureResourceExtraData11 : public IResourceExtraData
{
	friend class DdsResourceLoader;
	friend class WicResourceLoader;

public:
	D3DTextureResourceExtraData11();
	virtual ~D3DTextureResourceExtraData11();

	virtual std::string VToString() { return "D3DTextureResourceExtraData11"; }
	ID3D11ShaderResourceView* GetTexture() const { return m_pTexture; }

protected:
	ID3D11ShaderResourceView *m_pTexture;
};

class TextureResourceLoader : public IResourceLoader
{
public:
	virtual bool VUseRawFile() { return false; }
	virtual bool VDiscardRawBufferAfterLoad() { return true; }
	virtual uint32_t VGetLoadedResourceSize(char *rawBuffer, uint32_t rawSize) { return 0; }
	virtual bool VLoadResource(char *rawBuffer, uint32_t rawSize, shared_ptr<ResHandle> handle) { return true; }
};

class DdsResourceLoader : public TextureResourceLoader
{
public:
	virtual std::string VGetPattern() { return "*.dds"; }
	virtual bool VLoadResource(char *rawBuffer, uint32_t rawSize, shared_ptr<ResHandle> handle);
};

class WicResourceLoader : public TextureResourceLoader
{
public:
	virtual bool VLoadResource(char *rawBuffer, uint32_t rawSize, shared_ptr<ResHandle> handle);
};
