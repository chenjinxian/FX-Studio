#include "TransformComponent.h"

const std::string TransformComponent::m_Name = "TransformComponent";

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
	TiXmlElement* pBaseElement = GCC_NEW TiXmlElement(VGetComponentName().c_str());
	return pBaseElement;
}
