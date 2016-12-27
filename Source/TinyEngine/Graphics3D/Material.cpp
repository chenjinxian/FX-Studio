#include "Material.h"
#include "ModelImporter.h"
#include <d3dcompiler.h>
#include <cctype>

Effect::Effect(ID3D11Device1* pDevice, ID3DX11Effect* pD3DX11Effect)
	: m_Techniques(),
	m_TechniquesByName(),
	m_Variables(),
	m_VariablesByName()
{
	DEBUG_ASSERT(pDevice != nullptr);
	DEBUG_ASSERT(pD3DX11Effect != nullptr);

	D3DX11_EFFECT_DESC effectDesc;
	HRESULT hr = pD3DX11Effect->GetDesc(&effectDesc);
	if (FAILED(hr))
	{
		DEBUG_ERROR("ID3DX11Effect::GetDesc() failed.");
	}

	for (uint32_t i = 0; i < effectDesc.Techniques; i++)
	{
		Technique* technique = new Technique(pDevice, pD3DX11Effect->GetTechniqueByIndex(i));
		m_Techniques.push_back(technique);
		m_TechniquesByName.insert(std::make_pair(technique->GetTechniqueName(), technique));
	}

	for (uint32_t i = 0; i < effectDesc.GlobalVariables; i++)
	{
		Variable* variable = new Variable(pDevice, pD3DX11Effect->GetVariableByIndex(i));
		m_Variables.push_back(variable);
		m_VariablesByName.insert(std::make_pair(variable->GetVariableName(), variable));
	}
}

Effect::~Effect()
{
	for (auto technique : m_Techniques)
	{
		delete technique;
	}
	m_Techniques.clear();

	for (auto variable : m_Variables)
	{
		delete variable;
	}
	m_Variables.clear();
}

const std::vector<Technique*>& Effect::GetTechniques() const
{
	return m_Techniques;
}

const std::map<std::string, Technique*>& Effect::GetTechniquesByName() const
{
	return m_TechniquesByName;
}

const std::vector<Variable*>& Effect::GetVariables() const
{
	return m_Variables;
}

const std::map<std::string, Variable*>& Effect::GetVariablesByName() const
{
	return m_VariablesByName;
}

Technique::Technique(ID3D11Device1* pDevice, ID3DX11EffectTechnique* pD3DX11EffectTechnique)
	: m_pD3DX11EffectTechnique(pD3DX11EffectTechnique),
	m_D3DX11TechniqueDesc(),
	m_TechniqueName(),
	m_Passes(),
	m_PassesByName()
{
	m_pD3DX11EffectTechnique->GetDesc(&m_D3DX11TechniqueDesc);
	m_TechniqueName = m_D3DX11TechniqueDesc.Name;
	for (uint32_t i = 0; i < m_D3DX11TechniqueDesc.Passes; i++)
	{
		Pass* pass = DEBUG_NEW Pass(pDevice, m_pD3DX11EffectTechnique->GetPassByIndex(i));
		m_Passes.push_back(pass);
		m_PassesByName.insert(std::make_pair(pass->GetPassName(), pass));
	}
}

Technique::~Technique()
{
	for (auto pass : m_Passes)
	{
		SAFE_DELETE(pass);
	}
	m_Passes.clear();
}

const std::string& Technique::GetTechniqueName() const
{
	return m_TechniqueName;
}

const std::vector<Pass*>& Technique::GetPasses() const
{
	return m_Passes;
}

const std::map<std::string, Pass*>& Technique::GetPassesByName() const
{
	return m_PassesByName;
}

