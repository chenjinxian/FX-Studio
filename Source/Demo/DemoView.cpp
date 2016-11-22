#include "DemoView.h"
#include "DemoApp.h"

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
// 	if (!D3DRenderer::m_pTextHelper)
// 		return;
// 
// 	HumanView::VRenderText();
// 
// 	D3DRenderer::m_pTextHelper->Begin();
// 
// 	if (!m_GameplayText.empty())
// 	{
// 		D3DRenderer::m_pTextHelper->SetInsertionPos(g_pApp->GetScreenWidth() / 2, 5);
// 		D3DRenderer::m_pTextHelper->SetForegroundColor(Color(0.0f, 0.0f, 0.0f, 1.0f));
// 		D3DRenderer::m_pTextHelper->DrawTextLine(m_GameplayText.c_str());
// 		D3DRenderer::m_pTextHelper->SetInsertionPos(g_pApp->GetScreenWidth() / 2 - 1, 5 - 1);
// 		D3DRenderer::m_pTextHelper->SetForegroundColor(Color(0.25f, 1.0f, 0.25f, 1.0f));
// 		D3DRenderer::m_pTextHelper->DrawTextLine(m_GameplayText.c_str());
// 	}
// 
// 	if (m_bShowUI)
// 	{
// 		D3DRenderer::m_pTextHelper->SetInsertionPos(5, 5);
// 		D3DRenderer::m_pTextHelper->SetForegroundColor(Color(1.0f, 1.0f, 0.0f, 1.0f));
// 		D3DRenderer::m_pTextHelper->DrawTextLine(DXUTGetFrameStats(DXUTIsVsyncEnabled()));
// 		D3DRenderer::m_pTextHelper->DrawTextLine(DXUTGetDeviceStats());
// 		D3DRenderer::m_pTextHelper->SetForegroundColor(Color(0.0f, 0.0f, 0.0f, 0.5f));
// 
// 		switch (m_GameState)
// 		{
// 		case BGS_Initializing:
// 			D3DRenderer::m_pTextHelper->DrawTextLine(g_pApp->GetString(_T("IDS_INITIALIZING")).c_str());
// 			break;
// 
// 		case BGS_MainMenu:
// 			D3DRenderer::m_pTextHelper->DrawTextLine(L"Main Menu");
// 			break;
// 
// 		case BGS_WaitingForPlayers:
// 			D3DRenderer::m_pTextHelper->DrawTextLine(g_pApp->GetString(_T("IDS_WAITING")).c_str());
// 			break;
// 
// 		case BGS_LoadingGameEnvironment:
// 			D3DRenderer::m_pTextHelper->DrawTextLine(g_pApp->GetString(_T("IDS_LOADING")).c_str());
// 			break;
// 
// 		case BGS_Running:
// #ifndef DISABLE_PHYSICS
// 			D3DRenderer::m_pTextHelper->DrawTextLine(g_pApp->GetString(_T("IDS_RUNNING")).c_str());
// #else
// 			D3DRenderer::m_pTextHelper->DrawTextLine(g_pApp->GetString(_T("IDS_NOPHYSICS")).c_str());
// #endif //!DISABLE_PHYSICS
// 			break;
// 		}
// 	}
// 
// 	D3DRenderer::m_pTextHelper->End();
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

	m_StandardHUD.reset(DEBUG_NEW StandardHUD);
	VPushElement(m_StandardHUD);

// 	m_pFreeCameraController.reset(DEBUG_NEW MovementController(m_pCamera, 0, 0, false));

	HumanView::VOnRestore();
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

HRESULT MainMenuUI::VOnRestore()
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

StandardHUD::StandardHUD()
{
}

StandardHUD::~StandardHUD()
{

}

HRESULT StandardHUD::VOnRestore()
{
	return S_OK;
}

HRESULT StandardHUD::VOnRender(const GameTime& gameTime)
{
	return S_OK;
}

LRESULT CALLBACK StandardHUD::VOnMsgProc(AppMsg msg)
{
// 	return m_HUD.MsgProc(msg.m_hWnd, msg.m_uMsg, msg.m_wParam, msg.m_lParam);
	return S_OK;
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
