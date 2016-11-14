#pragma once
#include "../TinyEngineBase.h"
#include "../TinyEngineInterface.h"
#include "GameConfig.h"
#include "GameTime.h"

class TinyEngineConfig;
class ResCache;

class BaseGameApp : public boost::noncopyable
{
public:
	BaseGameApp();
	virtual ~BaseGameApp();

	bool InitEnvironment();
	HWND SetupWindow(HINSTANCE hInstance, WNDPROC wndproc);
	void HandleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool InitRenderer();
	void RenderLoop();

	int GetScreenWidth() const { return m_ScreenWidth; }
	int GetScreenHeight() const { return m_ScreenHeight; }

	GameConfig m_Config;
	unique_ptr<ResCache> m_pResCache;
	shared_ptr<IRenderer> m_pRenderer;

protected:
	virtual const std::wstring& VGetWindowTitle() = 0;
	virtual const std::wstring& VGetWindowClass() = 0;
	virtual HICON VGetIcon() = 0;

private:
	HINSTANCE m_hInstance;
	HWND m_hWindow;
	uint32_t m_ScreenWidth;
	uint32_t m_ScreenHeight;

	GameTime m_GameTime;
};

extern BaseGameApp* g_pApp;