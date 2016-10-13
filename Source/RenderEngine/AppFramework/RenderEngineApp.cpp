#include "RenderEngineApp.h"
#include "BaseGameLogic.h"
#include "../Mainloop/Initialization.h"
#include "../EventManager/EventManagerImpl.h"
#include "../EventManager/Events.h"
#include "../Graphics3D/D3DRenderer.h"
#include "../ResourceCache/ResCache.h"
#include "../ResourceCache/XmlResource.h"
#include "../UserInterface/HumanView.h"
#include "../UserInterface/MessageBox.h"

CDXUTDialog                         g_HUD;                  // manages the 3D UI
CDXUTDialog                         g_SampleUI;             // dialog for sample specific controls

float                               g_fModelWaviness = 0.0f;
bool                                g_bSpinning = true;

#define IDC_TOGGLEFULLSCREEN    1
#define IDC_TOGGLEREF           2
#define IDC_CHANGEDEVICE        3
#define IDC_TOGGLEWARP          4
#define IDC_TOGGLESPIN          5
#define IDC_PUFF_SCALE          6
#define IDC_PUFF_STATIC         7

void CALLBACK OnGUIEvent(UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext)
{
	switch (nControlID)
	{
	case IDC_TOGGLEFULLSCREEN:
		DXUTToggleFullScreen();
		break;
	case IDC_TOGGLEREF:
		DXUTToggleREF();
		break;
	case IDC_CHANGEDEVICE:
		break;
	case IDC_TOGGLEWARP:
		DXUTToggleWARP();
		break;

	case IDC_TOGGLESPIN:
	{
		g_bSpinning = g_SampleUI.GetCheckBox(IDC_TOGGLESPIN)->GetChecked();
		break;
	}

	case IDC_PUFF_SCALE:
	{
		g_fModelWaviness = (float)(g_SampleUI.GetSlider(IDC_PUFF_SCALE)->GetValue() * 0.01f);

		WCHAR sz[100];
		swprintf_s(sz, 100, L"Waviness: %0.2f", g_fModelWaviness);
		g_SampleUI.GetStatic(IDC_PUFF_STATIC)->SetText(sz);

		break;
	}
	}
}

#define MAX_LOADSTRING 100

RenderEngineApp* g_pApp = nullptr;

const int RenderEngineApp::MEGABYTE = 1024 * 1024;
const float RenderEngineApp::SIXTY_HERTZ = 16.66f;
const uint32_t RenderEngineApp::SCREEN_WIDTH = 1024;
const uint32_t RenderEngineApp::SCREEN_HEIGHT = 768;

RenderEngineApp::RenderEngineApp()
	: m_pGameLogic(nullptr), m_ScreenWidth(SCREEN_WIDTH), m_ScreenHeight(SCREEN_HEIGHT),
	m_IsRunning(false), m_IsEditorRunning(false),
	m_pEventManager(nullptr), m_pResCache(nullptr),
	m_IsQuitRequested(false), m_IsQuitting(false), m_HasModalDialog(0)
{
	g_pApp = this;
}

RenderEngineApp::~RenderEngineApp()
{
}

HWND RenderEngineApp::GetHwnd()
{
	return DXUTGetHWND();
}

