#include "Utility.h"
#include <Shlwapi.h>
#include <shlobj.h>
#include <direct.h>
#include <fstream>

bool Utility::AnsiToWideCch(WCHAR* dest, const CHAR* src, int charCount)
{
	if (dest == nullptr || src == nullptr || charCount < 1)
		return false;

	int nResult = MultiByteToWideChar(CP_ACP, 0, src, -1, dest, charCount);
	dest[charCount - 1] = 0;

	if (nResult == 0)
		return false;
	return true;
}

bool Utility::WideToAnsiCch(CHAR* dest, const WCHAR* src, int charCount)
{
	if (dest == nullptr || src == nullptr || charCount < 1)
		return false;

	int nResult = WideCharToMultiByte(CP_ACP, 0, src, -1, dest, charCount * sizeof(CHAR), nullptr, nullptr);
	dest[charCount - 1] = 0;

	if (nResult == 0)
		return false;
	return true;
}

std::string Utility::WS2S(const std::wstring& s)
{
	int slength = (int)s.length() + 1;
	int len = WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, 0, 0, 0, 0) - 1;
	std::string r(len, '\0');
	WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, &r[0], len, 0, 0);
	return r;
}

std::wstring Utility::S2WS(const std::string &s)
{
	int slength = (int)s.length() + 1;
	int len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0) - 1;
	std::wstring r(len, '\0');
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, &r[0], len);
	return r;
}

bool Utility::WildcardMatch(const char *pat, const char *str)
{
	int i, star;

new_segment:

	star = 0;
	if (*pat == '*')
	{
		star = 1;
		do { pat++; } while (*pat == '*');
	}

test_match:

	for (i = 0; pat[i] && (pat[i] != '*'); i++)
	{
		if (str[i] != pat[i])
		{
			if (!str[i]) return 0;
			if ((pat[i] == '?') && (str[i] != '.')) continue;
			if (!star) return 0;
			str++;
			goto test_match;
		}
	}
	if (pat[i] == '*')
	{
		str += i;
		pat += i;
		goto new_segment;
	}
	if (!str[i]) return 1;
	if (i && pat[i - 1] == '*') return 1;
	if (!star) return 0;
	str++;
	goto test_match;
}

std::wstring Utility::GetExecutableDirectory()
{
	WCHAR buffer[MAX_PATH];
	GetModuleFileName(nullptr, buffer, MAX_PATH);
	PathRemoveFileSpec(buffer);

	return std::wstring(buffer);
}

bool Utility::CheckStorage(DWORDLONG diskSpaceNeeded)
{
	int const drive = _getdrive();
	struct _diskfree_t diskfree;

	_getdiskfree(drive, &diskfree);

	unsigned __int64 const neededClusters =
		diskSpaceNeeded / (diskfree.sectors_per_cluster * diskfree.bytes_per_sector);

	if (diskfree.avail_clusters < neededClusters)
	{
		DEBUG_ERROR("CheckStorage Failure: Not enough physical storage.");
		return false;
	}
	return true;
}

DWORD Utility::ReadCPUSpeed()
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

bool Utility::CheckMemory(DWORDLONG physicalRAMNeeded, DWORDLONG virtualRAMNeeded)
{
	MEMORYSTATUSEX status;
	GlobalMemoryStatusEx(&status);
	if (status.ullTotalPhys < physicalRAMNeeded)
	{
		DEBUG_ERROR("CheckMemory Failure: Not enough physical memory.");
		return false;
	}

	if (status.ullAvailVirtual < virtualRAMNeeded)
	{
		DEBUG_ERROR("CheckMemory Failure: Not enough virtual memory.");
		return false;
	}

	char *buff = DEBUG_NEW char[(uint32_t)virtualRAMNeeded];
	if (buff)
		delete[] buff;
	else
	{
		DEBUG_ERROR("CheckMemory Failure: Not enough contiguous available memory.");
		return false;
	}
	return true;
}

bool Utility::IsOnlyInstance(LPCTSTR gameTitle)
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

const TCHAR * Utility::GetSaveGameDirectory(HWND hWnd, const TCHAR *gameAppDirectory)
{
	HRESULT hr;
	static TCHAR saveGameDirectory[MAX_PATH];
	TCHAR userDataPath[MAX_PATH];

	hr = SHGetSpecialFolderPath(hWnd, userDataPath, CSIDL_APPDATA, true);

	_tcscpy_s(saveGameDirectory, userDataPath);
	_tcscat_s(saveGameDirectory, _T("\\"));
	_tcscat_s(saveGameDirectory, gameAppDirectory);

	if (0xffffffff == GetFileAttributes(saveGameDirectory))
	{
		if (SHCreateDirectoryEx(hWnd, saveGameDirectory, NULL) != ERROR_SUCCESS)
			return false;
	}

	_tcscat_s(saveGameDirectory, _T("\\"));

	return saveGameDirectory;
}

std::string Utility::GetFileName(const std::string& filePath)
{
	std::string fullPath(filePath);
	std::replace(fullPath.begin(), fullPath.end(), '\\', '/');

	std::string::size_type lastSlashIndex = fullPath.find_last_of('/');

	if (lastSlashIndex == std::string::npos)
	{
		return fullPath;
	}
	else
	{
		return fullPath.substr(lastSlashIndex + 1, fullPath.size() - lastSlashIndex - 1);
	}
}

bool Utility::WriteFileData(const std::string& filePath, const char* fileData, uint32_t fileSize)
{
	std::ofstream file(filePath.c_str(), std::ios::out | std::ios::binary);
	if (file.bad())
	{
		return false;
	}

	file.write(fileData, fileSize);
	file.close();
}
