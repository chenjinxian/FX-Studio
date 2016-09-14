#pragma once
#include "../RenderEngineInterface.h"

class SceneNode;

class MovementController : public IPointerHandler, public IKeyboardHandler
{
public:
	MovementController(shared_ptr<SceneNode> ojbect, float initialYaw, float initialPitch);
	virtual ~MovementController();

	void OnUpdate(unsigned long elapsedMs);
	const Matrix& GetToWorld() { return m_ToWorld; }
	const Matrix& GetFromWorld() { return m_FromWorld; }

	virtual bool VOnPointerMove(const Vector2 &pos, const int radius) override;
	virtual bool VOnPointerButtonDown(const Vector2 &pos, const int radius, const std::string &buttonName) override;
	virtual bool VOnPointerButtonUp(const Vector2 &pos, const int radius, const std::string &buttonName) override;
	virtual bool VOnKeyDown(const BYTE c) override;
	virtual bool VOnKeyUp(const BYTE c) override;

private:
	Matrix m_ToWorld;
	Matrix m_FromWorld;
	Matrix m_Position;
	Vector2 m_LastMousePos;
	bool m_Keys[256];
	float m_TargetYaw;
	float m_TargetPitch;
	float m_Yaw;
	float m_Pitch;
	float m_PitchOnDown;
	float m_YawOnDown;
	float m_MaxSpeed;
	float m_CurrentSpeed;
	bool m_IsLButtonDown;
	shared_ptr<SceneNode> m_Ojbect;
};

