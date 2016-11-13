#pragma once
#include "../TinyEngineBase.h"

class Utility : public boost::noncopyable
{
public:
	static std::string WS2S(const std::wstring& s);
	static std::wstring S2WS(const std::string &s);

	static bool WildcardMatch(const char *pat, const char *str);

	static std::wstring GetExecutableDirectory();

private:
	Utility();
};
