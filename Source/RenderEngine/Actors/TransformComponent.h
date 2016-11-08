#pragma once
#include "../RenderEngineBase.h"
#include "../RenderEngineInterface.h"
#include "ActorComponent.h"

class TransformComponent : public ActorComponent
{
public:
	TransformComponent();
	virtual ~TransformComponent();

	virtual bool VInit(TiXmlElement* pData) override;
	virtual TiXmlElement* VGenerateXml() override;

	const Matrix& GetTransform(void) const
	{
		return m_Transform;
	}
	void SetTransform(const Matrix& newTransform) { m_Transform = newTransform; }
	Vector3 GetPosition(void) const { return m_Transform.Translation(); }
	void SetPosition(const Vector3& pos) { m_Transform.Translation(pos); }
	Vector3 GetDirection(void) const { return m_Transform.Translation(); }
	void SetDirection(const Vector3& pos) { m_Transform.Translation(pos); }
	Vector3 GetUp(void) const { return m_Transform.Translation(); }
	void SetUp(const Vector3& pos) { m_Transform.Translation(pos); }

	virtual const std::string& VGetComponentName() const override { return m_Name; }
	static const std::string m_Name;

private:
	Matrix m_Transform;
	Vector3 m_Position;
	Vector3 m_Direction;
	Vector3 m_Up;
};

