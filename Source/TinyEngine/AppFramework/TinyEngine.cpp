#include "BaseGameApp.h"
#include "../Debugging/MiniDump.h"
#include <crtdbg.h>

#ifdef _DEBUG
#pragma comment(lib, "DirectXTKd.lib")
#pragma comment(lib, "Effects11d.lib")
#pragma comment(lib, "tinyxml2d.lib")
#pragma comment(lib, "zlibstatic.lib")
#else
#pragma comment(lib, "DirectXTK.lib")
#pragma comment(lib, "Effects11.lib")
#pragma comment(lib, "tinyxml2.lib")
#pragma comment(lib, "zlibstatic.lib")
#endif

MiniDumper g_MiniDump(false);

INT WINAPI WindowBaseMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	int tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
	tmpDbgFlag |= _CRTDBG_CHECK_ALWAYS_DF;
	_CrtSetDbgFlag(tmpDbgFlag);

	SetCurrentDirectory(Utility::GetExecutableDirectory().c_str());
	Logger::Init("logging.xml");

	SetThreadAffinityMask(GetCurrentThread(), 1);

	if (g_pApp != nullptr)
	{
		g_pApp->GetGameConfig().InitConfig("TinyEngineConfig.xml", lpCmdLine);
		do
		{
			if (g_pApp->InitEnvironment())
			{
				HWND hWnd = g_pApp->SetupWindow(hInstance);
				if (g_pApp->InitRenderer() && g_pApp->InitResource())
				{
					g_pApp->RenderLoop();
				}
			}
		} while (g_pApp->IsRestoring());
	}

	Logger::Destroy();
	return 0;
}