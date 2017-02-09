#pragma once
#include "../TinyEngine/TinyEngine.h"

class EvtData_Move_Camera : public BaseEventData
{
public:
	EvtData_Move_Camera() : m_ActorId(INVALID_ACTOR_ID) {}
	explicit EvtData_Move_Camera(ActorId actorId)
		: m_ActorId(actorId)
	{

	}

	virtual void VDeserialize(std::istrstream& in)
	{
		in >> m_ActorId;
	}

	virtual void VSerialize(std::ostrstream& out)
	{
		out << m_ActorId << " ";
	}

	virtual EventType VGetEventType() const override
	{
		return sk_EventType;
	}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr(DEBUG_NEW EvtData_Move_Camera(m_ActorId));
	}

	virtual const char* VGetName() const
	{
		return "EvtData_Move_Camera";
	}

	ActorId GetActorId() const
	{
		return m_ActorId;
	}

	static const EventType sk_EventType;

private:
	ActorId m_ActorId;
};
