#include "TinyEngineBase.h"
#include <crtdbg.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (g_BaseApp != nullptr)
	{
		g_BaseApp->HandleMessages(hWnd, uMsg, wParam, lParam);
	}
	return (DefWindowProc(hWnd, uMsg, wParam, lParam));
}

INT WINAPI GameMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	int tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
	tmpDbgFlag |= _CRTDBG_CHECK_ALWAYS_DF;
	_CrtSetDbgFlag(tmpDbgFlag);

	if (g_BaseApp != nullptr)
	{
		g_BaseApp->SetupWindow(hInstance, WndProc);
	}

	return 0;
}