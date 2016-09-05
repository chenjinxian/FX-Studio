#pragma once
#include "RenderEngineInterface.h"

#ifdef RENDERENGINE_EXPORTS
#define RENDERENGINE_API __declspec(dllexport)
#else
#define RENDERENGINE_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus
	
	RENDERENGINE_API int CreateInstance(
		int *instancePtrAddress,
		int *hPrevInstancePtrAddress,
		int *hWndPtrAddress,
		int nCmdShow,
		int screenWidth, int screenHeight);

	RENDERENGINE_API int DestroyInstance();

	RENDERENGINE_API void WndProc(int *hWndPtrAddress, int msg, int wParam, int lParam);

	RENDERENGINE_API void RenderFrame();

#ifdef __cplusplus
}
#endif //__cplusplus