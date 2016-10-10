// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the FXSTUDIOCORE_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// FXSTUDIOCORE_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#include <wtypes.h>

#ifdef FXSTUDIOCORE_EXPORTS
#define FXSTUDIOCORE_API __declspec(dllexport)
#else
#define FXSTUDIOCORE_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

	FXSTUDIOCORE_API int CreateInstance(
		int *instancePtrAddress,
		int *hPrevInstancePtrAddress,
		int *hWndPtrAddress,
		int nCmdShow,
		int screenWidth, int screenHeight);

	FXSTUDIOCORE_API int DestroyInstance();

	FXSTUDIOCORE_API void WndProc(int *hWndPtrAddress, int msg, int wParam, int lParam);

	FXSTUDIOCORE_API void RenderFrame();

	FXSTUDIOCORE_API void OpenLevel(BSTR lFileName);


#ifdef __cplusplus
}
#endif //__cplusplus