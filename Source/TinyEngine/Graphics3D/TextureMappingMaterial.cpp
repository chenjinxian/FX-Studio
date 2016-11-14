#include "TextureMappingMaterial.h"
#include "ModelImporter.h"

TextureMappingMaterial::TextureMappingMaterial()
	: Material("main11"),
	m_pWorldViewProjection(nullptr),
	m_pColorTexture(nullptr)
{
}

void TextureMappingMaterial::Initialize(Effect* effect)
{
	Material::Initialize(effect);

	m_pWorldViewProjection = m_pEffect->GetVariablesByName().at("WorldViewProjection");
	m_pColorTexture = m_pEffect->GetVariablesByName().at("ColorTexture");

	D3D11_INPUT_ELEMENT_DESC inputElementDescriptions[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	CreateInputLayout("main11", "p0", inputElementDescriptions, ARRAYSIZE(inputElementDescriptions));
}

void TextureMappingMaterial::CreateVertexBuffer(const Mesh* pMesh, ID3D11Buffer** ppVertexBuffer) const
{
	const std::vector<Vector3>& sourceVertices = pMesh->GetVertices();

	std::vector<TextureMappingMaterialVertex> vertices;
	vertices.reserve(sourceVertices.size());
	if (pMesh->GetTextureCoordinates().size() > 0)
	{
		std::vector<Vector3>* textureCoordinates = pMesh->GetTextureCoordinates().at(0);
		assert(textureCoordinates->size() == sourceVertices.size());

		for (UINT i = 0; i < sourceVertices.size(); i++)
		{
			Vector3 position = sourceVertices.at(i);
			Vector3 uv = textureCoordinates->at(i);
			vertices.push_back(TextureMappingMaterialVertex(XMFLOAT4(position.x, position.y, position.z, 1.0f), XMFLOAT2(uv.x, uv.y)));
		}
	}

// 	CreateVertexBuffer(&vertices[0], vertices.size(), ppVertexBuffer);
}

void TextureMappingMaterial::CreateVertexBuffer(
	TextureMappingMaterialVertex* vertices, uint32_t vertexCount, ID3D11Buffer** ppVertexBuffer) const
{
// 	D3D11_BUFFER_DESC vertexBufferDesc;
// 	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
// 	vertexBufferDesc.ByteWidth = VertexSize() * vertexCount;
// 	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
// 	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
// 
// 	D3D11_SUBRESOURCE_DATA vertexSubResourceData;
// 	ZeroMemory(&vertexSubResourceData, sizeof(vertexSubResourceData));
// 	vertexSubResourceData.pSysMem = vertices;
// 	if (FAILED(DXUTGetD3D11Device()->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, ppVertexBuffer)))
// 	{
// 		DEBUG_ERROR("ID3D11Device::CreateBuffer() failed.");
// 	}
}

UINT TextureMappingMaterial::VertexSize() const
{
	return sizeof(TextureMappingMaterialVertex);
}
