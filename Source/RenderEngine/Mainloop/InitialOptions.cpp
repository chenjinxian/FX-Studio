#include "InitialOptions.h"
#include <shlobj.h>
#include <direct.h>

InitialOptions::InitialOptions()
	: m_Level(),
	m_Renderer("Direct3D 11"),
	m_RunFullSpeed(false),
	m_ScreenWidth(1024),
	m_ScreenHeight(768),
	m_pDoc(nullptr)
{
}

InitialOptions::~InitialOptions()
{
	SAFE_DELETE(m_pDoc)
}

void InitialOptions::Init(const std::string& xmlFilePath)
{
	m_pDoc = new TiXmlDocument(xmlFilePath.c_str());
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
			}

			if (pNode->Attribute("height"))
			{
				m_ScreenHeight = atoi(pNode->Attribute("height"));
			}

			if (pNode->Attribute("runfullspeed"))
			{
				attribute = pNode->Attribute("runfullspeed");
				m_RunFullSpeed = (attribute == "yes") ? true : false;
			}
		}
	}
}

bool InitialOptions::CheckStorage(const DWORDLONG diskSpaceNeeded)
{
	int const drive = _getdrive();
	struct _diskfree_t diskfree;

	_getdiskfree(drive, &diskfree);
	unsigned __int64 const neededClusters = diskSpaceNeeded / (diskfree.sectors_per_cluster*diskfree.bytes_per_sector);
	if (diskfree.avail_clusters < neededClusters)
	{
		GCC_ERROR("CheckStorage Failure: Not enough physical storage.");
		return false;
	}
	return true;
}

DWORD InitialOptions::ReadCPUSpeed()
{
	DWORD BufSize = sizeof(DWORD);
	DWORD dwMHz = 0;
	DWORD type = REG_DWORD;
	HKEY hKey;

	// open the key where the proc speed is hidden:
	long lError = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
		0, KEY_READ, &hKey);

	if (lError == ERROR_SUCCESS)
	{
		RegQueryValueEx(hKey, L"~MHz", NULL, &type, (LPBYTE)&dwMHz, &BufSize);
	}
	return dwMHz;
}

bool InitialOptions::CheckMemory(const DWORDLONG physicalRAMNeeded, const DWORDLONG virtualRAMNeeded)
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

bool InitialOptions::IsOnlyInstance(const std::wstring& windowTitle)
{
	HANDLE handle = CreateMutex(NULL, TRUE, windowTitle.c_str());

	if (GetLastError() != ERROR_SUCCESS)
	{
		HWND hWnd = FindWindow(windowTitle.c_str(), NULL);
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

const TCHAR * InitialOptions::GetSaveGameDirectory(HWND hWnd, const TCHAR *gameAppDirectory)
{
	HRESULT hr;
	static TCHAR m_SaveGameDirectory[MAX_PATH];
	TCHAR userDataPath[MAX_PATH];

	hr = SHGetSpecialFolderPath(hWnd, userDataPath, CSIDL_APPDATA, true);

	_tcscpy_s(m_SaveGameDirectory, userDataPath);
	_tcscat_s(m_SaveGameDirectory, _T("\\"));
	_tcscat_s(m_SaveGameDirectory, gameAppDirectory);

	if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(m_SaveGameDirectory))
	{
		if (SHCreateDirectoryEx(hWnd, m_SaveGameDirectory, NULL) != ERROR_SUCCESS)
			return false;
	}

	_tcscat_s(m_SaveGameDirectory, _T("\\"));

	return m_SaveGameDirectory;
}

bool InitialOptions::CheckForJoystick(HWND hWnd)
{
	JOYINFO joyinfo;
	UINT wNumDevs;
	BOOL bDev1Attached, bDev2Attached;

	if ((wNumDevs = joyGetNumDevs()) == 0)
		return false;
	bDev1Attached = joyGetPos(JOYSTICKID1, &joyinfo) != JOYERR_UNPLUGGED;
	bDev2Attached = joyGetPos(JOYSTICKID2, &joyinfo) != JOYERR_UNPLUGGED;
	if (bDev1Attached)
		joySetCapture(hWnd, JOYSTICKID1, 1000 / 30, true);
	if (bDev2Attached)
		joySetCapture(hWnd, JOYSTICKID2, 1000 / 30, true);

	return true;
}
