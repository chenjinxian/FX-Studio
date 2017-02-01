#include "Actor.h"
#include "ActorComponent.h"

Actor::Actor(ActorId actorId)
	: m_ActorId(actorId),
	m_ActorType("Unknown")
{
}


Actor::~Actor()
{
	DEBUG_LOG("Actor", std::string("Destroying Actor ") + std::to_string(m_ActorId));
	DEBUG_ASSERT(m_ActorComponents.empty());
}

bool Actor::Init(tinyxml2::XMLElement* pData)
{
	DEBUG_LOG("Actor", std::string("Initializing Actor ") + std::to_string(m_ActorId));

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

void Actor::Update(const GameTime& gameTime)
{
	for (auto& component : m_ActorComponents)
	{
		component.second->VUpdate(gameTime);
	}
}

std::string Actor::ToXml()
{
	tinyxml2::XMLDocument outDoc;

	tinyxml2::XMLElement* pActorElement = outDoc.NewElement("Actor");
	pActorElement->SetAttribute("type", m_ActorType.c_str());

	for (auto& component : m_ActorComponents)
	{
		tinyxml2::XMLElement* pComponentElement = component.second->VGenerateXml(&outDoc);
		pActorElement->LinkEndChild(pComponentElement);
	}

	outDoc.LinkEndChild(pActorElement);
	tinyxml2::XMLPrinter printer;
	outDoc.Accept(&printer);

	return printer.CStr();
}

void Actor::AddComponent(StrongActorComponentPtr pComponent)
{
	std::pair<ActorCompoentMap::iterator, bool> success =
		m_ActorComponents.insert(std::make_pair(pComponent->VGetComponentId(), pComponent));
	DEBUG_ASSERT(success.second);
}
