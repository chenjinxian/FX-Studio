#include "BasicMaterial.h"
#include "ModelImporter.h"

BasicMaterial::BasicMaterial()
	: Material("main11"),
	m_pWorldViewProjection(nullptr)
{
}

void BasicMaterial::Initialize(Effect* pEffect)
{
	Material::Initialize(pEffect);
	m_pWorldViewProjection = pEffect->GetVariablesByName().at("WorldViewProjection");

	D3D11_INPUT_ELEMENT_DESC inputElementDescriptions[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	CreateInputLayout("main11", "p0", inputElementDescriptions, ARRAYSIZE(inputElementDescriptions));
}

void BasicMaterial::CreateVertexBuffer(const Mesh* pMesh, ID3D11Buffer** ppVertexBuffer) const
{
	const std::vector<Vector3>& sourceVertices = pMesh->GetVertices();

	std::vector<BasicMaterialVertex> vertices;
	vertices.reserve(sourceVertices.size());
	if (pMesh->GetVertexColors().size() > 0)
	{
		std::vector<Vector4>* vertexColors = pMesh->GetVertexColors().at(0);
		assert(vertexColors->size() == sourceVertices.size());

		for (UINT i = 0; i < sourceVertices.size(); i++)
		{
			XMFLOAT3 position = sourceVertices.at(i);
			XMFLOAT4 color = vertexColors->at(i);
			vertices.push_back(BasicMaterialVertex(XMFLOAT4(position.x, position.y, position.z, 1.0f), color));
		}
	}
	else
	{
		XMFLOAT4 color = XMFLOAT4(Colors::White);
		for (UINT i = 0; i < sourceVertices.size(); i++)
		{
			XMFLOAT3 position = sourceVertices.at(i);
			vertices.push_back(BasicMaterialVertex(XMFLOAT4(position.x, position.y, position.z, 1.0f), color));
		}
	}

	CreateVertexBuffer(&vertices[0], vertices.size(), ppVertexBuffer);
}

void BasicMaterial::CreateVertexBuffer(BasicMaterialVertex* vertices, UINT vertexCount, ID3D11Buffer** ppVertexBuffer) const
{
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.ByteWidth = VertexSize() * vertexCount;
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertexSubResourceData;
	ZeroMemory(&vertexSubResourceData, sizeof(vertexSubResourceData));
	vertexSubResourceData.pSysMem = vertices;
	if (FAILED(DXUTGetD3D11Device()->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, ppVertexBuffer)))
	{
		DEBUG_ERROR("ID3D11Device::CreateBuffer() failed.");
	}
}

UINT BasicMaterial::VertexSize() const
{
	return sizeof(BasicMaterialVertex);
}
