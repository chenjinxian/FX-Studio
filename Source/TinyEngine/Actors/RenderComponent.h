#pragma once
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
	
	virtual TiXmlElement* VCreateBaseElement() { return DEBUG_NEW TiXmlElement(VGetComponentName().c_str()); }
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

	virtual const std::string& VGetComponentName() const override { return m_Name; }
	static const std::string m_Name;

protected:
	virtual bool VDelegateInit(TiXmlElement* pData);
	virtual shared_ptr<SceneNode> VCreateSceneNode();
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

	virtual const std::string& VGetComponentName() const override { return m_Name; }
	static const std::string m_Name;

protected:
	virtual bool VDelegateInit(TiXmlElement* pData);
	virtual shared_ptr<SceneNode> VCreateSceneNode();
	virtual void VCreateInheritedXmlElement(TiXmlElement* pBaseElement);

private:
	std::string m_ModelName;
	std::string m_TextureName;
	std::string m_EffectName;
};

class SkyboxRenderComponent : public BaseRenderComponent
{
public:
	SkyboxRenderComponent();
	virtual ~SkyboxRenderComponent();

	virtual const std::string& VGetComponentName() const override { return m_Name; }
	static const std::string m_Name;

protected:
	virtual bool VDelegateInit(TiXmlElement* pData);
	virtual shared_ptr<SceneNode> VCreateSceneNode();
	virtual void VCreateInheritedXmlElement(TiXmlElement* pBaseElement);
};

class LightRenderComponent : public BaseRenderComponent
{
public:
	LightRenderComponent();
	virtual ~LightRenderComponent();

	const std::string& GetModelName() { return m_ModelName; }

protected:
	virtual bool VDelegateInit(TiXmlElement* pData);
	virtual void VCreateInheritedXmlElement(TiXmlElement* pBaseElement);

private:
	std::string m_ModelName;
};

class DirectionalLightComponent : virtual public LightRenderComponent
{
public:
	DirectionalLightComponent();
	virtual ~DirectionalLightComponent();

	const Vector3& GetDirection() { return m_Direction; }
	const Vector3& GetUp() { return m_Up; }
	const Vector3& GetRight() { return m_Right; }

	virtual const std::string& VGetComponentName() const override { return m_Name; }
	static const std::string m_Name;

protected:
	virtual bool VDelegateInit(TiXmlElement* pData);
	virtual shared_ptr<SceneNode> VCreateSceneNode();
	virtual void VCreateInheritedXmlElement(TiXmlElement* pBaseElement);

	Vector3 m_Direction;
	Vector3 m_Up;
	Vector3 m_Right;
};

class PointLightComponent : virtual public LightRenderComponent
{
public:
	PointLightComponent();
	virtual ~PointLightComponent();

	float GetRadius() const { return m_Radius; }

	virtual const std::string& VGetComponentName() const override { return m_Name; }
	static const std::string m_Name;

protected:
	virtual bool VDelegateInit(TiXmlElement* pData);
	virtual shared_ptr<SceneNode> VCreateSceneNode();
	virtual void VCreateInheritedXmlElement(TiXmlElement* pBaseElement);

	float m_Radius;
};

class SpotLightComponent : public DirectionalLightComponent, public PointLightComponent
{
public:
	SpotLightComponent();
	virtual ~SpotLightComponent();

	float GetInnerAngle() const { return m_InnerAngle; }
	float GetOuterAngle() const { return m_OuterAngle; }

	virtual const std::string& VGetComponentName() const override { return m_Name; }
	static const std::string m_Name;

protected:
	virtual bool VDelegateInit(TiXmlElement* pData);
	virtual shared_ptr<SceneNode> VCreateSceneNode();
	virtual void VCreateInheritedXmlElement(TiXmlElement* pBaseElement);

private:
	float m_InnerAngle;
	float m_OuterAngle;
};
