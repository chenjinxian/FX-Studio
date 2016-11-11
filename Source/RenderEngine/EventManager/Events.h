#pragma once
#include "../RenderEngineBase.h"
#include "../RenderEngineInterface.h"
#include "EventManager.h"

class SceneNode;
class EvtData_New_Actor : public BaseEventData
{
public:
	EvtData_New_Actor() : m_ActorId(INVALID_ACTOR_ID), m_ViewId(INVALID_GAME_VIEW_ID) {}
	explicit EvtData_New_Actor(ActorId actorId, GameViewId viewId = INVALID_GAME_VIEW_ID)
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

	virtual EventType VGetEventType() const override
	{
		return sk_EventType;
	}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr(GCC_NEW EvtData_Environment_Loaded());
	}

	virtual const char* VGetName() const
	{
		return "EvtData_Environment_Loaded";
	}

	static const EventType sk_EventType;
};

class EvtData_Remote_Environment_Loaded : public BaseEventData
{
public:
	EvtData_Remote_Environment_Loaded() {}

	virtual EventType VGetEventType() const override
	{
		return sk_EventType;
	}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr(GCC_NEW EvtData_Remote_Environment_Loaded());
	}

	virtual const char* VGetName() const
	{
		return "EvtData_Remote_Environment_Loaded";
	}

	static const EventType sk_EventType;
};

class EvtData_Request_Start_Game : public BaseEventData
{

public:
	EvtData_Request_Start_Game() {}

	virtual EventType VGetEventType() const override
	{
		return sk_EventType;
	}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr(GCC_NEW EvtData_Request_Start_Game());
	}

	virtual const char* VGetName() const
	{
		return "EvtData_Request_Start_Game";
	}

	static const EventType sk_EventType;

};

class EvtData_Remote_Client : public BaseEventData
{
public:
	EvtData_Remote_Client() : m_SocketId(0), m_IpAddress(0) {}

	explicit EvtData_Remote_Client(int socketId, int ipAddress)
		: m_SocketId(socketId), m_IpAddress(ipAddress)
	{
	}

	virtual void VSerialize(std::ostrstream& out)
	{
		out << m_SocketId << " ";
		out << m_IpAddress;
	}

	virtual void VDeserialize(std::istrstream& in)
	{
		in >> m_SocketId;
		in >> m_IpAddress;
	}

	virtual EventType VGetEventType() const override
	{
		return sk_EventType;
	}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr(GCC_NEW EvtData_Remote_Client(m_SocketId, m_IpAddress));
	}

	virtual const char* VGetName() const
	{
		return "EvtData_Remote_Client";
	}

	int GetSocketId() const
	{
		return m_SocketId;
	}

	int GetIpAddress() const
	{
		return m_IpAddress;
	}

	static const EventType sk_EventType;

private:
	int m_SocketId;
	int m_IpAddress;
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
		DEBUG_ERROR("You should not be serializing update ticks!");
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

class EvtData_Network_Player_Actor_Assignment : public BaseEventData
{
public:
	EvtData_Network_Player_Actor_Assignment() : m_ActorId(INVALID_ACTOR_ID), m_SocketId(-1)
	{
	}

	explicit EvtData_Network_Player_Actor_Assignment(ActorId actorId, int socketId)
		: m_ActorId(actorId), m_SocketId(socketId)

	{
	}

	virtual EventType VGetEventType(void) const
	{
		return sk_EventType;
	}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr(GCC_NEW EvtData_Network_Player_Actor_Assignment(m_ActorId, m_SocketId));
	}

	virtual const char* VGetName(void) const
	{
		return "EvtData_Network_Player_Actor_Assignment";
	}

	virtual void VSerialize(std::ostrstream &out) const
	{
		out << m_ActorId << " ";
		out << m_SocketId;
	}

	virtual void VDeserialize(std::istrstream &in)
	{
		in >> m_ActorId;
		in >> m_SocketId;
	}

	ActorId GetActorId(void) const
	{
		return m_ActorId;
	}

	ActorId GetSocketId(void) const
	{
		return m_SocketId;
	}

	static const EventType sk_EventType;

