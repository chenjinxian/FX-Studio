#include "TransformComponent.h"

const std::string TransformComponent::m_Name = "TransformComponent";

TransformComponent::TransformComponent()
	: m_Transform(Matrix::Identity),
	m_Position(Vector3::Zero),
	m_Direction(Vector3::Forward),
	m_Up(Vector3::Up)
{
}


TransformComponent::~TransformComponent()
{
}

bool TransformComponent::VInit(TiXmlElement* pData)
{
	GCC_ASSERT(pData);

	Vector3 scales(1.0f, 1.0f, 1.0f);

	double x = 0; double y = 0; double z = 0;
	TiXmlElement* pPositionElement = pData->FirstChildElement("Translation");
	if (pPositionElement)
	{
		pPositionElement->Attribute("x", &x);
		pPositionElement->Attribute("y", &y);
		pPositionElement->Attribute("z", &z);
		m_Position = Vector3((float)x, (float)y, (float)z);
	}

	TiXmlElement* pScaleElement = pData->FirstChildElement("Scale");
	if (pScaleElement)
	{
		pScaleElement->Attribute("x", &x);
		pScaleElement->Attribute("y", &y);
		pScaleElement->Attribute("z", &z);
		scales = Vector3((float)x, (float)y, (float)z);
	}

	TiXmlElement* pDirectionElement = pData->FirstChildElement("Direction");
	if (pDirectionElement)
	{
		pDirectionElement->Attribute("x", &x);
		pDirectionElement->Attribute("y", &y);
		pDirectionElement->Attribute("z", &z);
		m_Direction = Vector3((float)x, (float)y, (float)z);
	}

	TiXmlElement* pUpElement = pData->FirstChildElement("Up");
	if (pUpElement)
	{
		pUpElement->Attribute("x", &x);
		pUpElement->Attribute("y", &y);
		pUpElement->Attribute("z", &z);
		m_Up = Vector3((float)x, (float)y, (float)z);
	}

	m_Transform = Matrix::CreateLookAt(m_Position, m_Position + m_Direction, m_Up);

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
