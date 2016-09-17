#include "Actor.h"
#include "ActorComponent.h"


Actor::Actor(ActorId id)
{
}


Actor::~Actor()
{
}

void Actor::Destroy()
{
	m_ActorComponents.clear();
}

void Actor::Update(uint32_t deltaMs)
{
	for (auto component : m_ActorComponents)
	{
		component.second->VUpdate(deltaMs);
	}
}
