#include "RenderEngineApp.h"
#include "BaseGameLogic.h"
#include "../Graphics3D/D3DRenderer.h"
#include "../ResourceCache/ResCache.h"
#include "../ResourceCache/XmlResource.h"

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

const UINT RenderEngineApp::SCREEN_WIDTH = 1024;
const UINT RenderEngineApp::SCREEN_HEIGHT = 768;

RenderEngineApp::RenderEngineApp(
	HINSTANCE hInstance, LPWSTR lpCmdLine, HWND hWnd /*= NULL*/, int screenWidth /*= SCREEN_WIDTH*/, int screenHeight /*= SCREEN_HEIGHT*/)
	: m_Instance(hInstance), m_CmdLine(lpCmdLine), m_WindowHandle(hWnd),
	m_ScreenWidth(screenWidth), m_ScreenHeight(screenHeight),
	m_pGameLogic(nullptr), m_pRenderer(nullptr), m_pResCache(nullptr)
{
}

RenderEngineApp::~RenderEngineApp()
{
}

int RenderEngineApp::Exit()
{
	return DXUTGetExitCode();
}

bool RenderEngineApp::Initialize()
{
	DXUTSetCallbackFrameMove(RenderEngineApp::OnFrameMove, reinterpret_cast<void*>(this));
	DXUTSetCallbackMsgProc(RenderEngineApp::MsgProc, reinterpret_cast<void*>(this));
	DXUTSetCallbackDeviceChanging(RenderEngineApp::ModifyDeviceSettings, reinterpret_cast<void*>(this));
	DXUTSetCallbackDeviceRemoved(RenderEngineApp::OnDeviceRemoved, reinterpret_cast<void*>(this));

	DXUTSetCallbackD3D11DeviceAcceptable(RenderEngineApp::IsD3D11DeviceAcceptable, reinterpret_cast<void*>(this));
	DXUTSetCallbackD3D11DeviceCreated(RenderEngineApp::OnD3D11CreateDevice, reinterpret_cast<void*>(this));
	DXUTSetCallbackD3D11SwapChainResized(RenderEngineApp::OnD3D11ResizedSwapChain, reinterpret_cast<void*>(this));
	DXUTSetCallbackD3D11SwapChainReleasing(RenderEngineApp::OnD3D11ReleasingSwapChain, reinterpret_cast<void*>(this));
	DXUTSetCallbackD3D11DeviceDestroyed(RenderEngineApp::OnD3D11DestroyDevice, reinterpret_cast<void*>(this));
	DXUTSetCallbackD3D11FrameRender(RenderEngineApp::OnD3D11FrameRender, reinterpret_cast<void*>(this));

	IResourceFile *zipFile = GCC_NEW DevelopmentResourceZipFile(L"Assets.zip", DevelopmentResourceZipFile::Editor);
	m_pResCache = GCC_NEW ResCache(50, zipFile);
	if (!m_pResCache->Init())
	{
		GCC_ERROR("Failed to initialize resource cache!  Are your paths set up correctly?");
		return false;
	}

	m_pResCache->RegisterLoader(CreateXmlResourceLoader());

	DXUTInit(true, true, m_CmdLine, true);
	DXUTSetCursorSettings(true, true);

	DXUTSetWindow(m_WindowHandle, m_WindowHandle, m_WindowHandle);
	DXUTCreateDevice(D3D_FEATURE_LEVEL_10_0, true, m_ScreenWidth, m_ScreenHeight);

	m_pRenderer = shared_ptr<IRenderer>(GCC_NEW D3DRenderer11());
	m_pRenderer->VSetBackgroundColor(Color(Colors::CornflowerBlue.f));

	m_pGameLogic = VCreateGameAndView();
	if (nullptr == m_pGameLogic)
	{
		return false;
	}

	m_pResCache->Preload("*.ogg", NULL);
	m_pResCache->Preload("*.dds", NULL);
	m_pResCache->Preload("*.jpg", NULL);

	return true;
}

bool RenderEngineApp::VLoadGame(const std::string& projectPath)
{
	return m_pGameLogic->VLoadGame(projectPath);
}

LRESULT CALLBACK RenderEngineApp::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void *pUserContext)
{
	RenderEngineApp* pApp = reinterpret_cast<RenderEngineApp*>(pUserContext);

	return 0;
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

	return S_OK;
}

HRESULT CALLBACK RenderEngineApp::OnD3D11ResizedSwapChain(
	ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext)
{
	RenderEngineApp* pApp = reinterpret_cast<RenderEngineApp*>(pUserContext);

	return S_OK;
}

void CALLBACK RenderEngineApp::OnD3D11ReleasingSwapChain(void* pUserContext)
{
	// 	D3DRenderer::g_DialogResourceManager.OnD3D11ReleasingSwapChain();
}

void CALLBACK RenderEngineApp::OnD3D11DestroyDevice(void* pUserContext)
{
	RenderEngineApp* pApp = reinterpret_cast<RenderEngineApp*>(pUserContext);
}

void CALLBACK RenderEngineApp::OnFrameMove(double fTime, float fElapsedTime, void *pUserContext)
{
	RenderEngineApp* pApp = reinterpret_cast<RenderEngineApp*>(pUserContext);
	if (pApp->m_pGameLogic != nullptr)
	{
		pApp->m_pGameLogic->VOnUpdate(static_cast<float>(fTime), fElapsedTime);
	}
}

void CALLBACK RenderEngineApp::OnD3D11FrameRender(
	ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime, float fElapsedTime, void* pUserContext)
{
	RenderEngineApp* pApp = reinterpret_cast<RenderEngineApp*>(pUserContext);
	for (auto view : pApp->m_pGameLogic->m_GameViews)
	{
		view->VOnRender(static_cast<float>(fTime), fElapsedTime);
	}
}
