#include "RenderComponent.h"

const char* GridRenderComponent::m_Name = "GridRenderComponent";

BaseRenderComponent::BaseRenderComponent() : m_pSceneNode(nullptr)
{
}

BaseRenderComponent::~BaseRenderComponent()
{
}

bool BaseRenderComponent::VInit(TiXmlElement* pData)
{
	return VDelegateInit(pData);
}

void BaseRenderComponent::VPostInit()
{

}

void BaseRenderComponent::VOnChanged()
{

}

TiXmlElement* BaseRenderComponent::VGenerateXml()
{
	TiXmlElement* pBaseElement = VCreateBaseElement();
	VCreateInheritedXmlElement(pBaseElement);
	return pBaseElement;
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
	return shared_ptr<SceneNode>();
}

void GridRenderComponent::VCreateInheritedXmlElement(TiXmlElement* pBaseElement)
{

}
