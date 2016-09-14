#include "stdafx.h"
#include "FXStudioHumanView.h"

FXStudioHumanView::FXStudioHumanView(shared_ptr<IRenderer> pRender) : HumanView(pRender), m_pCameraController(nullptr)
{
}

FXStudioHumanView::~FXStudioHumanView()
{
}

void FXStudioHumanView::VOnUpdate(unsigned long deltaMilliseconds)
{
	HumanView::VOnUpdate(deltaMilliseconds);
	if (m_pCameraController != nullptr)
	{
		m_pCameraController->OnUpdate(deltaMilliseconds);
	}
}

bool FXStudioHumanView::VLoadGameDelegate(TiXmlElement* pLevelData)
{
	if (!HumanView::VLoadGameDelegate(pLevelData))
		return false;
	
	m_pCamera->ClearTarget();

	return true;
}
