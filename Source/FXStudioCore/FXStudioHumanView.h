#pragma once

#include "../RenderEngine/RenderEngineInterface.h"

class FXStudioHumanView : public HumanView
{
public:
	FXStudioHumanView(shared_ptr<IRenderer> pRender);
	virtual ~FXStudioHumanView();

	virtual void VOnUpdate(unsigned long deltaMilliseconds);
// 	shared_ptr<ScreenElementScene> GetScene() { return m_pScene; }

protected:
// 	shared_ptr<MovementController> m_pFreeCameraController;
	virtual bool VLoadGameDelegate(TiXmlElement* pLevelData) override;
};

