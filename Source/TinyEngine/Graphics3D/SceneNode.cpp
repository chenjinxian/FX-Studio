#include "SceneNode.h"
#include "CameraNode.h"
#include "Scene.h"
#include "ModelImporter.h"
#include "Material.h"
#include "TextureMappingMaterial.h"
#include "TextureResourceLoader.h"
#include "VertexDeclarations.h"
#include "../Actors/Actor.h"
#include "../Actors/RenderComponent.h"
#include "../Actors/TransformComponent.h"
#include "../ResourceCache/ResCache.h"
#include "../AppFramework/BaseGameApp.h"
#include "../AppFramework/BaseGameLogic.h"
#include <DirectXColors.h>

SceneNodeProperties::SceneNodeProperties()
	: m_ActorId(INVALID_ACTOR_ID),
	m_RenderPass(RenderPass_0),
	m_worldMatrix(Matrix::Identity)
{

}

SceneNode::SceneNode(ActorId actorId, WeakBaseRenderComponentPtr renderComponent, RenderPass renderPass, const Matrix& worldMatrix)
	: m_pParent(nullptr), m_pRenderComponent(renderComponent)
{
	m_Properties.m_ActorId = actorId;
	m_Properties.m_ActorName = (renderComponent != nullptr) ? renderComponent->VGetComponentName() : "SceneNode";
	m_Properties.m_RenderPass = renderPass;
	VSetTransform(worldMatrix);
	SetRadius(0);
}

SceneNode::~SceneNode()
{
}

void SceneNode::VSetTransform(const Matrix& worldMatrix)
{
	m_Properties.m_worldMatrix = worldMatrix;
}

HRESULT SceneNode::VOnRestore(Scene* pScene)
{
	Color color = (m_pRenderComponent) ? m_pRenderComponent->GetColor() : Color(Colors::White.f);

	for (auto& child : m_Children)
	{
		child->VOnRestore(pScene);
	}
	return S_OK;
}

HRESULT SceneNode::VOnDestoryDevice(Scene *pScene)
{
	for (auto& child : m_Children)
	{
		child->VOnDestoryDevice(pScene);
	}
	return S_OK;
}

HRESULT SceneNode::VOnUpdate(Scene* pScene, double fTime, float fElapsedTime)
{
	for (auto& child : m_Children)
	{
		child->VOnUpdate(pScene, fTime, fElapsedTime);
	}
	return S_OK;
}

HRESULT SceneNode::VPreRender(Scene* pScene)
{
// 	StrongActorPtr pActor = MakeStrongPtr(g_pApp->GetGameLogic()->VGetActor(m_Properties.GetActorId()));
// 	if (pActor != nullptr)
// 	{
// 		shared_ptr<TransformComponent> pTransform = MakeStrongPtr(pActor->GetComponent<TransformComponent>(TransformComponent::m_Name));
// 		if (pTransform)
// 		{
// 			m_Properties.m_worldMatrix = pTransform->GetTransform();
// 		}
// 	}
// 
// 	pScene->PushAndSetMatrix(m_Properties.m_worldMatrix);

	return S_OK;
}

HRESULT SceneNode::VRender(Scene* pScene, double fTime, float fElapsedTime)
{
	return S_OK;
}

HRESULT SceneNode::VRenderChildren(Scene* pScene, double fTime, float fElapsedTime)
{
	for (auto& child : m_Children)
	{
		if (child->VPreRender(pScene) == S_OK)
		{
			if (child->VIsVisible(pScene))
			{
				child->VRender(pScene, fTime, fElapsedTime);
				child->VRenderChildren(pScene, fTime, fElapsedTime);
			}
		}
		child->VPostRender(pScene);
	}

	return S_OK;
}

HRESULT SceneNode::VPostRender(Scene* pScene)
{
	pScene->PopMatrix();
	return S_OK;
}

bool SceneNode::VIsVisible(Scene* pScene) const
{
	return true;
}

bool SceneNode::VAddChild(shared_ptr<ISceneNode> child)
{
	m_Children.push_back(child);
	shared_ptr<SceneNode> kid = static_pointer_cast<SceneNode>(child);
	kid->m_pParent = this;
	
	return true;
}

bool SceneNode::VRemoveChild(ActorId actorId)
{
	for (SceneNodeList::iterator i = m_Children.begin(); i != m_Children.end(); ++i)
	{
		const SceneNodeProperties* pProps = (*i)->VGet();
		if (pProps->GetActorId() != INVALID_ACTOR_ID && actorId == pProps->GetActorId())
		{
			i = m_Children.erase(i);
			return true;
		}
	}
	return false;
}

