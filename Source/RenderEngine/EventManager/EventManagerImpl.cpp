#include "EventManagerImpl.h"

EventManager::EventManager(const char* name, bool setAsGlobal)
	: IEventManager(nullptr, setAsGlobal), m_ActiveQueue(0)
{

}

EventManager::~EventManager()
{

}

bool EventManager::VAddListener(const EventListenerDelegate& eventDelegate, EventType type)
{
	void* ptr = eventDelegate.slot_function().functor.func_ptr;
	auto findIt = m_EventConnections.find(ptr);
	if (findIt != m_EventConnections.end())
	{
		findIt->second.connected();
		return true;
	}
	else
	{
		EventListenerSignal& eventSignal = m_EventSignals[type];
		m_EventConnections.insert(std::make_pair(ptr, eventSignal.connect(eventDelegate)));
		return true;
	}
}

bool EventManager::VRemoveListener(const EventListenerDelegate& eventDelegate, EventType type)
{
	auto findIt = m_EventConnections.find(eventDelegate.slot_function().functor.func_ptr);
	if (findIt != m_EventConnections.end())
	{
		findIt->second.disconnect();
		return true;
	}
	else
	{
		return false;
	}
}

bool EventManager::VTriggerEvent(const IEventDataPtr& pEvent) const
{
	if (pEvent == nullptr)
	{
		return false;
	}

	auto findIt = m_EventSignals.find(pEvent->VGetEventType());
	if (findIt != m_EventSignals.end())
	{
		findIt->second(pEvent);
		return true;
	}
	else
	{
		return false;
	}
}

bool EventManager::VQueueEvent(const IEventDataPtr& pEvent)
{
	if (pEvent == nullptr)
	{
		return false;
	}

	if (m_EventSignals.find(pEvent->VGetEventType()) != m_EventSignals.end())
	{
		m_EventQueues[m_ActiveQueue].push_back(pEvent);
		return true;
	}
	else
	{
		return false;
	}
}

bool EventManager::VThreadSafeQueueEvent(const IEventDataPtr& pEvent)
{
	m_RealtimeEventQueue.push(pEvent);
	return true;
}

bool EventManager::VAbortEvent(EventType type, bool allOfType /*= false*/)
{
	return true;
}

bool EventManager::VUpdate(uint32_t maxMillis /*= kINFINITE*/)
{
	uint32_t currMs = GetTickCount();
	uint32_t maxMs = ((maxMillis == IEventManager::kINFINITE) ? (IEventManager::kINFINITE) : (currMs + maxMillis));
	return true;
}
