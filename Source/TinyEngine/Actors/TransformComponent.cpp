#include "TransformComponent.h"

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

	Vector3 scale;
	Quaternion rotation;
	Vector3 translation;
	m_Transform.Decompose(scale, rotation, translation);

	float x = 0; float y = 0; float z = 0;
	tinyxml2::XMLElement* pPositionElement = pData->FirstChildElement("Translation");
	if (pPositionElement)
	{
		translation.x = pPositionElement->FloatAttribute("x");
		translation.y = pPositionElement->FloatAttribute("y");
		translation.z = pPositionElement->FloatAttribute("z");
	}

	tinyxml2::XMLElement* pScaleElement = pData->FirstChildElement("Scale");
	if (pScaleElement)
	{
		scale.x = pScaleElement->FloatAttribute("x");
		scale.y = pScaleElement->FloatAttribute("y");
		scale.z = pScaleElement->FloatAttribute("z");
	}

	tinyxml2::XMLElement* pRotationElement = pData->FirstChildElement("Rotation");
	if (pRotationElement)
	{
		rotation.CreateFromYawPitchRoll(
			XMConvertToRadians(pRotationElement->FloatAttribute("y")),
			XMConvertToRadians(pRotationElement->FloatAttribute("x")),
			XMConvertToRadians(pRotationElement->FloatAttribute("z")));
	}

	m_Transform = Matrix::CreateScale(scale) * Matrix::CreateFromQuaternion(rotation) * Matrix::CreateTranslation(translation);

	return true;
}

tinyxml2::XMLElement* TransformComponent::VGenerateXml(tinyxml2::XMLDocument* pDocument)
{
	Vector3 scale;
	Quaternion rotation;
	Vector3 translation;
	m_Transform.Decompose(scale, rotation, translation);

	tinyxml2::XMLElement* pBaseElement = pDocument->NewElement(VGetComponentName().c_str());

	tinyxml2::XMLElement* pPositionElement = pDocument->NewElement("Translation");
	pPositionElement->SetAttribute("x", translation.x);
	pPositionElement->SetAttribute("y", translation.y);
	pPositionElement->SetAttribute("z", translation.z);
	pBaseElement->LinkEndChild(pPositionElement);

	tinyxml2::XMLElement* pScaleElement = pDocument->NewElement("Scale");
	pScaleElement->SetAttribute("x", scale.x);
	pScaleElement->SetAttribute("y", scale.y);
	pScaleElement->SetAttribute("z", scale.z);
	pBaseElement->LinkEndChild(pScaleElement);

	return pBaseElement;
}
