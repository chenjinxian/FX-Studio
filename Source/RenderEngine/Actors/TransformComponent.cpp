#include "TransformComponent.h"

const char* TransformComponent::m_Name = "TransformComponent";

TransformComponent::TransformComponent()
{
}


TransformComponent::~TransformComponent()
{
}

bool TransformComponent::VInit(TiXmlElement* pData)
{
	return true;
}

TiXmlElement* TransformComponent::VGenerateXml()
{
	TiXmlElement* pBaseElement = GCC_NEW TiXmlElement(VGetName());
	return pBaseElement;
}
