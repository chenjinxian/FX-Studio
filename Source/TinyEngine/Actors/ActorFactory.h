#pragma once
#include "../TinyEngineBase.h"
#include "../TinyEngineInterface.h"
#include "../Utilities/Templates.h"

class ActorFactory
{
public:
	ActorFactory();
	virtual ~ActorFactory();

	StrongActorPtr CreateActor(
		TiXmlElement *pActorRoot, const Matrix& initialTransform, ActorId serversActorId);
	void ModifyActor(StrongActorPtr pActor, TiXmlElement* overrides);

protected:
	virtual StrongActorComponentPtr VCreateComponent(TiXmlElement* pData);

	GenericObjectFactory<ActorComponent, ComponentId> m_ComponentFactory;

private:
	ActorId GetNextActorId();

	ActorId m_LastActorId;
};

