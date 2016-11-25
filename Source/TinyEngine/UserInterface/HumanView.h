#pragma once
#include "UserInterface.h"
#include "../AppFramework/BaseGameLogic.h"

class MovementController;

class HumanView : public IGameView
{
	friend class GameCodeApp;

public:
	HumanView(shared_ptr<IRenderer> renderer);
	virtual ~HumanView();

	virtual HRESULT VOnInitGameViews() override;
	virtual HRESULT VOnDeleteGameViews() override;
	virtual void VOnUpdate(const GameTime& gameTime) override;
	virtual void VOnRender(const GameTime& gameTime) override;
	virtual GameViewType VGetType()  override { return GameView_Human; }
	virtual GameViewId VGetId() const  override { return m_ViewId; }

	virtual void VOnAttach(GameViewId vid, ActorId aid)
	{
		m_ViewId = vid; 
		m_ActorId = aid;
	}
	virtual LRESULT CALLBACK VOnMsgProc( AppMsg msg );

	virtual void VPushElement(shared_ptr<IScreenElement> pElement);
	virtual void VRemoveElement(shared_ptr<IScreenElement> pElement);

	virtual void VSetCameraOffset(const Vector4& camOffset);
	virtual void VSetControlledActor(ActorId actorId) { m_ActorId = actorId; }

	bool LoadGame(tinyxml2::XMLElement* pLevelData);
	void TogglePause(bool active);
	bool InitAudio();
	void PlaySoundDelegate(IEventDataPtr pEventData);
	void GameStateDelegate(IEventDataPtr pEventData);	

	ScreenElementList m_ScreenElements;
	int m_PointerRadius;
	shared_ptr<IPointerHandler> m_pPointerHandler;
	shared_ptr<IKeyboardHandler> m_pKeyboardHandler;
	shared_ptr<MovementController> m_pFreeCameraController;
	shared_ptr<ScreenElementScene> m_pScene;
	shared_ptr<CameraNode> m_pCamera;

protected:
	virtual void VRenderText(const GameTime& gameTime) { };
	virtual bool VLoadGameDelegate(tinyxml2::XMLElement* pLevelData) { VPushElement(m_pScene);  return true; }

	GameViewId m_ViewId;
	ActorId m_ActorId;

	BaseGameState m_GameState;

private:
	void RegisterAllDelegates(void);
	void RemoveAllDelegates(void);
};


