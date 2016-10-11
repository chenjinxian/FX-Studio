#pragma once
#include "../RenderEngineBase.h"

extern bool CheckStorage(DWORDLONG diskSpaceNeeded);
extern DWORD ReadCPUSpeed();
extern bool CheckMemory(DWORDLONG physicalRAMNeeded, DWORDLONG virtualRAMNeeded);
extern bool IsOnlyInstance(LPCTSTR gameTitle);
extern const TCHAR *GetSaveGameDirectory(HWND hWnd, const TCHAR *gameAppDirectory);

struct GameOptions
{
	GameOptions();
	~GameOptions() { SAFE_DELETE(m_pDoc); }

	void Init(const std::string& xmlFilePath, LPWSTR lpCmdLine);

	std::string m_Level;

	std::string m_Renderer;
	bool m_IsRunFullSpeed;
	uint32_t m_ScreenWidth;
	uint32_t m_ScreenHeight;

	float m_SoundEffectsVolume;			
	float m_MusicVolume;				

	int m_ExpectedPlayers;				
	int m_ListenPort;					
	std::string m_GameHost;				
	int m_NumAIs;
	int m_MaxAIs;
	int m_MaxPlayers;

	bool m_IsDevelopmentDirectories;

	TiXmlDocument *m_pDoc;
};
