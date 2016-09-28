#pragma once
#include "../RenderEngineBase.h"
#include "../RenderEngineInterface.h"
#include "EventManager.h"

const uint32_t EVENTMANAGER_NUM_QUEUES = 2;

class EventManager : public IEventManager
{
public:
	explicit EventManager(const char* name, bool setAsGlobal);
	virtual ~EventManager();

	virtual bool VAddListener(const EventListenerDelegate& eventDelegate, EventType type) override;
	virtual bool VRemoveListener(const EventListenerDelegate& eventDelegate, EventType type) override;
	
	virtual bool VTriggerEvent(const IEventDataPtr& pEvent) const override;
	virtual bool VQueueEvent(const IEventDataPtr& pEvent) override;
	virtual bool VThreadSafeQueueEvent(const IEventDataPtr& pEvent) override;
	virtual bool VAbortEvent(EventType type, bool allOfType = false) override;

	virtual bool VUpdate(uint32_t maxMillis = kINFINITE) override;

private:
	typedef std::list<boost::signals2::connection> EventConnectionList;
	typedef std::map<void*, boost::signals2::connection> EventConnectionMap;
	typedef std::map<EventType, EventListenerSignal> EventSingalMap;
	typedef std::list<IEventDataPtr> EventQueue;

	EventSingalMap m_EventSignals;
	EventConnectionMap m_EventConnections;
	EventQueue m_EventQueues[EVENTMANAGER_NUM_QUEUES];
	int m_ActiveQueue;
	ThreadSafeEventQueue m_RealtimeEventQueue;
};
