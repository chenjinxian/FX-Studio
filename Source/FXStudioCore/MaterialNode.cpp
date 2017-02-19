#include "stdafx.h"
#include "MaterialNode.h"
#include "boost/lexical_cast.hpp"

MaterialNode::MaterialNode(const std::string& materialName)
	: m_pEffect(nullptr),
	m_pCurrentPass(nullptr),
	m_pVertexBuffer(nullptr),
	m_pIndexBuffer(nullptr),
	m_Mesh(nullptr),
	m_MaterialName(materialName)
{		
	std::vector<VertexPositionNormalTexture> vertices;
	std::vector<uint16_t> indices;
	GeometricPrimitive::CreateGeoSphere(vertices, indices);
	m_IndexCount = indices.size();
	m_Mesh = unique_ptr<Mesh>(DEBUG_NEW Mesh(vertices, indices));

	m_World = Matrix::Identity;
	m_View = (Matrix::CreateTranslation(0.0f, 0.0f, 2.0f) * Matrix::CreateRotationY(45.0f)).Invert();
	m_Project = Matrix::CreatePerspectiveFieldOfView(XM_PIDIV4, 1.0f, 0.1f, 1000.0f);
}

MaterialNode::~MaterialNode()
{
}

HRESULT MaterialNode::OnInitSceneNode()
{
	OnDeleteSceneNode();

	const tinyxml2::XMLElement* rootNode = nullptr;

	Resource materialRes(m_MaterialName);
	shared_ptr<ResHandle> pMaterialResHandle = g_pApp->GetResCache()->GetHandle(&materialRes);
	if (pMaterialResHandle != nullptr)
	{
		shared_ptr<XmlResourceExtraData> extra = static_pointer_cast<XmlResourceExtraData>(pMaterialResHandle->GetExtraData());
		if (extra != nullptr)
		{
			rootNode = extra->GetRoot();
		}
	}

	if (nullptr == rootNode)
	{
		DEBUG_ERROR("Material is not exist or valid: " + m_MaterialName);
		return S_FALSE;
	}

	std::string effectName = rootNode->Attribute("object");
	Resource effectRes(effectName);
	shared_ptr<ResHandle> pEffectResHandle = g_pApp->GetResCache()->GetHandle(&effectRes);
	if (pEffectResHandle != nullptr)
	{
		shared_ptr<HlslResourceExtraData> extra = static_pointer_cast<HlslResourceExtraData>(pEffectResHandle->GetExtraData());
		if (extra != nullptr)
		{
			m_pEffect = extra->GetEffect();
		}
	}

	if (m_pEffect == nullptr)
	{
		DEBUG_ERROR("effect is not exist or valid: " + effectName);
		return S_FALSE;
	}

	const tinyxml2::XMLElement* pTechniques = rootNode->FirstChildElement("Techniques");
	if (pTechniques == nullptr)
	{
		DEBUG_ERROR("effect xml is invalid");
	}

	for (const tinyxml2::XMLElement* pNode = pTechniques->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
	{
		if (pNode->BoolAttribute("checked"))
		{
			std::string techniqueName = pNode->Attribute("name");
			Technique* pCurrentTechnique = m_pEffect->GetTechniquesByName().at(techniqueName);
			if (pCurrentTechnique == nullptr)
			{
				DEBUG_ERROR("technique is not exist: " + techniqueName);
			}

			m_pCurrentPass = pCurrentTechnique->GetPasses().at(0);
			if (m_pCurrentPass == nullptr)
			{
				DEBUG_ERROR("technique is not exist: " + techniqueName);
			}

			m_pCurrentPass->CreateVertexBuffer(m_Mesh.get(), &m_pVertexBuffer);
			m_pCurrentPass->CreateIndexBuffer(m_Mesh.get(), &m_pIndexBuffer);
			return S_OK;
		}
	}

	return S_FALSE;
}

HRESULT MaterialNode::OnDeleteSceneNode()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);

	return S_OK;
}

HRESULT MaterialNode::OnUpdate(const GameTime& gameTime)
{
	return S_OK;
}

