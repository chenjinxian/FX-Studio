#include "stdafx.h"
#include "FXStudioApp.h"

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

const std::wstring FXStudioApp::VGetWindowTitle()
{
	return _T("FX Studio");
}

HICON FXStudioApp::VGetIcon()
{
	return LoadIcon(GetInstance(), MAKEINTRESOURCE(IDI_APPLICATION));
}

BaseGameLogic* FXStudioApp::VCreateGameAndView()
{
	BaseGameLogic *game = NULL;

	game = GCC_NEW FXStudioLogic(this);
	game->Init();

	shared_ptr<IGameView> gameView(GCC_NEW FXStudioHumanView(m_pRenderer));
	game->VAddView(gameView);

	return game;
}

FXStudioLogic::FXStudioLogic(RenderEngineApp* pApp) : BaseGameLogic(pApp)
{

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

	VChangeState(BGS_Running);
	return true;
}

shared_ptr<FXStudioHumanView> FXStudioLogic::GetHumanView()
{
	GCC_ASSERT(m_GameViews.size() == 1);
	shared_ptr<IGameView> pGameView = *m_GameViews.begin();
	shared_ptr<FXStudioHumanView> editorHumanView = static_pointer_cast<FXStudioHumanView>(pGameView);
	return editorHumanView;
}
