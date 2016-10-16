#include "ShootGameView.h"
#include "ThirdPersonShoot.h"

extern ShootGameApp* g_GameApp;

#define CID_CREATE_GAME_RADIO			(2)
#define CID_JOIN_GAME_RADIO				(3)
#define CID_NUM_AI_SLIDER				(4)
#define CID_NUM_PLAYER_SLIDER			(5)
#define CID_HOST_LISTEN_PORT			(6)
#define CID_CLIENT_ATTACH_PORT			(7)
#define CID_START_BUTTON				(8)
#define CID_HOST_NAME					(9)
#define CID_NUM_AI_LABEL				(10)
#define CID_NUM_PLAYER_LABEL			(11)
#define CID_HOST_LISTEN_PORT_LABEL		(12)
#define CID_CLIENT_ATTACH_PORT_LABEL	(13)
#define CID_HOST_NAME_LABEL				(14)
#define CID_LEVEL_LABEL					(15)
#define CID_LEVEL_LISTBOX				(16)

const int g_SampleUIWidth = 600;
const int g_SampleUIHeight = 600;

ShootGameView::ShootGameView(shared_ptr<IRenderer> renderer) : HumanView(renderer)
{
	m_bShowUI = true;
	RegisterAllDelegates();
}


ShootGameView::~ShootGameView()
{
	RemoveAllDelegates();
}

LRESULT CALLBACK ShootGameView::VOnMsgProc(AppMsg msg)
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

void ShootGameView::VRenderText()
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
		D3DRenderer::m_pTextHelper->DrawTextLine(DXUTGetFrameStats());
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

		TCHAR buffer[256];
		const TCHAR *s = NULL;
		Matrix toWorld;
		Matrix fromWorld;
		if (m_pCamera)
		{
// 			m_pCamera->VGet()->Transform(&toWorld, &fromWorld);
		}
		swprintf(buffer, g_pApp->GetString(_T("IDS_CAMERA_LOCATION")).c_str(), toWorld.m[3][0], toWorld.m[3][1], toWorld.m[3][2]);
		D3DRenderer::m_pTextHelper->DrawTextLine(buffer);

		RECT helpRect;
		helpRect.left = 0;
		helpRect.right = g_pApp->GetScreenWidth() - 10;
		helpRect.top = g_pApp->GetScreenHeight() - 15 * 8;
		helpRect.bottom = g_pApp->GetScreenHeight();
		D3DRenderer::m_pTextHelper->SetInsertionPos(helpRect.right, helpRect.top);
		D3DRenderer::m_pTextHelper->SetForegroundColor(Color(1.0f, 0.75f, 0.0f, 1.0f));
		D3DRenderer::m_pTextHelper->DrawTextLine(helpRect, g_pApp->GetString(_T("IDS_CONTROLS_HEADER")).c_str());
		helpRect.top = g_pApp->GetScreenHeight() - 15 * 7;
		D3DRenderer::m_pTextHelper->DrawTextLine(helpRect, g_pApp->GetString(_T("IDS_CONTROLS")).c_str());
	}

	D3DRenderer::m_pTextHelper->End();
}

void ShootGameView::VOnUpdate(uint32_t deltaMs)
{
	HumanView::VOnUpdate(deltaMs);

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

void ShootGameView::VOnAttach(GameViewId vid, ActorId aid)
{
	HumanView::VOnAttach(vid, aid);
}

void ShootGameView::VSetControlledActor(ActorId actorId)
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
// 	m_pTeapotController.reset(GCC_NEW TeapotController(m_pTeapot));
// 	m_KeyboardHandler = m_pTeapotController;
// 	m_PointerHandler = m_pTeapotController;
// 	m_pCamera->SetTarget(m_pTeapot);
// 	m_pTeapot->SetAlpha(0.8f);
}

