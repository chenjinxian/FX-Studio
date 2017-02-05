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

private:
	std::vector<Mesh*> m_Meshes;
};

class Mesh : public boost::noncopyable
{
public:
	Mesh(Model* pModel, const tinyxml2::XMLElement* pMeshNode);
	Mesh(std::vector<VertexPositionNormalTexture> vertices, std::vector<uint16_t> indices);
	~Mesh();

	enum PrimitiveType
	{
		PT_Point, PT_Line, PT_Triangle
	};

	PrimitiveType GetPrimitiveType() { return m_PrimitiveType; }
	Model* GetModel();
	const std::string& GetMeshName() const;

	const std::vector<Vector3>& GetVertices() const;
	const std::vector<Vector3>& GetNormals() const;
	const std::vector<Vector3>& GetTangents() const;
	const std::vector<Vector3>& GetBiNormals() const;
	const std::vector<std::vector<Vector2> >& GetTextureCoordinates() const;
	const std::vector<std::vector<Vector4> >& GetVertexColors() const;
	uint32_t GetFaceCount() const;
	const std::vector<uint32_t>& GetIndices() const;

private:
	void CalculateTangentSpace();

	PrimitiveType m_PrimitiveType;
	Model* m_pModel;
	std::string m_MeshName;
	std::vector<Vector3> m_Vertices;
	std::vector<Vector3> m_Normals;
	std::vector<Vector3> m_Tangents;
	std::vector<Vector3> m_BiNormals;
	std::vector<std::vector<Vector2> > m_TextureCoordinates;
	std::vector<std::vector<Vector4> > m_VertexColors;
	std::vector<uint32_t> m_Indices;
	uint32_t m_FaceCount;

	BoundingBox m_AABox;
	BoundingSphere m_Sphere;
};
