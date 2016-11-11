#include "TinyEngineConfig.h"



TinyEngineConfig::TinyEngineConfig()
	: m_Level(), m_Renderer("Direct3D 11"), m_IsRunFullSpeed(false), m_ScreenWidth(1024), m_ScreenHeight(720),
	m_SoundEffectsVolume(1.0f), m_MusicVolume(1.0f), m_ExpectedPlayers(1), m_ListenPort(-1), m_GameHost(),
	m_NumAIs(1), m_MaxAIs(4), m_MaxPlayers(4), m_IsDevelopmentDirectories(false), m_pDoc(nullptr)
{
}


TinyEngineConfig::~TinyEngineConfig()
{
}

void TinyEngineConfig::Init(const std::string& xmlFileName, LPWSTR lpCmdLine)
{
	m_pDoc = new TiXmlDocument(xmlFileName.c_str());
	if (m_pDoc && m_pDoc->LoadFile())
	{
		TiXmlElement *pRoot = m_pDoc->RootElement();
		if (!pRoot)
			return;

		TiXmlElement* pNode = NULL;
		pNode = pRoot->FirstChildElement("Graphics");
		if (pNode)
		{
			std::string attribute;
			attribute = pNode->Attribute("renderer");
			if (attribute != "Direct3D 9" && attribute != "Direct3D 11")
			{
				DEBUG_ASSERT(0 && "Bad Renderer setting in Graphics options.");
			}
			else
			{
				m_Renderer = attribute;
			}

			if (pNode->Attribute("width"))
			{
				m_ScreenWidth = atoi(pNode->Attribute("width"));
				if (m_ScreenWidth < 800) m_ScreenWidth = 800;
			}

			if (pNode->Attribute("height"))
			{
				m_ScreenHeight = atoi(pNode->Attribute("height"));
				if (m_ScreenHeight < 600) m_ScreenHeight = 600;
			}

			if (pNode->Attribute("runfullspeed"))
			{
				attribute = pNode->Attribute("runfullspeed");
				m_IsRunFullSpeed = (attribute == "yes") ? true : false;
			}
		}

		pNode = pRoot->FirstChildElement("Sound");
		if (pNode)
		{
			m_MusicVolume = atoi(pNode->Attribute("musicVolume")) / 100.0f;
			m_SoundEffectsVolume = atoi(pNode->Attribute("sfxVolume")) / 100.0f;
		}

		pNode = pRoot->FirstChildElement("Multiplayer");
		if (pNode)
		{
			m_ExpectedPlayers = atoi(pNode->Attribute("expectedPlayers"));
			m_NumAIs = atoi(pNode->Attribute("numAIs"));
			m_MaxAIs = atoi(pNode->Attribute("maxAIs"));
			m_MaxPlayers = atoi(pNode->Attribute("maxPlayers"));

			m_ListenPort = atoi(pNode->Attribute("listenPort"));
			m_GameHost = pNode->Attribute("gameHost");
		}

		pNode = pRoot->FirstChildElement("ResCache");
		if (pNode)
		{
			std::string attribute(pNode->Attribute("useDevelopmentDirectories"));
			m_IsDevelopmentDirectories = ((attribute == "yes") ? (true) : (false));
		}
	}
}
