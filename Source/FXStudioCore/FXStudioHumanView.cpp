#include "stdafx.h"
#include "FXStudioHumanView.h"


FXStudioHumanView::FXStudioHumanView(shared_ptr<IRenderer> pRender) : HumanView(pRender)
{
}

FXStudioHumanView::~FXStudioHumanView()
{
}

void FXStudioHumanView::VOnUpdate(unsigned long deltaMilliseconds)
{
	HumanView::VOnUpdate(deltaMilliseconds);
}

bool FXStudioHumanView::VLoadGameDelegate(TiXmlElement* pLevelData)
{
	if (!HumanView::VLoadGameDelegate(pLevelData))
		return false;

	return true;
}
