#pragma once
#include "../TinyEngine/TinyEngine.h"
#include "ModelController.h"

class FXStudioView : public HumanView
{
public:
	FXStudioView(shared_ptr<IRenderer> pRenderer);
	virtual ~FXStudioView();

	virtual void VOnUpdate(const GameTime& gameTime) override;
	shared_ptr<ScreenElementScene> GetScene() { return m_pScene; }

protected:
	virtual bool VLoadGameDelegate(tinyxml2::XMLElement* pCameraNode) override;
	
private:
	shared_ptr<ModelController> m_pModelController;
};

