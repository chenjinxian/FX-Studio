#include "Material.h"
#include "ModelImporter.h"

Material::Material()
	: m_pEffect(nullptr),
	m_pCurrentTechnique(nullptr),
	m_DefaultTechniqueName(),
	m_InputLayouts()
{

}

Material::Material(const std::string& defaultTechniqueName)
	: m_pEffect(nullptr),
	m_pCurrentTechnique(nullptr),
	m_DefaultTechniqueName(defaultTechniqueName),
	m_InputLayouts()
{

}

Material::~Material()
{
	for (auto& inputLayout : m_InputLayouts)
	{
		SAFE_RELEASE(inputLayout.second);
	}
}

Variable* Material::operator[](const std::string& variableName)
{
	const std::map<std::string, Variable*>& variables = m_pEffect->GetVariablesByName();
	Variable* foundVariable = nullptr;

	std::map<std::string, Variable*>::const_iterator found = variables.find(variableName);
	if (found != variables.end())
	{
		foundVariable = found->second;
	}

	return foundVariable;
}

Effect* Material::GetEffect() const
{
	return m_pEffect;
}

Technique* Material::GetCurrentTechnique() const
{
	return m_pCurrentTechnique;
}

void Material::SetCurrentTechnique(Technique* pTechnique)
{
	m_pCurrentTechnique = pTechnique;
}

const std::map<Pass*, ID3D11InputLayout*>& Material::GetInputLayouts() const
{
	return m_InputLayouts;
}

void Material::Initialize(Effect* pEffect)
{
	m_pEffect = pEffect;
	DEBUG_ASSERT(m_pEffect != nullptr);

	Technique* defaultTechnique = nullptr;
	DEBUG_ASSERT(m_pEffect->GetTechniques().size() > 0);
	if (!m_DefaultTechniqueName.empty())
	{
		defaultTechnique = m_pEffect->GetTechniquesByName().at(m_DefaultTechniqueName);
		DEBUG_ASSERT(defaultTechnique != nullptr);
	}
	else
	{
		defaultTechnique = m_pEffect->GetTechniques().at(0);
	}

	SetCurrentTechnique(defaultTechnique);
}

void Material::CreateVertexBuffer(const Model*pModel, std::vector<ID3D11Buffer*>& vertexBuffers) const
{
	if (pModel != nullptr)
	{
		auto meshhes = pModel->GetMeshes();
		vertexBuffers.reserve(meshhes.size());
		for (auto mesh : meshhes)
		{
			ID3D11Buffer* vertexBuffer;
			CreateVertexBuffer(mesh, &vertexBuffer);
			vertexBuffers.push_back(vertexBuffer);
		}
	}
}

void Material::CreateInputLayout(
	const std::string& techniqueName, const std::string& passName,
	D3D11_INPUT_ELEMENT_DESC* inputElementDescriptions, uint32_t inputElementDescriptionCount)
{
	Technique* technique = m_pEffect->GetTechniquesByName().at(techniqueName);
	assert(technique != nullptr);

	Pass* pass = technique->GetPassesByName().at(passName);
	assert(pass != nullptr);

	ID3D11InputLayout* inputLayout;
	pass->CreateInputLayout(inputElementDescriptions, inputElementDescriptionCount, &inputLayout);

	m_InputLayouts.insert(std::make_pair(pass, inputLayout));
}

void Material::CreateInputLayout(
	Pass* pPass, D3D11_INPUT_ELEMENT_DESC* inputElementDescriptions, uint32_t inputElementDescriptionCount)
{
	ID3D11InputLayout* inputLayout;
	pPass->CreateInputLayout(inputElementDescriptions, inputElementDescriptionCount, &inputLayout);

	m_InputLayouts.insert(std::make_pair(pPass, inputLayout));
}

Effect::Effect()
	: m_pD3DX11Effect(nullptr),
	m_D3DX11EffectDesc(),
	m_Techniques(),
	m_TechniquesByName(),
	m_Variables(),
	m_VariablesByName()
{

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

	SAFE_RELEASE(m_pD3DX11Effect);
}

