#include "Initialization.h"
#include <shlobj.h>
#include <direct.h>

bool CheckStorage(const DWORDLONG diskSpaceNeeded)
{
	int const drive = _getdrive();
	struct _diskfree_t diskfree;

	_getdiskfree(drive, &diskfree);

	unsigned __int64 const neededClusters =
		diskSpaceNeeded / (diskfree.sectors_per_cluster * diskfree.bytes_per_sector);

	if (diskfree.avail_clusters < neededClusters)
	{
		GCC_ERROR("CheckStorage Failure: Not enough physical storage.");
		return false;
	}
	return true;
}

bool CheckMemory(const DWORDLONG physicalRAMNeeded, const DWORDLONG virtualRAMNeeded)
{
	MEMORYSTATUSEX status;
	GlobalMemoryStatusEx(&status);
	if (status.ullTotalPhys < physicalRAMNeeded)
	{
		GCC_ERROR("CheckMemory Failure: Not enough physical memory.");
		return false;
	}

	if (status.ullAvailVirtual < virtualRAMNeeded)
	{
		GCC_ERROR("CheckMemory Failure: Not enough virtual memory.");
		return false;
	}

	char *buff = GCC_NEW char[(unsigned int)virtualRAMNeeded];
	if (buff)
		delete[] buff;
	else
	{
		GCC_ERROR("CheckMemory Failure: Not enough contiguous available memory.");
		return false;
	}
	return true;
}

DWORD ReadCPUSpeed()
{
	DWORD BufSize = sizeof(DWORD);
	DWORD dwMHz = 0;
	DWORD type = REG_DWORD;
	HKEY hKey;

	long lError = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
		0, KEY_READ, &hKey);

	if (lError == ERROR_SUCCESS)
	{
		RegQueryValueEx(hKey, L"~MHz", NULL, &type, (LPBYTE)&dwMHz, &BufSize);
	}
	return dwMHz;
}

GameOptions::GameOptions()
	: m_Level(), m_Renderer("Direct3D 11"), m_IsRunFullSpeed(false), m_ScreenWidth(1024), m_ScreenHeight(720),
	m_SoundEffectsVolume(1.0f), m_MusicVolume(1.0f), m_ExpectedPlayers(1), m_ListenPort(-1), m_GameHost(),
	m_NumAIs(1), m_MaxAIs(4), m_MaxPlayers(4), m_IsDevelopmentDirectories(false), m_pDoc(nullptr)
{
}


void GameOptions::Init(const std::string& xmlFileName, LPWSTR lpCmdLine)
{
	m_pDoc = new TiXmlDocument(xmlFileName.c_str());
	if (m_pDoc && m_pDoc->LoadFile())
	{
		TiXmlElement *pRoot = m_pDoc->RootElement();
		if (!pRoot)
			return;

		TiXmlElement* pNode = NULL;
		pNode = pRoot->FirstChildElement("Graphics");
		if (pNode)
		{
			std::string attribute;
			attribute = pNode->Attribute("renderer");
			if (attribute != "Direct3D 9" && attribute != "Direct3D 11")
			{
				GCC_ASSERT(0 && "Bad Renderer setting in Graphics options.");
			}
			else
			{
				m_Renderer = attribute;
			}

			if (pNode->Attribute("width"))
			{
				m_ScreenWidth = atoi(pNode->Attribute("width"));
				if (m_ScreenWidth < 800) m_ScreenWidth = 800;
			}

			if (pNode->Attribute("height"))
			{
				m_ScreenHeight = atoi(pNode->Attribute("height"));
				if (m_ScreenHeight < 600) m_ScreenHeight = 600;
			}

			if (pNode->Attribute("runfullspeed"))
			{
				attribute = pNode->Attribute("runfullspeed");
				m_IsRunFullSpeed = (attribute == "yes") ? true : false;
			}
		}

		pNode = pRoot->FirstChildElement("Sound");
		if (pNode)
		{
			m_MusicVolume = atoi(pNode->Attribute("musicVolume")) / 100.0f;
			m_SoundEffectsVolume = atoi(pNode->Attribute("sfxVolume")) / 100.0f;
		}

		pNode = pRoot->FirstChildElement("Multiplayer");
		if (pNode)
		{
			m_ExpectedPlayers = atoi(pNode->Attribute("expectedPlayers"));
			m_NumAIs = atoi(pNode->Attribute("numAIs"));
			m_MaxAIs = atoi(pNode->Attribute("maxAIs"));
			m_MaxPlayers = atoi(pNode->Attribute("maxPlayers"));

			m_ListenPort = atoi(pNode->Attribute("listenPort"));
			m_GameHost = pNode->Attribute("gameHost");
		}

		pNode = pRoot->FirstChildElement("ResCache");
		if (pNode)
		{
			std::string attribute(pNode->Attribute("useDevelopmentDirectories"));
			m_IsDevelopmentDirectories = ((attribute == "yes") ? (true) : (false));
		}
	}
}

bool IsOnlyInstance(LPCTSTR gameTitle)
{
	HANDLE handle = CreateMutex(NULL, TRUE, gameTitle);

	if (GetLastError() != ERROR_SUCCESS)
	{
		HWND hWnd = FindWindow(gameTitle, NULL);
		if (hWnd)
		{
			ShowWindow(hWnd, SW_SHOWNORMAL);
			SetFocus(hWnd);
			SetForegroundWindow(hWnd);
			SetActiveWindow(hWnd);
			return false;
		}
	}
	return true;
}

const TCHAR *GetSaveGameDirectory(HWND hWnd, const TCHAR *gameAppDirectory)
{
	HRESULT hr;
	static TCHAR m_SaveGameDirectory[MAX_PATH];
	TCHAR userDataPath[MAX_PATH];

	hr = SHGetSpecialFolderPath(hWnd, userDataPath, CSIDL_APPDATA, true);

	_tcscpy_s(m_SaveGameDirectory, userDataPath);
	_tcscat_s(m_SaveGameDirectory, _T("\\"));
	_tcscat_s(m_SaveGameDirectory, gameAppDirectory);

	if (0xffffffff == GetFileAttributes(m_SaveGameDirectory))
	{
		if (SHCreateDirectoryEx(hWnd, m_SaveGameDirectory, NULL) != ERROR_SUCCESS)
			return false;
	}

	_tcscat_s(m_SaveGameDirectory, _T("\\"));

	return m_SaveGameDirectory;
}
