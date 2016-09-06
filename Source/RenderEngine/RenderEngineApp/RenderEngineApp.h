#pragma once

#include "../RenderEngineInterface.h"
#include "../Mainloop/InitialOptions.h"

class BaseGameLogic;
class ResCache;

class RenderEngineApp : public boost::noncopyable
{
public:
	RenderEngineApp(HINSTANCE hInstance, LPWSTR lpCmdLine, HWND hWnd = NULL, int screenWidth = SCREEN_WIDTH, int screenHeight = SCREEN_HEIGHT);
	virtual ~RenderEngineApp();

	HINSTANCE GetInstance() const { return m_Instance; }
	HWND GetWindowHandle() const { return m_WindowHandle; }
	virtual const std::wstring VGetWindowTitle() = 0;
	virtual HICON VGetIcon() = 0;
	int GetScreenWidth() const { return m_ScreenWidth; }
	int GetScreenHeight() const { return m_ScreenHeight; }

	virtual int Run();
	virtual int Exit();
	virtual bool Initialize();

	virtual bool VLoadGame();
	virtual BaseGameLogic* VCreateGameAndView() = 0;
	BaseGameLogic* GetGameLogic() const { return m_pGame; }
	ResCache* GetResCache() const { return m_pResCache; }

	InitialOptions m_Options;
	shared_ptr<IRenderer> m_pRenderer;

protected:
	virtual bool InitializeOptions();

	static const UINT SCREEN_WIDTH;
	static const UINT SCREEN_HEIGHT;

	HINSTANCE m_Instance;
	LPWSTR m_CmdLine;
	HWND m_WindowHandle;
	UINT m_ScreenWidth;
	UINT m_ScreenHeight;

	BaseGameLogic* m_pGame;
	ResCache* m_pResCache;

private:
	static bool CALLBACK ModifyDeviceSettings(
		DXUTDeviceSettings* pDeviceSettings, void* pUserContext);

	static bool CALLBACK OnDeviceRemoved(void* pUserContext);

	static bool CALLBACK IsD3D11DeviceAcceptable(
		const CD3D11EnumAdapterInfo *AdapterInfo, UINT Output, const CD3D11EnumDeviceInfo *DeviceInfo,
		DXGI_FORMAT BackBufferFormat, bool bWindowed, void* pUserContext);

	static HRESULT CALLBACK OnD3D11CreateDevice(
		ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext);

	static HRESULT CALLBACK OnD3D11ResizedSwapChain(
		ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext);

	static void CALLBACK OnD3D11ReleasingSwapChain(void* pUserContext);

	static void CALLBACK OnD3D11DestroyDevice(void* pUserContext);

	static void CALLBACK OnFrameMove(
		double fTime, float fElapsedTime, void *pUserContext);

	static void CALLBACK OnD3D11FrameRender(
		ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime, float fElapsedTime, void* pUserContext);

	static LRESULT CALLBACK MsgProc(
		HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void *pUserContext);

	LRESULT OnClose();
};
