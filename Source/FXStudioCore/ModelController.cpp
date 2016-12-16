#include "stdafx.h"
#include "ModelController.h"

ModelController::ModelController(shared_ptr<SceneNode> pOjbect, const Vector3& initialPostition, float initialYaw, float initialPitch)
	: m_pObject(pOjbect),
	m_Position(initialPostition),
	m_Yaw(-initialYaw),
	m_Pitch(initialPitch),
	m_TargetYaw(m_Yaw),
	m_TargetPitch(m_Pitch),
	m_Delta(0.0f),
	m_IsLButtonDown(false)
{
	POINT ptCursor;
	GetCursorPos(&ptCursor);
	m_LastMousePos.x = static_cast<float>(ptCursor.x);
	m_LastMousePos.y = static_cast<float>(ptCursor.y);

	memset(m_Keys, 0x00, sizeof(m_Keys));
}

void ModelController::OnUpdate(const GameTime& gameTime)
{
	m_Yaw += (m_TargetYaw - m_Yaw) * (.35f);
	m_Pitch += (m_TargetPitch - m_Pitch) * (.35f);

	float radiansYaw = XMConvertToRadians(m_Yaw);
	float radiansPitch = -XMConvertToRadians(m_Pitch);
	Matrix rotation = Matrix::CreateFromYawPitchRoll(radiansYaw, radiansPitch, 0.0f);
	rotation.Translation(Vector3::TransformNormal(m_Position, rotation));

	float speed = m_Delta * gameTime.GetElapsedTime() * 5.0f;
	m_Position += Vector3::Forward * speed;
	m_Delta = 0;

	if (m_pObject != nullptr)
	{
		m_pObject->VSetTransform(rotation);
	}
}

bool ModelController::VOnPointerButtonDown(const Vector2 &pos, int radius, const std::string &buttonName)
{
	if (buttonName == "PointerLeft")
	{
		m_IsLButtonDown = true;
		m_LastMousePos = pos;
		return true;
	}
	return false;
}

bool ModelController::VOnPointerButtonUp(const Vector2 &pos, int radius, const std::string &buttonName)
{
	if (buttonName == "PointerLeft")
	{
		m_IsLButtonDown = false;
		return true;
	}
	return false;
}

bool ModelController::VOnPointerMove(const Vector2 &pos, int radius)
{
	if (m_IsLButtonDown && m_LastMousePos != pos)
	{
		m_TargetYaw += (m_LastMousePos.x - pos.x);
		m_TargetPitch += (pos.y - m_LastMousePos.y);
		m_LastMousePos = pos;
	}

	return true;
}

bool ModelController::VOnPointerWheel(int16_t delta)
{
	m_Delta = static_cast<float>(delta);

	return true;
}

