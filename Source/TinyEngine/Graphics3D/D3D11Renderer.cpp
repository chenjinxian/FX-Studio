#include "D3D11Renderer.h"

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

bool D3D11Renderer::VInitRenderer()
{
// 	DXGI_SWAP_CHAIN_DESC1 swapChainDesc;
// 	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
// 	swapChainDesc.Width = mScreenWidth;
// 	swapChainDesc.Height = mScreenHeight;
// 	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
// 
// 	if (m_MultiSamplingEnabled)
// 	{
// 		swapChainDesc.SampleDesc.Count = m_MultiSamplingCount;
// 		swapChainDesc.SampleDesc.Quality = m_MultiSamplingQualityLevels - 1;
// 	}
// 	else
// 	{
// 		swapChainDesc.SampleDesc.Count = 1;
// 		swapChainDesc.SampleDesc.Quality = 0;
// 	}
// 
// 	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
// 	swapChainDesc.BufferCount = 1;
// 	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
// 
// 	IDXGIDevice* dxgiDevice = nullptr;
// 	if (FAILED(hr = m_pDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice))))
// 	{
// 		DEBUG_ERROR("ID3D11Device::QueryInterface() failed", hr);
// 	}
// 
// 	IDXGIAdapter *dxgiAdapter = nullptr;
// 	if (FAILED(hr = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&dxgiAdapter))))
// 	{
// 		ReleaseObject(dxgiDevice);
// 		DEBUG_ERROR("IDXGIDevice::GetParent() failed retrieving adapter.", hr);
// 	}
// 
// 	IDXGIFactory2* dxgiFactory = nullptr;
// 	if (FAILED(hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory))))
// 	{
// 		ReleaseObject(dxgiDevice);
// 		ReleaseObject(dxgiAdapter);
// 		DEBUG_ERROR("IDXGIAdapter::GetParent() failed retrieving factory.", hr);
// 	}
// 
// 	DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullScreenDesc;
// 	ZeroMemory(&fullScreenDesc, sizeof(fullScreenDesc));
// 	fullScreenDesc.RefreshRate.Numerator = mFrameRate;
// 	fullScreenDesc.RefreshRate.Denominator = 1;
// 	fullScreenDesc.Windowed = !mIsFullScreen;
// 
// 	if (FAILED(hr = dxgiFactory->CreateSwapChainForHwnd(dxgiDevice, mWindowHandle, &swapChainDesc, &fullScreenDesc, nullptr, &mSwapChain)))
// 	{
// 		ReleaseObject(dxgiDevice);
// 		ReleaseObject(dxgiAdapter);
// 		ReleaseObject(dxgiFactory);
// 		DEBUG_ERROR("IDXGIDevice::CreateSwapChainForHwnd() failed.", hr);
// 	}
// 
// 	ReleaseObject(dxgiDevice);
// 	ReleaseObject(dxgiAdapter);
// 	ReleaseObject(dxgiFactory);
// 
// 	ID3D11Texture2D* backBuffer;
// 	if (FAILED(hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer))))
// 	{
// 		DEBUG_ERROR("IDXGISwapChain::GetBuffer() failed.", hr);
// 	}
// 
// 	backBuffer->GetDesc(&mBackBufferDesc);
// 
// 	if (FAILED(hr = m_pDevice->CreateRenderTargetView(backBuffer, nullptr, &mRenderTargetView)))
// 	{
// 		ReleaseObject(backBuffer);
// 		DEBUG_ERROR("IDXGIDevice::CreateRenderTargetView() failed.", hr);
// 	}
// 
// 	ReleaseObject(backBuffer);
// 
// 	if (mDepthStencilBufferEnabled)
// 	{
// 		D3D11_TEXTURE2D_DESC depthStencilDesc;
// 		ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
// 		depthStencilDesc.Width = mScreenWidth;
// 		depthStencilDesc.Height = mScreenHeight;
// 		depthStencilDesc.MipLevels = 1;
// 		depthStencilDesc.ArraySize = 1;
// 		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
// 		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
// 		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
// 
// 		if (mMultiSamplingEnabled)
// 		{
// 			depthStencilDesc.SampleDesc.Count = mMultiSamplingCount;
// 			depthStencilDesc.SampleDesc.Quality = mMultiSamplingQualityLevels - 1;
// 		}
// 		else
// 		{
// 			depthStencilDesc.SampleDesc.Count = 1;
// 			depthStencilDesc.SampleDesc.Quality = 0;
// 		}
// 
// 		if (FAILED(hr = m_pDevice->CreateTexture2D(&depthStencilDesc, nullptr, &mDepthStencilBuffer)))
// 		{
// 			DEBUG_ERROR("IDXGIDevice::CreateTexture2D() failed.", hr);
// 		}
// 
// 		if (FAILED(hr = m_pDevice->CreateDepthStencilView(mDepthStencilBuffer, nullptr, &mDepthStencilView)))
// 		{
// 			DEBUG_ERROR("IDXGIDevice::CreateDepthStencilView() failed.", hr);
// 		}
// 	}
// 
// 	mDirect3DDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
// 
// 	mViewport.TopLeftX = 0.0f;
// 	mViewport.TopLeftY = 0.0f;
// 	mViewport.Width = static_cast<float>(mScreenWidth);
// 	mViewport.Height = static_cast<float>(mScreenHeight);
// 	mViewport.MinDepth = 0.0f;
// 	mViewport.MaxDepth = 1.0f;
// 
// 	mDirect3DDeviceContext->RSSetViewports(1, &mViewport);

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

	m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, m_MultiSamplingCount, &m_MultiSamplingQualityLevels);
	if (m_MultiSamplingQualityLevels == 0)
	{
		DEBUG_WARNING("Unsupported multi-sampling quality");
	}
}
