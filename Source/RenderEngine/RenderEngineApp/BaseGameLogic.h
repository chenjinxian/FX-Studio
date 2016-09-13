#pragma once
#include "../RenderEngineInterface.h"

class ActorFactory;

typedef std::map<ActorId, StrongActorPtr> ActorMap;

class BaseGameLogic : public IGameLogic
{
	friend class RenderEngineApp;

public:
	BaseGameLogic(RenderEngineApp* pApp);
	virtual ~BaseGameLogic();

	bool Init();
	ActorId GetNewActorID();

	virtual void VAddView(shared_ptr<IGameView> pView, ActorId actorId = INVALID_ACTOR_ID);
	virtual void VRemoveView(shared_ptr<IGameView> pView);

	virtual WeakActorPtr VGetActor(const ActorId actorId);
	virtual StrongActorPtr VCreateActor(const std::string &actorResource,
		TiXmlElement *overrides, const Matrix *initialTransform = NULL, const ActorId serversActorId = INVALID_ACTOR_ID);
	virtual void VDestroyActor(const ActorId actorId);
	virtual void VModifyActor(const ActorId actorId, TiXmlElement *overrides);

	virtual bool VLoadGame(const char* levelResource);
	virtual void VOnUpdate(float time, float elapsedTime);

protected:
	virtual ActorFactory* VCreateActorFactory(void);
	virtual bool VLoadGameDelegate(TiXmlElement* pLevelData) { return true; }

	RenderEngineApp* m_pApp;
	float m_LifeTime;
	ActorId m_LastActorId;
	ActorMap m_Actors;
	int m_HumanGamesLoaded;
	GameViewList m_GameViews;
	ActorFactory* m_pActorFactory;
};

