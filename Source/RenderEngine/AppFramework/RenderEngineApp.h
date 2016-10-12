#pragma once
#include "../RenderEngineBase.h"
#include "../RenderEngineInterface.h"
#include "../UserInterface/UserInterface.h"
#include "../Mainloop/Initialization.h"

class BaseGameLogic;
class HumanView;
class EventManager;

class RenderEngineApp : public boost::noncopyable
{
public:
	RenderEngineApp();
	virtual ~RenderEngineApp();

	virtual TCHAR *VGetGameTitle() = 0;
	virtual TCHAR *VGetGameAppDirectory() = 0;
	virtual HICON VGetIcon() = 0;

	HWND GetHwnd();
	HINSTANCE GetInstance() { return m_hInstance; }
	virtual bool InitInstance(HINSTANCE hInstance, LPWSTR lpCmdLine, HWND hWnd = NULL, int screenWidth = SCREEN_WIDTH, int screenHeight = SCREEN_HEIGHT);

	static LRESULT CALLBACK MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void *pUserContext);
	bool HasModalDialog() { return m_HasModalDialog != 0; }
	void ForceModalExit() { PostMessage(GetHwnd(), g_MsgEndModal, 0, g_QuitNoPrompt); }

	LRESULT OnDisplayChange(int colorDepth, int width, int height);
	LRESULT OnPowerBroadcast(int event);
	LRESULT OnSysCommand(WPARAM wParam, LPARAM lParam);
	LRESULT OnClose();

	LRESULT OnAltEnter();

	bool LoadStrings(std::string language);
	std::wstring GetString(std::wstring sID);
	UINT MapCharToKeycode(const char pHotkey);

	int Modal(shared_ptr<IScreenElement> pModalScreen, int defaultAnswer);

	int GetScreenWidth() const { return m_ScreenWidth; }
	int GetScreenHeight() const { return m_ScreenHeight; }

	enum Renderer
	{
		Renderer_Unknown,
		Renderer_D3D11
	};

	shared_ptr<IRenderer> m_pRenderer;

	static Renderer GetRendererImpl();

	static bool CALLBACK IsD3D11DeviceAcceptable(const CD3D11EnumAdapterInfo *AdapterInfo, UINT Output, const CD3D11EnumDeviceInfo *DeviceInfo, DXGI_FORMAT BackBufferFormat, bool bWindowed, void* pUserContext);
	static HRESULT CALLBACK OnD3D11CreateDevice(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext);
	static HRESULT CALLBACK OnD3D11ResizedSwapChain(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext);
	static void CALLBACK OnD3D11ReleasingSwapChain(void* pUserContext);
	static void CALLBACK OnD3D11DestroyDevice(void* pUserContext);
	static void CALLBACK OnD3D11FrameRender(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime, float fElapsedTime, void* pUserContext);

	static bool CALLBACK ModifyDeviceSettings(DXUTDeviceSettings* pDeviceSettings, void* pUserContext);
	static void CALLBACK OnUpdateGame(double fTime, float fElapsedTime, void *pUserContext);

	BaseGameLogic *m_pGameLogic;
	struct GameOptions m_Options;

	virtual BaseGameLogic *VCreateGameAndView() = 0;
	virtual bool VLoadGame(void);
	HumanView* GetHumanView();

	class ResCache *m_pResCache;
	TCHAR m_saveGameDirectory[MAX_PATH];

	bool IsEditorRunning() { return m_IsEditorRunning; }

	EventManager *m_pEventManager;

	bool AttachAsClient();

	void AbortGame() { m_IsQuitting = true; }
	int GetExitCode() { return DXUTGetExitCode(); }
	bool IsRunning() { return m_IsRunning; }
	void SetQuitting(bool quitting) { m_IsQuitting = quitting; }

	BaseGameLogic* GetGameLogic(void) const { return m_pGameLogic; }

protected:
	int PumpUntilMessage(UINT msgEnd, WPARAM* pWParam, LPARAM* pLParam);
	int	EatSpecificMessages(UINT msgType, boost::optional<LPARAM> lParam, boost::optional<WPARAM> wParam);
	void FlashWhileMinimized();

	virtual void VCreateNetworkEventForwarder(void);
	virtual void VDestroyNetworkEventForwarder(void);

	virtual void VRegisterGameEvents(void) {}

	HINSTANCE m_hInstance;
	bool m_IsFullScreen;
	bool m_IsRunning;
	bool m_IsQuitRequested;
	bool m_IsQuitting;
	bool m_IsEditorRunning;
	uint32_t m_ScreenWidth;
	uint32_t m_ScreenHeight;

	std::map<std::wstring, std::wstring> m_TextResource;
	std::map<std::wstring, UINT> m_Hotkeys;

	int m_HasModalDialog;

private:
	void RegisterEngineEvents(void);

	static const int MEGABYTE;
	static const float SIXTY_HERTZ;
	static const uint32_t SCREEN_WIDTH;
	static const uint32_t SCREEN_HEIGHT;
};

extern RenderEngineApp *g_pApp;