RootNode::RootNode() : SceneNode(INVALID_ACTOR_ID, WeakBaseRenderComponentPtr(), RenderPass_0)
{
	m_Children.reserve(RenderPass_Last);

	shared_ptr<SceneNode> staticGroup(DEBUG_NEW SceneNode(INVALID_ACTOR_ID, WeakBaseRenderComponentPtr(), RenderPass_Static));
	m_Children.push_back(staticGroup);

	shared_ptr<SceneNode> actorGroup(DEBUG_NEW SceneNode(INVALID_ACTOR_ID, WeakBaseRenderComponentPtr(), RenderPass_Actor));
	m_Children.push_back(actorGroup);

	shared_ptr<SceneNode> skyGroup(DEBUG_NEW SceneNode(INVALID_ACTOR_ID, WeakBaseRenderComponentPtr(), RenderPass_Sky));
	m_Children.push_back(skyGroup);

	shared_ptr<SceneNode> invisibleGroup(DEBUG_NEW SceneNode(INVALID_ACTOR_ID, WeakBaseRenderComponentPtr(), RenderPass_NotRendered));
	m_Children.push_back(invisibleGroup);
}

RootNode::~RootNode()
{

}

HRESULT RootNode::VRenderChildren(Scene* pScene, double fTime, float fElapsedTime)
{
	for (int pass = RenderPass_0; pass < RenderPass_Last; ++pass)
	{
		switch (pass)
		{
		case RenderPass_Static:
		case RenderPass_Actor:
			m_Children[pass]->VRenderChildren(pScene, fTime, fElapsedTime);
			break;

		case RenderPass_Sky:
		{
// 			shared_ptr<IRenderState> skyPass = pScene->GetRenderder()->
			m_Children[pass]->VRenderChildren(pScene, fTime, fElapsedTime);
			break;
		}
		}
	}

	return S_OK;
}

bool RootNode::VAddChild(shared_ptr<ISceneNode> child)
{
	RenderPass pass = child->VGet()->GetRenderPass();
	if ((unsigned)pass >= m_Children.size() || !m_Children[pass])
	{
		DEBUG_ASSERT(0 && _T("There is no such render pass"));
		return false;
	}

	return m_Children[pass]->VAddChild(child);
}

bool RootNode::VRemoveChild(ActorId actorId)
{
	bool anythingRemoved = false;
	for (int i = RenderPass_0; i < RenderPass_Last; ++i)
	{
		if (m_Children[i]->VRemoveChild(actorId))
		{
			anythingRemoved = true;
		}
	}
	return anythingRemoved;
}

GridNode::GridNode(ActorId actorId, WeakBaseRenderComponentPtr renderComponent, const Matrix& worldMatrix)
	: SceneNode(actorId, renderComponent, RenderPass_0, worldMatrix),
	mEffect(nullptr),
	mTechnique(nullptr),
	mPass(nullptr), 
	mWvpVariable(nullptr),
	mInputLayout(nullptr),
	mVertexBuffer(nullptr),
	mPosition(0, 0, 0),
	mWorldMatrix(Matrix::Identity),
	mSize(16),
	mScale(16),
	mColor(0.961f, 0.871f, 0.702f, 1.0f)
{
// 	HRESULT hr;
// 	auto pd3dImmediateContext = DXUTGetD3D11DeviceContext();
// 	ID3D11Device* pd3dDevice = DXUTGetD3D11Device();
// 	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
// #ifdef _DEBUG
// 	dwShaderFlags |= D3DCOMPILE_DEBUG;
// 	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
// #endif
// 
// #if D3D_COMPILER_VERSION >= 46
// 
// 	WCHAR str[MAX_PATH];
// 	VTrace(DXUTFindDXSDKMediaFileCch(str, MAX_PATH, L"BasicEffect.fx"));
// 
// 	VTrace(D3DX11CompileEffectFromFile(str, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, dwShaderFlags, 0, pd3dDevice, &mEffect, nullptr));
// 
// #else
// 
// 	ID3DBlob* pEffectBuffer = nullptr;
// 	V_RETURN(DXUTCompileFromFile(L"BasicEffect.fx", nullptr, "none", "fx_5_0", dwShaderFlags, 0, &pEffectBuffer));
// 	hr = D3DX11CreateEffectFromMemory(pEffectBuffer->GetBufferPointer(), pEffectBuffer->GetBufferSize(), 0, pd3dDevice, &mEffect);
// 	SAFE_RELEASE(pEffectBuffer);
// 	if (FAILED(hr))
// 		return hr;
// 
// #endif
// 
// 	mTechnique = mEffect->GetTechniqueByName("main11");
// 	mPass = mTechnique->GetPassByName("p0");
// 	ID3DX11EffectVariable* variable = mEffect->GetVariableByName("WorldViewProjection");
// 	mWvpVariable = variable->AsMatrix();
// 
// 	D3DX11_PASS_DESC passDesc;
// 	mPass->GetDesc(&passDesc);
// 
// 	D3D11_INPUT_ELEMENT_DESC inputElementDescriptions[] =
// 	{
// 		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
// 		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
// 	};
// 	VTrace(pd3dDevice->CreateInputLayout(inputElementDescriptions, ARRAYSIZE(inputElementDescriptions),
// 		passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &mInputLayout));
}

