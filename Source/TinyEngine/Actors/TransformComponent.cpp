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

	float x = 0; float y = 0; float z = 0;
	tinyxml2::XMLElement* pPositionElement = pData->FirstChildElement("Translation");
	if (pPositionElement)
	{
		pPositionElement->QueryFloatAttribute("x", &x);
		pPositionElement->QueryFloatAttribute("y", &y);
		pPositionElement->QueryFloatAttribute("z", &z);
		position = Vector3(x, y ,z);
	}

	tinyxml2::XMLElement* pScaleElement = pData->FirstChildElement("Scale");
	if (pScaleElement)
	{
		pScaleElement->QueryFloatAttribute("x", &x);
		pScaleElement->QueryFloatAttribute("y", &y);
		pScaleElement->QueryFloatAttribute("z", &z);
		scales = Vector3(x, y, z);
	}

	tinyxml2::XMLElement* pRotationElement = pData->FirstChildElement("Rotation");
	if (pRotationElement)
	{
		pRotationElement->QueryFloatAttribute("x", &x);
		pRotationElement->QueryFloatAttribute("y", &y);
		pRotationElement->QueryFloatAttribute("z", &z);
		yawPitchRoll = Vector3(x, y, z);
	}

	Matrix translation = Matrix::CreateTranslation(position);
	Matrix scale = Matrix::CreateScale(scales);
	Matrix rotation = Matrix::CreateFromYawPitchRoll(
		XMConvertToRadians(yawPitchRoll.y), XMConvertToRadians(yawPitchRoll.x), XMConvertToRadians(yawPitchRoll.z));
	
	m_Transform = scale * rotation * translation;

	return true;
}

tinyxml2::XMLElement* TransformComponent::VGenerateXml(tinyxml2::XMLDocument* pDocument)
{
	tinyxml2::XMLElement* pBaseElement = pDocument->NewElement(VGetComponentName().c_str());

	tinyxml2::XMLElement* pPosition = pDocument->NewElement("Position");
	Vector3 pos = m_Transform.Translation();
	pPosition->SetAttribute("x", pos.x);
	pPosition->SetAttribute("y", pos.y);
	pPosition->SetAttribute("z", pos.z);
	pBaseElement->LinkEndChild(pPosition);

	return pBaseElement;
}
