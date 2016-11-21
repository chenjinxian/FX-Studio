#include "D3D11Renderer.h"
#include "../AppFramework/BaseGameApp.h"

D3D11Renderer::D3D11Renderer()
{
	InitD3D11Device();
}

D3D11Renderer::~D3D11Renderer()
{
}

void D3D11Renderer::VSetBackgroundColor(const Color& color)
{

}

bool D3D11Renderer::VInitRenderer(HWND hWnd)
{
	HRESULT hr;

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.Width = g_pApp->m_Config.m_ScreenWidth;
	swapChainDesc.Height = g_pApp->m_Config.m_ScreenHeight;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.SampleDesc.Count = g_pApp->m_Config.m_AntiAliasingSample;
	swapChainDesc.SampleDesc.Quality = m_MultiSamplingQualityLevels;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	IDXGIDevice* dxgiDevice = nullptr;
	if (FAILED(hr = m_pDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice))))
	{
		DEBUG_ERROR("ID3D11Device::QueryInterface() failed");
	}

	IDXGIAdapter *dxgiAdapter = nullptr;
	if (FAILED(hr = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&dxgiAdapter))))
	{
		SAFE_RELEASE(dxgiDevice);
		DEBUG_ERROR("IDXGIDevice::GetParent() failed retrieving adapter.");
	}

	IDXGIFactory2* dxgiFactory = nullptr;
	if (FAILED(hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory))))
	{
		SAFE_RELEASE(dxgiDevice);
		SAFE_RELEASE(dxgiAdapter);
		DEBUG_ERROR("IDXGIAdapter::GetParent() failed retrieving factory.");
	}

	DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullScreenDesc;
	ZeroMemory(&fullScreenDesc, sizeof(fullScreenDesc));
	fullScreenDesc.RefreshRate.Numerator = 60;
	fullScreenDesc.RefreshRate.Denominator = 1;
	fullScreenDesc.Windowed = !g_pApp->m_Config.m_IsFullScreen;

	if (FAILED(hr = dxgiFactory->CreateSwapChainForHwnd(dxgiDevice, hWnd, &swapChainDesc, &fullScreenDesc, nullptr, &m_pSwapChain)))
	{
		SAFE_RELEASE(dxgiDevice);
		SAFE_RELEASE(dxgiAdapter);
		SAFE_RELEASE(dxgiFactory);
		DEBUG_ERROR("IDXGIDevice::CreateSwapChainForHwnd() failed.");
	}

	SAFE_RELEASE(dxgiDevice);
	SAFE_RELEASE(dxgiAdapter);
	SAFE_RELEASE(dxgiFactory);

	ID3D11Texture2D* backBuffer;
	if (FAILED(hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer))))
	{
		DEBUG_ERROR("IDXGISwapChain::GetBuffer() failed.");
	}

	backBuffer->GetDesc(&m_BackBufferDesc);

	if (FAILED(hr = m_pDevice->CreateRenderTargetView(backBuffer, nullptr, &m_pRenderTargetView)))
	{
		SAFE_RELEASE(backBuffer);
		DEBUG_ERROR("IDXGIDevice::CreateRenderTargetView() failed.");
	}

	SAFE_RELEASE(backBuffer);

	D3D11_TEXTURE2D_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.Width = g_pApp->m_Config.m_ScreenWidth;
	depthStencilDesc.Height = g_pApp->m_Config.m_ScreenHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.SampleDesc.Count = g_pApp->m_Config.m_AntiAliasingSample;
	depthStencilDesc.SampleDesc.Quality = m_MultiSamplingQualityLevels;

	if (FAILED(hr = m_pDevice->CreateTexture2D(&depthStencilDesc, nullptr, &m_pDepthStencilBuffer)))
	{
		DEBUG_ERROR("IDXGIDevice::CreateTexture2D() failed.");
	}

	if (FAILED(hr = m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer, nullptr, &m_pDepthStencilView)))
	{
		DEBUG_ERROR("IDXGIDevice::CreateDepthStencilView() failed.");
	}

	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	m_Viewport.TopLeftX = 0.0f;
	m_Viewport.TopLeftY = 0.0f;
	m_Viewport.Width = static_cast<float>(g_pApp->m_Config.m_ScreenWidth);
	m_Viewport.Height = static_cast<float>(g_pApp->m_Config.m_ScreenHeight);
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;

	m_pDeviceContext->RSSetViewports(1, &m_Viewport);

	return true;
}

bool D3D11Renderer::VPreRender()
{
	return true;
}

bool D3D11Renderer::VPostRender()
{
	return true;
}

void D3D11Renderer::VShutdown()
{

}

void D3D11Renderer::InitD3D11Device()
{
	HRESULT hr;
	UINT createDeviceFlags = 0;

#if defined(DEBUG) || defined(_DEBUG)  
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};

	ID3D11Device* direct3DDevice = nullptr;
	ID3D11DeviceContext* direct3DDeviceContext = nullptr;
	if (FAILED(hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &direct3DDevice, &m_FeatureLevel, &direct3DDeviceContext)))
	{
		DEBUG_ERROR("D3D11CreateDevice() failed");
	}

	if (FAILED(hr = direct3DDevice->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&m_pDevice))))
	{
		DEBUG_ERROR("ID3D11Device::QueryInterface() failed");
	}

	if (FAILED(hr = direct3DDeviceContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&m_pDeviceContext))))
	{
		DEBUG_ERROR("ID3D11Device::QueryInterface() failed");
	}

	SAFE_RELEASE(direct3DDevice);
	SAFE_RELEASE(direct3DDeviceContext);

	while (g_pApp->m_Config.m_AntiAliasingSample > 0)
	{
		if (SUCCEEDED(m_pDevice->CheckMultisampleQualityLevels(
			DXGI_FORMAT_R8G8B8A8_UNORM, g_pApp->m_Config.m_AntiAliasingSample, &m_MultiSamplingQualityLevels)) &&
			m_MultiSamplingQualityLevels > 0)
		{
			m_MultiSamplingQualityLevels--;
			break;
		}
		else
		{
			g_pApp->m_Config.m_AntiAliasingSample /= 2;
		}
	}
}