bool ShootGameView::VLoadGameDelegate(TiXmlElement* pLevelData)
{
	if (!HumanView::VLoadGameDelegate(pLevelData))
		return false;

	m_StandardHUD.reset(GCC_NEW StandardHUD);
	VPushElement(m_StandardHUD);

// 	m_pFreeCameraController.reset(GCC_NEW MovementController(m_pCamera, 0, 0, false));

	m_pScene->VOnRestore();
	return true;
}

void ShootGameView::GameplayUiUpdateDelegate(IEventDataPtr pEventData)
{
// 	shared_ptr<EvtData_Gameplay_UI_Update> pCastEventData = static_pointer_cast<EvtData_Gameplay_UI_Update>(pEventData);
// 	if (!pCastEventData->GetUiString().empty())
// 		m_GameplayText = s2ws(pCastEventData->GetUiString());
// 	else
// 		m_GameplayText.clear();
}

void ShootGameView::SetControlledActorDelegate(IEventDataPtr pEventData)
{
// 	shared_ptr<EvtData_Set_Controlled_Actor> pCastEventData = static_pointer_cast<EvtData_Set_Controlled_Actor>(pEventData);
// 	VSetControlledActor(pCastEventData->GetActorId());
}

void ShootGameView::RegisterAllDelegates(void)
{
// 	IEventManager* pGlobalEventManager = IEventManager::Get();
// 	pGlobalEventManager->VAddListener(MakeDelegate(this, &TeapotWarsHumanView::GameplayUiUpdateDelegate), EvtData_Gameplay_UI_Update::sk_EventType);
// 	pGlobalEventManager->VAddListener(MakeDelegate(this, &TeapotWarsHumanView::SetControlledActorDelegate), EvtData_Set_Controlled_Actor::sk_EventType);
}

