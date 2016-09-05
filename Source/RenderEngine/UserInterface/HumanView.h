#pragma once
#include "../RenderEngineInterface.h"
#include "../RenderEngineApp/BaseGameLogic.h"

class ProcessManager;

class HumanView : public IGameView
{
	friend class GameCodeApp;

protected:
	GameViewId m_ViewId;
	ActorId m_ActorId;
	weak_ptr<IRenderer> m_pRender;
	ProcessManager* m_pProcessManager;				// strictly for things like button animations, etc.

	DWORD m_currTick;		// time right now
	DWORD m_lastDraw;		// last time the game rendered
	bool m_runFullSpeed;	// set to true if you want to run full speed

	BaseGameState m_BaseGameState;					// Added post-press - what is the current game state

	virtual void VRenderText() { };

public:
	bool LoadGame(TiXmlElement* pLevelData);
protected:
	virtual bool VLoadGameDelegate(TiXmlElement* pLevelData) {   /*VPushElement(m_pScene);*/  return true; }

public:
	// Implement the IGameView interface, except for the VOnRender() method, which is renderer specific
	virtual HRESULT VOnRestore();
	virtual HRESULT VOnLostDevice();
	virtual void VOnRender(double fTime, float fElapsedTime);
	virtual GameViewType VGetType() { return GameView_Human; }
	virtual GameViewId VGetId() const { return m_ViewId; }

	virtual void VOnAttach(GameViewId vid, ActorId aid)
	{
		m_ViewId = vid; 
		m_ActorId = aid;
	}
	virtual LRESULT CALLBACK VOnMsgProc( AppMsg msg );
	virtual void VOnUpdate(const int deltaMilliseconds );

	// Virtual methods to control the layering of interface elements
	virtual void VPushElement(shared_ptr<IScreenElement> pElement);
	virtual void VRemoveElement(shared_ptr<IScreenElement> pElement);

	virtual ~HumanView();
	HumanView(shared_ptr<IRenderer> pRender);

	ScreenElementList m_ScreenElements;						// a game screen entity

	// Interface sensitive objects
	shared_ptr<IPointerHandler> m_PointerHandler;
	int m_PointerRadius;
	shared_ptr<IKeyboardHandler> m_KeyboardHandler;

	ProcessManager* GetProcessManager() { return m_pProcessManager; }

// 	virtual void VSetCameraOffset(const Vector4 & camOffset );

// 	shared_ptr<ScreenElementScene> m_pScene;			
// 	shared_ptr<CameraNode> m_pCamera;					

	// Added post press - this helps the network system attach views to the right actor.
	virtual void VSetControlledActor(ActorId actorId) { m_ActorId = actorId; }

	// Event delegates
// 	void PlaySoundDelegate(IEventDataPtr pEventData);	
// 	void GameStateDelegate(IEventDataPtr pEventData);	

private:
// 	void RegisterAllDelegates(void);
// 	void RemoveAllDelegates(void);
};


