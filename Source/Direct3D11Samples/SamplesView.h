#pragma once
#include "../RenderEngine/RenderEngine.h"

class MainMenuUI : public BaseUI
{
public:
	MainMenuUI();
	virtual ~MainMenuUI();

	virtual HRESULT VOnRestore();
	virtual HRESULT VOnRender(double fTime, float fElapsedTime);
	virtual int VGetZOrder() const { return 1; }
	virtual void VSetZOrder(int zOrder) { }

	virtual LRESULT CALLBACK VOnMsgProc(AppMsg msg);
	static void CALLBACK OnGUIEvent(UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext);
	void CALLBACK _OnGUIEvent(UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext);

protected:
	void Set();

	CDXUTDialog m_SampleUI;
	std::vector<std::wstring> m_Levels;
	int m_LevelIndex;
};


class StandardHUD : public BaseUI
{
public:
	StandardHUD();
	virtual ~StandardHUD();

	virtual HRESULT VOnRestore();
	virtual HRESULT VOnRender(double fTime, float fElapsedTime);
	virtual int VGetZOrder() const { return 1; }
	virtual void VSetZOrder(int zOrder) { }

	virtual LRESULT CALLBACK VOnMsgProc(AppMsg msg);
	static void CALLBACK OnGUIEvent(UINT nEvent, int nControlID, CDXUTControl* pControl, void *pUserContext);

protected:
	CDXUTDialog m_HUD;
};

class IEventManager;
class TeapotController;

class MainMenuView : public HumanView
{
public:

	MainMenuView();
	~MainMenuView();

	virtual void VRenderText() override;
	virtual void VOnUpdate(double fTime, float fElapsedTime) override;
	virtual LRESULT CALLBACK VOnMsgProc(AppMsg msg) override;

protected:
	shared_ptr<MainMenuUI> m_MainMenuUI;
};

class SamplesView : public HumanView
{
public:
	SamplesView(shared_ptr<IRenderer> renderer);
	virtual ~SamplesView();

	virtual LRESULT CALLBACK VOnMsgProc(AppMsg msg);
	virtual void VRenderText() override;
	virtual void VOnUpdate(double fTime, float fElapsedTime) override;
	virtual void VOnAttach(GameViewId vid, ActorId aid) override;

	virtual void VSetControlledActor(ActorId actorId);
	virtual bool VLoadGameDelegate(TiXmlElement* pLevelData) override;

	void GameplayUiUpdateDelegate(IEventDataPtr pEventData);
	void SetControlledActorDelegate(IEventDataPtr pEventData);

protected:
// 	shared_ptr<TeapotController> m_pTeapotController;
	shared_ptr<SceneNode> m_pTeapot;
	shared_ptr<StandardHUD> m_StandardHUD;

	bool m_bShowUI;
	std::wstring m_GameplayText;

private:
	void RegisterAllDelegates(void);
	void RemoveAllDelegates(void);
};