void ShootGameView::RemoveAllDelegates(void)
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
	int iX2 = g_SampleUIWidth / 2;
	int width = (g_SampleUIWidth / 2) - 10;
	int height = 25;
	int lineHeight = height + 2;

	m_NumAIs = g_pApp->m_Options.m_NumAIs;
	m_NumPlayers = g_pApp->m_Options.m_ExpectedPlayers;
	m_HostName = g_pApp->m_Options.m_GameHost;
	m_HostListenPort = g_pApp->m_Options.m_ListenPort;
	m_ClientAttachPort = g_pApp->m_Options.m_ListenPort;

	m_bCreatingGame = true;

	D3DCOLOR color = 0x50505050;
	m_SampleUI.SetBackgroundColors(color);

	m_SampleUI.SetFont(0, L"Ariel", height, 0);

	m_SampleUI.AddStatic(0, L"Main Menu", iX - 20, iY, g_SampleUIWidth, height * 2);
	iY += (lineHeight * 3);

	m_SampleUI.AddRadioButton(CID_CREATE_GAME_RADIO, 1, L"Create Game", iX, iY, g_SampleUIWidth, height);
	iY += lineHeight;

	m_SampleUI.AddStatic(CID_LEVEL_LABEL, L"Level", iX, iY, width, height);
	m_SampleUI.AddListBox(CID_LEVEL_LISTBOX, iX2, iY, width, lineHeight * 5);
	std::vector<Level> levels = g_pApp->GetGameLogic()->GetLevelManager()->GetLevels();
	m_Levels.reserve(levels.size());
	int count = 0;
	for (auto i = levels.begin(); i != levels.end(); ++i, ++count)
	{
		m_Levels.push_back(s2ws(*i));
		m_SampleUI.GetListBox(CID_LEVEL_LISTBOX)->AddItem(m_Levels[count].c_str(), NULL);
	}
	iY += (lineHeight * 5);

	m_SampleUI.AddStatic(CID_NUM_AI_LABEL, L"", iX, iY, width, height);
	m_SampleUI.AddSlider(CID_NUM_AI_SLIDER, iX2, iY, width, height);
	m_SampleUI.GetSlider(CID_NUM_AI_SLIDER)->SetRange(0, g_pApp->m_Options.m_MaxAIs);
	m_SampleUI.GetSlider(CID_NUM_AI_SLIDER)->SetValue(m_NumAIs);
	iY += lineHeight;

	m_SampleUI.AddStatic(CID_NUM_PLAYER_LABEL, L"", iX, iY, width, height);
	m_SampleUI.AddSlider(CID_NUM_PLAYER_SLIDER, iX2, iY, width, height);
	m_SampleUI.GetSlider(CID_NUM_PLAYER_SLIDER)->SetRange(1, g_pApp->m_Options.m_MaxPlayers);
	m_SampleUI.GetSlider(CID_NUM_PLAYER_SLIDER)->SetValue(m_NumPlayers);
	iY += lineHeight;

	m_SampleUI.AddStatic(CID_HOST_LISTEN_PORT_LABEL, L"Host Listen Port", iX, iY, width, height);
	m_SampleUI.AddEditBox(CID_HOST_LISTEN_PORT, L"57", iX2, iY, width, height * 2);
	CDXUTEditBox *eb = m_SampleUI.GetEditBox(CID_HOST_LISTEN_PORT);
	eb->SetVisible(false);
	iY += lineHeight * 3;

	m_SampleUI.AddRadioButton(CID_JOIN_GAME_RADIO, 1, L"Join Game", iX, iY, width, height);
	m_SampleUI.GetRadioButton(CID_JOIN_GAME_RADIO)->SetChecked(true);
	iY += lineHeight;

	m_SampleUI.AddStatic(CID_CLIENT_ATTACH_PORT_LABEL, L"Host Attach Port", iX, iY, width, height);
	m_SampleUI.AddEditBox(CID_CLIENT_ATTACH_PORT, L"57", iX2, iY, width, height * 2);
	iY += lineHeight * 3;


	m_SampleUI.AddStatic(CID_HOST_NAME_LABEL, L"Host Name", iX, iY, width, height);
	m_SampleUI.AddEditBox(CID_HOST_NAME, L"sunshine", iX2, iY, width, height * 2);
	iY += lineHeight;

	m_SampleUI.AddButton(CID_START_BUTTON, L"Start Game", (g_SampleUIWidth - (width / 2)) / 2, iY += lineHeight, width / 2, height);

	m_SampleUI.GetRadioButton(CID_CREATE_GAME_RADIO)->SetChecked(true);

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
	case CID_CREATE_GAME_RADIO:
	{
		m_bCreatingGame = true;
		break;
	}

	case CID_JOIN_GAME_RADIO:
	{
		m_bCreatingGame = false;
		break;
	}

	case CID_LEVEL_LISTBOX:
	case CID_NUM_AI_SLIDER:
	case CID_NUM_PLAYER_SLIDER:
	case CID_HOST_LISTEN_PORT:
	case CID_CLIENT_ATTACH_PORT:
	case CID_HOST_NAME:
	{
		break;
	}

	case CID_START_BUTTON:
	{
		g_pApp->m_Options.m_NumAIs = m_NumAIs;
		g_pApp->m_Options.m_ExpectedPlayers = m_NumPlayers;
		if (m_bCreatingGame)
		{
			if (m_LevelIndex == -1)
			{
				return;
			}
			g_pApp->m_Options.m_Level = ws2s(m_Levels[m_LevelIndex]);
			g_pApp->m_Options.m_GameHost = "";
			g_pApp->m_Options.m_ListenPort = atoi(m_HostListenPort.c_str());
		}
		else
		{
			g_pApp->m_Options.m_GameHost = m_HostName;
			g_pApp->m_Options.m_ListenPort = atoi(m_ClientAttachPort.c_str());
		}

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
	WCHAR buffer[256];
	CHAR ansiBuffer[256];

	m_LevelIndex = m_SampleUI.GetListBox(CID_LEVEL_LISTBOX)->GetSelectedIndex();
	m_SampleUI.GetListBox(CID_LEVEL_LISTBOX)->SetVisible(m_bCreatingGame);

	m_NumAIs = m_SampleUI.GetSlider(CID_NUM_AI_SLIDER)->GetValue();
	m_SampleUI.GetSlider(CID_NUM_AI_SLIDER)->SetVisible(m_bCreatingGame);

	wsprintf(buffer, _T("%s: %d\n"), L"Number of AIs", m_NumAIs);
	m_SampleUI.GetStatic(CID_NUM_AI_LABEL)->SetText(buffer);
	m_SampleUI.GetStatic(CID_NUM_AI_LABEL)->SetVisible(m_bCreatingGame);

	m_NumPlayers = m_SampleUI.GetSlider(CID_NUM_PLAYER_SLIDER)->GetValue();
	m_SampleUI.GetSlider(CID_NUM_PLAYER_SLIDER)->SetVisible(m_bCreatingGame);
	wsprintf(buffer, _T("%s: %d\n"), L"Number of Players", m_NumPlayers);
	m_SampleUI.GetStatic(CID_NUM_PLAYER_LABEL)->SetText(buffer);
	m_SampleUI.GetStatic(CID_NUM_PLAYER_LABEL)->SetVisible(m_bCreatingGame);

	m_SampleUI.GetStatic(CID_HOST_LISTEN_PORT_LABEL)->SetVisible(m_NumPlayers > 1 && m_bCreatingGame);
	m_SampleUI.GetEditBox(CID_HOST_LISTEN_PORT)->SetVisible(m_NumPlayers > 1 && m_bCreatingGame);
	if (m_bCreatingGame)
	{
		WideToAnsiCch(ansiBuffer, m_SampleUI.GetEditBox(CID_HOST_LISTEN_PORT)->GetText(), 256);
		m_HostListenPort = ansiBuffer;
	}

	m_SampleUI.GetStatic(CID_HOST_NAME_LABEL)->SetVisible(!m_bCreatingGame);
	m_SampleUI.GetEditBox(CID_HOST_NAME)->SetVisible(!m_bCreatingGame);

	WideToAnsiCch(ansiBuffer, m_SampleUI.GetEditBox(CID_HOST_NAME)->GetText(), 256);
	m_HostName = ansiBuffer;

	m_SampleUI.GetStatic(CID_CLIENT_ATTACH_PORT_LABEL)->SetVisible(!m_bCreatingGame);
	m_SampleUI.GetEditBox(CID_CLIENT_ATTACH_PORT)->SetVisible(!m_bCreatingGame);
	if (!m_bCreatingGame)
	{
		WideToAnsiCch(ansiBuffer, m_SampleUI.GetEditBox(CID_CLIENT_ATTACH_PORT)->GetText(), 256);
		m_ClientAttachPort = ansiBuffer;
	}
}

#define IDC_TOGGLEFULLSCREEN    1
#define IDC_TOGGLEREF           2
#define IDC_CHANGEDEVICE        3

StandardHUD::StandardHUD()
{
	m_HUD.Init(&D3DRenderer::m_DialogResourceManager);
	m_HUD.SetCallback(OnGUIEvent); int iY = 10;
	m_HUD.AddButton(IDC_TOGGLEFULLSCREEN, L"Toggle full screen", 35, iY, 125, 22);
	m_HUD.AddButton(IDC_TOGGLEREF, L"Toggle REF (F3)", 35, iY += 24, 125, 22);
}

StandardHUD::~StandardHUD()
{

}

HRESULT StandardHUD::VOnRestore()
{
	m_HUD.SetLocation(g_pApp->GetScreenWidth() - 170, 0);
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
	case IDC_TOGGLEFULLSCREEN: DXUTToggleFullScreen(); break;
	case IDC_TOGGLEREF:        DXUTToggleREF(); break;
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

void MainMenuView::VOnUpdate(uint32_t deltaMs)
{
	HumanView::VOnUpdate(deltaMs);
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
