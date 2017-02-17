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

protected:
	virtual bool VDelegateInit(tinyxml2::XMLElement* pData) { return true; }
	virtual shared_ptr<SceneNode> VCreateSceneNode() = 0;
	
	virtual tinyxml2::XMLElement* VCreateBaseElement(tinyxml2::XMLDocument* pDocument)
	{
		return pDocument->NewElement(VGetComponentName().c_str());
	}
	virtual void VCreateInheritedXmlElement(tinyxml2::XMLElement* pBaseElement, tinyxml2::XMLDocument* pDocument) = 0;

private:
	virtual shared_ptr<SceneNode> VGetSceneNode() override;

	shared_ptr<SceneNode> m_pSceneNode;
};

class GridRenderComponent : public BaseRenderComponent
{
public:
	GridRenderComponent();
	virtual ~GridRenderComponent();

	const std::string& GetTextureName() { return m_TextureName; }
	const Vector2& GetGridSize() const { return m_GridSize; }
	const float GetTicksInterval() const { return m_TicksInterval; }

	virtual const std::string& VGetComponentName() const override { return m_Name; }
	static const std::string m_Name;

protected:
	virtual bool VDelegateInit(tinyxml2::XMLElement* pData);
	virtual shared_ptr<SceneNode> VCreateSceneNode();
	virtual void VCreateInheritedXmlElement(tinyxml2::XMLElement* pBaseElement, tinyxml2::XMLDocument* pDocument);

	std::string m_TextureName;
	Vector2 m_GridSize;
	float m_TicksInterval;
};

class GeometryRenderComponent : public BaseRenderComponent
{
public:
	GeometryRenderComponent();
	virtual ~GeometryRenderComponent();

	const std::string& GetMaterialName() { return m_MaterialName; }

protected:
	virtual bool VDelegateInit(tinyxml2::XMLElement* pData);
	virtual shared_ptr<SceneNode> VCreateSceneNode();
	virtual void VCreateInheritedXmlElement(tinyxml2::XMLElement* pBaseElement, tinyxml2::XMLDocument* pDocument);

	std::string m_MaterialName;
};

class SphereRenderComponent : public GeometryRenderComponent
{
public:
	SphereRenderComponent();
	virtual ~SphereRenderComponent();

	float GetDiameter() const { return m_Diameter; }
	uint32_t GetTessellation() const { return m_Tessellation; }
	bool UseRHcoords() const { return m_RHcoords; }

	virtual const std::string& VGetComponentName() const override { return m_Name; }
	static const std::string m_Name;

protected:
	virtual bool VDelegateInit(tinyxml2::XMLElement* pData);
	virtual void VCreateInheritedXmlElement(tinyxml2::XMLElement* pBaseElement, tinyxml2::XMLDocument* pDocument);

	float m_Diameter;
	uint32_t m_Tessellation;
	bool m_RHcoords;
};

class TorusRenderComponent : public GeometryRenderComponent
{
public:
	TorusRenderComponent();
	virtual ~TorusRenderComponent();

	float GetDiameter() const { return m_Diameter; }
	float GetThickness() const { return m_Thickness; }
	uint32_t GetTessellation() const { return m_Tessellation; }
	bool UseRHcoords() const { return m_RHcoords; }

	virtual const std::string& VGetComponentName() const override { return m_Name; }
	static const std::string m_Name;

protected:
	virtual bool VDelegateInit(tinyxml2::XMLElement* pData);
	virtual void VCreateInheritedXmlElement(tinyxml2::XMLElement* pBaseElement, tinyxml2::XMLDocument* pDocument);

	float m_Diameter;
	float m_Thickness;
	uint32_t m_Tessellation;
	bool m_RHcoords;
};

class TeapotRenderComponent : public GeometryRenderComponent
{
public:
	TeapotRenderComponent();
	virtual ~TeapotRenderComponent();

	float GetSize() const { return m_Size; }
	uint32_t GetTessellation() const { return m_Tessellation; }
	bool UseRHcoords() const { return m_RHcoords; }

	virtual const std::string& VGetComponentName() const override { return m_Name; }
	static const std::string m_Name;

protected:
	virtual bool VDelegateInit(tinyxml2::XMLElement* pData);
	virtual void VCreateInheritedXmlElement(tinyxml2::XMLElement* pBaseElement, tinyxml2::XMLDocument* pDocument);

	float m_Size;
	uint32_t m_Tessellation;
	bool m_RHcoords;
};

class PlaneRenderComponent : public GeometryRenderComponent
{
public:
	PlaneRenderComponent();
	virtual ~PlaneRenderComponent();

	float GetSize() const { return m_Size; }
	bool UseRHcoords() const { return m_RHcoords; }

	virtual const std::string& VGetComponentName() const override { return m_Name; }
	static const std::string m_Name;

protected:
	virtual bool VDelegateInit(tinyxml2::XMLElement* pData);
	virtual void VCreateInheritedXmlElement(tinyxml2::XMLElement* pBaseElement, tinyxml2::XMLDocument* pDocument);

	float m_Size;
	bool m_RHcoords;
};

class ModelRenderComponent : public BaseRenderComponent
{
public:
	ModelRenderComponent();
	virtual ~ModelRenderComponent();

	const std::string& GetModelName() { return m_ModelName; }
	const std::string& GetMaterialName() { return m_MaterialName; }

	virtual const std::string& VGetComponentName() const override { return m_Name; }
	static const std::string m_Name;

protected:
	virtual bool VDelegateInit(tinyxml2::XMLElement* pData);
	virtual shared_ptr<SceneNode> VCreateSceneNode();
	virtual void VCreateInheritedXmlElement(tinyxml2::XMLElement* pBaseElement, tinyxml2::XMLDocument* pDocument);

private:
	std::string m_ModelName;
	std::string m_MaterialName;
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
