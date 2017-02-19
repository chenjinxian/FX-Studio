#pragma once

#include "../TinyEngineBase.h"
#include "../TinyEngineInterface.h"

class FullScreenRenderTarget : public boost::noncopyable
{
public:
	FullScreenRenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, bool material = false);
	~FullScreenRenderTarget();

	ID3D11Texture2D* GetOutputTexture() { return m_OutputResource; }
	ID3D11ShaderResourceView* GetOutputResource() { return m_OutputTexture; }
	ID3D11RenderTargetView* GetRenderTargetView() { return m_RenderTargetView; }
	ID3D11DepthStencilView* GetDepthStencilView() { return m_DepthStencilView; }

private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	ID3D11Texture2D* m_OutputResource;
	ID3D11ShaderResourceView* m_OutputTexture;
	ID3D11RenderTargetView* m_RenderTargetView;
	ID3D11DepthStencilView* m_DepthStencilView;
};