GridNode::~GridNode()
{
	SAFE_RELEASE(mWvpVariable);
	SAFE_RELEASE(mPass);
	SAFE_RELEASE(mTechnique);
	SAFE_RELEASE(mEffect);
	SAFE_RELEASE(mInputLayout);
	SAFE_RELEASE(mVertexBuffer);
}

HRESULT GridNode::VOnDestoryDevice(Scene *pScene)
{
	return S_OK;
}

HRESULT GridNode::VOnRestore(Scene* pScene)
{
// 	HRESULT hr;
// 
// 	V_RETURN(SceneNode::VOnRestore(pScene));
// 
// 	SAFE_RELEASE(mVertexBuffer);
// 
// 	auto pd3dImmediateContext = DXUTGetD3D11DeviceContext();
// 	ID3D11Device* pd3dDevice = DXUTGetD3D11Device();
// 	int length = 4 * (mSize + 1);
// 	int size = sizeof(VertexPositionColor) * length;
// 	std::unique_ptr<VertexPositionColor> vertexData(new VertexPositionColor[length]);
// 	VertexPositionColor* vertices = vertexData.get();
// 
// 	float adjustedScale = mScale * 0.1f;
// 	float maxPosition = mSize * adjustedScale / 2;
// 
// 	for (uint32_t i = 0, j = 0; i < mSize + 1; i++, j = 4 * i)
// 	{
// 		float position = maxPosition - (i * adjustedScale);
// 
// 		// Vertical line
// 		vertices[j] = VertexPositionColor(XMFLOAT4(position, 0.0f, maxPosition, 1.0f), mColor);
// 		vertices[j + 1] = VertexPositionColor(XMFLOAT4(position, 0.0f, -maxPosition, 1.0f), mColor);
// 
// 		// Horizontal line
// 		vertices[j + 2] = VertexPositionColor(XMFLOAT4(maxPosition, 0.0f, position, 1.0f), mColor);
// 		vertices[j + 3] = VertexPositionColor(XMFLOAT4(-maxPosition, 0.0f, position, 1.0f), mColor);
// 	}
// 
// 	D3D11_BUFFER_DESC vertexBufferDesc;
// 	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
// 	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
// 	vertexBufferDesc.ByteWidth = size;
// 	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
// 
// 	D3D11_SUBRESOURCE_DATA vertexSubResourceData;
// 	ZeroMemory(&vertexSubResourceData, sizeof(vertexSubResourceData));
// 	vertexSubResourceData.pSysMem = vertices;
// 
// 	V_RETURN(pd3dDevice->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, &mVertexBuffer));
// 
// 	return hr;

	return S_OK;
}

HRESULT GridNode::VRender(Scene* pScene, double fTime, float fElapsedTime)
{
// 	auto pd3dImmediateContext = DXUTGetD3D11DeviceContext();
// 	pd3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
// 	pd3dImmediateContext->IASetInputLayout(mInputLayout);
// 
// 	UINT stride = sizeof(VertexPositionColor);
// 	UINT offset = 0;
// 	pd3dImmediateContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
// 
// 	XMMATRIX wvp = mWorldMatrix * pScene->GetCamera()->GetViewMatrix() * pScene->GetCamera()->GetProjectMatrix();
// 	mWvpVariable->SetMatrix(reinterpret_cast<const float*>(&wvp));
// 
// 	mPass->Apply(0, pd3dImmediateContext);
// 
// 	pd3dImmediateContext->Draw((mSize + 1) * 4, 0);

	return S_OK;
}

HRESULT GridNode::VOnUpdate(Scene* pScene, double fTime, float fElapsedTime)
{
	return S_OK;
}

