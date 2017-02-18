#pragma once
#include "../TinyEngineBase.h"
#include "../TinyEngineInterface.h"

class D3D11Renderer : public IRenderer
{
public:
	D3D11Renderer();
	virtual ~D3D11Renderer();

	virtual bool VInitRenderer(HWND hWnd, HWND hMaterialWnd) override;
	virtual void VDeleteRenderer() override;
	virtual void VResizeSwapChain() override;
	virtual bool VPreRender(const GameTime& gameTime, int index) override;
	virtual bool VPostRender(int index) override;
	virtual void VSetBackgroundColor(const Color& color) override;

	virtual shared_ptr<IRenderState> VPrepareAlphaPass() override;
	virtual shared_ptr<IRenderState> VPrepareSkyBoxPass() override;
	virtual shared_ptr<IRenderState> VPrepareDebugPass() override;

	virtual void VInputSetup(D3D_PRIMITIVE_TOPOLOGY topology, ID3D11InputLayout* pInputLayout) override;
	virtual void VSetVertexBuffers(ID3D11Buffer* pVertexBuffer, uint32_t* stride, uint32_t* offset) override;
	virtual void VSetIndexBuffer(ID3D11Buffer* pIndexBuffer, IRenderer::IndexFormat format, uint32_t offset) override;
	virtual void VDrawMesh(uint32_t indexCount, uint32_t startIndex, int32_t baseVertex, ID3DX11EffectPass* pD3DX11EffectPass) override;
	virtual void VResetShader(bool geoemtryShader, bool hullShader, bool domainShader) override;

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
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	IDXGISwapChain* m_pSwapChain[2];

	ID3D11RenderTargetView* m_pRenderTargetView[2];
	ID3D11DepthStencilView* m_pDepthStencilView;
	D3D11_VIEWPORT m_Viewport;

	std::string m_DeviceName;
	Color m_BackgroundColor;
};

