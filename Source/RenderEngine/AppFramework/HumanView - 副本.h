#pragma once
#include "../RenderEngineBase.h"
#include "../RenderEngineInterface.h"
#include "../Graphics3D/Scene.h"

class SceenElementScene : public ISceenElement, public Scene
{
public:
	SceenElementScene(shared_ptr<IRenderer> pRenderer) : Scene(pRenderer) {}
	virtual ~SceenElementScene() {}

	virtual HRESULT VOnRestore() override { return OnRestore(); }
	virtual void VOnUpdate(uint32_t deltaMs) override { OnUpdate(deltaMs); }
	virtual void VOnRender(float totalTime, float elapsedTime) override { OnRender(); }
	virtual bool VIsVisible() const override { return true; }
};

class HumanView : public IGameView
{
	friend class RenderEngineApp;

public:
	HumanView(shared_ptr<IRenderer> pRenderer);
	virtual ~HumanView();
	
	virtual HRESULT VOnRestore() override;
	virtual void VOnUpdate(uint32_t deltaMs) override;
	virtual void VOnRender(float totalTime, float elapsedTime) override;
	virtual void VOnAttach(GameViewId viewId, ActorId actorId) override;

	virtual void VPushElement(shared_ptr<ISceenElement> pElement);
	virtual void VRemoveElement(shared_ptr<ISceenElement> pElement);

	bool LoadGame(TiXmlElement* pData);

protected:
	virtual bool VLoadGameDelegate(TiXmlElement* pProjectData) { VPushElement(m_pScene); return true; }

public:
	SceenElementList m_SceenElements;
	shared_ptr<SceenElementScene> m_pScene;

private:
	uint32_t m_CurrentTick;
	uint32_t m_LastDraw;
	weak_ptr<IRenderer> m_pRenderer;
	GameViewId m_ViewId;
	ActorId m_ActorId;
};

