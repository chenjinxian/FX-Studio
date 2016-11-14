#include "RenderComponent.h"
#include "Actor.h"
#include "TransformComponent.h"
#include "../EventManager/Events.h"
#include "boost/lexical_cast.hpp"

const std::string GridRenderComponent::m_Name = "GridRenderComponent";
const std::string ModelRenderComponent::m_Name = "ModelRenderComponent";
const std::string SkyboxRenderComponent::m_Name = "SkyboxRenderComponent";
const std::string DirectionalLightComponent::m_Name = "DirectionalLightComponent";
const std::string PointLightComponent::m_Name = "PointLightComponent";
const std::string SpotLightComponent::m_Name = "SpotLightComponent";

BaseRenderComponent::BaseRenderComponent() : m_pSceneNode(nullptr)
{
}

BaseRenderComponent::~BaseRenderComponent()
{
}

bool BaseRenderComponent::VInit(TiXmlElement* pData)
{
	TiXmlElement* pColorNode = pData->FirstChildElement("Color");
	if (pColorNode != nullptr)
	{
		m_Color = LoadColor(pColorNode);
	}

	return VDelegateInit(pData);
}

void BaseRenderComponent::VPostInit()
{
	shared_ptr<SceneNode> pSceneNode(VGetSceneNode());
	shared_ptr<EvtData_New_Render_Component> pEvent(DEBUG_NEW EvtData_New_Render_Component(m_pOwner->GetActorId(), pSceneNode));
	IEventManager::Get()->VTriggerEvent(pEvent);
}

void BaseRenderComponent::VOnChanged()
{
	shared_ptr<EvtData_Modified_Render_Component> pEvent(DEBUG_NEW EvtData_Modified_Render_Component(m_pOwner->GetActorId()));
	IEventManager::Get()->VTriggerEvent(pEvent);
}

TiXmlElement* BaseRenderComponent::VGenerateXml()
{
	TiXmlElement* pBaseElement = VCreateBaseElement();

	TiXmlElement* pColor = DEBUG_NEW TiXmlElement("Color");
	pColor->SetAttribute("r", boost::lexical_cast<std::string>(m_Color.x).c_str());
	pColor->SetAttribute("g", boost::lexical_cast<std::string>(m_Color.y).c_str());
	pColor->SetAttribute("b", boost::lexical_cast<std::string>(m_Color.z).c_str());
	pColor->SetAttribute("a", boost::lexical_cast<std::string>(m_Color.w).c_str());
	pBaseElement->LinkEndChild(pColor);

	VCreateInheritedXmlElement(pBaseElement);
	return pBaseElement;
}

Color BaseRenderComponent::LoadColor(TiXmlElement* pData)
{
	Color color;

	double r = 1.0;
	double g = 1.0;
	double b = 1.0;
	double a = 1.0;

	pData->Attribute("r", &r);
	pData->Attribute("g", &g);
	pData->Attribute("b", &b);
	pData->Attribute("a", &a);

	color.x = (float)r;
	color.y = (float)g;
	color.z = (float)b;
	color.w = (float)a;

	return color;
}

shared_ptr<SceneNode> BaseRenderComponent::VGetSceneNode()
{
	if (m_pSceneNode == nullptr)
	{
		m_pSceneNode = VCreateSceneNode();
	}
	return m_pSceneNode;
}

GridRenderComponent::GridRenderComponent()
{

}

GridRenderComponent::~GridRenderComponent()
{

}

bool GridRenderComponent::VDelegateInit(TiXmlElement* pData)
{
	return true;
}

shared_ptr<SceneNode> GridRenderComponent::VCreateSceneNode()
{
// 	shared_ptr<TransformComponent> pTransformComponent =
// 		MakeStrongPtr(m_pOwner->GetComponent<TransformComponent>(TransformComponent::m_Name));
// 	if (pTransformComponent)
// 	{
// 		WeakBaseRenderComponentPtr weakThis(this);
// 
// 		switch (BaseGameApp::GetRendererImpl())
// 		{
// 		case BaseGameApp::Renderer_D3D11:
// 			return shared_ptr<SceneNode>(DEBUG_NEW GridNode(m_pOwner->GetActorId(), weakThis, pTransformComponent->GetTransform()));
// 
// 		default:
// 			DEBUG_ERROR("Unknown Renderer Implementation in GridRenderComponent");
// 		}
// 	}

	return shared_ptr<SceneNode>();
}

