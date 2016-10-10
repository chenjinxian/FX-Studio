#pragma once
#include "../RenderEngineBase.h"
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

	virtual void VOnUpdate(float totalTime, float elapsedTime) override;
	virtual bool VLoadGame(const std::string& projectXml) override;

	virtual void VAddView(shared_ptr<IGameView> pView, ActorId actorId = INVALID_ACTOR_ID);
	virtual void VRemoveView(shared_ptr<IGameView> pView);

	virtual StrongActorPtr VCreateActor(
		const std::string& actorResource, TiXmlElement *overrides, const Matrix& initialTransform) override;
	virtual void VDestroyActor(ActorId actorId) override;
	virtual WeakActorPtr VGetActor(ActorId actorId) override;
	virtual void VMoveActor(ActorId actorId, const Matrix& mat) override;

protected:
	virtual ActorFactory* VCreateActorFactory();
	virtual bool VLoadGameDelegate(TiXmlElement* pData) { return true; }

	RenderEngineApp* m_pApp;
	GameViewList m_GameViews;
	ActorFactory* m_pActorFactory;
	ActorMap m_Actors;
};

