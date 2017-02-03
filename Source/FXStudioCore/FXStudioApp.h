#pragma once
#include "../TinyEngine/TinyEngine.h"

class FXStudioView;

class FXStudioApp : public BaseGameApp
{
public:
	FXStudioApp() : BaseGameApp() {}
	virtual ~FXStudioApp() {}

	virtual const wchar_t* VGetWindowTitle() override { return _T("FX-Studio Editor"); }
	virtual const wchar_t* VGetWindowClass() override { return _T("FX-Studio"); }
	virtual HICON VGetIcon() override;

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

	uint32_t GetNumActors() { return (uint32_t)m_Actors.size(); }

	const ActorMap& GetActorMap() { return m_Actors; }

	bool IsRunning() { return (BGS_Running == m_GameState); }
	shared_ptr<FXStudioView> GetHumanView();

protected:
	std::string m_ProjectDirectory;
};