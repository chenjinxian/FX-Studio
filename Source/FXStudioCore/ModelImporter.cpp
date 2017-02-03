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
	m_AssimpImporter(new Assimp::Importer())
{
	m_AssimpImporter->SetProgressHandler(this);
}

ModelImporter::~ModelImporter()
{
	m_AssimpImporter->SetProgressHandler(nullptr);
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

// 	std::thread workThread([=]() {
// 	});
// 
// 	workThread.join();

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

	tinyxml2::XMLDocument xmlDoc;

	tinyxml2::XMLElement* pRoot = xmlDoc.NewElement("Model");
	xmlDoc.InsertEndChild(pRoot);
	WriteNode(scene->mRootNode, &xmlDoc, pRoot, 0);

	tinyxml2::XMLPrinter printer;
	xmlDoc.Accept(&printer);

	m_AssimpImporter->FreeScene();
}

void ModelImporter::WriteNode(const aiNode* pSceneNode, tinyxml2::XMLDocument* pXmlDoc, tinyxml2::XMLElement* pXmlNode, uint32_t depth)
{
	tinyxml2::XMLElement* pRoot = pXmlDoc->NewElement("Node");
	pXmlNode->InsertEndChild(pRoot);
	tinyxml2::XMLElement* pMatrix = pXmlDoc->NewElement("Matrix");
	pRoot->InsertEndChild(pMatrix);

	const aiMatrix4x4& mat = pSceneNode->mTransformation;
	char buffer[4096];
	sprintf_s(buffer, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f",
		mat.a1, mat.a2, mat.a3, mat.a4, mat.b1, mat.b2, mat.b3, mat.b4, mat.c1, mat.c2, mat.c3, mat.c4, mat.d1, mat.d2, mat.d3, mat.d4);

	if (pSceneNode->mNumMeshes)
	{

	}
	
	if (pSceneNode->mNumChildren)
	{
		tinyxml2::XMLElement* pList = pXmlDoc->NewElement("NodeList");
		pRoot->InsertEndChild(pList);

		for (uint32_t i = 0; i < pSceneNode->mNumChildren; i++)
		{
			WriteNode(pSceneNode->mChildren[i], pXmlDoc, pList, depth + 2);
		}
	}
}
