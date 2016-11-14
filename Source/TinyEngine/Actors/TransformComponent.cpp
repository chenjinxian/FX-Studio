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

bool TransformComponent::VInit(tinyxml2::XMLElement* pData)
{
	DEBUG_ASSERT(pData);

	Vector3 position(0.0f, 0.0f, 0.0f);
	Vector3 scales(1.0f, 1.0f, 1.0f);
	Vector3 yawPitchRoll(0.0f, 0.0f, 0.0f);

	double x = 0; double y = 0; double z = 0;
	tinyxml2::XMLElement* pPositionElement = pData->FirstChildElement("Translation");
	if (pPositionElement)
	{
// 		pPositionElement->FloatAttribute("x");
// 		pPositionElement->FloatAttribute("y");
// 		pPositionElement->FloatAttribute("z");
		position = Vector3(pPositionElement->FloatAttribute("x"), pPositionElement->FloatAttribute("y"), pPositionElement->FloatAttribute("z"));
	}

	tinyxml2::XMLElement* pScaleElement = pData->FirstChildElement("Scale");
	if (pScaleElement)
	{
		pScaleElement->Attribute("x", &x);
		pScaleElement->Attribute("y", &y);
		pScaleElement->Attribute("z", &z);
		scales = Vector3((float)x, (float)y, (float)z);
	}

	tinyxml2::XMLElement* pRotationElement = pData->FirstChildElement("Rotation");
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

tinyxml2::XMLElement* TransformComponent::VGenerateXml()
{
	tinyxml2::XMLElement* pBaseElement = DEBUG_NEW tinyxml2::XMLElement(VGetComponentName().c_str());

	tinyxml2::XMLElement* pPosition = DEBUG_NEW tinyxml2::XMLElement("Position");
	Vector3 pos = m_Transform.Translation();
	pPosition->SetAttribute("x", boost::lexical_cast<std::string>(pos.x).c_str());
	pPosition->SetAttribute("y", boost::lexical_cast<std::string>(pos.y).c_str());
	pPosition->SetAttribute("z", boost::lexical_cast<std::string>(pos.z).c_str());
	pBaseElement->LinkEndChild(pPosition);

	return pBaseElement;
}
