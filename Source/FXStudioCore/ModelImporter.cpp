#include "stdafx.h"
#include "ModelImporter.h"
#include "../TinyEngine/TinyEngine.h"
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
		return m_Callback(percentage * 0.6f, nullptr);
	}

	return true;
}

void ModelImporter::LoadModer(const std::string& importPath, const std::string& exportPath, ProgressCallback callback)
{
	m_Callback = callback;

// 	std::thread workThread([=]() {
// 	});
// 
// 	workThread.join();

	uint32_t flags = aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_FlipUVs | aiProcess_FlipWindingOrder;
	const aiScene* scene = m_AssimpImporter->ReadFile(importPath, flags);

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

	if (scene->HasAnimations())
	{
		WriteAnimation(scene, &xmlDoc, pRoot);
	}

	m_Callback(0.6f + 0.1f, nullptr);

	if (scene->HasMeshes())
	{
		WriteMesh(scene, &xmlDoc, pRoot);
	}

	m_AssimpImporter->FreeScene();

	xmlDoc.SaveFile(exportPath.c_str());
}

void ModelImporter::WriteNode(const aiNode* pSceneNode, tinyxml2::XMLDocument* pXmlDoc, tinyxml2::XMLElement* pXmlNode, uint32_t depth)
{
	tinyxml2::XMLElement* pRoot = pXmlDoc->NewElement("Node");
	pXmlNode->InsertEndChild(pRoot);
	tinyxml2::XMLElement* pMatrix = pXmlDoc->NewElement("Matrix");
	pRoot->InsertEndChild(pMatrix);

	const aiMatrix4x4& mat = pSceneNode->mTransformation;
	char buffer[1024];
	sprintf_s(buffer, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f",
		mat.a1, mat.a2, mat.a3, mat.a4, mat.b1, mat.b2, mat.b3, mat.b4, mat.c1, mat.c2, mat.c3, mat.c4, mat.d1, mat.d2, mat.d3, mat.d4);
	pMatrix->SetText(buffer);

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

void ModelImporter::WriteAnimation(const aiScene* pScene, tinyxml2::XMLDocument* pXmlDoc, tinyxml2::XMLElement* pXmlNode)
{
	tinyxml2::XMLElement* pAnimationList = pXmlDoc->NewElement("AnimationList");
	pXmlNode->InsertEndChild(pAnimationList);

	for (uint32_t i = 0; i < pScene->mNumAnimations; i++)
	{
		aiAnimation* animation = pScene->mAnimations[i];
		tinyxml2::XMLElement* pChildAnimation = pXmlDoc->NewElement("Animation");
		pChildAnimation->SetAttribute("name", animation->mName.C_Str());
		pChildAnimation->SetAttribute("duration", animation->mDuration);
		pChildAnimation->SetAttribute("ticks", animation->mTicksPerSecond);
		pAnimationList->InsertEndChild(pChildAnimation);

		if (animation->mNumChannels > 0)
		{
			tinyxml2::XMLElement* pNodeList = pXmlDoc->NewElement("NodeList");
			pChildAnimation->InsertEndChild(pNodeList);

			for (uint32_t j = 0; j < animation->mNumChannels; j++)
			{
				aiNodeAnim* node = animation->mChannels[j];
				tinyxml2::XMLElement* pNodeAnim = pXmlDoc->NewElement("Node");
				pNodeAnim->SetAttribute("name", node->mNodeName.C_Str());
				pNodeList->InsertEndChild(pNodeAnim);

				if (node->mNumPositionKeys > 0)
				{
					tinyxml2::XMLElement* pPositionKeys = pXmlDoc->NewElement("PositionKeyList");
					pNodeAnim->InsertEndChild(pPositionKeys);

					for (uint32_t k = 0; k < node->mNumPositionKeys; k++)
					{
						aiVectorKey* vectorKey = node->mPositionKeys + k;
						tinyxml2::XMLElement* pKey = pXmlDoc->NewElement("PositionKey");
						pKey->SetAttribute("time", vectorKey->mTime);
						char buffer[256];
						sprintf_s(buffer, "%f %f %f", vectorKey->mValue.x, vectorKey->mValue.y, vectorKey->mValue.z);
						pKey->SetText(buffer);
					}
				}

				if (node->mNumScalingKeys > 0)
				{
					tinyxml2::XMLElement* pScalingKeys = pXmlDoc->NewElement("ScalingKeyList");
					pNodeAnim->InsertEndChild(pScalingKeys);

					for (uint32_t k = 0; k < node->mNumScalingKeys; k++)
					{
						aiVectorKey* vectorKey = node->mScalingKeys + k;
						tinyxml2::XMLElement* pKey = pXmlDoc->NewElement("ScalingKey");
						pKey->SetAttribute("time", vectorKey->mTime);
						char buffer[256];
						sprintf_s(buffer, "%f %f %f", vectorKey->mValue.x, vectorKey->mValue.y, vectorKey->mValue.z);
						pKey->SetText(buffer);
					}
				}

				if (node->mNumRotationKeys > 0)
				{
					tinyxml2::XMLElement* pRotationKeys = pXmlDoc->NewElement("RotationKeyList");
					pNodeAnim->InsertEndChild(pRotationKeys);

					for (uint32_t k = 0; k < node->mNumRotationKeys; k++)
					{
						aiQuatKey* quatKey = node->mRotationKeys + k;
						tinyxml2::XMLElement* pKey = pXmlDoc->NewElement("RotationKey");
						pKey->SetAttribute("time", quatKey->mTime);
						char buffer[256];
						sprintf_s(buffer, "%f %f %f %f", quatKey->mValue.x, quatKey->mValue.y, quatKey->mValue.z, quatKey->mValue.w);
						pKey->SetText(buffer);
					}
				}
			}
		}
	}
}

void ModelImporter::WriteMesh(const aiScene* pScene, tinyxml2::XMLDocument* pXmlDoc, tinyxml2::XMLElement* pXmlNode)
{
	tinyxml2::XMLElement* pMeshList = pXmlDoc->NewElement("MeshList");
	pXmlNode->InsertEndChild(pMeshList);

	for (uint32_t i = 0; i < pScene->mNumMeshes; i++)
	{
		aiMesh* mesh = pScene->mMeshes[i];

		std::string types;
		if (mesh->mPrimitiveTypes & aiPrimitiveType_POINT)
			types.append("point ");
		if (mesh->mPrimitiveTypes & aiPrimitiveType_LINE)
			types.append("line ");
		if (mesh->mPrimitiveTypes & aiPrimitiveType_TRIANGLE)
			types.append("triangle ");
		if (mesh->mPrimitiveTypes & aiPrimitiveType_POLYGON)
			types.append("polygon");

		tinyxml2::XMLElement* pMesh = pXmlDoc->NewElement("Mesh");
		pMesh->SetAttribute("types", types.c_str());
		pMeshList->InsertEndChild(pMesh);

		if (mesh->HasBones())
		{
			tinyxml2::XMLElement* pBoneList = pXmlDoc->NewElement("BoneList");
			pMesh->InsertEndChild(pBoneList);

			for (uint32_t j = 0; j < mesh->mNumBones; j++)
			{
				aiBone* bone = mesh->mBones[j];

				tinyxml2::XMLElement* pChildBone = pXmlDoc->NewElement("Bone");
				pChildBone->SetAttribute("name", bone->mName.C_Str());

				const aiMatrix4x4& mat = bone->mOffsetMatrix;
				char buffer[1024];
				sprintf_s(buffer, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f",
					mat.a1, mat.a2, mat.a3, mat.a4, mat.b1, mat.b2, mat.b3, mat.b4, mat.c1, mat.c2, mat.c3, mat.c4, mat.d1, mat.d2, mat.d3, mat.d4);
				tinyxml2::XMLElement* pMatrix = pXmlDoc->NewElement("Matrix");
				pMatrix->SetText(buffer);
				pChildBone->InsertEndChild(pMatrix);

				if (bone->mNumWeights > 0)
				{
					tinyxml2::XMLElement* pWeightList = pXmlDoc->NewElement("WeightList");
					pChildBone->InsertEndChild(pWeightList);

					for (uint32_t k = 0; k < bone->mNumWeights; k++)
					{
						aiVertexWeight* vertexWeight = bone->mWeights + k;
						tinyxml2::XMLElement* pChildWeight = pXmlDoc->NewElement("Weight");
						pChildWeight->SetAttribute("index", vertexWeight->mVertexId);
						pChildWeight->SetText(vertexWeight->mWeight);
						pWeightList->InsertEndChild(pChildWeight);
					}
				}
			}
		}

		if (mesh->HasFaces())
		{
			tinyxml2::XMLElement* pFaceList = pXmlDoc->NewElement("FaceList");
			pMesh->InsertEndChild(pFaceList);

			for (uint32_t j = 0; j < mesh->mNumFaces; j++)
			{
				const aiFace& face = mesh->mFaces[j];
				std::string indices;
				for (uint32_t k = 0; k < face.mNumIndices; k++)
				{
					char buffer[16];
					sprintf_s(buffer, "%d ", face.mIndices[k]);
					indices.append(buffer);
				}

				tinyxml2::XMLElement* pChildFace = pXmlDoc->NewElement("Face");
				pChildFace->SetText(indices.c_str());
				pFaceList->InsertEndChild(pChildFace);
			}
		}

		if (mesh->HasPositions())
		{
			tinyxml2::XMLElement* pVertices = pXmlDoc->NewElement("Vertices");
			pMesh->InsertEndChild(pVertices);

			std::string vertices;
			for (uint32_t j = 0; j < mesh->mNumVertices; j++)
			{
				char buffer[256];
				sprintf_s(buffer, "%f %f %f ", mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z);
				vertices.append(buffer);
			}

			pVertices->SetText(vertices.c_str());
		}

		if (mesh->HasNormals())
		{
			tinyxml2::XMLElement* pNormals = pXmlDoc->NewElement("Normals");
			pMesh->InsertEndChild(pNormals);

			std::string normals;
			for (uint32_t j = 0; j < mesh->mNumVertices; j++)
			{
				char buffer[256];
				sprintf_s(buffer, "%f %f %f ", mesh->mNormals[j].x, mesh->mNormals[j].y, mesh->mNormals[j].z);
				normals.append(buffer);
			}

			pNormals->SetText(normals.c_str());
		}

		if (mesh->HasTangentsAndBitangents())
		{
			tinyxml2::XMLElement* pTangents = pXmlDoc->NewElement("Tangents");
			pMesh->InsertEndChild(pTangents);

			std::string tangents;
			for (uint32_t j = 0; j < mesh->mNumVertices; j++)
			{
				char buffer[256];
				sprintf_s(buffer, "%f %f %f ", mesh->mTangents[j].x, mesh->mTangents[j].y, mesh->mTangents[j].z);
				tangents.append(buffer);
			}

			pTangents->SetText(tangents.c_str());

			tinyxml2::XMLElement* pBinormals = pXmlDoc->NewElement("Binormals");
			pMesh->InsertEndChild(pBinormals);

			std::string binoramls;
			for (uint32_t j = 0; j < mesh->mNumVertices; j++)
			{
				char buffer[256];
				sprintf_s(buffer, "%f %f %f ", mesh->mBitangents[j].x, mesh->mBitangents[j].y, mesh->mBitangents[j].z);
				binoramls.append(buffer);
			}

			pBinormals->SetText(binoramls.c_str());
		}

		for (uint32_t j = 0; j < AI_MAX_NUMBER_OF_TEXTURECOORDS; j++)
		{
			if (mesh->HasTextureCoords(j))
			{
				tinyxml2::XMLElement* pTextureCoords = pXmlDoc->NewElement("TextureCoords");
				pMesh->InsertEndChild(pTextureCoords);

				std::string textureCoords;
				if (mesh->mNumUVComponents[j] == 3)
				{
					for (uint32_t k = 0; k < mesh->mNumVertices; k++)
					{
						char buffer[256];
						sprintf_s(buffer, "%f %f %f ", mesh->mTextureCoords[j][k].x, mesh->mTextureCoords[j][k].y, mesh->mTextureCoords[j][k].z);
						textureCoords.append(buffer);
					}
				}
				else
				{
					for (uint32_t k = 0; k < mesh->mNumVertices; k++)
					{
						char buffer[256];
						sprintf_s(buffer, "%f %f ", mesh->mTextureCoords[j][k].x, mesh->mTextureCoords[j][k].y);
						textureCoords.append(buffer);
					}
				}

				pTextureCoords->SetText(textureCoords.c_str());

				// need only one texture coordinates
				break;
			}
		}

		for (uint32_t j = 0; j < AI_MAX_NUMBER_OF_COLOR_SETS; j++)
		{
			if (mesh->HasVertexColors(j))
			{
				tinyxml2::XMLElement* pColors = pXmlDoc->NewElement("Colors");
				pMesh->InsertEndChild(pColors);

				std::string colors;
				for (uint32_t k = 0; k < mesh->mNumVertices; k++)
				{
					char buffer[256];
					sprintf_s(buffer, "%f %f %f %f ", mesh->mColors[j][k].r, mesh->mColors[j][k].g, mesh->mColors[j][k].b, mesh->mColors[j][k].a);
					colors.append(buffer);
				}

				pColors->SetText(colors.c_str());

				// need only one vertex colors
				break;
			}
		}

		m_Callback(0.7 + 0.3 * (i + 1) / pScene->mNumMeshes, nullptr);
	}
}
