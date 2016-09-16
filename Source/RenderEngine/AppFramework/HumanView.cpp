#include "HumanView.h"
#include "../Utilities/Templates.h"

HumanView::HumanView(shared_ptr<IRenderer> pRenderer)
	: m_CurrentTick(0), m_LastDraw(0), m_pRenderer(pRenderer)
{
	m_pScene.reset(GCC_NEW SceenElementScene(pRenderer));
}


HumanView::~HumanView()
{
}

void HumanView::VOnUpdate(uint32_t deltaMilliseconds)
{
	for (auto element : m_SceenElements)
	{
		element->VOnUpdate(deltaMilliseconds);
	}
}

void HumanView::VOnRender(float totalTime, float elapsedTime)
{
	m_CurrentTick = GetTickCount();
	if (m_CurrentTick == m_LastDraw)
	{
		return;
	}

	shared_ptr<IRenderer> pRenderer = MakeStrongPtr(m_pRenderer);
	if (pRenderer->VPreRender())
	{
		m_LastDraw = m_CurrentTick;
	}
	pRenderer->VPostRender();
}

void HumanView::VPushElement(shared_ptr<ISceenElement> pElement)
{
	m_SceenElements.push_front(pElement);
}

void HumanView::VRemoveElement(shared_ptr<ISceenElement> pElement)
{
	m_SceenElements.remove(pElement);
}
