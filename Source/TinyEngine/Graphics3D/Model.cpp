#include "Model.h"
#include "../Utilities/SpatialSort.h"
#include <sstream>

Model::Model(const std::string& filename)
	: m_Meshes()
{
	unique_ptr<tinyxml2::XMLDocument> pDoc = unique_ptr<tinyxml2::XMLDocument>(DEBUG_NEW tinyxml2::XMLDocument());
	if (pDoc != nullptr && (pDoc->LoadFile(filename.c_str()) == tinyxml2::XML_SUCCESS))
	{
		tinyxml2::XMLElement *pRoot = pDoc->RootElement();
		if (pRoot == nullptr)
			return;

		tinyxml2::XMLElement* pNode = pRoot->FirstChildElement("MeshList");
		if (pNode != nullptr)
		{
			for (tinyxml2::XMLElement* pMeshNode = pNode->FirstChildElement(); pMeshNode; pMeshNode = pMeshNode->NextSiblingElement())
			{
				Mesh* mesh = DEBUG_NEW Mesh(this, pMeshNode);
				m_Meshes.push_back(mesh);
				BoundingBox::CreateMerged(m_AABox, mesh->GetBoundingBox(), m_AABox);
			}
		}
	}

	BoundingSphere::CreateFromBoundingBox(m_Sphere, m_AABox);
}

Model::Model(const char* pBuffer, uint32_t length)
	: m_Meshes()
{
	unique_ptr<tinyxml2::XMLDocument> pDoc = unique_ptr<tinyxml2::XMLDocument>(DEBUG_NEW tinyxml2::XMLDocument());
	if (pDoc != nullptr && (pDoc->Parse(pBuffer, length) == tinyxml2::XML_SUCCESS))
	{
		tinyxml2::XMLElement *pRoot = pDoc->RootElement();
		if (pRoot == nullptr)
			return;

		tinyxml2::XMLElement* pNode = pRoot->FirstChildElement("MeshList");
		if (pNode != nullptr)
		{
			for (tinyxml2::XMLElement* pMeshNode = pNode->FirstChildElement(); pMeshNode; pMeshNode = pMeshNode->NextSiblingElement())
			{
				Mesh* mesh = DEBUG_NEW Mesh(this, pMeshNode);
				m_Meshes.push_back(mesh);
				BoundingBox::CreateMerged(m_AABox, mesh->GetBoundingBox(), m_AABox);
			}
		}
	}

	BoundingSphere::CreateFromBoundingBox(m_Sphere, m_AABox);
}

Model::~Model()
{
	for (auto mesh : m_Meshes)
	{
		SAFE_DELETE(mesh);
	}
	m_Meshes.clear();
}

const std::vector<Mesh*>& Model::GetMeshes() const
{
	return m_Meshes;
}