Pass::Pass(ID3D11Device1* pDevice, ID3DX11EffectPass* pD3DX11EffectPass)
	: p_Device(pDevice),
	m_pD3DX11EffectPass(pD3DX11EffectPass),
	m_PassName(),
	m_pInputLayouts(nullptr),
	m_VertexFormat(),
	m_VertexSize(0)
{
	D3DX11_PASS_DESC passDesc;
	m_pD3DX11EffectPass->GetDesc(&passDesc);
	m_PassName = passDesc.Name;


	D3DX11_PASS_SHADER_DESC vertexShaderDesc;
	m_pD3DX11EffectPass->GetVertexShaderDesc(&vertexShaderDesc);
	ID3DX11EffectShaderVariable* pShaderVariable = vertexShaderDesc.pShaderVariable;
	if (pShaderVariable != nullptr)
	{
		D3DX11_EFFECT_SHADER_DESC shaderDesc;
		pShaderVariable->GetShaderDesc(vertexShaderDesc.ShaderIndex, &shaderDesc);
		std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDescs(shaderDesc.NumInputSignatureEntries);

		for (uint32_t i = 0; i < shaderDesc.NumInputSignatureEntries; i++)
		{
			D3D11_SIGNATURE_PARAMETER_DESC parameterDesc;
			vertexShaderDesc.pShaderVariable->GetInputSignatureElementDesc(vertexShaderDesc.ShaderIndex, i, &parameterDesc);
			inputElementDescs[i].SemanticName = parameterDesc.SemanticName;
			inputElementDescs[i].SemanticIndex = parameterDesc.SemanticIndex;
			inputElementDescs[i].Format = GetElementFormat(parameterDesc.ComponentType, parameterDesc.Mask);
			inputElementDescs[i].InputSlot = 0;
			inputElementDescs[i].AlignedByteOffset = parameterDesc.Register > 0 ? D3D11_APPEND_ALIGNED_ELEMENT : 0;
			inputElementDescs[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			inputElementDescs[i].InstanceDataStepRate = 0;

			m_VertexFormat.push_back(inputElementDescs[i].SemanticName);
		}
	
		pDevice->CreateInputLayout(
			&inputElementDescs[0], inputElementDescs.size(), passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &m_pInputLayouts);
	}
}

void Pass::CreateVertexBuffer(const void* pVertexData, uint32_t size, ID3D11Buffer** ppVertexBuffer) const
{
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.ByteWidth = size;
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertexSubResourceData;
	ZeroMemory(&vertexSubResourceData, sizeof(vertexSubResourceData));
	vertexSubResourceData.pSysMem = pVertexData;

	if (FAILED(p_Device->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, ppVertexBuffer)))
	{
		DEBUG_ERROR("ID3D11Device::CreateBuffer() failed.");
	}
}

void Pass::CreateIndexBuffer(const void* pIndexData, uint32_t size, ID3D11Buffer** ppIndexBuffer) const
{
	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.ByteWidth = size;
	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexSubResourceData;
	ZeroMemory(&indexSubResourceData, sizeof(indexSubResourceData));
	indexSubResourceData.pSysMem = pIndexData;

	if (FAILED(p_Device->CreateBuffer(&indexBufferDesc, &indexSubResourceData, ppIndexBuffer)))
	{
		DEBUG_ERROR("ID3D11Device::CreateBuffer() failed.");
	}
}

void Pass::CreateVertexBuffer(const Mesh* mesh, ID3D11Buffer** ppVertexBuffer) const
{
	std::vector<Vector3> vertices = mesh->GetVertices();
	uint32_t vertexCount = vertices.size();
	std::vector<Vector2> textureCoordinates;
	if (!mesh->GetTextureCoordinates().empty())
	{
		textureCoordinates = mesh->GetTextureCoordinates().at(0);
	}

	std::vector<float> vertexData;
	vertexData.reserve(vertexCount * m_VertexSize);

	for (uint32_t i = 0; i < vertexCount; i++)
	{
		for (auto& vertexFormat : m_VertexFormat)
		{
			if (vertexFormat == "POSITION")
			{
				vertexData.push_back(vertices.at(i).x);
				vertexData.push_back(vertices.at(i).y);
				vertexData.push_back(vertices.at(i).z);
				vertexData.push_back(1.0f);
			}
			else if (vertexFormat == "TEXCOORD")
			{
				if (textureCoordinates.size() > i)
				{
					vertexData.push_back(textureCoordinates.at(i).x);
					vertexData.push_back(textureCoordinates.at(i).y);
				}
			}
		}
	}

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.ByteWidth = m_VertexSize * vertexCount;
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertexSubResourceData;
	ZeroMemory(&vertexSubResourceData, sizeof(vertexSubResourceData));
	vertexSubResourceData.pSysMem = &vertexData[0];

	if (FAILED(p_Device->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, ppVertexBuffer)))
	{
		DEBUG_ERROR("ID3D11Device::CreateBuffer() failed.");
	}
}

