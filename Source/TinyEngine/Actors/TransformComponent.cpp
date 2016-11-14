#include "TransformComponent.h"
#include "boost/lexical_cast.hpp"

const std::string TransformComponent::m_Name = "TransformComponent";

TransformComponent::TransformComponent()
	: m_Transform(Matrix::Identity)
{
}


TransformComponent::~TransformComponent()
{
}

bool TransformComponent::VInit(TiXmlElement* pData)
{
	DEBUG_ASSERT(pData);

	Vector3 position(0.0f, 0.0f, 0.0f);
	Vector3 scales(1.0f, 1.0f, 1.0f);
	Vector3 yawPitchRoll(0.0f, 0.0f, 0.0f);

	double x = 0; double y = 0; double z = 0;
	TiXmlElement* pPositionElement = pData->FirstChildElement("Translation");
	if (pPositionElement)
	{
		pPositionElement->Attribute("x", &x);
		pPositionElement->Attribute("y", &y);
		pPositionElement->Attribute("z", &z);
		position = Vector3((float)x, (float)y, (float)z);
	}

	TiXmlElement* pScaleElement = pData->FirstChildElement("Scale");
	if (pScaleElement)
	{
		pScaleElement->Attribute("x", &x);
		pScaleElement->Attribute("y", &y);
		pScaleElement->Attribute("z", &z);
		scales = Vector3((float)x, (float)y, (float)z);
	}

	TiXmlElement* pRotationElement = pData->FirstChildElement("Rotation");
	if (pRotationElement)
	{
		pRotationElement->Attribute("x", &x);
		pRotationElement->Attribute("y", &y);
		pRotationElement->Attribute("z", &z);
		yawPitchRoll = Vector3((float)x, (float)y, (float)z);
	}

	Matrix translation = Matrix::CreateTranslation(position);
	Matrix scale = Matrix::CreateScale(scales);
	Matrix rotation = Matrix::CreateFromYawPitchRoll(
		XMConvertToRadians(yawPitchRoll.y), XMConvertToRadians(yawPitchRoll.x), XMConvertToRadians(yawPitchRoll.z));
	
	m_Transform = scale * rotation * translation;

	return true;
}

TiXmlElement* TransformComponent::VGenerateXml()
{
	TiXmlElement* pBaseElement = DEBUG_NEW TiXmlElement(VGetComponentName().c_str());

	TiXmlElement* pPosition = DEBUG_NEW TiXmlElement("Position");
	Vector3 pos = m_Transform.Translation();
	pPosition->SetAttribute("x", boost::lexical_cast<std::string>(pos.x).c_str());
	pPosition->SetAttribute("y", boost::lexical_cast<std::string>(pos.y).c_str());
	pPosition->SetAttribute("z", boost::lexical_cast<std::string>(pos.z).c_str());
	pBaseElement->LinkEndChild(pPosition);

	return pBaseElement;
}
