#pragma once
#include "../RenderEngineBase.h"
#include "../RenderEngineInterface.h"
#include "../Utilities/Templates.h"

class ActorFactory
{
public:
	ActorFactory();
	virtual ~ActorFactory();

	StrongActorPtr CreateActor(
		const std::string& actorResource, TiXmlElement *overrides,
		const Matrix& initialTransform, const ActorId serversActorId);
	void ModifyActor(StrongActorPtr pActor, TiXmlElement* overrides);

protected:
	virtual StrongActorComponentPtr VCreateComponent(TiXmlElement* pData);
	GenericObjectFactory<ActorComponent, ComponentId> m_ComponentFactory;

private:
	ActorId GetNextActorId();

	ActorId m_LastActorId;
};

