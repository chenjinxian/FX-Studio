#pragma once
#include <stdint.h>

class Actor;
class ActorComponent;

typedef uint32_t ActorId;
typedef uint32_t ComponentId;

const ActorId INVALID_ACTOR_ID = 0;
const ComponentId INVALID_COMPONENT_ID = 0;

typedef shared_ptr<Actor> StrongActorPtr;
typedef weak_ptr<Actor> WeakActorPtr;
typedef shared_ptr<ActorComponent> StrongActorComponentPtr;
typedef weak_ptr<ActorComponent> WeakActorComponentPtr;

class ISceenElement
{
public:
	ISceenElement() {}
	~ISceenElement() {}

	virtual HRESULT VOnRestore() = 0;
	virtual void VOnUpdate(uint32_t deltaMilliseconds) = 0;
	virtual void VOnRender(float totalTime, float elapsedTime) = 0;
	virtual bool VIsVisible() const = 0;
};

class IGameLogic
{
public:
	IGameLogic() {}
	~IGameLogic() {}

	virtual void VOnUpdate(float totalTime, float elapsedTime) = 0;
	virtual void VLoadGame(const std::string& projectXml) = 0;
};

typedef uint32_t GameViewId;
const GameViewId INVALID_GAME_VIEW_ID = 0xffffffff;

class IGameView
{
public:
	IGameView() {}
	~IGameView() {}

	virtual HRESULT VOnRestore() = 0;
	virtual void VOnUpdate(uint32_t deltaMilliseconds) = 0;
	virtual void VOnRender(float totalTime, float elapsedTime) = 0;
	virtual void VOnAttach(GameViewId viewId, ActorId actorId) = 0;
};

typedef std::list<shared_ptr<ISceenElement> > SceenElementList;
typedef std::list<shared_ptr<IGameView> > GameViewList;

class IRenderer
{
public:
	IRenderer() {}
	~IRenderer() {}

	virtual void VSetBackgroundColor(const Color& color) = 0;
	virtual bool VPreRender() = 0;
	virtual bool VPostRender() = 0;
	virtual void VShutdown() = 0;
};