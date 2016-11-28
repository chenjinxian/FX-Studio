#include "D3D11Renderer.h"
#include "../AppFramework/BaseGameApp.h"
#include "imgui.h"
#include <d3dcompiler.h>

static ID3D11Buffer*            g_pVB = NULL;
static ID3D11Buffer*            g_pIB = NULL;
static ID3D10Blob *             g_pVertexShaderBlob = NULL;
static ID3D11VertexShader*      g_pVertexShader = NULL;
static ID3D11InputLayout*       g_pInputLayout = NULL;
static ID3D11Buffer*            g_pVertexConstantBuffer = NULL;
static ID3D10Blob *             g_pPixelShaderBlob = NULL;
static ID3D11PixelShader*       g_pPixelShader = NULL;
static ID3D11SamplerState*      g_pFontSampler = NULL;
static ID3D11ShaderResourceView*g_pFontTextureView = NULL;
static ID3D11RasterizerState*   g_pRasterizerState = NULL;
static ID3D11BlendState*        g_pBlendState = NULL;
static ID3D11DepthStencilState* g_pDepthStencilState = NULL;
static int                      g_VertexBufferSize = 5000, g_IndexBufferSize = 10000;

struct VERTEX_CONSTANT_BUFFER
{
	float        mvp[4][4];
};

D3D11Renderer::D3D11Renderer()
	: m_pDevice(nullptr),
	m_pDeviceContext(nullptr),
	m_pSwapChain(nullptr),
	m_pDepthStencilBuffer(nullptr),
	m_pRenderTargetView(nullptr),
	m_pDepthStencilView(nullptr),
	m_DeviceName(),
	m_BackgroundColor()
{
	InitD3D11Device();
}

D3D11Renderer::~D3D11Renderer()
{
}

void D3D11Renderer::VSetBackgroundColor(const Color& color)
{
	m_BackgroundColor = color;
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
	swapChainDesc.SampleDesc.Quality = 0;
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

	DXGI_ADAPTER_DESC desc;
	dxgiAdapter->GetDesc(&desc);
	m_DeviceName = Utility::WS2S(desc.Description);

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

	CreateBuffers();
	InitImGui(hWnd);
	CreateImGuiBuffers();

	return true;
}

void D3D11Renderer::VDeleteRenderer()
{
	DeleteImGuiBuffers();
	ImGui::Shutdown();
	DeleteBuffers();

	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pDeviceContext);
	SAFE_RELEASE(m_pDevice);
}

void D3D11Renderer::VResizeSwapChain()
{
	DeleteImGuiBuffers();
	DeleteBuffers();
	m_pSwapChain->ResizeBuffers(0, g_pApp->m_Config.m_ScreenWidth, g_pApp->m_Config.m_ScreenHeight, DXGI_FORMAT_UNKNOWN, 0);
	CreateBuffers();
	CreateImGuiBuffers();
}

bool D3D11Renderer::VPreRender(const GameTime& gameTime)
{
	if (m_pDeviceContext != nullptr && m_pRenderTargetView != nullptr)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DeltaTime = gameTime.GetElapsedTime();
		ImGui::NewFrame();

		m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, m_BackgroundColor);
		m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}
	return true;
}

bool D3D11Renderer::VPostRender()
{
	ImGui::Render();
	RenderDrawLists();
	m_pSwapChain->Present(g_pApp->m_Config.m_IsVSync ? 1 : 0, 0);
	return true;
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
		uint32_t qualityLevel = 0;
		if (SUCCEEDED(m_pDevice->CheckMultisampleQualityLevels(
			DXGI_FORMAT_R8G8B8A8_UNORM, g_pApp->m_Config.m_AntiAliasingSample, &qualityLevel)) && qualityLevel > 0)
		{
			break;
		}
		else
		{
			g_pApp->m_Config.m_AntiAliasingSample /= 2;
		}
	}
}

void D3D11Renderer::CreateBuffers()
{
	HRESULT hr;
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
	depthStencilDesc.SampleDesc.Quality = 0;

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
}

