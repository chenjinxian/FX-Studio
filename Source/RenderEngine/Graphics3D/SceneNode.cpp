#include "SceneNode.h"
#include "Scene.h"
#include "../Actors/RenderComponent.h"

SceneNode::SceneNode(ActorId actorId, WeakBaseRenderComponentPtr renderComponent, RenderPass renderPass)
	: m_pParent(nullptr), m_pRenderComponent(renderComponent)
{
	m_Properties.m_ActorId = actorId;
	m_Properties.m_ActorName = (renderComponent != nullptr) ? renderComponent->VGetName() : "SceneNode";
}

SceneNode::~SceneNode()
{
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

HRESULT SceneNode::VOnLostDevice(Scene *pScene)
{
	for (auto& child : m_Children)
	{
		child->VOnLostDevice(pScene);
	}
	return S_OK;
}

HRESULT SceneNode::VOnUpdate(Scene* pScene, float fElapsedTime)
{
	for (auto& child : m_Children)
	{
		child->VOnUpdate(pScene, fElapsedTime);
	}
	return S_OK;
}

HRESULT SceneNode::VPreRender(Scene* pScene)
{
	return S_OK;
}

HRESULT SceneNode::VRender(Scene* pScene)
{
	return S_OK;
}

HRESULT SceneNode::VRenderChildren(Scene* pScene)
{
	for (auto& child : m_Children)
	{
		if (child->VPreRender(pScene) == S_OK)
		{
			if (child->VIsVisible(pScene))
			{
				child->VRender(pScene);
				child->VRenderChildren(pScene);
			}
		}
		child->VPostRender(pScene);
	}

	return S_OK;
}

HRESULT SceneNode::VPostRender(Scene* pScene)
{
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

	shared_ptr<SceneNode> staticGroup(GCC_NEW SceneNode(INVALID_ACTOR_ID, WeakBaseRenderComponentPtr(), RenderPass_Static));
	m_Children.push_back(staticGroup);

	shared_ptr<SceneNode> actorGroup(GCC_NEW SceneNode(INVALID_ACTOR_ID, WeakBaseRenderComponentPtr(), RenderPass_Actor));
	m_Children.push_back(actorGroup);

	shared_ptr<SceneNode> skyGroup(GCC_NEW SceneNode(INVALID_ACTOR_ID, WeakBaseRenderComponentPtr(), RenderPass_Sky));
	m_Children.push_back(skyGroup);

	shared_ptr<SceneNode> invisibleGroup(GCC_NEW SceneNode(INVALID_ACTOR_ID, WeakBaseRenderComponentPtr(), RenderPass_NotRendered));
	m_Children.push_back(invisibleGroup);
}

RootNode::~RootNode()
{

}

HRESULT RootNode::VRenderChildren(Scene* pScene)
{
	for (int pass = RenderPass_0; pass < RenderPass_Last; ++pass)
	{
		switch (pass)
		{
		case RenderPass_Static:
		case RenderPass_Actor:
			m_Children[pass]->VRenderChildren(pScene);
			break;

		case RenderPass_Sky:
		{
// 			shared_ptr<IRenderState> skyPass = pScene->GetRenderder()->
			m_Children[pass]->VRenderChildren(pScene);
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
		GCC_ASSERT(0 && _T("There is no such render pass"));
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

CameraNode::CameraNode() : SceneNode(INVALID_ACTOR_ID, WeakBaseRenderComponentPtr(), RenderPass_0)
{

}

CameraNode::~CameraNode()
{

}

HRESULT CameraNode::VOnUpdate(Scene* pScene, float fElapsedTime)
{
	m_ModelViewer.FrameMove(fElapsedTime);
	return S_OK;
}

HRESULT CameraNode::VRender(Scene* pScene)
{
	return S_OK;
}

HRESULT CameraNode::VOnRestore(Scene* pScene)
{
	static const XMVECTORF32 s_Eye = { 0.0f, 3.0f, -800.0f, 0.f };
	static const XMVECTORF32 s_At = { 0.0f, 1.0f, 0.0f, 0.f };
	m_ModelViewer.SetViewParams(s_Eye, s_At);

	const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc = DXUTGetDXGIBackBufferSurfaceDesc();
	float fAspectRatio = pBackBufferSurfaceDesc->Width / (FLOAT)pBackBufferSurfaceDesc->Height;
	m_ModelViewer.SetProjParams(XM_PI / 4, fAspectRatio, 0.1f, 5000.0f);
	m_ModelViewer.SetWindow(pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height);
	m_ModelViewer.SetButtonMasks(MOUSE_LEFT_BUTTON, MOUSE_WHEEL, MOUSE_MIDDLE_BUTTON);

	return S_OK;
}

GridNode::GridNode(ActorId actorId, WeakBaseRenderComponentPtr renderComponent)
	: SceneNode(actorId, renderComponent, RenderPass_0)
{
	g_fModelWaviness = 0.0f;
	g_bSpinning = true;
	g_pEffect = nullptr;
	g_pVertexLayout = nullptr;
	g_pTechnique = nullptr;
	g_ptxDiffuseVariable = nullptr;
	g_pWorldVariable = nullptr;
	g_pViewVariable = nullptr;
	g_pProjectionVariable = nullptr;
	g_pWavinessVariable = nullptr;
	g_pTimeVariable = nullptr;
}

GridNode::~GridNode()
{

}

HRESULT GridNode::VOnRestore(Scene* pScene)
{
	HRESULT hr;

	V_RETURN(SceneNode::VOnRestore(pScene));

	auto pd3dImmediateContext = DXUTGetD3D11DeviceContext();
	ID3D11Device* pd3dDevice = DXUTGetD3D11Device();
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	dwShaderFlags |= D3DCOMPILE_DEBUG;
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

#if D3D_COMPILER_VERSION >= 46

	WCHAR str[MAX_PATH];
	V_RETURN(DXUTFindDXSDKMediaFileCch(str, MAX_PATH, L"Tutorial11.fx"));

	V_RETURN(D3DX11CompileEffectFromFile(str, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, dwShaderFlags, 0, pd3dDevice, &g_pEffect, nullptr));

#else

	ID3DBlob* pEffectBuffer = nullptr;
	V_RETURN(DXUTCompileFromFile(L"Tutorial11.fx", nullptr, "none", "fx_5_0", dwShaderFlags, 0, &pEffectBuffer));
	hr = D3DX11CreateEffectFromMemory(pEffectBuffer->GetBufferPointer(), pEffectBuffer->GetBufferSize(), 0, pd3dDevice, &g_pEffect);
	SAFE_RELEASE(pEffectBuffer);
	if (FAILED(hr))
		return hr;

#endif

	g_pTechnique = g_pEffect->GetTechniqueByName("Render");

	g_ptxDiffuseVariable = g_pEffect->GetVariableByName("g_txDiffuse")->AsShaderResource();
	g_pWorldVariable = g_pEffect->GetVariableByName("World")->AsMatrix();
	g_pViewVariable = g_pEffect->GetVariableByName("View")->AsMatrix();
	g_pProjectionVariable = g_pEffect->GetVariableByName("Projection")->AsMatrix();
	g_pWavinessVariable = g_pEffect->GetVariableByName("Waviness")->AsScalar();
	g_pTimeVariable = g_pEffect->GetVariableByName("Time")->AsScalar();

	g_pWavinessVariable->SetFloat(g_fModelWaviness);

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	D3DX11_PASS_DESC PassDesc;
	V_RETURN(g_pTechnique->GetPassByIndex(0)->GetDesc(&PassDesc));
	V_RETURN(pd3dDevice->CreateInputLayout(layout, numElements, PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize, &g_pVertexLayout));

	pd3dImmediateContext->IASetInputLayout(g_pVertexLayout);

	V_RETURN(g_Mesh.Create(pd3dDevice, L"Tiny\\tiny.sdkmesh"));

	g_World = XMMatrixIdentity();

	return hr;
}

HRESULT GridNode::VRender(Scene* pScene)
{
	XMMATRIX mView = pScene->GetCamera()->GetViewMatrix();
	XMMATRIX mProj = pScene->GetCamera()->GetProjectMatrix();
	XMMATRIX mWorldViewProjection = g_World * mView * mProj;

	//
	// Update variables that change once per frame
	//
	g_pProjectionVariable->SetMatrix((float*)&mProj);
	g_pViewVariable->SetMatrix((float*)&mView);
	g_pWorldVariable->SetMatrix((float*)&g_World);
// 	g_pTimeVariable->SetFloat((float)fTime);

	auto pd3dImmediateContext = DXUTGetD3D11DeviceContext();
	pd3dImmediateContext->IASetInputLayout(g_pVertexLayout);

	//
	// Render the mesh
	//
	UINT Strides[1];
	UINT Offsets[1];
	ID3D11Buffer* pVB[1];
	pVB[0] = g_Mesh.GetVB11(0, 0);
	Strides[0] = (UINT)g_Mesh.GetVertexStride(0, 0);
	Offsets[0] = 0;
	pd3dImmediateContext->IASetVertexBuffers(0, 1, pVB, Strides, Offsets);
	pd3dImmediateContext->IASetIndexBuffer(g_Mesh.GetIB11(0), g_Mesh.GetIBFormat11(0), 0);

	D3DX11_TECHNIQUE_DESC techDesc;
	HRESULT hr;
	VTrace(g_pTechnique->GetDesc(&techDesc));

	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		for (UINT subset = 0; subset < g_Mesh.GetNumSubsets(0); ++subset)
		{
			auto pSubset = g_Mesh.GetSubset(0, subset);

			auto PrimType = g_Mesh.GetPrimitiveType11((SDKMESH_PRIMITIVE_TYPE)pSubset->PrimitiveType);
			pd3dImmediateContext->IASetPrimitiveTopology(PrimType);

			auto pDiffuseRV = g_Mesh.GetMaterial(pSubset->MaterialID)->pDiffuseRV11;
			g_ptxDiffuseVariable->SetResource(pDiffuseRV);

			g_pTechnique->GetPassByIndex(p)->Apply(0, pd3dImmediateContext);
			pd3dImmediateContext->DrawIndexed((UINT)pSubset->IndexCount, 0, (UINT)pSubset->VertexStart);
		}
	}

	return S_OK;
}

HRESULT GridNode::VOnUpdate(Scene* pScene, float fElapsedTime)
{
	if (g_bSpinning)
	{
		static float t = 0.0f;
		static ULONGLONG timeStart = 0;
		ULONGLONG timeCur = GetTickCount64();
		if (timeStart == 0)
			timeStart = timeCur;
		t = (timeCur - timeStart) / 1000.0f;
		g_World = XMMatrixRotationY(t);
	}
	else
	{
		g_World = XMMatrixRotationY(XMConvertToRadians(180.f));
	}

	XMMATRIX mRot = XMMatrixRotationX(XMConvertToRadians(-90.0f));
	g_World = mRot * g_World;
	return S_OK;
}
