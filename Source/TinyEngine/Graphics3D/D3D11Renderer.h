#pragma once
#include "../TinyEngineBase.h"
#include "../TinyEngineInterface.h"

class D3D11Renderer : public IRenderer
{
public:
	D3D11Renderer();
	virtual ~D3D11Renderer();

	virtual bool VInitRenderer(HWND hWnd) override;
	virtual void VDeleteRenderer() override;
	virtual void VResizeSwapChain() override;
	virtual bool VPreRender(const GameTime& gameTime) override;
	virtual bool VPostRender() override;
	virtual void VSetBackgroundColor(const Color& color) override;

	virtual void VInputSetup(D3D_PRIMITIVE_TOPOLOGY topology, ID3D11InputLayout* pInputLayout) override;
	virtual void VDrawMeshe(uint32_t vertexSize, ID3D11Buffer* pVertexBuffer,
		ID3D11Buffer* pIndexBuffer, uint32_t indexCount, ID3DX11EffectPass* pD3DX11EffectPass) override;

	virtual bool VCompileShaderFromMemory(const void* pBuffer, uint32_t lenght, shared_ptr<IResourceExtraData> pExtraData) override;
	virtual bool VCreateShaderFromMemory(const void* pBuffer, uint32_t lenght, shared_ptr<IResourceExtraData> pExtraData) override;
	virtual bool VCreateDDSTextureResoure(char *rawBuffer, uint32_t rawSize, shared_ptr<IResourceExtraData> pExtraData) override;
	virtual bool VCreateWICTextureResoure(char *rawBuffer, uint32_t rawSize, shared_ptr<IResourceExtraData> pExtraData) override;
	virtual const std::string& VGetDeviceName() override { return m_DeviceName; }

private:
	void InitD3D11Device();
	bool InitImGui(HWND hWnd);
	void CreateBuffers();
	void DeleteBuffers();
	bool CreateImGuiBuffers();
	void DeleteImGuiBuffers();
	void RenderDrawLists();

	D3D_FEATURE_LEVEL m_FeatureLevel;
	ID3D11Device1* m_pDevice;
	ID3D11DeviceContext1* m_pDeviceContext;
	IDXGISwapChain1* m_pSwapChain;

	ID3D11Texture2D* m_pDepthStencilBuffer;
	D3D11_TEXTURE2D_DESC m_BackBufferDesc;
	ID3D11RenderTargetView* m_pRenderTargetView;
	ID3D11DepthStencilView* m_pDepthStencilView;
	D3D11_VIEWPORT m_Viewport;

	std::string m_DeviceName;
	Color m_BackgroundColor;
};

