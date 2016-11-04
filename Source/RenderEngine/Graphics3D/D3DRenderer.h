#pragma once
#include "../RenderEngineBase.h"
#include "../RenderEngineInterface.h"

class D3DRenderer : public IRenderer
{
public:
	D3DRenderer();
	virtual ~D3DRenderer();

	virtual HRESULT VOnRestore() override { return S_OK; }
	virtual void VShutdown() override { SAFE_DELETE(m_pTextHelper); }
	static CDXUTDialogResourceManager m_DialogResourceManager;
	static CDXUTTextHelper* m_pTextHelper;
};

class D3DRenderer11 : public D3DRenderer
{
public:
	D3DRenderer11();
	~D3DRenderer11();

	virtual void VSetBackgroundColor(const Color& color) override { m_backgroundColor = color; }
	virtual bool VPreRender();
	virtual bool VPostRender();
	virtual HRESULT VOnRestore();
	virtual void VShutdown() { D3DRenderer::VShutdown(); }

	virtual shared_ptr<IRenderState> VPrepareAlphaPass();
	virtual shared_ptr<IRenderState> VPrepareSkyBoxPass();

private:
	Color m_backgroundColor;
};

class D3DBlendStates : public boost::noncopyable
{
public:
	static void Initialize(ID3D11Device* pd3dDevice);
	static void Release();

	static ID3D11BlendState* AlphaBlend;
	static ID3D11BlendState* MultiplicativeBlend;
};

class D3DRasterizerStates : public boost::noncopyable
{
public:
	static void Initialize(ID3D11Device* pd3dDevice);
	static void Release();

	static ID3D11RasterizerState* CullNone;
	static ID3D11RasterizerState* CullBack;
	static ID3D11RasterizerState* CullFront;
	static ID3D11RasterizerState* Wireframe;
};

class D3DDepthStencilStates : public boost::noncopyable
{
public:
	static void Initialize(ID3D11Device* pd3dDevice);
	static void Release();

	static ID3D11DepthStencilState* DepthNone;
	static ID3D11DepthStencilState* DepthRead;
};
