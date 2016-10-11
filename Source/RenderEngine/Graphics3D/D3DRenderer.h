#pragma once
#include "../RenderEngineBase.h"
#include "../RenderEngineInterface.h"

class D3DRenderer : public IRenderer
{
public:
	D3DRenderer();
	virtual ~D3DRenderer();

	virtual HRESULT VOnRestore() override { return S_OK; }
	virtual void VShutdown() override { SAFE_DELETE(m_pTextHelper); }
	static CDXUTDialogResourceManager m_DialogResourceManager;
	static CDXUTTextHelper* m_pTextHelper;
};

class D3DRenderer11 : public D3DRenderer
{
public:
	D3DRenderer11();
	~D3DRenderer11();

	virtual void VSetBackgroundColor(const Color& color) override { m_backgroundColor = color; }
	virtual bool VPreRender();
	virtual bool VPostRender();
	virtual HRESULT VOnRestore();
	virtual void VShutdown() { D3DRenderer::VShutdown(); }

private:
	Color m_backgroundColor;
};