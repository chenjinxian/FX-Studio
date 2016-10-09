#pragma once
#include "../RenderEngineBase.h"
#include "../RenderEngineInterface.h"
#include "../Utilities/Templates.h"

class RenderEngineApp;
class ActorFactory
{
public:
	ActorFactory(RenderEngineApp* pApp);
	virtual ~ActorFactory();

	StrongActorPtr CreateActor(
		const std::string& actorResource, TiXmlElement *overrides, const Matrix& initialTransform);
	void ModifyActor(StrongActorPtr pActor, TiXmlElement* overrides);

protected:
	virtual StrongActorComponentPtr VCreateComponent(TiXmlElement* pData);
	GenericObjectFactory<ActorComponent, ComponentId> m_ComponentFactory;

private:
	ActorId GetNextActorId();

	RenderEngineApp* m_pApp;
	ActorId m_LastActorId;
};

