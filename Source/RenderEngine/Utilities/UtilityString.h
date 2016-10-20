#pragma once
#include "../RenderEngineBase.h"
#include "../RenderEngineInterface.h"

#define MAX_DIGITS_IN_INT 12

extern BOOL WildcardMatch(const char *pat, const char *str);

extern HRESULT AnsiToWideCch(WCHAR* dest, const CHAR* src, int charCount);
extern HRESULT WideToAnsiCch(CHAR* dest, const WCHAR* src, int charCount);
extern HRESULT GenericToAnsiCch(CHAR* dest, const TCHAR* src, int charCount);
extern HRESULT GenericToWideCch(WCHAR* dest, const TCHAR* src, int charCount);
extern HRESULT AnsiToGenericCch(TCHAR* dest, const CHAR* src, int charCount);
extern HRESULT WideToGenericCch(TCHAR* dest, const WCHAR* src, int charCount);

extern std::string ToStr(int num, int base = 10);
extern std::string ToStr(unsigned int num, int base = 10);
extern std::string ToStr(unsigned long num, int base = 10);
extern std::string ToStr(float num);
extern std::string ToStr(double num);
extern std::string ToStr(bool val);
extern std::string ToStr(const Vector3& vec);

extern std::string ws2s(const std::wstring& s);
extern std::wstring s2ws(const std::string &s);

class Utility : public boost::noncopyable
{
public:
	static std::string CurrentDirectory();
	static std::wstring ExecutableDirectory();
	static void GetFileName(const std::string& inputPath, std::string& filename);
	static void GetDirectory(const std::string& inputPath, std::string& directory);
	static void GetFileNameAndDirectory(const std::string& inputPath, std::string& directory, std::string& filename);
	static void LoadBinaryFile(const std::wstring& filename, std::vector<char>& data);
	static void PathJoin(std::wstring& dest, const std::wstring& sourceDirectory, const std::wstring& sourceFile);
	static void GetPathExtension(const std::wstring& source, std::wstring& dest);

	static void Clamp(long& value, long lower, long upper);
};


#pragma warning(push)
#pragma warning(disable : 4311)

// A hashed string.  It retains the initial (ANSI) string in addition to the hash value for easy reference.

class HashedString
{
public:
	explicit HashedString(char const * const pIdentString)
		: m_ident(hash_name(pIdentString))
		, m_identStr(pIdentString)
	{
	}

	unsigned long getHashValue(void) const
	{

		return reinterpret_cast<unsigned long>(m_ident);
	}

	const std::string & getStr() const
	{
		return m_identStr;
	}

	static void * hash_name(char const *  pIdentStr);

	bool operator< (HashedString const & o) const
	{
		bool r = (getHashValue() < o.getHashValue());
		return r;
	}

	bool operator== (HashedString const & o) const
	{
		bool r = (getHashValue() == o.getHashValue());
		return r;
	}

private:

	// note: m_ident is stored as a void* not an int, so that in
	// the debugger it will show up as hex-values instead of
	// integer values. This is a bit more representative of what
	// we're doing here and makes it easy to allow external code
	// to assign event types as desired.

	void *             m_ident;
	std::string		   m_identStr;
};
//Remove the warning for warning #4311...
#pragma warning(pop)
