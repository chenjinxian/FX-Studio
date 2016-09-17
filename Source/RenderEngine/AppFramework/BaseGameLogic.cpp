#include "BaseGameLogic.h"
#include "../Actors/ActorFactory.h"
#include "../Actors/Actor.h"

BaseGameLogic::BaseGameLogic() : m_pActorFactory(nullptr)
{
}


BaseGameLogic::~BaseGameLogic()
{
	while (!m_GameViews.empty())
	{
		m_GameViews.pop_front();
	}

	SAFE_DELETE(m_pActorFactory);

	for (auto actor : m_Actors)
	{
		actor.second->Destroy();
	}
	m_Actors.clear();
}

bool BaseGameLogic::Init()
{
	m_pActorFactory = VCreateActorFactory();

	return true;
}

void BaseGameLogic::VOnUpdate(float totalTime, float elapsedTime)
{
	int deltaMilliseconds = int(elapsedTime * 1000.0f);
	for (auto view : m_GameViews)
	{
		view->VOnUpdate(deltaMilliseconds);
	}

	for (auto actor : m_Actors)
	{
		actor.second->Update(deltaMilliseconds);
	}
}

void BaseGameLogic::VLoadGame(const std::string& projectXml)
{

}

void BaseGameLogic::VAddView(shared_ptr<IGameView> pView, ActorId actorId)
{
	uint32_t viewId = m_GameViews.size();
	m_GameViews.push_back(pView);
	pView->VOnAttach(viewId, actorId);
	pView->VOnRestore();
}

void BaseGameLogic::VRemoveView(shared_ptr<IGameView> pView)
{
	m_GameViews.remove(pView);
}

ActorFactory* BaseGameLogic::VCreateActorFactory()
{
	return GCC_NEW ActorFactory();
}