HRESULT MaterialNode::Render(const GameTime& gameTime)
{
	const tinyxml2::XMLElement* rootNode = nullptr;

	Resource materialRes(m_MaterialName);
	shared_ptr<ResHandle> pMaterialResHandle = g_pApp->GetResCache()->GetHandle(&materialRes);
	if (pMaterialResHandle != nullptr)
	{
		shared_ptr<XmlResourceExtraData> extra = static_pointer_cast<XmlResourceExtraData>(pMaterialResHandle->GetExtraData());
		if (extra != nullptr)
		{
			rootNode = extra->GetRoot();
		}
	}
	if (nullptr == rootNode)
		return S_FALSE;

	const tinyxml2::XMLElement* pVariables = rootNode->FirstChildElement("Variables");
	DEBUG_ASSERT(pVariables != nullptr);

	const std::map<std::string, Variable*>& variables = m_pEffect->GetVariablesByName();
	for (const tinyxml2::XMLElement* pNode = pVariables->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
	{
		Variable* variable = variables.at(pNode->Name());
		std::string semantic = variable->GetVariableSemantic();
		std::string type = variable->GetVariableType();

		if (semantic == "worldviewprojection")
		{
			if (type == "float4x4")
			{
				const Matrix& wvp = m_World * m_View * m_Project;
				variable->SetMatrix(wvp);
			}
		}
		else if (semantic == "worldinversetranspose")
		{
			if (type == "float4x4")
			{
				Matrix& worldIT = m_World.Invert().Transpose();
				variable->SetMatrix(worldIT);
			}
		}
		else if (semantic == "world")
		{
			if (type == "float4x4")
			{
				const Matrix& world = m_World;
				variable->SetMatrix(world);
			}
		}
		else if (semantic == "viewinverse")
		{
			if (type == "float4x4")
			{
				Matrix& viewI = m_View.Invert();
				variable->SetMatrix(viewI);
			}
		}
		else if (semantic == "time")
		{
			variable->SetFloat(gameTime.GetTotalTime());
		}
		else if (type == "float4")
		{
			std::stringstream ss(pNode->GetText());
			Vector4 value;
			ss >> value.x >> value.y >> value.z >> value.w;
			variable->SetVector(value);
		}
		else if (type == "float3")
		{
			std::stringstream ss(pNode->GetText());
			Vector3 value;
			ss >> value.x >> value.y >> value.z;
			variable->SetVector(value);
		}
		else if (type == "float2")
		{
			std::stringstream ss(pNode->GetText());
			Vector2 value;
			ss >> value.x >> value.y;
			variable->SetVector(value);
		}
		else if (type == "float")
		{
			switch (variable->GetElementsCount())
			{
			case 0:
			{
				variable->SetFloat(boost::lexical_cast<float>(pNode->GetText()));
				break;
			}
			case 2:
			{
				std::stringstream ss(pNode->GetText());
				std::vector<float> value(2);
				ss >> value[0] >> value[1];
				variable->SetFloatArray(value);
				break;
			}
			case 3:
			{
				std::stringstream ss(pNode->GetText());
				std::vector<float> value(3);
				ss >> value[0] >> value[1] >> value[2];
				variable->SetFloatArray(value);
				break;
			}
			case 4:
			{
				std::stringstream ss(pNode->GetText());
				std::vector<float> value(4);
				ss >> value[0] >> value[1] >> value[2] >> value[3];
				variable->SetFloatArray(value);
				break;
			}
			default:
				break;
			}
		}
		else if (type == "int")
		{
			variable->SetInt(boost::lexical_cast<int>(pNode->GetText()));
		}
		else if (type == "Texture1D" || type == "Texture2D" || type == "Texture3D" || type == "TextureCube")
		{
			const char* resourName = pNode->Attribute("resourcename");
			if (resourName != nullptr)
			{
				std::string textureName = std::string("Textures\\") + resourName;
				Resource resource(textureName);
				shared_ptr<ResHandle> pTextureRes = g_pApp->GetResCache()->GetHandle(&resource);
				if (pTextureRes != nullptr)
				{
					shared_ptr<D3D11TextureResourceExtraData> extra =
						static_pointer_cast<D3D11TextureResourceExtraData>(pTextureRes->GetExtraData());
					if (extra != nullptr)
					{
						variable->SetResource(extra->GetTexture());
					}
				}
			}
		}
	}

	if (m_pCurrentPass->HasHullShader())
	{
		// D3D_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST start from 33 (32+1)
		g_pApp->GetRendererAPI()->VInputSetup(
			(D3D_PRIMITIVE_TOPOLOGY)(m_pCurrentPass->GetTessellationPrimitive() + 32), m_pCurrentPass->GetInputLayout());
	}
	else
	{
		g_pApp->GetRendererAPI()->VInputSetup(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, m_pCurrentPass->GetInputLayout());
	}
	uint32_t stride = m_pCurrentPass->GetVertexSize();
	uint32_t offset = 0;
	g_pApp->GetRendererAPI()->VSetVertexBuffers(m_pVertexBuffer, &stride, &offset);
	g_pApp->GetRendererAPI()->VSetIndexBuffer(m_pIndexBuffer, IRenderer::Format_uint32, 0);
	g_pApp->GetRendererAPI()->VDrawMesh(m_IndexCount, 0, 0, m_pCurrentPass->GetEffectPass());

	g_pApp->GetRendererAPI()->VResetShader(
		m_pCurrentPass->HasGeometryShader(), m_pCurrentPass->HasHullShader(), m_pCurrentPass->HasDomainShader());
	return S_OK;
}
