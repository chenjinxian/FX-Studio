#include "stdafx.h"
#include "FXStudioApp.h"
#include <direct.h>

#ifdef _DEBUG
#pragma comment(lib, "RenderEngined.lib")
#else
#pragma comment(lib, "RenderEngine.lib")
#endif

FXStudioApp::FXStudioApp(
	HINSTANCE hInstance, LPWSTR lpCmdLine, HWND hWnd /*= NULL*/, int screenWidth /*= SCREEN_WIDTH*/, int screenHeight /*= SCREEN_HEIGHT*/)
	: RenderEngineApp(hInstance, lpCmdLine, hWnd, screenWidth, screenHeight)
{

}

FXStudioApp::~FXStudioApp()
{

}

BaseGameLogic* FXStudioApp::VCreateGameAndView()
{
	BaseGameLogic* gameLogic = GCC_NEW FXStudioLogic(this);
	gameLogic->Init();

	shared_ptr<IGameView> gameView(GCC_NEW FXStudioHumanView(m_pRenderer));
	gameLogic->VAddView(gameView);

	return gameLogic;
}

FXStudioLogic::FXStudioLogic(RenderEngineApp* pApp) : BaseGameLogic(pApp)
{
	m_ProjectDirectory = getcwd(NULL, 0);
	int slashGamePos = m_ProjectDirectory.rfind("\\Game");
	m_ProjectDirectory = m_ProjectDirectory.substr(0, slashGamePos);
}

FXStudioLogic::~FXStudioLogic()
{

}

bool FXStudioLogic::VLoadGame(const char* levelName)
{
	while (m_Actors.size() > 0)
	{
		ActorId id = m_Actors.begin()->first;
		VDestroyActor(id);
	}

	if (!BaseGameLogic::VLoadGame(levelName))
	{
		return false;
	}

	return true;
}

shared_ptr<FXStudioHumanView> FXStudioLogic::GetHumanView()
{
	GCC_ASSERT(m_GameViews.size() == 1);
	shared_ptr<IGameView> pGameView = *m_GameViews.begin();
	shared_ptr<FXStudioHumanView> editorHumanView = static_pointer_cast<FXStudioHumanView>(pGameView);
	return editorHumanView;
}
