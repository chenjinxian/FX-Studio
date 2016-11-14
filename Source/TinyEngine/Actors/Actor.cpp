#include "Actor.h"
#include "ActorComponent.h"
#include "boost/lexical_cast.hpp"

Actor::Actor(ActorId actorId)
	: m_ActorId(actorId),
	m_ActorType("Unknown")
{
}


Actor::~Actor()
{
	DEBUG_LOG("Actor", std::string("Destroying Actor ") + boost::lexical_cast<std::string>(m_ActorId));
	DEBUG_ASSERT(m_ActorComponents.empty());
}

bool Actor::Init(TiXmlElement* pData)
{
	DEBUG_LOG("Actor", std::string("Initializing Actor ") + boost::lexical_cast<std::string>(m_ActorId));

	m_ActorType = pData->Attribute("type");
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

void Actor::Update(double fTime, float fElapsedTime)
{
	for (auto& component : m_ActorComponents)
	{
		component.second->VUpdate(fTime, fElapsedTime);
	}
}

std::string Actor::ToXml()
{
	TiXmlDocument outDoc;

	TiXmlElement* pActorElement = DEBUG_NEW TiXmlElement("Actor");
	pActorElement->SetAttribute("type", m_ActorType.c_str());

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
	std::pair<ActorCompoentMap::iterator, bool> success =
		m_ActorComponents.insert(std::make_pair(pComponent->VGetComponentId(), pComponent));
	DEBUG_ASSERT(success.second);
}
