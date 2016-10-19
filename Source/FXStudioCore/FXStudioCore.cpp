// FXStudioCore.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "FXStudioCore.h"
#include "FXStudioApp.h"
#include "FXStudioView.h"

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

	Logger::Init("logging.xml");

	g_pApp->m_Options.Init("EditorOptions.xml", lpCmdLine);

	DXUTSetCallbackMsgProc(RenderEngineApp::MsgProc);
	DXUTSetCallbackFrameMove(RenderEngineApp::OnFrameMove);
	DXUTSetCallbackDeviceChanging(RenderEngineApp::ModifyDeviceSettings);

	if (g_pApp->m_Options.m_Renderer == "Direct3D 11")
	{
		DXUTSetCallbackD3D11DeviceAcceptable(RenderEngineApp::IsD3D11DeviceAcceptable);
		DXUTSetCallbackD3D11DeviceCreated(RenderEngineApp::OnD3D11CreateDevice);
		DXUTSetCallbackD3D11SwapChainResized(RenderEngineApp::OnD3D11ResizedSwapChain);
		DXUTSetCallbackD3D11SwapChainReleasing(RenderEngineApp::OnD3D11ReleasingSwapChain);
		DXUTSetCallbackD3D11DeviceDestroyed(RenderEngineApp::OnD3D11DestroyDevice);
		DXUTSetCallbackD3D11FrameRender(RenderEngineApp::OnD3D11FrameRender);
	}
	else
	{
		GCC_ASSERT(0 && "Unknown renderer specified in game options.");
		return false;
	}

	DXUTSetCursorSettings(true, true);

	if (!g_pApp->InitInstance(hInstance, lpCmdLine, hWnd, screenWidth, screenHeight))
	{
		return FALSE;
	}

	return true;
}

FXSTUDIOCORE_API int DestroyInstance()
{
	DXUTShutdown();
	return g_pApp->GetExitCode();
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

FXSTUDIOCORE_API bool IsGameRunning()
{
	FXStudioLogic* game = (FXStudioLogic*)g_pApp->m_pGameLogic;
	return (game) ? game->IsRunning() : false;
}

FXSTUDIOCORE_API void OpenLevel(BSTR lFileName)
{
	std::string levelFile = ws2s(std::wstring(lFileName, SysStringLen(lFileName)));
	FXStudioLogic* pEditorLogic = dynamic_cast<FXStudioLogic*>(g_pApp->GetGameLogic());
	if (pEditorLogic != nullptr)
	{
		std::string assetsDir = "\\Assets\\";
		int projDirLength = pEditorLogic->GetProjectDirectory().length() + assetsDir.length();
		g_pApp->m_Options.m_Level = levelFile.substr(projDirLength, levelFile.length() - projDirLength);
		pEditorLogic->VChangeState(BGS_LoadingGameEnvironment);
	}
}
