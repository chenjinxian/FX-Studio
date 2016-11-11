#include "TinyEngineApp.h"

TinyEngineApp* g_pApp = nullptr;

TinyEngineApp::TinyEngineApp()
{
}


TinyEngineApp::~TinyEngineApp()
{
}

HWND TinyEngineApp::SetupWindow(HINSTANCE hInstance, WNDPROC wndproc)
{
	m_hInstance = hInstance;

	bool fullscreen = false;

	WNDCLASSEX wndClass;

	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = wndproc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
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

	if (fullscreen)
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
					fullscreen = false;
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

	if (fullscreen)
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
	windowRect.right = fullscreen ? (long)screenWidth : (long)m_ScreenWidth;
	windowRect.bottom = fullscreen ? (long)screenHeight : (long)m_ScreenHeight;

	AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);

	m_hWindow = CreateWindowEx(0,
		VGetWindowClass().c_str(), VGetWindowTitle().c_str(),
		dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		0, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
		NULL, NULL, m_hInstance, NULL);

	if (!fullscreen)
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
