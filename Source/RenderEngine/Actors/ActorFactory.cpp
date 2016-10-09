#include "ActorFactory.h"
#include "ActorComponent.h"
#include "RenderComponent.h"
#include "TransformComponent.h"
#include "Actor.h"
#include "../ResourceCache/XmlResource.h"
#include "../AppFramework/RenderEngineApp.h"

ActorFactory::ActorFactory(RenderEngineApp* pApp) : m_pApp(pApp), m_LastActorId(INVALID_ACTOR_ID)
{
	m_ComponentFactory.Register<TransformComponent>(ActorComponent::GetIdFromName(TransformComponent::m_Name));
	m_ComponentFactory.Register<GridRenderComponent>(ActorComponent::GetIdFromName(GridRenderComponent::m_Name));
}


ActorFactory::~ActorFactory()
{
}

StrongActorPtr ActorFactory::CreateActor(
	const std::string& actorResource, TiXmlElement *overrides, const Matrix& initialTransform)
{
	TiXmlElement* pRoot = XmlResourceLoader::LoadAndReturnRootXmlElement(m_pApp->m_pResCache, actorResource.c_str());
	if (!pRoot)
	{
		GCC_ERROR("Failed to create actor from resource: " + std::string(actorResource));
		return StrongActorPtr();
	}

	StrongActorPtr pActor(GCC_NEW Actor(GetNextActorId()));
	if (!pActor->Init(pRoot))
	{
		GCC_ERROR("Failed to initialize actor: " + std::string(actorResource));
		return StrongActorPtr();
	}

	bool initialTransformSet = false;

	for (TiXmlElement* pNode = pRoot->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
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

	if (overrides)
	{
		ModifyActor(pActor, overrides);
	}

	shared_ptr<TransformComponent> pTransformComponent = MakeStrongPtr(pActor->GetComponent<TransformComponent>(TransformComponent::m_Name));
	if (pTransformComponent)
	{
		pTransformComponent->SetPosition(initialTransform.Backward());
	}

	// Now that the actor has been fully created, run the post init phase
	pActor->PostInit();

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