void GridRenderComponent::VCreateInheritedXmlElement(TiXmlElement* pBaseElement)
{

}

ModelRenderComponent::ModelRenderComponent()
	: m_ModelName(),
	m_TextureName()
{

}

ModelRenderComponent::~ModelRenderComponent()
{

}

bool ModelRenderComponent::VDelegateInit(TiXmlElement* pData)
{
	TiXmlElement* pModel = pData->FirstChildElement("Model");
	if (pModel != nullptr)
	{
		m_ModelName = pModel->FirstChild()->Value();
	}

	TiXmlElement* pTexture = pData->FirstChildElement("Texture");
	if (pTexture != nullptr)
	{
		m_TextureName = pTexture->FirstChild()->Value();
	}

	TiXmlElement* pEffect = pData->FirstChildElement("Effect");
	if (pEffect != nullptr)
	{
		m_EffectName = pEffect->FirstChild()->Value();
	}

	return true;
}

shared_ptr<SceneNode> ModelRenderComponent::VCreateSceneNode()
{
// 	shared_ptr<TransformComponent> pTransformComponent =
// 		MakeStrongPtr(m_pOwner->GetComponent<TransformComponent>(TransformComponent::m_Name));
// 	if (pTransformComponent)
// 	{
// 		WeakBaseRenderComponentPtr weakThis(this);
// 
// 		switch (BaseGameApp::GetRendererImpl())
// 		{
// 		case BaseGameApp::Renderer_D3D11:
// 		{
// 			std::string extension =  m_ModelName.substr(m_ModelName.find_last_of('.'));
// 			if (!extension.compare("sdkmesh"))
// 			{
// 				return shared_ptr<SceneNode>(DEBUG_NEW D3DShaderMeshNode11(
// 					m_pOwner->GetActorId(), weakThis, RenderPass_Actor, pTransformComponent->GetTransform()));
// 			}
// 			else
// 			{
// 				return shared_ptr<SceneNode>(DEBUG_NEW ModelNode(
// 					m_pOwner->GetActorId(), weakThis, RenderPass_Actor, pTransformComponent->GetTransform()));
// 			}
// 		}
// 
// 		default:
// 			DEBUG_ERROR("Unknown Renderer Implementation in ModelRenderComponent");
// 		}
// 	}

	return shared_ptr<SceneNode>();
}

void ModelRenderComponent::VCreateInheritedXmlElement(TiXmlElement* pBaseElement)
{
	TiXmlElement* pSdkMesh = DEBUG_NEW TiXmlElement("Model");
	TiXmlText* pSdkMeshName = DEBUG_NEW TiXmlText(m_ModelName.c_str());
	pSdkMesh->LinkEndChild(pSdkMeshName);
	pBaseElement->LinkEndChild(pSdkMesh);

	TiXmlElement* pTexture = DEBUG_NEW TiXmlElement("Texture");
	TiXmlText* pTextureName = DEBUG_NEW TiXmlText(m_TextureName.c_str());
	pTexture->LinkEndChild(pTextureName);
	pBaseElement->LinkEndChild(pTexture);

	TiXmlElement* pEffect = DEBUG_NEW TiXmlElement("Effect");
	TiXmlText* pEffectName = DEBUG_NEW TiXmlText(m_EffectName.c_str());
	pEffect->LinkEndChild(pEffectName);
	pBaseElement->LinkEndChild(pEffect);
}

SkyboxRenderComponent::SkyboxRenderComponent()
{

}

SkyboxRenderComponent::~SkyboxRenderComponent()
{

}

bool SkyboxRenderComponent::VDelegateInit(TiXmlElement* pData)
{
	TiXmlElement* pTexture = pData->FirstChildElement("Texture");
	if (pTexture)
	{
// 		m_textureResource = pTexture->FirstChild()->Value();
	}
	return true;
}

shared_ptr<SceneNode> SkyboxRenderComponent::VCreateSceneNode()
{
// 	shared_ptr<SkyboxNode> sky;
// 	if (BaseGameApp::GetRendererImpl() == BaseGameApp::Renderer_D3D11)
// 	{
// 		sky = shared_ptr<SkyboxNode>(DEBUG_NEW SkyboxNode(L"Assets\\Textures\\Plains_of_abraham.dds"));
// 	}
// 	else
// 	{
// 		DEBUG_ERROR("Unknown Renderer Implementation in SkyboxRenderComponent");
// 	}
// 	return sky;

	return shared_ptr<SceneNode>();
}

