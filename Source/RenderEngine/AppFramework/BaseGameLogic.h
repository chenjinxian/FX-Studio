#pragma once
#include "../RenderEngineInterface.h"
#include "../Mainloop/ProcessManager.h"

class ActorFactory;
class LevelManager;

enum BaseGameState
{
	BGS_Invalid,
	BGS_Initializing,
	BGS_LoadingGameEnvironment,
	BGS_Running
};

typedef std::map<ActorId, StrongActorPtr> ActorMap;
typedef std::string Level;

class LevelManager
{
public:
	const std::vector<Level> &GetLevels() const { return m_Levels; }
	const int GetCurrentLevel() const { return m_CurrentLevel; }
	bool Initialize(std::vector<std::string> &levels)
	{
		std::vector<std::string>::iterator i = levels.begin();
		while (i != levels.end())
		{
			m_Levels.push_back(*i);
			++i;
		}
		return true;
	}

protected:
	std::vector<Level> m_Levels;
	int m_CurrentLevel;
};

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

	virtual StrongActorPtr VCreateActor(const std::string &actorResource,
		TiXmlElement *overrides, const Matrix *initialTransform = NULL, const ActorId serversActorId = INVALID_ACTOR_ID);
	virtual void VDestroyActor(const ActorId actorId);
	virtual WeakActorPtr VGetActor(const ActorId actorId);
	virtual void VModifyActor(const ActorId actorId, TiXmlElement *overrides);
	virtual void VMoveActor(const ActorId actorId, Matrix const &mat);

	virtual bool VLoadGame(const char* levelResource);
	const LevelManager* GetLevelManager() { return m_pLevelManager; }

	virtual void VChangeState(enum BaseGameState newState);
	const BaseGameState GetState() const { return m_State; }

	virtual void VOnUpdate(float time, float elapsedTime);

protected:
	virtual ActorFactory* VCreateActorFactory(void);
	virtual bool VLoadGameDelegate(TiXmlElement* pLevelData) { return true; }

	RenderEngineApp* m_pApp;
	float m_LifeTime;
	ActorId m_LastActorId;
	ActorMap m_Actors;
	BaseGameState m_State;
	int m_HumanGamesLoaded;
	GameViewList m_GameViews;
	ActorFactory* m_pActorFactory;
	LevelManager* m_pLevelManager;
	ProcessManager* m_pProcessManager;
};