bool RenderEngineApp::InitInstance(HINSTANCE hInstance, LPWSTR lpCmdLine, HWND hWnd, int screenWidth, int screenHeight)
{
#ifndef _DEBUG
	if (!IsOnlyInstance(VGetGameTitle()))
		return false;
#endif

	SetCursor(NULL);

	bool resourceCheck = false;
	while (!resourceCheck)
	{
		const DWORDLONG physicalRAM = 512 * MEGABYTE;
		const DWORDLONG virtualRAM = 1024 * MEGABYTE;
		const DWORDLONG diskSpace = 10 * MEGABYTE;
		if (!CheckStorage(diskSpace))
			return false;

		const DWORD minCpuSpeed = 1300;
		DWORD thisCPU = ReadCPUSpeed();
		if (thisCPU < minCpuSpeed)
		{
			GCC_ERROR("GetCPUSpeed reports CPU is too slow for this game.");
			return false;
		}

		resourceCheck = true;
	}

	m_hInstance = hInstance;

	RegisterEngineEvents();
	VRegisterGameEvents();

	IResourceFile *zipFile = (m_IsEditorRunning || m_Options.m_IsDevelopmentDirectories) ?
		GCC_NEW DevelopmentResourceZipFile(L"Assets.zip", DevelopmentResourceZipFile::Editor) :
		GCC_NEW ResourceZipFile(L"Assets.zip");

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

// 	m_pResCache->RegisterLoader(CreateWAVResourceLoader());
// 	m_pResCache->RegisterLoader(CreateOGGResourceLoader());
// 	m_pResCache->RegisterLoader(CreateDDSResourceLoader());
// 	m_pResCache->RegisterLoader(CreateJPGResourceLoader());
	m_pResCache->RegisterLoader(CreateXmlResourceLoader());
// 	m_pResCache->RegisterLoader(CreateSdkMeshResourceLoader());
// 	m_pResCache->RegisterLoader(CreateScriptResourceLoader());

	if (!LoadStrings("English"))
	{
		GCC_ERROR("Failed to load strings");
		return false;
	}

	m_pEventManager = GCC_NEW EventManager("RenderEngineApp Event Mgr", true);
	if (!m_pEventManager)
	{
		GCC_ERROR("Failed to create EventManager.");
		return false;
	}

	DXUTInit(true, true, lpCmdLine, true);

	g_fModelWaviness = 0.0f;
	g_bSpinning = true;

	g_HUD.Init(&D3DRenderer::m_DialogResourceManager);
	g_SampleUI.Init(&D3DRenderer::m_DialogResourceManager);

	g_HUD.SetCallback(OnGUIEvent); int iY = 10;
	g_HUD.AddButton(IDC_TOGGLEFULLSCREEN, L"Toggle full screen", 0, iY, 170, 22);
	g_HUD.AddButton(IDC_CHANGEDEVICE, L"Change device (F2)", 0, iY += 26, 170, 22, VK_F2);
	g_HUD.AddButton(IDC_TOGGLEREF, L"Toggle REF (F3)", 0, iY += 26, 170, 22, VK_F3);
	g_HUD.AddButton(IDC_TOGGLEWARP, L"Toggle WARP (F4)", 0, iY += 26, 170, 22, VK_F4);

	g_SampleUI.SetCallback(OnGUIEvent); iY = 10;

	WCHAR sz[100];
	iY += 24;
	swprintf_s(sz, 100, L"Waviness: %0.2f", g_fModelWaviness);
	g_SampleUI.AddStatic(IDC_PUFF_STATIC, sz, 0, iY += 26, 170, 22);
	g_SampleUI.AddSlider(IDC_PUFF_SCALE, 50, iY += 26, 100, 22, 0, 2000, (int)(g_fModelWaviness * 100.0f));

	iY += 24;
	g_SampleUI.AddCheckBox(IDC_TOGGLESPIN, L"Toggle Spinning", 0, iY += 26, 170, 22, g_bSpinning);

	if (hWnd == NULL)
	{
		DXUTCreateWindow(VGetGameTitle(), hInstance, VGetIcon());
	}
	else
	{
		DXUTSetWindow(hWnd, hWnd, hWnd);
	}

	if (!GetHwnd())
	{
		return false;
	}
	SetWindowText(GetHwnd(), VGetGameTitle());

	_tcscpy_s(m_saveGameDirectory, GetSaveGameDirectory(GetHwnd(), VGetGameAppDirectory()));
	m_ScreenWidth = screenWidth;
	m_ScreenHeight = screenHeight;

	DXUTCreateDevice(D3D_FEATURE_LEVEL_10_1, true, screenWidth, screenHeight);

	if (GetRendererImpl() == Renderer_D3D11)
	{
		m_pRenderer = shared_ptr<IRenderer>(GCC_NEW D3DRenderer11());
	}
	m_pRenderer->VSetBackgroundColor(Color(Colors::CornflowerBlue.f));
	m_pRenderer->VOnRestore();


	m_pGameLogic = VCreateGameAndView();
	if (m_pGameLogic == nullptr)
		return false;

	m_pResCache->Preload("*.ogg", NULL);
	m_pResCache->Preload("*.dds", NULL);
	m_pResCache->Preload("*.jpg", NULL);

	if (GetRendererImpl() == Renderer_D3D11)
		m_pResCache->Preload("*.sdkmesh", NULL);

	m_IsRunning = true;

	return true;
}

