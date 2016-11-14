#pragma once
#include "../TinyEngineBase.h"
#include "../TinyEngineInterface.h"

class SceneNode;

class MovementController : public IPointerHandler, public IKeyboardHandler
{
public:
	MovementController(shared_ptr<SceneNode> pOjbect, float initialYaw, float initialPitch, bool rotateWithLButtonDown);

	void SetObject(shared_ptr<SceneNode> newObject) { m_pObject = newObject; }
	void OnUpdate(double fTime, float fElapsedTime);

	virtual bool VOnPointerButtonDown(const Vector2 &pos, int radius, const std::string &buttonName) override;
	virtual bool VOnPointerButtonUp(const Vector2 &pos, int radius, const std::string &buttonName) override;
	virtual bool VOnPointerMove(const Vector2 &pos, int radius) override;

	bool VOnKeyDown(uint8_t c) { m_Keys[c] = true; return true; }
	bool VOnKeyUp(uint8_t c) { m_Keys[c] = false; return true; }

protected:
	shared_ptr<SceneNode> m_pObject;

	float m_Yaw;
	float m_Pitch;
	float m_TargetYaw;
	float m_TargetPitch;
	float m_MaxSpeed;
	float m_CurrentSpeed;

	bool m_IsLButtonDown;
	bool m_IsRotateWithLButtonDown;
	Vector2 m_LastMousePos;
	bool m_Keys[256];
};

