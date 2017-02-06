#pragma once
#include "../TinyEngineBase.h"
#include "../TinyEngineInterface.h"
#include "VertexTypes.h"

class Mesh;
class Material;

class Model : public boost::noncopyable
{
	friend class Mesh;

public:
	Model(const std::string& filename);
	Model(const char* pBuffer, uint32_t length);
	~Model();

	const std::vector<Mesh*>& GetMeshes() const;

	const BoundingBox& GetBoundingBox() const { return m_AABox; }
	const BoundingSphere& GetBoundingSphere() const { return m_Sphere; }

private:
	std::vector<Mesh*> m_Meshes;
	BoundingBox m_AABox;
	BoundingSphere m_Sphere;
};

class Mesh : public boost::noncopyable
{
public:
	Mesh(Model* pModel, const tinyxml2::XMLElement* pMeshNode);
	Mesh(std::vector<VertexPositionNormalTexture> vertices, std::vector<uint16_t> indices);
	~Mesh();

	enum PrimitiveType
	{
		PT_Unknow, PT_Point, PT_Line, PT_Triangle
	};

	PrimitiveType GetPrimitiveType() { return m_PrimitiveType; }

	const std::vector<Vector3>& GetVertices() const { return m_Vertices; }
	const std::vector<Vector3>& GetNormals() const { return m_Normals; }
	const std::vector<Vector3>& GetTangents() const { return m_Tangents; }
	const std::vector<Vector3>& GetBiNormals() const { return m_BiNormals; }
	const std::vector<std::vector<Vector2> >& GetTextureCoordinates() const { return m_TextureCoordinates; }
	const std::vector<std::vector<Vector4> >& GetVertexColors() const { return m_VertexColors; }
	const std::vector<uint32_t>& GetIndices() const { return m_Indices; }

	const BoundingBox& GetBoundingBox() const { return m_AABox; }
	const BoundingSphere& GetBoundingSphere() const { return m_Sphere; }

private:
	void CalculateTangentSpace();

	PrimitiveType m_PrimitiveType;
	std::vector<Vector3> m_Vertices;
	std::vector<Vector3> m_Normals;
	std::vector<Vector3> m_Tangents;
	std::vector<Vector3> m_BiNormals;
	std::vector<std::vector<Vector2> > m_TextureCoordinates;
	std::vector<std::vector<Vector4> > m_VertexColors;
	std::vector<uint32_t> m_Indices;

	BoundingBox m_AABox;
	BoundingSphere m_Sphere;
};
