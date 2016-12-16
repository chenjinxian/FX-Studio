#pragma once
#include "../TinyEngine/TinyEngine.h"

class SceneNode;

class ModelController : public IPointerHandler, public IKeyboardHandler
{
public:
	ModelController(shared_ptr<SceneNode> pOjbect, const Vector3& initialPostition, float initialYaw, float initialPitch);

	void SetObject(shared_ptr<SceneNode> newObject) { m_pObject = newObject; }
	void OnUpdate(const GameTime& gameTime);

	virtual bool VOnPointerButtonDown(const Vector2 &pos, int radius, const std::string &buttonName) override;
	virtual bool VOnPointerButtonUp(const Vector2 &pos, int radius, const std::string &buttonName) override;
	virtual bool VOnPointerMove(const Vector2 &pos, int radius) override;
	virtual bool VOnPointerWheel(int16_t delta) override;

	virtual bool VOnKeyDown(uint8_t c) override { m_Keys[c] = true; return true; }
	virtual bool VOnKeyUp(uint8_t c) override { m_Keys[c] = false; return true; }

protected:
	shared_ptr<SceneNode> m_pObject;

	Vector3 m_Position;
	float m_Yaw;
	float m_Pitch;
	float m_TargetYaw;
	float m_TargetPitch;
	float m_Delta;

	bool m_IsLButtonDown;
	Vector2 m_LastMousePos;
	bool m_Keys[256];
};

