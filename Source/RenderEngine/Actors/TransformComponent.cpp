#include "TransformComponent.h"

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
	GCC_ASSERT(pData);

	Vector3 position(0.0f, 0.0f, 0.0f);
	Vector3 scales(1.0f, 1.0f, 1.0f);
	Vector3 yawPitchRoll(0.0f, 0.0f, 0.0f);

	double x = 0; double y = 0; double z = 0;
	TiXmlElement* pPositionElement = pData->FirstChildElement("Position");
	if (pPositionElement)
	{
		pPositionElement->Attribute("x", &x);
		pPositionElement->Attribute("y", &y);
		pPositionElement->Attribute("z", &z);
		position = Vector3(x, y, z);
	}

	TiXmlElement* pScaleElement = pData->FirstChildElement("Scale");
	if (pScaleElement)
	{
		pScaleElement->Attribute("x", &x);
		pScaleElement->Attribute("y", &y);
		pScaleElement->Attribute("z", &z);
		scales = Vector3(x, y, z);
	}

	TiXmlElement* pRotationElement = pData->FirstChildElement("Rotation");
	if (pRotationElement)
	{
		pRotationElement->Attribute("yaw", &x);
		pRotationElement->Attribute("pitch", &y);
		pRotationElement->Attribute("roll", &z);
		yawPitchRoll = Vector3(x, y, z);
	}

	Matrix translation = Matrix::CreateTranslation(position);
	Matrix scale = Matrix::CreateScale(scales);
	Matrix rotation = Matrix::CreateFromYawPitchRoll(
		XMConvertToRadians(yawPitchRoll.x), XMConvertToRadians(yawPitchRoll.y), XMConvertToRadians(yawPitchRoll.z));

	m_Transform = rotation * translation;

	return true;
}

TiXmlElement* TransformComponent::VGenerateXml()
{
	TiXmlElement* pBaseElement = GCC_NEW TiXmlElement(VGetComponentName().c_str());

	TiXmlElement* pPosition = GCC_NEW TiXmlElement("Position");
	Vector3 pos = m_Transform.Translation();
	pPosition->SetAttribute("x", ToStr(pos.x).c_str());
	pPosition->SetAttribute("y", ToStr(pos.y).c_str());
	pPosition->SetAttribute("z", ToStr(pos.z).c_str());
	pBaseElement->LinkEndChild(pPosition);

	return pBaseElement;
}
