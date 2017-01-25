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

template <class BaseType, class SubType>
BaseType* GenericObjectCreationFunction(void) { return DEBUG_NEW SubType; }

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
			m_creationFunctions[id] = &GenericObjectCreationFunction<BaseClass, SubClass>;
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

