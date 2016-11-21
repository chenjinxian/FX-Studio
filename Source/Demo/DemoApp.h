#pragma once
#include "../TinyEngine/TinyEngine.h"

class DemoApp : public BaseGameApp
{
public:
	DemoApp();
	~DemoApp();

protected:
	virtual const wchar_t* VGetWindowTitle() override;
	virtual const wchar_t* VGetWindowClass() override;
	virtual HICON VGetIcon() override;

	virtual BaseGameLogic *VCreateGameAndView();
};

class DemoLogic : public BaseGameLogic
{
public:
	DemoLogic();
	virtual ~DemoLogic();

	virtual void VSetProxy();
	virtual void VMoveActor(ActorId id, const Matrix& mat);
	virtual void VChangeState(BaseGameState gameState);
	virtual void VAddView(shared_ptr<IGameView> pView, ActorId actorId = INVALID_ACTOR_ID);
	virtual shared_ptr<IGamePhysics> VGetGamePhysics(void) { return m_pPhysics; }

	void RequestStartGameDelegate(IEventDataPtr pEventData);
	void RemoteClientDelegate(IEventDataPtr pEventData);
	void NetworkPlayerActorAssignmentDelegate(IEventDataPtr pEventData);
	void EnvironmentLoadedDelegate(IEventDataPtr pEventData);
	void ThrustDelegate(IEventDataPtr pEventData);
	void SteerDelegate(IEventDataPtr pEventData);
	void StartThrustDelegate(IEventDataPtr pEventData);
	void EndThrustDelegate(IEventDataPtr pEventData);
	void StartSteerDelegate(IEventDataPtr pEventData);
	void EndSteerDelegate(IEventDataPtr pEventData);

protected:
	virtual bool VLoadGameDelegate(tinyxml2::XMLElement* pLevelData);

private:
	void RegisterAllDelegates(void);
	void RemoveAllDelegates(void);
};