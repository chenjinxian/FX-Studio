// FX_StudioDll.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "FX_StudioDll.h"
#include "FXStudioApp.h"

static std::unique_ptr<FXStudioApp> app = nullptr;

FX_STUDIODLL_API int CreateInstance(
	int *instancePtrAddress,
	int *hPrevInstancePtrAddress,
	int *hWndPtrAddress,
	int nCmdShow,
	int screenWidth, int screenHeight)
{
	HINSTANCE hInstance = (HINSTANCE)instancePtrAddress;
	HINSTANCE hPrevInstance = (HINSTANCE)hPrevInstancePtrAddress;
	HWND hWnd = (HWND)hWndPtrAddress;
	WCHAR *lpCmdLine = L"";

	int tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	tmpDbgFlag |= _CRTDBG_DELAY_FREE_MEM_DF;
	tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(tmpDbgFlag);

	app.reset(new FXStudioApp(hInstance, lpCmdLine, hWnd, screenWidth, screenHeight));
	return app->Initialize();
}

FX_STUDIODLL_API int DestroyInstance()
{
	return app->Exit();
}

FX_STUDIODLL_API void WndProc(int *hWndPtrAddress, int msg, int wParam, int lParam)
{
	HWND hWnd = (HWND)hWndPtrAddress;
	DXUTStaticWndProc(hWnd, msg, WPARAM(wParam), LPARAM(lParam));
}

FX_STUDIODLL_API void RenderFrame()
{
	DXUTRender3DEnvironment();
}