bool RenderEngineApp::VLoadGame()
{
	return m_pGameLogic->VLoadGame(m_Options.m_Level);
}

void RenderEngineApp::RegisterEngineEvents(void)
{
	REGISTER_EVENT(EvtData_Environment_Loaded);
	REGISTER_EVENT(EvtData_New_Actor);
	REGISTER_EVENT(EvtData_Move_Actor);
	REGISTER_EVENT(EvtData_Destroy_Actor);
// 	REGISTER_EVENT(EvtData_Request_New_Actor);
// 	REGISTER_EVENT(EvtData_Network_Player_Actor_Assignment);
}

bool RenderEngineApp::LoadStrings(std::string language)
{
	std::string languageFile = "Strings\\";
	languageFile += language;
	languageFile += ".xml";

	TiXmlElement* pRoot = XmlResourceLoader::LoadAndReturnRootXmlElement(languageFile.c_str());
	if (!pRoot)
	{
		GCC_ERROR("Strings are missing.");
		return false;
	}

	for (TiXmlElement* pElem = pRoot->FirstChildElement(); pElem; pElem = pElem->NextSiblingElement())
	{
		const char *pKey = pElem->Attribute("id");
		const char *pText = pElem->Attribute("value");
		const char *pHotkey = pElem->Attribute("hotkey");
		if (pKey && pText)
		{
			wchar_t wideKey[64];
			wchar_t wideText[1024];
			AnsiToWideCch(wideKey, pKey, 64);
			AnsiToWideCch(wideText, pText, 1024);
			m_TextResource[std::wstring(wideKey)] = std::wstring(wideText);

			if (pHotkey)
			{
				m_Hotkeys[std::wstring(wideKey)] = MapCharToKeycode(*pHotkey);
			}
		}
	}
	return true;
}

UINT RenderEngineApp::MapCharToKeycode(const char pHotKey)
{
	if (pHotKey >= '0' && pHotKey <= '9')
		return 0x30 + pHotKey - '0';

	if (pHotKey >= 'A' && pHotKey <= 'Z')
		return 0x41 + pHotKey - 'A';

	GCC_ASSERT(0 && "Platform specific hotkey is not defined");
	return 0;
}

std::wstring RenderEngineApp::GetString(std::wstring sID)
{
	auto localizedString = m_TextResource.find(sID);
	if (localizedString == m_TextResource.end())
	{
		GCC_ASSERT(0 && "String not found!");
		return L"";
	}
	return localizedString->second;
}

