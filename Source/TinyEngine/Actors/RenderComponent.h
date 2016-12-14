#pragma once
#include "RenderComponentInterface.h"

class BaseRenderComponent : public RenderComponentInterface
{
public:
	BaseRenderComponent();
	virtual ~BaseRenderComponent();

	virtual bool VInit(tinyxml2::XMLElement* pData) override;
	virtual void VPostInit() override;
	virtual void VOnChanged() override;
	virtual tinyxml2::XMLElement* VGenerateXml(tinyxml2::XMLDocument* pDocument) override;
	const Color& GetColor() const { return m_Color; }

protected:
	virtual bool VDelegateInit(tinyxml2::XMLElement* pData) { return true; }
	virtual shared_ptr<SceneNode> VCreateSceneNode() = 0;
	Color LoadColor(tinyxml2::XMLElement* pData);
	
	virtual tinyxml2::XMLElement* VCreateBaseElement(tinyxml2::XMLDocument* pDocument)
	{
		return pDocument->NewElement(VGetComponentName().c_str());
	}
	virtual void VCreateInheritedXmlElement(tinyxml2::XMLElement* pBaseElement, tinyxml2::XMLDocument* pDocument) = 0;

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

	const Color& GetMajorTicksColor() const { return m_MajorTicksColor; }
	const Color& GetTicksColor() const { return m_TicksColor; }

	virtual const std::string& VGetComponentName() const override { return m_Name; }
	static const std::string m_Name;

protected:
	virtual bool VDelegateInit(tinyxml2::XMLElement* pData);
	virtual shared_ptr<SceneNode> VCreateSceneNode();
	virtual void VCreateInheritedXmlElement(tinyxml2::XMLElement* pBaseElement, tinyxml2::XMLDocument* pDocument);

	Color m_MajorTicksColor;
	Color m_TicksColor;
};

class ModelRenderComponent : public BaseRenderComponent
{
public:
	ModelRenderComponent();
	virtual ~ModelRenderComponent();

	const std::string& GetModelName() { return m_ModelName; }
	const std::vector<std::string>& GetTextureName() { return m_TextureNames; }
	const std::string& GetEffectName() { return m_EffectName; }
	const std::string& GetCurrentTechniqueName() { return m_CurrentTechnique; }
	const std::string& GetCurrentPassName() { return m_CurrentPass; }

	virtual const std::string& VGetComponentName() const override { return m_Name; }
	static const std::string m_Name;

protected:
	virtual bool VDelegateInit(tinyxml2::XMLElement* pData);
	virtual shared_ptr<SceneNode> VCreateSceneNode();
	virtual void VCreateInheritedXmlElement(tinyxml2::XMLElement* pBaseElement, tinyxml2::XMLDocument* pDocument);

private:
	std::string m_ModelName;
	std::string m_EffectName;
	std::string m_CurrentTechnique;
	std::string m_CurrentPass;
	std::vector<std::string> m_TextureNames;
};

class SkyboxRenderComponent : public BaseRenderComponent
{
public:
	SkyboxRenderComponent();
	virtual ~SkyboxRenderComponent();

	const std::string& GetTextureName() { return m_TextureName; }

	virtual const std::string& VGetComponentName() const override { return m_Name; }
	static const std::string m_Name;

protected:
	virtual bool VDelegateInit(tinyxml2::XMLElement* pData);
	virtual shared_ptr<SceneNode> VCreateSceneNode();
	virtual void VCreateInheritedXmlElement(tinyxml2::XMLElement* pBaseElement, tinyxml2::XMLDocument* pDocument);

private:
	std::string m_TextureName;
};

class LightRenderComponent : public BaseRenderComponent
{
public:
	LightRenderComponent();
	virtual ~LightRenderComponent();

	const std::string& GetModelName() { return m_ModelName; }

protected:
	virtual bool VDelegateInit(tinyxml2::XMLElement* pData);
	virtual void VCreateInheritedXmlElement(tinyxml2::XMLElement* pBaseElement, tinyxml2::XMLDocument* pDocument);

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
	virtual bool VDelegateInit(tinyxml2::XMLElement* pData);
	virtual shared_ptr<SceneNode> VCreateSceneNode();
	virtual void VCreateInheritedXmlElement(tinyxml2::XMLElement* pBaseElement, tinyxml2::XMLDocument* pDocument);

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
	virtual bool VDelegateInit(tinyxml2::XMLElement* pData);
	virtual shared_ptr<SceneNode> VCreateSceneNode();
	virtual void VCreateInheritedXmlElement(tinyxml2::XMLElement* pBaseElement, tinyxml2::XMLDocument* pDocument);

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
	virtual bool VDelegateInit(tinyxml2::XMLElement* pData);
	virtual shared_ptr<SceneNode> VCreateSceneNode();
	virtual void VCreateInheritedXmlElement(tinyxml2::XMLElement* pBaseElement, tinyxml2::XMLDocument* pDocument);

private:
	float m_InnerAngle;
	float m_OuterAngle;
};
