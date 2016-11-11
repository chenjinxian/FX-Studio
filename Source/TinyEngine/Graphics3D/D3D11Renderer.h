#pragma once
#include "../TinyEngineBase.h"
#include "../TinyEngineInterface.h"

class D3D11Renderer : public IRenderer
{
public:
	D3D11Renderer();
	virtual ~D3D11Renderer();

	virtual void VSetBackgroundColor(const Color& color) override;
	virtual bool VInitAPI() override;
	virtual bool VPreRender() override;
	virtual bool VPostRender() override;
	virtual void VShutdown() override;

private:
	D3D_FEATURE_LEVEL m_FeatureLevel;
	ID3D11Device1* m_pDevice;
	ID3D11DeviceContext1* m_pDeviceContext;
	IDXGISwapChain1* m_pSwapChain;

	UINT m_FrameRate;
	bool m_IsFullScreen;
	bool m_DepthStencilBufferEnabled;
	bool m_MultiSamplingEnabled;
	UINT m_MultiSamplingCount;
	UINT m_MultiSamplingQualityLevels;

	ID3D11Texture2D* m_pDepthStencilBuffer;
	D3D11_TEXTURE2D_DESC m_BackBufferDesc;
	ID3D11RenderTargetView* m_pRenderTargetView;
	ID3D11DepthStencilView* m_pDepthStencilView;
	D3D11_VIEWPORT m_Viewport;
};

