#pragma once
#include "../RenderEngineInterface.h"
#include <DXUTGui.h>

class D3DRenderer : public IRenderer
{
public:
	static CDXUTDialogResourceManager g_DialogResourceManager;
	static CDXUTTextHelper* g_pTextHelper;

	virtual HRESULT VOnRestore() { return S_OK; }
	virtual void VShutdown() { SAFE_DELETE(g_pTextHelper); }
};

class D3DRenderer11 : public D3DRenderer
{
public:
	D3DRenderer11() : m_BackgroundColor(Colors::CornflowerBlue.f) {}
	virtual void VShutdown() { D3DRenderer::VShutdown(); }

	virtual void VSetBackgroundColor(Color color)
	{
		m_BackgroundColor = color;
	}

	virtual bool VPreRender();
	virtual bool VPostRender();
	virtual HRESULT VOnRestore();

	virtual void VSetWorldTransform(const Matrix *m) {  }
	virtual void VSetViewTransform(const Matrix *m) {  }
	virtual void VSetProjectionTransform(const Matrix *m) {  }

protected:
	Color m_BackgroundColor;
};

