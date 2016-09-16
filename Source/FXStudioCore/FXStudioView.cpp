#include "stdafx.h"
#include "FXStudioView.h"


FXStudioView::FXStudioView(shared_ptr<IRenderer> pRenderer) : HumanView(pRenderer)
{
}


FXStudioView::~FXStudioView()
{
}

void FXStudioView::VOnUpdate(uint32_t deltaMilliseconds)
{

}

bool FXStudioView::VLoadGameDelegate(TiXmlElement* pProjectData)
{
	return true;
}
