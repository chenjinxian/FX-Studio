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
	Quaternion quat;
	Vector3 translation;
	m_Transform.Decompose(scale, quat, translation);

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
		quat = Quaternion::CreateFromYawPitchRoll(
			XMConvertToRadians(pRotationElement->FloatAttribute("y")),
			XMConvertToRadians(pRotationElement->FloatAttribute("x")),
			XMConvertToRadians(pRotationElement->FloatAttribute("z")));
	}

	m_Transform = Matrix::CreateScale(scale) * Matrix::CreateFromQuaternion(quat) * Matrix::CreateTranslation(translation);

	return true;
}

tinyxml2::XMLElement* TransformComponent::VGenerateXml(tinyxml2::XMLDocument* pDocument)
{
	Vector3 scale;
	Quaternion quat;
	Vector3 translation;
	m_Transform.Decompose(scale, quat, translation);

	tinyxml2::XMLElement* pBaseElement = pDocument->NewElement(VGetComponentName().c_str());

	tinyxml2::XMLElement* pPositionElement = pDocument->NewElement("Translation");
	pPositionElement->SetAttribute("x", translation.x);
	pPositionElement->SetAttribute("y", translation.y);
	pPositionElement->SetAttribute("z", translation.z);
	pBaseElement->InsertEndChild(pPositionElement);

	tinyxml2::XMLElement* pScaleElement = pDocument->NewElement("Scale");
	pScaleElement->SetAttribute("x", scale.x);
	pScaleElement->SetAttribute("y", scale.y);
	pScaleElement->SetAttribute("z", scale.z);
	pBaseElement->InsertEndChild(pScaleElement);

	float x = 2.0f * (quat.w * quat.x - quat.y * quat.z);
	if (x > 1.0f) x = 1.0f;
	if (x < -1.0f) x = -1.0f;

	float pitch = XMConvertToDegrees(std::asinf(x));
	float yaw = 0.0f;
	float roll = 0.0f;

	if (fabsf(fabsf(x) - 1.0f) > FLT_EPSILON)
	{
		yaw = XMConvertToDegrees(std::atan2f(2.0f * (quat.x * quat.z + quat.w * quat.y), 1.0f - 2.0f * (quat.x * quat.x + quat.y * quat.y)));
		roll = XMConvertToDegrees(std::atan2f(2.0f * (quat.x * quat.y + quat.w * quat.z), 1.0f - 2.0f * (quat.x * quat.x + quat.z * quat.z)));
	}
	else
	{
		roll = XMConvertToDegrees(std::atan2f(2.0f * (quat.x * quat.y - quat.w * quat.z), 1.0f - 2.0f * (quat.y * quat.y + quat.z * quat.z)));
	}

	tinyxml2::XMLElement* pRotationElement = pDocument->NewElement("Rotation");
	pRotationElement->SetAttribute("x", pitch);
	pRotationElement->SetAttribute("y", yaw);
	pRotationElement->SetAttribute("z", roll);
	pBaseElement->InsertEndChild(pRotationElement);

	return pBaseElement;
}