LRESULT CALLBACK RenderEngineApp::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext)
{
// 	*pbNoFurtherProcessing = D3DRenderer::m_DialogResourceManager.MsgProc(hWnd, uMsg, wParam, lParam);
// 	if (*pbNoFurtherProcessing)
// 		return 0;
// 
// 	LRESULT result = 0;
// 
// 	switch (uMsg)
// 	{
// 	case WM_POWERBROADCAST:
// 	{
// 		int event = (int)wParam;
// 		result = g_pApp->OnPowerBroadcast(event);
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
// 
// 	case WM_SYSCOMMAND:
// 	{
// 		result = g_pApp->OnSysCommand(wParam, lParam);
// 		if (result)
// 		{
// 			*pbNoFurtherProcessing = true;
// 		}
// 		break;
// 	}
// 
// 	case WM_SYSKEYDOWN:
// 	{
// 		if (wParam == VK_RETURN)
// 		{
// 			*pbNoFurtherProcessing = true;
// 			return g_pApp->OnAltEnter();
// 		}
// 		return DefWindowProc(hWnd, uMsg, wParam, lParam);
// 	}
// 
// 
// 	case WM_CLOSE:
// 	{
// 		if (g_pApp->m_IsQuitting)
// 		{
// 			result = g_pApp->OnClose();
// 		}
// 		else
// 		{
// 			*pbNoFurtherProcessing = true;
// 		}
// 		break;
// 	}
// 
// 
// 	case WM_KEYDOWN:
// 	case WM_KEYUP:
// 	case WM_CHAR:
// 	case WM_MOUSEMOVE:
// 	case WM_LBUTTONDOWN:
// 	case WM_LBUTTONUP:
// 	case WM_RBUTTONDOWN:
// 	case WM_RBUTTONUP:
// 	{
// 		if (g_pApp->m_pGameLogic)
// 		{
// 			BaseGameLogic *pGame = g_pApp->m_pGameLogic;
// 			AppMsg msg;
// 			msg.m_hWnd = hWnd;
// 			msg.m_uMsg = uMsg;
// 			msg.m_wParam = wParam;
// 			msg.m_lParam = lParam;
// 			for (GameViewList::reverse_iterator i = pGame->m_GameViews.rbegin(); i != pGame->m_GameViews.rend(); ++i)
// 			{
// 				if ((*i)->VOnMsgProc(msg))
// 				{
// 					result = true;
// 					break;
// 				}
// 			}
// 		}
// 		break;
// 	}
// 	}
// 
// 	return result;

	*pbNoFurtherProcessing = D3DRenderer::m_DialogResourceManager.MsgProc(hWnd, uMsg, wParam, lParam);
	if (*pbNoFurtherProcessing)
		return 0;

	// Give the dialogs a chance to handle the message first
	*pbNoFurtherProcessing = g_HUD.MsgProc(hWnd, uMsg, wParam, lParam);
	if (*pbNoFurtherProcessing)
		return 0;
	*pbNoFurtherProcessing = g_SampleUI.MsgProc(hWnd, uMsg, wParam, lParam);
	if (*pbNoFurtherProcessing)
		return 0;

	// Pass all remaining windows messages to camera so it can respond to user input
// 	g_Camera.HandleMessages(hWnd, uMsg, wParam, lParam);

	return 0;
}

LRESULT RenderEngineApp::OnDisplayChange(int colorDepth, int width, int height)
{
	return 0;
}

LRESULT RenderEngineApp::OnPowerBroadcast(int event)
{
	if (event == PBT_APMQUERYSUSPEND)
		return BROADCAST_QUERY_DENY;
	else if (event == PBT_APMBATTERYLOW)
	{
		AbortGame();
	}

	return true;
}

LRESULT RenderEngineApp::OnSysCommand(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case SC_MAXIMIZE:
	{
		if (!m_IsFullScreen && IsRunning())
		{
			OnAltEnter();
		}
	}
	return 0;

	case SC_CLOSE:
	{
		if (lParam != g_QuitNoPrompt)
		{
			if (m_IsQuitRequested)
				return true;

			m_IsQuitRequested = true;
			if (MessageBox::Ask(QUESTION_QUIT_GAME) == IDNO)
			{
				m_IsQuitRequested = false;

				return true;
			}
		}

		m_IsQuitting = true;

		if (HasModalDialog())
		{
			ForceModalExit();

			PostMessage(GetHwnd(), WM_SYSCOMMAND, SC_CLOSE, g_QuitNoPrompt);

			m_IsQuitRequested = false;

			return true;
		}

		m_IsQuitRequested = false;
	}
	return 0;

	default:
		return DefWindowProc(GetHwnd(), WM_SYSCOMMAND, wParam, lParam);
	}

	return 0;
}

LRESULT RenderEngineApp::OnClose()
{
	SAFE_DELETE(m_pGameLogic);

	DestroyWindow(GetHwnd());

	VDestroyNetworkEventForwarder();

	SAFE_DELETE(m_pEventManager);

	SAFE_DELETE(m_pResCache);

	return 0;
}

void RenderEngineApp::FlashWhileMinimized()
{
	if (!GetHwnd())
		return;

	if (IsIconic(GetHwnd()))
	{
		DWORD now = timeGetTime();
		DWORD then = now;
		MSG msg;

		FlashWindow(GetHwnd(), true);

		while (true)
		{
			if (PeekMessage(&msg, NULL, 0, 0, 0))
			{
				if (msg.message != WM_SYSCOMMAND || msg.wParam != SC_CLOSE)
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

				if (!IsIconic(GetHwnd()))
				{
					FlashWindow(GetHwnd(), false);
					break;
				}
			}
			else
			{
				now = timeGetTime();
				DWORD timeSpan = now > then ? (now - then) : (then - now);
				if (timeSpan > 1000)
				{
					then = now;
					FlashWindow(GetHwnd(), true);
				}
			}
		}
	}
}

