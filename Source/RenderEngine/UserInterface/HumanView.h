#pragma once
#include "../RenderEngineInterface.h"
#include "../RenderEngineApp/BaseGameLogic.h"

class ScreenElementScene;
class CameraNode;

class HumanView : public IGameView
{
	friend class GameCodeApp;

public:
	HumanView(shared_ptr<IRenderer> pRender);
	virtual ~HumanView();

	bool LoadGame(TiXmlElement* pLevelData);

	virtual HRESULT VOnRestore() override;
	virtual HRESULT VOnLostDevice() override;
	virtual void VOnRender(double fTime, float fElapsedTime) override;
	virtual GameViewId VGetId() const override { return m_ViewId; }

	virtual void VOnAttach(GameViewId vid, ActorId aid) override
	{
		m_ViewId = vid;
		m_ActorId = aid;
	}
	virtual LRESULT CALLBACK VOnMsgProc(AppMsg msg) override;
	virtual void VOnUpdate(const int deltaMilliseconds) override;

	virtual void VPushElement(shared_ptr<IScreenElement> pElement);
	virtual void VRemoveElement(shared_ptr<IScreenElement> pElement);
	virtual void VRenderText() { };
	virtual void VSetCameraOffset(const Vector4& camOffset);

protected:
	virtual bool VLoadGameDelegate(TiXmlElement* pLevelData) {   /*VPushElement(m_pScene);*/  return true; }

	GameViewId m_ViewId;
	ActorId m_ActorId;
	weak_ptr<IRenderer> m_pRender;

	DWORD m_currTick;
	DWORD m_lastDraw;
	bool m_runFullSpeed;
	
public:
	ScreenElementList m_ScreenElements;
	shared_ptr<IPointerHandler> m_PointerHandler;
	shared_ptr<IKeyboardHandler> m_KeyboardHandler;
	shared_ptr<ScreenElementScene> m_pScene;
	shared_ptr<CameraNode> m_pCamera;
	int m_PointerRadius;
};


