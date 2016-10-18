#pragma once
#include "../RenderEngine/RenderEngine.h"

class FXStudioView : public HumanView
{
public:
	FXStudioView(shared_ptr<IRenderer> pRenderer);
	virtual ~FXStudioView();

	virtual void VOnUpdate(double fTime, float fElapsedTime) override;
	shared_ptr<SceenElementScene> GetScene() { return m_pScene; }

protected:
	virtual bool VLoadGameDelegate(TiXmlElement* pProjectData) override;
};

