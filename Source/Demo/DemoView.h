#pragma once
#include "../TinyEngine/TinyEngine.h"

class MainMenuUI : public BaseUI
{
public:
	MainMenuUI();
	virtual ~MainMenuUI();

	virtual HRESULT VOnInitScreenElements() override;
	virtual HRESULT VOnDeleteScreenElements() override;
	virtual HRESULT VOnRender(const GameTime& gameTime) override;
	virtual int VGetZOrder() const override { return 1; }
	virtual void VSetZOrder(int zOrder) override { }

	virtual LRESULT CALLBACK VOnMsgProc(AppMsg msg);

private:
	uint32_t m_UIWidth;
	uint32_t m_UIHeight;
	uint32_t m_UIPositionX;
	uint32_t m_UIPositionY;
	int m_CurrentSelect;
};

class MainMenuView : public HumanView
{
public:
	MainMenuView();
	~MainMenuView();

	virtual void VRenderText(const GameTime& gameTime) override;
	virtual void VOnUpdate(const GameTime& gameTime) override;
	virtual LRESULT CALLBACK VOnMsgProc(AppMsg msg) override;

protected:
	shared_ptr<MainMenuUI> m_MainMenuUI;
};

class StandardHUD : public BaseUI
{
public:
	StandardHUD();
	virtual ~StandardHUD();

	virtual HRESULT VOnInitScreenElements() override;
	virtual HRESULT VOnDeleteScreenElements() override;
	virtual HRESULT VOnRender(const GameTime& gameTime) override;
	virtual int VGetZOrder() const override { return 1; }
	virtual void VSetZOrder(int zOrder) override { }

	virtual LRESULT CALLBACK VOnMsgProc(AppMsg msg);

private:
	uint32_t m_UIWidth;
	uint32_t m_UIHeight;
	uint32_t m_UIPositionX;
	uint32_t m_UIPositionY;
	bool m_isShowConfig;
};

class DemoView : public HumanView
{
public:
	DemoView(shared_ptr<IRenderer> renderer);
	virtual ~DemoView();

	virtual LRESULT CALLBACK VOnMsgProc(AppMsg msg);
	virtual void VRenderText(const GameTime& gameTime) override;
	virtual void VOnUpdate(const GameTime& gameTime) override;
	virtual void VOnAttach(GameViewId vid, ActorId aid) override;

	virtual void VSetControlledActor(ActorId actorId);
	virtual bool VLoadGameDelegate(tinyxml2::XMLElement* pLevelData) override;

	void GameplayUiUpdateDelegate(IEventDataPtr pEventData);
	void SetControlledActorDelegate(IEventDataPtr pEventData);

protected:
	shared_ptr<SceneNode> m_pTeapot;
	shared_ptr<StandardHUD> m_StandardHUD;

	std::wstring m_GameplayText;

private:
	void RegisterAllDelegates(void);
	void RemoveAllDelegates(void);
};

