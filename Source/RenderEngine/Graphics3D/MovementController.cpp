#include "MovementController.h"
#include "SceneNode.h"

MovementController::MovementController(
	shared_ptr<SceneNode> pOjbect, float initialYaw, float initialPitch, bool rotateWithLButtonDown)
	: m_pObject(pOjbect),
	m_Yaw(XMConvertToRadians(-initialYaw)),
	m_Pitch(XMConvertToDegrees(initialPitch)),
	m_TargetYaw(m_Yaw),
	m_TargetPitch(m_Pitch),
	m_CurrentSpeed(0.0f),
	m_MaxSpeed(30.0f),
	m_IsLButtonDown(false),
	m_IsRotateWithLButtonDown(rotateWithLButtonDown)
{
	POINT ptCursor;
	GetCursorPos(&ptCursor);
	m_LastMousePos.x = static_cast<float>(ptCursor.x);
	m_LastMousePos.y = static_cast<float>(ptCursor.y);
	
	memset(m_Keys, 0x00, sizeof(m_Keys));
}

void MovementController::OnUpdate(double fTime, float fElapsedTime)
{
	bool isTranslating = false;
	Vector3 forward = Vector3::Zero;
	Vector3 strafe = Vector3::Zero;

	Matrix worldMatrix = m_pObject->VGet()->GetWorldMatrix();
	if (m_Keys['W'] || m_Keys[VK_UP])
	{
		forward = Vector3::TransformNormal(Vector3::Forward, worldMatrix);
		isTranslating = true;
	}
	else if (m_Keys['S'] || m_Keys[VK_DOWN])
	{
		forward = Vector3::TransformNormal(Vector3::Backward, worldMatrix);
		isTranslating = true;
	}

	if (m_Keys['A'] || m_Keys[VK_LEFT])
	{
		strafe = Vector3::TransformNormal(Vector3::Right, worldMatrix);
		isTranslating = true;
	}
	else if (m_Keys['D'] || m_Keys[VK_RIGHT])
	{
		strafe = Vector3::TransformNormal(Vector3::Left, worldMatrix);
		isTranslating = true;
	}

	Vector3 position = worldMatrix.Translation();

	if (isTranslating)
	{
		float numberOfSeconds = 5.0f;
		m_CurrentSpeed += m_MaxSpeed * ((fElapsedTime * fElapsedTime) / numberOfSeconds);
		if (m_CurrentSpeed > m_MaxSpeed)
			m_CurrentSpeed = m_MaxSpeed;

		Vector3 direction = forward + strafe;
		direction.Normalize();
		direction *= m_CurrentSpeed;
		position += direction;
	}
	else
	{
		m_CurrentSpeed = 0.0f;
	}

	{
		m_Yaw += (m_TargetYaw - m_Yaw) * (.35f);
		m_TargetPitch = std::max<float>(-90.0f, std::min<float>(90.0f, m_TargetPitch));
		m_Pitch += (m_TargetPitch - m_Pitch) * (.35f);

		Matrix rotation = Matrix::CreateFromYawPitchRoll(
			XMConvertToRadians(-m_Yaw), XMConvertToRadians(m_Pitch), 0.0f);
		rotation.Translation(position);

		if (m_pObject != nullptr)
		{
			m_pObject->VSetTransform(rotation);
		}
	}
}

bool MovementController::VOnPointerButtonDown(const Vector2 &pos, int radius, const std::string &buttonName)
{
	if (buttonName == "PointerLeft")
	{
		m_IsLButtonDown = true;
		m_LastMousePos = pos;
		return true;
	}
	return false;
}

bool MovementController::VOnPointerButtonUp(const Vector2 &pos, int radius, const std::string &buttonName)
{
	if (buttonName == "PointerLeft")
	{
		m_IsLButtonDown = false;
		return true;
	}
	return false;
}

bool MovementController::VOnPointerMove(const Vector2 &pos, int radius)
{
	if (m_IsRotateWithLButtonDown)
	{
		if (m_IsLButtonDown && m_LastMousePos != pos)
		{
			m_TargetYaw += (m_LastMousePos.x - pos.x);
			m_TargetPitch += (pos.y - m_LastMousePos.y);
			m_LastMousePos = pos;
		}
	}
	else if (m_LastMousePos != pos)
	{
		m_TargetYaw += (m_LastMousePos.x - pos.x);
		m_TargetPitch += (pos.y - m_LastMousePos.y);
		m_LastMousePos = pos;
	}
	else
	{
		//Do nothing here
	}

	return true;
}
