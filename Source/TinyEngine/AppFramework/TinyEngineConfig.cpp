#include "TinyEngineConfig.h"

std::string m_Project;

std::string m_Renderer;
uint32_t m_ScreenWidth;
uint32_t m_ScreenHeight;
bool m_IsFullScreen;
bool m_IsVSync;
uint32_t m_AntiAliasing;

bool m_IsDevelopmentDirectories;

unique_ptr<TiXmlDocument> m_pDocument;


TinyEngineConfig::TinyEngineConfig()
	: m_Project(),
	m_Renderer("Direct3D 11"),
	m_ScreenWidth(1024),
	m_ScreenHeight(720),
	m_IsFullScreen(false),
	m_IsVSync(false),
	m_AntiAliasing(0),
	m_IsZipResource(false),
	m_pDocument(nullptr)
{
}


TinyEngineConfig::~TinyEngineConfig()
{
}

void TinyEngineConfig::InitConfig(const std::string& xmlFileName, LPWSTR lpCmdLine)
{
	m_pDocument = std::make_unique<TiXmlDocument>(new TiXmlDocument(xmlFileName.c_str()));
	if (m_pDocument != nullptr && m_pDocument->LoadFile())
	{
		TiXmlElement *pRoot = m_pDocument->RootElement();
		if (pRoot == nullptr)
			return;

		TiXmlElement* pNode = pRoot->FirstChildElement("Graphics");
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
				m_ScreenWidth = atoi(pNode->Attribute("width"));
			}

			if (pNode->Attribute("height") != nullptr)
			{
				m_ScreenHeight = atoi(pNode->Attribute("height"));
			}

			if (pNode->Attribute("fullscreen") != nullptr)
			{
				attribute = pNode->Attribute("fullscreen");
				m_IsFullScreen = (attribute == "yes") ? true : false;
			}

			if (pNode->Attribute("vsync") != nullptr)
			{
				attribute = pNode->Attribute("vsync");
				m_IsVSync = (attribute == "yes") ? true : false;
			}

			if (pNode->Attribute("anti-aliasing") != nullptr)
			{
				m_AntiAliasing = atoi(pNode->Attribute("anti-aliasing"));
			}
		}

		pNode = pRoot->FirstChildElement("ResCache");
		if (pNode)
		{
			std::string attribute(pNode->Attribute("useZipResource"));
			m_IsZipResource = ((attribute == "yes") ? (true) : (false));
		}
	}
}
