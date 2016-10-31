#include "RenderComponent.h"
#include "Actor.h"
#include "TransformComponent.h"
#include "../Graphics3D/Skybox.h"
#include "../Graphics3D/D3DSdkMesh.h"
#include "../EventManager/Events.h"
#include "../AppFramework/RenderEngineApp.h"

const char* GridRenderComponent::m_Name = "GridRenderComponent";
const char* ModelRenderComponent::m_Name = "ModelRenderComponent";
const char* SkyRenderComponent::m_Name = "SkyRenderComponent";
const char* LightRenderComponent::m_Name = "LightRenderComponent";

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
	shared_ptr<EvtData_New_Render_Component> pEvent(GCC_NEW EvtData_New_Render_Component(m_pOwner->GetActorId(), pSceneNode));
	IEventManager::Get()->VTriggerEvent(pEvent);
}

void BaseRenderComponent::VOnChanged()
{
	shared_ptr<EvtData_Modified_Render_Component> pEvent(GCC_NEW EvtData_Modified_Render_Component(m_pOwner->GetActorId()));
	IEventManager::Get()->VTriggerEvent(pEvent);
}

TiXmlElement* BaseRenderComponent::VGenerateXml()
{
	TiXmlElement* pBaseElement = VCreateBaseElement();

	TiXmlElement* pColor = GCC_NEW TiXmlElement("Color");
	pColor->SetAttribute("r", ToStr(m_Color.x).c_str());
	pColor->SetAttribute("g", ToStr(m_Color.y).c_str());
	pColor->SetAttribute("b", ToStr(m_Color.z).c_str());
	pColor->SetAttribute("a", ToStr(m_Color.w).c_str());
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
	shared_ptr<TransformComponent> pTransformComponent =
		MakeStrongPtr(m_pOwner->GetComponent<TransformComponent>(TransformComponent::m_Name));
	if (pTransformComponent)
	{
		WeakBaseRenderComponentPtr weakThis(this);

		switch (RenderEngineApp::GetRendererImpl())
		{
		case RenderEngineApp::Renderer_D3D11:
			return shared_ptr<SceneNode>(GCC_NEW GridNode(m_pOwner->GetActorId(), weakThis));

		default:
			GCC_ERROR("Unknown Renderer Implementation in GridRenderComponent");
		}
	}

	return shared_ptr<SceneNode>();
}

void GridRenderComponent::VCreateInheritedXmlElement(TiXmlElement* pBaseElement)
{

}

ModelRenderComponent::ModelRenderComponent()
	: m_SdkMeshName(),
	m_TextureName()
{

}

ModelRenderComponent::~ModelRenderComponent()
{

}

bool ModelRenderComponent::VDelegateInit(TiXmlElement* pData)
{
	TiXmlElement* pSdkMesh = pData->FirstChildElement("SdkMesh");
	if (pSdkMesh != nullptr)
	{
		m_SdkMeshName = pSdkMesh->FirstChild()->Value();
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
	shared_ptr<TransformComponent> pTransformComponent =
		MakeStrongPtr(m_pOwner->GetComponent<TransformComponent>(TransformComponent::m_Name));
	if (pTransformComponent)
	{
		WeakBaseRenderComponentPtr weakThis(this);

		switch (RenderEngineApp::GetRendererImpl())
		{
		case RenderEngineApp::Renderer_D3D11:
		{
			return shared_ptr<SceneNode>(GCC_NEW D3DShaderMeshNode11(
				m_pOwner->GetActorId(), weakThis, RenderPass_Actor, pTransformComponent->GetTransform()));
		}

		default:
			GCC_ERROR("Unknown Renderer Implementation in TeapotRenderComponent");
		}
	}

	return shared_ptr<SceneNode>();
}

void ModelRenderComponent::VCreateInheritedXmlElement(TiXmlElement* pBaseElement)
{
	TiXmlElement* pSdkMesh = GCC_NEW TiXmlElement("SdkMesh");
	TiXmlText* pSdkMeshName = GCC_NEW TiXmlText(m_SdkMeshName.c_str());
	pSdkMesh->LinkEndChild(pSdkMeshName);
	pBaseElement->LinkEndChild(pSdkMesh);

	TiXmlElement* pTexture = GCC_NEW TiXmlElement("Texture");
	TiXmlText* pTextureName = GCC_NEW TiXmlText(m_TextureName.c_str());
	pTexture->LinkEndChild(pTextureName);
	pBaseElement->LinkEndChild(pTexture);

	TiXmlElement* pEffect = GCC_NEW TiXmlElement("Effect");
	TiXmlText* pEffectName = GCC_NEW TiXmlText(m_EffectName.c_str());
	pEffect->LinkEndChild(pEffectName);
	pBaseElement->LinkEndChild(pEffect);
}

SkyRenderComponent::SkyRenderComponent()
{

}

SkyRenderComponent::~SkyRenderComponent()
{

}

bool SkyRenderComponent::VDelegateInit(TiXmlElement* pData)
{
	TiXmlElement* pTexture = pData->FirstChildElement("Texture");
	if (pTexture)
	{
// 		m_textureResource = pTexture->FirstChild()->Value();
	}
	return true;
}

shared_ptr<SceneNode> SkyRenderComponent::VCreateSceneNode()
{
	shared_ptr<SkyboxNode> sky;
	if (RenderEngineApp::GetRendererImpl() == RenderEngineApp::Renderer_D3D11)
	{
		sky = shared_ptr<SkyboxNode>(GCC_NEW SkyboxNode(L"Assets\\Textures\\Maskonaive2_1024.dds"));
	}
	else
	{
		GCC_ERROR("Unknown Renderer Implementation in VLoadGameDelegate");
	}
	return sky;
}

void SkyRenderComponent::VCreateInheritedXmlElement(TiXmlElement* pBaseElement)
{
// 	TiXmlElement* pTextureNode = GCC_NEW TiXmlElement("Texture");
// 	TiXmlText* pTextureText = GCC_NEW TiXmlText(m_textureResource.c_str());
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
	return true;
}

shared_ptr<SceneNode> LightRenderComponent::VCreateSceneNode()
{
	return shared_ptr<SceneNode>();
}

void LightRenderComponent::VCreateInheritedXmlElement(TiXmlElement* pBaseElement)
{

}
