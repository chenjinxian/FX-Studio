#pragma once
#include "../RenderEngineBase.h"
#include "../RenderEngineInterface.h"

class Scene
{
public:
	Scene(shared_ptr<IRenderer> pRenderer);
	virtual ~Scene();

	void OnUpdate(uint32_t deltaMilliseconds);
	void OnRender();
	HRESULT OnRestore();

	shared_ptr<IRenderer> GetRenderder() { return m_pRenderer; }

private:
	shared_ptr<IRenderer> m_pRenderer;
};

