#include "BaseGameApp.h"
#include "GameConfig.h"
#include "BaseGameLogic.h"
#include "../Graphics3D/D3D11Renderer.h"
#include "../ResourceCache/ResCache.h"
#include "../ResourceCache/XmlResource.h"
#include "../EventManager/EventManagerImpl.h"
#include "../ResourceCache/ShaderResource.h"

BaseGameApp* g_pApp = nullptr;
const int BaseGameApp::MEGABYTE = 1024 * 1024;

BaseGameApp::BaseGameApp()
	: m_Config(),
	m_pResCache(nullptr),
	m_pRenderer(nullptr),
	m_pGameLogic(nullptr),
	m_hInstance(nullptr),
	m_hWindow(nullptr),
	m_pEventManager(nullptr),
	m_HasModalDialog(false),
	m_IsExiting(false),
	m_IsRestoring(false)
{
	g_pApp = this;
}

BaseGameApp::~BaseGameApp()
{
}

bool BaseGameApp::InitEnvironment()
{
#ifndef _DEBUG
	if (!Utility::IsOnlyInstance(VGetWindowTitle()))
		return false;
#endif

	bool resourceCheck = false;
	while (!resourceCheck)
	{
		const DWORDLONG physicalRAM = 512 * MEGABYTE;
		const DWORDLONG virtualRAM = 1024 * MEGABYTE;
		const DWORDLONG diskSpace = 10 * MEGABYTE;
		if (!Utility::CheckStorage(diskSpace))
			return false;

		const DWORD minCpuSpeed = 1300;
		DWORD thisCPU = Utility::ReadCPUSpeed();
		if (thisCPU < minCpuSpeed)
		{
			DEBUG_ERROR("GetCPUSpeed reports CPU is too slow for this game.");
			return false;
		}

		resourceCheck = true;
	}

	RegisterEngineEvents();
	VRegisterGameEvents();

// 	if (!LoadStrings("English"))
// 	{
// 		DEBUG_ERROR("Failed to load strings");
// 		return false;
// 	}

	if (m_pEventManager == nullptr)
	{
		m_pEventManager = DEBUG_NEW EventManager("TinyEngine Event Manager", true);
		if (!m_pEventManager)
		{
			DEBUG_ERROR("Failed to create EventManager.");
			return false;
		}
	}

	return true;
}

bool BaseGameApp::SetupWindow(HINSTANCE hInstance, HWND hWnd)
{
	if (hWnd)
	{
		m_hWindow = hWnd;
		return true;
	}

	m_hInstance = hInstance;

	WNDCLASSEX wndClass;
	ZeroMemory(&wndClass, sizeof(wndClass));

	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = VGetIcon();
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = VGetWindowClass();
	wndClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);

	if (!RegisterClassEx(&wndClass))
	{
		return false;
	}

	const int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	const int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	if (m_Config.m_IsFullScreen)
	{
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = screenWidth;
		dmScreenSettings.dmPelsHeight = screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		if ((m_Config.m_ScreenWidth != screenWidth) && (m_Config.m_ScreenHeight != screenHeight))
		{
			if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
			{
				if (MessageBox(NULL, L"Fullscreen Mode not supported!\n Switch to window mode?", L"Error",
					MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
				{
					m_Config.m_IsFullScreen = false;
				}
				else
				{
					return false;
				}
			}
		}

	}

	DWORD dwExStyle = 0;;
	DWORD dwStyle = 0;

	if (m_Config.m_IsFullScreen)
	{
		dwExStyle = WS_EX_APPWINDOW;
		dwStyle = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

		m_Config.m_ScreenWidth = screenWidth;
		m_Config.m_ScreenHeight = screenHeight;
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	}

	RECT windowRect;
	windowRect.left = 0L;
	windowRect.top = 0L;
	windowRect.right = (long)m_Config.m_ScreenWidth;
	windowRect.bottom = (long)m_Config.m_ScreenHeight;

	AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);

	m_hWindow = CreateWindowEx(dwExStyle, VGetWindowClass(), VGetWindowTitle(), dwStyle,
		0, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
		NULL, NULL, m_hInstance, NULL);

	if (!m_hWindow)
	{
		return false;
	}

	if (!m_Config.m_IsFullScreen)
	{
		uint32_t x = (screenWidth - windowRect.right) / 2;
		uint32_t y = (screenHeight - windowRect.bottom) / 2;
		SetWindowPos(m_hWindow, 0, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	}

	ShowWindow(m_hWindow, SW_SHOW);
	UpdateWindow(m_hWindow);
	SetForegroundWindow(m_hWindow);

	return true;
}

bool BaseGameApp::InitRenderer()
{
	if (GetRendererType() == Renderer_D3D11)
	{
		try
		{
			m_pRenderer = shared_ptr<IRenderer>(DEBUG_NEW D3D11Renderer());
		}
		catch (std::invalid_argument e)
		{
			MessageBoxA(nullptr, e.what(), nullptr, MB_OK | MB_ICONERROR);
			return false;
		}
	}
	else if (GetRendererType() == Renderer_Vulkan)
	{
// 		m_pRenderer = shared_ptr<IRenderer>(DEBUG_NEW VulkanRenderer());
	}

// 	m_IsRunning = true;

	if (m_pRenderer != nullptr && m_pRenderer->VInitRenderer(m_hWindow))
	{
		m_pRenderer->VSetBackgroundColor(Color(0.1f, 0.1f, 0.1f, 1.0f));

		if (m_pGameLogic == nullptr)
		{
			m_pGameLogic = VCreateGameAndView();
			if (m_pGameLogic == nullptr)
				return false;
		}

		return true;
	}
	else
	{
		DEBUG_ERROR("Renderer init failed.");
		return false;
	}
}

void BaseGameApp::RenderLoop()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	m_GameTime.Reset();

	while (TRUE)
	{
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
		{
			break;
		}

		OnRender(m_GameTime);
		
		m_GameTime.UpdateGameTime();
		OnUpdate(m_GameTime);
	}
}

