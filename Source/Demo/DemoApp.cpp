#include "DemoApp.h"

DemoApp g_DemoApp;

DemoApp::DemoApp() : BaseGameApp()
{
}

DemoApp::~DemoApp()
{
}

const std::wstring& DemoApp::VGetWindowTitle()
{
	return _T("TinyEngine - Demo");
}

const std::wstring& DemoApp::VGetWindowClass()
{
	return _T("TinyEngine");
}

HICON DemoApp::VGetIcon()
{
	return LoadIcon(NULL, IDI_APPLICATION);
}
