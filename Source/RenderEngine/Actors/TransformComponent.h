#pragma once
#include "../RenderEngineBase.h"
#include "../RenderEngineInterface.h"
#include "ActorComponent.h"

class TransformComponent : public ActorComponent
{
public:
	TransformComponent();
	virtual ~TransformComponent();

	virtual const char* VGetName() const { return m_Name; }
	virtual bool VInit(TiXmlElement* pData) override;
	virtual TiXmlElement* VGenerateXml() override;

	static const char* m_Name;
};

