#include "HumanView.h"
#include "../AppFramework/BaseGameApp.h"
#include "../Actors/RenderComponentInterface.h"
#include "../EventManager/Events.h"
#include "../EventManager/EventManagerImpl.h"
#include "../Graphics3D/Scene.h"
#include "../Graphics3D/CameraNode.h"
#include "../Graphics3D/MovementController.h"

const uint32_t SCREEN_REFRESH_RATE(1000 / 60);

HumanView::HumanView(shared_ptr<IRenderer> renderer)
{
	InitAudio();

	m_PointerRadius = 1;
	m_ViewId = INVALID_GAME_VIEW_ID;

	RegisterAllDelegates();
	m_GameState = BGS_Initializing;

	if (renderer)
	{
		m_pScene.reset(DEBUG_NEW ScreenElementScene(renderer));

		Frustum frustum;
		frustum.Init(XM_PIDIV4, 1.0f, 0.01f, 1000.0f);
		m_pCamera.reset(DEBUG_NEW CameraNode(frustum));
		DEBUG_ASSERT(m_pScene && m_pCamera && _T("Out of memory"));

		m_pScene->VAddChild(INVALID_ACTOR_ID, m_pCamera);
		m_pScene->SetCamera(m_pCamera);
	}

	m_pPointerHandler = nullptr;
	m_pKeyboardHandler = nullptr;
	m_pFreeCameraController = nullptr;
}


HumanView::~HumanView()
{
	RemoveAllDelegates();

	while (!m_ScreenElements.empty())
	{
		m_ScreenElements.pop_front();
	}
}

bool HumanView::LoadGame(tinyxml2::XMLElement* pLevelData)
{
	m_pFreeCameraController.reset(DEBUG_NEW MovementController(m_pCamera, 0, 0, true));
	m_pPointerHandler = m_pFreeCameraController;
	m_pKeyboardHandler = m_pFreeCameraController;
	return VLoadGameDelegate(pLevelData);
}

void HumanView::VOnRender(double fTime, float fElapsedTime)
{
	m_currTick = GetTickCount();
	if (m_currTick == m_lastDraw)
		return;

	if (m_runFullSpeed || ((m_currTick - m_lastDraw) > SCREEN_REFRESH_RATE))
	{
		if (g_pApp->m_pRenderer->VPreRender())
		{
			m_ScreenElements.sort(SortBy_SharedPtr_Content<IScreenElement>());

			for (ScreenElementList::iterator i = m_ScreenElements.begin(); i != m_ScreenElements.end(); ++i)
			{
				if ((*i)->VIsVisible())
				{
					(*i)->VOnRender(fTime, fElapsedTime);
				}
			}

			VRenderText();

			m_lastDraw = m_currTick;
		}

		g_pApp->m_pRenderer->VPostRender();
	}
}

HRESULT HumanView::VOnRestore()
{
	HRESULT hr = S_OK;
	for (ScreenElementList::iterator i = m_ScreenElements.begin(); i != m_ScreenElements.end(); ++i)
	{
		(*i)->VOnRestore();
	}

	return hr;
}

HRESULT HumanView::VOnDestoryDevice()
{
// 	HRESULT hr;

	for (ScreenElementList::iterator i = m_ScreenElements.begin(); i != m_ScreenElements.end(); ++i)
	{
		(*i)->VOnDestoryDevice();
	}

	return S_OK;
}

bool HumanView::InitAudio()
{
	return true;
}

void HumanView::TogglePause(bool active)
{
}

LRESULT CALLBACK HumanView::VOnMsgProc(AppMsg msg)
{
	for (ScreenElementList::reverse_iterator i = m_ScreenElements.rbegin(); i != m_ScreenElements.rend(); ++i)
	{
		if ((*i)->VIsVisible())
		{
			if ((*i)->VOnMsgProc(msg))
			{
				return 1;
			}
		}
	}

	LRESULT result = 0;
	switch (msg.m_uMsg)
	{
	case WM_KEYDOWN:
		if (m_pKeyboardHandler)
		{
			result = m_pKeyboardHandler->VOnKeyDown(static_cast<uint8_t>(msg.m_wParam));
		}
		break;

	case WM_KEYUP:
		if (m_pKeyboardHandler)
			result = m_pKeyboardHandler->VOnKeyUp(static_cast<uint8_t>(msg.m_wParam));
		break;

	case WM_MOUSEMOVE:
		if (m_pPointerHandler)
			result = m_pPointerHandler->VOnPointerMove(Vector2(LOWORD(msg.m_lParam), HIWORD(msg.m_lParam)), 1);
		break;

	case WM_LBUTTONDOWN:
		if (m_pPointerHandler)
		{
			SetCapture(msg.m_hWnd);
			result = m_pPointerHandler->VOnPointerButtonDown(Vector2(LOWORD(msg.m_lParam), HIWORD(msg.m_lParam)), 1, "PointerLeft");
		}
		break;

	case WM_LBUTTONUP:
		if (m_pPointerHandler)
		{
			SetCapture(NULL);
			result = m_pPointerHandler->VOnPointerButtonUp(Vector2(LOWORD(msg.m_lParam), HIWORD(msg.m_lParam)), 1, "PointerLeft");
		}
		break;

	case WM_RBUTTONDOWN:
		if (m_pPointerHandler)
		{
			SetCapture(msg.m_hWnd);
			result = m_pPointerHandler->VOnPointerButtonDown(Vector2(LOWORD(msg.m_lParam), HIWORD(msg.m_lParam)), 1, "PointerRight");
		}
		break;

	case WM_RBUTTONUP:
		if (m_pPointerHandler)
		{
			SetCapture(NULL);
			result = m_pPointerHandler->VOnPointerButtonUp(Vector2(LOWORD(msg.m_lParam), HIWORD(msg.m_lParam)), 1, "PointerRight");
		}
		break;
	case WM_CHAR:
		break;
	default:
		return 0;
	}

	return 0;
}

void HumanView::VOnUpdate(double fTime, float fElapsedTime)
{
	for (ScreenElementList::iterator i = m_ScreenElements.begin(); i != m_ScreenElements.end(); ++i)
	{
		(*i)->VOnUpdate(fTime, fElapsedTime);
	}

	if (m_pFreeCameraController)
	{
		m_pFreeCameraController->OnUpdate(fTime, fElapsedTime);
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
	DEBUG_ASSERT(m_pCamera);
	if (m_pCamera)
	{
		m_pCamera->SetCameraOffset(camOffset);
	}
}

void HumanView::RegisterAllDelegates(void)
{
	// 	IEventManager* pGlobalEventManager = IEventManager::Get();
	//	pGlobalEventManager->VAddListener(MakeDelegate(this, &HumanView::GameStateDelegate), EvtData_Game_State::sk_EventType);
	// 	pGlobalEventManager->VAddListener(MakeDelegate(this, &HumanView::PlaySoundDelegate), EvtData_PlaySound::sk_EventType);
}

void HumanView::RemoveAllDelegates(void)
{
	// 	IEventManager* pGlobalEventManager = IEventManager::Get();
	//	pGlobalEventManager->VRemoveListener(MakeDelegate(this, &HumanView::GameStateDelegate), EvtData_Game_State::sk_EventType);
	// 	pGlobalEventManager->VRemoveListener(MakeDelegate(this, &HumanView::PlaySoundDelegate), EvtData_PlaySound::sk_EventType);
}

void HumanView::PlaySoundDelegate(IEventDataPtr pEventData)
{
}

void HumanView::GameStateDelegate(IEventDataPtr pEventData)
{
}
