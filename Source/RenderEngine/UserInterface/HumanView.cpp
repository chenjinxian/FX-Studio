#include "HumanView.h"
#include "UserInterface.h"
#include "../Utilities/String.h"
#include "../Graphics3D/SceneNode.h"
#include <DXUTGui.h>

const unsigned int SCREEN_REFRESH_RATE(1000/60);
const GameViewId gc_InvalidGameViewId = 0xffffffff;

HumanView::HumanView(shared_ptr<IRenderer> pRender) : m_pRender(pRender)
{
	m_PointerRadius = 1;	// we assume we are on a mouse enabled machine - if this were a tablet we should detect it here.
	m_ViewId = gc_InvalidGameViewId;

	// Added post press for move, new, and destroy actor events and others
// 	RegisterAllDelegates();

	if (pRender)
	{
// 		m_pScene.reset(GCC_NEW ScreenElementScene(renderer));
// 
// 		Frustum frustum;
// 		frustum.Init(GCC_PI/4.0f, 1.0f, 1.0f, 100.0f);
// 		m_pCamera.reset(GCC_NEW CameraNode(&Mat4x4::g_Identity, frustum));
// 		GCC_ASSERT(m_pScene && m_pCamera && _T("Out of memory"));
// 
// 		m_pScene->VAddChild(INVALID_ACTOR_ID, m_pCamera);
// 		m_pScene->SetCamera(m_pCamera);
	}
}


HumanView::~HumanView()
{
// 	RemoveAllDelegates();

	while (!m_ScreenElements.empty())
	{
		m_ScreenElements.pop_front();		
	}

}

bool HumanView::LoadGame(TiXmlElement* pLevelData)
{
	return VLoadGameDelegate(pLevelData);
}

void HumanView::VOnRender(double fTime, float fElapsedTime )
{
	m_currTick = timeGetTime();
	if (m_currTick == m_lastDraw)
		return;

	if( m_runFullSpeed || ( (m_currTick - m_lastDraw) > SCREEN_REFRESH_RATE) )
	{
		shared_ptr<IRenderer> pRender = MakeStrongPtr(m_pRender);
		if (pRender->VPreRender())
		{
// 			m_ScreenElements.sort(SortBy_SharedPtr_Content<IScreenElement>());

			for(ScreenElementList::iterator i=m_ScreenElements.begin(); i!=m_ScreenElements.end(); ++i)
			{
				if ( (*i)->VIsVisible() )
				{
					(*i)->VOnRender(fTime, fElapsedTime);
				}
			}

			VRenderText();

			m_lastDraw = m_currTick;
		}
		
		pRender->VPostRender();
	}
}

HRESULT HumanView::VOnRestore()
{
	HRESULT hr = S_OK;
// 	for(ScreenElementList::iterator i=m_ScreenElements.begin(); i!=m_ScreenElements.end(); ++i)
// 	{
// 		V_RETURN ( (*i)->VOnRestore() );
// 	}

	return hr;
}

HRESULT HumanView::VOnLostDevice() 
{
	HRESULT hr;

// 	for(ScreenElementList::iterator i=m_ScreenElements.begin(); i!=m_ScreenElements.end(); ++i)
// 	{
// 		V_RETURN ( (*i)->VOnLostDevice() );
// 	}

	return S_OK;
}

