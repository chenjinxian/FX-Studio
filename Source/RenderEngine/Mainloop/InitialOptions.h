#pragma once

class InitialOptions
{
public:
	InitialOptions();
	~InitialOptions();

	void Init(const std::string& xmlFilePath);
	static bool CheckStorage(const DWORDLONG diskSpaceNeeded);
	static DWORD ReadCPUSpeed();
	static bool CheckMemory(const DWORDLONG physicalRAMNeeded, const DWORDLONG virtualRAMNeeded);
	static bool IsOnlyInstance(const std::wstring& windowTitle);
	static const TCHAR *GetSaveGameDirectory(HWND hWnd, const TCHAR *gameAppDirectory);
	static bool CheckForJoystick(HWND hWnd);

public:
	std::string m_Level;
	std::string m_Renderer;
	bool m_RunFullSpeed;
	UINT m_ScreenWidth;
	UINT m_ScreenHeight;
	TiXmlDocument *m_pDoc;
};
