#include "DemoView.h"
#include "DemoApp.h"
#include "imgui.h"
#include "imgui_internal.h"

DemoView::DemoView(shared_ptr<IRenderer> renderer)
	: HumanView(renderer)
{
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
		if (msg.m_wParam == 'Q' || msg.m_wParam == VK_ESCAPE)
		{
			// 			if (MessageBox::Ask(QUESTION_QUIT_GAME) == IDYES)
			// 			{
			g_pApp->SetQuitting(true, false);
			// 			}
			return 1;
		}
	}

	return 0;
}

void DemoView::VRenderText(const GameTime& gameTime)
{
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(500, 100));

	if (!ImGui::Begin("Device State", nullptr, ImVec2(0, 0), 0.0f,
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings))
	{
		ImGui::End();
		return;
	}

	ImGui::Text(g_pApp->m_pRenderer->VGetDeviceName().c_str());
	ImGui::Text("%d (fps)", gameTime.GetFrameRate());

	ImGui::End();
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
	: m_UIWidth(600),
	m_UIHeight(600),
	m_UIPositionX((g_pApp->m_Config.m_ScreenWidth - m_UIWidth) / 2),
	m_UIPositionY((g_pApp->m_Config.m_ScreenHeight - m_UIHeight) / 2),
	m_CurrentSelect(0)
{

}

MainMenuUI::~MainMenuUI()
{
}

HRESULT MainMenuUI::VOnInitScreenElements()
{
	m_UIPositionX = (g_pApp->m_Config.m_ScreenWidth - m_UIWidth) / 2;
	m_UIPositionY = (g_pApp->m_Config.m_ScreenHeight - m_UIHeight) / 2;
	return S_OK;
}

HRESULT MainMenuUI::VOnDeleteScreenElements()
{
	return S_OK;
}

HRESULT MainMenuUI::VOnRender(const GameTime& gameTime)
{
	ImGui::SetNextWindowPos(ImVec2(m_UIPositionX, m_UIPositionY));
	ImGui::SetNextWindowSize(ImVec2(m_UIWidth, m_UIHeight));

	if (!ImGui::Begin("Main Menu", nullptr, ImVec2(0, 0), 0.3f,
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings))
	{
		ImGui::End();
		return S_FALSE;
	}

	ImGui::NewLine();
	ImGui::NewLine();
	ImGui::Text("Main Menu");
	ImGui::NewLine();

	ImGui::Text("Select a project");
	std::vector<const char*> projects = g_pApp->GetGameLogic()->GetProjectManager()->GetProjects();
	ImGui::PushItemWidth(-1);
	ImGui::ListBox("", &m_CurrentSelect, &projects[0], projects.size(), 15);
	ImGui::PopItemWidth();

	ImGui::NewLine();
	ImGui::NewLine();
	ImGui::SameLine(220);
	bool m_IsStarted = ImGui::Button("Start Game", ImVec2(160, 36));

	ImGui::End();

	if (m_IsStarted && m_CurrentSelect >= 0)
	{
		g_pApp->m_Config.m_Project = projects[m_CurrentSelect];

		VSetVisible(false);

		shared_ptr<EvtData_Request_Start_Game> pRequestStartGameEvent(DEBUG_NEW EvtData_Request_Start_Game());
		IEventManager::Get()->VQueueEvent(pRequestStartGameEvent);
	}

	return S_OK;
}

