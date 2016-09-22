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
	return true;
}

bool EventManager::VRemoveListener(const EventListenerDelegate& eventDelegate, EventType type)
{
	return true;
}

bool EventManager::VTriggerEvent(const IEventDataPtr& pEvent) const
{
	return true;
}

bool EventManager::VQueueEvent(const IEventDataPtr& pEvent)
{
	return true;
}

bool EventManager::VThreadSafeQueueEvent(const IEventDataPtr& pEvent)
{
	return true;
}

bool EventManager::VAbortEvent(EventType type, bool allOfType /*= false*/)
{
	return true;
}

bool EventManager::VUpdate(uint32_t maxMillis /*= kINFINITE*/)
{
	return true;
}
