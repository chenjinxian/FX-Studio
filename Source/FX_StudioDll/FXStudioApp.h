#pragma once

#include "../RenderEngine/RenderEngineInterface.h"
#include "FXStudioHumanView.h"

class FXStudioApp : public RenderEngineApp
{
public:
	FXStudioApp(HINSTANCE hInstance, LPWSTR lpCmdLine, HWND hWnd = NULL, int screenWidth = SCREEN_WIDTH, int screenHeight = SCREEN_HEIGHT);
	virtual ~FXStudioApp();

	virtual const std::wstring VGetWindowTitle() override;
	virtual HICON VGetIcon() override;

protected:
	virtual BaseGameLogic* VCreateGameAndView();
};

class FXStudioLogic : public BaseGameLogic
{
public:
	FXStudioLogic(RenderEngineApp* pApp);
	~FXStudioLogic();

	virtual bool VLoadGame(const char* levelName);
	const std::string &GetProjectDirectory(void) { return m_ProjectDirectory; }

	int GetNumActors() { return (int)m_Actors.size(); }
	const ActorMap& GetActorMap() { return m_Actors; }

	bool IsRunning() { return (BGS_Running == m_State); }
	shared_ptr<FXStudioHumanView> GetHumanView();

protected:
	std::string m_ProjectDirectory;
};
