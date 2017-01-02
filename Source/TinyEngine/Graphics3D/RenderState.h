#pragma once
#include "../TinyEngineBase.h"
#include "../TinyEngineInterface.h"

class D3D11RendererAlphaPass : public IRenderState
{
public:
	D3D11RendererAlphaPass(ID3D11Device1* pDevice, ID3D11DeviceContext1* pContext)
		: IRenderState(pDevice, pContext)
	{
		m_pDeviceContext->OMGetBlendState(&m_pOldBlendState, m_OldBlendFactor, &m_OldSampleMask);
		m_pCurrentBlendState = nullptr;

		D3D11_BLEND_DESC blendState;
		ZeroMemory(&blendState, sizeof(D3D11_BLEND_DESC));

		blendState.AlphaToCoverageEnable = false;
		blendState.IndependentBlendEnable = false;
		blendState.RenderTarget[0].BlendEnable = TRUE;
		blendState.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendState.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendState.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendState.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
		blendState.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendState.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendState.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		m_pDevice->CreateBlendState(&blendState, &m_pCurrentBlendState);
		m_pDeviceContext->OMSetBlendState(m_pCurrentBlendState, 0, 0xffffffff);
	}

	~D3D11RendererAlphaPass()
	{
		m_pDeviceContext->OMSetBlendState(m_pOldBlendState, m_OldBlendFactor, m_OldSampleMask);
		SAFE_RELEASE(m_pCurrentBlendState);
		SAFE_RELEASE(m_pOldBlendState);
	}
	std::string VToString() { return "D3D11RendererAlphaPass"; }

protected:
	ID3D11BlendState* m_pCurrentBlendState;
	ID3D11BlendState* m_pOldBlendState;
	float m_OldBlendFactor[4];
	uint32_t m_OldSampleMask;
};

class D3D11RendererSkyBoxPass : public IRenderState
{
public:
	D3D11RendererSkyBoxPass(ID3D11Device1* pDevice, ID3D11DeviceContext1* pContext)
		: IRenderState(pDevice, pContext)
	{
		D3D11_DEPTH_STENCIL_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_DEPTH_STENCIL_DESC));
		desc.DepthEnable = TRUE;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		desc.DepthFunc = D3D11_COMPARISON_LESS;
		desc.StencilEnable = FALSE;
		m_pDevice->CreateDepthStencilState(&desc, &m_pSkyboxDepthStencilState);

		uint32_t stencilRef;
		m_pDeviceContext->OMGetDepthStencilState(&m_pOldDepthStencilState, &stencilRef);
		m_pDeviceContext->OMSetDepthStencilState(m_pSkyboxDepthStencilState, 0);
	}

	~D3D11RendererSkyBoxPass()
	{
		m_pDeviceContext->OMSetDepthStencilState(m_pOldDepthStencilState, 0);
		SAFE_RELEASE(m_pOldDepthStencilState);
		SAFE_RELEASE(m_pSkyboxDepthStencilState);
	}

	std::string VToString() { return "D3DRendererSkyBoxPass11"; }

protected:
	ID3D11DepthStencilState* m_pOldDepthStencilState;
	ID3D11DepthStencilState* m_pSkyboxDepthStencilState;
};

class D3D11RendererDebugPass : public IRenderState
{
public:
	D3D11RendererDebugPass(ID3D11Device1* pDevice, ID3D11DeviceContext1* pContext)
		: IRenderState(pDevice, pContext)
	{
		D3D11_DEPTH_STENCIL_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_DEPTH_STENCIL_DESC));
		desc.DepthEnable = FALSE;
		desc.StencilEnable = FALSE;
		m_pDevice->CreateDepthStencilState(&desc, &m_pDebugDepthStencilState);

		uint32_t stencilRef;
		m_pDeviceContext->OMGetDepthStencilState(&m_pOldDepthStencilState, &stencilRef);
		m_pDeviceContext->OMSetDepthStencilState(m_pDebugDepthStencilState, 0);
	}

	~D3D11RendererDebugPass()
	{
		m_pDeviceContext->OMSetDepthStencilState(m_pOldDepthStencilState, 0);
		SAFE_RELEASE(m_pOldDepthStencilState);
		SAFE_RELEASE(m_pDebugDepthStencilState);
	}

	std::string VToString() { return "D3DRendererSkyBoxPass11"; }

protected:
	ID3D11DepthStencilState* m_pOldDepthStencilState;
	ID3D11DepthStencilState* m_pDebugDepthStencilState;
};
