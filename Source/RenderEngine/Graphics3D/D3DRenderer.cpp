#include "D3DRenderer.h"

CDXUTTextHelper* D3DRenderer::m_pTextHelper = nullptr;

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
