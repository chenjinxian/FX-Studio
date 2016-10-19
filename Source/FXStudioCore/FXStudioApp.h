#pragma once
#include "../RenderEngine/RenderEngine.h"

class FXStudioView;

class FXStudioApp : public RenderEngineApp
{
public:
	FXStudioApp() : RenderEngineApp() { m_IsEditorRunning = true; }
	virtual ~FXStudioApp() {}

	TCHAR *VGetGameTitle() { return _T("FX-Studio Editor"); }
	TCHAR *VGetGameAppDirectory() { return _T("FX-Studio"); }
	HICON VGetIcon();

protected:
	BaseGameLogic* VCreateGameAndView() override;
};

class FXStudioLogic : public BaseGameLogic
{
public:
	FXStudioLogic();
	~FXStudioLogic();

	virtual bool VLoadGame(const std::string& projectXml) override;
	const std::string &GetProjectDirectory(void) { return m_ProjectDirectory; }

	uint32_t GetNumActors() { return m_Actors.size(); }

	const ActorMap& GetActorMap() { return m_Actors; }

	bool IsRunning() { return (BGS_Running == m_GameState); }
	shared_ptr<FXStudioView> GetHumanView();

protected:
	std::string m_ProjectDirectory;
};