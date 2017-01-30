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

	uint32_t flags = aiProcessPreset_TargetRealtime_Fast;

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

	uint32_t flags = aiProcessPreset_TargetRealtime_Fast;

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

	if (mesh->HasNormals())
	{
		m_Normals.reserve(mesh->mNumVertices);
		for (uint32_t i = 0; i < mesh->mNumVertices; i++)
		{
			m_Normals.push_back(Vector3(reinterpret_cast<const float*>(&mesh->mNormals[i])));
		}
	}

	uint32_t uvChannelCount = mesh->GetNumUVChannels();
	for (uint32_t i = 0; i < uvChannelCount; i++)
	{
		std::vector<Vector2> textureCoordinates(mesh->mNumVertices);

		aiVector3D* aiTextureCoordinates = mesh->mTextureCoords[i];
		for (uint32_t j = 0; j < mesh->mNumVertices; j++)
		{
			Vector3 temp = Vector3(reinterpret_cast<const float*>(&aiTextureCoordinates[j]));
			textureCoordinates[j] = Vector2(temp.x, temp.y);
		}

		m_TextureCoordinates.push_back(textureCoordinates);
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
}

Mesh::Mesh(std::vector<VertexPositionNormalTexture> vertices, std::vector<uint16_t> indices)
{
	m_Vertices.reserve(vertices.size());
	m_Normals.reserve(vertices.size());
	std::vector<Vector2> textureCoordinates;
	textureCoordinates.reserve(vertices.size());
	std::vector<Vector4> vertexColors;
	vertexColors.reserve(vertices.size());
	for (auto& vertex : vertices)
	{
		m_Vertices.push_back(vertex.position);
		m_Normals.push_back(vertex.normal);
		textureCoordinates.push_back(vertex.textureCoordinate);
		vertexColors.push_back(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	}
	m_TextureCoordinates.push_back(textureCoordinates);
	m_VertexColors.push_back(vertexColors);

	m_Indices.reserve(indices.size());
	for (auto index : indices)
	{
		m_Indices.push_back(index);
	}

	CalculateTangentSpace();
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

const std::vector<std::vector<Vector2> >& Mesh::GetTextureCoordinates() const
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

void Mesh::CalculateTangentSpace()
{
	std::vector<std::pair<Vector3, Vector3> > tbs;
	tbs.resize(m_Vertices.size());

	bool hasNorml = !m_Normals.empty();
	if (m_Normals.empty())
		m_Normals.resize(m_Vertices.size());

	for (uint32_t i = 0, len = m_Indices.size(); i < len; i += 3)
	{
		uint32_t vertex0 = m_Indices[i];
		uint32_t vertex1 = m_Indices[i + 1];
		uint32_t vertex2 = m_Indices[i + 2];

		Vector3 tri0 = m_Vertices.at(vertex0);
		Vector3 tri1 = m_Vertices.at(vertex1);
		Vector3 tri2 = m_Vertices.at(vertex2);

		Vector2 uv0 = m_TextureCoordinates[0].at(vertex0);
		Vector2 uv1 = m_TextureCoordinates[0].at(vertex1);
		Vector2 uv2 = m_TextureCoordinates[0].at(vertex2);

		Vector3 edge1 = tri1 - tri0;
		Vector3 edge2 = tri2 - tri0;
		Vector2 deltaUV1 = uv1 - uv0;
		Vector2 deltaUV2 = uv2 - uv0;

		if (!hasNorml)
		{
			Vector3 normal = edge1.Cross(edge2); normal.Normalize();
			m_Normals[vertex0] += normal;
			m_Normals[vertex1] += normal;
			m_Normals[vertex2] += normal;
		}

		float dirCorrection = (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y) < 0.0f ? -1.0f : 1.0f;
		if (deltaUV1 == Vector2::Zero && deltaUV2 == Vector2::Zero)
		{
			deltaUV1.x = 0.0f; deltaUV1.y = 1.0f;
			deltaUV2.x = 1.0f; deltaUV2.y = 0.0f;
		}

		Vector3 tangent, binormal;
		tangent = (edge1 * deltaUV2.y - edge2 * deltaUV1.y) * dirCorrection;
		binormal = (edge1 * deltaUV2.x - edge2 * deltaUV1.x) * dirCorrection;

		tbs[vertex0].first += tangent;
		tbs[vertex0].second += binormal;
		tbs[vertex1].first += tangent;
		tbs[vertex1].second += binormal;
		tbs[vertex2].first += tangent;
		tbs[vertex2].second += binormal;
	}

	m_Tangents.resize(m_Vertices.size());
	m_BiNormals.resize(m_Vertices.size());

	for (uint32_t i = 0, len = m_Vertices.size(); i < len; i += 3)
	{
		m_Normals[i].Normalize();
		const Vector3& normal = m_Normals[i];
		const std::pair<Vector3, Vector3>& tb = tbs[i];

// 		m_Tangents[i] = (tb.first - normal * normal.Dot(tb.first));
// 		m_Tangents[i].Normalize();
// 		m_BiNormals[i] = (tb.second - normal * normal.Dot(tb.second));
// 		m_BiNormals[i].Normalize();
// 
// 		if (m_Tangents[i].Cross(m_BiNormals[i]).Dot(normal) > 0.0f)
// 		{
// 			m_Tangents[i] = -m_Tangents[i];
// 			m_BiNormals[i] = -m_BiNormals[i];
// 		}

		Vector3 localTangent = tb.first - normal * (tb.first * normal);
		Vector3 localBinormal = tb.second - normal * (tb.second * normal);
		localTangent.Normalize();
		localBinormal.Normalize();

		bool invalidT = std::isnan(localTangent.x) || std::isfinite(localTangent.x) ||
			std::isnan(localTangent.x) || std::isfinite(localTangent.x) ||
			std::isnan(localTangent.x) || std::isfinite(localTangent.x);
		bool invalidB = std::isnan(localBinormal.x) || std::isfinite(localBinormal.x) ||
			std::isnan(localBinormal.x) || std::isfinite(localBinormal.x) ||
			std::isnan(localBinormal.x) || std::isfinite(localBinormal.x);

		if (invalidT != invalidB)
		{
			if (invalidT)
			{
				localTangent = normal.Cross(localBinormal);
				localTangent.Normalize();
			}
			else
			{
				localBinormal = localTangent.Cross(normal);
				localBinormal.Normalize();
			}
		}

		m_Tangents[i] = localTangent;
		m_BiNormals[i] = localBinormal;
	}
}
