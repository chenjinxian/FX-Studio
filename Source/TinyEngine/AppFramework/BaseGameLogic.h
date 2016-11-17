#pragma once
#include "../TinyEngineBase.h"
#include "../TinyEngineInterface.h"
#include "../EventManager/EventManager.h"

class ActorFactory;

enum BaseGameState
{
	BGS_Invalid,
	BGS_Initializing,
	BGS_MainMenu,
	BGS_WaitingForPlayers,
	BGS_LoadingGameEnvironment,
	BGS_WaitingForPlayersToLoadEnvironment,
	BGS_SpawningPlayersActors,
	BGS_Running
};


typedef std::map<ActorId, StrongActorPtr> ActorMap;
typedef std::string Level;

class BaseGameLogic : public IGameLogic
{
	friend class BaseGameApp;

public:
	BaseGameLogic();
	virtual ~BaseGameLogic();

	bool Init();

	void SetProxy(bool isProxy)
	{
		m_IsProxy = isProxy;
	}
	bool IsProxy() const { return m_IsProxy; }

	ActorId GetNewActorID(void)
	{
		return ++m_LastActorId;
	}
	std::string GetActorXml(ActorId id);
	virtual void VSetProxy();

	virtual void VOnUpdate(double fTime, float fElapsedTime) override;
	virtual bool VLoadGame(const std::string& projectXml) override;

	virtual void VAddView(shared_ptr<IGameView> pView, ActorId actorId = INVALID_ACTOR_ID);
	virtual void VRemoveView(shared_ptr<IGameView> pView);

	virtual StrongActorPtr VCreateActor(
		tinyxml2::XMLElement *pActorRoot, const Matrix& initialTransform = Matrix::Identity, ActorId serversActorId = INVALID_ACTOR_ID) override;
	virtual void VDestroyActor(ActorId actorId) override;
	virtual WeakActorPtr VGetActor(ActorId actorId) override;
	virtual void VMoveActor(ActorId actorId, const Matrix& mat) override;
	virtual void VModifyActor(ActorId actorId, tinyxml2::XMLElement *overrides);

	virtual void VChangeState(BaseGameState gameState);
	const BaseGameState GetState() const { return m_GameState; }

	void ToggleRenderDiagnostics() { m_IsRenderDiagnostics = !m_IsRenderDiagnostics; }
	virtual void VRenderDiagnostics();
	virtual shared_ptr<IGamePhysics> VGetGamePhysics(void) { return m_pPhysics; }
	void RequestDestroyActorDelegate(IEventDataPtr pEventData);

protected:
	virtual ActorFactory* VCreateActorFactory();
	virtual bool VLoadGameDelegate(tinyxml2::XMLElement* pData) { return true; }
	void MoveActorDelegate(IEventDataPtr pEventData);
	void RequestNewActorDelegate(IEventDataPtr pEventData);

	ActorMap m_Actors;
	ActorId m_LastActorId;
	BaseGameState m_GameState;
	GameViewList m_GameViews;
	ActorFactory* m_pActorFactory;

	int m_ExpectedPlayers;
	int m_ExpectedRemotePlayers;
	int m_ExpectedAI;
	int m_HumanPlayersAttached;
	int m_AIPlayersAttached;
	int m_HumanGamesLoaded;

	bool m_IsProxy;
	int m_RemotePlayerId;
	bool m_IsRenderDiagnostics;
	shared_ptr<IGamePhysics> m_pPhysics;
};
