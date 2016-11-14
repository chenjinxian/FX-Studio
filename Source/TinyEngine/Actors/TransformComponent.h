#pragma once
#include "ActorComponent.h"

class TransformComponent : public ActorComponent
{
public:
	TransformComponent();
	virtual ~TransformComponent();

	virtual bool VInit(TiXmlElement* pData) override;
	virtual TiXmlElement* VGenerateXml() override;

	const Matrix& GetTransform(void) const { return m_Transform; }
	void SetTransform(const Matrix& newTransform) { m_Transform = newTransform; }
	Vector3 GetPosition(void) const { return m_Transform.Translation(); }
	void SetPosition(const Vector3& pos) { m_Transform.Translation(pos); }
	Vector3 GetDirection(void) const { return Vector3::TransformNormal(Vector3::Forward, m_Transform); }

	virtual const std::string& VGetComponentName() const override { return m_Name; }
	static const std::string m_Name;

private:
	Matrix m_Transform;
};

