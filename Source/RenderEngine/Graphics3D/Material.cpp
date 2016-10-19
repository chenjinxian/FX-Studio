#include "Material.h"
#include "Model.h"

Material::Material()
{

}

Material::Material(const std::string& defaultTechniqueName)
{

}

Material::~Material()
{

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

void Material::SetCurrentTechnique(Technique* currentTechnique)
{

}

const std::map<Pass*, ID3D11InputLayout*>& Material::GetInputLayouts() const
{
	return m_InputLayouts;
}

void Material::Initialize(Effect* effect)
{

}

void Material::CreateVertexBuffer(ID3D11Device* device, const Model& model, std::vector<ID3D11Buffer*>& vertexBuffers) const
{

}

void Material::CreateInputLayout(const std::string& techniqueName, const std::string& passName, D3D11_INPUT_ELEMENT_DESC* inputElementDescriptions, UINT inputElementDescriptionCount)
{

}

void Material::CreateInputLayout(Pass& pass, D3D11_INPUT_ELEMENT_DESC* inputElementDescriptions, UINT inputElementDescriptionCount)
{

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

void Effect::CompileEffectFromFile(ID3D11Device* direct3DDevice, ID3DX11Effect** effect, const std::wstring& filename)
{

}

void Effect::LoadCompiledEffect(ID3D11Device* direct3DDevice, ID3DX11Effect** effect, const std::wstring& filename)
{

}

void Effect::LoadCompiledEffect(const std::wstring& filename)
{

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

}

void Effect::Initialize()
{

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

HRESULT Pass::CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* inputElementDesc, UINT numElements, ID3D11InputLayout **inputLayout)
{
	HRESULT hr;
	V_RETURN(DXUTGetD3D11Device()->CreateInputLayout(
		inputElementDesc, numElements, m_D3DX11PassDesc.pIAInputSignature, m_D3DX11PassDesc.IAInputSignatureSize, inputLayout));
	return S_OK;
}

void Pass::Apply(UINT flags, ID3D11DeviceContext* context)
{

}

Variable::Variable(Effect* pEffect, ID3DX11EffectVariable* pD3DX11EffectVariable)
{

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
	return *this;
}

Variable& Variable::operator<<(ID3D11ShaderResourceView* value)
{
	return *this;
}

Variable& Variable::operator<<(ID3D11UnorderedAccessView* value)
{
	return *this;
}

Variable& Variable::operator<<(FXMVECTOR value)
{
	return *this;
}

Variable& Variable::operator<<(int value)
{
	return *this;
}

Variable& Variable::operator<<(float value)
{
	return *this;
}

Variable& Variable::operator<<(const std::vector<float>& values)
{
	return *this;
}

Variable& Variable::operator<<(const std::vector<XMFLOAT2>& values)
{
	return *this;
}

Variable& Variable::operator<<(const std::vector<XMFLOAT4X4>& values)
{
	return *this;
}
