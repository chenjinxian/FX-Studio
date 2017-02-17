#include "stdafx.h"
#include "FXStudioView.h"
#include "FXStudioApp.h"
#include "FXStudioEvent.h"
#include "imgui.h"
#include "imgui_internal.h"

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

	float pitch = 0.0f;
	float yaw = 0.0f;
	float roll = 0.0f;
	Utility::QuaternionToAngle(quat, yaw, pitch, roll);

	tinyxml2::XMLElement* pRotationElement = pDocument->NewElement("Rotation");
	pRotationElement->SetAttribute("x", pitch);
	pRotationElement->SetAttribute("y", yaw);
	pRotationElement->SetAttribute("z", roll);
	pBaseElement->InsertEndChild(pRotationElement);

	return pBaseElement;
}

void FXStudioView::SetCameraType(int type)
{
	m_pModelController->SetCameraType((CameraType)type);
}

void FXStudioView::VRenderText(const GameTime& gameTime)
{
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(500, 100));

	if (!ImGui::Begin("Device State", nullptr, ImVec2(0, 0), 0.0f,
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings))
	{
		ImGui::End();
		return;
	}

	ImGui::Text(g_pApp->GetRendererAPI()->VGetDeviceName().c_str());
	ImGui::Text("%d (fps)", gameTime.GetFrameRate());

	ImGui::End();
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
			roll = pRotationElement->FloatAttribute("z");
		}
	}

	m_pModelController.reset(DEBUG_NEW ModelController(m_pEditorCamera, m_pGizmosNode, position, yaw, pitch));
	m_pEditorCamera->ClearTarget();

	m_pPointerHandler = m_pModelController;
	m_pKeyboardHandler = m_pModelController;

	m_pScene->VOnInitScreenElements();
	return true;
}

HRESULT FXStudioView::VOnInitGameViews(bool onlyCamera)
{
	HumanView::VOnInitGameViews(onlyCamera);

	if (onlyCamera)
	{
		if (m_pEditorCamera != nullptr)
			m_pEditorCamera->VOnInitSceneNode(m_pScene.get());
	}

	return S_OK;
}

HRESULT FXStudioView::VOnDeleteGameViews(bool onlyCamera)
{
	HumanView::VOnDeleteGameViews(onlyCamera);

	if (onlyCamera)
	{
		if (m_pEditorCamera != nullptr)
			m_pEditorCamera->VOnDeleteSceneNode(m_pScene.get());
	}

	return S_OK;
}
