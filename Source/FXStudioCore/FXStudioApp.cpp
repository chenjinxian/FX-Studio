#include "stdafx.h"
#include "FXStudioApp.h"
#include "FXStudioView.h"
#include <direct.h>

#ifdef _DEBUG
#pragma comment(lib, "DirectXTKd.lib")
#pragma comment(lib, "DXUTd.lib")
#pragma comment(lib, "DXUTOptd.lib")
#pragma comment(lib, "Effects11d.lib")
#pragma comment(lib, "tinyxmld.lib")
#pragma comment(lib, "zlibstaticd.lib")
#else
#pragma comment(lib, "DirectXTK.lib")
#pragma comment(lib, "DXUT.lib")
#pragma comment(lib, "DXUTOpt.lib")
#pragma comment(lib, "Effects11.lib")
#pragma comment(lib, "tinyxml.lib")
#pragma comment(lib, "zlibstatic.lib")
#endif

#ifdef _DEBUG
#pragma comment(lib, "RenderEngined.lib")
#else
#pragma comment(lib, "RenderEngine.lib")
#endif

FXStudioApp globalApp;

HICON FXStudioApp::VGetIcon()
{
	return LoadIcon(GetInstance(), MAKEINTRESOURCE(IDI_APPLICATION));
}

BaseGameLogic* FXStudioApp::VCreateGameAndView()
{
	BaseGameLogic* pGameLogic = GCC_NEW FXStudioLogic();
	pGameLogic->Init();

	shared_ptr<IGameView> pGameView(GCC_NEW FXStudioView(m_pRenderer));
	pGameLogic->VAddView(pGameView);

	return pGameLogic;
}

FXStudioLogic::FXStudioLogic() : BaseGameLogic()
{
	m_ProjectDirectory = getcwd(NULL, 0);
	int slashGamePos = m_ProjectDirectory.rfind("\\Game");
	m_ProjectDirectory = m_ProjectDirectory.substr(0, slashGamePos);
}

FXStudioLogic::~FXStudioLogic()
{

}

bool FXStudioLogic::VLoadGame(const std::string& projectXml)
{
	while (m_Actors.size() > 0)
	{
		ActorId id = m_Actors.begin()->first;
		VDestroyActor(id);
	}

	if (!BaseGameLogic::VLoadGame(projectXml))
	{
		return false;
	}

	VChangeState(BGS_Running);
	return true;
}

shared_ptr<FXStudioView> FXStudioLogic::GetHumanView()
{
	GCC_ASSERT(m_GameViews.size() == 1);
	shared_ptr<IGameView> pGameView = *m_GameViews.begin();
	shared_ptr<FXStudioView> editorHumanView = static_pointer_cast<FXStudioView>(pGameView);
	return editorHumanView;
}
