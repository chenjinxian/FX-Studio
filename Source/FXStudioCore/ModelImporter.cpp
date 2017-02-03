#include "stdafx.h"
#include "ModelImporter.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include <thread>

#pragma comment(lib, "assimp-vc140-mt.lib")

ModelImporter& ModelImporter::GetImporter()
{
	static ModelImporter g_Importer;
	return g_Importer;
}

ModelImporter::ModelImporter()
	: m_Callback(nullptr),
	m_AssimpImporter()
{
	m_AssimpImporter.SetProgressHandler(this);
}

ModelImporter::~ModelImporter()
{
	m_AssimpImporter.SetProgressHandler(nullptr);
}

bool ModelImporter::Update(float percentage /*= -1.f*/)
{
	if (m_Callback != nullptr)
	{
		return m_Callback(percentage, nullptr);
	}

	return true;
}

void ModelImporter::LoadModer(const std::string& filePath, ProgressCallback callback)
{
	m_Callback = callback;

	uint32_t flags = aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_FlipUVs | aiProcess_FlipWindingOrder;
	const aiScene* scene = m_AssimpImporter.ReadFile(filePath, flags);

	if (scene == nullptr)
	{
		if (callback != nullptr)
		{
			callback(-1.f, m_AssimpImporter.GetErrorString());
		}
		return;
	}

	m_AssimpImporter.FreeScene();
}