void Effect::CompileEffectFromFile(ID3DX11Effect** ppEffect, const std::wstring& filename)
{
	uint32_t shaderFlags = 0;

#if defined( DEBUG ) || defined( _DEBUG )
	shaderFlags |= D3DCOMPILE_DEBUG;
	shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3D10Blob* compiledShader = nullptr;
	ID3D10Blob* errorMessages = nullptr;
	HRESULT hr = D3DCompileFromFile(filename.c_str(), nullptr, nullptr, nullptr, "fx_5_0", shaderFlags, 0, &compiledShader, &errorMessages);
	if (FAILED(hr))
	{
		char* errorMessage = (errorMessages != nullptr ? (char*)errorMessages->GetBufferPointer() : "D3DX11CompileFromFile() failed");
		DEBUG_ERROR(errorMessage);
		SAFE_RELEASE(errorMessages);
	}

	hr = D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), NULL, DXUTGetD3D11Device(), ppEffect);
	if (FAILED(hr))
	{
		DEBUG_ERROR("D3DX11CreateEffectFromMemory() failed.");
	}

	SAFE_RELEASE(compiledShader);
}

void Effect::LoadCompiledEffect(ID3DX11Effect** ppEffect, const std::wstring& filename)
{
	std::vector<char> compiledShader;
	Utility::LoadBinaryFile(filename, compiledShader);

	HRESULT hr = D3DX11CreateEffectFromMemory(&compiledShader.front(), compiledShader.size(), NULL, DXUTGetD3D11Device(), ppEffect);
	if (FAILED(hr))
	{
		DEBUG_ERROR("D3DX11CreateEffectFromMemory() failed.");
	}
}

void Effect::CompileEffectFromFile(ID3DX11Effect** ppEffect, const void* pBuffer, uint32_t lenght)
{
	uint32_t shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	shaderFlags |= D3DCOMPILE_DEBUG;
	shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3D10Blob* errorMessages = nullptr;
	HRESULT hr = D3DX11CompileEffectFromMemory(
		pBuffer, lenght, nullptr, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		shaderFlags, 0, DXUTGetD3D11Device(), ppEffect, &errorMessages);

	if (FAILED(hr))
	{
		char* errorMessage = (errorMessages != nullptr ? (char*)errorMessages->GetBufferPointer() : "D3DX11CompileEffectFromMemory() failed");
		DEBUG_ERROR(errorMessage);
		SAFE_RELEASE(errorMessages);
	}
}

void Effect::LoadCompiledEffect(ID3DX11Effect** ppEffect, const void* pBuffer, uint32_t lenght)
{
	HRESULT hr = D3DX11CreateEffectFromMemory(pBuffer, lenght, 0, DXUTGetD3D11Device(), ppEffect);
	if (FAILED(hr))
	{
		DEBUG_ERROR("D3DX11CreateEffectFromMemory() failed.");
	}
}


ID3DX11Effect* Effect::GetD3DX11Effect() const
{
	return m_pD3DX11Effect;
}

void Effect::SetD3DX11Effect(ID3DX11Effect* pD3DX11Effect)
{
	if (m_pD3DX11Effect != nullptr)
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

	m_pD3DX11Effect = pD3DX11Effect;
	Initialize();
}

