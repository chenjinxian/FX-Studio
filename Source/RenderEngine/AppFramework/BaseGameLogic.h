#pragma once
#include "../RenderEngineBase.h"
#include "../RenderEngineInterface.h"

class BaseGameLogic : public IGameLogic
{
	friend class RenderEngineApp;

public:
	BaseGameLogic();
	virtual ~BaseGameLogic();

	bool Init();

	virtual void VOnUpdate(float totalTime, float elapsedTime) override;
	virtual void VLoadGame(const std::string& projectXml) override;

	virtual void VAddView(shared_ptr<IGameView> pView);
	virtual void VRemoveView(shared_ptr<IGameView> pView);

private:
	GameViewList m_gameViews;
};