void Pass::CreateIndexBuffer(const Mesh* mesh, ID3D11Buffer** ppIndexBuffer) const
{
	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.ByteWidth = sizeof(uint32_t) * (uint32_t)mesh->GetIndices().size();
	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexSubResourceData;
	ZeroMemory(&indexSubResourceData, sizeof(indexSubResourceData));
	indexSubResourceData.pSysMem = &(mesh->GetIndices()[0]);

	if (FAILED(p_Device->CreateBuffer(&indexBufferDesc, &indexSubResourceData, ppIndexBuffer)))
	{
		DEBUG_ERROR("ID3D11Device::CreateBuffer() failed.");
	}
}

void Pass::Apply(uint32_t flags, ID3D11DeviceContext1* pDeviceContext)
{
	m_pD3DX11EffectPass->Apply(flags, pDeviceContext);
}

DXGI_FORMAT Pass::GetElementFormat(D3D_REGISTER_COMPONENT_TYPE compoentType, uint8_t mask)
{
	switch (compoentType)
	{
	case D3D_REGISTER_COMPONENT_UNKNOWN:
		return DXGI_FORMAT_UNKNOWN;
	case D3D_REGISTER_COMPONENT_UINT32:
		switch (mask & 0xf)
		{
		case 15: m_VertexSize += 16; return DXGI_FORMAT_R32G32B32A32_UINT;
		case 7: m_VertexSize += 12; return DXGI_FORMAT_R32G32B32_UINT;
		case 3: m_VertexSize += 8; return DXGI_FORMAT_R32G32_UINT;
		case 1: m_VertexSize += 4; return DXGI_FORMAT_R32_UINT;
		default: return DXGI_FORMAT_UNKNOWN;
		}
	case D3D_REGISTER_COMPONENT_SINT32:
		DEBUG_ERROR("need to add new format!");
		return DXGI_FORMAT_UNKNOWN;
	case D3D_REGISTER_COMPONENT_FLOAT32:
	{
		switch (mask & 0xf)
		{
		case 15: m_VertexSize += 16; return DXGI_FORMAT_R32G32B32A32_FLOAT;
		case 7: m_VertexSize += 12; return DXGI_FORMAT_R32G32B32_FLOAT;
		case 3: m_VertexSize += 8; return DXGI_FORMAT_R32G32_FLOAT;
		case 1: m_VertexSize += 4; return DXGI_FORMAT_R32_FLOAT;
		default: return DXGI_FORMAT_UNKNOWN;
		}
	}
	default:
		return DXGI_FORMAT_UNKNOWN;
	}
}

Variable::Variable(ID3D11Device1* pDevice, ID3DX11EffectVariable* pD3DX11EffectVariable)
	: m_pD3DX11EffectVariable(pD3DX11EffectVariable),
	m_pD3DX11EffectType(nullptr),
	m_VariableName()
{
	D3DX11_EFFECT_VARIABLE_DESC variableDesc;
	m_pD3DX11EffectVariable->GetDesc(&variableDesc);
	m_VariableName = variableDesc.Name;
	if (variableDesc.Semantic != nullptr)
	{
		m_VariableSemantic = variableDesc.Semantic;
		std::transform(m_VariableSemantic.begin(), m_VariableSemantic.end(), m_VariableSemantic.begin(), (int(*)(int)) std::tolower);
	}

	m_pD3DX11EffectType = m_pD3DX11EffectVariable->GetType();
	D3DX11_EFFECT_TYPE_DESC typeDesc;
	m_pD3DX11EffectType->GetDesc(&typeDesc);
	m_VariableType = typeDesc.TypeName;
}

