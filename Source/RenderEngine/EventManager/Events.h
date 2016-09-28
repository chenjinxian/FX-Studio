#pragma once
#include "../RenderEngineBase.h"
#include "../RenderEngineInterface.h"
#include "EventManager.h"

class SceneNode;
class EvtData_New_Actor : public BaseEventData
{
public:
	EvtData_New_Actor() : m_ActorId(INVALID_ACTOR_ID), m_ViewId(INVALID_GAME_VIEW_ID) {}
	explicit EvtData_New_Actor(ActorId actorId, GameViewId viewId)
		: m_ActorId(actorId), m_ViewId(viewId)
	{

	}

	virtual void VDeserialize(std::istrstream& in)
	{
		in >> m_ActorId;
		in >> m_ViewId;
	}

	virtual void VSerialize(std::ostrstream& out)
	{
		out << m_ActorId << " ";
		out << m_ViewId << " ";
	}

	virtual EventType VGetEventType() const override
	{
		return sk_EventType;
	}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr(GCC_NEW EvtData_New_Actor(m_ActorId, m_ViewId));
	}

	virtual const char* VGetName() const
	{
		return "EvtData_New_Actor";
	}

	ActorId GetActorId() const
	{
		return m_ActorId;
	}

	GameViewId GetViewId() const
	{
		return m_ViewId;
	}

	static const EventType sk_EventType;

private:
	ActorId m_ActorId;
	GameViewId m_ViewId;
};

class EvtData_Destroy_Actor : public BaseEventData
{
public:
	explicit EvtData_Destroy_Actor(ActorId actorId = INVALID_ACTOR_ID)
		: m_ActorId(actorId)
	{

	}

	virtual void VDeserialize(std::istrstream& in)
	{
		in >> m_ActorId;
	}

	virtual void VSerialize(std::ostrstream& out)
	{
		out << m_ActorId;
	}

	virtual EventType VGetEventType() const override
	{
		return sk_EventType;
	}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr(GCC_NEW EvtData_Destroy_Actor(m_ActorId));
	}

	virtual const char* VGetName() const
	{
		return "EvtData_Destroy_Actor";
	}

	ActorId GetActorId() const
	{
		return m_ActorId;
	}

	static const EventType sk_EventType;

private:
	ActorId m_ActorId;
};

class EvtData_Move_Actor : public BaseEventData
{
public:
	EvtData_Move_Actor() : m_ActorId(INVALID_ACTOR_ID) {}
	explicit EvtData_Move_Actor(ActorId actorId, const Matrix& matrix)
		: m_ActorId(actorId), m_Matrix(matrix)
	{

	}

	virtual void VDeserialize(std::istrstream& in)
	{
		in >> m_ActorId;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				in >> m_Matrix.m[i][j];
			}
		}
	}

	virtual void VSerialize(std::ostrstream& out)
	{
		out << m_ActorId << " ";
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				out << m_Matrix.m[i][j] << " ";
			}
		}
	}

	virtual EventType VGetEventType() const override
	{
		return sk_EventType;
	}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr(GCC_NEW EvtData_Move_Actor(m_ActorId, m_Matrix));
	}

	virtual const char* VGetName() const
	{
		return "EvtData_Move_Actor";
	}

	ActorId GetActorId() const
	{
		return m_ActorId;
	}

	const Matrix& GetMatrix() const
	{
		return m_Matrix;
	}

	static const EventType sk_EventType;

private:
	ActorId m_ActorId;
	Matrix m_Matrix;
};

class EvtData_New_Render_Component : public BaseEventData
{
public:
	EvtData_New_Render_Component() : m_ActorId(INVALID_ACTOR_ID) {}
	explicit EvtData_New_Render_Component(ActorId actorId, shared_ptr<SceneNode> pSceneNode)
		: m_ActorId(actorId), m_pSceneNode(pSceneNode)
	{

	}

	virtual void VDeserialize(std::istrstream& in)
	{

	}

	virtual void VSerialize(std::ostrstream& out)
	{

	}

	virtual EventType VGetEventType() const override
	{
		return sk_EventType;
	}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr(GCC_NEW EvtData_New_Render_Component(m_ActorId, m_pSceneNode));
	}

	virtual const char* VGetName() const
	{
		return "EvtData_New_Render_Component";
	}

	ActorId GetActorId() const
	{
		return m_ActorId;
	}

	shared_ptr<SceneNode> GetSceneNode(void) const
	{
		return m_pSceneNode;
	}

	static const EventType sk_EventType;

private:
	ActorId m_ActorId;
	shared_ptr<SceneNode> m_pSceneNode;
};

class EvtData_Modified_Render_Component : public BaseEventData
{
public:
	EvtData_Modified_Render_Component() : m_ActorId(INVALID_ACTOR_ID) {}
	explicit EvtData_Modified_Render_Component(ActorId actorId = INVALID_ACTOR_ID)
		: m_ActorId(actorId)
	{

	}

	virtual void VDeserialize(std::istrstream& in)
	{
		in >> m_ActorId;
	}

	virtual void VSerialize(std::ostrstream& out)
	{
		out << m_ActorId;
	}

	virtual EventType VGetEventType() const override
	{
		return sk_EventType;
	}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr(GCC_NEW EvtData_Modified_Render_Component(m_ActorId));
	}

	virtual const char* VGetName() const
	{
		return "EvtData_Modified_Render_Component";
	}

	ActorId GetActorId() const
	{
		return m_ActorId;
	}

	static const EventType sk_EventType;

private:
	ActorId m_ActorId;
};


class EvtData_Environment_Loaded : public BaseEventData
{
public:
	EvtData_Environment_Loaded() {}
	virtual EventType VGetEventType() const override { return sk_EventType; }
	virtual IEventDataPtr VCopy() const { return IEventDataPtr(GCC_NEW EvtData_Environment_Loaded()); }
	virtual const char* VGetName() const { return "EvtData_Environment_Loaded"; }

	static const EventType sk_EventType;
};

class EvtData_Update_Tick : public BaseEventData
{
public:
	explicit EvtData_Update_Tick(int deltaMilliseconds)
		: m_DeltaMilliseconds(deltaMilliseconds)
	{

	}

	virtual void VSerialize(std::ostrstream& out)
	{

	}

	virtual EventType VGetEventType() const override
	{
		return sk_EventType;
	}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr(GCC_NEW EvtData_Update_Tick(m_DeltaMilliseconds));
	}

	virtual const char* VGetName() const
	{
		return "EvtData_Update_Tick";
	}

	static const EventType sk_EventType;

private:
	int m_DeltaMilliseconds;
};
