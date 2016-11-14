#include "DemoApp.h"

DemoApp g_DemoApp;

DemoApp::DemoApp() : BaseGameApp()
{
}

DemoApp::~DemoApp()
{
}

const wchar_t* DemoApp::VGetWindowTitle()
{
	return _T("TinyEngine - Demo");
}

const wchar_t* DemoApp::VGetWindowClass()
{
	return _T("TinyEngine");
}

HICON DemoApp::VGetIcon()
{
	return LoadIcon(NULL, IDI_APPLICATION);
}
