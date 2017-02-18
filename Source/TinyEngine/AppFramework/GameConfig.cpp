#include "GameConfig.h"

GameConfig::GameConfig()
	: m_Project(),
	m_Renderer("Direct3D 11"),
	m_IsFullScreen(false),
	m_IsVSync(true),
	m_AntiAliasingSample(0),
	m_IsZipResource(false),
	m_pDocument(nullptr)
{
	for (int i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
	{
		m_ScreenWidth[i] = 1024;
		m_ScreenHeight[i] = 720;
	}
}


GameConfig::~GameConfig()
{
}

void GameConfig::InitConfig(const std::string& xmlFileName, LPWSTR lpCmdLine)
{
	m_pDocument = std::unique_ptr<tinyxml2::XMLDocument>(DEBUG_NEW tinyxml2::XMLDocument());
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
				m_ScreenWidth[0] = pNode->IntAttribute("width");
			}

			if (pNode->Attribute("height") != nullptr)
			{
				m_ScreenHeight[0] = pNode->IntAttribute("height");
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
				if (m_AntiAliasingSample < 1)
				{
					m_AntiAliasingSample = 1;
				}
			}
		}

		pNode = pRoot->FirstChildElement("ResCache");
		if (pNode)
		{
			m_IsZipResource = pNode->BoolAttribute("useZipResource");
		}
	}
}
