#pragma once
#include "ResCache.h"
#include "../Graphics3D/Material.h"

class HlslResourceExtraData : public IResourceExtraData
{
	friend class D3D11Renderer;
	friend class FxEffectResourceLoader;
	friend class CsoEffectResourceLoader;

public:
	HlslResourceExtraData();
	virtual ~HlslResourceExtraData();

	virtual std::string VToString() { return "ShaderResourceExtraData"; }

private:
	Effect* m_pEffect;
	Technique* mCurrentTechnique;
	std::map<Pass*, ID3D11InputLayout*> mInputLayouts;
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
