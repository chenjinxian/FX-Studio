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
	bool SetupWindow(HINSTANCE hInstance, HWND hWnd);
	bool InitRenderer();
	void RenderLoop();

	enum Renderer
	{
		Renderer_Unknown,
		Renderer_D3D11,
		Renderer_Vulkan
	};
	Renderer GetRendererType();

	virtual bool VLoadGame(void);
	bool IsRestoring() { return m_IsRestoring; }
	void SetQuitting(bool isExiting, bool isRestoring) { m_IsExiting = isExiting; m_IsRestoring = isRestoring; }
	void OnResize(int screenWidth, int screenHeight);
	void OnDispatchMsg(AppMsg msg);
	void OnStartRender() { m_GameTime.Reset(); }
	void OnRenderFrame();
	void OnClose();

	uint32_t AddEffect(const std::string& effectObjectPath, const std::string& effectName);
	uint32_t ModifyEffect(const std::string& effectObjectPath, const std::string& effectName);
	const std::string& GetEffectXml(const std::string& effectObjectPath);
	void ModifyMaterial(const std::string& materialPath, bool withEffect);

	BaseGameLogic* GetGameLogic(void) const { return m_pGameLogic; }
	GameConfig& GetGameConfig() { return m_Config; }
	ResCache* GetResCache() const { return m_pResCache; }
	shared_ptr<IRenderer> GetRendererAPI() { return m_pRenderer; }

protected:
	virtual const wchar_t* VGetWindowTitle() = 0;
	virtual const wchar_t* VGetWindowClass() = 0;
	virtual HICON VGetIcon() = 0;
	virtual void VRegisterGameEvents(void) {}
	virtual BaseGameLogic* VCreateGameAndView() = 0;

	BaseGameLogic* m_pGameLogic;
	GameConfig m_Config;
	ResCache* m_pResCache;
	shared_ptr<IRenderer> m_pRenderer;

private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnUpdate(const GameTime& gameTime);
	void OnRender(const GameTime& gameTime);

	bool InitResource();
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
	std::map<std::wstring, std::wstring> m_TextResource;
	std::map<std::wstring, UINT> m_Hotkeys;
};

extern BaseGameApp* g_pApp;