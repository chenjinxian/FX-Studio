#include "ActorFactory.h"
#include "ActorComponent.h"
#include "RenderComponent.h"
#include "TransformComponent.h"
#include "Actor.h"
#include "../ResourceCache/XmlResource.h"

ActorFactory::ActorFactory() : m_LastActorId(INVALID_ACTOR_ID)
{
	m_ComponentFactory.Register<TransformComponent>(ActorComponent::GetIdFromName(TransformComponent::m_Name));
	m_ComponentFactory.Register<GridRenderComponent>(ActorComponent::GetIdFromName(GridRenderComponent::m_Name));
	m_ComponentFactory.Register<SkyRenderComponent>(ActorComponent::GetIdFromName(SkyRenderComponent::m_Name));
	m_ComponentFactory.Register<MeshRenderComponent>(ActorComponent::GetIdFromName(MeshRenderComponent::m_Name));
}


ActorFactory::~ActorFactory()
{
}

StrongActorPtr ActorFactory::CreateActor(
	TiXmlElement *pActorRoot, const Matrix& initialTransform, ActorId serversActorId)
{
	GCC_ASSERT(pActorRoot != nullptr);

	ActorId nextActorId = serversActorId;
	if (nextActorId == INVALID_ACTOR_ID)
	{
		nextActorId = GetNextActorId();
	}
	StrongActorPtr pActor(GCC_NEW Actor(nextActorId));
	if (!pActor->Init(pActorRoot))
	{
		GCC_ERROR("Failed to initialize actor: " + std::string(pActorRoot->Attribute("type")));
		return StrongActorPtr();
	}

	bool initialTransformSet = false;

	for (TiXmlElement* pNode = pActorRoot->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
	{
		StrongActorComponentPtr pComponent(VCreateComponent(pNode));
		if (pComponent)
		{
			pActor->AddComponent(pComponent);
			pComponent->SetOwner(pActor);
		}
		else
		{
			return StrongActorPtr();
		}
	}

	shared_ptr<TransformComponent> pTransformComponent = MakeStrongPtr(pActor->GetComponent<TransformComponent>(TransformComponent::m_Name));
	if (pTransformComponent)
	{
		pTransformComponent->SetPosition(initialTransform.Backward());
	}

	pActor->PostInit();

	return pActor;
}

void ActorFactory::ModifyActor(StrongActorPtr pActor, TiXmlElement* overrides)
{
	for (TiXmlElement* pNode = overrides->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
	{
		ComponentId componentId = ActorComponent::GetIdFromName(pNode->Value());
		StrongActorComponentPtr pComponent = MakeStrongPtr(pActor->GetComponent<ActorComponent>(componentId));
		if (pComponent)
		{
			pComponent->VInit(pNode);
			pComponent->VOnChanged();
		}
		else
		{
			pComponent = VCreateComponent(pNode);
			if (pComponent)
			{
				pActor->AddComponent(pComponent);
				pComponent->SetOwner(pActor);
			}
		}
	}
}

StrongActorComponentPtr ActorFactory::VCreateComponent(TiXmlElement* pData)
{
	const char* name = pData->Value();
	StrongActorComponentPtr pComponent(m_ComponentFactory.Create(ActorComponent::GetIdFromName(name)));
	if (pComponent != nullptr)
	{
		if (!pComponent->VInit(pData))
		{
			GCC_ERROR("Component failed to initialize: " + std::string(name));
			return StrongActorComponentPtr();
		}
	}
	else
	{
		GCC_ERROR("Couldn't find ActorComponent named " + std::string(name));
		return StrongActorComponentPtr();
	}

	return pComponent;
}

ActorId ActorFactory::GetNextActorId()
{
	m_LastActorId++;
	return m_LastActorId;
}