void SkyboxRenderComponent::VCreateInheritedXmlElement(TiXmlElement* pBaseElement)
{
// 	TiXmlElement* pTextureNode = DEBUG_NEW TiXmlElement("Texture");
// 	TiXmlText* pTextureText = DEBUG_NEW TiXmlText(m_textureResource.c_str());
// 	pTextureNode->LinkEndChild(pTextureText);
// 	pBaseElement->LinkEndChild(pTextureNode);
}

LightRenderComponent::LightRenderComponent()
{

}

LightRenderComponent::~LightRenderComponent()
{

}

bool LightRenderComponent::VDelegateInit(TiXmlElement* pData)
{
	TiXmlElement* pModel = pData->FirstChildElement("Model");
	if (pModel != nullptr)
	{
		m_ModelName = pModel->FirstChild()->Value();
	}

	return true;
}

void LightRenderComponent::VCreateInheritedXmlElement(TiXmlElement* pBaseElement)
{
	DEBUG_ASSERT(pBaseElement);
	if (pBaseElement == nullptr)
	{
		return;
	}

	TiXmlElement* pModelNode = DEBUG_NEW TiXmlElement("Model");
	TiXmlText* pModelName = DEBUG_NEW TiXmlText(m_ModelName.c_str());
	pModelNode->LinkEndChild(pModelName);
	pBaseElement->LinkEndChild(pModelNode);
}

DirectionalLightComponent::DirectionalLightComponent()
{

}

DirectionalLightComponent::~DirectionalLightComponent()
{

}

bool DirectionalLightComponent::VDelegateInit(TiXmlElement* pData)
{
	LightRenderComponent::VDelegateInit(pData);

	double x = 0;
	double y = 0;
	double z = 0;
	TiXmlElement* pLight = pData->FirstChildElement("Light");
	TiXmlElement* pDirectionNode = pLight->FirstChildElement("Direction");
	if (pDirectionNode != nullptr)
	{
		pDirectionNode->Attribute("x", &x);
		pDirectionNode->Attribute("y", &y);
		pDirectionNode->Attribute("z", &z);
		m_Direction = Vector3((float)x, (float)y, (float)z);
	}

	TiXmlElement* pUpNode = pLight->FirstChildElement("Up");
	if (pUpNode != nullptr)
	{
		pUpNode->Attribute("x", &x);
		pUpNode->Attribute("y", &y);
		pUpNode->Attribute("z", &z);
		m_Up = Vector3((float)x, (float)y, (float)z);
	}

	TiXmlElement* pRightNode = pLight->FirstChildElement("Right");
	if (pRightNode != nullptr)
	{
		pRightNode->Attribute("x", &x);
		pRightNode->Attribute("y", &y);
		pRightNode->Attribute("z", &z);
		m_Right = Vector3((float)x, (float)y, (float)z);
	}

	return true;
}

shared_ptr<SceneNode> DirectionalLightComponent::VCreateSceneNode()
{
// 	shared_ptr<TransformComponent> pTransformComponent =
// 		MakeStrongPtr(m_pOwner->GetComponent<TransformComponent>(TransformComponent::m_Name));
// 	if (pTransformComponent)
// 	{
// 		WeakBaseRenderComponentPtr weakThis(this);
// 
// 		switch (BaseGameApp::GetRendererImpl())
// 		{
// 		case BaseGameApp::Renderer_D3D11:
// 		{
// 			return shared_ptr<SceneNode>(DEBUG_NEW DirectionalLightNode(
// 				m_pOwner->GetActorId(), weakThis, RenderPass_Actor, pTransformComponent->GetTransform()));
// 		}
// 
// 		default:
// 			DEBUG_ERROR("Unknown Renderer Implementation in LightRenderComponent");
// 		}
// 	}

	return shared_ptr<SceneNode>();
}