LRESULT RenderEngineApp::OnAltEnter()
{
	DXUTToggleFullScreen();
	return 0;
}

HumanView* RenderEngineApp::GetHumanView()
{
	HumanView *pView = NULL;
	for (GameViewList::iterator i = m_pGameLogic->m_GameViews.begin(); i != m_pGameLogic->m_GameViews.end(); ++i)
	{
		if ((*i)->VGetType() == GameView_Human)
		{
			shared_ptr<IGameView> pIGameView(*i);
			pView = static_cast<HumanView *>(&*pIGameView);
			break;
		}
	}
	return pView;
}

int RenderEngineApp::Modal(shared_ptr<IScreenElement> pModalScreen, int defaultAnswer)
{
	HumanView* pView = GetHumanView();

	if (!pView)
	{
		return defaultAnswer;
	}

	if (m_HasModalDialog & 0x10000000)
	{
		GCC_ASSERT(0 && "Too Many nested dialogs!");
		return defaultAnswer;
	}

	GCC_ASSERT(GetHwnd() != NULL && _T("Main Window is NULL!"));
	if ((GetHwnd() != NULL) && IsIconic(GetHwnd()))
	{
		FlashWhileMinimized();
	}

	m_HasModalDialog <<= 1;
	m_HasModalDialog |= 1;

	pView->VPushElement(pModalScreen);

	LPARAM lParam = 0;
	int result = PumpUntilMessage(g_MsgEndModal, NULL, &lParam);

	if (lParam != 0)
	{
		if (lParam == g_QuitNoPrompt)
			result = defaultAnswer;
		else
			result = (int)lParam;
	}

	pView->VRemoveElement(pModalScreen);
	m_HasModalDialog >>= 1;

	return result;
}

int RenderEngineApp::PumpUntilMessage(UINT msgEnd, WPARAM* pWParam, LPARAM* pLParam)
{
	int currentTime = timeGetTime();
	MSG msg;
	for (;; )
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (msg.message == WM_CLOSE)
			{
				m_IsQuitting = true;
				GetMessage(&msg, NULL, 0, 0);
				break;
			}
			else
			{
				if (GetMessage(&msg, NULL, NULL, NULL))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

				if (msg.message == msgEnd)
					break;
			}
		}
		else
		{
			if (m_pGameLogic)
			{
				int timeNow = timeGetTime();
				int deltaMilliseconds = timeNow - currentTime;
				for (GameViewList::iterator i = m_pGameLogic->m_GameViews.begin(); i != m_pGameLogic->m_GameViews.end(); ++i)
				{
					(*i)->VOnUpdate(deltaMilliseconds);
				}
				currentTime = timeNow;
				DXUTRender3DEnvironment();
			}
		}
	}
	if (pLParam)
		*pLParam = msg.lParam;
	if (pWParam)
		*pWParam = msg.wParam;

	return 0;
}

int	RenderEngineApp::EatSpecificMessages(UINT msgType, boost::optional<LPARAM> lParam, boost::optional<WPARAM> wParam)
{
	bool done = false;

	while (!done)
	{
		MSG msg;

		if (PeekMessage(&msg, NULL, msgType, msgType, PM_NOREMOVE))
		{
			bool valid = true;

			if (lParam.is_initialized())
			{
				valid &= (*lParam == msg.lParam);
			}

			if (wParam.is_initialized())
			{
				valid &= (*wParam == msg.wParam);
			}

			if (valid)
			{
				GetMessage(&msg, NULL, msgType, msgType);
			}
			else
			{
				done = true;
			}
		}
		else
		{
			done = true;
		}
	}

	return 0;
}

RenderEngineApp::Renderer RenderEngineApp::GetRendererImpl()
{
	return Renderer_D3D11;
};

bool CALLBACK RenderEngineApp::IsD3D11DeviceAcceptable(
	const CD3D11EnumAdapterInfo *AdapterInfo, UINT Output, const CD3D11EnumDeviceInfo *DeviceInfo,
	DXGI_FORMAT BackBufferFormat, bool bWindowed, void* pUserContext)
{
	return true;
}

