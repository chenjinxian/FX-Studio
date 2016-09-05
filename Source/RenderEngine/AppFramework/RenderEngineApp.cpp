#include "RenderEngineApp.h"
#include "../ResourceCache/ResCache.h"
#include "../Graphics3D/D3DRenderer.h"
#include "BaseGameLogic.h"

#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "winmm.lib")

#ifdef _M_IX86
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
#elif defined(_M_IA64)
#ifdef _DEBUG
#else
#endif
#endif

const UINT RenderEngineApp::SCREEN_WIDTH = 1024;
const UINT RenderEngineApp::SCREEN_HEIGHT = 768;

RenderEngineApp::RenderEngineApp(
	HINSTANCE hInstance, LPWSTR lpCmdLine, HWND hWnd /*= NULL*/, int screenWidth /*= SCREEN_WIDTH*/, int screenHeight /*= SCREEN_HEIGHT*/)
	: m_Options(), m_pRenderer(nullptr),
	m_Instance(hInstance), m_CmdLine(lpCmdLine), m_WindowHandle(hWnd),
	m_ScreenWidth(screenWidth), m_ScreenHeight(screenHeight),
	m_pGame(nullptr), m_pResCache(nullptr)
{
}

RenderEngineApp::~RenderEngineApp()
{
}

int RenderEngineApp::Run()
{
	Initialize();
	DXUTMainLoop();
	DXUTShutdown();

	Logger::Destroy();

	return DXUTGetExitCode();
}

int RenderEngineApp::Exit()
{
	DXUTShutdown();
	Logger::Destroy();
	return DXUTGetExitCode();
}

bool RenderEngineApp::Initialize()
{
	DXUTSetCallbackFrameMove(RenderEngineApp::OnFrameMove, reinterpret_cast<void*>(this));
	DXUTSetCallbackMsgProc(RenderEngineApp::MsgProc, reinterpret_cast<void*>(this));
	DXUTSetCallbackDeviceChanging(RenderEngineApp::ModifyDeviceSettings, reinterpret_cast<void*>(this));
	DXUTSetCallbackDeviceRemoved(RenderEngineApp::OnDeviceRemoved, reinterpret_cast<void*>(this));

	if (m_Options.m_Renderer == "Direct3D 11")
	{
		DXUTSetCallbackD3D11DeviceAcceptable(RenderEngineApp::IsD3D11DeviceAcceptable, reinterpret_cast<void*>(this));
		DXUTSetCallbackD3D11DeviceCreated(RenderEngineApp::OnD3D11CreateDevice, reinterpret_cast<void*>(this));
		DXUTSetCallbackD3D11SwapChainResized(RenderEngineApp::OnD3D11ResizedSwapChain, reinterpret_cast<void*>(this));
		DXUTSetCallbackD3D11SwapChainReleasing(RenderEngineApp::OnD3D11ReleasingSwapChain, reinterpret_cast<void*>(this));
		DXUTSetCallbackD3D11DeviceDestroyed(RenderEngineApp::OnD3D11DestroyDevice, reinterpret_cast<void*>(this));
		DXUTSetCallbackD3D11FrameRender(RenderEngineApp::OnD3D11FrameRender, reinterpret_cast<void*>(this));
	}
	else
	{
		GCC_ASSERT(0 && "Unknown renderer specified in game options.");
		return false;
	}

	DXUTInit(true, true, m_CmdLine, true);
	DXUTSetCursorSettings(true, true);
	return InitializeOptions();
}

