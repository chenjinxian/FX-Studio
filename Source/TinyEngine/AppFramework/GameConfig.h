#pragma once
#include "../TinyEngineBase.h"
#include "../TinyEngineInterface.h"

class GameConfig
{
public:
	GameConfig();
	~GameConfig();

	void InitConfig(const std::string& xmlFilePath, LPWSTR lpCmdLine);

	std::string m_Project;

	std::string m_Renderer;
	uint32_t m_ScreenWidth[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
	uint32_t m_ScreenHeight[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
	bool m_IsFullScreen;
	bool m_IsVSync;
	uint32_t m_AntiAliasingSample;

	bool m_IsZipResource;

	unique_ptr<tinyxml2::XMLDocument> m_pDocument;
};

