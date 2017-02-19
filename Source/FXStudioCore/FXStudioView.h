#pragma once
#include "../TinyEngine/TinyEngine.h"
#include "ModelController.h"

class MaterialNode;

class FXStudioView : public HumanView
{
public:
	FXStudioView(shared_ptr<IRenderer> pRenderer);
	virtual ~FXStudioView();

	virtual void VOnUpdate(const GameTime& gameTime) override;
	shared_ptr<ScreenElementScene> GetScene() { return m_pScene; }
	shared_ptr<DebugGizmosNode> GetGizmosNode() { return m_pGizmosNode; }
	void MoveEditorCamera(tinyxml2::XMLElement* pCameraNode);
	tinyxml2::XMLElement* GenerateCameraXml(tinyxml2::XMLDocument* pDocument);
	void SetCameraType(int type);
	void AddMaterial(const std::string& materialName, const std::string& bmpPath);

protected:
	virtual void VRenderText(const GameTime& gameTime) override;
	virtual bool VLoadGameDelegate(tinyxml2::XMLElement* pCameraNode) override;
	
	virtual HRESULT VOnInitGameViews(bool onlyCamera) override;
	virtual HRESULT VOnDeleteGameViews(bool onlyCamera) override;
	virtual void VOnRender(const GameTime& gameTime) override;

private:
	shared_ptr<ModelController> m_pModelController;
	shared_ptr<CameraNode> m_pEditorCamera;
	shared_ptr<DebugGizmosNode> m_pGizmosNode;
};