void DirectionalLightComponent::VCreateInheritedXmlElement(TiXmlElement* pBaseElement)
{
	LightRenderComponent::VCreateInheritedXmlElement(pBaseElement);

	TiXmlElement* pSceneNode = DEBUG_NEW TiXmlElement("Light");

	TiXmlElement* pDirectionNode = DEBUG_NEW TiXmlElement("Direction");
	pDirectionNode->SetAttribute("x", boost::lexical_cast<std::string>(m_Direction.x).c_str());
	pDirectionNode->SetAttribute("y", boost::lexical_cast<std::string>(m_Direction.y).c_str());
	pDirectionNode->SetAttribute("z", boost::lexical_cast<std::string>(m_Direction.z).c_str());
	pSceneNode->LinkEndChild(pDirectionNode);

	TiXmlElement* pUpNode = DEBUG_NEW TiXmlElement("Up");
	pUpNode->SetAttribute("x", boost::lexical_cast<std::string>(m_Up.x).c_str());
	pUpNode->SetAttribute("y", boost::lexical_cast<std::string>(m_Up.y).c_str());
	pUpNode->SetAttribute("z", boost::lexical_cast<std::string>(m_Up.z).c_str());
	pSceneNode->LinkEndChild(pUpNode);

	TiXmlElement* pRightNode = DEBUG_NEW TiXmlElement("Right");
	pRightNode->SetAttribute("x", boost::lexical_cast<std::string>(m_Right.x).c_str());
	pRightNode->SetAttribute("y", boost::lexical_cast<std::string>(m_Right.y).c_str());
	pRightNode->SetAttribute("z", boost::lexical_cast<std::string>(m_Right.z).c_str());
	pSceneNode->LinkEndChild(pRightNode);

	pBaseElement->LinkEndChild(pSceneNode);
}

PointLightComponent::PointLightComponent()
{

}

PointLightComponent::~PointLightComponent()
{

}

bool PointLightComponent::VDelegateInit(TiXmlElement* pData)
{
	LightRenderComponent::VDelegateInit(pData);

	TiXmlElement* pLight = pData->FirstChildElement("Light");
	TiXmlElement* pShapeNode = pLight->FirstChildElement("Shape");
	if (pShapeNode != nullptr)
	{
		double temp;
		pShapeNode->Attribute("Radius", &temp);
		m_Radius = (float)temp;
	}

	return true;
}

shared_ptr<SceneNode> PointLightComponent::VCreateSceneNode()
{
// 	shared_ptr<TransformComponent> pTransformComponent =
// 		MakeStrongPtr(m_pOwner->GetComponent<TransformComponent>(TransformComponent::m_Name));
// 	if (pTransformComponent)
// 	{
// 		WeakBaseRenderComponentPtr weakThis(this);
// 
// 		switch (BaseGameApp::GetRendererImpl())
// 		{
// 		case BaseGameApp::Renderer_D3D11:
// 		{
// 			// 			return shared_ptr<SceneNode>(DEBUG_NEW D3DShaderMeshNode11(
// 			// 				m_pOwner->GetActorId(), weakThis, RenderPass_Actor, pTransformComponent->GetTransform()));
// 		}
// 
// 		default:
// 			DEBUG_ERROR("Unknown Renderer Implementation in LightRenderComponent");
// 		}
// 	}

	return shared_ptr<SceneNode>();
}

void PointLightComponent::VCreateInheritedXmlElement(TiXmlElement* pBaseElement)
{
	LightRenderComponent::VCreateInheritedXmlElement(pBaseElement);

	TiXmlElement* pSceneNode = DEBUG_NEW TiXmlElement("Light");
	TiXmlElement* pShapeNode = DEBUG_NEW TiXmlElement("Shape");
	pShapeNode->SetAttribute("Radius", boost::lexical_cast<std::string>(m_Radius).c_str());
	pSceneNode->LinkEndChild(pShapeNode);

	pBaseElement->LinkEndChild(pSceneNode);
}

SpotLightComponent::SpotLightComponent()
{

}

SpotLightComponent::~SpotLightComponent()
{

}

