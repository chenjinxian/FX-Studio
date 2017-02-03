#include "stdafx.h"
#include "ModelImporter.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#pragma comment(lib, "assimp-vc140-mt.lib")

ModelImporter* ModelImporter::g_Importer = new ModelImporter();

ModelImporter* ModelImporter::GetImporter()
{
	return g_Importer;
}

void ModelImporter::DeleteImporter()
{
	if (g_Importer != nullptr)
	{
		delete g_Importer;
		g_Importer = nullptr;
	}
}

ModelImporter::ModelImporter()
	: m_Callback(nullptr)
{
	m_AssimpImporter = new Assimp::Importer();
	m_AssimpImporter->SetProgressHandler(this);
}

ModelImporter::~ModelImporter()
{
// 	if (m_AssimpImporter != nullptr)
// 	{
// 		delete m_AssimpImporter;
// 		m_AssimpImporter = nullptr;
// 	}
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
	const aiScene* scene = m_AssimpImporter->ReadFile(filePath, flags);
	
	if (scene == nullptr)
	{
		if (callback != nullptr)
		{
			callback(-1.f, m_AssimpImporter->GetErrorString());
		}
		return;
	}
	m_AssimpImporter->FreeScene();
}
