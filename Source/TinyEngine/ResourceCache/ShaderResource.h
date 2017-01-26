#pragma once
#include "ResCache.h"
#include "../Graphics3D/Material.h"

class HlslResourceExtraData : public IResourceExtraData
{
	friend class D3D11Renderer;
	friend class FxSourceEffectResourceLoader;
	friend class FxObjectEffectResourceLoader;

public:
	HlslResourceExtraData();
	virtual ~HlslResourceExtraData();

	virtual std::string VToString() { return "ShaderResourceExtraData"; }
	Effect* GetEffect() const { return m_pEffect; }

private:
	ID3DX11Effect* m_pD3DX11Effect;
	Effect* m_pEffect;
};

class ShaderResourceLoader : public IResourceLoader
{
public:
	virtual bool VUseRawFile() override { return false; }
	virtual bool VDiscardRawBufferAfterLoad() override { return true; }
	virtual uint32_t VGetLoadedResourceSize(char *rawBuffer, uint32_t rawSize) override { return 0; }
	virtual bool VLoadResource(char *rawBuffer, uint32_t rawSize, shared_ptr<ResHandle> handle) override { return true; }
};

class FxSourceEffectResourceLoader : public ShaderResourceLoader
{
public:
	virtual std::string VGetPattern() override { return "*.fx"; }
	virtual bool VLoadResource(char *rawBuffer, uint32_t rawSize, shared_ptr<ResHandle> handle) override;
};

class FxObjectEffectResourceLoader : public ShaderResourceLoader
{
public:
	virtual std::string VGetPattern() override { return "*.fxo"; }
	virtual bool VLoadResource(char *rawBuffer, uint32_t rawSize, shared_ptr<ResHandle> handle) override;
};
