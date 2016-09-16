#include "BaseGameLogic.h"



BaseGameLogic::BaseGameLogic()
{
}


BaseGameLogic::~BaseGameLogic()
{
}

bool BaseGameLogic::Init()
{
	return true;
}

void BaseGameLogic::VOnUpdate(float totalTime, float elapsedTime)
{
	int deltaMilliseconds = int(elapsedTime * 1000.0f);
	for (auto view : m_gameViews)
	{
		view->VOnUpdate(deltaMilliseconds);
	}
}

void BaseGameLogic::VLoadGame(const std::string& projectXml)
{

}

void BaseGameLogic::VAddView(shared_ptr<IGameView> pView)
{
	m_gameViews.push_back(pView);
}

void BaseGameLogic::VRemoveView(shared_ptr<IGameView> pView)
{
	m_gameViews.remove(pView);
}
