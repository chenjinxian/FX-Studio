#pragma once
#include "../TinyEngineBase.h"
#include "../TinyEngineInterface.h"

typedef std::string ActorType;
typedef std::map<ComponentId, StrongActorComponentPtr> ActorCompoentMap;

class Actor
{
	friend class ActorFactory;

public:
	explicit Actor(ActorId actorId);
	virtual ~Actor();

	bool Init(TiXmlElement* pData);
	void PostInit();
	void Destroy();
	void Update(double fTime, float fElapsedTime);

	std::string ToXml();

	ActorId GetActorId() const { return m_ActorId; }
	ActorType GetActorType() const { return m_ActorType; }

	template <class ComponentType>
	weak_ptr<ComponentType> GetComponent(ComponentId componentId)
	{
		auto findIt = m_ActorComponents.find(componentId);
		if (findIt != m_ActorComponents.end())
		{
			StrongActorComponentPtr pBase(findIt->second);
			shared_ptr<ComponentType> pSub(static_pointer_cast<ComponentType>(pBase));
			weak_ptr<ComponentType> pWeakSub(pSub);
			return pWeakSub;
		}
		else
		{
			return weak_ptr<ComponentType>();
		}
	}

	template <class ComponentType>
	weak_ptr<ComponentType> GetComponent(const std::string& name)
	{
		ComponentId componentId = ActorComponent::GetIdFromName(name);
		auto findIt = m_ActorComponents.find(componentId);
		if (findIt != m_ActorComponents.end())
		{
			StrongActorComponentPtr pBase(findIt->second);
			shared_ptr<ComponentType> pSub(static_pointer_cast<ComponentType>(pBase));
			weak_ptr<ComponentType> pWeakSub(pSub);
			return pWeakSub;
		}
		else
		{
			return weak_ptr<ComponentType>();
		}
	}

	const ActorCompoentMap* GetComponents() { return &m_ActorComponents; }

	void AddComponent(StrongActorComponentPtr pComponent);

private:
	ActorId m_ActorId;
	ActorType m_ActorType;
	ActorCompoentMap m_ActorComponents;
};

