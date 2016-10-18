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
	const Color& GetColor() const { return m_Color; }

protected:
	virtual bool VDelegateInit(TiXmlElement* pData) { return true; }
	virtual shared_ptr<SceneNode> VCreateSceneNode() = 0;
	Color LoadColor(TiXmlElement* pData);
	
	virtual TiXmlElement* VCreateBaseElement() { return GCC_NEW TiXmlElement(VGetName()); }
	virtual void VCreateInheritedXmlElement(TiXmlElement* pBaseElement) = 0;

private:
	virtual shared_ptr<SceneNode> VGetSceneNode() override;

	shared_ptr<SceneNode> m_pSceneNode;
	Color m_Color;
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

class MeshRenderComponent : public BaseRenderComponent
{
public:
	MeshRenderComponent();
	~MeshRenderComponent();

	virtual const char* VGetName() const { return m_Name; }
	static const char* m_Name;

protected:
	virtual shared_ptr<SceneNode> VCreateSceneNode() override;
	virtual void VCreateInheritedXmlElement(TiXmlElement* pBaseElement);
};

class LightRenderComponent : public BaseRenderComponent
{
public:
	LightRenderComponent();
	~LightRenderComponent();

	virtual const char* VGetName() const { return m_Name; }
	static const char* m_Name;

protected:
	virtual bool VDelegateInit(TiXmlElement* pData) override;
	virtual shared_ptr<SceneNode> VCreateSceneNode() override;
	virtual void VCreateInheritedXmlElement(TiXmlElement* pBaseElement);
};

class SkyRenderComponent : public BaseRenderComponent
{
public:
	SkyRenderComponent();
	~SkyRenderComponent();

	virtual const char* VGetName() const { return m_Name; }
	static const char* m_Name;

protected:
	virtual bool VDelegateInit(TiXmlElement* pData) override;
	virtual shared_ptr<SceneNode> VCreateSceneNode() override;
	virtual void VCreateInheritedXmlElement(TiXmlElement* pBaseElement);
};