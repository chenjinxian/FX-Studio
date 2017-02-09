#include "stdafx.h"
#include "FXStudioView.h"
#include "FXStudioApp.h"

extern FXStudioApp globalApp;

FXStudioView::FXStudioView(shared_ptr<IRenderer> pRenderer)
	: HumanView(pRenderer),
	m_pModelController(nullptr)
{
	Frustum frustum;
	frustum.Init(XM_PIDIV4, 1.0f, 0.1f, 10000.0f);
	m_pEditorCamera.reset(DEBUG_NEW CameraNode(frustum));
	DEBUG_ASSERT(m_pEditorCamera && _T("Out of memory"));

	m_pScene->VAddChild(INVALID_ACTOR_ID, m_pEditorCamera);
	m_pScene->SetCamera(m_pEditorCamera);

	m_pGizmosNode.reset(DEBUG_NEW DebugGizmosNode());
	DEBUG_ASSERT(m_pGizmosNode && _T("Out of memory"));
	m_pScene->VAddChild(INVALID_ACTOR_ID, m_pGizmosNode);
}


FXStudioView::~FXStudioView()
{
}

void FXStudioView::VOnUpdate(const GameTime& gameTime)
{
	HumanView::VOnUpdate(gameTime);

	if (m_pModelController != nullptr)
	{
		m_pModelController->OnUpdate(gameTime);
	}
}

void FXStudioView::MoveEditorCamera(tinyxml2::XMLElement* pCameraNode)
{
	Matrix transform = m_pEditorCamera->VGet()->GetWorldMatrix();
	Vector3 scale;
	Quaternion quat;
	Vector3 translation;
	transform.Decompose(scale, quat, translation);

	if (pCameraNode != nullptr)
	{
		tinyxml2::XMLElement* pPositionElement = pCameraNode->FirstChildElement("Translation");
		if (pPositionElement)
		{
			translation.x = pPositionElement->FloatAttribute("x");
			translation.y = pPositionElement->FloatAttribute("y");
			translation.z = pPositionElement->FloatAttribute("z");
		}

		tinyxml2::XMLElement* pRotationElement = pCameraNode->FirstChildElement("Rotation");
		if (pRotationElement)
		{
			quat = Quaternion::CreateFromYawPitchRoll(
				XMConvertToRadians(pRotationElement->FloatAttribute("y")),
				XMConvertToRadians(pRotationElement->FloatAttribute("x")),
				XMConvertToRadians(pRotationElement->FloatAttribute("z")));
		}

		m_pEditorCamera->VSetTransform(
			Matrix::CreateScale(scale) * Matrix::CreateFromQuaternion(quat) * Matrix::CreateTranslation(translation));
	}
}

tinyxml2::XMLElement* FXStudioView::GenerateCameraXml(tinyxml2::XMLDocument* pDocument)
{
	Vector3 scale;
	Quaternion quat;
	Vector3 translation;
	Matrix world = m_pEditorCamera->VGet()->GetWorldMatrix();
	world.Decompose(scale, quat, translation);

	tinyxml2::XMLElement* pBaseElement = pDocument->NewElement("EditorCamera");
	pBaseElement->SetAttribute("type", "EditorCamera");

	tinyxml2::XMLElement* pPositionElement = pDocument->NewElement("Translation");
	pPositionElement->SetAttribute("x", translation.x);
	pPositionElement->SetAttribute("y", translation.y);
	pPositionElement->SetAttribute("z", translation.z);
	pBaseElement->InsertEndChild(pPositionElement);

	float x = 2.0f * (quat.w * quat.x - quat.y * quat.z);
	if (x > 1.0f) x = 1.0f;
	if (x < -1.0f) x = -1.0f;

	float pitch = XMConvertToDegrees(std::asinf(x));
	float yaw = 0.0f;
	float roll = 0.0f;

	if (fabsf(fabsf(x) - 1.0f) > FLT_EPSILON)
	{
		yaw = XMConvertToDegrees(std::atan2f(2.0f * (quat.x * quat.z + quat.w * quat.y), 1.0f - 2.0f * (quat.x * quat.x + quat.y * quat.y)));
		roll = XMConvertToDegrees(std::atan2f(2.0f * (quat.x * quat.y + quat.w * quat.z), 1.0f - 2.0f * (quat.x * quat.x + quat.z * quat.z)));
	}
	else
	{
		roll = XMConvertToDegrees(std::atan2f(2.0f * (quat.x * quat.y - quat.w * quat.z), 1.0f - 2.0f * (quat.y * quat.y + quat.z * quat.z)));
	}

	tinyxml2::XMLElement* pRotationElement = pDocument->NewElement("Rotation");
	pRotationElement->SetAttribute("x", pitch);
	pRotationElement->SetAttribute("y", yaw);
	pRotationElement->SetAttribute("z", roll);
	pBaseElement->InsertEndChild(pRotationElement);

	return pBaseElement;
}

HRESULT FXStudioView::VOnInitGameViews()
{
	return S_OK;
}

HRESULT FXStudioView::VOnDeleteGameViews()
{
	return S_OK;
}

bool FXStudioView::VLoadGameDelegate(tinyxml2::XMLElement* pCameraNode)
{
	if (!HumanView::VLoadGameDelegate(pCameraNode))
		return false;

	Vector3 position(0.0f, 0.0f, 0.0f);
	float yaw = 0.0f;
	float pitch = 0.0f;
	float roll = 0.0f;

	if (pCameraNode != nullptr)
	{
		tinyxml2::XMLElement* pPositionElement = pCameraNode->FirstChildElement("Translation");
		if (pPositionElement)
		{
			position.x = pPositionElement->FloatAttribute("x");
			position.y = pPositionElement->FloatAttribute("y");
			position.z = pPositionElement->FloatAttribute("z");
		}

		tinyxml2::XMLElement* pRotationElement = pCameraNode->FirstChildElement("Rotation");
		if (pRotationElement)
		{
			yaw = pRotationElement->FloatAttribute("y");
			pitch = pRotationElement->FloatAttribute("x");
// 			roll = pRotationElement->FloatAttribute("z");
		}
	}

	m_pEditorCamera->VSetTransform(Matrix::CreateFromYawPitchRoll(yaw, pitch, roll) * Matrix::CreateTranslation(position));

	m_pModelController.reset(DEBUG_NEW ModelController(m_pEditorCamera, m_pGizmosNode, position, yaw, pitch));
	m_pCamera->ClearTarget();

	m_pPointerHandler = m_pModelController;
	m_pKeyboardHandler = m_pModelController;

	m_pScene->VOnInitScreenElements();
	return true;
}
