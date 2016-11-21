#include "BaseGameApp.h"
#include "GameConfig.h"
#include "../Graphics3D/D3D11Renderer.h"
#include "../ResourceCache/ResCache.h"
#include "../ResourceCache/XmlResource.h"
#include "../Graphics3D/TextureResourceLoader.h"
#include "../EventManager/EventManagerImpl.h"

BaseGameApp* g_pApp = nullptr;
const int BaseGameApp::MEGABYTE = 1024 * 1024;

BaseGameApp::BaseGameApp()
	: m_pResCache(nullptr),
	m_pRenderer(nullptr),
	m_pGameLogic(nullptr)
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

	SetCursor(NULL);

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

	IResourceFile *zipFile = (m_IsEditorRunning || !m_Config.m_IsZipResource) ?
		DEBUG_NEW DevelopmentResourceZipFile(L"Assets.zip", DevelopmentResourceZipFile::Editor) :
		DEBUG_NEW ResourceZipFile(L"Assets.zip");

	m_pResCache = unique_ptr<ResCache>(DEBUG_NEW ResCache(50, zipFile));

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

	if (!LoadStrings("English"))
	{
		DEBUG_ERROR("Failed to load strings");
		return false;
	}

	m_pEventManager = DEBUG_NEW EventManager("TinyEngine Event Manager", true);
	if (!m_pEventManager)
	{
		DEBUG_ERROR("Failed to create EventManager.");
		return false;
	}

	m_pGameLogic = VCreateGameAndView();
	if (m_pGameLogic == nullptr)
		return false;


// 	_tcscpy_s(m_saveGameDirectory, GetSaveGameDirectory(GetHwnd(), VGetGameAppDirectory()));

	m_pResCache->Preload("*.dds", NULL);
	m_pResCache->Preload("*.jpg", NULL);
	m_pResCache->Preload("*.png", NULL);
	m_pResCache->Preload("*.bmp", NULL);
	m_pResCache->Preload("*.tiff", NULL);

	return true;
}

HWND BaseGameApp::SetupWindow(HINSTANCE hInstance)
{
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
					return nullptr;
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
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	}

	RECT windowRect;
	windowRect.left = 0L;
	windowRect.top = 0L;
	windowRect.right = m_Config.m_IsFullScreen ? (long)screenWidth : (long)m_Config.m_ScreenWidth;
	windowRect.bottom = m_Config.m_IsFullScreen ? (long)screenHeight : (long)m_Config.m_ScreenHeight;

	AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);

	m_hWindow = CreateWindowEx(0,
		VGetWindowClass(), VGetWindowTitle(),
		dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		0, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
		NULL, NULL, m_hInstance, NULL);

	if (!m_Config.m_IsFullScreen)
	{
		uint32_t x = (screenWidth - windowRect.right) / 2;
		uint32_t y = (screenHeight - windowRect.bottom) / 2;
		SetWindowPos(m_hWindow, 0, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	}

	if (!m_hWindow)
	{

	}

	ShowWindow(m_hWindow, SW_SHOW);
	SetForegroundWindow(m_hWindow);
	SetFocus(m_hWindow);

	return m_hWindow;
}

bool BaseGameApp::InitRenderer()
{
	if (GetRendererAPI() == Renderer_D3D11)
	{
		m_pRenderer = shared_ptr<IRenderer>(DEBUG_NEW D3D11Renderer());
	}
	else if (GetRendererAPI() == Renderer_Vulkan)
	{
// 		m_pRenderer = shared_ptr<IRenderer>(DEBUG_NEW VulkanRenderer());
	}
	m_pRenderer->VSetBackgroundColor(Color(0.392156899f, 0.584313750f, 0.929411829f, 1.000000000f));

// 	m_IsRunning = true;

	m_pRenderer = shared_ptr<IRenderer>(DEBUG_NEW D3D11Renderer());
	if (m_pRenderer != nullptr)
	{
		return m_pRenderer->VInitRenderer(m_hWindow);
	}
	else
	{
		DEBUG_ERROR("out of memory.");
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

// 		render();
		Sleep(20);
		
		m_GameTime.UpdateGameTime();
	}
}

LRESULT CALLBACK BaseGameApp::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
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

	tinyxml2::XMLElement* pRoot = XmlResourceLoader::LoadAndReturnRootXmlElement(languageFile.c_str());
	if (!pRoot)
	{
		DEBUG_ERROR("Strings are missing.");
		return false;
	}

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

BaseGameApp::Renderer BaseGameApp::GetRendererAPI()
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
