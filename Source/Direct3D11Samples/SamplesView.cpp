#include "SamplesView.h"
#include "SamplesApp.h"

extern SamplesApp* g_GameApp;

#define CID_START_BUTTON				(8)
#define CID_LEVEL_LISTBOX				(16)

const int g_SampleUIWidth = 600;
const int g_SampleUIHeight = 600;

SamplesView::SamplesView(shared_ptr<IRenderer> renderer) : HumanView(renderer)
{
	m_bShowUI = true;
	RegisterAllDelegates();
}


SamplesView::~SamplesView()
{
	RemoveAllDelegates();
}

LRESULT CALLBACK SamplesView::VOnMsgProc(AppMsg msg)
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
			POINT ptCursor;
			GetCursorPos(&ptCursor);
			ScreenToClient(g_pApp->GetHwnd(), &ptCursor);
		}
		else if (msg.m_wParam == 'Q' || msg.m_wParam == VK_ESCAPE)
		{
			if (MessageBox::Ask(QUESTION_QUIT_GAME) == IDYES)
			{
				g_pApp->SetQuitting(true);
			}
			return 1;
		}
	}

	return 0;
}

void SamplesView::VRenderText()
{
	if (!D3DRenderer::m_pTextHelper)
		return;

	HumanView::VRenderText();

	D3DRenderer::m_pTextHelper->Begin();

	if (!m_GameplayText.empty())
	{
		D3DRenderer::m_pTextHelper->SetInsertionPos(g_pApp->GetScreenWidth() / 2, 5);
		D3DRenderer::m_pTextHelper->SetForegroundColor(Color(0.0f, 0.0f, 0.0f, 1.0f));
		D3DRenderer::m_pTextHelper->DrawTextLine(m_GameplayText.c_str());
		D3DRenderer::m_pTextHelper->SetInsertionPos(g_pApp->GetScreenWidth() / 2 - 1, 5 - 1);
		D3DRenderer::m_pTextHelper->SetForegroundColor(Color(0.25f, 1.0f, 0.25f, 1.0f));
		D3DRenderer::m_pTextHelper->DrawTextLine(m_GameplayText.c_str());
	}

	if (m_bShowUI)
	{
		D3DRenderer::m_pTextHelper->SetInsertionPos(5, 5);
		D3DRenderer::m_pTextHelper->SetForegroundColor(Color(1.0f, 1.0f, 0.0f, 1.0f));
		D3DRenderer::m_pTextHelper->DrawTextLine(DXUTGetFrameStats(DXUTIsVsyncEnabled()));
		D3DRenderer::m_pTextHelper->DrawTextLine(DXUTGetDeviceStats());
		D3DRenderer::m_pTextHelper->SetForegroundColor(Color(0.0f, 0.0f, 0.0f, 0.5f));

		switch (m_GameState)
		{
		case BGS_Initializing:
			D3DRenderer::m_pTextHelper->DrawTextLine(g_pApp->GetString(_T("IDS_INITIALIZING")).c_str());
			break;

		case BGS_MainMenu:
			D3DRenderer::m_pTextHelper->DrawTextLine(L"Main Menu");
			break;

		case BGS_WaitingForPlayers:
			D3DRenderer::m_pTextHelper->DrawTextLine(g_pApp->GetString(_T("IDS_WAITING")).c_str());
			break;

		case BGS_LoadingGameEnvironment:
			D3DRenderer::m_pTextHelper->DrawTextLine(g_pApp->GetString(_T("IDS_LOADING")).c_str());
			break;

		case BGS_Running:
#ifndef DISABLE_PHYSICS
			D3DRenderer::m_pTextHelper->DrawTextLine(g_pApp->GetString(_T("IDS_RUNNING")).c_str());
#else
			D3DRenderer::m_pTextHelper->DrawTextLine(g_pApp->GetString(_T("IDS_NOPHYSICS")).c_str());
#endif //!DISABLE_PHYSICS
			break;
		}
	}

	D3DRenderer::m_pTextHelper->End();
}

void SamplesView::VOnUpdate(double fTime, float fElapsedTime)
{
	HumanView::VOnUpdate(fTime, fElapsedTime);

// 	if (m_pFreeCameraController)
// 	{
// 		m_pFreeCameraController->OnUpdate(deltaMs);
// 	}
// 
// 	if (m_pTeapotController)
// 	{
// 		m_pTeapotController->OnUpdate(deltaMs);
// 	}
// 
// 	shared_ptr<EvtData_Update_Tick> pTickEvent(GCC_NEW EvtData_Update_Tick(deltaMs));
// 	IEventManager::Get()->VTriggerEvent(pTickEvent);
}

void SamplesView::VOnAttach(GameViewId vid, ActorId aid)
{
	HumanView::VOnAttach(vid, aid);
}

