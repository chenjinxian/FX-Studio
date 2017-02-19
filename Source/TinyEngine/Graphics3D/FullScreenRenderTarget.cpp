#include "FullScreenRenderTarget.h"
#include "../AppFramework/BaseGameApp.h"

FullScreenRenderTarget::FullScreenRenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, bool material)
	: m_pDevice(pDevice),
	m_pDeviceContext(pDeviceContext),
	m_OutputResource(nullptr),
	m_OutputTexture(nullptr),
	m_RenderTargetView(nullptr),
	m_DepthStencilView(nullptr)
{
	D3D11_TEXTURE2D_DESC fullScreenTextureDesc;
	ZeroMemory(&fullScreenTextureDesc, sizeof(fullScreenTextureDesc));
	fullScreenTextureDesc.Width = material ? 100 : g_pApp->GetGameConfig().m_ScreenWidth;
	fullScreenTextureDesc.Height = material ? 100 : g_pApp->GetGameConfig().m_ScreenHeight;
	fullScreenTextureDesc.MipLevels = 1;
	fullScreenTextureDesc.ArraySize = 1;
	fullScreenTextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	fullScreenTextureDesc.SampleDesc.Count = g_pApp->GetGameConfig().m_AntiAliasingSample;
	fullScreenTextureDesc.SampleDesc.Quality = 0;
	fullScreenTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	fullScreenTextureDesc.Usage = D3D11_USAGE_DEFAULT;

	HRESULT hr;
	if (FAILED(hr = m_pDevice->CreateTexture2D(&fullScreenTextureDesc, nullptr, &m_OutputResource)))
	{
		DEBUG_ERROR("IDXGIDevice::CreateTexture2D() failed.");
	}

	if (FAILED(hr = m_pDevice->CreateShaderResourceView(m_OutputResource, nullptr, &m_OutputTexture)))
	{
		DEBUG_ERROR("IDXGIDevice::CreateShaderResourceView() failed.");
	}

	if (FAILED(hr = m_pDevice->CreateRenderTargetView(m_OutputResource, nullptr, &m_RenderTargetView)))
	{
		DEBUG_ERROR("IDXGIDevice::CreateRenderTargetView() failed.");
	}

	D3D11_TEXTURE2D_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.Width = material ? 100 : g_pApp->GetGameConfig().m_ScreenWidth;
	depthStencilDesc.Height = material ? 100 : g_pApp->GetGameConfig().m_ScreenHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = g_pApp->GetGameConfig().m_AntiAliasingSample;;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;

	ID3D11Texture2D* depthStencilBuffer = nullptr;
	if (FAILED(hr = m_pDevice->CreateTexture2D(&depthStencilDesc, nullptr, &depthStencilBuffer)))
	{
		DEBUG_ERROR("IDXGIDevice::CreateTexture2D() failed.");
	}

	if (FAILED(hr = m_pDevice->CreateDepthStencilView(depthStencilBuffer, nullptr, &m_DepthStencilView)))
	{
		SAFE_RELEASE(depthStencilBuffer);
		DEBUG_ERROR("IDXGIDevice::CreateDepthStencilView() failed.");
	}

	SAFE_RELEASE(depthStencilBuffer);
}

FullScreenRenderTarget::~FullScreenRenderTarget()
{
	SAFE_RELEASE(m_OutputResource);
	SAFE_RELEASE(m_OutputTexture);
	SAFE_RELEASE(m_DepthStencilView);
	SAFE_RELEASE(m_RenderTargetView);
}
