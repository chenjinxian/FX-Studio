#include "D3DRenderer.h"

CDXUTDialogResourceManager D3DRenderer::m_DialogResourceManager;
CDXUTTextHelper *D3DRenderer::m_pTextHelper = nullptr;

D3DRenderer::D3DRenderer()
{
}


D3DRenderer::~D3DRenderer()
{
}

class D3DRendererAlphaPass11 : public IRenderState
{
public:
	D3DRendererAlphaPass11()
	{
		DXUTGetD3D11DeviceContext()->OMGetBlendState(&m_pOldBlendState, m_OldBlendFactor, &m_OldSampleMask);
		DXUTGetD3D11DeviceContext()->OMSetBlendState(D3DBlendStates::AlphaBlend, 0, 0xffffffff);
	}

	virtual ~D3DRendererAlphaPass11()
	{
		DXUTGetD3D11DeviceContext()->OMSetBlendState(m_pOldBlendState, m_OldBlendFactor, m_OldSampleMask);
		SAFE_RELEASE(m_pOldBlendState);
	}

	std::string VToString() { return "D3DRendererAlphaPass11"; }

protected:
	ID3D11BlendState* m_pOldBlendState;
	float m_OldBlendFactor[4];
	uint32_t m_OldSampleMask;
};

class D3DRendererSkyBoxPass11 : public IRenderState
{
public:
	D3DRendererSkyBoxPass11()
	{
		DXUTGetD3D11DeviceContext()->OMGetDepthStencilState(&m_pOldDepthStencilState, &m_OldStencilRef);
		DXUTGetD3D11DeviceContext()->OMSetDepthStencilState(D3DDepthStencilStates::DepthRead, 0);
	}

	~D3DRendererSkyBoxPass11()
	{
		DXUTGetD3D11DeviceContext()->OMSetDepthStencilState(m_pOldDepthStencilState, m_OldStencilRef);
		SAFE_RELEASE(m_pOldDepthStencilState);
	}

	std::string VToString() { return "D3DRendererSkyBoxPass11"; }

protected:
	ID3D11DepthStencilState* m_pOldDepthStencilState;
	uint32_t m_OldStencilRef;
};

D3DRenderer11::D3DRenderer11() : m_backgroundColor(Color(Colors::CornflowerBlue.f))
{

}

D3DRenderer11::~D3DRenderer11()
{

}