void Variable::SetMatrix(XMMATRIX value)
{
	ID3DX11EffectMatrixVariable* variable = m_pD3DX11EffectVariable->AsMatrix();
	if (!variable->IsValid())
	{
		DEBUG_ERROR("Invalid effect variable cast.");
	}

	variable->SetMatrix(reinterpret_cast<const float*>(&value));
}

void Variable::SetResource(ID3D11ShaderResourceView* value)
{
	ID3DX11EffectShaderResourceVariable* variable = m_pD3DX11EffectVariable->AsShaderResource();
	if (!variable->IsValid())
	{
		DEBUG_ERROR("Invalid effect variable cast.");
	}

	variable->SetResource(value);
}

Variable& Variable::operator<<(ID3D11UnorderedAccessView* value)
{
	ID3DX11EffectUnorderedAccessViewVariable* variable = m_pD3DX11EffectVariable->AsUnorderedAccessView();
	if (!variable->IsValid())
	{
		DEBUG_ERROR("Invalid effect variable cast.");
	}

	variable->SetUnorderedAccessView(value);

	return *this;
}

Variable& Variable::operator<<(FXMVECTOR value)
{
	ID3DX11EffectVectorVariable* variable = m_pD3DX11EffectVariable->AsVector();
	if (!variable->IsValid())
	{
		DEBUG_ERROR("Invalid effect variable cast.");
	}

	variable->SetFloatVector(reinterpret_cast<const float*>(&value));

	return *this;
}

Variable& Variable::operator<<(int value)
{
	ID3DX11EffectScalarVariable* variable = m_pD3DX11EffectVariable->AsScalar();
	if (!variable->IsValid())
	{
		DEBUG_ERROR("Invalid effect variable cast.");
	}

	variable->SetInt(value);

	return *this;
}

Variable& Variable::operator<<(float value)
{
	ID3DX11EffectScalarVariable* variable = m_pD3DX11EffectVariable->AsScalar();
	if (!variable->IsValid())
	{
		DEBUG_ERROR("Invalid effect variable cast.");
	}

	variable->SetFloat(value);

	return *this;
}

Variable& Variable::operator<<(const std::vector<float>& values)
{
	ID3DX11EffectScalarVariable* variable = m_pD3DX11EffectVariable->AsScalar();
	if (!variable->IsValid())
	{
		DEBUG_ERROR("Invalid effect variable cast.");
	}

	variable->SetFloatArray(&values[0], 0, (uint32_t)values.size());

	return *this;
}

Variable& Variable::operator<<(const std::vector<XMFLOAT2>& values)
{
	ID3DX11EffectVectorVariable* variable = m_pD3DX11EffectVariable->AsVector();
	if (!variable->IsValid())
	{
		DEBUG_ERROR("Invalid effect variable cast.");
	}

	variable->SetFloatVectorArray(reinterpret_cast<const float*>(&values[0]), 0, (uint32_t)values.size());

	return *this;
}

Variable& Variable::operator<<(const std::vector<XMFLOAT4X4>& values)
{
	ID3DX11EffectMatrixVariable* variable = m_pD3DX11EffectVariable->AsMatrix();
	if (!variable->IsValid())
	{
		DEBUG_ERROR("Invalid effect variable cast.");
	}

	variable->SetMatrixArray(reinterpret_cast<const float*>(&values[0]), 0, (uint32_t)values.size());

	return *this;
}
