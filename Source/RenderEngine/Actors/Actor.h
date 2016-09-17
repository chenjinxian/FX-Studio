#pragma once
#include "../RenderEngineBase.h"
#include "../RenderEngineInterface.h"

typedef std::string ActorType;
typedef std::map<ComponentId, StrongActorComponentPtr> ActorCompoentMap;

class Actor
{
	friend class ActorFactory;

public:
	Actor(ActorId id);
	virtual ~Actor();

	void Destroy();
	void Update(uint32_t deltaMs);

private:
	ActorId m_ActorId;
	ActorCompoentMap m_ActorComponents;
	ActorType m_ActorType;
};

