#pragma once
#include "../RenderEngineInterface.h"
#include "../Graphics3D/Scene.h"

class ScreenElementScene : public IScreenElement, public Scene
{
public:
	ScreenElementScene(shared_ptr<IRenderer> renderer) : Scene(renderer) { }
	virtual ~ScreenElementScene(void)
	{
		GCC_WARNING("~ScreenElementScene()");
	}

	virtual void VOnUpdate(int deltaMS) { OnUpdate(deltaMS); };
	virtual HRESULT VOnRestore() { OnRestore(); return S_OK; }
	virtual HRESULT VOnRender(double fTime, float fElapsedTime) { OnRender(); return S_OK; }
	virtual HRESULT VOnLostDevice() { OnLostDevice(); return S_OK; } 
	virtual int VGetZOrder() const { return 0; }
	virtual void VSetZOrder(int const zOrder) { }

	virtual LRESULT CALLBACK VOnMsgProc( AppMsg msg ) { return 0; }

	virtual bool VIsVisible() const { return true; }
	virtual void VSetVisible(bool visible) { }
	virtual bool VAddChild(ActorId id, shared_ptr<ISceneNode> kid) { return Scene::AddChild(id, kid); }
};



