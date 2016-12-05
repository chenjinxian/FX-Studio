#include "Material.h"
#include "ModelImporter.h"
#include <d3dcompiler.h>

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
	m_VertexFormat()
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

			m_VertexFormat.insert(std::make_pair(inputElementDescs[i].SemanticName, inputElementDescs[i].Format));
		}
	
		pDevice->CreateInputLayout(
			&inputElementDescs[0], inputElementDescs.size(), passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &m_pInputLayouts);
	}
}

void Pass::CreateVertexBuffer(const Mesh* mesh, ID3D11Buffer** ppVertexBuffer) const
{
	std::vector<float> vertexData;
	uint32_t vertexSize = 0;
	uint32_t vertexCount = mesh->GetVertices().size();
	std::vector<Vector3> textureCoordinates;
	if (!mesh->GetTextureCoordinates().empty())
	{
		std::vector<Vector3> textureCoordinates = mesh->GetTextureCoordinates().at(0);
	}

	for (uint32_t i = 0; i < vertexCount; i++)
	{
		for (auto vertexFormat : m_VertexFormat)
		{
			std::string type = vertexFormat.first;
			DXGI_FORMAT format = vertexFormat.second;

			if (type == "POSITION")
			{
				vertexData.push_back(mesh->GetVertices().at(i).x);
				vertexData.push_back(mesh->GetVertices().at(i).y);
				vertexData.push_back(mesh->GetVertices().at(i).z);
				vertexData.push_back(1.0f);
				vertexSize += 16;
			}
			else if (type == "TEXCOORD")
			{
				if (textureCoordinates.size() > i)
				{
					vertexData.push_back(textureCoordinates.at(i).x);
					vertexData.push_back(textureCoordinates.at(i).y);
					vertexSize += 8;
				}
			}
		}
	}

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.ByteWidth = vertexSize * vertexCount;
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

void Pass::Apply(uint32_t flags)
{
// 	m_pD3DX11EffectPass->Apply(flags, DXUTGetD3D11DeviceContext());
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
		case 15: return DXGI_FORMAT_R32G32B32A32_UINT;
		case 7: return DXGI_FORMAT_R32G32B32_UINT;
		case 3: return DXGI_FORMAT_R32G32_UINT;
		case 1: return DXGI_FORMAT_R32_UINT;
		default: return DXGI_FORMAT_UNKNOWN;
		}
	case D3D_REGISTER_COMPONENT_SINT32:
		DEBUG_ERROR("need to add new format!");
		break;
	case D3D_REGISTER_COMPONENT_FLOAT32:
	{
		switch (mask & 0xf)
		{
		case 15: return DXGI_FORMAT_R32G32B32A32_FLOAT;
		case 7: return DXGI_FORMAT_R32G32B32_FLOAT;
		case 3: return DXGI_FORMAT_R32G32_FLOAT;
		case 1: return DXGI_FORMAT_R32_FLOAT;
		default: return DXGI_FORMAT_UNKNOWN;
		}
	}
	default:
		return DXGI_FORMAT_UNKNOWN;
	}
}

Variable::Variable(ID3D11Device1* pDevice, ID3DX11EffectVariable* pD3DX11EffectVariable)
	: m_pD3DX11EffectVariable(pD3DX11EffectVariable),
	m_D3DX11EffectVariableDesc(),
	m_pD3DX11EffectType(nullptr),
	m_D3DX11EffectTypeDesc(),
	m_VariableName()
{
	m_pD3DX11EffectVariable->GetDesc(&m_D3DX11EffectVariableDesc);
	m_VariableName = m_D3DX11EffectVariableDesc.Name;
	m_pD3DX11EffectType = m_pD3DX11EffectVariable->GetType();
	m_pD3DX11EffectType->GetDesc(&m_D3DX11EffectTypeDesc);
}

ID3DX11EffectVariable* Variable::GetD3DX11EffectVariable() const
{
	return m_pD3DX11EffectVariable;
}

const D3DX11_EFFECT_VARIABLE_DESC& Variable::GetD3DX11EffectVariableDesc() const
{
	return m_D3DX11EffectVariableDesc;
}

ID3DX11EffectType* Variable::GetD3DX11EffectType() const
{
	return m_pD3DX11EffectType;
}

const D3DX11_EFFECT_TYPE_DESC& Variable::GetD3DX11EffectTypeDesc() const
{
	return m_D3DX11EffectTypeDesc;
}

const std::string& Variable::GetVariableName() const
{
	return m_VariableName;
}

Variable& Variable::operator<<(CXMMATRIX value)
{
	ID3DX11EffectMatrixVariable* variable = m_pD3DX11EffectVariable->AsMatrix();
	if (!variable->IsValid())
	{
		DEBUG_ERROR("Invalid effect variable cast.");
	}

	variable->SetMatrix(reinterpret_cast<const float*>(&value));

	return *this;
}

Variable& Variable::operator<<(ID3D11ShaderResourceView* value)
{
	ID3DX11EffectShaderResourceVariable* variable = m_pD3DX11EffectVariable->AsShaderResource();
	if (!variable->IsValid())
	{
		DEBUG_ERROR("Invalid effect variable cast.");
	}

	variable->SetResource(value);

	return *this;
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