bool D3DRenderer11::VPreRender()
{
	if (DXUTGetD3D11DeviceContext() && DXUTGetD3D11RenderTargetView())
	{
		DXUTGetD3D11DeviceContext()->ClearRenderTargetView(DXUTGetD3D11RenderTargetView(), m_backgroundColor);
		DXUTGetD3D11DeviceContext()->ClearDepthStencilView(DXUTGetD3D11DepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	}
	return true;
}

bool D3DRenderer11::VPostRender()
{
	return true;
}

HRESULT D3DRenderer11::VOnRestore()
{
	HRESULT hr;
	V_RETURN(D3DRenderer::VOnRestore());
	SAFE_DELETE(D3DRenderer::m_pTextHelper);
	D3DRenderer::m_pTextHelper = GCC_NEW CDXUTTextHelper(DXUTGetD3D11Device(), DXUTGetD3D11DeviceContext(), &m_DialogResourceManager, 15);

	return S_OK;
}

shared_ptr<IRenderState> D3DRenderer11::VPrepareAlphaPass()
{
	return shared_ptr<IRenderState>(GCC_NEW D3DRendererAlphaPass11());
}

shared_ptr<IRenderState> D3DRenderer11::VPrepareSkyBoxPass()
{
	return shared_ptr<IRenderState>(GCC_NEW D3DRendererSkyBoxPass11());
}

ID3D11BlendState* D3DBlendStates::AlphaBlend = nullptr;
ID3D11BlendState* D3DBlendStates::MultiplicativeBlend = nullptr;

void D3DBlendStates::Initialize(ID3D11Device* pd3dDevice)
{
	DEBUG_ASSERT(pd3dDevice != nullptr);

	D3D11_BLEND_DESC blendStateDesc;
	ZeroMemory(&blendStateDesc, sizeof(blendStateDesc));
	blendStateDesc.RenderTarget[0].BlendEnable = true;
	blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HRESULT hr;
	if (FAILED(hr = pd3dDevice->CreateBlendState(&blendStateDesc, &AlphaBlend)))
	{
		DEBUG_ERROR("ID3D11Device::CreateBlendState() failed.");
	}

	ZeroMemory(&blendStateDesc, sizeof(blendStateDesc));
	blendStateDesc.RenderTarget[0].BlendEnable = true;
	blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ZERO;
	blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_SRC_COLOR;
	blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	if (FAILED(hr = pd3dDevice->CreateBlendState(&blendStateDesc, &MultiplicativeBlend)))
	{
		DEBUG_ERROR("ID3D11Device::CreateBlendState() failed.");
	}
}

void D3DBlendStates::Release()
{
	SAFE_RELEASE(AlphaBlend);
	SAFE_RELEASE(MultiplicativeBlend);
}

ID3D11RasterizerState* D3DRasterizerStates::CullNone = nullptr;
ID3D11RasterizerState* D3DRasterizerStates::CullBack = nullptr;
ID3D11RasterizerState* D3DRasterizerStates::CullFront = nullptr;
ID3D11RasterizerState* D3DRasterizerStates::Wireframe = nullptr;

void D3DRasterizerStates::Initialize(ID3D11Device* pd3dDevice)
{
	DEBUG_ASSERT(pd3dDevice != nullptr);

	D3D11_RASTERIZER_DESC rasterizerStateDesc;
	ZeroMemory(&rasterizerStateDesc, sizeof(rasterizerStateDesc));
	rasterizerStateDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerStateDesc.CullMode = D3D11_CULL_BACK;
	rasterizerStateDesc.DepthClipEnable = true;
	rasterizerStateDesc.MultisampleEnable = true;

	HRESULT hr = pd3dDevice->CreateRasterizerState(&rasterizerStateDesc, &CullBack);
	if (FAILED(hr))
	{
		DEBUG_ERROR("ID3D11Device::CreateRasterizerState() failed.");
	}

	ZeroMemory(&rasterizerStateDesc, sizeof(rasterizerStateDesc));
	rasterizerStateDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerStateDesc.CullMode = D3D11_CULL_BACK;
	rasterizerStateDesc.FrontCounterClockwise = true;
	rasterizerStateDesc.DepthClipEnable = true;
	rasterizerStateDesc.MultisampleEnable = true;

	hr = pd3dDevice->CreateRasterizerState(&rasterizerStateDesc, &CullFront);
	if (FAILED(hr))
	{
		DEBUG_ERROR("ID3D11Device::CreateRasterizerState() failed.");
	}

	ZeroMemory(&rasterizerStateDesc, sizeof(rasterizerStateDesc));
	rasterizerStateDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerStateDesc.CullMode = D3D11_CULL_NONE;
	rasterizerStateDesc.DepthClipEnable = true;
	rasterizerStateDesc.MultisampleEnable = true;

	hr = pd3dDevice->CreateRasterizerState(&rasterizerStateDesc, &CullNone);
	if (FAILED(hr))
	{
		DEBUG_ERROR("ID3D11Device::CreateRasterizerState() failed.");
	}

	ZeroMemory(&rasterizerStateDesc, sizeof(rasterizerStateDesc));
	rasterizerStateDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterizerStateDesc.CullMode = D3D11_CULL_NONE;
	rasterizerStateDesc.DepthClipEnable = true;
	rasterizerStateDesc.MultisampleEnable = true;

	hr = pd3dDevice->CreateRasterizerState(&rasterizerStateDesc, &Wireframe);
	if (FAILED(hr))
	{
		DEBUG_ERROR("ID3D11Device::CreateRasterizerState() failed.");
	}
}

void D3DRasterizerStates::Release()
{
	SAFE_RELEASE(CullNone);
	SAFE_RELEASE(CullBack);
	SAFE_RELEASE(CullFront);
	SAFE_RELEASE(Wireframe);
}

ID3D11DepthStencilState* D3DDepthStencilStates::DepthNone = nullptr;
ID3D11DepthStencilState* D3DDepthStencilStates::DepthRead = nullptr;

void D3DDepthStencilStates::Initialize(ID3D11Device* pd3dDevice)
{
	DEBUG_ASSERT(pd3dDevice != nullptr);

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = false;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	depthStencilDesc.StencilEnable = false;
	depthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	depthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

	depthStencilDesc.BackFace = depthStencilDesc.FrontFace;

	HRESULT hr;
	if (FAILED(hr = pd3dDevice->CreateDepthStencilState(&depthStencilDesc, &DepthNone)))
	{
		DEBUG_ERROR("ID3D11Device::CreateDepthStencilState() failed.");
	}

	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	depthStencilDesc.StencilEnable = false;
	depthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	depthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

	depthStencilDesc.BackFace = depthStencilDesc.FrontFace;

	if (FAILED(hr = pd3dDevice->CreateDepthStencilState(&depthStencilDesc, &DepthNone)))
	{
		DEBUG_ERROR("ID3D11Device::CreateDepthStencilState() failed.");
	}
}

void D3DDepthStencilStates::Release()
{
	SAFE_RELEASE(DepthNone);
	SAFE_RELEASE(DepthRead);
}
