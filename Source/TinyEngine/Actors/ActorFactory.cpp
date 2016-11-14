#include "ActorFactory.h"
#include "ActorComponent.h"
#include "RenderComponent.h"
#include "TransformComponent.h"
#include "Actor.h"
#include "../ResourceCache/XmlResource.h"

ActorFactory::ActorFactory() : m_LastActorId(INVALID_ACTOR_ID)
{
	m_ComponentFactory.Register<TransformComponent>(ActorComponent::GetIdFromName(TransformComponent::m_Name));
	m_ComponentFactory.Register<SkyboxRenderComponent>(ActorComponent::GetIdFromName(SkyboxRenderComponent::m_Name));
	m_ComponentFactory.Register<DirectionalLightComponent>(ActorComponent::GetIdFromName(DirectionalLightComponent::m_Name));
	m_ComponentFactory.Register<PointLightComponent>(ActorComponent::GetIdFromName(PointLightComponent::m_Name));
	m_ComponentFactory.Register<SpotLightComponent>(ActorComponent::GetIdFromName(SpotLightComponent::m_Name));
	m_ComponentFactory.Register<GridRenderComponent>(ActorComponent::GetIdFromName(GridRenderComponent::m_Name));
	m_ComponentFactory.Register<ModelRenderComponent>(ActorComponent::GetIdFromName(ModelRenderComponent::m_Name));
}


ActorFactory::~ActorFactory()
{
}

StrongActorPtr ActorFactory::CreateActor(
	tinyxml2::XMLElement *pActorRoot, const Matrix& initialTransform, ActorId serversActorId)
{
	DEBUG_ASSERT(pActorRoot != nullptr);

	ActorId nextActorId = serversActorId;
	if (nextActorId == INVALID_ACTOR_ID)
	{
		nextActorId = GetNextActorId();
	}
	StrongActorPtr pActor(DEBUG_NEW Actor(nextActorId));
	if (!pActor->Init(pActorRoot))
	{
		DEBUG_ERROR("Failed to initialize actor: " + std::string(pActorRoot->Attribute("type")));
		return StrongActorPtr();
	}

	bool initialTransformSet = false;

	for (tinyxml2::XMLElement* pNode = pActorRoot->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
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
	if (initialTransform.Translation() != Vector3::Zero && pTransformComponent)
	{
		pTransformComponent->SetPosition(initialTransform.Translation());
	}

	pActor->PostInit();

	return pActor;
}

void ActorFactory::ModifyActor(StrongActorPtr pActor, tinyxml2::XMLElement* overrides)
{
	for (tinyxml2::XMLElement* pNode = overrides->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
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

StrongActorComponentPtr ActorFactory::VCreateComponent(tinyxml2::XMLElement* pData)
{
	const char* name = pData->Value();
	StrongActorComponentPtr pComponent(m_ComponentFactory.Create(ActorComponent::GetIdFromName(name)));
	if (pComponent != nullptr)
	{
		if (!pComponent->VInit(pData))
		{
			DEBUG_ERROR("Component failed to initialize: " + std::string(name));
			return StrongActorComponentPtr();
		}
	}
	else
	{
		DEBUG_ERROR("Couldn't find ActorComponent named " + std::string(name));
		return StrongActorComponentPtr();
	}

	return pComponent;
}

ActorId ActorFactory::GetNextActorId()
{
	m_LastActorId++;
	return m_LastActorId;
}
