#include "stdafx.h"
#include "ModelController.h"
#include "FXStudioEvent.h"

ModelController::ModelController(shared_ptr<CameraNode> pEditorCamera, shared_ptr<DebugGizmosNode> pGizmosNode,
	const Vector3& cameraPos, float cameraYaw, float cameraPitch)
	: m_pEditorCamera(pEditorCamera),
	m_pGizmosNode(pGizmosNode),
	m_Yaw(cameraYaw),
	m_Pitch(cameraPitch),
	m_TargetYaw(m_Yaw),
	m_TargetPitch(m_Pitch),
	m_Delta(0.0f),
	m_MoveX(0.0f),
	m_MoveY(0.0f),
	m_CurrentSpeed(0.0f),
	m_MaxSpeed(50.0f),
	m_IsLButtonDown(false)
{
	Matrix rotation = Matrix::CreateFromYawPitchRoll(XMConvertToRadians(m_Yaw), -XMConvertToRadians(m_Pitch), 0.0f);
	Matrix translation = Matrix::CreateTranslation(cameraPos);
	m_pEditorCamera->VSetTransform(rotation * translation);
	shared_ptr<EvtData_Move_Camera> pEvent(DEBUG_NEW EvtData_Move_Camera());
	IEventManager::Get()->VQueueEvent(pEvent);

	SetCameraType(CT_OrbitView);

	POINT ptCursor;
	GetCursorPos(&ptCursor);
	m_LastMousePos.x = static_cast<float>(ptCursor.x);
	m_LastMousePos.y = static_cast<float>(ptCursor.y);

	memset(m_Keys, 0x00, sizeof(m_Keys));
}

void ModelController::SetCameraType(CameraType type)
{
	m_CameraType = type;

	Vector3 scale;
	Quaternion quat;
	Vector3 postion;
	Matrix world = m_pEditorCamera->VGet()->GetWorldMatrix();
	world.Decompose(scale, quat, postion);
	float pitch = 0.0f;
	float yaw = 0.0f;
	float roll = 0.0f;
	Utility::QuaternionToAngle(quat, yaw, pitch, roll);

	switch (m_CameraType)
	{
	case CT_FirstPerson:
	{
		m_TargetYaw = m_Yaw = yaw;
		m_TargetPitch = m_Pitch = pitch;

		break;
	}
	case CT_OrbitView:
	{
		Matrix translate = world * Matrix::CreateFromQuaternion(quat).Invert();
		m_Position = translate.Translation();
		break;
	}
	default:
		break;
	}
}

