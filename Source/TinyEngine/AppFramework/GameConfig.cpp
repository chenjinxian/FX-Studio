#include "GameConfig.h"

GameConfig::GameConfig()
	: m_Project(),
	m_Renderer("Direct3D 11"),
	m_ScreenWidth(1024),
	m_ScreenHeight(720),
	m_IsFullScreen(false),
	m_IsVSync(false),
	m_AntiAliasingSample(0),
	m_IsZipResource(false),
	m_pDocument(nullptr)
{
}


GameConfig::~GameConfig()
{
}

void GameConfig::InitConfig(const std::string& xmlFileName, LPWSTR lpCmdLine)
{
	m_pDocument = unique_ptr<tinyxml2::XMLDocument>(DEBUG_NEW tinyxml2::XMLDocument());
	if (m_pDocument != nullptr && (m_pDocument->LoadFile(xmlFileName.c_str()) == tinyxml2::XML_SUCCESS))
	{
		tinyxml2::XMLElement *pRoot = m_pDocument->RootElement();
		if (pRoot == nullptr)
			return;

		tinyxml2::XMLElement* pNode = pRoot->FirstChildElement("Graphics");
		if (pNode != nullptr)
		{
			std::string attribute = pNode->Attribute("renderer");
			if (attribute != "Direct3D 11" && attribute != "Vulkan")
			{
				DEBUG_ASSERT(0 && "Bad Renderer setting in Graphics configs.");
			}
			else
			{
				m_Renderer = attribute;
			}

			if (pNode->Attribute("width") != nullptr)
			{
				m_ScreenWidth = pNode->IntAttribute("width");
			}

			if (pNode->Attribute("height") != nullptr)
			{
				m_ScreenHeight = pNode->IntAttribute("height");
			}

			if (pNode->Attribute("fullscreen") != nullptr)
			{
				m_IsFullScreen = pNode->BoolAttribute("fullscreen");
			}

			if (pNode->Attribute("vsync") != nullptr)
			{
				m_IsVSync = pNode->BoolAttribute("vsync");
			}

			if (pNode->Attribute("anti-aliasing") != nullptr)
			{
				m_AntiAliasingSample = pNode->IntAttribute("anti-aliasing");
			}
		}

		pNode = pRoot->FirstChildElement("ResCache");
		if (pNode)
		{
			m_IsZipResource = pNode->BoolAttribute("useZipResource");
		}
	}
}
