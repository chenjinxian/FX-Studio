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

