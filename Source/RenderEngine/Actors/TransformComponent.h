#pragma once
#include "../RenderEngineBase.h"
#include "../RenderEngineInterface.h"
#include "ActorComponent.h"

class TransformComponent : public ActorComponent
{
public:
	TransformComponent();
	virtual ~TransformComponent();

	virtual const char* VGetName() const { return m_Name; }
	virtual bool VInit(TiXmlElement* pData) override;
	virtual TiXmlElement* VGenerateXml() override;

	Matrix GetTransform(void) const { return m_Transform; }
	void SetTransform(const Matrix& newTransform) { m_Transform = newTransform; }
	Vector3 GetPosition(void) const { return m_Transform.Backward(); }
	void SetPosition(const Vector3& pos) { m_Transform.Backward(pos); }

	static const char* m_Name;

private:
	Matrix m_Transform;
};

