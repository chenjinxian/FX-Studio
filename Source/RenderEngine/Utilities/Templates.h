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
