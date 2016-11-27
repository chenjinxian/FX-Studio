#pragma once
#include "../TinyEngineBase.h"
#include "../TinyEngineInterface.h"
#include "GameConfig.h"
#include "GameTime.h"

class BaseGameLogic;
class EventManager;
class TinyEngineConfig;
class ResCache;

class BaseGameApp : public boost::noncopyable
{
public:
	BaseGameApp();
	virtual ~BaseGameApp();

	bool InitEnvironment();
	HWND SetupWindow(HINSTANCE hInstance);
	bool InitRenderer();
	bool InitResource();
	void RenderLoop();

	enum Renderer
	{
		Renderer_Unknown,
		Renderer_D3D11,
		Renderer_Vulkan
	};
	Renderer GetRendererAPI();

	virtual bool VLoadGame(void);
	bool IsRestoring() { return m_IsRestoring; }
	void SetQuitting(bool isExiting, bool isRestoring) { m_IsExiting = isExiting; m_IsRestoring = isRestoring; }
	bool AttachAsClient();

	BaseGameLogic* GetGameLogic(void) const { return m_pGameLogic; }

	GameConfig m_Config;
	unique_ptr<ResCache> m_pResCache;
	shared_ptr<IRenderer> m_pRenderer;

protected:
	virtual const wchar_t* VGetWindowTitle() = 0;
	virtual const wchar_t* VGetWindowClass() = 0;
	virtual HICON VGetIcon() = 0;
	virtual void VRegisterGameEvents(void) {}
	virtual BaseGameLogic* VCreateGameAndView() = 0;

	BaseGameLogic* m_pGameLogic;

private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnClose();
	void OnUpdate(const GameTime& gameTime);
	void OnRender(const GameTime& gameTime);

	bool HasModalDialog() { return m_HasModalDialog != 0; }
	void RegisterEngineEvents(void);
	bool LoadStrings(std::string language);
	std::wstring GetString(std::wstring sID);
	UINT MapCharToKeycode(const char hotKey);

	static const int MEGABYTE;

	HINSTANCE m_hInstance;
	HWND m_hWindow;

	GameTime m_GameTime;
	EventManager* m_pEventManager;

	int m_HasModalDialog;
	bool m_IsExiting;
	bool m_IsRestoring;
	bool m_IsEditorRunning;
	std::map<std::wstring, std::wstring> m_TextResource;
	std::map<std::wstring, UINT> m_Hotkeys;
};

extern BaseGameApp* g_pApp;