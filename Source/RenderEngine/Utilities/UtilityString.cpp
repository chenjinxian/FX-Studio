#include "UtilityString.h"
#include <Shlwapi.h>
#include <fstream>
using std::string;

std::string ws2s(const std::wstring& s)
{
	int slength = (int)s.length() + 1;
	int len = WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, 0, 0, 0, 0) - 1;
	std::string r(len, '\0');
	WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, &r[0], len, 0, 0);
	return r;
}


std::wstring s2ws(const std::string &s)
{
	int slength = (int)s.length() + 1;
	int len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0) - 1;
	std::wstring r(len, '\0');
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, &r[0], len);
	return r;
}

extern BOOL WildcardMatch(const char *pat, const char *str)
{
	int i, star;

new_segment:

	star = 0;
	if (*pat == '*') {
		star = 1;
		do { pat++; } while (*pat == '*'); /* enddo */
	} /* endif */

test_match:

	for (i = 0; pat[i] && (pat[i] != '*'); i++) {
		//if (mapCaseTable[str[i]] != mapCaseTable[pat[i]]) {
		if (str[i] != pat[i]) {
			if (!str[i]) return 0;
			if ((pat[i] == '?') && (str[i] != '.')) continue;
			if (!star) return 0;
			str++;
			goto test_match;
		}
	}
	if (pat[i] == '*') {
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

HRESULT AnsiToWideCch(WCHAR* wstrDestination, const CHAR* strSource, int cchDestChar)
{
	if (wstrDestination == NULL || strSource == NULL || cchDestChar < 1)
		return E_INVALIDARG;

	int nResult = MultiByteToWideChar(CP_ACP, 0, strSource, -1,
		wstrDestination, cchDestChar);
	wstrDestination[cchDestChar - 1] = 0;

	if (nResult == 0)
		return E_FAIL;
	return S_OK;
}

HRESULT WideToAnsiCch(CHAR* strDestination, const WCHAR* wstrSource, int cchDestChar)
{
	if (strDestination == NULL || wstrSource == NULL || cchDestChar < 1)
		return E_INVALIDARG;

	int nResult = WideCharToMultiByte(CP_ACP, 0, wstrSource, -1, strDestination,
		cchDestChar*sizeof(CHAR), NULL, NULL);
	strDestination[cchDestChar - 1] = 0;

	if (nResult == 0)
		return E_FAIL;
	return S_OK;
}

HRESULT GenericToAnsiCch(CHAR* strDestination, const TCHAR* tstrSource, int cchDestChar)
{
	if (strDestination == NULL || tstrSource == NULL || cchDestChar < 1)
		return E_INVALIDARG;

#ifdef _UNICODE
	return WideToAnsiCch(strDestination, tstrSource, cchDestChar);
#else
	strncpy(strDestination, tstrSource, cchDestChar);
	strDestination[cchDestChar - 1] = '\0';
	return S_OK;
#endif   
}

HRESULT GenericToWideCch(WCHAR* wstrDestination, const TCHAR* tstrSource, int cchDestChar)
{
	if (wstrDestination == NULL || tstrSource == NULL || cchDestChar < 1)
		return E_INVALIDARG;

#ifdef _UNICODE
	wcsncpy(wstrDestination, tstrSource, cchDestChar);
	wstrDestination[cchDestChar - 1] = L'\0';
	return S_OK;
#else
	return AnsiToWideCch(wstrDestination, tstrSource, cchDestChar);
#endif    
}

HRESULT AnsiToGenericCch(TCHAR* tstrDestination, const CHAR* strSource, int cchDestChar)
{
	if (tstrDestination == NULL || strSource == NULL || cchDestChar < 1)
		return E_INVALIDARG;

#ifdef _UNICODE
	return AnsiToWideCch(tstrDestination, strSource, cchDestChar);
#else
	strncpy(tstrDestination, strSource, cchDestChar);
	tstrDestination[cchDestChar - 1] = '\0';
	return S_OK;
#endif    
}

HRESULT WideToGenericCch(TCHAR* tstrDestination, const WCHAR* wstrSource, int cchDestChar)
{
	if (tstrDestination == NULL || wstrSource == NULL || cchDestChar < 1)
		return E_INVALIDARG;

#ifdef _UNICODE
	wcsncpy(tstrDestination, wstrSource, cchDestChar);
	tstrDestination[cchDestChar - 1] = L'\0';
	return S_OK;
#else
	return WideToAnsiCch(tstrDestination, wstrSource, cchDestChar);
#endif
}

string ToStr(int num, int base)
{
	char str[MAX_DIGITS_IN_INT];
	memset(str, 0, MAX_DIGITS_IN_INT);
	_itoa_s(num, str, MAX_DIGITS_IN_INT, base);
	return (string(str));
}  // end ToStr()

string ToStr(unsigned int num, int base)
{
	char str[MAX_DIGITS_IN_INT];
	memset(str, 0, MAX_DIGITS_IN_INT);
	_ultoa_s((unsigned long)num, str, MAX_DIGITS_IN_INT, base);
	return (string(str));
}

string ToStr(unsigned long num, int base)
{
	char str[MAX_DIGITS_IN_INT];
	memset(str, 0, MAX_DIGITS_IN_INT);
	_ultoa_s(num, str, MAX_DIGITS_IN_INT, base);
	return (string(str));
}

string ToStr(float num)
{
	char str[64];  // I'm sure this is overkill
	memset(str, 0, 64);
	_sprintf_p(str, 64, "%f", num);
	return (string(str));
}

string ToStr(double num)
{
	char str[64];  // I'm sure this is overkill
	memset(str, 0, 64);
	_sprintf_p(str, 64, "%fL", num);
	return (string(str));
}

string ToStr(bool val)
{
	return (string((val == true ? "true" : "false")));
}

string ToStr(const Vector3& vec)
{
	return string("(" + ToStr(vec.x) + "," + ToStr(vec.y) + "," + ToStr(vec.z) + ")");
}

std::string Utility::CurrentDirectory()
{
	WCHAR buffer[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, buffer);
	std::wstring currentDirectoryW(buffer);

	return std::string(currentDirectoryW.begin(), currentDirectoryW.end());
}

std::wstring Utility::ExecutableDirectory()
{
	WCHAR buffer[MAX_PATH];
	GetModuleFileName(nullptr, buffer, MAX_PATH);
	PathRemoveFileSpec(buffer);

	return std::wstring(buffer);
}

void Utility::GetFileName(const std::string& inputPath, std::string& filename)
{
	std::string fullPath(inputPath);
	std::replace(fullPath.begin(), fullPath.end(), '\\', '/');

	std::string::size_type lastSlashIndex = fullPath.find_last_of('/');

	if (lastSlashIndex == std::string::npos)
	{
		filename = fullPath;
	}
	else
	{
		filename = fullPath.substr(lastSlashIndex + 1, fullPath.size() - lastSlashIndex - 1);
	}
}

void Utility::GetDirectory(const std::string& inputPath, std::string& directory)
{
	std::string fullPath(inputPath);
	std::replace(fullPath.begin(), fullPath.end(), '\\', '/');

	std::string::size_type lastSlashIndex = fullPath.find_last_of('/');

	if (lastSlashIndex == std::string::npos)
	{
		directory = "";
	}
	else
	{
		directory = fullPath.substr(0, lastSlashIndex);
	}
}

void Utility::GetFileNameAndDirectory(const std::string& inputPath, std::string& directory, std::string& filename)
{
	std::string fullPath(inputPath);
	std::replace(fullPath.begin(), fullPath.end(), '\\', '/');

	std::string::size_type lastSlashIndex = fullPath.find_last_of('/');

	if (lastSlashIndex == std::string::npos)
	{
		directory = "";
		filename = fullPath;
	}
	else
	{
		directory = fullPath.substr(0, lastSlashIndex);
		filename = fullPath.substr(lastSlashIndex + 1, fullPath.size() - lastSlashIndex - 1);
	}
}

void Utility::LoadBinaryFile(const std::wstring& filename, std::vector<char>& data)
{
	std::ifstream file(filename.c_str(), std::ios::binary);
	if (file.bad())
	{
		throw std::exception("Could not open file.");
	}

	file.seekg(0, std::ios::end);
	UINT size = (UINT)file.tellg();

	if (size > 0)
	{
		data.resize(size);
		file.seekg(0, std::ios::beg);
		file.read(&data.front(), size);
	}

	file.close();
}

void Utility::PathJoin(std::wstring& dest, const std::wstring& sourceDirectory, const std::wstring& sourceFile)
{
	WCHAR buffer[MAX_PATH];

	PathCombine(buffer, sourceDirectory.c_str(), sourceFile.c_str());
	dest = buffer;
}

void Utility::GetPathExtension(const std::wstring& source, std::wstring& dest)
{
	dest = PathFindExtension(source.c_str());
}

void Utility::Clamp(long& value, long lower, long upper)
{
	if (value <= lower)
	{
		value = lower;
	}
	else if (value >= upper)
	{
		value = upper;
	}
	else
	{
	}
}

void* HashedString::hash_name(char const * pIdentStr)
{
	// Relatively simple hash of arbitrary text string into a
	// 32-bit identifier Output value is
	// input-valid-deterministic, but no guarantees are made
	// about the uniqueness of the output per-input
	//
	// Input value is treated as lower-case to cut down on false
	// separations cause by human mistypes. Sure, it could be
	// construed as a programming error to mix up your cases, and
	// it cuts down on permutations, but in Real World Usage
	// making this text case-sensitive will likely just lead to
	// Pain and Suffering.
	//
	// This code lossely based upon the adler32 checksum by Mark
	// Adler and published as part of the zlib compression
	// library sources.

	// largest prime smaller than 65536
	unsigned long BASE = 65521L;

	// NMAX is the largest n such that 255n(n+1)/2 +
	// (n+1)(BASE-1) <= 2^32-1
	unsigned long NMAX = 5552;

#define DO1(buf,i)  {s1 += tolower(buf[i]); s2 += s1;}
#define DO2(buf,i)  DO1(buf,i); DO1(buf,i+1);
#define DO4(buf,i)  DO2(buf,i); DO2(buf,i+2);
#define DO8(buf,i)  DO4(buf,i); DO4(buf,i+4);
#define DO16(buf)   DO8(buf,0); DO8(buf,8);

	if (pIdentStr == NULL)
		return NULL;

	unsigned long s1 = 0;
	unsigned long s2 = 0;

	for (size_t len = strlen(pIdentStr); len > 0; )
	{
		unsigned long k = len < NMAX ? len : NMAX;

		len -= k;

		while (k >= 16)
		{
			DO16(pIdentStr);
			pIdentStr += 16;
			k -= 16;
		}

		if (k != 0) do
		{
			s1 += tolower(*pIdentStr++);
			s2 += s1;
		} while (--k);

		s1 %= BASE;
		s2 %= BASE;
	}

#pragma warning(push)
#pragma warning(disable : 4312)

	return reinterpret_cast<void *>((s2 << 16) | s1);

#pragma warning(pop)
#undef DO1
#undef DO2
#undef DO4
#undef DO8
#undef DO16
}