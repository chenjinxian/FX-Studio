#include "DemoView.h"
#include "DemoApp.h"
#include "imgui.h"

#define CID_START_BUTTON				(8)
#define CID_LEVEL_LISTBOX				(16)

const int g_SampleUIWidth = 600;
const int g_SampleUIHeight = 600;

DemoView::DemoView(shared_ptr<IRenderer> renderer) : HumanView(renderer)
{
	m_bShowUI = true;
	RegisterAllDelegates();
}


DemoView::~DemoView()
{
	RemoveAllDelegates();
}

LRESULT CALLBACK DemoView::VOnMsgProc(AppMsg msg)
{
	if (HumanView::VOnMsgProc(msg))
		return 1;

	if (msg.m_uMsg == WM_KEYDOWN)
	{
		if (msg.m_wParam == VK_F1)
		{
			m_bShowUI = !m_bShowUI;
			return 1;
		}
		else if (msg.m_wParam == VK_F2)
		{
// 			POINT ptCursor;
// 			GetCursorPos(&ptCursor);
// 			ScreenToClient(g_pApp->GetHwnd(), &ptCursor);
		}
		else if (msg.m_wParam == 'Q' || msg.m_wParam == VK_ESCAPE)
		{
// 			if (MessageBox::Ask(QUESTION_QUIT_GAME) == IDYES)
// 			{
// 				g_pApp->SetQuitting(true);
// 			}
			return 1;
		}
	}

	return 0;
}

void DemoView::VRenderText()
{
	ImGui::Text("Hello, world!");
}

void DemoView::VOnUpdate(const GameTime& gameTime)
{
	HumanView::VOnUpdate(gameTime);

// 	if (m_pTeapotController)
// 	{
// 		m_pTeapotController->OnUpdate(deltaMs);
// 	}

	shared_ptr<EvtData_Update_Tick> pTickEvent(DEBUG_NEW EvtData_Update_Tick(gameTime.GetElapsedTime()));
	IEventManager::Get()->VTriggerEvent(pTickEvent);
}

void DemoView::VOnAttach(GameViewId vid, ActorId aid)
{
	HumanView::VOnAttach(vid, aid);
}

void DemoView::VSetControlledActor(ActorId actorId)
{
// 	m_pTeapot = static_pointer_cast<SceneNode>(m_pScene->FindActor(actorId));
// 	if (!m_pTeapot)
// 	{
// 		GCC_ERROR("Invalid teapot");
// 		return;
// 	}
// 
// 	HumanView::VSetControlledActor(actorId);
// 
// 	m_pTeapotController.reset(DEBUG_NEW TeapotController(m_pTeapot));
// 	m_KeyboardHandler = m_pTeapotController;
// 	m_PointerHandler = m_pTeapotController;
// 	m_pCamera->SetTarget(m_pTeapot);
// 	m_pTeapot->SetAlpha(0.8f);
}

bool DemoView::VLoadGameDelegate(tinyxml2::XMLElement* pLevelData)
{
	if (!HumanView::VLoadGameDelegate(pLevelData))
		return false;

// 	m_StandardHUD.reset(DEBUG_NEW StandardHUD);
// 	VPushElement(m_StandardHUD);

// 	m_pFreeCameraController.reset(DEBUG_NEW MovementController(m_pCamera, 0, 0, false));

	HumanView::VOnInitGameViews();
	return true;
}

void DemoView::GameplayUiUpdateDelegate(IEventDataPtr pEventData)
{
// 	shared_ptr<EvtData_Gameplay_UI_Update> pCastEventData = static_pointer_cast<EvtData_Gameplay_UI_Update>(pEventData);
// 	if (!pCastEventData->GetUiString().empty())
// 		m_GameplayText = s2ws(pCastEventData->GetUiString());
// 	else
// 		m_GameplayText.clear();
}

void DemoView::SetControlledActorDelegate(IEventDataPtr pEventData)
{
// 	shared_ptr<EvtData_Set_Controlled_Actor> pCastEventData = static_pointer_cast<EvtData_Set_Controlled_Actor>(pEventData);
// 	VSetControlledActor(pCastEventData->GetActorId());
}

void DemoView::RegisterAllDelegates(void)
{
// 	IEventManager* pGlobalEventManager = IEventManager::Get();
// 	pGlobalEventManager->VAddListener(MakeDelegate(this, &TeapotWarsHumanView::GameplayUiUpdateDelegate), EvtData_Gameplay_UI_Update::sk_EventType);
// 	pGlobalEventManager->VAddListener(MakeDelegate(this, &TeapotWarsHumanView::SetControlledActorDelegate), EvtData_Set_Controlled_Actor::sk_EventType);
}

void DemoView::RemoveAllDelegates(void)
{
// 	IEventManager* pGlobalEventManager = IEventManager::Get();
// 	pGlobalEventManager->VRemoveListener(MakeDelegate(this, &TeapotWarsHumanView::GameplayUiUpdateDelegate), EvtData_Gameplay_UI_Update::sk_EventType);
// 	pGlobalEventManager->VRemoveListener(MakeDelegate(this, &TeapotWarsHumanView::SetControlledActorDelegate), EvtData_Set_Controlled_Actor::sk_EventType);
}

MainMenuUI::MainMenuUI()
{
// 	Set();
}

MainMenuUI::~MainMenuUI()
{
}

HRESULT MainMenuUI::VOnInitScreenElements()
{
	return S_OK;
}

HRESULT MainMenuUI::VOnDeleteScreenElements()
{
	return S_OK;
}

HRESULT MainMenuUI::VOnRender(const GameTime& gameTime)
{
// 	HRESULT hr;
// 	DXUT_BeginPerfEvent(DXUT_PERFEVENTCOLOR, L"ShootGameHUD");
// 	VTrace(m_SampleUI.OnRender(fElapsedTime));
// 	DXUT_EndPerfEvent();
	return S_OK;
}

LRESULT CALLBACK MainMenuUI::VOnMsgProc(AppMsg msg)
{
// 	return m_SampleUI.MsgProc(msg.m_hWnd, msg.m_uMsg, msg.m_wParam, msg.m_lParam);
	return S_OK;
}

void MainMenuUI::Set()
{
// 	m_LevelIndex = m_SampleUI.GetListBox(CID_LEVEL_LISTBOX)->GetSelectedIndex();
}

MainMenuView::MainMenuView() : HumanView(shared_ptr<IRenderer>())
{
	m_MainMenuUI.reset(DEBUG_NEW MainMenuUI);
	VPushElement(m_MainMenuUI);
}

MainMenuView::~MainMenuView()
{

}

void MainMenuView::VRenderText()
{
	HumanView::VRenderText();
}

void MainMenuView::VOnUpdate(const GameTime& gameTime)
{
	HumanView::VOnUpdate(gameTime);
}

LRESULT CALLBACK MainMenuView::VOnMsgProc(AppMsg msg)
{
	if (m_MainMenuUI->VIsVisible())
	{
		if (HumanView::VOnMsgProc(msg))
			return 1;
	}
	return 0;
}
