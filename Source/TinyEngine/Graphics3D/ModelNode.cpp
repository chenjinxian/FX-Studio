#include "ModelNode.h"
#include "CameraNode.h"
#include "Scene.h"
#include "Model.h"
#include "Material.h"
#include "../Actors/Actor.h"
#include "../Actors/RenderComponent.h"
#include "../Actors/TransformComponent.h"
#include "../ResourceCache/ResCache.h"
#include "../ResourceCache/TextureResource.h"
#include "../ResourceCache/MaterialResource.h"
#include "../ResourceCache/XmlResource.h"
#include "../AppFramework/BaseGameApp.h"
#include "boost/lexical_cast.hpp"
#include <DirectXColors.h>

ModelNode::ModelNode(
	ActorId actorId, WeakBaseRenderComponentPtr renderComponent, RenderPass renderPass, const Matrix& worldMatrix)
	: SceneNode(actorId, renderComponent, renderPass, worldMatrix),
	m_pModel(nullptr)
{
	ModelRenderComponent* pMeshRender = static_cast<ModelRenderComponent*>(m_pRenderComponent);
	if (pMeshRender != nullptr)
	{
		m_ModelName = pMeshRender->GetModelName();
	}

	Resource modelRes(m_ModelName);
	shared_ptr<ResHandle> pModelResHandle = g_pApp->GetResCache()->GetHandle(&modelRes);
	m_pModel = unique_ptr<Model>(DEBUG_NEW Model(pModelResHandle->Buffer(), pModelResHandle->Size()));
	SetBoundingBox(m_pModel->GetBoundingBox());
}

ModelNode::~ModelNode()
{
	VOnDeleteSceneNode(nullptr);
}

HRESULT ModelNode::VOnInitSceneNode(Scene *pScene)
{
	VOnDeleteSceneNode(pScene);

	ModelRenderComponent* pMeshRender = static_cast<ModelRenderComponent*>(m_pRenderComponent);
	if (pMeshRender != nullptr)
	{
		m_MaterialNames = pMeshRender->GetMaterialName();
	}

	uint32_t meshSize = m_pModel->GetMeshes().size();
	m_pEffects.resize(meshSize);
	m_pPasses.resize(meshSize);
	m_pVertexBuffers.resize(meshSize);
	m_pIndexBuffers.resize(meshSize);
	m_IndexCounts.resize(meshSize);
	DEBUG_ASSERT(m_MaterialNames.size() == meshSize);

	for (uint32_t i = 0; i < meshSize; i++)
	{
		const tinyxml2::XMLElement* rootNode = nullptr;

		Resource materialRes(m_MaterialNames[i]);
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
			DEBUG_ERROR("Material is not exist or valid: " + m_MaterialNames[i]);
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
				m_pEffects[i] = extra->GetEffect();
			}
		}

		if (m_pEffects[i] == nullptr)
		{
			DEBUG_ERROR("effect is not exist or valid: " + effectName);
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
				Technique* pCurrentTechnique = m_pEffects[i]->GetTechniquesByName().at(techniqueName);
				if (pCurrentTechnique == nullptr)
				{
					DEBUG_ERROR("technique is not exist: " + techniqueName);
				}

				m_pPasses[i] = pCurrentTechnique->GetPasses().at(0);
				if (m_pPasses[i] == nullptr)
				{
					DEBUG_ERROR("technique is not exist: " + techniqueName);
				}

				auto mesh = m_pModel->GetMeshes().at(i);
				ID3D11Buffer* pVertexBuffer = nullptr;
				ID3D11Buffer* pIndexBuffer = nullptr;
				m_pPasses[i]->CreateVertexBuffer(mesh, &pVertexBuffer);
				m_pPasses[i]->CreateIndexBuffer(mesh, &pIndexBuffer);

				m_pVertexBuffers[i] = pVertexBuffer;
				m_pIndexBuffers[i] = pIndexBuffer;
				m_IndexCounts[i] = mesh->GetIndices().size();

				break;
			}
		}
	}

	return S_FALSE;
}

HRESULT ModelNode::VOnDeleteSceneNode(Scene* pScene)
{
	for (auto pVertexBuffer : m_pVertexBuffers)
	{
		SAFE_RELEASE(pVertexBuffer);
	}
	m_pVertexBuffers.clear();

	for (auto pIndexBuffer : m_pIndexBuffers)
	{
		SAFE_RELEASE(pIndexBuffer);
	}
	m_pIndexBuffers.clear();
	
	return S_OK;
}

HRESULT ModelNode::VOnUpdate(Scene* pScene, const GameTime& gameTime)
{
	return S_OK;
}

