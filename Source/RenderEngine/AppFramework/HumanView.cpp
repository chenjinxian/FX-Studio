#include "HumanView.h"
#include "../Utilities/Templates.h"

HumanView::HumanView(shared_ptr<IRenderer> pRenderer)
	: m_CurrentTick(0), m_LastDraw(0), m_pRenderer(pRenderer),
	m_ViewId(INVALID_GAME_VIEW_ID), m_ActorId(INVALID_ACTOR_ID)
{
	m_pScene.reset(GCC_NEW SceenElementScene(pRenderer));
}


HumanView::~HumanView()
{
}

HRESULT HumanView::VOnRestore()
{
	HRESULT hr = S_OK;
	for (auto element : m_SceenElements)
	{
		V_RETURN(element->VOnRestore());
	}

	return hr;
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
		for (auto element : m_SceenElements)
		{
			if (element->VIsVisible())
			{
				element->VOnRender(totalTime, elapsedTime);
			}
		}

		m_LastDraw = m_CurrentTick;
	}
	pRenderer->VPostRender();
}

void HumanView::VOnAttach(GameViewId viewId, ActorId actorId)
{
	m_ViewId = viewId;
	m_ActorId = actorId;
}

void HumanView::VPushElement(shared_ptr<ISceenElement> pElement)
{
	m_SceenElements.push_front(pElement);
}

void HumanView::VRemoveElement(shared_ptr<ISceenElement> pElement)
{
	m_SceenElements.remove(pElement);
}