ModelNode::ModelNode(
	ActorId actorId, WeakBaseRenderComponentPtr renderComponent, RenderPass renderPass, const Matrix& worldMatrix)
	: SceneNode(actorId, renderComponent, renderPass, worldMatrix),
	m_pEffect(nullptr),
	m_pTextureMappingMaterial(nullptr),
	m_WorldMatrix(worldMatrix)
{
	ModelRenderComponent* pMeshRender = static_cast<ModelRenderComponent*>(m_pRenderComponent);
	if (pMeshRender != nullptr)
	{
		m_ModelName = pMeshRender->GetModelName();
		m_TextureName = pMeshRender->GetTextureName();
		m_EffectName = pMeshRender->GetEffectName();
	}

	Resource modelRes(m_ModelName);
	shared_ptr<ResHandle> pModelResHandle = g_pApp->m_pResCache->GetHandle(&modelRes);
	std::unique_ptr<Model> model(new Model(pModelResHandle->Buffer(), pModelResHandle->Size(), true));

	Resource effectRes(m_EffectName);
	shared_ptr<ResHandle> pEffectResHandle = g_pApp->m_pResCache->GetHandle(&effectRes);
	m_pEffect = new Effect();
	m_pEffect->CompileFromMemory(pEffectResHandle->Buffer(), pEffectResHandle->Size());
	m_pTextureMappingMaterial = new TextureMappingMaterial();
	m_pTextureMappingMaterial->Initialize(m_pEffect);

	for (auto mesh : model->GetMeshes())
	{
		ID3D11Buffer* pVertexBuffer = nullptr;
		ID3D11Buffer* pIndexBuffer = nullptr;
		m_pTextureMappingMaterial->CreateVertexBuffer(mesh, &pVertexBuffer);
		mesh->CreateIndexBuffer(&pIndexBuffer);

		m_pVertexBuffers.push_back(pVertexBuffer);
		m_pIndexBuffers.push_back(pIndexBuffer);
		m_IndexCounts.push_back(mesh->GetIndices().size());
	}
}

ModelNode::~ModelNode()
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

	SAFE_DELETE(m_pEffect);
	SAFE_DELETE(m_pTextureMappingMaterial);
}

HRESULT ModelNode::VOnRestore(Scene *pScene)
{
	return S_OK;
}

HRESULT ModelNode::VOnDestoryDevice(Scene* pScene)
{
	return S_OK;
}

HRESULT ModelNode::VOnUpdate(Scene* pScene, double fTime, float fElapsedTime)
{
	return S_OK;
}

HRESULT ModelNode::VRender(Scene* pScene, double fTime, float fElapsedTime)
{
// 	ID3D11DeviceContext* direct3DDeviceContext = DXUTGetD3D11DeviceContext();
// 	direct3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
// 
// 	Pass* pass = m_pTextureMappingMaterial->GetCurrentTechnique()->GetPasses().at(0);
// 	ID3D11InputLayout* inputLayout = m_pTextureMappingMaterial->GetInputLayouts().at(pass);
// 	direct3DDeviceContext->IASetInputLayout(inputLayout);
// 
// 	XMMATRIX wvp = pScene->GetCamera()->GetWorldViewProjection(pScene);
// 	m_pTextureMappingMaterial->GetWorldViewProjection() << wvp;
// 
// 	UINT stride = m_pTextureMappingMaterial->VertexSize();
// 	UINT offset = 0;
// 
// 	for (uint32_t i = 0, count = m_IndexCounts.size(); i < count; i++)
// 	{
// 		direct3DDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffers[i], &stride, &offset);
// 		direct3DDeviceContext->IASetIndexBuffer(m_pIndexBuffers[i], DXGI_FORMAT_R32_UINT, 0);
// 
// 		Resource resource(m_TextureName);
// 		shared_ptr<ResHandle> pTextureRes = g_pApp->m_pResCache->GetHandle(&resource);
// 		if (pTextureRes != nullptr)
// 		{
// 			shared_ptr<D3DTextureResourceExtraData11> extra = static_pointer_cast<D3DTextureResourceExtraData11>(pTextureRes->GetExtra());
// 			if (extra != nullptr)
// 			{
// 				m_pTextureMappingMaterial->GetColorTexture() << extra->GetTexture();
// 			}
// 		}
// 
// 		pass->Apply(0);
// 
// 		direct3DDeviceContext->DrawIndexed(m_IndexCounts[i], 0, 0);
// 	}

	return S_OK;
}
