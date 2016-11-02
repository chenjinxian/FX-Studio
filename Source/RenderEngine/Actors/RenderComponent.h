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
	virtual ~GridRenderComponent();

	virtual const char* VGetName() const { return m_Name; }
	static const char* m_Name;

protected:
	virtual bool VDelegateInit(TiXmlElement* pData) override;
	virtual shared_ptr<SceneNode> VCreateSceneNode() override;
	virtual void VCreateInheritedXmlElement(TiXmlElement* pBaseElement);
};

class ModelRenderComponent : public BaseRenderComponent
{
public:
	ModelRenderComponent();
	virtual ~ModelRenderComponent();

	const std::string& GetModelName() { return m_ModelName; }
	const std::string& GetTextureName() { return m_TextureName; }
	const std::string& GetEffectName() { return m_EffectName; }

	virtual const char* VGetName() const { return m_Name; }
	static const char* m_Name;

protected:
	virtual bool VDelegateInit(TiXmlElement* pData) override;
	virtual shared_ptr<SceneNode> VCreateSceneNode() override;
	virtual void VCreateInheritedXmlElement(TiXmlElement* pBaseElement);

private:
	std::string m_ModelName;
	std::string m_TextureName;
	std::string m_EffectName;
};

class LightRenderComponent : public BaseRenderComponent
{
public:
	LightRenderComponent();
	virtual ~LightRenderComponent();

	const std::string& GetModelName() { return m_ModelName; }

protected:
	virtual bool VDelegateInit(TiXmlElement* pData) override;
	virtual shared_ptr<SceneNode> VCreateSceneNode() override;
	virtual void VCreateInheritedXmlElement(TiXmlElement* pBaseElement);

private:
	std::string m_ModelName;
};

class DirectionalLightComponent : virtual public LightRenderComponent
{
public:
	DirectionalLightComponent();
	virtual ~DirectionalLightComponent();

	virtual const char* VGetName() const { return m_Name; }
	static const char* m_Name;

protected:
	virtual bool VDelegateInit(TiXmlElement* pData) override;
	virtual shared_ptr<SceneNode> VCreateSceneNode() override;
	virtual void VCreateInheritedXmlElement(TiXmlElement* pBaseElement);

private:
	Vector3 m_Direction;
	Vector3 m_Up;
	Vector3 m_Right;
};

class PointLightComponent : virtual public LightRenderComponent
{
public:
	PointLightComponent();
	virtual ~PointLightComponent();

	virtual const char* VGetName() const { return m_Name; }
	static const char* m_Name;

protected:
	virtual bool VDelegateInit(TiXmlElement* pData) override;
	virtual shared_ptr<SceneNode> VCreateSceneNode() override;
	virtual void VCreateInheritedXmlElement(TiXmlElement* pBaseElement);

private:
	float m_Radius;
};

class SpotLightComponent : public DirectionalLightComponent, public PointLightComponent
{
public:
	SpotLightComponent();
	virtual ~SpotLightComponent();

	virtual const char* VGetName() const { return m_Name; }
	static const char* m_Name;

protected:
	virtual bool VDelegateInit(TiXmlElement* pData) override;
	virtual shared_ptr<SceneNode> VCreateSceneNode() override;
	virtual void VCreateInheritedXmlElement(TiXmlElement* pBaseElement);

private:
	float m_InnerAngle;
	float m_OuterAngle;
};

class SkyRenderComponent : public BaseRenderComponent
{
public:
	SkyRenderComponent();
	virtual ~SkyRenderComponent();

	virtual const char* VGetName() const { return m_Name; }
	static const char* m_Name;

protected:
	virtual bool VDelegateInit(TiXmlElement* pData) override;
	virtual shared_ptr<SceneNode> VCreateSceneNode() override;
	virtual void VCreateInheritedXmlElement(TiXmlElement* pBaseElement);
};