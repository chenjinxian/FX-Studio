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
	HWND SetupWindow(HINSTANCE hInstance);
	bool InitRenderer();
	void RenderLoop();

	int GetScreenWidth() const { return m_Config.m_ScreenWidth; }
	int GetScreenHeight() const { return m_Config.m_ScreenHeight; }

	GameConfig m_Config;
	unique_ptr<ResCache> m_pResCache;
	shared_ptr<IRenderer> m_pRenderer;

protected:
	virtual const wchar_t* VGetWindowTitle() = 0;
	virtual const wchar_t* VGetWindowClass() = 0;
	virtual HICON VGetIcon() = 0;

private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	HINSTANCE m_hInstance;
	HWND m_hWindow;

	GameTime m_GameTime;
};

extern BaseGameApp* g_pApp;