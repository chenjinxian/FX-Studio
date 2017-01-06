#include "stdafx.h"
#include "ModelController.h"

ModelController::ModelController(shared_ptr<CameraNode> pEditorCamera, shared_ptr<DebugGizmosNode> pGizmosNode,
	const Vector3& cameraPos, float cameraYaw, float cameraPitch)
	: m_pEditorCamera(pEditorCamera),
	m_pGizmosNode(pGizmosNode),
	m_Position(cameraPos),
	m_Yaw(-cameraYaw),
	m_Pitch(cameraPitch),
	m_TargetYaw(m_Yaw),
	m_TargetPitch(m_Pitch),
	m_Delta(0.0f),
	m_MoveX(0.0f),
	m_MoveY(0.0f),
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

	float speed = m_Delta * 0.01f;
	m_Position += Vector3::Forward * speed;
	m_Delta = 0.0f;

	if (m_pEditorCamera != nullptr)
	{
		m_pEditorCamera->VSetTransform(rotation);
	}
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
		if (m_IsLButtonDown)
		{
			if (GetKeyState(VK_MENU) < 0)
			{
				m_TargetYaw += (m_LastMousePos.x - pos.x);
				m_TargetPitch += (pos.y - m_LastMousePos.y);
			}
			else if (m_Keys[VK_SHIFT])
			{
				m_Delta = (m_LastMousePos.y - pos.y);
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

		m_LastMousePos = pos;
	}

	return true;
}

bool ModelController::VOnPointerWheel(int16_t delta)
{
	m_Delta = static_cast<float>(delta);

	return true;
}

