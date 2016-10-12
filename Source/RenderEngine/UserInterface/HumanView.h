#pragma once
#include "../RenderEngineBase.h"
#include "../RenderEngineInterface.h"
#include "../AppFramework/BaseGameLogic.h"
#include "../Graphics3D/D3DRenderer.h"
#include "UserInterface.h"

class HumanView : public IGameView
{
	friend class GameCodeApp;

public:
	virtual ~HumanView();
	HumanView(shared_ptr<IRenderer> renderer);

	virtual HRESULT VOnRestore();
	virtual HRESULT VOnLostDevice();
	virtual void VOnUpdate(uint32_t deltaMilliseconds);
	virtual void VOnRender(float fTime, float fElapsedTime);
	virtual GameViewType VGetType() { return GameView_Human; }
	virtual GameViewId VGetId() const { return m_ViewId; }

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

	bool LoadGame(TiXmlElement* pLevelData);
	void TogglePause(bool active);
	ProcessManager* GetProcessManager() { return m_pProcessManager; }
	bool InitAudio();
	void PlaySoundDelegate(IEventDataPtr pEventData);
	void GameStateDelegate(IEventDataPtr pEventData);	

	ScreenElementList m_ScreenElements;
	int m_PointerRadius;
	shared_ptr<IPointerHandler> m_PointerHandler;
	shared_ptr<IKeyboardHandler> m_KeyboardHandler;
	shared_ptr<ScreenElementScene> m_pScene;
	shared_ptr<CameraNode> m_pCamera;

protected:
	virtual void VRenderText() { };
	virtual bool VLoadGameDelegate(TiXmlElement* pLevelData) { VPushElement(m_pScene);  return true; }

	GameViewId m_ViewId;
	ActorId m_ActorId;

	ProcessManager* m_pProcessManager;

	DWORD m_currTick;
	DWORD m_lastDraw;
	bool m_runFullSpeed;

	BaseGameState m_GameState;

private:
	void RegisterAllDelegates(void);
	void RemoveAllDelegates(void);
};


