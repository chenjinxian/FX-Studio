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

typedef void(*MoveDelegate)(const char*);

class FXStudioLogic : public BaseGameLogic
{
public:
	FXStudioLogic();
	~FXStudioLogic();

	virtual void VMoveActor(ActorId actorId) override;
	virtual bool VLoadGame(const std::string& projectXml) override;

	void SetMoveDelegate(MoveDelegate delegate) { m_MoveDelegate = delegate; }
	const std::string &GetProjectDirectory(void) { return m_ProjectDirectory; }
	uint32_t GetNumActors() { return (uint32_t)m_Actors.size(); }
	const ActorMap& GetActorMap() { return m_Actors; }
	bool IsRunning() { return (BGS_Running == m_GameState); }
	shared_ptr<FXStudioView> GetHumanView();
	
private:
	void MoveCameraDelegate(IEventDataPtr pEventData);

	std::string m_ProjectDirectory;
	MoveDelegate m_MoveDelegate;
};