HRESULT ModelNode::VRender(Scene* pScene, const GameTime& gameTime)
{
	for (uint32_t i = 0, count = m_pModel->GetMeshes().size(); i < count; i++)
	{
		const tinyxml2::XMLElement* rootNode = nullptr;

		Resource materialRes(m_MaterialNames[i]);
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

		const std::map<std::string, Variable*>& variables = m_pEffects[i]->GetVariablesByName();
		for (const tinyxml2::XMLElement* pNode = pVariables->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
		{
			Variable* variable = variables.at(pNode->Name());
			std::string semantic = variable->GetVariableSemantic();
			std::string type = variable->GetVariableType();

			if (semantic == "worldviewprojection")
			{
				if (type == "float4x4")
				{
					const Matrix& wvp = pScene->GetCamera()->GetWorldViewProjection(pScene);
					variable->SetMatrix(wvp);
				}
			}
			else if (semantic == "worldinversetranspose")
			{
				if (type == "float4x4")
				{
					Matrix& worldIT = pScene->GetTopMatrix().Invert().Transpose();
					variable->SetMatrix(worldIT);
				}
			}
			else if (semantic == "world")
			{
				if (type == "float4x4")
				{
					const Matrix& world = pScene->GetTopMatrix();
					variable->SetMatrix(world);
				}
			}
			else if (semantic == "viewinverse")
			{
				if (type == "float4x4")
				{
					Matrix& viewI = pScene->GetCamera()->GetViewMatrix().Invert();
					variable->SetMatrix(viewI);
				}
			}
			else if (semantic == "time")
			{
				variable->SetFloat(gameTime.GetElapsedTime());
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

		if (m_pPasses[i]->HasHullShader())
		{
			// D3D_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST start from 33 (32+1)
			pScene->GetRenderder()->VInputSetup(
				(D3D_PRIMITIVE_TOPOLOGY)(m_pPasses[i]->GetTessellationPrimitive() + 32), m_pPasses[i]->GetInputLayout());
		}
		else
		{
			Mesh* mesh = m_pModel->GetMeshes().at(i);
			switch (mesh->GetPrimitiveType())
			{
			case Mesh::PT_Point:
				pScene->GetRenderder()->VInputSetup(D3D_PRIMITIVE_TOPOLOGY_POINTLIST, m_pPasses[i]->GetInputLayout());
				break;
			case Mesh::PT_Line:
				pScene->GetRenderder()->VInputSetup(D3D_PRIMITIVE_TOPOLOGY_LINELIST, m_pPasses[i]->GetInputLayout());
				break;
			case Mesh::PT_Triangle:
				pScene->GetRenderder()->VInputSetup(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, m_pPasses[i]->GetInputLayout());
				break;
			default:
				DEBUG_ERROR("Unsupport primitive type!");
				return S_FALSE;
			}
		}

		uint32_t stride = m_pPasses[i]->GetVertexSize();
		uint32_t offset = 0;
		pScene->GetRenderder()->VSetVertexBuffers(m_pVertexBuffers[i], &stride, &offset);
		pScene->GetRenderder()->VSetIndexBuffer(m_pIndexBuffers[i], IRenderer::Format_uint32, 0);
		pScene->GetRenderder()->VDrawMesh(m_IndexCounts[i], 0, 0, m_pPasses[i]->GetEffectPass());

		pScene->GetRenderder()->VResetShader(
			m_pPasses[i]->HasGeometryShader(), m_pPasses[i]->HasHullShader(), m_pPasses[i]->HasDomainShader());
	}

	return S_OK;
}

void ModelNode::VPick(Scene* pScene, int cursorX, int cursorY)
{
	const Matrix& projectMat = pScene->GetCamera()->GetProjectMatrix();
	float viewX = (2.0f * cursorX / g_pApp->GetGameConfig().m_ScreenWidth - 1.0f) / projectMat.m[0][0];
	float viewY = (1.0f - 2.0f * cursorY / g_pApp->GetGameConfig().m_ScreenHeight) / projectMat.m[1][1];

	Matrix toLocal = (m_Properties.GetWorldMatrix() * pScene->GetCamera()->GetViewMatrix()).Invert();
	Vector3 rayPos = toLocal.Translation();
	//use right-hand coordinates, z should be -1
	Vector3 rayDir = Vector3::TransformNormal(Vector3(viewX, viewY, -1.0f), toLocal);
	rayDir.Normalize();

	Ray ray(rayPos, rayDir);

	float distance = 0.0f;
	if (ray.Intersects(m_Properties.GetBoundingBox(), distance))
	{
		if (distance < pScene->GetPickDistance())
		{
			pScene->SetPickDistance(distance);
			int index = 0;
			for (auto mesh : m_pModel->GetMeshes())
			{
				switch (mesh->GetPrimitiveType())
				{
				case Mesh::PT_Point:
				case Mesh::PT_Line:
				{
					distance = 0.0f;
					if (ray.Intersects(mesh->GetBoundingBox(), distance))
					{
						pScene->SetPickedActor(m_Properties.GetActorId(), index);
					}
					break;
				}
				case Mesh::PT_Triangle:
				{
					uint32_t len = mesh->GetIndices().size();
					len = len - len % 3;
					for (uint32_t i = 0; i < len; i += 3)
					{
						const std::vector<Vector3>& vertices = mesh->GetVertices();
						const std::vector<uint32_t>& indices = mesh->GetIndices();
						Vector3 tri0 = vertices.at(indices[i]);
						Vector3 tri1 = vertices.at(indices[i + 1]);
						Vector3 tri2 = vertices.at(indices[i + 2]);

						distance = 0.0f;
						if (ray.Intersects(tri0, tri1, tri2, distance))
						{
							pScene->SetPickedActor(m_Properties.GetActorId(), index);
							return;
						}
					}
					break;
				}
				default:
					break;
				}

				index++;
			}
		}
	}
}
