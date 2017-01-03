#include "stdafx.h"
#include "FXStudioView.h"
#include "FXStudioApp.h"

extern FXStudioApp globalApp;

FXStudioView::FXStudioView(shared_ptr<IRenderer> pRenderer)
	: HumanView(pRenderer),
	m_pModelController(nullptr)
{
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

	m_pModelController.reset(DEBUG_NEW ModelController(m_pScene, position, yaw, pitch));
	m_pCamera->ClearTarget();

	m_pPointerHandler = m_pModelController;
	m_pKeyboardHandler = m_pModelController;

	m_pScene->VOnInitScreenElements();
	return true;
}