HRESULT CALLBACK RenderEngineApp::OnD3D11CreateDevice(
	ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext)
{
// 	HRESULT hr;
// 
// 	ID3D11DeviceContext* pd3dImmediateContext = DXUTGetD3D11DeviceContext();
// 	V_RETURN(D3DRenderer::m_DialogResourceManager.OnD3D11CreateDevice(pd3dDevice, pd3dImmediateContext));
// 
// 	return S_OK;

	HRESULT hr = S_OK;

	auto pd3dImmediateContext = DXUTGetD3D11DeviceContext();
	V_RETURN(D3DRenderer::m_DialogResourceManager.OnD3D11CreateDevice(pd3dDevice, pd3dImmediateContext));
	D3DRenderer::m_pTextHelper = new CDXUTTextHelper(pd3dDevice, pd3dImmediateContext, &D3DRenderer::m_DialogResourceManager, 15);

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;

	// Disable optimizations to further improve shader debugging
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	return S_OK;
}

HRESULT CALLBACK RenderEngineApp::OnD3D11ResizedSwapChain(
	ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain,
	const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext)
{
// 	HRESULT hr;
// 
// 	V_RETURN(D3DRenderer::m_DialogResourceManager.OnD3D11ResizedSwapChain(pd3dDevice, pBackBufferSurfaceDesc));
// 
// 	if (g_pApp->m_pGameLogic)
// 	{
// 		BaseGameLogic *pGame = g_pApp->m_pGameLogic;
// 		for (GameViewList::iterator i = pGame->m_GameViews.begin(); i != pGame->m_GameViews.end(); ++i)
// 		{
// 			(*i)->VOnRestore();
// 		}
// 	}
// 
// 	return S_OK;

	HRESULT hr;

	V_RETURN(D3DRenderer::m_DialogResourceManager.OnD3D11ResizedSwapChain(pd3dDevice, pBackBufferSurfaceDesc));

	// Setup the camera's projection parameters
	float fAspectRatio = pBackBufferSurfaceDesc->Width / (FLOAT)pBackBufferSurfaceDesc->Height;
// 	g_Camera.SetProjParams(XM_PI / 4, fAspectRatio, 0.1f, 5000.0f);
// 	g_Camera.SetWindow(pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height);
// 	g_Camera.SetButtonMasks(MOUSE_LEFT_BUTTON, MOUSE_WHEEL, MOUSE_MIDDLE_BUTTON);

	g_HUD.SetLocation(pBackBufferSurfaceDesc->Width - 170, 0);
	g_HUD.SetSize(170, 170);
	g_SampleUI.SetLocation(pBackBufferSurfaceDesc->Width - 170, pBackBufferSurfaceDesc->Height - 300);
	g_SampleUI.SetSize(170, 300);

	return S_OK;
}

void CALLBACK RenderEngineApp::OnD3D11FrameRender(
	ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext,
	double fTime, float fElapsedTime, void* pUserContext)
{
// 	BaseGameLogic *pGame = g_pApp->m_pGameLogic;
// 
// 	for (GameViewList::iterator i = pGame->m_GameViews.begin(),
// 		end = pGame->m_GameViews.end(); i != end; ++i)
// 	{
// 		(*i)->VOnRender(fTime, fElapsedTime);
// 	}
// 
// 	g_pApp->m_pGameLogic->VRenderDiagnostics();

	//
	// Clear the back buffer
	//
	auto pRTV = DXUTGetD3D11RenderTargetView();
	pd3dImmediateContext->ClearRenderTargetView(pRTV, Colors::MidnightBlue);

	//
	// Clear the depth stencil
	//
	auto pDSV = DXUTGetD3D11DepthStencilView();
	pd3dImmediateContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0, 0);

	//
	// Render the UI
	//
	g_HUD.OnRender(fElapsedTime);
	g_SampleUI.OnRender(fElapsedTime);
	D3DRenderer::m_pTextHelper->Begin();
	D3DRenderer::m_pTextHelper->SetInsertionPos(2, 0);
	D3DRenderer::m_pTextHelper->SetForegroundColor(Colors::Yellow);
	D3DRenderer::m_pTextHelper->DrawTextLine(DXUTGetFrameStats(DXUTIsVsyncEnabled()));
	D3DRenderer::m_pTextHelper->DrawTextLine(DXUTGetDeviceStats());
	D3DRenderer::m_pTextHelper->End();
}

