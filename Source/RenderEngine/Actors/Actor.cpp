#include "Actor.h"
#include "ActorComponent.h"

Actor::Actor(ActorId actorId) : m_ActorId(actorId), m_ActorType("Unknown"), m_ActorResource("Unknown")
{
}


Actor::~Actor()
{
	GCC_LOG("Actor", std::string("Destroying Actor ") + ToStr(m_ActorId));
	GCC_ASSERT(m_ActorComponents.empty());
}

bool Actor::Init(TiXmlElement* pData)
{
	GCC_LOG("Actor", std::string("Initializing Actor ") + ToStr(m_ActorId));

	m_ActorType = pData->Attribute("type");
	m_ActorResource = pData->Attribute("resource");
	return true;
}

void Actor::PostInit()
{
	for (auto& component : m_ActorComponents)
	{
		component.second->VPostInit();
	}
}

void Actor::Destroy()
{
	m_ActorComponents.clear();
}

void Actor::Update(uint32_t deltaMs)
{
	for (auto& component : m_ActorComponents)
	{
		component.second->VUpdate(deltaMs);
	}
}

std::string Actor::ToXml()
{
	TiXmlDocument outDoc;

	TiXmlElement* pActorElement = GCC_NEW TiXmlElement("Actor");
	pActorElement->SetAttribute("type", m_ActorType.c_str());
	pActorElement->SetAttribute("resource", m_ActorResource.c_str());

	for (auto& component : m_ActorComponents)
	{
		TiXmlElement* pComponentElement = component.second->VGenerateXml();
		pActorElement->LinkEndChild(pComponentElement);
	}

	outDoc.LinkEndChild(pActorElement);
	TiXmlPrinter printer;
	outDoc.Accept(&printer);

	return printer.CStr();
}

void Actor::AddComponent(StrongActorComponentPtr pComponent)
{
	std::pair<ActorCompoentMap::iterator, bool> success = m_ActorComponents.insert(std::make_pair(pComponent->VGetId(), pComponent));
	GCC_ASSERT(success.second);
}
