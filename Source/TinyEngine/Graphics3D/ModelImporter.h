#pragma once
#include "../TinyEngineBase.h"
#include "../TinyEngineInterface.h"

struct aiMesh;
class Mesh;
class Material;
class ModelMaterial;

class Model : public boost::noncopyable
{
	friend class Mesh;

public:
	Model(const std::string& filename, bool flipUVs = false);
	Model(const void* pBuffer, uint32_t length, bool flipUVs = false);
	~Model();

	bool HasMeshes() const;
	bool HasMaterials() const;

	const std::vector<Mesh*>& GetMeshes() const;
	const std::vector<ModelMaterial*>& GetMaterials() const;

private:
	std::vector<Mesh*> m_Meshes;
	std::vector<ModelMaterial*> m_Materials;
};

class Mesh : public boost::noncopyable
{
	friend class Model;

public:
	Mesh(Model* pModel, aiMesh* mesh);
	~Mesh();

	Model* GetModel();
	ModelMaterial* GetMaterial();
	const std::string& GetMeshName() const;

	const std::vector<Vector3>& GetVertices() const;
	const std::vector<Vector3>& GetNormals() const;
	const std::vector<Vector3>& GetTangents() const;
	const std::vector<Vector3>& GetBiNormals() const;
	const std::vector<std::vector<Vector3>*>& GetTextureCoordinates() const;
	const std::vector<std::vector<Vector4>*>& GetVertexColors() const;
	uint32_t GetFaceCount() const;
	const std::vector<uint32_t>& GetIndices() const;

	void CreateIndexBuffer(ID3D11Buffer** ppIndexBuffer);

private:
	Model* m_pModel;
	ModelMaterial* m_pModelMaterial;
	std::string m_MeshName;
	std::vector<Vector3> m_Vertices;
	std::vector<Vector3> m_Normals;
	std::vector<Vector3> m_Tangents;
	std::vector<Vector3> m_BiNormals;
	std::vector<std::vector<Vector3>*> m_TextureCoordinates;
	std::vector<std::vector<Vector4>*> m_VertexColors;
	uint32_t m_FaceCount;
	std::vector<uint32_t> m_Indices;
};