private:
	ActorId m_ActorId;
	int m_SocketId;
};

class EvtData_Decompress_Request : public BaseEventData
{

public:
	explicit EvtData_Decompress_Request(std::wstring zipFileName, std::string filename)
		: m_zipFileName(zipFileName), m_fileName(filename)
	{
	}

	virtual EventType VGetEventType(void) const
	{
		return sk_EventType;
	}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr(GCC_NEW EvtData_Decompress_Request(m_zipFileName, m_fileName));
	}

	virtual void VSerialize(std::ostrstream & out)
	{
		DEBUG_ERROR("You should not be serializing decompression requests!");
	}

	const std::wstring& GetZipFilename(void) const
	{
		return m_zipFileName;
	}

	const std::string& GetFilename(void) const
	{
		return m_fileName;
	}
	virtual const char* VGetName(void) const
	{
		return "EvtData_Decompress_Request";
	}

	static const EventType sk_EventType;

private:
	std::wstring m_zipFileName;
	std::string m_fileName;
};

class EvtData_Decompression_Progress : public BaseEventData
{
public:
	EvtData_Decompression_Progress(int progress, std::wstring zipFileName, std::string filename, void *buffer)
		: m_progress(progress),
		m_zipFileName(zipFileName),
		m_fileName(filename),
		m_buffer(buffer)
	{
	}

	virtual EventType VGetEventType(void) const
	{
		return sk_EventType;
	}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr(GCC_NEW EvtData_Decompression_Progress(m_progress, m_zipFileName, m_fileName, m_buffer));
	}

	virtual void VSerialize(std::ostrstream & out)
	{
		DEBUG_ERROR("You should not be serializing decompression progress events!");
	}

	virtual const char* VGetName(void) const
	{
		return "EvtData_Decompression_Progress";
	}

	static const EventType sk_EventType;

private:
	int m_progress;
	std::wstring m_zipFileName;
	std::string m_fileName;
	void *m_buffer;
};

class EvtData_Request_New_Actor : public BaseEventData
{
public:
	static const EventType sk_EventType;

	EvtData_Request_New_Actor()
		: m_ActorResource(),
		m_HasInitialTransform(false),
		m_InitialTransform(Matrix::Identity),
		m_ServerActorId(-1),
		m_GameViewId(INVALID_GAME_VIEW_ID)
	{
	}

	explicit EvtData_Request_New_Actor(
		const std::string &actorResource, const Matrix& initialTransform,
		ActorId serverActorId = INVALID_ACTOR_ID, GameViewId viewId = INVALID_GAME_VIEW_ID)
	{
		m_ActorResource = actorResource;
		m_InitialTransform = initialTransform;
		m_ServerActorId = serverActorId;
		m_GameViewId = viewId;
	}

	virtual EventType VGetEventType(void) const
	{
		return sk_EventType;
	}

	virtual void VDeserialize(std::istrstream & in)
	{
		in >> m_ActorResource;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				in >> m_InitialTransform.m[i][j];
			}
		}
		in >> m_ServerActorId;
		in >> m_GameViewId;
	}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr(GCC_NEW EvtData_Request_New_Actor(
			m_ActorResource, m_InitialTransform, m_ServerActorId, m_GameViewId));
	}

	virtual void VSerialize(std::ostrstream & out) const
	{
		out << m_ActorResource << " ";
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				out << m_InitialTransform.m[i][j] << " ";
			}
		}
		out << m_ServerActorId << " ";
		out << m_GameViewId << " ";
	}

	virtual const char* VGetName(void) const { return "EvtData_Request_New_Actor"; }

	const std::string &GetActorResource(void) const { return m_ActorResource; }
	const Matrix& GetInitialTransform(void) const { return m_InitialTransform; }
	ActorId GetServerActorId(void) const { return m_ServerActorId; }
	GameViewId GetViewId(void) const { return m_GameViewId; }

private:
	std::string m_ActorResource;
	bool m_HasInitialTransform;
	Matrix m_InitialTransform;
	ActorId m_ServerActorId;
	GameViewId m_GameViewId;
};
