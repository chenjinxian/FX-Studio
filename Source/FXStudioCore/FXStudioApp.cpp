#include "stdafx.h"
#include "FXStudioApp.h"
#include "FXStudioView.h"

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
	BaseGameLogic* pGameLogic = GCC_NEW FXStudioLogic(this);
	pGameLogic->Init();

	shared_ptr<IGameView> pGameView(GCC_NEW FXStudioView(m_pRenderer));
	pGameLogic->VAddView(pGameView);

	return pGameLogic;
}

FXStudioLogic::FXStudioLogic(RenderEngineApp* pApp) : BaseGameLogic(pApp)
{

}

FXStudioLogic::~FXStudioLogic()
{

}
