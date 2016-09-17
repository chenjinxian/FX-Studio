#pragma once
#include "../RenderEngine/RenderEngine.h"

class FXStudioView : public HumanView
{
public:
	FXStudioView(shared_ptr<IRenderer> pRenderer);
	virtual ~FXStudioView();

	virtual void VOnUpdate(uint32_t deltaMs) override;
	shared_ptr<SceenElementScene> GetScene() { return m_pScene; }

protected:
	virtual bool VLoadGameDelegate(TiXmlElement* pProjectData) override;
};

