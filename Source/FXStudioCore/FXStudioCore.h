// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the FXSTUDIOCORE_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// FXSTUDIOCORE_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#include "ModelImporter.h"
#include "FXStudioApp.h"
#include <wtypes.h>

#ifdef FXSTUDIOCORE_EXPORTS
#define FXSTUDIOCORE_API __declspec(dllexport)
#else
#define FXSTUDIOCORE_API __declspec(dllimport)
#endif

#define FX_APIENTRY __stdcall 

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

	FXSTUDIOCORE_API bool FX_APIENTRY CreateInstance(
		int* instancePtrAddress,
		int* hPrevInstancePtrAddress,
		int* hWndMain,
		int* hWndMaterial,
		int nCmdShow,
		int screenWidth, int screenHeight,
		int materialWidth, int materialHeight);
	FXSTUDIOCORE_API int FX_APIENTRY DestroyInstance();

	FXSTUDIOCORE_API void FX_APIENTRY ResizeWnd(int screenWidth, int screenHeight, int wndIndex);
	FXSTUDIOCORE_API void FX_APIENTRY WndProc(int *hWndPtrAddress, int msg, int* wParam, int* lParam);
	FXSTUDIOCORE_API void FX_APIENTRY RenderFrame();

	FXSTUDIOCORE_API bool FX_APIENTRY IsGameRunning();

	FXSTUDIOCORE_API void FX_APIENTRY OpenProject(BSTR lFileName);

	FXSTUDIOCORE_API void FX_APIENTRY SetCameraType(int type);
	FXSTUDIOCORE_API void FX_APIENTRY SetTransformType(int type);
	FXSTUDIOCORE_API unsigned int FX_APIENTRY GetPickedActor(int cursorX, int cursorY);
	FXSTUDIOCORE_API void FX_APIENTRY SetPickedActor(unsigned int actorId);

	FXSTUDIOCORE_API unsigned int FX_APIENTRY AddActor(BSTR actorResource);
	FXSTUDIOCORE_API bool FX_APIENTRY ModifyActor(BSTR modificationXml);
	FXSTUDIOCORE_API bool FX_APIENTRY RemoveActor(unsigned int actorId);

	FXSTUDIOCORE_API int FX_APIENTRY ImportModel(BSTR modelImportPath, BSTR modelExportPath, ProgressCallback progressCallback);
	FXSTUDIOCORE_API unsigned int FX_APIENTRY AddEffect(BSTR effectObjectPath, BSTR effectName);
	FXSTUDIOCORE_API unsigned int FX_APIENTRY ModifyEffect(BSTR effectObjectPath, BSTR effectName);
	FXSTUDIOCORE_API void FX_APIENTRY GetMaterialXml(BSTR effectObjectPath, char* effectXmlPtr, unsigned int size);
	FXSTUDIOCORE_API void FX_APIENTRY ModifyMaterial(BSTR materialPath, bool withEffect);
	FXSTUDIOCORE_API void FX_APIENTRY AddMaterial(BSTR materialName);

	FXSTUDIOCORE_API void FX_APIENTRY SetMoveDelegate(MoveDelegate delegate);

#ifdef __cplusplus
}
#endif //__cplusplus