#pragma once
#include "ResCache.h"

class HlslResourceExtraData : public IResourceExtraData
{
	friend class D3D11Renderer;
	friend class FxEffectResourceLoader;
	friend class CsoEffectResourceLoader;

public:
	HlslResourceExtraData();
	virtual ~HlslResourceExtraData();

	virtual std::string VToString() { return "ShaderResourceExtraData"; }

protected:
	ID3DX11Effect* m_pEffect;
	std::vector<ID3DX11EffectTechnique*> m_Techniques;
};

class ShaderResourceLoader : public IResourceLoader
{
public:
	virtual bool VUseRawFile() override { return false; }
	virtual bool VDiscardRawBufferAfterLoad() override { return true; }
	virtual uint32_t VGetLoadedResourceSize(char *rawBuffer, uint32_t rawSize) override { return 0; }
	virtual bool VLoadResource(char *rawBuffer, uint32_t rawSize, shared_ptr<ResHandle> handle) override { return true; }
};

class FxEffectResourceLoader : public ShaderResourceLoader
{
public:
	virtual std::string VGetPattern() override { return "*.fx"; }
	virtual bool VLoadResource(char *rawBuffer, uint32_t rawSize, shared_ptr<ResHandle> handle) override;
};

class CsoEffectResourceLoader : public ShaderResourceLoader
{
public:
	virtual std::string VGetPattern() override { return "*.cso"; }
	virtual bool VLoadResource(char *rawBuffer, uint32_t rawSize, shared_ptr<ResHandle> handle) override;
};