void BaseGameApp::OnUpdate(const GameTime& gameTime)
{
	if (g_pApp->HasModalDialog())
	{
		return;
	}

	if (g_pApp->m_IsExiting)
	{
		PostMessage(m_hWindow, WM_CLOSE, 0, 0);
	}

	if (g_pApp->m_pGameLogic != nullptr)
	{
		IEventManager::Get()->VUpdate(20);
		g_pApp->m_pGameLogic->VOnUpdate(gameTime);
	}
}

void BaseGameApp::OnRender(const GameTime& gameTime)
{
	BaseGameLogic *pGame = g_pApp->m_pGameLogic;

	for (GameViewList::iterator i = pGame->m_GameViews.begin(), end = pGame->m_GameViews.end(); i != end; ++i)
	{
		(*i)->VOnRender(gameTime);
	}

	g_pApp->m_pGameLogic->VRenderDiagnostics();
}

LRESULT CALLBACK BaseGameApp::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DISPLAYCHANGE:
		break;
	case WM_SYSCOMMAND:
	{
		switch (wParam)
		{
		case SC_CLOSE:
			g_pApp->m_IsExiting = true;
			return 0;
		default:
			break;
		}
		break;
	}
	case WM_CLOSE:
		if (g_pApp->m_IsExiting)
		{
			g_pApp->OnClose();
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_SIZE:
	{
		if (wParam != SIZE_MINIMIZED)
		{
			g_pApp->OnResize(LOWORD(lParam), HIWORD(lParam));
		}
		break;
	}
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_CHAR:
	case WM_MOUSEMOVE:
	case WM_MOUSEWHEEL:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_LBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
	{
		if (g_pApp->m_pGameLogic != nullptr)
		{
			AppMsg msg;
			msg.m_hWnd = hWnd;
			msg.m_uMsg = uMsg;
			msg.m_wParam = wParam;
			msg.m_lParam = lParam;

			g_pApp->OnDispatchMsg(msg);
		}
		break;
	}
	}

	return (DefWindowProc(hWnd, uMsg, wParam, lParam));
}

void BaseGameApp::RegisterEngineEvents(void)
{

}

bool BaseGameApp::LoadStrings(std::string language)
{
	std::string languageFile = "Strings\\";
	languageFile += language;
	languageFile += ".xml";

	unique_ptr<tinyxml2::XMLDocument> pDoc = unique_ptr<tinyxml2::XMLDocument>(DEBUG_NEW tinyxml2::XMLDocument());
	if (pDoc != nullptr && (pDoc->LoadFile(languageFile.c_str()) == tinyxml2::XML_SUCCESS))
	{
		tinyxml2::XMLElement *pRoot = pDoc->RootElement();
		if (pRoot == nullptr)
			return false;

		for (tinyxml2::XMLElement* pElem = pRoot->FirstChildElement(); pElem; pElem = pElem->NextSiblingElement())
		{
			const char *pKey = pElem->Attribute("id");
			const char *pText = pElem->Attribute("value");
			const char *pHotkey = pElem->Attribute("hotkey");
			if (pKey && pText)
			{
				wchar_t wideKey[64];
				wchar_t wideText[1024];
				Utility::AnsiToWideCch(wideKey, pKey, 64);
				Utility::AnsiToWideCch(wideText, pText, 1024);
				m_TextResource[std::wstring(wideKey)] = std::wstring(wideText);

				if (pHotkey)
				{
					m_Hotkeys[std::wstring(wideKey)] = MapCharToKeycode(*pHotkey);
				}
			}
		}
	}

	return true;
}

std::wstring BaseGameApp::GetString(std::wstring sID)
{
	auto localizedString = m_TextResource.find(sID);
	if (localizedString == m_TextResource.end())
	{
		DEBUG_ASSERT(0 && "String not found!");
		return L"";
	}
	return localizedString->second;
}

UINT BaseGameApp::MapCharToKeycode(const char hotKey)
{
	if (hotKey >= '0' && hotKey <= '9')
		return 0x30 + hotKey - '0';

	if (hotKey >= 'A' && hotKey <= 'Z')
		return 0x41 + hotKey - 'A';

	DEBUG_ASSERT(0 && "Platform specific hotkey is not defined");
	return 0;
}

