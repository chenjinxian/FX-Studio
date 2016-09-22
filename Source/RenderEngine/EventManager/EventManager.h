#pragma once
#include "../RenderEngineBase.h"
#include "../RenderEngineInterface.h"
#include "../Utilities/ConcurrentQueue.h"
#include "../Utilities/Templates.h"
#include "boost/signals2/signal.hpp"
#include <strstream>

class IEventData;

typedef uint32_t EventType;
typedef shared_ptr<IEventData> IEventDataPtr;
typedef boost::signals2::signal<void(IEventDataPtr)> EventListenerSignal;
typedef EventListenerSignal::slot_type EventListenerDelegate;
typedef concurrent_queue<IEventDataPtr> ThreadSafeEventQueue;

extern GenericObjectFactory<IEventData, EventType> g_eventFactory;
#define REGISTER_EVENT(eventClass) g_eventFactory.Register<eventClass>(eventClass::sk_EventType)
#define CRATE_EVENT(eventType) g_eventFactory.Create(eventType)

class IEventData
{
public:
	virtual ~IEventData() {}
	virtual float GetTimeStamp() const = 0;
	virtual void VSerialize(std::ostrstream& out) const = 0;
	virtual void VDeserialize(std::istrstream& in) = 0;
	virtual IEventDataPtr VCopy() const = 0;
	virtual const char* GetName() const = 0;
};

class BaseEventData : public IEventData
{
public:
	BaseEventData(float timeStamp = 0.0f) : m_TimeStamp(timeStamp) {}
	virtual ~BaseEventData() {}

	virtual EventType VGetEventType() const = 0;
	virtual float GetTimeStamp() const override { return m_TimeStamp; }
	virtual void VSerialize(std::ostrstream& out) const override {}
	virtual void VDeserialize(std::istrstream& in) override {}

private:
	float m_TimeStamp;
};

class IEventManager
{
public:
	enum { kINFINITE = 0xffffffff };

	explicit IEventManager(const char* name, bool setAsGlobal);
	~IEventManager();

	virtual bool VAddListener(const EventListenerDelegate& eventDelegate, EventType type) = 0;
	virtual bool VRemoveListener(const EventListenerDelegate& eventDelegate, EventType type) = 0;
	virtual bool VTriggerEvent(const IEventDataPtr& pEvent) const = 0;
	virtual bool VQueueEvent(const IEventDataPtr& pEvent) = 0;
	virtual bool VThreadSafeQueueEvent(const IEventDataPtr& pEvent) = 0;
	virtual bool VAbortEvent(EventType type, bool allOfType = false) = 0;
	virtual bool VUpdate(uint32_t maxMillis = kINFINITE) = 0;

	static IEventManager* Get();
};