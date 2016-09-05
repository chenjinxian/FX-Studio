#pragma once

template <class T>
class singleton
{
	T m_OldValue;
	T* m_pGlobalValue;

public:
	singleton(T newValue, T* globalValue)
	{ 
		m_pGlobalValue = globalValue;
		m_OldValue = *globalValue; 
		*m_pGlobalValue = newValue;
	}

	virtual ~singleton() { *m_pGlobalValue = m_OldValue; }
};

template <class Type>
shared_ptr<Type> MakeStrongPtr(weak_ptr<Type> pWeakPtr)
{
	if (!pWeakPtr.expired())
		return shared_ptr<Type>(pWeakPtr);
	else
		return shared_ptr<Type>();
}

#include <new>

class optional_empty { };

template <unsigned long size>
class optional_base
{
public:
	// Default - invalid.

	optional_base() : m_bValid(false) { }

	optional_base & operator = (optional_base const & t)
	{
		m_bValid = t.m_bValid;
		return * this;
	}

	//Copy constructor
	optional_base(optional_base const & other)
		: m_bValid(other.m_bValid)  { }

	//utility functions
	bool const valid() const		{ return m_bValid; }
	bool const invalid() const		{ return !m_bValid; }

protected:
	bool m_bValid;
	char m_data[size];  // storage space for T
};

template <class T>
class optional : public optional_base<sizeof(T)>
{
public:
	// Default - invalid.

	optional()	 {    }
	optional(T const & t)  { construct(t); m_bValid = (true);  }
	optional(optional_empty const &) {	}

	optional & operator = (T const & t)
	{
		if (m_bValid)
		{
			* GetT() = t;
		}
		else
		{
			construct(t);
			m_bValid = true;	// order important for exception safety.
		}

		return * this;
	}

	//Copy constructor
	optional(optional const & other)
	{
		if (other.m_bValid)
		{
			construct(* other);
			m_bValid = true;	// order important for exception safety.
		}
	}

	optional & operator = (optional const & other)
	{
		GCC_ASSERT(! (this == & other));	// don't copy over self!
		if (m_bValid)
		{						// first, have to destroy our original.
			m_bValid = false;	// for exception safety if destroy() throws.
								// (big trouble if destroy() throws, though)
			destroy();
		}

		if (other.m_bValid)
		{
			construct(* other);
			m_bValid = true;	// order vital.

		}
		return * this;
	}


	bool const operator == (optional const & other) const
	{
		if ((! valid()) && (! other.valid())) { return true; }
		if (valid() ^ other.valid()) { return false; }
		return ((* * this) == (* other));
	}

	bool const operator < (optional const & other) const
	{
		// equally invalid - not smaller.
		if ((! valid()) && (! other.valid()))   { return false; }

		// I'm not valid, other must be, smaller.
		if (! valid())	{ return true; }

		// I'm valid, other is not valid, I'm larger
		if (! other.valid()) { return false; }

		return ((* * this) < (* other));
	}

	~optional() { if (m_bValid) destroy(); }

	// Accessors.

	T const & operator * () const			{ GCC_ASSERT(m_bValid); return * GetT(); }
	T & operator * ()						{ GCC_ASSERT(m_bValid); return * GetT(); }
	T const * const operator -> () const	{ GCC_ASSERT(m_bValid); return GetT(); }
	T		* const operator -> ()			{ GCC_ASSERT(m_bValid); return GetT(); }

	//This clears the value of this optional variable and makes it invalid once again.
	void clear()
	{
		if (m_bValid)
		{
			m_bValid = false;
			destroy();
		}
	}

	//utility functions
	bool const valid() const		{ return m_bValid; }
	bool const invalid() const		{ return !m_bValid; }

private:


	T const * const GetT() const { return reinterpret_cast<T const * const>(m_data); }
	T * const GetT()			 { return reinterpret_cast<T * const>(m_data);}
	void construct(T const & t)  { new (GetT()) T(t); }
	void destroy() { GetT()->~T(); }
};


template <class BaseType, class SubType>
BaseType* GenericObjectCreationFunction(void) { return new SubType; }

template <class BaseClass, class IdType>
class GenericObjectFactory
{
	typedef BaseClass* (*ObjectCreationFunction)(void);
	std::map<IdType, ObjectCreationFunction> m_creationFunctions;

public:
	template <class SubClass>
	bool Register(IdType id)
	{
		auto findIt = m_creationFunctions.find(id);
		if (findIt == m_creationFunctions.end())
		{
			m_creationFunctions.insert(std::pair<IdType, ObjectCreationFunction>(id, &GenericObjectCreationFunction<BaseClass, SubClass>));
//             m_creationFunctions[id] = &GenericObjectCreationFunction<BaseClass, SubClass>;  // insert() is giving me compiler errors
			return true;
		}

		return false;
	}

	BaseClass* Create(IdType id)
	{
		auto findIt = m_creationFunctions.find(id);
		if (findIt != m_creationFunctions.end())
		{
			ObjectCreationFunction pFunc = findIt->second;
			return pFunc();
		}

		return NULL;
	}
};

