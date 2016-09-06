#ifdef FX_STUDIODLL_EXPORTS
#define FX_STUDIODLL_API __declspec(dllexport)
#else
#define FX_STUDIODLL_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

	FX_STUDIODLL_API int CreateInstance(
		int *instancePtrAddress,
		int *hPrevInstancePtrAddress,
		int *hWndPtrAddress,
		int nCmdShow,
		int screenWidth, int screenHeight);

	FX_STUDIODLL_API int DestroyInstance();

	FX_STUDIODLL_API void WndProc(int *hWndPtrAddress, int msg, int wParam, int lParam);

	FX_STUDIODLL_API void RenderFrame();

#ifdef __cplusplus
}
#endif //__cplusplus