#pragma once
#include "../RenderEngineBase.h"
#include "../RenderEngineInterface.h"
#include "../Graphics3D/SceneNode.h"

class ActorComponent
{
	friend class ActorFactory;

public:
	ActorComponent() : m_pOwner(nullptr) {}
	virtual ~ActorComponent() { m_pOwner.reset(); }

	virtual bool VInit(TiXmlElement* pData) = 0;
	virtual void VPostInit() {}
	virtual void VUpdate(double fTime, float fElapsedTime) {}
	virtual void VOnChanged() {}

	virtual TiXmlElement* VGenerateXml() = 0;
	virtual ComponentId VGetComponentId() const { return GetIdFromName(VGetComponentName()); }
	virtual const std::string& VGetComponentName() const = 0;
	static ComponentId GetIdFromName(const std::string& compoentStr)
	{
		void* rawId = HashedString::hash_name(compoentStr.c_str());
		return reinterpret_cast<ComponentId>(rawId);
	}

protected:
	StrongActorPtr m_pOwner;

private:
	void SetOwner(StrongActorPtr pOwner) { m_pOwner = pOwner; }
};