bool RenderEngineApp::InitializeOptions()
{
#ifndef _DEBUG
	if (!InitialOptions::IsOnlyInstance(VGetWindowTitle()))
		return false;
#endif

	Logger::Init("logging.xml");
	m_Options.Init("PlayerOptions.xml");

	bool resourceCheck = false;
	while (!resourceCheck)
	{
		const int MEGABYTE = 1024 * 1024;
		const DWORDLONG physicalRAM = 512 * MEGABYTE;
		const DWORDLONG virtualRAM = 1024 * MEGABYTE;
		const DWORDLONG diskSpace = 10 * MEGABYTE;
		if (!InitialOptions::CheckStorage(diskSpace))
			return false;

		const DWORD minCpuSpeed = 1300;			// 1.3Ghz
		DWORD thisCPU = InitialOptions::ReadCPUSpeed();
		if (thisCPU < minCpuSpeed)
		{
			GCC_ERROR("GetCPUSpeed reports CPU is too slow for this game.");
			return false;
		}

		resourceCheck = true;
	}

// 	RegisterEngineEvents();
// 	VRegisterGameEvents();

	IResourceFile *zipFile = GCC_NEW DevelopmentResourceZipFile(L"Assets.zip", DevelopmentResourceZipFile::Editor);

	m_pResCache = GCC_NEW ResCache(50, zipFile);
	if (!m_pResCache->Init())
	{
		GCC_ERROR("Failed to initialize resource cache!  Are your paths set up correctly?");
		return false;
	}

// 	extern shared_ptr<IResourceLoader> CreateWAVResourceLoader();
// 	extern shared_ptr<IResourceLoader> CreateOGGResourceLoader();
// 	extern shared_ptr<IResourceLoader> CreateDDSResourceLoader();
// 	extern shared_ptr<IResourceLoader> CreateJPGResourceLoader();
// 	extern shared_ptr<IResourceLoader> CreateXmlResourceLoader();
// 	extern shared_ptr<IResourceLoader> CreateSdkMeshResourceLoader();
// 	extern shared_ptr<IResourceLoader> CreateScriptResourceLoader();
// 
// 	m_pResCache->RegisterLoader(CreateWAVResourceLoader());
// 	m_pResCache->RegisterLoader(CreateOGGResourceLoader());
// 	m_pResCache->RegisterLoader(CreateDDSResourceLoader());
// 	m_pResCache->RegisterLoader(CreateJPGResourceLoader());
// 	m_pResCache->RegisterLoader(CreateXmlResourceLoader());
// 	m_pResCache->RegisterLoader(CreateSdkMeshResourceLoader());
// 	m_pResCache->RegisterLoader(CreateScriptResourceLoader());

// 	if (!LoadStrings("English"))
// 	{
// 		GCC_ERROR("Failed to load strings");
// 		return false;
// 	}
// 
// 	m_pEventManager = GCC_NEW EventManager("RenderEngineApp Event Mgr", true);
// 	if (!m_pEventManager)
// 	{
// 		GCC_ERROR("Failed to create EventManager.");
// 		return false;
// 	}

	if (m_WindowHandle == NULL)
	{
		DXUTCreateWindow(VGetWindowTitle().c_str(), m_Instance, VGetIcon());
		m_WindowHandle = DXUTGetHWND();
	}
	else
	{
		DXUTSetWindow(m_WindowHandle, m_WindowHandle, m_WindowHandle);
	}

	if (!GetWindowHandle())
	{
		return false;
	}
	SetWindowText(GetWindowHandle(), VGetWindowTitle().c_str());

// 	_tcscpy_s(m_saveGameDirectory, GetSaveGameDirectory(GetHwnd(), VGetGameAppDirectory()));

	DXUTCreateDevice(D3D_FEATURE_LEVEL_10_0, true, m_ScreenWidth, m_ScreenHeight);

	if (m_Options.m_Renderer == "Direct3D 11")
	{
		m_pRenderer = shared_ptr<IRenderer>(GCC_NEW D3DRenderer11());
	}
// 	m_pRenderer->VSetBackgroundColor(Color(Colors::);
	m_pRenderer->VOnRestore();

	m_pGame = VCreateGameAndView();
	if (nullptr == m_pGame)
		return false;

	m_pResCache->Preload(m_pResCache, "*.ogg", nullptr);
	m_pResCache->Preload(m_pResCache, "*.dds", nullptr);
	m_pResCache->Preload(m_pResCache, "*.jpg", nullptr);

	InitialOptions::CheckForJoystick(GetWindowHandle());

	return true;
}

LRESULT CALLBACK RenderEngineApp::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void *pUserContext)
{
	RenderEngineApp* pApp = reinterpret_cast<RenderEngineApp*>(pUserContext);

// 	*pbNoFurtherProcessing = D3DRenderer::g_DialogResourceManager.MsgProc(hWnd, uMsg, wParam, lParam);
// 	if (*pbNoFurtherProcessing)
// 		return 0;

	LRESULT result = 0;

	switch (uMsg)
	{
// 	case WM_POWERBROADCAST:
// 	{
// 		int event = (int)wParam;
// 		result = pApp->OnPowerBroadcast(event);
// 		break;
// 	}
// 
// 	case WM_DISPLAYCHANGE:
// 	{
// 		int colorDepth = (int)wParam;
// 		int width = (int)(short)LOWORD(lParam);
// 		int height = (int)(short)HIWORD(lParam);
// 
// 		result = g_pApp->OnDisplayChange(colorDepth, width, height);
// 		break;
// 	}

	case WM_CLOSE:
	{
// 		if (pApp->m_bQuitting)
// 		{
// 			result = pApp->OnClose();
// 		}
// 		else
// 		{
// 			*pbNoFurtherProcessing = true;
// 		}
		break;
	}


	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_CHAR:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case MM_JOY1BUTTONDOWN:
	case MM_JOY1BUTTONUP:
	case MM_JOY1MOVE:
	case MM_JOY1ZMOVE:
	case MM_JOY2BUTTONDOWN:
	case MM_JOY2BUTTONUP:
	case MM_JOY2MOVE:
	case MM_JOY2ZMOVE:
	{
// 		if (pApp->m_pGame)
// 		{
// 			BaseGameLogic *pGame = g_pApp->m_pGame;
// 			AppMsg msg;
// 			msg.m_hWnd = hWnd;
// 			msg.m_uMsg = uMsg;
// 			msg.m_wParam = wParam;
// 			msg.m_lParam = lParam;
// 			for (GameViewList::reverse_iterator i = pGame->m_gameViews.rbegin(); i != pGame->m_gameViews.rend(); ++i)
// 			{
// 				if ((*i)->VOnMsgProc(msg))
// 				{
// 					result = true;
// 					break;
// 				}
// 			}
// 		}
		break;
	}

	/**********************
	WARNING!!!!! You MIGHT think you need this, but if you use the DirectX
	Framework the DefWindowProc is called for you....

	default:
	return DefWindowProc(hWnd, message, wParam, lParam);

	***********************/
	}

	return result;
}

