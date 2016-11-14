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
		tinyxml2::XMLElement *pActorRoot, const Matrix& initialTransform, ActorId serversActorId);
	void ModifyActor(StrongActorPtr pActor, tinyxml2::XMLElement* overrides);

protected:
	virtual StrongActorComponentPtr VCreateComponent(tinyxml2::XMLElement* pData);

	GenericObjectFactory<ActorComponent, ComponentId> m_ComponentFactory;

private:
	ActorId GetNextActorId();

	ActorId m_LastActorId;
};

