#include "ActorFactory.h"
#include "ActorComponent.h"
#include "RenderComponent.h"
#include "TransformComponent.h"
#include "Actor.h"

ActorFactory::ActorFactory() : m_LastActorId(INVALID_ACTOR_ID)
{
	m_ComponentFactory.Register<TransformComponent>(ActorComponent::GetIdFromName(TransformComponent::m_Name));
	m_ComponentFactory.Register<GridRenderComponent>(ActorComponent::GetIdFromName(GridRenderComponent::m_Name));
}


ActorFactory::~ActorFactory()
{
}

StrongActorPtr ActorFactory::CreateActor(
	const char* actorResource, TiXmlElement* overrides, const Matrix& initialTransform, const ActorId serversActorId)
{
	ActorId nextActorId = serversActorId;
	if (nextActorId == INVALID_ACTOR_ID)
	{
		nextActorId = GetNextActorId();
	}
	StrongActorPtr pActor(GCC_NEW Actor(nextActorId));
	return pActor;
}

void ActorFactory::ModifyActor(StrongActorPtr pActor, TiXmlElement* overrides)
{

}

StrongActorComponentPtr ActorFactory::VCreateComponent(TiXmlElement* pData)
{
	const char* name = pData->Value();
	StrongActorComponentPtr pComponent(m_ComponentFactory.Create(ActorComponent::GetIdFromName(name)));
	return pComponent;
}

ActorId ActorFactory::GetNextActorId()
{
	m_LastActorId++;
	return m_LastActorId;
}
