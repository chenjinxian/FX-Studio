#pragma once
#include "../RenderEngineBase.h"
#include "../RenderEngineInterface.h"

class ActorFactory;
typedef std::map<ActorId, StrongActorPtr> ActorMap;

class BaseGameLogic : public IGameLogic
{
	friend class RenderEngineApp;

public:
	BaseGameLogic();
	virtual ~BaseGameLogic();

	bool Init();

	virtual void VOnUpdate(float totalTime, float elapsedTime) override;
	virtual void VLoadGame(const std::string& projectXml) override;

	virtual void VAddView(shared_ptr<IGameView> pView, ActorId actorId = INVALID_ACTOR_ID);
	virtual void VRemoveView(shared_ptr<IGameView> pView);

protected:
	virtual ActorFactory* VCreateActorFactory();

private:
	GameViewList m_GameViews;
	ActorFactory* m_pActorFactory;
	ActorMap m_Actors;
};

