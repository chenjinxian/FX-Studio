#pragma once
#include "../TinyEngineBase.h"

class Utility : public boost::noncopyable
{
public:
	static bool AnsiToWideCch(WCHAR* dest, const CHAR* src, int charCount);
	static bool WideToAnsiCch(CHAR* dest, const WCHAR* src, int charCount);
	static std::string WS2S(const std::wstring& s);
	static std::wstring S2WS(const std::string &s);
	static bool WildcardMatch(const char *pat, const char *str);

	static std::wstring GetExecutableDirectory();
	static std::string GetFileName(const std::string& filePath);
	static std::string GetDirectory(const std::string& filePath);
	static bool WriteFileData(const std::string& filePath, const char* fileData, uint32_t fileSize);

	static bool CheckStorage(DWORDLONG diskSpaceNeeded);
	static DWORD ReadCPUSpeed();
	static bool CheckMemory(DWORDLONG physicalRAMNeeded, DWORDLONG virtualRAMNeeded);
	static bool IsOnlyInstance(LPCTSTR gameTitle);
	static const TCHAR *GetSaveGameDirectory(HWND hWnd, const TCHAR *gameAppDirectory);

	static void QuaternionToAngle(const Quaternion& quat, float& yaw, float& pitch, float& roll);
};