bool SpotLightComponent::VDelegateInit(TiXmlElement* pData)
{
	LightRenderComponent::VDelegateInit(pData);

	double x = 0;
	double y = 0;
	double z = 0;
	TiXmlElement* pLight = pData->FirstChildElement("Light");
	TiXmlElement* pDirectionNode = pLight->FirstChildElement("Direction");
	if (pDirectionNode != nullptr)
	{
		pDirectionNode->Attribute("x", &x);
		pDirectionNode->Attribute("y", &y);
		pDirectionNode->Attribute("z", &z);
		m_Direction = Vector3((float)x, (float)y, (float)z);
	}

	TiXmlElement* pUpNode = pLight->FirstChildElement("Up");
	if (pUpNode != nullptr)
	{
		pUpNode->Attribute("x", &x);
		pUpNode->Attribute("y", &y);
		pUpNode->Attribute("z", &z);
		m_Up = Vector3((float)x, (float)y, (float)z);
	}

	TiXmlElement* pRightNode = pLight->FirstChildElement("Right");
	if (pRightNode != nullptr)
	{
		pRightNode->Attribute("x", &x);
		pRightNode->Attribute("y", &y);
		pRightNode->Attribute("z", &z);
		m_Right = Vector3((float)x, (float)y, (float)z);
	}

	TiXmlElement* pShapeNode = pLight->FirstChildElement("Shape");
	if (pShapeNode != nullptr)
	{
		double temp;
		pShapeNode->Attribute("Radius", &temp);
		m_Radius = (float)temp;
		pShapeNode->Attribute("InnerAngle", &temp);
		m_InnerAngle = (float)temp;
		pShapeNode->Attribute("OuterAngle", &temp);
		m_OuterAngle = (float)temp;
	}

	return true;
}

shared_ptr<SceneNode> SpotLightComponent::VCreateSceneNode()
{
// 	shared_ptr<TransformComponent> pTransformComponent =
// 		MakeStrongPtr(m_pOwner->GetComponent<TransformComponent>(TransformComponent::m_Name));
// 	if (pTransformComponent)
// 	{
// 		WeakBaseRenderComponentPtr weakThis(this);
// 
// 		switch (BaseGameApp::GetRendererImpl())
// 		{
// 		case BaseGameApp::Renderer_D3D11:
// 		{
// 			// 			return shared_ptr<SceneNode>(DEBUG_NEW D3DShaderMeshNode11(
// 			// 				m_pOwner->GetActorId(), weakThis, RenderPass_Actor, pTransformComponent->GetTransform()));
// 		}
// 
// 		default:
// 			DEBUG_ERROR("Unknown Renderer Implementation in LightRenderComponent");
// 		}
// 	}

	return shared_ptr<SceneNode>();
}

void SpotLightComponent::VCreateInheritedXmlElement(TiXmlElement* pBaseElement)
{
	LightRenderComponent::VCreateInheritedXmlElement(pBaseElement);

	TiXmlElement* pSceneNode = DEBUG_NEW TiXmlElement("Light");

	TiXmlElement* pDirectionNode = DEBUG_NEW TiXmlElement("Direction");
	pDirectionNode->SetAttribute("x", boost::lexical_cast<std::string>(m_Direction.x).c_str());
	pDirectionNode->SetAttribute("y", boost::lexical_cast<std::string>(m_Direction.y).c_str());
	pDirectionNode->SetAttribute("z", boost::lexical_cast<std::string>(m_Direction.z).c_str());
	pSceneNode->LinkEndChild(pDirectionNode);

	TiXmlElement* pUpNode = DEBUG_NEW TiXmlElement("Up");
	pUpNode->SetAttribute("x", boost::lexical_cast<std::string>(m_Up.x).c_str());
	pUpNode->SetAttribute("y", boost::lexical_cast<std::string>(m_Up.y).c_str());
	pUpNode->SetAttribute("z", boost::lexical_cast<std::string>(m_Up.z).c_str());
	pSceneNode->LinkEndChild(pUpNode);

	TiXmlElement* pRightNode = DEBUG_NEW TiXmlElement("Right");
	pRightNode->SetAttribute("x", boost::lexical_cast<std::string>(m_Right.x).c_str());
	pRightNode->SetAttribute("y", boost::lexical_cast<std::string>(m_Right.y).c_str());
	pRightNode->SetAttribute("z", boost::lexical_cast<std::string>(m_Right.z).c_str());
	pSceneNode->LinkEndChild(pRightNode);

	TiXmlElement* pShapeNode = DEBUG_NEW TiXmlElement("Shape");
	pShapeNode->SetAttribute("Radius", boost::lexical_cast<std::string>(m_Radius).c_str());
	pShapeNode->SetAttribute("InnerAngle", boost::lexical_cast<std::string>(m_InnerAngle).c_str());
	pShapeNode->SetAttribute("OuterAngle", boost::lexical_cast<std::string>(m_OuterAngle).c_str());
	pSceneNode->LinkEndChild(pShapeNode);

	pBaseElement->LinkEndChild(pSceneNode);
}