void SamplesView::VSetControlledActor(ActorId actorId)
{
	m_pTeapot = static_pointer_cast<SceneNode>(m_pScene->FindActor(actorId));
// 	if (!m_pTeapot)
// 	{
// 		GCC_ERROR("Invalid teapot");
// 		return;
// 	}
// 
// 	HumanView::VSetControlledActor(actorId);
// 
// 	m_pTeapotController.reset(GCC_NEW TeapotController(m_pTeapot));
// 	m_KeyboardHandler = m_pTeapotController;
// 	m_PointerHandler = m_pTeapotController;
// 	m_pCamera->SetTarget(m_pTeapot);
// 	m_pTeapot->SetAlpha(0.8f);
}

bool SamplesView::VLoadGameDelegate(TiXmlElement* pLevelData)
{
	if (!HumanView::VLoadGameDelegate(pLevelData))
		return false;

	m_StandardHUD.reset(GCC_NEW StandardHUD);
	VPushElement(m_StandardHUD);

// 	m_pFreeCameraController.reset(GCC_NEW MovementController(m_pCamera, 0, 0, false));

	HumanView::VOnRestore();
	return true;
}

void SamplesView::GameplayUiUpdateDelegate(IEventDataPtr pEventData)
{
// 	shared_ptr<EvtData_Gameplay_UI_Update> pCastEventData = static_pointer_cast<EvtData_Gameplay_UI_Update>(pEventData);
// 	if (!pCastEventData->GetUiString().empty())
// 		m_GameplayText = s2ws(pCastEventData->GetUiString());
// 	else
// 		m_GameplayText.clear();
}

void SamplesView::SetControlledActorDelegate(IEventDataPtr pEventData)
{
// 	shared_ptr<EvtData_Set_Controlled_Actor> pCastEventData = static_pointer_cast<EvtData_Set_Controlled_Actor>(pEventData);
// 	VSetControlledActor(pCastEventData->GetActorId());
}

void SamplesView::RegisterAllDelegates(void)
{
// 	IEventManager* pGlobalEventManager = IEventManager::Get();
// 	pGlobalEventManager->VAddListener(MakeDelegate(this, &TeapotWarsHumanView::GameplayUiUpdateDelegate), EvtData_Gameplay_UI_Update::sk_EventType);
// 	pGlobalEventManager->VAddListener(MakeDelegate(this, &TeapotWarsHumanView::SetControlledActorDelegate), EvtData_Set_Controlled_Actor::sk_EventType);
}

void SamplesView::RemoveAllDelegates(void)
{
// 	IEventManager* pGlobalEventManager = IEventManager::Get();
// 	pGlobalEventManager->VRemoveListener(MakeDelegate(this, &TeapotWarsHumanView::GameplayUiUpdateDelegate), EvtData_Gameplay_UI_Update::sk_EventType);
// 	pGlobalEventManager->VRemoveListener(MakeDelegate(this, &TeapotWarsHumanView::SetControlledActorDelegate), EvtData_Set_Controlled_Actor::sk_EventType);
}

MainMenuUI::MainMenuUI()
{
	m_SampleUI.Init(&D3DRenderer::m_DialogResourceManager);
	m_SampleUI.SetCallback(OnGUIEvent, this);

	int iY = 10;
	int iX = 35;
	int width = (g_SampleUIWidth / 2) - 10;
	int height = 30;
	int lineHeight = height + 1;

	D3DCOLOR color = 0x50505050;
	m_SampleUI.SetBackgroundColors(color);

	m_SampleUI.AddStatic(0, L"Main Menu", iX - 20, iY, g_SampleUIWidth, height * 2);
	iY += (lineHeight * 3);

	m_SampleUI.AddStatic(1, L"Create Game", iX, iY, g_SampleUIWidth, height);
	iY += lineHeight;

	m_SampleUI.AddListBox(CID_LEVEL_LISTBOX, iX, iY, g_SampleUIWidth - 70, lineHeight * 10);
	std::vector<Level> levels = g_pApp->GetGameLogic()->GetLevelManager()->GetLevels();
	m_Levels.reserve(levels.size());
	int count = 0;
	for (auto i = levels.begin(); i != levels.end(); ++i, ++count)
	{
		m_Levels.push_back(s2ws(*i));
		m_SampleUI.GetListBox(CID_LEVEL_LISTBOX)->AddItem(m_Levels[count].c_str(), NULL);
	}
	iY += (lineHeight * 15);

	m_SampleUI.AddButton(CID_START_BUTTON, L"Start Game", (g_SampleUIWidth - (width / 2)) / 2, iY += lineHeight, width / 2, height);

	Set();
}

MainMenuUI::~MainMenuUI()
{
	D3DRenderer::m_DialogResourceManager.UnregisterDialog(&m_SampleUI);
}