const D3DX11_EFFECT_DESC& Effect::GetD3DX11EffectDesc() const
{
	return m_D3DX11EffectDesc;
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

void Effect::CompileFromFile(const std::wstring& filename)
{
	CompileEffectFromFile(&m_pD3DX11Effect, filename);
	Initialize();
}

void Effect::LoadCompiledEffect(const std::wstring& filename)
{
	LoadCompiledEffect(&m_pD3DX11Effect, filename);
	Initialize();
}

void Effect::CompileFromMemory(const void* pBuffer, uint32_t lenght)
{
	CompileEffectFromFile(&m_pD3DX11Effect, pBuffer, lenght);
	Initialize();
}

void Effect::LoadCompiledEffect(const void* pBuffer, uint32_t lenght)
{
	LoadCompiledEffect(&m_pD3DX11Effect, pBuffer, lenght);
	Initialize();
}

void Effect::Initialize()
{
	HRESULT hr = m_pD3DX11Effect->GetDesc(&m_D3DX11EffectDesc);
	if (FAILED(hr))
	{
		DEBUG_ERROR("ID3DX11Effect::GetDesc() failed.");
	}

	for (uint32_t i = 0; i < m_D3DX11EffectDesc.Techniques; i++)
	{
		Technique* technique = new Technique(this, m_pD3DX11Effect->GetTechniqueByIndex(i));
		m_Techniques.push_back(technique);
		m_TechniquesByName.insert(std::make_pair(technique->GetTechniqueName(), technique));
	}

	for (uint32_t i = 0; i < m_D3DX11EffectDesc.GlobalVariables; i++)
	{
		Variable* variable = new Variable(this, m_pD3DX11Effect->GetVariableByIndex(i));
		m_Variables.push_back(variable);
		m_VariablesByName.insert(std::make_pair(variable->GetVariableName(), variable));
	}
}

Technique::Technique(Effect* pEffect, ID3DX11EffectTechnique* pD3DX11EffectTechnique)
	: m_pEffect(pEffect),
	m_pD3DX11EffectTechnique(pD3DX11EffectTechnique),
	m_D3DX11TechniqueDesc(),
	m_TechniqueName(),
	m_Passes(),
	m_PassesByName()
{
	m_pD3DX11EffectTechnique->GetDesc(&m_D3DX11TechniqueDesc);
	m_TechniqueName = m_D3DX11TechniqueDesc.Name;
	for (uint32_t i = 0; i < m_D3DX11TechniqueDesc.Passes; i++)
	{
		Pass* pass = GCC_NEW Pass(this, m_pD3DX11EffectTechnique->GetPassByIndex(i));
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

Effect* Technique::GetEffect()
{
	return m_pEffect;
}

ID3DX11EffectTechnique* Technique::GetD3DX11EffectTechnique() const
{
	return m_pD3DX11EffectTechnique;
}

const D3DX11_TECHNIQUE_DESC& Technique::GetD3DX11TechniqueDesc() const
{
	return m_D3DX11TechniqueDesc;
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

Pass::Pass(Technique* pTechnique, ID3DX11EffectPass* pD3DX11EffectPass)
	: m_pTechnique(pTechnique),
	m_pD3DX11EffectPass(pD3DX11EffectPass),
	m_D3DX11PassDesc(),
	m_PassName()
{
	m_pD3DX11EffectPass->GetDesc(&m_D3DX11PassDesc);
	m_PassName = m_D3DX11PassDesc.Name;
}

Technique* Pass::GetTechnique()
{
	return m_pTechnique;
}

ID3DX11EffectPass* Pass::GetD3DX11EffectPass() const
{
	return m_pD3DX11EffectPass;
}

const D3DX11_PASS_DESC& Pass::GetD3DX11PassDesc() const
{
	return m_D3DX11PassDesc;
}

const std::string& Pass::GetPassName() const
{
	return m_PassName;
}

HRESULT Pass::CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* inputElementDesc, uint32_t numElements, ID3D11InputLayout **inputLayout)
{
	HRESULT hr;
	V_RETURN(DXUTGetD3D11Device()->CreateInputLayout(
		inputElementDesc, numElements, m_D3DX11PassDesc.pIAInputSignature, m_D3DX11PassDesc.IAInputSignatureSize, inputLayout));
	return S_OK;
}

void Pass::Apply(uint32_t flags)
{
	m_pD3DX11EffectPass->Apply(flags, DXUTGetD3D11DeviceContext());
}

Variable::Variable(Effect* pEffect, ID3DX11EffectVariable* pD3DX11EffectVariable)
	: m_pEffect(pEffect),
	m_pD3DX11EffectVariable(pD3DX11EffectVariable),
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

Effect* Variable::GetEffect()
{
	return m_pEffect;
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

	variable->SetFloatArray(&values[0], 0, values.size());

	return *this;
}

Variable& Variable::operator<<(const std::vector<XMFLOAT2>& values)
{
	ID3DX11EffectVectorVariable* variable = m_pD3DX11EffectVariable->AsVector();
	if (!variable->IsValid())
	{
		DEBUG_ERROR("Invalid effect variable cast.");
	}

	variable->SetFloatVectorArray(reinterpret_cast<const float*>(&values[0]), 0, values.size());

	return *this;
}

Variable& Variable::operator<<(const std::vector<XMFLOAT4X4>& values)
{
	ID3DX11EffectMatrixVariable* variable = m_pD3DX11EffectVariable->AsMatrix();
	if (!variable->IsValid())
	{
		DEBUG_ERROR("Invalid effect variable cast.");
	}

	variable->SetMatrixArray(reinterpret_cast<const float*>(&values[0]), 0, values.size());

	return *this;
}
