#include "TinyEngineApp.h"
#include "TinyEngineConfig.h"
#include "../Graphics3D/D3D11Renderer.h"
#include <chrono>

TinyEngineApp* g_pApp = nullptr;

TinyEngineApp::TinyEngineApp()
	: m_pResCache(nullptr),
	m_pRenderer(nullptr)
{
	g_pApp = this;
}

TinyEngineApp::~TinyEngineApp()
{
}

bool TinyEngineApp::InitEnvironment()
{
	return true;
}

HWND TinyEngineApp::SetupWindow(HINSTANCE hInstance, WNDPROC wndproc)
{
	m_hInstance = hInstance;

	WNDCLASSEX wndClass;

	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = wndproc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = VGetIcon();
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = VGetWindowClass().c_str();
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

		if ((m_ScreenWidth != screenWidth) && (m_ScreenHeight != screenHeight))
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
	windowRect.right = m_Config.m_IsFullScreen ? (long)screenWidth : (long)m_ScreenWidth;
	windowRect.bottom = m_Config.m_IsFullScreen ? (long)screenHeight : (long)m_ScreenHeight;

	AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);

	m_hWindow = CreateWindowEx(0,
		VGetWindowClass().c_str(), VGetWindowTitle().c_str(),
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

void TinyEngineApp::HandleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
}

bool TinyEngineApp::InitRenderer()
{
	m_pRenderer = shared_ptr<IRenderer>(DEBUG_NEW D3D11Renderer());
	if (m_pRenderer != nullptr)
	{
		return m_pRenderer->VInitRenderer();
	}
	else
	{
		DEBUG_ERROR("out of memory.");
		return false;
	}
}

void TinyEngineApp::RenderLoop()
{
	auto tStart = std::chrono::high_resolution_clock::now();

	MSG msg;
	while (TRUE)
	{
// 		if (viewUpdated)
// 		{
// 			viewUpdated = false;
// 			viewChanged();
// 		}

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
// 		frameCounter++;
// 		auto tEnd = std::chrono::high_resolution_clock::now();
// 		auto tDiff = std::chrono::duration<double, std::milli>(tEnd - tStart).count();
// 		frameTimer = (float)tDiff / 1000.0f;
// 		// Convert to clamped timer value
// 		if (!paused)
// 		{
// 			timer += timerSpeed * frameTimer;
// 			if (timer > 1.0)
// 			{
// 				timer -= 1.0f;
// 			}
// 		}
// 		fpsTimer += (float)tDiff;
// 		if (fpsTimer > 1000.0f)
// 		{
// 			lastFPS = roundf(1.0f / frameTimer);
// 			updateTextOverlay();
// 			fpsTimer = 0.0f;
// 			frameCounter = 0;
// 		}
	}
}
