// stdafx.cpp : source file that includes just the standard includes
// RenderEngine.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "winmm.lib")

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