void CALLBACK RenderEngineApp::OnD3D11ReleasingSwapChain(void* pUserContext)
{
	D3DRenderer::m_DialogResourceManager.OnD3D11ReleasingSwapChain();
}

void CALLBACK RenderEngineApp::OnD3D11DestroyDevice(void* pUserContext)
{
	if (g_pApp->m_pRenderer)
		g_pApp->m_pRenderer->VShutdown();
	D3DRenderer::m_DialogResourceManager.OnD3D11DestroyDevice();
	g_pApp->m_pRenderer = shared_ptr<IRenderer>(NULL);

// 	D3DRenderer::m_DialogResourceManager.OnD3D11DestroyDevice();
	DXUTGetGlobalResourceCache().OnDestroyDevice();
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

void CALLBACK RenderEngineApp::OnUpdateGame(double fTime, float fElapsedTime, void* pUserContext)
{
// 	if (g_pApp->HasModalDialog())
// 	{
// 		return;
// 	}
// 
// 	if (g_pApp->m_IsQuitting)
// 	{
// 		PostMessage(g_pApp->GetHwnd(), WM_CLOSE, 0, 0);
// 	}
// 
// 	if (g_pApp->m_pGameLogic)
// 	{
// 		IEventManager::Get()->VUpdate(20);
// 
// // 		if (g_pApp->m_pBaseSocketManager)
// // 			g_pApp->m_pBaseSocketManager->DoSelect(0);
// 
// 		g_pApp->m_pGameLogic->VOnUpdate(float(fTime), fElapsedTime);
// 	}

}

bool RenderEngineApp::AttachAsClient()
{
// 	ClientSocketManager *pClient = GCC_NEW ClientSocketManager(g_pApp->m_Options.m_gameHost, g_pApp->m_Options.m_listenPort);
// 	if (!pClient->Connect())
// 	{
// 		return false;
// 	}
// 	g_pApp->m_pBaseSocketManager = pClient;
// 	VCreateNetworkEventForwarder();

	return true;
}

void RenderEngineApp::VCreateNetworkEventForwarder(void)
{
// 	if (m_pNetworkEventForwarder != NULL)
// 	{
// 		GCC_ERROR("Overwriting network event forwarder in TeapotWarsApp!");
// 		SAFE_DELETE(m_pNetworkEventForwarder);
// 	}
// 
// 	m_pNetworkEventForwarder = GCC_NEW NetworkEventForwarder(0);
// 
// 	IEventManager* pGlobalEventManager = IEventManager::Get();
// 	pGlobalEventManager->VAddListener(MakeDelegate(m_pNetworkEventForwarder, &NetworkEventForwarder::ForwardEvent), EvtData_Request_New_Actor::sk_EventType);
// 	pGlobalEventManager->VAddListener(MakeDelegate(m_pNetworkEventForwarder, &NetworkEventForwarder::ForwardEvent), EvtData_Environment_Loaded::sk_EventType);
// 	pGlobalEventManager->VAddListener(MakeDelegate(m_pNetworkEventForwarder, &NetworkEventForwarder::ForwardEvent), EvtData_PhysCollision::sk_EventType);
}

void RenderEngineApp::VDestroyNetworkEventForwarder(void)
{
// 	if (m_pNetworkEventForwarder)
// 	{
// 		IEventManager* pGlobalEventManager = IEventManager::Get();
// 		pGlobalEventManager->VRemoveListener(MakeDelegate(m_pNetworkEventForwarder, &NetworkEventForwarder::ForwardEvent), EvtData_Request_New_Actor::sk_EventType);
// 		pGlobalEventManager->VRemoveListener(MakeDelegate(m_pNetworkEventForwarder, &NetworkEventForwarder::ForwardEvent), EvtData_Environment_Loaded::sk_EventType);
// 		pGlobalEventManager->VRemoveListener(MakeDelegate(m_pNetworkEventForwarder, &NetworkEventForwarder::ForwardEvent), EvtData_PhysCollision::sk_EventType);
// 		SAFE_DELETE(m_pNetworkEventForwarder);
// 	}
}
