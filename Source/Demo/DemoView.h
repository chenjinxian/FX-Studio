#pragma once
#include "../TinyEngine/TinyEngine.h"

class MainMenuUI : public BaseUI
{
public:
	MainMenuUI();
	virtual ~MainMenuUI();

	virtual HRESULT VOnInitScreenElements() override;
	virtual HRESULT VOnDeleteScreenElements() override;
	virtual HRESULT VOnRender(const GameTime& gameTime);
	virtual int VGetZOrder() const { return 1; }
	virtual void VSetZOrder(int zOrder) { }

	virtual LRESULT CALLBACK VOnMsgProc(AppMsg msg);

protected:
	void Set();

	std::vector<std::wstring> m_Levels;
	int m_LevelIndex;
};

class MainMenuView : public HumanView
{
public:

	MainMenuView();
	~MainMenuView();

	virtual void VRenderText() override;
	virtual void VOnUpdate(const GameTime& gameTime) override;
	virtual LRESULT CALLBACK VOnMsgProc(AppMsg msg) override;

protected:
	shared_ptr<MainMenuUI> m_MainMenuUI;
};

class DemoView : public HumanView
{
public:
	DemoView(shared_ptr<IRenderer> renderer);
	virtual ~DemoView();

	virtual LRESULT CALLBACK VOnMsgProc(AppMsg msg);
	virtual void VRenderText() override;
	virtual void VOnUpdate(const GameTime& gameTime) override;
	virtual void VOnAttach(GameViewId vid, ActorId aid) override;

	virtual void VSetControlledActor(ActorId actorId);
	virtual bool VLoadGameDelegate(tinyxml2::XMLElement* pLevelData) override;

	void GameplayUiUpdateDelegate(IEventDataPtr pEventData);
	void SetControlledActorDelegate(IEventDataPtr pEventData);

protected:
	shared_ptr<SceneNode> m_pTeapot;
	shared_ptr<StandardHUD> m_StandardHUD;

	bool m_bShowUI;
	std::wstring m_GameplayText;

private:
	void RegisterAllDelegates(void);
	void RemoveAllDelegates(void);
};

