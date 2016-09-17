#pragma once
#include "../RenderEngineBase.h"
#include "../RenderEngineInterface.h"

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