LRESULT CALLBACK HumanView::VOnMsgProc( AppMsg msg )
{
	// Iterate through the screen layers first
	// In reverse order since we'll send input messages to the 
	// screen on top
	for(ScreenElementList::reverse_iterator i=m_ScreenElements.rbegin(); i!=m_ScreenElements.rend(); ++i)
	{
		if ( (*i)->VIsVisible() )
		{
			if ( (*i)->VOnMsgProc( msg ) )
			{
				return 1;
			}
		}
	}

	LRESULT result = 0;
	switch (msg.m_uMsg) 
	{
		case WM_KEYDOWN:
			if (m_KeyboardHandler)
			{
				result = m_KeyboardHandler->VOnKeyDown(static_cast<const BYTE>(msg.m_wParam));
			}
			break;
	
		case WM_KEYUP:
			if (m_KeyboardHandler)
				result = m_KeyboardHandler->VOnKeyUp(static_cast<const BYTE>(msg.m_wParam));
			break;

		case WM_MOUSEMOVE:
			if (m_PointerHandler)
// 				result = m_PointerHandler->VOnPointerMove(Point(LOWORD(msg.m_lParam), HIWORD(msg.m_lParam)), 1);
			break;

		case WM_LBUTTONDOWN:
			if (m_PointerHandler)
			{
				SetCapture(msg.m_hWnd);
// 				result = m_PointerHandler->VOnPointerButtonDown(Point(LOWORD(msg.m_lParam), HIWORD(msg.m_lParam)), 1, "PointerLeft");
			}	
			break;

		case WM_LBUTTONUP:
			if (m_PointerHandler)
			{
				SetCapture(NULL);
// 				result = m_PointerHandler->VOnPointerButtonUp(Point(LOWORD(msg.m_lParam), HIWORD(msg.m_lParam)), 1, "PointerLeft");
			}
			break;

		case WM_RBUTTONDOWN:
			if (m_PointerHandler)
			{
				SetCapture(msg.m_hWnd);
// 				result = m_PointerHandler->VOnPointerButtonDown(Point(LOWORD(msg.m_lParam), HIWORD(msg.m_lParam)), 1, "PointerRight");
			}
			break;

		case WM_RBUTTONUP:
			if (m_PointerHandler)
			{
				SetCapture(NULL);
// 				result = m_PointerHandler->VOnPointerButtonUp(Point(LOWORD(msg.m_lParam), HIWORD(msg.m_lParam)), 1, "PointerRight");
			}
			break;
		case WM_CHAR:
// 			if (m_Console.IsActive())
// 			{
// 				const unsigned int oemScan = int( msg.m_lParam & ( 0xff << 16 ) ) >> 16;
// 				m_Console.HandleKeyboardInput( msg.m_wParam, MapVirtualKey( oemScan, 1 ), true );
// 			}
// 			else
// 			{
// 				//See if it was the console key.
// 				if (('~'==msg.m_wParam) || ('`'==msg.m_wParam))
// 				{
// 					m_Console.SetActive(true);
// 				}
// 			}
			break;
		default:
			return 0;
	}

	return 0;
}

void HumanView::VOnUpdate(const int deltaMilliseconds)
{
	for(ScreenElementList::iterator i=m_ScreenElements.begin(); i!=m_ScreenElements.end(); ++i)
	{
		(*i)->VOnUpdate(deltaMilliseconds);
	}
}

void HumanView::VPushElement(shared_ptr<IScreenElement> pElement)
{
	m_ScreenElements.push_front(pElement);
}

void HumanView::VRemoveElement(shared_ptr<IScreenElement> pElement)
{
	m_ScreenElements.remove(pElement);
}

void HumanView::VSetCameraOffset(const Vector4& camOffset)
{

}

// void HumanView::VSetCameraOffset(const Vec4 & camOffset )
// {
// 	GCC_ASSERT(m_pCamera);
// 	if (m_pCamera)
// 	{
// 		m_pCamera->SetCameraOffset( camOffset );
// 	}
// }

// void HumanView::RegisterAllDelegates(void)
// {
// 	IEventManager* pGlobalEventManager = IEventManager::Get();
// //	pGlobalEventManager->VAddListener(MakeDelegate(this, &HumanView::GameStateDelegate), EvtData_Game_State::sk_EventType);
// 	pGlobalEventManager->VAddListener(MakeDelegate(this, &HumanView::PlaySoundDelegate), EvtData_PlaySound::sk_EventType);
// }
// 
// void HumanView::RemoveAllDelegates(void)
// {
// 	IEventManager* pGlobalEventManager = IEventManager::Get();
// //	pGlobalEventManager->VRemoveListener(MakeDelegate(this, &HumanView::GameStateDelegate), EvtData_Game_State::sk_EventType);
// 	pGlobalEventManager->VRemoveListener(MakeDelegate(this, &HumanView::PlaySoundDelegate), EvtData_PlaySound::sk_EventType);
// }
