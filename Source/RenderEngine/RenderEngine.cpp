// RenderEngine.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "RenderEngine.h"
#include <crtdbg.h>

RENDERENGINE_API int CreateInstance(
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

	return 0;
}

RENDERENGINE_API int DestroyInstance()
{
	return 0;
}

RENDERENGINE_API void WndProc(int *hWndPtrAddress, int msg, int wParam, int lParam)
{

}

RENDERENGINE_API void RenderFrame()
{

}