void D3D11Renderer::DeleteBuffers()
{
	if (m_pDeviceContext != nullptr)
	{
		m_pDeviceContext->OMSetRenderTargets(0, nullptr, nullptr);
	}
	SAFE_RELEASE(m_pRenderTargetView);
	SAFE_RELEASE(m_pDepthStencilView);
	SAFE_RELEASE(m_pDepthStencilBuffer);
}

bool D3D11Renderer::CreateImGuiBuffers()
{
	{
		static const char* vertexShader =
			"cbuffer vertexBuffer : register(b0) \
			{\
			float4x4 ProjectionMatrix; \
			};\
			struct VS_INPUT\
			{\
			float2 pos : POSITION;\
			float4 col : COLOR0;\
			float2 uv  : TEXCOORD0;\
			};\
			\
			struct PS_INPUT\
			{\
			float4 pos : SV_POSITION;\
			float4 col : COLOR0;\
			float2 uv  : TEXCOORD0;\
			};\
			\
			PS_INPUT main(VS_INPUT input)\
			{\
			PS_INPUT output;\
			output.pos = mul( ProjectionMatrix, float4(input.pos.xy, 0.f, 1.f));\
			output.col = input.col;\
			output.uv  = input.uv;\
			return output;\
			}";

		D3DCompile(vertexShader, strlen(vertexShader), NULL, NULL, NULL, "main", "vs_4_0", 0, 0, &g_pVertexShaderBlob, NULL);
		if (g_pVertexShaderBlob == NULL) // NB: Pass ID3D10Blob* pErrorBlob to D3DCompile() to get error showing in (const char*)pErrorBlob->GetBufferPointer(). Make sure to Release() the blob!
			return false;
		if (m_pDevice->CreateVertexShader((DWORD*)g_pVertexShaderBlob->GetBufferPointer(), g_pVertexShaderBlob->GetBufferSize(), NULL, &g_pVertexShader) != S_OK)
			return false;

		// Create the input layout
		D3D11_INPUT_ELEMENT_DESC local_layout[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT,   0, (size_t)(&((ImDrawVert*)0)->pos), D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,   0, (size_t)(&((ImDrawVert*)0)->uv),  D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, (size_t)(&((ImDrawVert*)0)->col), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		if (m_pDevice->CreateInputLayout(local_layout, 3, g_pVertexShaderBlob->GetBufferPointer(), g_pVertexShaderBlob->GetBufferSize(), &g_pInputLayout) != S_OK)
			return false;

		// Create the constant buffer
		{
			D3D11_BUFFER_DESC desc;
			desc.ByteWidth = sizeof(VERTEX_CONSTANT_BUFFER);
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			desc.MiscFlags = 0;
			m_pDevice->CreateBuffer(&desc, NULL, &g_pVertexConstantBuffer);
		}
	}

	// Create the pixel shader
	{
		static const char* pixelShader =
			"struct PS_INPUT\
			{\
			float4 pos : SV_POSITION;\
			float4 col : COLOR0;\
			float2 uv  : TEXCOORD0;\
			};\
			sampler sampler0;\
			Texture2D texture0;\
			\
			float4 main(PS_INPUT input) : SV_Target\
			{\
			float4 out_col = input.col * texture0.Sample(sampler0, input.uv); \
			return out_col; \
			}";

		D3DCompile(pixelShader, strlen(pixelShader), NULL, NULL, NULL, "main", "ps_4_0", 0, 0, &g_pPixelShaderBlob, NULL);
		if (g_pPixelShaderBlob == NULL)  // NB: Pass ID3D10Blob* pErrorBlob to D3DCompile() to get error showing in (const char*)pErrorBlob->GetBufferPointer(). Make sure to Release() the blob!
			return false;
		if (m_pDevice->CreatePixelShader((DWORD*)g_pPixelShaderBlob->GetBufferPointer(), g_pPixelShaderBlob->GetBufferSize(), NULL, &g_pPixelShader) != S_OK)
			return false;
	}

	// Create the blending setup
	{
		D3D11_BLEND_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.AlphaToCoverageEnable = false;
		desc.RenderTarget[0].BlendEnable = true;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		m_pDevice->CreateBlendState(&desc, &g_pBlendState);
	}

	// Create the rasterizer state
	{
		D3D11_RASTERIZER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_NONE;
		desc.ScissorEnable = true;
		desc.DepthClipEnable = true;
		m_pDevice->CreateRasterizerState(&desc, &g_pRasterizerState);
	}

	// Create depth-stencil State
	{
		D3D11_DEPTH_STENCIL_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.DepthEnable = false;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
		desc.StencilEnable = false;
		desc.FrontFace.StencilFailOp = desc.FrontFace.StencilDepthFailOp = desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		desc.BackFace = desc.FrontFace;
		m_pDevice->CreateDepthStencilState(&desc, &g_pDepthStencilState);
	}

	ImGuiIO& io = ImGui::GetIO();
	unsigned char* pixels;
	int width, height;
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

	// Upload texture to graphics system
	{
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;

		ID3D11Texture2D *pTexture = NULL;
		D3D11_SUBRESOURCE_DATA subResource;
		subResource.pSysMem = pixels;
		subResource.SysMemPitch = desc.Width * 4;
		subResource.SysMemSlicePitch = 0;
		m_pDevice->CreateTexture2D(&desc, &subResource, &pTexture);

		// Create texture view
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = desc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;
		m_pDevice->CreateShaderResourceView(pTexture, &srvDesc, &g_pFontTextureView);
		pTexture->Release();
	}

	// Store our identifier
	io.Fonts->TexID = (void *)g_pFontTextureView;

	// Create texture sampler
	{
		D3D11_SAMPLER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.MipLODBias = 0.f;
		desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		desc.MinLOD = 0.f;
		desc.MaxLOD = 0.f;
		m_pDevice->CreateSamplerState(&desc, &g_pFontSampler);
	}
	
	io.DisplaySize = ImVec2((float)g_pApp->m_Config.m_ScreenWidth, (float)g_pApp->m_Config.m_ScreenHeight);
	return true;
}

void D3D11Renderer::DeleteImGuiBuffers()
{
	if (g_pFontSampler) { g_pFontSampler->Release(); g_pFontSampler = NULL; }
	if (g_pFontTextureView) { g_pFontTextureView->Release(); g_pFontTextureView = NULL; ImGui::GetIO().Fonts->TexID = 0; }
	if (g_pIB) { g_pIB->Release(); g_pIB = NULL; }
	if (g_pVB) { g_pVB->Release(); g_pVB = NULL; }

	if (g_pBlendState) { g_pBlendState->Release(); g_pBlendState = NULL; }
	if (g_pDepthStencilState) { g_pDepthStencilState->Release(); g_pDepthStencilState = NULL; }
	if (g_pRasterizerState) { g_pRasterizerState->Release(); g_pRasterizerState = NULL; }
	if (g_pPixelShader) { g_pPixelShader->Release(); g_pPixelShader = NULL; }
	if (g_pPixelShaderBlob) { g_pPixelShaderBlob->Release(); g_pPixelShaderBlob = NULL; }
	if (g_pVertexConstantBuffer) { g_pVertexConstantBuffer->Release(); g_pVertexConstantBuffer = NULL; }
	if (g_pInputLayout) { g_pInputLayout->Release(); g_pInputLayout = NULL; }
	if (g_pVertexShader) { g_pVertexShader->Release(); g_pVertexShader = NULL; }
	if (g_pVertexShaderBlob) { g_pVertexShaderBlob->Release(); g_pVertexShaderBlob = NULL; }
}

void D3D11Renderer::RenderDrawLists()
{
	ImDrawData* draw_data = ImGui::GetDrawData();

	// Create and grow vertex/index buffers if needed
	if (!g_pVB || g_VertexBufferSize < draw_data->TotalVtxCount)
	{
		if (g_pVB) { g_pVB->Release(); g_pVB = NULL; }
		g_VertexBufferSize = draw_data->TotalVtxCount + 5000;
		D3D11_BUFFER_DESC desc;
		memset(&desc, 0, sizeof(D3D11_BUFFER_DESC));
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.ByteWidth = g_VertexBufferSize * sizeof(ImDrawVert);
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		if (m_pDevice->CreateBuffer(&desc, NULL, &g_pVB) < 0)
			return;
	}
	if (!g_pIB || g_IndexBufferSize < draw_data->TotalIdxCount)
	{
		if (g_pIB) { g_pIB->Release(); g_pIB = NULL; }
		g_IndexBufferSize = draw_data->TotalIdxCount + 10000;
		D3D11_BUFFER_DESC desc;
		memset(&desc, 0, sizeof(D3D11_BUFFER_DESC));
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.ByteWidth = g_IndexBufferSize * sizeof(ImDrawIdx);
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		if (m_pDevice->CreateBuffer(&desc, NULL, &g_pIB) < 0)
			return;
	}

	// Copy and convert all vertices into a single contiguous buffer
	D3D11_MAPPED_SUBRESOURCE vtx_resource, idx_resource;
	if (m_pDeviceContext->Map(g_pVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &vtx_resource) != S_OK)
		return;
	if (m_pDeviceContext->Map(g_pIB, 0, D3D11_MAP_WRITE_DISCARD, 0, &idx_resource) != S_OK)
		return;
	ImDrawVert* vtx_dst = (ImDrawVert*)vtx_resource.pData;
	ImDrawIdx* idx_dst = (ImDrawIdx*)idx_resource.pData;
	for (int n = 0; n < draw_data->CmdListsCount; n++)
	{
		const ImDrawList* cmd_list = draw_data->CmdLists[n];
		memcpy(vtx_dst, cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Size * sizeof(ImDrawVert));
		memcpy(idx_dst, cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));
		vtx_dst += cmd_list->VtxBuffer.Size;
		idx_dst += cmd_list->IdxBuffer.Size;
	}
	m_pDeviceContext->Unmap(g_pVB, 0);
	m_pDeviceContext->Unmap(g_pIB, 0);

	// Setup orthographic projection matrix into our constant buffer
	{
		D3D11_MAPPED_SUBRESOURCE mapped_resource;
		if (m_pDeviceContext->Map(g_pVertexConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource) != S_OK)
			return;
		VERTEX_CONSTANT_BUFFER* constant_buffer = (VERTEX_CONSTANT_BUFFER*)mapped_resource.pData;
		float L = 0.0f;
		float R = ImGui::GetIO().DisplaySize.x;
		float B = ImGui::GetIO().DisplaySize.y;
		float T = 0.0f;
		float mvp[4][4] =
		{
			{ 2.0f / (R - L),   0.0f,           0.0f,       0.0f },
			{ 0.0f,         2.0f / (T - B),     0.0f,       0.0f },
			{ 0.0f,         0.0f,           0.5f,       0.0f },
			{ (R + L) / (L - R),  (T + B) / (B - T),    0.5f,       1.0f },
		};
		memcpy(&constant_buffer->mvp, mvp, sizeof(mvp));
		m_pDeviceContext->Unmap(g_pVertexConstantBuffer, 0);
	}

	// Backup DX state that will be modified to restore it afterwards (unfortunately this is very ugly looking and verbose. Close your eyes!)
	struct BACKUP_DX11_STATE
	{
		UINT                        ScissorRectsCount, ViewportsCount;
		D3D11_RECT                  ScissorRects[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
		D3D11_VIEWPORT              Viewports[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
		ID3D11RasterizerState*      RS;
		ID3D11BlendState*           BlendState;
		FLOAT                       BlendFactor[4];
		UINT                        SampleMask;
		UINT                        StencilRef;
		ID3D11DepthStencilState*    DepthStencilState;
		ID3D11ShaderResourceView*   PSShaderResource;
		ID3D11SamplerState*         PSSampler;
		ID3D11PixelShader*          PS;
		ID3D11VertexShader*         VS;
		UINT                        PSInstancesCount, VSInstancesCount;
		ID3D11ClassInstance*        PSInstances[256], *VSInstances[256];   // 256 is max according to PSSetShader documentation
		D3D11_PRIMITIVE_TOPOLOGY    PrimitiveTopology;
		ID3D11Buffer*               IndexBuffer, *VertexBuffer, *VSConstantBuffer;
		UINT                        IndexBufferOffset, VertexBufferStride, VertexBufferOffset;
		DXGI_FORMAT                 IndexBufferFormat;
		ID3D11InputLayout*          InputLayout;
	};
	BACKUP_DX11_STATE old;
	old.ScissorRectsCount = old.ViewportsCount = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
	m_pDeviceContext->RSGetScissorRects(&old.ScissorRectsCount, old.ScissorRects);
	m_pDeviceContext->RSGetViewports(&old.ViewportsCount, old.Viewports);
	m_pDeviceContext->RSGetState(&old.RS);
	m_pDeviceContext->OMGetBlendState(&old.BlendState, old.BlendFactor, &old.SampleMask);
	m_pDeviceContext->OMGetDepthStencilState(&old.DepthStencilState, &old.StencilRef);
	m_pDeviceContext->PSGetShaderResources(0, 1, &old.PSShaderResource);
	m_pDeviceContext->PSGetSamplers(0, 1, &old.PSSampler);
	old.PSInstancesCount = old.VSInstancesCount = 256;
	m_pDeviceContext->PSGetShader(&old.PS, old.PSInstances, &old.PSInstancesCount);
	m_pDeviceContext->VSGetShader(&old.VS, old.VSInstances, &old.VSInstancesCount);
	m_pDeviceContext->VSGetConstantBuffers(0, 1, &old.VSConstantBuffer);
	m_pDeviceContext->IAGetPrimitiveTopology(&old.PrimitiveTopology);
	m_pDeviceContext->IAGetIndexBuffer(&old.IndexBuffer, &old.IndexBufferFormat, &old.IndexBufferOffset);
	m_pDeviceContext->IAGetVertexBuffers(0, 1, &old.VertexBuffer, &old.VertexBufferStride, &old.VertexBufferOffset);
	m_pDeviceContext->IAGetInputLayout(&old.InputLayout);

	// Setup viewport
	D3D11_VIEWPORT vp;
	memset(&vp, 0, sizeof(D3D11_VIEWPORT));
	vp.Width = ImGui::GetIO().DisplaySize.x;
	vp.Height = ImGui::GetIO().DisplaySize.y;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = vp.TopLeftY = 0.0f;
	m_pDeviceContext->RSSetViewports(1, &vp);

	// Bind shader and vertex buffers
	unsigned int stride = sizeof(ImDrawVert);
	unsigned int offset = 0;
	m_pDeviceContext->IASetInputLayout(g_pInputLayout);
	m_pDeviceContext->IASetVertexBuffers(0, 1, &g_pVB, &stride, &offset);
	m_pDeviceContext->IASetIndexBuffer(g_pIB, sizeof(ImDrawIdx) == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT, 0);
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pDeviceContext->VSSetShader(g_pVertexShader, NULL, 0);
	m_pDeviceContext->VSSetConstantBuffers(0, 1, &g_pVertexConstantBuffer);
	m_pDeviceContext->PSSetShader(g_pPixelShader, NULL, 0);
	m_pDeviceContext->PSSetSamplers(0, 1, &g_pFontSampler);

	// Setup render state
	const float blend_factor[4] = { 0.f, 0.f, 0.f, 0.f };
	m_pDeviceContext->OMSetBlendState(g_pBlendState, blend_factor, 0xffffffff);
	m_pDeviceContext->OMSetDepthStencilState(g_pDepthStencilState, 0);
	m_pDeviceContext->RSSetState(g_pRasterizerState);

	// Render command lists
	int vtx_offset = 0;
	int idx_offset = 0;
	for (int n = 0; n < draw_data->CmdListsCount; n++)
	{
		const ImDrawList* cmd_list = draw_data->CmdLists[n];
		for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
		{
			const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
			if (pcmd->UserCallback)
			{
				pcmd->UserCallback(cmd_list, pcmd);
			}
			else
			{
				const D3D11_RECT r = { (LONG)pcmd->ClipRect.x, (LONG)pcmd->ClipRect.y, (LONG)pcmd->ClipRect.z, (LONG)pcmd->ClipRect.w };
				m_pDeviceContext->PSSetShaderResources(0, 1, (ID3D11ShaderResourceView**)&pcmd->TextureId);
				m_pDeviceContext->RSSetScissorRects(1, &r);
				m_pDeviceContext->DrawIndexed(pcmd->ElemCount, idx_offset, vtx_offset);
			}
			idx_offset += pcmd->ElemCount;
		}
		vtx_offset += cmd_list->VtxBuffer.Size;
	}

	// Restore modified DX state
	m_pDeviceContext->RSSetScissorRects(old.ScissorRectsCount, old.ScissorRects);
	m_pDeviceContext->RSSetViewports(old.ViewportsCount, old.Viewports);
	m_pDeviceContext->RSSetState(old.RS); if (old.RS) old.RS->Release();
	m_pDeviceContext->OMSetBlendState(old.BlendState, old.BlendFactor, old.SampleMask); if (old.BlendState) old.BlendState->Release();
	m_pDeviceContext->OMSetDepthStencilState(old.DepthStencilState, old.StencilRef); if (old.DepthStencilState) old.DepthStencilState->Release();
	m_pDeviceContext->PSSetShaderResources(0, 1, &old.PSShaderResource); if (old.PSShaderResource) old.PSShaderResource->Release();
	m_pDeviceContext->PSSetSamplers(0, 1, &old.PSSampler); if (old.PSSampler) old.PSSampler->Release();
	m_pDeviceContext->PSSetShader(old.PS, old.PSInstances, old.PSInstancesCount); if (old.PS) old.PS->Release();
	for (UINT i = 0; i < old.PSInstancesCount; i++) if (old.PSInstances[i]) old.PSInstances[i]->Release();
	m_pDeviceContext->VSSetShader(old.VS, old.VSInstances, old.VSInstancesCount); if (old.VS) old.VS->Release();
	m_pDeviceContext->VSSetConstantBuffers(0, 1, &old.VSConstantBuffer); if (old.VSConstantBuffer) old.VSConstantBuffer->Release();
	for (UINT i = 0; i < old.VSInstancesCount; i++) if (old.VSInstances[i]) old.VSInstances[i]->Release();
	m_pDeviceContext->IASetPrimitiveTopology(old.PrimitiveTopology);
	m_pDeviceContext->IASetIndexBuffer(old.IndexBuffer, old.IndexBufferFormat, old.IndexBufferOffset); if (old.IndexBuffer) old.IndexBuffer->Release();
	m_pDeviceContext->IASetVertexBuffers(0, 1, &old.VertexBuffer, &old.VertexBufferStride, &old.VertexBufferOffset); if (old.VertexBuffer) old.VertexBuffer->Release();
	m_pDeviceContext->IASetInputLayout(old.InputLayout); if (old.InputLayout) old.InputLayout->Release();
}

bool D3D11Renderer::InitImGui(HWND hWnd)
{
	ImGuiIO& io = ImGui::GetIO();
	io.KeyMap[ImGuiKey_Tab] = VK_TAB;
	io.KeyMap[ImGuiKey_LeftArrow] = VK_LEFT;
	io.KeyMap[ImGuiKey_RightArrow] = VK_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow] = VK_UP;
	io.KeyMap[ImGuiKey_DownArrow] = VK_DOWN;
	io.KeyMap[ImGuiKey_PageUp] = VK_PRIOR;
	io.KeyMap[ImGuiKey_PageDown] = VK_NEXT;
	io.KeyMap[ImGuiKey_Home] = VK_HOME;
	io.KeyMap[ImGuiKey_End] = VK_END;
	io.KeyMap[ImGuiKey_Delete] = VK_DELETE;
	io.KeyMap[ImGuiKey_Backspace] = VK_BACK;
	io.KeyMap[ImGuiKey_Enter] = VK_RETURN;
	io.KeyMap[ImGuiKey_Escape] = VK_ESCAPE;
	io.KeyMap[ImGuiKey_A] = 'A';
	io.KeyMap[ImGuiKey_C] = 'C';
	io.KeyMap[ImGuiKey_V] = 'V';
	io.KeyMap[ImGuiKey_X] = 'X';
	io.KeyMap[ImGuiKey_Y] = 'Y';
	io.KeyMap[ImGuiKey_Z] = 'Z';
	io.RenderDrawListsFn = nullptr;
	io.ImeWindowHandle = hWnd;
	io.Fonts->AddFontFromFileTTF("Fonts/Cousine-Regular.ttf", 20.0f);

	return true;
}
