#pragma once
#include "../TinyEngineBase.h"
#include "../TinyEngineInterface.h"
#include "../Graphics3D/SceneNode.h"

class ActorComponent
{
	friend class ActorFactory;

public:
	ActorComponent() : m_pOwner(nullptr) {}
	virtual ~ActorComponent() { m_pOwner.reset(); }

	virtual bool VInit(tinyxml2::XMLElement* pData) = 0;
	virtual void VPostInit() {}
	virtual void VUpdate(const GameTime& gameTime) {}
	virtual void VOnChanged() {}

	virtual tinyxml2::XMLElement* VGenerateXml(tinyxml2::XMLDocument* pDocument) = 0;
	virtual ComponentId VGetComponentId() const { return GetIdFromName(VGetComponentName()); }
	virtual const std::string& VGetComponentName() const = 0;
	static ComponentId GetIdFromName(const std::string& compoentStr)
	{
		std::hash<std::string> hashString;
		ComponentId rawId = hashString(compoentStr);
		return rawId;
	}

protected:
	StrongActorPtr m_pOwner;

private:
	void SetOwner(StrongActorPtr pOwner) { m_pOwner = pOwner; }
};

