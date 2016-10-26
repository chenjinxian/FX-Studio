#pragma once
#include "../RenderEngineBase.h"
#include "../RenderEngineInterface.h"
#include "../ResourceCache/ResCache.h"
#include "SceneNode.h"

shared_ptr<IResourceLoader> CreateSdkMeshResourceLoader();

class SdkMeshResourceLoader : public IResourceLoader
{
public:
	virtual bool VUseRawFile() { return false; }
	virtual bool VDiscardRawBufferAfterLoad() { return false; }
	virtual unsigned int VGetLoadedResourceSize(char *rawBuffer, unsigned int rawSize);
	virtual bool VLoadResource(char *rawBuffer, unsigned int rawSize, shared_ptr<ResHandle> handle);
	virtual std::string VGetPattern() { return "*.sdkmesh"; }
};

class D3DSdkMeshResourceExtraData11 : public IResourceExtraData
{
	friend class SdkMeshResourceLoader;

public:
	D3DSdkMeshResourceExtraData11() { };
	virtual ~D3DSdkMeshResourceExtraData11() { }
	virtual std::string VToString() { return "D3DSdkMeshResourceExtraData11"; }

	CDXUTSDKMesh m_Mesh11;	
};

class D3DShaderMeshNode11 : public SceneNode
{
public:
	D3DShaderMeshNode11(ActorId actorId, WeakBaseRenderComponentPtr renderComponent,
		std::string meshFileName, RenderPass renderPass, const Matrix& mat);

	virtual HRESULT VOnRestore(Scene *pScene);
	virtual HRESULT VOnLostDevice(Scene *pScene) { return S_OK; }
	virtual HRESULT VOnDestoryDevice() { return S_OK; }
	virtual HRESULT VRender(Scene* pScene, double fTime, float fElapsedTime) override;

private:
	std::string m_MeshFileName;
	XMMATRIX                            g_World;
	ID3DX11Effect*                      g_pEffect;
	ID3D11InputLayout*                  g_pVertexLayout;
	ID3DX11EffectTechnique*             g_pTechnique;
	CDXUTSDKMesh                        g_Mesh;
	ID3DX11EffectShaderResourceVariable* g_ptxDiffuseVariable;
	ID3DX11EffectMatrixVariable*        g_pWorldVariable;
	ID3DX11EffectMatrixVariable*        g_pViewVariable;
	ID3DX11EffectMatrixVariable*        g_pProjectionVariable;
	ID3DX11EffectScalarVariable*        g_pWavinessVariable;
	ID3DX11EffectScalarVariable*        g_pTimeVariable;
};
