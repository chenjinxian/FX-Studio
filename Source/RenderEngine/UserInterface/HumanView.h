#pragma once

class HumanView : public IGameView
{
	friend class GameCodeApp;

protected:
	GameViewId m_ViewId;
	ActorId m_ActorId;

	ProcessManager* m_pProcessManager;				// strictly for things like button animations, etc.

	DWORD m_currTick;		// time right now
	DWORD m_lastDraw;		// last time the game rendered
	bool m_runFullSpeed;	// set to true if you want to run full speed

	BaseGameState m_BaseGameState;					// Added post-press - what is the current game state

	virtual void VRenderText() { };

public:
	bool LoadGame(TiXmlElement* pLevelData);
protected:
	virtual bool VLoadGameDelegate(TiXmlElement* pLevelData) {   VPushElement(m_pScene);  return true; }

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

	void TogglePause(bool active);

	virtual ~HumanView();
	HumanView(shared_ptr<IRenderer> renderer);

	ScreenElementList m_ScreenElements;						// a game screen entity

	// Interface sensitive objects
	shared_ptr<IPointerHandler> m_PointerHandler;
	int m_PointerRadius;
	shared_ptr<IKeyboardHandler> m_KeyboardHandler;

	// Audio
	bool InitAudio();
	ProcessManager* GetProcessManager() { return m_pProcessManager; }

	//Camera adjustments.
	virtual void VSetCameraOffset(const Vec4 & camOffset );

	// Added post press
	shared_ptr<ScreenElementScene> m_pScene;			
	shared_ptr<CameraNode> m_pCamera;					

	void HandleGameState(BaseGameState newState);		

	// Added post press - this helps the network system attach views to the right actor.
	virtual void VSetControlledActor(ActorId actorId) { m_ActorId = actorId; }

	// Event delegates
	void PlaySoundDelegate(IEventDataPtr pEventData);	
	void GameStateDelegate(IEventDataPtr pEventData);	

private:
	void RegisterAllDelegates(void);
	void RemoveAllDelegates(void);

public:
	// Class Console						- not described in the book
	//
	// This class offers a method for entering script commands at runtime.
	// Activate it by pressing the Tilde (~) key during runtime, and type
	// "exit" or "~" and pressing enter.
	class Console
	{
	public:
		Console(void);

		~Console(void);

		void AddDisplayText( const std::string & newText );
		void SetDisplayText( const std::string & newText );

		void SetActive( const bool bIsActive )  { m_bActive = bIsActive; }
		bool IsActive(void) const  { return m_bActive; }

		void HandleKeyboardInput( const unsigned int keyVal, const unsigned int oemKeyVal, const bool bKeyDown );

		void Update( const int deltaMilliseconds );

		void Render( );

	private:
		bool m_bActive;

		std::queue<std::string> m_DisplayStrings;

		RECT m_ConsoleOutputRect;	//Where results get shown
		RECT m_ConsoleInputRect;	//Where input is entered

		Color m_InputColor;
		Color m_OutputColor;

		std::string m_CurrentOutputString;	//What's the current output string?
		std::string m_CurrentInputString;	//What's the current input string?

		int m_ConsoleInputSize;	//Height of the input console window

		int m_CursorBlinkTimer;	//Countdown to toggle cursor blink state
		bool m_bCursorOn;	//Is the cursor currently displayed?

		bool m_bShiftDown;	//Is the shift button down?
		bool m_bCapsLockDown;	//Is the caps lock button down?

		//If this is true, we have a string to execute on our next update cycle.  
		//We have to do this because otherwise the user could interrupt in the midst
		//of an ::Update() call in another system.  This causes problems.
		bool m_bExecuteStringOnUpdate;
	};

	Console & GetConsole( void )
	{
		return m_Console;
	}

protected:
	Console m_Console;
};


