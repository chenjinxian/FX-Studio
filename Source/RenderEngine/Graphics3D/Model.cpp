#include "Model.h"


Model::Model(const std::string& filename, bool flipUVs /*= false*/)
{

}

Model::~Model()
{

}

bool Model::HasMeshes() const
{
	return true;
}

bool Model::HasMaterials() const
{
	return true;
}

const std::vector<Mesh*>& Model::GetMeshes() const
{
	return m_Meshes;
}

const std::vector<ModelMaterial*>& Model::GetMaterials() const
{
	return m_Materials;
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

const std::vector<std::vector<Vector3>*>& Mesh::GetTextureCoordinates() const
{
	return m_TextureCoordinates;
}

const std::vector<std::vector<Vector4>*>& Mesh::GetVertexColors() const
{
	return m_VertexColors;
}

UINT Mesh::FaceCount() const
{
	return m_FaceCount;
}

const std::vector<UINT>& Mesh::Indices() const
{
	return m_Indices;
}

void Mesh::CreateIndexBuffer(ID3D11Buffer** indexBuffer)
{

}
