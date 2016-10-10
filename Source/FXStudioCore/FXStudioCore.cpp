// FXStudioCore.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "FXStudioCore.h"
#include "FXStudioApp.h"
#include "FXStudioView.h"

static std::unique_ptr<FXStudioApp> app = nullptr;

FXSTUDIOCORE_API int CreateInstance(
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

FXSTUDIOCORE_API int DestroyInstance()
{
	return app->Exit();
}

FXSTUDIOCORE_API void WndProc(int *hWndPtrAddress, int msg, int wParam, int lParam)
{
	HWND hWnd = (HWND)hWndPtrAddress;
	DXUTStaticWndProc(hWnd, msg, WPARAM(wParam), LPARAM(lParam));
}

FXSTUDIOCORE_API void RenderFrame()
{
	DXUTRender3DEnvironment();
}

FXSTUDIOCORE_API void OpenLevel(BSTR lFileName)
{
	std::string levelFile = ws2s(std::wstring(lFileName, SysStringLen(lFileName)));
	FXStudioLogic* pGameLogic = dynamic_cast<FXStudioLogic*>(app->GetGameLogic());
	if (pGameLogic != nullptr)
	{
// 		std::string assetsDir = "\\Assets\\";
// 		int projDirLength = pGameLogic->GetProjectDirectory().length() + assetsDir.length();
// 		app->m_Options.m_Level = levelFile.substr(projDirLength, levelFile.length() - projDirLength);
// 		pEditorLogic->VChangeState(BGS_LoadingGameEnvironment);
		if (!app->VLoadGame(levelFile))
		{
			GCC_ERROR("The game failed to load.");
		}
	}
}
