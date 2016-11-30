#include "EventManagerImpl.h"
#include "../Debugging/Logger.h"
#include "boost/lexical_cast.hpp"

EventManager::EventManager(const char* name, bool setAsGlobal)
	: IEventManager(nullptr, setAsGlobal), m_ActiveQueue(0)
{

}

EventManager::~EventManager()
{

}

bool EventManager::VAddListener(const EventListenerDelegate& eventDelegate, EventType type)
{
	DEBUG_LOG("Events", "Attempting to add delegate function for event type: " + boost::lexical_cast<std::string>(type));

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
		DEBUG_LOG("Events", "Successfully added delegate for event type: " + boost::lexical_cast<std::string>(type));

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
	DEBUG_ASSERT(m_ActiveQueue >= 0);
	DEBUG_ASSERT(m_ActiveQueue < EVENTMANAGER_NUM_QUEUES);

	bool success = false;
	auto findIt = m_EventSignals.find(type);
	if (findIt != m_EventSignals.end())
	{
		EventQueue& eventQueue = m_EventQueues[m_ActiveQueue];
		auto it = eventQueue.begin();
		while (it != eventQueue.end())
		{
			auto thisIt = it;
			++it;

			if ((*thisIt)->VGetEventType() == type)
			{
				eventQueue.erase(thisIt);
				success = true;
				if (!allOfType)
					break;
			}
		}
	}

	return success;
}

bool EventManager::VUpdate(uint32_t maxMillis /*= kINFINITE*/)
{
	uint32_t currMs = GetTickCount();
	uint32_t maxMs = ((maxMillis == IEventManager::kINFINITE) ? (IEventManager::kINFINITE) : (currMs + maxMillis));

	IEventDataPtr pRealtimeEvent;
	while (m_RealtimeEventQueue.try_pop(pRealtimeEvent))
	{
		VQueueEvent(pRealtimeEvent);

		currMs = GetTickCount();
		if (maxMillis != IEventManager::kINFINITE)
		{
			if (currMs >= maxMs)
			{
				DEBUG_ERROR("A realtime process is spamming the event manager!");
			}
		}
	}

	int queueToProcess = m_ActiveQueue;
	m_ActiveQueue = (m_ActiveQueue + 1) % EVENTMANAGER_NUM_QUEUES;
	m_EventQueues[m_ActiveQueue].clear();

// 	DEBUG_LOG("EventLoop", "Processing Event Queue " + boost::lexical_cast<std::string>(queueToProcess) + "; " + boost::lexical_cast<std::string>(m_EventQueues[queueToProcess].size()) + " events to process");

	while (!m_EventQueues[queueToProcess].empty())
	{
		IEventDataPtr pEvent = m_EventQueues[queueToProcess].front();
		m_EventQueues[queueToProcess].pop_front();
// 		DEBUG_LOG("EventLoop", "\t\tProcessing Event " + std::string(pEvent->VGetName()));

		auto findIt = m_EventSignals.find(pEvent->VGetEventType());
		if (findIt != m_EventSignals.end())
		{
// 			DEBUG_LOG("EventLoop", "\t\tSending event " + std::string(pEvent->VGetName()) + " to delegate");
			findIt->second(pEvent);
		}

		currMs = GetTickCount();
		if (maxMillis != IEventManager::kINFINITE && currMs >= maxMs)
		{
// 			DEBUG_LOG("EventLoop", "Aborting event processing; time ran out");
			break;
		}
	}

	bool queueFlushed = (m_EventQueues[queueToProcess].empty());
	if (!queueFlushed)
	{
		while (!m_EventQueues[queueToProcess].empty())
		{
			IEventDataPtr pEvent = m_EventQueues[queueToProcess].back();
			m_EventQueues[queueToProcess].pop_back();
			m_EventQueues[m_ActiveQueue].push_front(pEvent);
		}
	}

	return queueFlushed;
}