LRESULT CALLBACK MainMenuUI::VOnMsgProc(AppMsg msg)
{
	ImGuiIO& io = ImGui::GetIO();

	switch (msg.m_uMsg)
	{
	case WM_KEYDOWN:
		if (msg.m_wParam < 256) io.KeysDown[msg.m_wParam] = 1;
		break;

	case WM_KEYUP:
		if (msg.m_wParam < 256) io.KeysDown[msg.m_wParam] = 0;
		break;

	case WM_MOUSEMOVE:
		io.MousePos.x = LOWORD(msg.m_lParam);
		io.MousePos.y = HIWORD(msg.m_lParam);
		break;

	case WM_MOUSEWHEEL:
		io.MouseWheel += GET_WHEEL_DELTA_WPARAM(msg.m_wParam) > 0 ? +1.0f : -1.0f;
		break;

	case WM_LBUTTONDOWN:
		io.MouseDown[0] = true;
		break;

	case WM_LBUTTONUP:
		io.MouseDown[0] = false;
		break;

	case WM_RBUTTONDOWN:
		io.MouseDown[1] = true;
		break;

	case WM_RBUTTONUP:
		io.MouseDown[1] = false;
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

MainMenuView::MainMenuView() : HumanView(shared_ptr<IRenderer>())
{
	m_MainMenuUI.reset(DEBUG_NEW MainMenuUI);
	VPushElement(m_MainMenuUI);
}

MainMenuView::~MainMenuView()
{

}

void MainMenuView::VRenderText(const GameTime& gameTime)
{

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

StandardHUD::StandardHUD()
	: m_UIWidth(400),
	m_UIHeight(400),
	m_UIPositionX((g_pApp->m_Config.m_ScreenWidth - m_UIWidth) / 2),
	m_UIPositionY((g_pApp->m_Config.m_ScreenHeight - m_UIHeight) / 2),
	m_isShowConfig(false)
{
}

StandardHUD::~StandardHUD()
{

}

HRESULT StandardHUD::VOnInitScreenElements()
{
	m_UIPositionX = (g_pApp->m_Config.m_ScreenWidth - m_UIWidth) / 2;
	m_UIPositionY = (g_pApp->m_Config.m_ScreenHeight - m_UIHeight) / 2;
	return S_OK;
}

HRESULT StandardHUD::VOnDeleteScreenElements()
{
	return S_OK;
}

HRESULT StandardHUD::VOnRender(const GameTime& gameTime)
{
	if (m_isShowConfig)
	{
		ImGui::SetNextWindowPos(ImVec2(m_UIPositionX, m_UIPositionY));
		ImGui::SetNextWindowSize(ImVec2(m_UIWidth, m_UIHeight));

		if (!ImGui::Begin("App Options", nullptr, ImVec2(0, 0), 0.3f,
			ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings))
		{
			ImGui::End();
			return S_FALSE;
		}

		ImGui::NewLine();
		ImGui::NewLine();
		ImGui::Text("App Options");
		ImGui::NewLine();

		ImGui::Checkbox("Full Screen", &g_pApp->m_Config.m_IsFullScreen);
		ImGui::Checkbox("VSync", &g_pApp->m_Config.m_IsVSync);

		ImGui::NewLine();
		ImGui::NewLine();
		ImGui::SameLine(20);
		if (ImGui::Button("Ok", ImVec2(160, 36)))
		{
			g_pApp->SetQuitting(true, true);
		}

		ImGui::SameLine(220);
		m_isShowConfig = !ImGui::Button("Cancel", ImVec2(160, 36));

		ImGui::End();
	}

	return S_OK;
}

LRESULT CALLBACK StandardHUD::VOnMsgProc(AppMsg msg)
{
	ImGuiIO& io = ImGui::GetIO();

	switch (msg.m_uMsg)
	{
	case WM_KEYDOWN:
		if (msg.m_wParam < 256) io.KeysDown[msg.m_wParam] = 1;
		if (msg.m_wParam == VK_F1)
		{
			m_isShowConfig = !m_isShowConfig;
			return 1;
		}
		break;

	case WM_KEYUP:
		if (msg.m_wParam < 256) io.KeysDown[msg.m_wParam] = 0;
		break;

	case WM_MOUSEMOVE:
		io.MousePos.x = LOWORD(msg.m_lParam);
		io.MousePos.y = HIWORD(msg.m_lParam);
		break;

	case WM_MOUSEWHEEL:
		io.MouseWheel += GET_WHEEL_DELTA_WPARAM(msg.m_wParam) > 0 ? +1.0f : -1.0f;
		break;

	case WM_LBUTTONDOWN:
		io.MouseDown[0] = true;
		break;

	case WM_LBUTTONUP:
		io.MouseDown[0] = false;
		break;

	case WM_RBUTTONDOWN:
		io.MouseDown[1] = true;
		break;

	case WM_RBUTTONUP:
		io.MouseDown[1] = false;
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