HRESULT MainMenuUI::VOnRestore()
{
	m_SampleUI.SetLocation((g_pApp->GetScreenWidth() - g_SampleUIWidth) / 2, (g_pApp->GetScreenHeight() - g_SampleUIHeight) / 2);
	m_SampleUI.SetSize(g_SampleUIWidth, g_SampleUIHeight);
	return S_OK;
}

HRESULT MainMenuUI::VOnRender(double fTime, float fElapsedTime)
{
	HRESULT hr;
	DXUT_BeginPerfEvent(DXUT_PERFEVENTCOLOR, L"ShootGameHUD");
	VTrace(m_SampleUI.OnRender(fElapsedTime));
	DXUT_EndPerfEvent();
	return S_OK;
}

LRESULT CALLBACK MainMenuUI::VOnMsgProc(AppMsg msg)
{
	return m_SampleUI.MsgProc(msg.m_hWnd, msg.m_uMsg, msg.m_wParam, msg.m_lParam);
}

void CALLBACK MainMenuUI::OnGUIEvent(UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext)
{
	MainMenuUI *ui = static_cast<MainMenuUI *>(pUserContext);
	ui->_OnGUIEvent(nEvent, nControlID, pControl, pUserContext);
}

void CALLBACK MainMenuUI::_OnGUIEvent(UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext)
{
	switch (nControlID)
	{
	case CID_LEVEL_LISTBOX:
	{
		break;
	}

	case CID_START_BUTTON:
	{
		if (m_LevelIndex == -1)
		{
			return;
		}
		g_pApp->m_Options.m_Level = ws2s(m_Levels[m_LevelIndex]);

		VSetVisible(false);

		shared_ptr<EvtData_Request_Start_Game> pRequestStartGameEvent(GCC_NEW EvtData_Request_Start_Game());
		IEventManager::Get()->VQueueEvent(pRequestStartGameEvent);

		break;
	}

	default:
	{
		GCC_ERROR("Unknown control.");
	}
	}

	Set();
}

void MainMenuUI::Set()
{
	m_LevelIndex = m_SampleUI.GetListBox(CID_LEVEL_LISTBOX)->GetSelectedIndex();
}

#define IDC_TOGGLEFULLSCREEN    1
#define IDC_TOGGLEREF           2
#define IDC_TOGGLEWARP          3

StandardHUD::StandardHUD()
{
	m_HUD.Init(&D3DRenderer::m_DialogResourceManager);
	m_HUD.SetCallback(OnGUIEvent); int iY = 10;
	m_HUD.AddButton(IDC_TOGGLEFULLSCREEN, L"Toggle full screen", 0, iY, 170, 22);
	m_HUD.AddButton(IDC_TOGGLEREF, L"Toggle REF", 0, iY += 26, 170, 22);
	m_HUD.AddButton(IDC_TOGGLEWARP, L"Toggle WARP", 0, iY += 26, 170, 22);
}

StandardHUD::~StandardHUD()
{

}

HRESULT StandardHUD::VOnRestore()
{
	m_HUD.SetLocation(DXUTGetDXGIBackBufferSurfaceDesc()->Width - 170, 0);
	m_HUD.SetSize(170, 170);
	return S_OK;
}

HRESULT StandardHUD::VOnRender(double fTime, float fElapsedTime)
{
	HRESULT hr;
	DXUT_BeginPerfEvent(DXUT_PERFEVENTCOLOR, L"StandardUI");
	VTrace(m_HUD.OnRender(fElapsedTime));
	DXUT_EndPerfEvent();
	return S_OK;
}

LRESULT CALLBACK StandardHUD::VOnMsgProc(AppMsg msg)
{
	return m_HUD.MsgProc(msg.m_hWnd, msg.m_uMsg, msg.m_wParam, msg.m_lParam);
}

void CALLBACK StandardHUD::OnGUIEvent(UINT nEvent, int nControlID, CDXUTControl* pControl, void *pUserContext)
{
	switch (nControlID)
	{
	case IDC_TOGGLEFULLSCREEN:
		DXUTToggleFullScreen();
		break;
	case IDC_TOGGLEREF:
		DXUTToggleREF();
		break;
	case IDC_TOGGLEWARP:
		DXUTToggleWARP();
		break;
	default:
		break;
	}
}

MainMenuView::MainMenuView() : HumanView(shared_ptr<IRenderer>())
{
	m_MainMenuUI.reset(GCC_NEW MainMenuUI);
	VPushElement(m_MainMenuUI);
}

MainMenuView::~MainMenuView()
{

}

void MainMenuView::VRenderText()
{
	HumanView::VRenderText();
}

void MainMenuView::VOnUpdate(double fTime, float fElapsedTime)
{
	HumanView::VOnUpdate(fTime, fElapsedTime);
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
