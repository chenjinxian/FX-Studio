#pragma once

#define MAX_DIGITS_IN_INT 12  // max number of digits in an int (-2147483647 = 11 digits, +1 for the '\0')
typedef std::vector<std::string> StringVec;

// Removes characters up to the first '\n'
extern void RemoveFirstLine(std::wstring &src, std::wstring &result);

// Removes leading white space
extern void TrimLeft(std::wstring &s);

// Counts the number of lines in a block of text
extern int CountLines(const std::wstring &s);

// Does a classic * & ? pattern match on a file name - this is case sensitive!
extern BOOL WildcardMatch(const char *pat, const char *str);

// converts a regular string to a wide string
extern void StringToWideString(const std::string& source, std::wstring& outDest);

extern HRESULT AnsiToWideCch( WCHAR* dest, const CHAR* src, int charCount);  
extern HRESULT WideToAnsiCch( CHAR* dest, const WCHAR* src, int charCount);  
extern HRESULT GenericToAnsiCch( CHAR* dest, const TCHAR* src, int charCount); 
extern HRESULT GenericToWideCch( WCHAR* dest, const TCHAR* src, int charCount); 
extern HRESULT AnsiToGenericCch( TCHAR* dest, const CHAR* src, int charCount); 
extern HRESULT WideToGenericCch( TCHAR* dest, const WCHAR* src, int charCount);

extern std::string ToStr(int num, int base = 10);
extern std::string ToStr(unsigned int num, int base = 10);
extern std::string ToStr(unsigned long num, int base = 10);
extern std::string ToStr(float num);
extern std::string ToStr(double num);
extern std::string ToStr(bool val);
extern std::string ToStr(const Vector3& vec);

extern std::string ws2s(const std::wstring& s);
extern std::wstring s2ws(const std::string &s);

// Splits a string by the delimeter into a vector of strings.  For example, say you have the following string:
// std::string test("one,two,three");
// You could call Split() like this:
// Split(test, outVec, ',');
// outVec will have the following values:
// "one", "two", "three"
void Split(const std::string& str, StringVec& vec, char delimiter);


#pragma warning(push)
#pragma warning(disable : 4311)

// A hashed string.  It retains the initial (ANSI) string in addition to the hash value for easy reference.

// class HashedString				- Chapter 10, page 274

class HashedString
{
public:
	explicit HashedString( char const * const pIdentString )
		: m_ident( hash_name( pIdentString ) )
	, m_identStr( pIdentString )
	{
	}

	unsigned long getHashValue( void ) const
	{

		return reinterpret_cast<unsigned long>( m_ident );
	}

	const std::string & getStr() const
	{
		return m_identStr;
	}

	static
	void * hash_name( char const *  pIdentStr );

	bool operator< ( HashedString const & o ) const
	{
		bool r = ( getHashValue() < o.getHashValue() );
		return r;
	}

	bool operator== ( HashedString const & o ) const
	{
		bool r = ( getHashValue() == o.getHashValue() );
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
