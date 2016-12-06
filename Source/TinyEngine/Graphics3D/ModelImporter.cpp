#include "ModelImporter.h"
#include "ModelMaterial.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#pragma comment(lib, "assimp-vc140-mt.lib")

Model::Model(const std::string& filename, bool flipUVs /*= false*/)
	: m_Meshes(), m_Materials()
{
	Assimp::Importer importer;

	uint32_t flags = aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType | aiProcess_FlipWindingOrder;
	if (flipUVs)
	{
		flags |= aiProcess_FlipUVs;
	}

	const aiScene* scene = importer.ReadFile(filename, flags);
	if (scene == nullptr)
	{
		DEBUG_ERROR(importer.GetErrorString());
	}

	if (scene->HasMaterials())
	{
		for (uint32_t i = 0; i < scene->mNumMaterials; i++)
		{
			m_Materials.push_back(DEBUG_NEW ModelMaterial(*this, scene->mMaterials[i]));
		}
	}

	if (scene->HasMeshes())
	{
		for (uint32_t i = 0; i < scene->mNumMeshes; i++)
		{
			Mesh* mesh = DEBUG_NEW Mesh(this, (scene->mMeshes[i]));
			m_Meshes.push_back(mesh);
		}
	}
}

Model::Model(const void* pBuffer, uint32_t length, bool flipUVs /*= false*/)
	: m_Meshes(), m_Materials()
{
	Assimp::Importer importer;

	uint32_t flags = aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType | aiProcess_FlipWindingOrder;
	if (flipUVs)
	{
		flags |= aiProcess_FlipUVs;
	}

	const aiScene* scene = importer.ReadFileFromMemory(pBuffer, length, flags);
	if (scene == nullptr)
	{
		DEBUG_ERROR(importer.GetErrorString());
	}

	if (scene->HasMaterials())
	{
		m_Materials.reserve(scene->mNumMaterials);
		for (uint32_t i = 0; i < scene->mNumMaterials; i++)
		{
			m_Materials.push_back(DEBUG_NEW ModelMaterial(*this, scene->mMaterials[i]));
		}
	}

	if (scene->HasMeshes())
	{
		m_Meshes.reserve(scene->mNumMeshes);
		for (uint32_t i = 0; i < scene->mNumMeshes; i++)
		{
			Mesh* mesh = DEBUG_NEW Mesh(this, (scene->mMeshes[i]));
			m_Meshes.push_back(mesh);
		}
	}
}

Model::~Model()
{
	for (auto mesh : m_Meshes)
	{
		SAFE_DELETE(mesh);
	}
	m_Meshes.clear();

	for (auto material : m_Materials)
	{
		SAFE_DELETE(material);
	}
	m_Materials.clear();
}

bool Model::HasMeshes() const
{
	return m_Meshes.size() > 0;
}

bool Model::HasMaterials() const
{
	return m_Materials.size() > 0;
}

const std::vector<Mesh*>& Model::GetMeshes() const
{
	return m_Meshes;
}

const std::vector<ModelMaterial*>& Model::GetMaterials() const
{
	return m_Materials;
}

Mesh::Mesh(Model* pModel, aiMesh* mesh)
	: m_pModel(pModel),
	m_pModelMaterial(nullptr),
	m_MeshName(),
	m_Vertices(),
	m_Normals(),
	m_Tangents(),
	m_BiNormals(),
	m_TextureCoordinates(),
	m_VertexColors(),
	m_FaceCount(0),
	m_Indices()
{
	DEBUG_ASSERT(mesh != nullptr);
	m_pModelMaterial = m_pModel->GetMaterials().at(mesh->mMaterialIndex);

	m_Vertices.reserve(mesh->mNumVertices);
	for (uint32_t i = 0; i < mesh->mNumVertices; i++)
	{
		m_Vertices.push_back(Vector3(reinterpret_cast<const float*>(&mesh->mVertices[i])));
	}

	if (mesh->HasNormals())
	{
		m_Normals.reserve(mesh->mNumVertices);
		for (uint32_t i = 0; i < mesh->mNumVertices; i++)
		{
			m_Normals.push_back(Vector3(reinterpret_cast<const float*>(&mesh->mNormals[i])));
		}
	}

	if (mesh->HasTangentsAndBitangents())
	{
		m_Tangents.reserve(mesh->mNumVertices);
		m_BiNormals.reserve(mesh->mNumVertices);
		for (uint32_t i = 0; i < mesh->mNumVertices; i++)
		{
			m_Tangents.push_back(Vector3(reinterpret_cast<const float*>(&mesh->mTangents[i])));
			m_BiNormals.push_back(Vector3(reinterpret_cast<const float*>(&mesh->mBitangents[i])));
		}
	}

	uint32_t uvChannelCount = mesh->GetNumUVChannels();
	for (uint32_t i = 0; i < uvChannelCount; i++)
	{
		std::vector<Vector3> textureCoordinates(mesh->mNumVertices);

		aiVector3D* aiTextureCoordinates = mesh->mTextureCoords[i];
		for (uint32_t j = 0; j < mesh->mNumVertices; j++)
		{
			textureCoordinates[j] = Vector3(reinterpret_cast<const float*>(&aiTextureCoordinates[j]));
		}

		m_TextureCoordinates.push_back(textureCoordinates);
	}

	uint32_t colorChannelCount = mesh->GetNumColorChannels();
	for (uint32_t i = 0; i < colorChannelCount; i++)
	{
		std::vector<Vector4> vertexColors(mesh->mNumVertices);

		aiColor4D* aiVertexColors = mesh->mColors[i];
		for (uint32_t j = 0; j < mesh->mNumVertices; j++)
		{
			vertexColors[j] = Vector4(reinterpret_cast<const float*>(&aiVertexColors[j]));
		}

		m_VertexColors.push_back(vertexColors);
	}

	if (mesh->HasFaces())
	{
		m_FaceCount = mesh->mNumFaces;
		for (uint32_t i = 0; i < m_FaceCount; i++)
		{
			aiFace* face = &mesh->mFaces[i];

			for (uint32_t j = 0; j < face->mNumIndices; j++)
			{
				m_Indices.push_back(face->mIndices[j]);
			}
		}
	}
}

Mesh::~Mesh()
{

}

Model* Mesh::GetModel()
{
	return m_pModel;
}

ModelMaterial* Mesh::GetMaterial()
{
	return m_pModelMaterial;
}

const std::string& Mesh::GetMeshName() const
{
	return m_MeshName;
}

const std::vector<Vector3>& Mesh::GetVertices() const
{
	return m_Vertices;
}

const std::vector<Vector3>& Mesh::GetNormals() const
{
	return m_Normals;
}

const std::vector<Vector3>& Mesh::GetTangents() const
{
	return m_Tangents;
}

const std::vector<Vector3>& Mesh::GetBiNormals() const
{
	return m_BiNormals;
}

const std::vector<std::vector<Vector3> >& Mesh::GetTextureCoordinates() const
{
	return m_TextureCoordinates;
}

const std::vector<std::vector<Vector4> >& Mesh::GetVertexColors() const
{
	return m_VertexColors;
}

uint32_t Mesh::GetFaceCount() const
{
	return m_FaceCount;
}

const std::vector<uint32_t>& Mesh::GetIndices() const
{
	return m_Indices;
}
