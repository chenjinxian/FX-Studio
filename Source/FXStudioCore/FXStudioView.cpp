#include "stdafx.h"
#include "FXStudioView.h"
#include "FXStudioApp.h"

extern FXStudioApp globalApp;

FXStudioView::FXStudioView(shared_ptr<IRenderer> pRenderer) : HumanView(pRenderer)
{
}


FXStudioView::~FXStudioView()
{
}

void FXStudioView::VOnUpdate(const GameTime& gameTime)
{
	HumanView::VOnUpdate(gameTime);
}

bool FXStudioView::VLoadGameDelegate(tinyxml2::XMLElement* pLevelData)
{
	if (!HumanView::VLoadGameDelegate(pLevelData))
		return false;

	m_pScene->VOnInitScreenElements();
	return true;
}
