#pragma once
#include "../TinyEngineBase.h"

class TinyEngineApp : public boost::noncopyable
{
public:
	TinyEngineApp();
	virtual ~TinyEngineApp();

	HWND SetupWindow(HINSTANCE hInstance, WNDPROC wndproc);
	void HandleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	int GetScreenWidth() const { return m_ScreenWidth; }
	int GetScreenHeight() const { return m_ScreenHeight; }

	class ResCache *m_pResCache;

protected:
	virtual const std::wstring& VGetWindowTitle() = 0;
	virtual const std::wstring& VGetWindowClass() = 0;
	virtual HICON VGetIcon() = 0;

private:
	HINSTANCE m_hInstance;
	HWND m_hWindow;
	uint32_t m_ScreenWidth;
	uint32_t m_ScreenHeight;
};

extern TinyEngineApp* g_pApp;