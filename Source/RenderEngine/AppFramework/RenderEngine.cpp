#include "RenderEngineApp.h"
#include "BaseGameLogic.h"
#include "../Debugging/MiniDump.h"

#ifdef _DEBUG
#pragma comment(lib, "DirectXTKd.lib")
#pragma comment(lib, "DXUTd.lib")
#pragma comment(lib, "DXUTOptd.lib")
#pragma comment(lib, "Effects11d.lib")
#pragma comment(lib, "tinyxmld.lib")
#pragma comment(lib, "zlibstaticd.lib")
#else
#pragma comment(lib, "DirectXTK.lib")
#pragma comment(lib, "DXUT.lib")
#pragma comment(lib, "DXUTOpt.lib")
#pragma comment(lib, "Effects11.lib")
#pragma comment(lib, "tinyxml.lib")
#pragma comment(lib, "zlibstatic.lib")
#endif

MiniDumper g_MiniDump(false);

INT WINAPI GameMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	int tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(tmpDbgFlag);

	Logger::Init("logging.xml");

	g_pApp->m_Options.Init("PlayerOptions.xml", lpCmdLine);

	DXUTSetCallbackMsgProc(RenderEngineApp::MsgProc);
	DXUTSetCallbackFrameMove(RenderEngineApp::OnUpdateGame);
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

	if (!g_pApp->InitInstance(hInstance, lpCmdLine, 0, g_pApp->m_Options.m_ScreenWidth, g_pApp->m_Options.m_ScreenHeight))
	{
		return FALSE;
	}

	DXUTMainLoop();
	DXUTShutdown();

	Logger::Destroy();

	return g_pApp->GetExitCode();
}