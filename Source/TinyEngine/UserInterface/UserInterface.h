#pragma once
#include "../TinyEngineBase.h"
#include "../TinyEngineInterface.h"
#include "../Graphics3D/Scene.h"

const DWORD g_QuitNoPrompt = MAKELPARAM(-1,-1);
const UINT g_MsgEndModal = (WM_USER+100);

enum
{
	ZORDER_BACKGROUND, 
	ZORDER_LAYER1, 
	ZORDER_LAYER2, 
	ZORDER_LAYER3, 
	ZORDER_TOOLTIP
};


enum MessageBox_Questions {
	QUESTION_WHERES_THE_CD,
	QUESTION_QUIT_GAME,
};

class FontHandler;
class BaseUI;

class BaseUI : public IScreenElement
{
public:
	BaseUI() : m_bIsVisible(true), m_PosX(0), m_PosY(0), m_Width(100), m_Height(100) {}
	virtual void VOnUpdate(const GameTime& gameTime) {}
	virtual HRESULT VOnInitScreenElements() override { return S_OK; }
	virtual HRESULT VOnDeleteScreenElements() override { return S_OK; }
	virtual bool VIsVisible() const override { return m_bIsVisible; }
	virtual void VSetVisible(bool visible) override { m_bIsVisible = visible; }

protected:
	bool m_bIsVisible;
	int m_PosX, m_PosY;
	int m_Width, m_Height;
	boost::optional<int> m_Result;
};

class ScreenElementScene : public IScreenElement, public Scene
{
public:
	ScreenElementScene(shared_ptr<IRenderer> renderer) : Scene(renderer) { }
	virtual ~ScreenElementScene(void)
	{
		DEBUG_WARNING("~ScreenElementScene()");
	}

	virtual void VOnUpdate(const GameTime& gameTime) override { OnUpdate(gameTime); };
	virtual HRESULT VOnInitScreenElements() override { return OnInitScene(); }
	virtual HRESULT VOnDeleteScreenElements() override { return OnDeleteScene(); }
	virtual HRESULT VOnRender(const GameTime& gameTime) override { return OnRender(gameTime); }
	virtual int VGetZOrder() const override { return 0; }
	virtual void VSetZOrder(int zOrder) override { }

	virtual LRESULT CALLBACK VOnMsgProc(AppMsg msg) override { return 0; }

	virtual bool VIsVisible() const override { return true; }
	virtual void VSetVisible(bool visible) override { }
	virtual bool VAddChild(ActorId id, shared_ptr<ISceneNode> kid) { return Scene::AddChild(id, kid); }
};