Mesh::Mesh(Model* pModel, const tinyxml2::XMLElement* pMeshNode)
	: m_PrimitiveType(PT_Unknow),
	m_Vertices(),
	m_Normals(),
	m_Tangents(),
	m_BiNormals(),
	m_TextureCoordinates(),
	m_VertexColors(),
	m_Indices()
{
	DEBUG_ASSERT(pMeshNode != nullptr);

	std::string type = pMeshNode->Attribute("types");
	if (type == "points")
		m_PrimitiveType = PT_Point;
	else if (type == "lines")
		m_PrimitiveType = PT_Line;
	else if (type == "triangles")
		m_PrimitiveType = PT_Triangle;

	for (const tinyxml2::XMLElement* pNode = pMeshNode->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
	{
		if (0 == strcmp(pNode->Name(), "FaceList"))
		{
			for (const tinyxml2::XMLElement* pFace = pNode->FirstChildElement(); pFace; pFace = pFace->NextSiblingElement())
			{
				std::stringstream ss(pFace->GetText());
				int index = 0;
				for (int i = 0, count = pFace->IntAttribute("num"); i < count; i++)
				{
					ss >> index;
					m_Indices.push_back(index);
				}
			}
		}
		else if (0 == strcmp(pNode->Name(), "Positions"))
		{
			m_Vertices.resize(pNode->IntAttribute("num"));

			uint32_t i = 0;
			for (const tinyxml2::XMLElement* pPosition = pNode->FirstChildElement(); pPosition; pPosition = pPosition->NextSiblingElement())
			{
				sscanf_s(pPosition->GetText(), "%f %f %f", &m_Vertices[i].x, &m_Vertices[i].y, &m_Vertices[i].z);
				i++;
			}

			BoundingBox::CreateFromPoints(m_AABox, m_Vertices.size(), &m_Vertices.front(), sizeof(Vector3));
		}
		else if (0 == strcmp(pNode->Name(), "Normals"))
		{
			m_Normals.resize(pNode->IntAttribute("num"));

			uint32_t i = 0;
			for (const tinyxml2::XMLElement* pNormal = pNode->FirstChildElement(); pNormal; pNormal = pNormal->NextSiblingElement())
			{
				sscanf_s(pNormal->GetText(), "%f %f %f", &m_Normals[i].x, &m_Normals[i].y, &m_Normals[i].z);
				i++;
			}
		}
		else if (0 == strcmp(pNode->Name(), "Tangents"))
		{
			m_Tangents.resize(pNode->IntAttribute("num"));

			uint32_t i = 0;
			for (const tinyxml2::XMLElement* pTangent = pNode->FirstChildElement(); pTangent; pTangent = pTangent->NextSiblingElement())
			{
				sscanf_s(pTangent->GetText(), "%f %f %f", &m_Tangents[i].x, &m_Tangents[i].y, &m_Tangents[i].z);
				i++;
			}
		}
		else if (0 == strcmp(pNode->Name(), "BiNormals"))
		{
			m_BiNormals.resize(pNode->IntAttribute("num"));

			uint32_t i = 0;
			for (const tinyxml2::XMLElement* pBiNormal = pNode->FirstChildElement(); pBiNormal; pBiNormal = pBiNormal->NextSiblingElement())
			{
				sscanf_s(pBiNormal->GetText(), "%f %f %f", &m_BiNormals[i].x, &m_BiNormals[i].y, &m_BiNormals[i].z);
				i++;
			}
		}
		else if (0 == strcmp(pNode->Name(), "TextureCoords"))
		{
			std::vector<Vector2> textureCoordinates(pNode->IntAttribute("num"));

			uint32_t i = 0;
			for (const tinyxml2::XMLElement* pUV = pNode->FirstChildElement(); pUV; pUV = pUV->NextSiblingElement())
			{
				sscanf_s(pUV->GetText(), "%f %f", &textureCoordinates[i].x, &textureCoordinates[i].y);
				i++;
			}

			m_TextureCoordinates.push_back(textureCoordinates);
		}
		else if (0 == strcmp(pNode->Name(), "Colors"))
		{
			std::vector<Vector4> vertexColors(pNode->IntAttribute("num"));

			uint32_t i = 0;
			for (const tinyxml2::XMLElement* pColor = pNode->FirstChildElement(); pColor; pColor = pColor->NextSiblingElement())
			{
				sscanf_s(pColor->GetText(), "%f %f %f %f", &vertexColors[i].x, &vertexColors[i].y, &vertexColors[i].z, &vertexColors[i].z);
				i++;
			}

			m_VertexColors.push_back(vertexColors);
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

	BoundingBox::CreateFromPoints(m_AABox, m_Vertices.size(), &m_Vertices.front(), sizeof(Vector3));
	BoundingSphere::CreateFromBoundingBox(m_Sphere, m_AABox);
	CalculateTangentSpace();
}

Mesh::~Mesh()
{

}

void Mesh::CalculateTangentSpace()
{
	if (!m_Tangents.empty()) return;
	if (m_Normals.empty() || m_TextureCoordinates.empty())
	{
		DEBUG_ERROR("Generate vertex normal and texturecoords first");
		return;
	}

	const float angleEpsilon = 0.9999f;
	std::vector<bool> vertexDone(m_Vertices.size(), false);

	m_Tangents.resize(m_Vertices.size());
	m_BiNormals.resize(m_Vertices.size());

	for (uint32_t i = 0, len = m_Indices.size(); i < len; i += 3)
	{
		const uint32_t vertex0 = m_Indices[i];
		const uint32_t vertex1 = m_Indices[i + 1];
		const uint32_t vertex2 = m_Indices[i + 2];

		Vector3 edge1 = m_Vertices.at(vertex1) - m_Vertices.at(vertex0);
		Vector3 edge2 = m_Vertices.at(vertex2) - m_Vertices.at(vertex0);
		Vector2 deltaUV1 = m_TextureCoordinates[0].at(vertex1) - m_TextureCoordinates[0].at(vertex0);
		Vector2 deltaUV2 = m_TextureCoordinates[0].at(vertex2) - m_TextureCoordinates[0].at(vertex0);

		float dirCorrection = (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y) < 0.0f ? -1.0f : 1.0f;
		if (deltaUV1 == Vector2::Zero && deltaUV2 == Vector2::Zero)
		{
			deltaUV1.x = 0.0f; deltaUV1.y = 1.0f;
			deltaUV2.x = 1.0f; deltaUV2.y = 0.0f;
		}

		Vector3 tangent = (edge1 * deltaUV2.y - edge2 * deltaUV1.y) * dirCorrection;
		Vector3 binormal = (edge1 * deltaUV2.x - edge2 * deltaUV1.x) * dirCorrection;
	
		for (uint32_t j = 0; j < 3; j++)
		{
			const uint32_t index = m_Indices[i + j];
			Vector3 localTangent = tangent - m_Normals[index] * (tangent.Dot(m_Normals[index]));
			Vector3 localBinormal = binormal - m_Normals[index] * (binormal.Dot(m_Normals[index]));
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
					localTangent = m_Normals[index].Cross(localBinormal);
					localTangent.Normalize();
				}
				else
				{
					localBinormal = localTangent.Cross(m_Normals[index]);
					localBinormal.Normalize();
				}
			}

			m_Tangents[index] = localTangent;
			m_BiNormals[index] = localBinormal;
		}
	}

	std::unique_ptr<SpatialSort> vertexFinder(DEBUG_NEW SpatialSort(m_Vertices));
	float posEpsilon = (m_AABox.Extents * 2.0f).Length() * 0.0001f;

	std::vector<uint32_t> verticesFound;
	const float fLimit = cos(XMConvertToRadians(45.0f));
	std::vector<uint32_t> closeVertices;

	for (uint32_t i = 0, len = m_Vertices.size(); i < len; i++)
	{
		if (vertexDone[i])
			continue;

		closeVertices.clear();
		vertexFinder->FindPositions(m_Vertices[i], posEpsilon, verticesFound);
		closeVertices.reserve(verticesFound.size() + 5);
		closeVertices.push_back(i);

		for (uint32_t j : verticesFound)
		{
			if (vertexDone[j])
				continue;
			if (m_Normals[j].Dot(m_Normals[i]) < angleEpsilon)
				continue;
			if (m_Tangents[j].Dot(m_Tangents[i]) < fLimit)
				continue;
			if (m_BiNormals[j].Dot(m_BiNormals[i]) < fLimit)
				continue;

			closeVertices.push_back(j);
			vertexDone[j] = true;
		}

		Vector3 smoothTangent, smoothBinormal;
		for (uint32_t j : closeVertices)
		{
			smoothTangent += m_Tangents[j];
			smoothBinormal += m_BiNormals[j];
		}
		smoothTangent.Normalize();
		smoothBinormal.Normalize();

		for (uint32_t j : closeVertices)
		{
			m_Tangents[j] = smoothTangent;
			m_BiNormals[j] = smoothBinormal;
		}
	}
}
