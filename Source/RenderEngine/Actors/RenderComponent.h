#pragma once
#include "../RenderEngineBase.h"
#include "../RenderEngineInterface.h"
#include "RenderComponentInterface.h"

class BaseRenderComponent : public RenderComponentInterface
{
public:
	BaseRenderComponent();
	virtual ~BaseRenderComponent();

	virtual bool VInit(TiXmlElement* pData) override;
	virtual void VPostInit() override;
	virtual void VOnChanged() override;
	virtual TiXmlElement* VGenerateXml() override;

protected:
	virtual bool VDelegateInit(TiXmlElement* pData) { return true; }
	virtual shared_ptr<SceneNode> VCreateSceneNode() = 0;
	
	virtual TiXmlElement* VCreateBaseElement() { return GCC_NEW TiXmlElement(VGetName()); }
	virtual void VCreateInheritedXmlElement(TiXmlElement* pBaseElement) = 0;

private:
	virtual shared_ptr<SceneNode> VGetSceneNode() override;

	shared_ptr<SceneNode> m_pSceneNode;
};

class GridRenderComponent : public BaseRenderComponent
{
public:
	GridRenderComponent();
	~GridRenderComponent();

	virtual const char* VGetName() const { return m_Name; }
	static const char* m_Name;

protected:
	virtual bool VDelegateInit(TiXmlElement* pData) override;
	virtual shared_ptr<SceneNode> VCreateSceneNode() override;
	virtual void VCreateInheritedXmlElement(TiXmlElement* pBaseElement);
};

