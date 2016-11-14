#include "BaseGameApp.h"
#include <crtdbg.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (g_pApp != nullptr)
	{
		g_pApp->HandleMessages(hWnd, uMsg, wParam, lParam);
	}
	return (DefWindowProc(hWnd, uMsg, wParam, lParam));
}

INT WINAPI WindowBaseMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	int tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
	tmpDbgFlag |= _CRTDBG_CHECK_ALWAYS_DF;
	_CrtSetDbgFlag(tmpDbgFlag);

	SetCurrentDirectory(Utility::GetExecutableDirectory().c_str());
	Logger::Init("logging.xml");

	if (g_pApp != nullptr)
	{
		g_pApp->m_Config.InitConfig("TinyEngine.xml", lpCmdLine);
		if (g_pApp->InitEnvironment())
		{
			g_pApp->SetupWindow(hInstance, WndProc);
			g_pApp->InitRenderer();
			g_pApp->RenderLoop();
		}
	}

	Logger::Destroy();
	return 0;
}