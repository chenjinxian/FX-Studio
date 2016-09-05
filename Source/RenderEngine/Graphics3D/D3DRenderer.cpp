#include "D3DRenderer.h"
#include "../UserInterface/HumanView.h"

CDXUTDialogResourceManager D3DRenderer::g_DialogResourceManager;
CDXUTTextHelper *D3DRenderer::g_pTextHelper = NULL;

//
// class D3DRendererAlphaPass911			- Chapter 16, page 543
//
class D3DRendererAlphaPass11 : public IRenderState
{
protected:
	ID3D11BlendState* m_pOldBlendState;
	FLOAT m_OldBlendFactor[ 4 ];
	UINT m_OldSampleMask;
		
	ID3D11BlendState* m_pCurrentBlendState;

public:
	D3DRendererAlphaPass11();
	~D3DRendererAlphaPass11();
	std::string VToString() { return "D3DRendererAlphaPass11"; }
};

//
// D3DRendererAlphaPass11::D3DRendererAlphaPass11			- Chapter 16, page 544
//
D3DRendererAlphaPass11::D3DRendererAlphaPass11()
{
	DXUTGetD3D11DeviceContext()->OMGetBlendState(&m_pOldBlendState, m_OldBlendFactor, &m_OldSampleMask);
	m_pCurrentBlendState = NULL;

	D3D11_BLEND_DESC BlendState;
	ZeroMemory(&BlendState, sizeof(D3D11_BLEND_DESC));

	BlendState.AlphaToCoverageEnable = false;
	BlendState.IndependentBlendEnable = false;
	BlendState.RenderTarget[0].BlendEnable = TRUE;
	BlendState.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BlendState.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	BlendState.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendState.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	BlendState.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendState.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendState.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	DXUTGetD3D11Device()->CreateBlendState(&BlendState, &m_pCurrentBlendState);
	DXUTGetD3D11DeviceContext()->OMSetBlendState(m_pCurrentBlendState, 0, 0xffffffff);
}

//
// D3DRendererAlphaPass11::~D3DRendererAlphaPass11			- Chapter 16, page 544
//
D3DRendererAlphaPass11::~D3DRendererAlphaPass11()
{
	DXUTGetD3D11DeviceContext()->OMSetBlendState(m_pOldBlendState, m_OldBlendFactor, m_OldSampleMask);
	SAFE_RELEASE(m_pCurrentBlendState);
	SAFE_RELEASE(m_pOldBlendState);
}

 
//
//
// class D3DRendererSkyBoxPass11							- Chapter 16, page 548
//
class D3DRendererSkyBoxPass11 : public IRenderState
{
protected:
	ID3D11DepthStencilState* m_pOldDepthStencilState;
	ID3D11DepthStencilState* m_pSkyboxDepthStencilState;

public:
	D3DRendererSkyBoxPass11();
	~D3DRendererSkyBoxPass11();
	std::string VToString() { return "D3DRendererSkyBoxPass11"; }
};

//
// D3DRendererSkyBoxPass11::D3DRendererSkyBoxPass11()		- Chapter 16, page 548
//
D3DRendererSkyBoxPass11::D3DRendererSkyBoxPass11()
{
	// Depth stencil state
	D3D11_DEPTH_STENCIL_DESC DSDesc;
	ZeroMemory( &DSDesc, sizeof( D3D11_DEPTH_STENCIL_DESC ) );
	DSDesc.DepthEnable = TRUE;
	DSDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	DSDesc.DepthFunc = D3D11_COMPARISON_LESS;
	DSDesc.StencilEnable = FALSE;
	DXUTGetD3D11Device()->CreateDepthStencilState( &DSDesc, &m_pSkyboxDepthStencilState );
	DXUT_SetDebugName( m_pSkyboxDepthStencilState, "SkyboxDepthStencil" );

	UINT StencilRef;
	DXUTGetD3D11DeviceContext()->OMGetDepthStencilState( &m_pOldDepthStencilState, &StencilRef );
	DXUTGetD3D11DeviceContext()->OMSetDepthStencilState( m_pSkyboxDepthStencilState, 0 );
}

//
// D3DRendererSkyBoxPass11::~D3DRendererSkyBoxPass11()		- Chapter 16, page 548
//
D3DRendererSkyBoxPass11::~D3DRendererSkyBoxPass11()
{
	DXUTGetD3D11DeviceContext()->OMSetDepthStencilState( m_pOldDepthStencilState, 0 );
	SAFE_RELEASE(m_pOldDepthStencilState);
	SAFE_RELEASE(m_pSkyboxDepthStencilState);
}

HRESULT D3DRenderer11::VOnRestore()
{
	HRESULT hr;
	V_RETURN ( D3DRenderer::VOnRestore() );
	SAFE_DELETE(D3DRenderer::g_pTextHelper);
	D3DRenderer::g_pTextHelper = GCC_NEW CDXUTTextHelper( DXUTGetD3D11Device(), DXUTGetD3D11DeviceContext(), &g_DialogResourceManager, 15 );

	return S_OK;
}


bool D3DRenderer11::VPreRender()
{
	if (DXUTGetD3D11DeviceContext() && DXUTGetD3D11RenderTargetView())
	{
		DXUTGetD3D11DeviceContext()->ClearRenderTargetView( DXUTGetD3D11RenderTargetView(), m_BackgroundColor );

		//
		// Clear the depth buffer to 1.0 (max depth)
		//
		DXUTGetD3D11DeviceContext()->ClearDepthStencilView( DXUTGetD3D11DepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0 );
	}
	return true;
}

bool D3DRenderer11::VPostRender(void)
{
	return true;
}
