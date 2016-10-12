#pragma once
#include "../RenderEngine/RenderEngine.h"

class ShootGameApp : public RenderEngineApp
{
public:
	ShootGameApp();
	virtual ~ShootGameApp();

	virtual TCHAR *VGetGameTitle() { return _T("Shoot Game"); }
	virtual TCHAR *VGetGameAppDirectory() { return _T("ThirdPersonShoot"); }
	virtual HICON VGetIcon();

protected:
	virtual BaseGameLogic *VCreateGameAndView();

	virtual void VRegisterGameEvents(void);
	virtual void VCreateNetworkEventForwarder(void);
	virtual void VDestroyNetworkEventForwarder(void);
};

class ShootGameLogic : public BaseGameLogic
{
public:
	ShootGameLogic();
	virtual ~ShootGameLogic();

	virtual void VSetProxy();
	virtual void VMoveActor(ActorId id, const Matrix& mat);
	virtual void VChangeState(BaseGameState gameState);
	virtual void VAddView(shared_ptr<IGameView> pView, ActorId actorId = INVALID_ACTOR_ID);
	virtual shared_ptr<IGamePhysics> VGetGamePhysics(void) { return m_pPhysics; }

	void RequestStartGameDelegate(IEventDataPtr pEventData);
	void RemoteClientDelegate(IEventDataPtr pEventData);
	void EnvironmentLoadedDelegate(IEventDataPtr pEventData);
	void ThrustDelegate(IEventDataPtr pEventData);
	void SteerDelegate(IEventDataPtr pEventData);
	void StartThrustDelegate(IEventDataPtr pEventData);
	void EndThrustDelegate(IEventDataPtr pEventData);
	void StartSteerDelegate(IEventDataPtr pEventData);
	void EndSteerDelegate(IEventDataPtr pEventData);

protected:
	virtual bool VLoadGameDelegate(TiXmlElement* pLevelData);

private:
	void RegisterAllDelegates(void);
	void RemoveAllDelegates(void);
};
