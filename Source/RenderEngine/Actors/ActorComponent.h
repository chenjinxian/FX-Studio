#pragma once
#include "../RenderEngineBase.h"
#include "../RenderEngineInterface.h"
#include "../Utilities/HashedString.h"
#include "../Graphics3D/SceneNode.h"

class ActorComponent
{
	friend class ActorFactory;

public:
	ActorComponent() : m_pOwner(nullptr) {}
	virtual ~ActorComponent() { m_pOwner.reset(); }

	virtual bool VInit(TiXmlElement* pData) = 0;
	virtual void VPostInit() {}
	virtual void VUpdate(uint32_t deltaMs) {}
	virtual void VOnChanged() {}

	virtual TiXmlElement* VGenerateXml() = 0;
	virtual ComponentId VGetId() const { return GetIdFromName(VGetName()); }
	virtual const char* VGetName() const = 0;
	static ComponentId GetIdFromName(const char* compoentStr)
	{
		void* rawId = HashedString::hash_name(compoentStr);
		return reinterpret_cast<ComponentId>(rawId);
	}

protected:
	StrongActorPtr m_pOwner;

private:
	void SetOwner(StrongActorPtr pOwner) { m_pOwner = pOwner; }
};

