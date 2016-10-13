#include "D3DRenderer.h"

CDXUTDialogResourceManager D3DRenderer::m_DialogResourceManager;
CDXUTTextHelper *D3DRenderer::m_pTextHelper = nullptr;

D3DRenderer::D3DRenderer()
{
}


D3DRenderer::~D3DRenderer()
{
}

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