BaseGameApp::Renderer BaseGameApp::GetRendererType()
{
	if (m_Config.m_Renderer == "Direct3D 11")
	{
		return Renderer_D3D11;
	}
	else if (m_Config.m_Renderer == "Vulkan")
	{
		return Renderer_Vulkan;
	}
	else
	{
		return Renderer_Unknown;
	}
}

bool BaseGameApp::VLoadGame(void)
{
	InitResource();
	return m_pGameLogic->VLoadGame(m_Config.m_Project);
}

void BaseGameApp::OnClose()
{
	if (!m_IsRestoring)
	{
		SAFE_DELETE(m_pGameLogic);
		SAFE_DELETE(m_pEventManager);
		SAFE_DELETE(m_pResCache);
	}

	if (m_pRenderer != nullptr)
	{
		m_pRenderer->VDeleteRenderer();
		m_pRenderer.reset();
	}
	DestroyWindow(m_hWindow);
	m_IsExiting = false;
}

bool BaseGameApp::InitResource()
{
	if (m_pResCache == nullptr)
	{
		m_pResCache = DEBUG_NEW ResCache(50, Utility::GetDirectory(m_Config.m_Project), m_Config.m_IsZipResource);

		if (!m_pResCache->Init())
		{
			DEBUG_ERROR("Failed to initialize resource cache!  Are your paths set up correctly?");
			return false;
		}

		m_pResCache->RegisterLoader(CreateDdsResourceLoader());
		m_pResCache->RegisterLoader(CreateJpgResourceLoader());
		m_pResCache->RegisterLoader(CreatePngResourceLoader());
		m_pResCache->RegisterLoader(CreateBmpResourceLoader());
		m_pResCache->RegisterLoader(CreateTiffResourceLoader());
		m_pResCache->RegisterLoader(CreateXmlResourceLoader());
		m_pResCache->RegisterLoader(CreateFxSourceEffectResourceLoader());
		m_pResCache->RegisterLoader(CreateFxObjectEffectResourceLoader());
	}

	m_pResCache->Preload("*.dds", NULL);
	m_pResCache->Preload("*.jpg", NULL);
	m_pResCache->Preload("*.png", NULL);
	m_pResCache->Preload("*.bmp", NULL);
	m_pResCache->Preload("*.tiff", NULL);
	m_pResCache->Preload("*.fx", NULL);
	m_pResCache->Preload("*.fxo", NULL);
	return true;
}

void BaseGameApp::OnResize(int screenWidth, int screenHeight)
{
	if (g_pApp->m_pGameLogic != nullptr && g_pApp->m_pRenderer != nullptr)
	{
		for (auto view : g_pApp->m_pGameLogic->m_GameViews)
		{
			view->VOnDeleteGameViews();
		}

		g_pApp->m_Config.m_ScreenWidth = screenWidth;
		g_pApp->m_Config.m_ScreenHeight = screenHeight;
		g_pApp->m_pRenderer->VResizeSwapChain();

		for (auto view : g_pApp->m_pGameLogic->m_GameViews)
		{
			view->VOnInitGameViews();
		}
	}
}

void BaseGameApp::OnDispatchMsg(AppMsg msg)
{
	if (g_pApp->m_pGameLogic != nullptr)
	{
		for (auto gameView : g_pApp->m_pGameLogic->m_GameViews)
		{
			if (gameView->VOnMsgProc(msg))
			{
				break;
			}
		}
	}
}

void BaseGameApp::OnRenderFrame()
{
	OnRender(m_GameTime);

	m_GameTime.UpdateGameTime();
	OnUpdate(m_GameTime);
}

uint32_t BaseGameApp::AddEffect(const std::string& effectObjectPath, const std::string& effectSourcePath, const std::string& effectName)
{
	Resource effectRes(effectObjectPath);
	shared_ptr<ResHandle> pEffectResHandle = g_pApp->GetResCache()->GetHandle(&effectRes);
	if (pEffectResHandle != nullptr)
	{
		shared_ptr<HlslResourceExtraData> extra = static_pointer_cast<HlslResourceExtraData>(pEffectResHandle->GetExtraData());
		if (extra != nullptr)
		{
			Effect* pEffect = extra->GetEffect();
			return pEffect->GenerateXml(effectObjectPath, effectSourcePath, effectName).length();
		}
	}

	return 0;
}

const std::string& BaseGameApp::GetEffectXml(const std::string& effectObjectPath)
{
	Resource effectRes(effectObjectPath);
	shared_ptr<ResHandle> pEffectResHandle = g_pApp->GetResCache()->GetHandle(&effectRes);
	if (pEffectResHandle != nullptr)
	{
		shared_ptr<HlslResourceExtraData> extra = static_pointer_cast<HlslResourceExtraData>(pEffectResHandle->GetExtraData());
		if (extra != nullptr)
		{
			Effect* pEffect = extra->GetEffect();
			return pEffect->GetEffectXmlString();
		}
	}

	return nullptr;
}