void ModelController::OnUpdate(const GameTime& gameTime)
{
	switch (m_CameraType)
	{
	case CT_FirstPerson:
	{
		Vector3 forward = Vector3::Zero;
		Vector3 strafe = Vector3::Zero;

		Matrix worldMatrix = m_pEditorCamera->VGet()->GetWorldMatrix();
		if (m_Keys['W'] || m_Keys[VK_UP])
		{
			forward = Vector3::TransformNormal(Vector3::Forward, worldMatrix);
			m_IsChanged = true;
		}
		else if (m_Keys['S'] || m_Keys[VK_DOWN])
		{
			forward = Vector3::TransformNormal(Vector3::Backward, worldMatrix);
			m_IsChanged = true;
		}

		if (m_Keys['A'] || m_Keys[VK_LEFT])
		{
			strafe = Vector3::TransformNormal(Vector3::Left, worldMatrix);
			m_IsChanged = true;
		}
		else if (m_Keys['D'] || m_Keys[VK_RIGHT])
		{
			strafe = Vector3::TransformNormal(Vector3::Right, worldMatrix);
			m_IsChanged = true;
		}

		Vector3 position = worldMatrix.Translation();
		if (m_IsChanged)
		{
			float numberOfSeconds = 5.0f;
			m_CurrentSpeed += m_MaxSpeed * ((gameTime.GetElapsedTime() * gameTime.GetElapsedTime()) / numberOfSeconds);
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
			m_Yaw += (m_TargetYaw - m_Yaw) * (.25f);
			m_TargetPitch = std::max<float>(-90.0f, std::min<float>(90.0f, m_TargetPitch));
			m_Pitch += (m_TargetPitch - m_Pitch) * (.25f);

			float radiansYaw = XMConvertToRadians(m_Yaw);
			float radiansPitch = XMConvertToRadians(m_Pitch);
			Matrix rotation = Matrix::CreateFromYawPitchRoll(radiansYaw, -radiansPitch, 0.0f);
			Matrix translation = Matrix::CreateTranslation(position);

			if (m_IsChanged && m_pEditorCamera != nullptr)
			{
				m_pEditorCamera->VSetTransform(rotation * translation);
			}
		}

		break;
	}
	case CT_OrbitView:
	{
		float speed = m_Delta * 0.01f;
		m_Position += Vector3::Forward * speed;
		m_Delta = 0.0f;

		m_Yaw += (m_TargetYaw - m_Yaw) * (.25f);
		m_Pitch += (m_TargetPitch - m_Pitch) * (.25f);

		float radiansYaw = XMConvertToRadians(m_Yaw);
		float radiansPitch = XMConvertToRadians(m_Pitch);
		Matrix rotation = Matrix::CreateFromYawPitchRoll(radiansYaw, -radiansPitch, 0.0f);
		Matrix translation = Matrix::CreateTranslation(m_Position);

		if (m_IsChanged && m_pEditorCamera != nullptr)
		{
			m_pEditorCamera->VSetTransform(translation * rotation);
		}
		break;
	}
	default:
		break;
	}

	if (m_IsChanged)
	{
		shared_ptr<EvtData_Move_Camera> pEvent(DEBUG_NEW EvtData_Move_Camera());
		IEventManager::Get()->VQueueEvent(pEvent);
	}
	m_IsChanged = false;
}

bool ModelController::VOnPointerLeftButtonDown(const Vector2 &pos, int radius)
{
	m_IsLButtonDown = true;
	m_LastMousePos = pos;

	if (m_pGizmosNode != nullptr)
	{
		m_pGizmosNode->PointerLeftClick(pos);
	}

	return true;
}

bool ModelController::VOnPointerLeftButtonUp(const Vector2 &pos, int radius)
{
	m_IsLButtonDown = false;

	if (m_pGizmosNode != nullptr)
	{
		m_pGizmosNode->PointerMove(pos, m_IsLButtonDown);
	}
	return true;
}

bool ModelController::VOnPointerRightButtonDown(const Vector2 &pos, int radius)
{
	return true;
}

bool ModelController::VOnPointerRightButtonUp(const Vector2 &pos, int radius)
{
	return true;
}

bool ModelController::VOnPointerMove(const Vector2 &pos, int radius)
{
	if (m_LastMousePos != pos)
	{
		switch (m_CameraType)
		{
		case CT_FirstPerson:
		{
			if (m_IsLButtonDown)
			{
				m_TargetYaw += (m_LastMousePos.x - pos.x);
				m_TargetPitch += (pos.y - m_LastMousePos.y);
				m_IsChanged = true;
			}

			break;
		}
		case CT_OrbitView:
		{
			if (m_IsLButtonDown)
			{
				if (GetKeyState(VK_MENU) < 0)
				{
					m_TargetYaw += (m_LastMousePos.x - pos.x);
					m_TargetPitch += (pos.y - m_LastMousePos.y);
					m_IsChanged = true;
				}
				else if (m_Keys[VK_SHIFT])
				{
					m_Delta = (m_LastMousePos.y - pos.y);
					m_IsChanged = true;
				}
				else
				{
					if (m_pGizmosNode != nullptr)
					{
						m_pGizmosNode->PointerMove(pos, m_IsLButtonDown);
					}
				}
			}
			else
			{
				if (m_pGizmosNode != nullptr)
				{
					m_pGizmosNode->PointerMove(pos, m_IsLButtonDown);
				}
			}

			break;
		}
		default:
			break;
		}

		m_LastMousePos = pos;
	}
	else
	{
		m_IsChanged = false;
	}

	return true;
}

bool ModelController::VOnPointerWheel(int16_t delta)
{
	m_Delta = static_cast<float>(delta);
	m_IsChanged = true;
	return true;
}

