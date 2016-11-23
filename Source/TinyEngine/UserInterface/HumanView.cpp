#include "HumanView.h"
#include "../AppFramework/BaseGameApp.h"
#include "../Actors/RenderComponentInterface.h"
#include "../EventManager/Events.h"
#include "../EventManager/EventManagerImpl.h"
#include "../Graphics3D/Scene.h"
#include "../Graphics3D/CameraNode.h"
#include "../Graphics3D/MovementController.h"
#include "imgui.h"

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

void HumanView::VOnRender(const GameTime& gameTime)
{
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2((float)g_pApp->m_Config.m_ScreenWidth, (float)g_pApp->m_Config.m_ScreenHeight);
	io.DeltaTime = gameTime.GetElapsedTime();
	ImGui::NewFrame();

	if (g_pApp->m_pRenderer->VPreRender())
	{
		m_ScreenElements.sort(SortBy_SharedPtr_Content<IScreenElement>());

		for (ScreenElementList::iterator i = m_ScreenElements.begin(); i != m_ScreenElements.end(); ++i)
		{
			if ((*i)->VIsVisible())
			{
				(*i)->VOnRender(gameTime);
			}
		}

		VRenderText();

		ImGui::Render();
	}

	g_pApp->m_pRenderer->VPostRender();
}

HRESULT HumanView::VOnInitGameViews()
{
	HRESULT hr = S_OK;
	for (ScreenElementList::iterator i = m_ScreenElements.begin(); i != m_ScreenElements.end(); ++i)
	{
		(*i)->VOnInitScreenElements();
	}

	return hr;
}

HRESULT HumanView::VOnDeleteGameViews()
{
// 	HRESULT hr;

	for (ScreenElementList::iterator i = m_ScreenElements.begin(); i != m_ScreenElements.end(); ++i)
	{
		(*i)->VOnDeleteScreenElements();
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
				return true;
			}
		}
	}

	ImGuiIO& io = ImGui::GetIO();

	LRESULT result = 0;
	switch (msg.m_uMsg)
	{
	case WM_KEYDOWN:
		if (msg.m_wParam < 256)
			io.KeysDown[msg.m_wParam] = 1;
		if (m_pKeyboardHandler)
		{
			result = m_pKeyboardHandler->VOnKeyDown(static_cast<uint8_t>(msg.m_wParam));
		}
		break;

	case WM_KEYUP:
		if (msg.m_wParam < 256)
			io.KeysDown[msg.m_wParam] = 0;
		if (m_pKeyboardHandler)
			result = m_pKeyboardHandler->VOnKeyUp(static_cast<uint8_t>(msg.m_wParam));
		break;

	case WM_MOUSEMOVE:
		io.MousePos.x = LOWORD(msg.m_lParam);
		io.MousePos.y = HIWORD(msg.m_lParam);
		if (m_pPointerHandler)
			result = m_pPointerHandler->VOnPointerMove(Vector2(LOWORD(msg.m_lParam), HIWORD(msg.m_lParam)), 1);
		break;

	case WM_MOUSEWHEEL:
		io.MouseWheel += GET_WHEEL_DELTA_WPARAM(msg.m_wParam) > 0 ? +1.0f : -1.0f;
		break;

	case WM_LBUTTONDOWN:
		io.MouseDown[0] = true;
		if (m_pPointerHandler)
		{
			SetCapture(msg.m_hWnd);
			result = m_pPointerHandler->VOnPointerButtonDown(Vector2(LOWORD(msg.m_lParam), HIWORD(msg.m_lParam)), 1, "PointerLeft");
		}
		break;

	case WM_LBUTTONUP:
		io.MouseDown[0] = false;
		if (m_pPointerHandler)
		{
			SetCapture(NULL);
			result = m_pPointerHandler->VOnPointerButtonUp(Vector2(LOWORD(msg.m_lParam), HIWORD(msg.m_lParam)), 1, "PointerLeft");
		}
		break;

	case WM_RBUTTONDOWN:
		io.MouseDown[1] = true;
		if (m_pPointerHandler)
		{
			SetCapture(msg.m_hWnd);
			result = m_pPointerHandler->VOnPointerButtonDown(Vector2(LOWORD(msg.m_lParam), HIWORD(msg.m_lParam)), 1, "PointerRight");
		}
		break;

	case WM_RBUTTONUP:
		io.MouseDown[1] = false;
		if (m_pPointerHandler)
		{
			SetCapture(NULL);
			result = m_pPointerHandler->VOnPointerButtonUp(Vector2(LOWORD(msg.m_lParam), HIWORD(msg.m_lParam)), 1, "PointerRight");
		}
		break;

	case WM_MBUTTONDOWN:
		io.MouseDown[2] = true;
		break;

	case WM_MBUTTONUP:
		io.MouseDown[2] = false;
		break;

	case WM_CHAR:
		if (msg.m_wParam > 0 && msg.m_wParam < 0x10000)
			io.AddInputCharacter((unsigned short)msg.m_wParam);
		break;

	default:
		return 0;
	}

	return 0;
}

void HumanView::VOnUpdate(const GameTime& gameTime)
{
	for (ScreenElementList::iterator i = m_ScreenElements.begin(); i != m_ScreenElements.end(); ++i)
	{
		(*i)->VOnUpdate(gameTime);
	}

	if (m_pFreeCameraController)
	{
		m_pFreeCameraController->OnUpdate(gameTime);
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