bool CALLBACK RenderEngineApp::ModifyDeviceSettings(DXUTDeviceSettings* pDeviceSettings, void* pUserContext)
{
	static bool s_bFirstTime = true;
	if (s_bFirstTime)
	{
		s_bFirstTime = false;
		if (pDeviceSettings->d3d11.DriverType == D3D_DRIVER_TYPE_REFERENCE)
		{
			DXUTDisplaySwitchingToREFWarning();
		}

	}

	return true;
}

bool CALLBACK RenderEngineApp::OnDeviceRemoved(void* pUserContext)
{
	return true;
}

bool CALLBACK RenderEngineApp::IsD3D11DeviceAcceptable(
	const CD3D11EnumAdapterInfo *AdapterInfo, UINT Output, const CD3D11EnumDeviceInfo *DeviceInfo,
	DXGI_FORMAT BackBufferFormat, bool bWindowed, void* pUserContext)
{
	return true;
}

HRESULT CALLBACK RenderEngineApp::OnD3D11CreateDevice(
	ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext)
{
	RenderEngineApp* pApp = reinterpret_cast<RenderEngineApp*>(pUserContext);
// 	HRESULT hr;

	ID3D11DeviceContext* pd3dImmediateContext = DXUTGetD3D11DeviceContext();
// 	V_RETURN(D3DRenderer::g_DialogResourceManager.OnD3D11CreateDevice(pd3dDevice, pd3dImmediateContext));

	return S_OK;
}

HRESULT CALLBACK RenderEngineApp::OnD3D11ResizedSwapChain(
	ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext)
{
	RenderEngineApp* pApp = reinterpret_cast<RenderEngineApp*>(pUserContext);
// 	HRESULT hr;

// 	V_RETURN(D3DRenderer::g_DialogResourceManager.OnD3D11ResizedSwapChain(pd3dDevice, pBackBufferSurfaceDesc));
// 
// 	if (pApp->m_pGame)
// 	{
// 		BaseGameLogic *pGame = pApp->m_pGame;
// 		for (GameViewList::iterator i = pGame->m_gameViews.begin(); i != pGame->m_gameViews.end(); ++i)
// 		{
// 			(*i)->VOnRestore();
// 		}
// 	}

	return S_OK;
}

void CALLBACK RenderEngineApp::OnD3D11ReleasingSwapChain(void* pUserContext)
{
// 	D3DRenderer::g_DialogResourceManager.OnD3D11ReleasingSwapChain();
}

void CALLBACK RenderEngineApp::OnD3D11DestroyDevice(void* pUserContext)
{
	RenderEngineApp* pApp = reinterpret_cast<RenderEngineApp*>(pUserContext);
// 	if (pApp->m_Renderer)
// 		pApp->m_Renderer->VShutdown();
// 	D3DRenderer::g_DialogResourceManager.OnD3D11DestroyDevice();
// 	pApp->m_Renderer = shared_ptr<IRenderer>(NULL);
}

void CALLBACK RenderEngineApp::OnFrameMove(double fTime, float fElapsedTime, void *pUserContext)
{
	RenderEngineApp* pApp = reinterpret_cast<RenderEngineApp*>(pUserContext);
	if (pApp->m_pGame)
	{
// 		IEventManager::Get()->VUpdate(20);
		pApp->m_pGame->VOnUpdate(float(fTime), fElapsedTime);
	}
}

void CALLBACK RenderEngineApp::OnD3D11FrameRender(
	ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime, float fElapsedTime, void* pUserContext)
{
	RenderEngineApp* pApp = reinterpret_cast<RenderEngineApp*>(pUserContext);
	BaseGameLogic *pGame = pApp->m_pGame;

	for (auto it : pGame->m_GameViews)
	{
		it->VOnRender(fTime, fElapsedTime);
	}

// 	for (GameViewList::iterator i = pGame->m_gameViews.begin(),
// 		end = pGame->m_gameViews.end(); i != end; ++i)
// 	{
// 		(*i)->VOnRender(fTime, fElapsedTime);
// 	}

// 	pApp->m_pGame->VRenderDiagnostics();
}

bool RenderEngineApp::VLoadGame()
{
	return m_pGame->VLoadGame(m_Options.m_Level.c_str());
}

LRESULT RenderEngineApp::OnClose()
{
	SAFE_DELETE(m_pGame);
	DestroyWindow(GetWindowHandle());
	SAFE_DELETE(m_pResCache);
	return 0;
}
