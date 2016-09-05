#include "ActorFactory.h"
#include "../ResourceCache/XmlResource.h"
#include "../Utilities/String.h"
#include "ActorComponent.h"
#include "Actor.h"

ActorFactory::ActorFactory(void)
{
	m_lastActorId = INVALID_ACTOR_ID;

// 	m_componentFactory.Register<TransformComponent>(ActorComponent::GetIdFromName(TransformComponent::g_Name));
// 	m_componentFactory.Register<MeshRenderComponent>(ActorComponent::GetIdFromName(MeshRenderComponent::g_Name));
// 	m_componentFactory.Register<SphereRenderComponent>(ActorComponent::GetIdFromName(SphereRenderComponent::g_Name));
// 	m_componentFactory.Register<PhysicsComponent>(ActorComponent::GetIdFromName(PhysicsComponent::g_Name));
// 	m_componentFactory.Register<TeapotRenderComponent>(ActorComponent::GetIdFromName(TeapotRenderComponent::g_Name));
// 	m_componentFactory.Register<GridRenderComponent>(ActorComponent::GetIdFromName(GridRenderComponent::g_Name));
// 	m_componentFactory.Register<LightRenderComponent>(ActorComponent::GetIdFromName(LightRenderComponent::g_Name));
// 	m_componentFactory.Register<SkyRenderComponent>(ActorComponent::GetIdFromName(SkyRenderComponent::g_Name));
// 	m_componentFactory.Register<AudioComponent>(ActorComponent::GetIdFromName(AudioComponent::g_Name));
// 
// 	// FUTURE WORK - certainly don't need to do this now, but the following stuff should be in a TeapotWarsActorFactory, eh?
// 	m_componentFactory.Register<AmmoPickup>(ActorComponent::GetIdFromName(AmmoPickup::g_Name));
// 	m_componentFactory.Register<HealthPickup>(ActorComponent::GetIdFromName(HealthPickup::g_Name));
// 	m_componentFactory.Register<BaseScriptComponent>(ActorComponent::GetIdFromName(BaseScriptComponent::g_Name));
}

StrongActorPtr ActorFactory::CreateActor(const char* actorResource, TiXmlElement *overrides, const Matrix *pInitialTransform, const ActorId serversActorId)
{
	return StrongActorPtr();
	// Grab the root XML node
// 	TiXmlElement* pRoot = XmlResourceLoader::LoadAndReturnRootXmlElement(actorResource);
// 	if (!pRoot)
// 	{
// 		GCC_ERROR("Failed to create actor from resource: " + std::string(actorResource));
// 		return StrongActorPtr();
// 	}
// 
// 	// create the actor instance
// 	ActorId nextActorId = serversActorId;
// 	if (nextActorId == INVALID_ACTOR_ID)
// 	{
// 		nextActorId = GetNextActorId();
// 	}
// 	StrongActorPtr pActor(GCC_NEW Actor(nextActorId));
// 	if (!pActor->Init(pRoot))
// 	{
// 		GCC_ERROR("Failed to initialize actor: " + std::string(actorResource));
// 		return StrongActorPtr();
// 	}
// 
// 	bool initialTransformSet = false;
// 
// 	// Loop through each child element and load the component
// 	for (TiXmlElement* pNode = pRoot->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
// 	{
// 		StrongActorComponentPtr pComponent(VCreateComponent(pNode));
// 		if (pComponent)
// 		{
// 			pActor->AddComponent(pComponent);
// 			pComponent->SetOwner(pActor);
// 		}
// 		else
// 		{
// 			// If an error occurs, we kill the actor and bail.  We could keep going, but the actor is will only be 
// 			// partially complete so it's not worth it.  Note that the pActor instance will be destroyed because it
// 			// will fall out of scope with nothing else pointing to it.
// 			return StrongActorPtr();
// 		}
// 	}
// 
// 	if (overrides)
// 	{
// 		ModifyActor(pActor, overrides);
// 	}
// 
// 	// This is a bit of a hack to get the initial transform of the transform component set before the 
// 	// other components (like PhysicsComponent) read it.
// 	shared_ptr<TransformComponent> pTransformComponent = MakeStrongPtr(pActor->GetComponent<TransformComponent>(TransformComponent::g_Name));
// 	if (pInitialTransform && pTransformComponent)
// 	{
// 		pTransformComponent->SetPosition(pInitialTransform->GetPosition());
// 	}
// 
// 	// Now that the actor has been fully created, run the post init phase
// 	pActor->PostInit();
// 
// 	return pActor;
}

StrongActorComponentPtr ActorFactory::VCreateComponent(TiXmlElement* pData)
{
	const char* name = pData->Value();
	StrongActorComponentPtr pComponent(m_componentFactory.Create(ActorComponent::GetIdFromName(name)));

	// initialize the component if we found one
	if (pComponent)
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
		return StrongActorComponentPtr();  // fail
	}

	// pComponent will be NULL if the component wasn't found.  This isn't necessarily an error since you might have a 
	// custom CreateComponent() function in a sub class.
	return pComponent;
}


void ActorFactory::ModifyActor(StrongActorPtr pActor, TiXmlElement* overrides)
{
	// Loop through each child element and load the component
	for (TiXmlElement* pNode = overrides->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
	{
		ComponentId componentId = ActorComponent::GetIdFromName(pNode->Value());
		StrongActorComponentPtr pComponent = MakeStrongPtr(pActor->GetComponent<ActorComponent>(componentId));
		if (pComponent)
		{
			pComponent->VInit(pNode);

			// [mrmike] - added post press to ensure that components that need it have
			//            Events generated that can notify subsystems when changes happen.
			//            This was done to have SceneNode derived classes respond to RenderComponent
			//            changes.

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