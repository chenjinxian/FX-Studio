#include "DebugGizmosNode.h"
#include "CameraNode.h"
#include "Scene.h"
#include "Material.h"
#include "../Actors/Actor.h"
#include "../Actors/RenderComponent.h"
#include "../Actors/TransformComponent.h"
#include "../ResourceCache/ResCache.h"
#include "../ResourceCache/TextureResource.h"
#include "../ResourceCache/ShaderResource.h"
#include "../AppFramework/BaseGameApp.h"
#include "../AppFramework/BaseGameLogic.h"
#include "boost/algorithm/clamp.hpp"

DebugGizmosNode::DebugGizmosNode()
	: SceneNode(INVALID_ACTOR_ID, nullptr, RenderPass_Debug, Matrix::CreateTranslation(0.0f, 0.5f, 0.0f)),
	m_pEffect(nullptr),
	m_pCurrentPass(nullptr),
	m_pVertexBuffer(nullptr),
	m_pIndexBuffer(nullptr),
	m_Type(TT_None),
	m_Axis(TA_None),
	m_MousePos(),
	m_IsLButtonClick(false),
	m_IsLButtonDown(false)
{

}

DebugGizmosNode::~DebugGizmosNode()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
}

HRESULT DebugGizmosNode::VOnInitSceneNode(Scene* pScene)
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);


	Resource effectRes("Effects\\DebugAssist.fx");
	shared_ptr<ResHandle> pEffectResHandle = g_pApp->GetResCache()->GetHandle(&effectRes);
	if (pEffectResHandle == nullptr)
	{
		return S_FALSE;
	}
	shared_ptr<HlslResourceExtraData> extra = static_pointer_cast<HlslResourceExtraData>(pEffectResHandle->GetExtraData());
	if (extra == nullptr)
	{
		return S_FALSE;
	}

	m_pEffect = extra->GetEffect();

	Technique* pCurrentTechnique = m_pEffect->GetTechniquesByName().at("main11");
	if (pCurrentTechnique == nullptr)
	{
		DEBUG_ERROR(std::string("technique is not exist: ") + "main11");
		return S_FALSE;
	}
	m_pCurrentPass = pCurrentTechnique->GetPassesByName().at("p0");
	if (m_pCurrentPass == nullptr)
	{
		DEBUG_ERROR(std::string("technique is not exist: ") + "p0");
		return S_FALSE;
	}

	CreateGeometryBuffers();

	return S_OK;
}

HRESULT DebugGizmosNode::VOnDeleteSceneNode(Scene *pScene)
{
	return S_OK;
}

HRESULT DebugGizmosNode::VOnUpdate(Scene* pScene, const GameTime& gameTime)
{
	switch (m_Type)
	{
	case DebugGizmosNode::TT_None:
		break;
	case DebugGizmosNode::TT_Translation:
		TranslatePicked(pScene);
		break;
	case DebugGizmosNode::TT_Rotation:
		RotatePicked(pScene);
		break;
	case DebugGizmosNode::TT_Scale:
		ScalePicked(pScene);
		break;
	default:
		break;
	}

	m_IsLButtonClick = false;
	return S_OK;
}

void DebugGizmosNode::PointerLeftClick(const Vector2& pos)
{
	m_IsLButtonClick = true;
	m_MousePos = pos;
}

void DebugGizmosNode::PointerMove(const Vector2 &pos, bool leftButtonDown)
{
	if (!leftButtonDown)
	{
		m_Axis = TA_None;
	}

	m_IsLButtonDown = leftButtonDown;
	m_MousePos = pos;
}

HRESULT DebugGizmosNode::VRender(Scene* pScene, const GameTime& gameTime)
{
	shared_ptr<ISceneNode> pPickedNode = pScene->FindActor(pScene->GetPickedActor());
	if (pPickedNode == nullptr)
	{
		return S_OK;
	}

	uint32_t stride = m_pCurrentPass->GetVertexSize();
	uint32_t offset = 0;
	pScene->GetRenderder()->VSetVertexBuffers(m_pVertexBuffer, &stride, &offset);
	pScene->GetRenderder()->VSetIndexBuffer(m_pIndexBuffer, IRenderer::Format_uint16, 0);

	pScene->GetRenderder()->VInputSetup(D3D11_PRIMITIVE_TOPOLOGY_LINELIST, m_pCurrentPass->GetInputLayout());
	RenderBoundingBox(pScene, pPickedNode->VGet()->GetBoundingBox(), pPickedNode->VGet()->GetWorldMatrix());

	shared_ptr<IRenderState> debugPass = pScene->GetRenderder()->VPrepareDebugPass();

	pScene->GetRenderder()->VInputSetup(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, m_pCurrentPass->GetInputLayout());
	switch (m_Type)
	{
	case DebugGizmosNode::TT_None:
		break;
	case DebugGizmosNode::TT_Translation:
		RenderTranslateAxes(pScene, pPickedNode->VGet()->GetBoundingBox(), pPickedNode->VGet()->GetWorldMatrix());
		break;
	case DebugGizmosNode::TT_Rotation:
		RenderRotateRings(pScene, pPickedNode->VGet()->GetBoundingBox(), pPickedNode->VGet()->GetWorldMatrix());
		break;
	case DebugGizmosNode::TT_Scale:
		RenderScaleAxes(pScene, pPickedNode->VGet()->GetBoundingBox(), pPickedNode->VGet()->GetWorldMatrix());
		break;
	default:
		break;
	}

	return S_OK;
}

bool DebugGizmosNode::VIsVisible(Scene* pScene) const
{
	return pScene->GetPickedActor() != INVALID_ACTOR_ID;
}

HRESULT DebugGizmosNode::RenderBoundingBox(Scene* pScene, const BoundingBox& aaBox, const Matrix& world)
{
	Variable* variable = m_pEffect->GetVariablesByName().at("WorldViewProjection");
	Matrix boxWorld = Matrix::CreateScale(aaBox.Extents * 2.0f) * world;
	boxWorld = boxWorld * Matrix::CreateTranslation(aaBox.Center);
	const XMMATRIX& wvp = boxWorld * pScene->GetCamera()->GetViewMatrix() * pScene->GetCamera()->GetProjectMatrix();
	variable->SetMatrix(wvp);

	Variable* ambientColor = m_pEffect->GetVariablesByName().at("AmbientColor");
	ambientColor->SetVector(Color(Colors::LightSkyBlue.f));

	pScene->GetRenderder()->VDrawMesh(m_AABoxIndexCount, m_AABoxIndexOffset, m_AABoxVertexOffset, m_pCurrentPass->GetEffectPass());

	return S_OK;
}

HRESULT DebugGizmosNode::RenderTranslateAxes(Scene* pScene, const BoundingBox& aaBox, const Matrix& world)
{
	Variable* objectWvp = m_pEffect->GetVariablesByName().at("WorldViewProjection");
	Variable* ambientColor = m_pEffect->GetVariablesByName().at("AmbientColor");

	Vector3 scale;
	Quaternion rotation;
	Vector3 translation;
	Matrix temp = world;
	temp.Decompose(scale, rotation, translation);

	const XMMATRIX& viewProj = pScene->GetCamera()->GetViewMatrix() * pScene->GetCamera()->GetProjectMatrix();

	bool isPicked = false;
	Vector3 cameraPos = pScene->GetCamera()->GetPosition();
	// x axis
	{
		Matrix aixsWorld = Matrix::CreateScale(Vector3::Distance(cameraPos, world.Translation()) * 0.2f) *
			Matrix::CreateFromQuaternion(rotation) * Matrix::CreateTranslation(translation);
		if (m_IsLButtonDown)
		{
			aixsWorld = Matrix::CreateScale(Vector3::Distance(cameraPos, m_LastTranslation) * 0.2f) *
				Matrix::CreateFromQuaternion(rotation) * Matrix::CreateTranslation(m_LastTranslation);
			ambientColor->SetVector(Color(1.0f, 0.27f, 0.0f));
		}
		else if (!m_IsLButtonDown && IsAxisPicked(
			CreateRay(pScene, Matrix::CreateRotationZ(XMConvertToRadians(-90)) * Matrix::CreateTranslation(Vector3(0.3f, 0.0f, 0.0f)) * aixsWorld)))
		{
			ambientColor->SetVector(Color(1.0f, 1.0f, 1.0f));
			isPicked = true;
			m_Axis = TA_AxisX;
		}
		else
			ambientColor->SetVector(Color(1.0f, 0.0f, 0.0f));

		aixsWorld = aixsWorld * Matrix::CreateTranslation(aaBox.Center);

		objectWvp->SetMatrix(Matrix::CreateRotationZ(XMConvertToRadians(-90)) * Matrix::CreateTranslation(Vector3(0.3f, 0.0f, 0.0f)) * aixsWorld * viewProj);
		pScene->GetRenderder()->VDrawMesh(m_CylinderIndexCount, m_CylinderIndexOffset, m_CylinderVertexOffset, m_pCurrentPass->GetEffectPass());
		objectWvp->SetMatrix(Matrix::CreateRotationZ(XMConvertToRadians(-90)) * Matrix::CreateTranslation(Vector3(0.6f, 0.0f, 0.0f)) * aixsWorld * viewProj);
		pScene->GetRenderder()->VDrawMesh(m_ConeIndexCount, m_ConeIndexOffset, m_ConeVertexOffset, m_pCurrentPass->GetEffectPass());
	}

	if (m_IsLButtonDown && m_Axis == TA_AxisX)
	{
		Matrix aixsWorld = Matrix::CreateScale(Vector3::Distance(cameraPos, world.Translation()) * 0.2f) * world;
		aixsWorld = aixsWorld * Matrix::CreateTranslation(aaBox.Center);
		ambientColor->SetVector(Color(1.0f, 1.0f, 0.0f));
		isPicked = true;

		objectWvp->SetMatrix(Matrix::CreateRotationZ(XMConvertToRadians(-90)) * Matrix::CreateTranslation(Vector3(0.3f, 0.0f, 0.0f)) * aixsWorld * viewProj);
		pScene->GetRenderder()->VDrawMesh(m_CylinderIndexCount, m_CylinderIndexOffset, m_CylinderVertexOffset, m_pCurrentPass->GetEffectPass());
		objectWvp->SetMatrix(Matrix::CreateRotationZ(XMConvertToRadians(-90)) * Matrix::CreateTranslation(Vector3(0.6f, 0.0f, 0.0f)) * aixsWorld * viewProj);
		pScene->GetRenderder()->VDrawMesh(m_ConeIndexCount, m_ConeIndexOffset, m_ConeVertexOffset, m_pCurrentPass->GetEffectPass());
	}

	// y axis
	{
		Matrix aixsWorld = Matrix::CreateScale(Vector3::Distance(cameraPos, world.Translation()) * 0.2f) *
			Matrix::CreateFromQuaternion(rotation) * Matrix::CreateTranslation(translation);
		if (m_IsLButtonDown)
		{
			aixsWorld = Matrix::CreateScale(Vector3::Distance(cameraPos, m_LastTranslation) * 0.2f) *
				Matrix::CreateFromQuaternion(rotation) * Matrix::CreateTranslation(m_LastTranslation);
			ambientColor->SetVector(Color(1.0f, 0.27f, 0.0f));
		}
		else if (!m_IsLButtonDown && !isPicked && IsAxisPicked(
			CreateRay(pScene, Matrix::CreateTranslation(Vector3(0.0f, 0.3f, 0.0f)) * aixsWorld)))
		{
			ambientColor->SetVector(Color(1.0f, 1.0f, 1.0f));
			isPicked = true;
			m_Axis = TA_AxisY;
		}
		else
			ambientColor->SetVector(Color(0.0f, 1.0f, 0.0f));

		aixsWorld = aixsWorld * Matrix::CreateTranslation(aaBox.Center);

		objectWvp->SetMatrix(Matrix::CreateTranslation(Vector3(0.0f, 0.3f, 0.0f)) * aixsWorld * viewProj);
		pScene->GetRenderder()->VDrawMesh( m_CylinderIndexCount, m_CylinderIndexOffset, m_CylinderVertexOffset, m_pCurrentPass->GetEffectPass());
		objectWvp->SetMatrix(Matrix::CreateTranslation(Vector3(0.0f, 0.6f, 0.0f)) * aixsWorld * viewProj);
		pScene->GetRenderder()->VDrawMesh( m_ConeIndexCount, m_ConeIndexOffset, m_ConeVertexOffset, m_pCurrentPass->GetEffectPass());
	}

	if (m_IsLButtonDown && m_Axis == TA_AxisY)
	{
		Matrix aixsWorld = Matrix::CreateScale(Vector3::Distance(cameraPos, world.Translation()) * 0.2f) * world;
		aixsWorld = aixsWorld * Matrix::CreateTranslation(aaBox.Center);
		ambientColor->SetVector(Color(1.0f, 1.0f, 0.0f));
		isPicked = true;

		objectWvp->SetMatrix(Matrix::CreateTranslation(Vector3(0.0f, 0.3f, 0.0f)) * aixsWorld * viewProj);
		pScene->GetRenderder()->VDrawMesh(m_CylinderIndexCount, m_CylinderIndexOffset, m_CylinderVertexOffset, m_pCurrentPass->GetEffectPass());
		objectWvp->SetMatrix(Matrix::CreateTranslation(Vector3(0.0f, 0.6f, 0.0f)) * aixsWorld * viewProj);
		pScene->GetRenderder()->VDrawMesh(m_ConeIndexCount, m_ConeIndexOffset, m_ConeVertexOffset, m_pCurrentPass->GetEffectPass());
	}

	// z axis
	{
		Matrix aixsWorld = Matrix::CreateScale(Vector3::Distance(cameraPos, world.Translation()) * 0.2f) *
			Matrix::CreateFromQuaternion(rotation) * Matrix::CreateTranslation(translation);
		if (m_IsLButtonDown)
		{
			aixsWorld = Matrix::CreateScale(Vector3::Distance(cameraPos, m_LastTranslation) * 0.2f) *
				Matrix::CreateFromQuaternion(rotation) * Matrix::CreateTranslation(m_LastTranslation);
			ambientColor->SetVector(Color(1.0f, 0.27f, 0.0f));
		}
		else if (!m_IsLButtonDown && !isPicked && IsAxisPicked(
			CreateRay(pScene, Matrix::CreateRotationX(XMConvertToRadians(90)) * Matrix::CreateTranslation(Vector3(0.0f, 0.0f, 0.3f)) * aixsWorld)))
		{
			ambientColor->SetVector(Color(1.0f, 1.0f, 1.0f));
			isPicked = true;
			m_Axis = TA_AxisZ;
		}
		else
			ambientColor->SetVector(Color(0.0f, 0.0f, 1.0f));

		aixsWorld = aixsWorld * Matrix::CreateTranslation(aaBox.Center);

		objectWvp->SetMatrix(Matrix::CreateRotationX(XMConvertToRadians(90)) * Matrix::CreateTranslation(Vector3(0.0f, 0.0f, 0.3f)) * aixsWorld * viewProj);
		pScene->GetRenderder()->VDrawMesh( m_CylinderIndexCount, m_CylinderIndexOffset, m_CylinderVertexOffset, m_pCurrentPass->GetEffectPass());
		objectWvp->SetMatrix(Matrix::CreateRotationX(XMConvertToRadians(90)) * Matrix::CreateTranslation(Vector3(0.0f, 0.0f, 0.6f)) * aixsWorld * viewProj);
		pScene->GetRenderder()->VDrawMesh( m_ConeIndexCount, m_ConeIndexOffset, m_ConeVertexOffset, m_pCurrentPass->GetEffectPass());
	}

	if (m_IsLButtonDown && m_Axis == TA_AxisZ)
	{
		Matrix aixsWorld = Matrix::CreateScale(Vector3::Distance(cameraPos, world.Translation()) * 0.2f) * world;
		aixsWorld = aixsWorld * Matrix::CreateTranslation(aaBox.Center);
		ambientColor->SetVector(Color(1.0f, 1.0f, 0.0f));
		isPicked = true;

		objectWvp->SetMatrix(Matrix::CreateRotationX(XMConvertToRadians(90)) * Matrix::CreateTranslation(Vector3(0.0f, 0.0f, 0.3f)) * aixsWorld * viewProj);
		pScene->GetRenderder()->VDrawMesh(m_CylinderIndexCount, m_CylinderIndexOffset, m_CylinderVertexOffset, m_pCurrentPass->GetEffectPass());
		objectWvp->SetMatrix(Matrix::CreateRotationX(XMConvertToRadians(90)) * Matrix::CreateTranslation(Vector3(0.0f, 0.0f, 0.6f)) * aixsWorld * viewProj);
		pScene->GetRenderder()->VDrawMesh(m_ConeIndexCount, m_ConeIndexOffset, m_ConeVertexOffset, m_pCurrentPass->GetEffectPass());
	}

	return S_OK;
}

HRESULT DebugGizmosNode::RenderRotateRings(Scene* pScene, const BoundingBox& aaBox, const Matrix& world)
{
	Variable* objectWvp = m_pEffect->GetVariablesByName().at("WorldViewProjection");
	Variable* ambientColor = m_pEffect->GetVariablesByName().at("AmbientColor");

	Vector3 scale;
	Quaternion rotation;
	Vector3 translation;
	Matrix temp = world;
	temp.Decompose(scale, rotation, translation);

	Vector3 cameraPos = pScene->GetCamera()->GetPosition();
	Matrix ringWorld = Matrix::CreateScale(Vector3::Distance(cameraPos, world.Translation()) * 0.2f) *
		Matrix::CreateFromQuaternion(rotation) * Matrix::CreateTranslation(translation);
	ringWorld = ringWorld * Matrix::CreateTranslation(aaBox.Center);
	const XMMATRIX& wvp = ringWorld * pScene->GetCamera()->GetViewMatrix() * pScene->GetCamera()->GetProjectMatrix();

	bool isPicked = false;
	// x axis
	{
		if (m_IsLButtonDown)
		{
			ambientColor->SetVector(Color(1.0f, 0.27f, 0.0f));
		}
		else if (!m_IsLButtonDown && IsTorusPicked(CreateRay(pScene, Matrix::CreateRotationZ(XMConvertToRadians(-90)) * ringWorld)))
		{
			ambientColor->SetVector(Color(1.0f, 1.0f, 1.0f));
			isPicked = true;
			m_Axis = TA_AxisX;
		}
		else
			ambientColor->SetVector(Color(1.0f, 0.0f, 0.0f));

		objectWvp->SetMatrix(Matrix::CreateRotationZ(XMConvertToRadians(-90)) * wvp);
		pScene->GetRenderder()->VDrawMesh(m_TorusIndexCount, m_TorusIndexOffset, m_TorusVertexOffset, m_pCurrentPass->GetEffectPass());
	}

	if (m_IsLButtonDown && m_Axis == TA_AxisX)
	{
		ambientColor->SetVector(Color(1.0f, 1.0f, 0.0f));
		isPicked = true;
	}

	// y axis
	{
		if (m_IsLButtonDown)
		{
			ambientColor->SetVector(Color(1.0f, 0.27f, 0.0f));
		}
		else if (!m_IsLButtonDown && !isPicked && IsTorusPicked(CreateRay(pScene, ringWorld)))
		{
			ambientColor->SetVector(Color(1.0f, 1.0f, 1.0f));
			isPicked = true;
			m_Axis = TA_AxisY;
		}
		else
			ambientColor->SetVector(Color(0.0f, 1.0f, 0.0f));

		objectWvp->SetMatrix(wvp);
		pScene->GetRenderder()->VDrawMesh(m_TorusIndexCount, m_TorusIndexOffset, m_TorusVertexOffset, m_pCurrentPass->GetEffectPass());
	}

	if (m_IsLButtonDown && m_Axis == TA_AxisY)
	{
		ambientColor->SetVector(Color(1.0f, 1.0f, 0.0f));
		isPicked = true;
	}

	// z axis
	{
		if (m_IsLButtonDown)
		{
			ambientColor->SetVector(Color(1.0f, 0.27f, 0.0f));
		}
		else if (!m_IsLButtonDown && !isPicked && IsTorusPicked(CreateRay(pScene, Matrix::CreateRotationX(XMConvertToRadians(90)) * ringWorld)))
		{
			ambientColor->SetVector(Color(1.0f, 1.0f, 1.0f));
			isPicked = true;
			m_Axis = TA_AxisZ;
		}
		else
			ambientColor->SetVector(Color(0.0f, 0.0f, 1.0f));

		objectWvp->SetMatrix(Matrix::CreateRotationX(XMConvertToRadians(90)) * wvp);
		pScene->GetRenderder()->VDrawMesh(m_TorusIndexCount, m_TorusIndexOffset, m_TorusVertexOffset, m_pCurrentPass->GetEffectPass());
	}

	if (m_IsLButtonDown && m_Axis == TA_AxisZ)
	{
		ambientColor->SetVector(Color(1.0f, 1.0f, 0.0f));
		isPicked = true;
	}

	return S_OK;
}

HRESULT DebugGizmosNode::RenderScaleAxes(Scene* pScene, const BoundingBox& aaBox, const Matrix& world)
{
	Variable* objectWvp = m_pEffect->GetVariablesByName().at("WorldViewProjection");
	Variable* ambientColor = m_pEffect->GetVariablesByName().at("AmbientColor");

	Vector3 scale;
	Quaternion rotation;
	Vector3 translation;
	Matrix temp = world;
	temp.Decompose(scale, rotation, translation);

	const XMMATRIX& viewProj = pScene->GetCamera()->GetViewMatrix() * pScene->GetCamera()->GetProjectMatrix();

	bool isPicked = false;
	Vector3 cameraPos = pScene->GetCamera()->GetPosition();
	// x axis
	{
		Matrix aixsWorld = Matrix::CreateScale(Vector3::Distance(cameraPos, world.Translation()) * 0.2f) *
			Matrix::CreateFromQuaternion(rotation) * Matrix::CreateTranslation(translation);
		if (m_IsLButtonDown)
		{
			ambientColor->SetVector(Color(1.0f, 0.27f, 0.0f));
		}
		else if (!m_IsLButtonDown && IsAxisPicked(
			CreateRay(pScene, Matrix::CreateRotationZ(XMConvertToRadians(-90)) * Matrix::CreateTranslation(Vector3(0.3f, 0.0f, 0.0f)) * aixsWorld)))
		{
			ambientColor->SetVector(Color(1.0f, 1.0f, 1.0f));
			isPicked = true;
			m_Axis = TA_AxisX;
		}
		else
			ambientColor->SetVector(Color(1.0f, 0.0f, 0.0f));

		aixsWorld = aixsWorld * Matrix::CreateTranslation(aaBox.Center);

		objectWvp->SetMatrix(Matrix::CreateRotationZ(XMConvertToRadians(-90)) * Matrix::CreateTranslation(Vector3(0.3f, 0.0f, 0.0f)) * aixsWorld * viewProj);
		pScene->GetRenderder()->VDrawMesh(m_CylinderIndexCount, m_CylinderIndexOffset, m_CylinderVertexOffset, m_pCurrentPass->GetEffectPass());
		objectWvp->SetMatrix(Matrix::CreateRotationZ(XMConvertToRadians(-90)) * Matrix::CreateTranslation(Vector3(0.6f, 0.0f, 0.0f)) * aixsWorld * viewProj);
		pScene->GetRenderder()->VDrawMesh(m_CubeIndexCount, m_CubeIndexOffset, m_CubeVertexOffset, m_pCurrentPass->GetEffectPass());
	}

	if (m_IsLButtonDown && m_Axis == TA_AxisX)
	{
		Matrix aixsWorld = Matrix::CreateScale(Vector3::Distance(cameraPos, world.Translation()) * 0.2f) * world;
		aixsWorld = aixsWorld * Matrix::CreateTranslation(aaBox.Center);
		ambientColor->SetVector(Color(1.0f, 1.0f, 0.0f));
		isPicked = true;

		objectWvp->SetMatrix(Matrix::CreateRotationZ(XMConvertToRadians(-90)) * Matrix::CreateTranslation(Vector3(0.3f, 0.0f, 0.0f)) * aixsWorld * viewProj);
		pScene->GetRenderder()->VDrawMesh(m_CylinderIndexCount, m_CylinderIndexOffset, m_CylinderVertexOffset, m_pCurrentPass->GetEffectPass());
		objectWvp->SetMatrix(Matrix::CreateRotationZ(XMConvertToRadians(-90)) * Matrix::CreateTranslation(Vector3(0.6f, 0.0f, 0.0f)) * aixsWorld * viewProj);
		pScene->GetRenderder()->VDrawMesh(m_CubeIndexCount, m_CubeIndexOffset, m_CubeVertexOffset, m_pCurrentPass->GetEffectPass());
	}

	// y axis
	{
		Matrix aixsWorld = Matrix::CreateScale(Vector3::Distance(cameraPos, world.Translation()) * 0.2f) *
			Matrix::CreateFromQuaternion(rotation) * Matrix::CreateTranslation(translation);
		if (m_IsLButtonDown)
		{
			ambientColor->SetVector(Color(1.0f, 0.27f, 0.0f));
		}
		else if (!m_IsLButtonDown && !isPicked && IsAxisPicked(
			CreateRay(pScene, Matrix::CreateTranslation(Vector3(0.0f, 0.3f, 0.0f)) * aixsWorld)))
		{
			ambientColor->SetVector(Color(1.0f, 1.0f, 1.0f));
			isPicked = true;
			m_Axis = TA_AxisY;
		}
		else
			ambientColor->SetVector(Color(0.0f, 1.0f, 0.0f));

		aixsWorld = aixsWorld * Matrix::CreateTranslation(aaBox.Center);

		objectWvp->SetMatrix(Matrix::CreateTranslation(Vector3(0.0f, 0.3f, 0.0f)) * aixsWorld * viewProj);
		pScene->GetRenderder()->VDrawMesh(m_CylinderIndexCount, m_CylinderIndexOffset, m_CylinderVertexOffset, m_pCurrentPass->GetEffectPass());
		objectWvp->SetMatrix(Matrix::CreateTranslation(Vector3(0.0f, 0.6f, 0.0f)) * aixsWorld * viewProj);
		pScene->GetRenderder()->VDrawMesh(m_CubeIndexCount, m_CubeIndexOffset, m_CubeVertexOffset, m_pCurrentPass->GetEffectPass());
	}

	if (m_IsLButtonDown && m_Axis == TA_AxisY)
	{
		Matrix aixsWorld = Matrix::CreateScale(Vector3::Distance(cameraPos, world.Translation()) * 0.2f) * world;
		aixsWorld = aixsWorld * Matrix::CreateTranslation(aaBox.Center);
		ambientColor->SetVector(Color(1.0f, 1.0f, 0.0f));
		isPicked = true;

		objectWvp->SetMatrix(Matrix::CreateTranslation(Vector3(0.0f, 0.3f, 0.0f)) * aixsWorld * viewProj);
		pScene->GetRenderder()->VDrawMesh(m_CylinderIndexCount, m_CylinderIndexOffset, m_CylinderVertexOffset, m_pCurrentPass->GetEffectPass());
		objectWvp->SetMatrix(Matrix::CreateTranslation(Vector3(0.0f, 0.6f, 0.0f)) * aixsWorld * viewProj);
		pScene->GetRenderder()->VDrawMesh(m_CubeIndexCount, m_CubeIndexOffset, m_CubeVertexOffset, m_pCurrentPass->GetEffectPass());
	}

	// z axis
	{
		Matrix aixsWorld = Matrix::CreateScale(Vector3::Distance(cameraPos, world.Translation()) * 0.2f) *
			Matrix::CreateFromQuaternion(rotation) * Matrix::CreateTranslation(translation);
		if (m_IsLButtonDown)
		{
			ambientColor->SetVector(Color(1.0f, 0.27f, 0.0f));
		}
		else if (!m_IsLButtonDown && !isPicked && IsAxisPicked(
			CreateRay(pScene, Matrix::CreateRotationX(XMConvertToRadians(90)) * Matrix::CreateTranslation(Vector3(0.0f, 0.0f, 0.3f)) * aixsWorld)))
		{
			ambientColor->SetVector(Color(1.0f, 1.0f, 1.0f));
			isPicked = true;
			m_Axis = TA_AxisZ;
		}
		else
			ambientColor->SetVector(Color(0.0f, 0.0f, 1.0f));

		aixsWorld = aixsWorld * Matrix::CreateTranslation(aaBox.Center);

		objectWvp->SetMatrix(Matrix::CreateRotationX(XMConvertToRadians(90)) * Matrix::CreateTranslation(Vector3(0.0f, 0.0f, 0.3f)) * aixsWorld * viewProj);
		pScene->GetRenderder()->VDrawMesh(m_CylinderIndexCount, m_CylinderIndexOffset, m_CylinderVertexOffset, m_pCurrentPass->GetEffectPass());
		objectWvp->SetMatrix(Matrix::CreateRotationX(XMConvertToRadians(90)) * Matrix::CreateTranslation(Vector3(0.0f, 0.0f, 0.6f)) * aixsWorld * viewProj);
		pScene->GetRenderder()->VDrawMesh(m_CubeIndexCount, m_CubeIndexOffset, m_CubeVertexOffset, m_pCurrentPass->GetEffectPass());
	}

	if (m_IsLButtonDown && m_Axis == TA_AxisZ)
	{
		Matrix aixsWorld = Matrix::CreateScale(Vector3::Distance(cameraPos, world.Translation()) * 0.2f) * world;
		aixsWorld = aixsWorld * Matrix::CreateTranslation(aaBox.Center);
		ambientColor->SetVector(Color(1.0f, 1.0f, 0.0f));
		isPicked = true;

		objectWvp->SetMatrix(Matrix::CreateRotationX(XMConvertToRadians(90)) * Matrix::CreateTranslation(Vector3(0.0f, 0.0f, 0.3f)) * aixsWorld * viewProj);
		pScene->GetRenderder()->VDrawMesh(m_CylinderIndexCount, m_CylinderIndexOffset, m_CylinderVertexOffset, m_pCurrentPass->GetEffectPass());
		objectWvp->SetMatrix(Matrix::CreateRotationX(XMConvertToRadians(90)) * Matrix::CreateTranslation(Vector3(0.0f, 0.0f, 0.6f)) * aixsWorld * viewProj);
		pScene->GetRenderder()->VDrawMesh(m_CubeIndexCount, m_CubeIndexOffset, m_CubeVertexOffset, m_pCurrentPass->GetEffectPass());
	}

	return S_OK;
}

void DebugGizmosNode::CreateAABox(std::vector<Vector3>& vertices, std::vector<uint16_t>& indices)
{
	vertices.clear();
	vertices.reserve(8);
	Color color(Colors::LightSkyBlue.f);

	vertices.push_back(Vector3(-0.5f, -0.5f, -0.5f));
	vertices.push_back(Vector3(0.5f, -0.5f, -0.5f));
	vertices.push_back(Vector3(0.5f, -0.5f, 0.5f));
	vertices.push_back(Vector3(-0.5f, -0.5f, 0.5f));
	vertices.push_back(Vector3(-0.5f, 0.5f, -0.5f));
	vertices.push_back(Vector3(0.5f, 0.5f, -0.5f));
	vertices.push_back(Vector3(0.5f, 0.5f, 0.5f));
	vertices.push_back(Vector3(-0.5f, 0.5f, 0.5f));

	uint16_t arrayIndices[] = { 0, 1, 1, 2, 2, 3, 3, 0, 4, 5, 5, 6, 6, 7, 7, 4, 0, 4, 1, 5, 2, 6, 3, 7 };
	indices.clear();
	indices.resize(ARRAYSIZE(arrayIndices));
	memcpy_s(&indices.front(), sizeof(arrayIndices), arrayIndices, sizeof(arrayIndices));
}

void DebugGizmosNode::AddVertex(std::vector<Vector3>& vertices, const std::vector<struct VertexPositionNormalTexture>& inputVertices)
{
	for (auto& vertex : inputVertices)
	{
		vertices.push_back(vertex.position);
	}
}

void DebugGizmosNode::CreateGeometryBuffers()
{
	std::vector<Vector3> boxVertices;
	std::vector<uint16_t> boxIndices;
	CreateAABox(boxVertices, boxIndices);

	std::vector<VertexPositionNormalTexture> cylinderVertices;
	std::vector<uint16_t> cylinderIndices;
	GeometricPrimitive::CreateCylinder(cylinderVertices, cylinderIndices, 0.6f, 0.01f);

	std::vector<VertexPositionNormalTexture> coneVertices;
	std::vector<uint16_t> coneIndices;
	GeometricPrimitive::CreateCone(coneVertices, coneIndices, 0.06f, 0.1f);

	std::vector<VertexPositionNormalTexture> torusVertices;
	std::vector<uint16_t> torusIndices;
	GeometricPrimitive::CreateTorus(torusVertices, torusIndices, 1.2f, 0.01f);

	std::vector<VertexPositionNormalTexture> cubeVertices;
	std::vector<uint16_t> cubeIndices;
	GeometricPrimitive::CreateCube(cubeVertices, cubeIndices, 0.06f);

	m_AABoxVertexOffset = 0;
	m_CylinderVertexOffset = boxVertices.size();
	m_ConeVertexOffset = m_CylinderVertexOffset + cylinderVertices.size();
	m_TorusVertexOffset = m_ConeVertexOffset + coneVertices.size();
	m_CubeVertexOffset = m_TorusVertexOffset + torusVertices.size();

	m_AABoxIndexCount = boxIndices.size();
	m_CylinderIndexCount = cylinderIndices.size();
	m_ConeIndexCount = coneIndices.size();
	m_TorusIndexCount = torusIndices.size();
	m_CubeIndexCount = cubeIndices.size();

	m_AABoxIndexOffset = 0;
	m_CylinderIndexOffset = m_AABoxIndexCount;
	m_ConeIndexOffset = m_CylinderIndexOffset + m_CylinderIndexCount;
	m_TorusIndexOffset = m_ConeIndexOffset + m_ConeIndexCount;
	m_CubeIndexOffset = m_TorusIndexOffset + m_TorusIndexCount;

	uint32_t vertexCount = 
		boxVertices.size() + cylinderVertices.size() + coneVertices.size() + torusVertices.size() + cubeVertices.size();
	std::vector<Vector3> vertices;
	vertices.reserve(vertexCount);

	vertices.insert(vertices.end(), boxVertices.begin(), boxVertices.end());

	AddVertex(vertices, cylinderVertices);
	AddVertex(vertices, coneVertices);
	AddVertex(vertices, torusVertices);
	AddVertex(vertices, cubeVertices);

	std::vector<uint16_t> indices;
	indices.insert(indices.end(), boxIndices.begin(), boxIndices.end());
	indices.insert(indices.end(), cylinderIndices.begin(), cylinderIndices.end());
	indices.insert(indices.end(), coneIndices.begin(), coneIndices.end());
	indices.insert(indices.end(), torusIndices.begin(), torusIndices.end());
	indices.insert(indices.end(), cubeIndices.begin(), cubeIndices.end());

	m_pCurrentPass->CreateVertexBuffer(&vertices.front(), vertices.size() * sizeof(Vector3), &m_pVertexBuffer);
	m_pCurrentPass->CreateIndexBuffer(&indices.front(), indices.size() * sizeof(uint16_t), &m_pIndexBuffer);

	vertices.clear();
	AddVertex(vertices, cylinderVertices);
	AddVertex(vertices, cubeVertices);
	SetBoundingBox(vertices);

	m_TorusVertices.clear();
	GeometricPrimitive::CreateTorus(torusVertices, m_TorusIndices, 1.2f, 0.06f);
	AddVertex(m_TorusVertices, torusVertices);
	SetBoundingSphere(m_TorusVertices);
}

Ray DebugGizmosNode::CreateRay(Scene* pScene, const Matrix& world, bool inLocal)
{
	const Matrix& projectMat = pScene->GetCamera()->GetProjectMatrix();
	float viewX = (2.0f * m_MousePos.x / g_pApp->GetGameConfig().m_ScreenWidth - 1.0f) / projectMat.m[0][0];
	float viewY = (1.0f - 2.0f * m_MousePos.y / g_pApp->GetGameConfig().m_ScreenHeight) / projectMat.m[1][1];

	Matrix toSpace;
	if (inLocal)
	{
		toSpace = (world * pScene->GetCamera()->GetViewMatrix()).Invert();
	}
	else
	{
		toSpace = (pScene->GetCamera()->GetViewMatrix()).Invert();
	}

	Vector3 rayPos = toSpace.Translation();
	//use right-hand coordinates, z should be -1
	Vector3 rayDir = Vector3::TransformNormal(Vector3(viewX, viewY, -1.0f), toSpace);
	rayDir.Normalize();

	Ray ray(rayPos, rayDir);
	return ray;
}

bool DebugGizmosNode::IsAxisPicked(const Ray& ray)
{
	float distance = 0.0f;
	return ray.Intersects(m_Properties.GetBoundingBox(), distance);
}

bool DebugGizmosNode::IsTorusPicked(const Ray& ray)
{
	float distance = 0.0f;
	if (ray.Intersects(m_Properties.GetBoundingSphere(), distance))
	{
		for (uint32_t i = 0, len = m_TorusIndices.size(); i < len; i += 3)
		{
			Vector3 tri0 = m_TorusVertices.at(m_TorusIndices[i]);
			Vector3 tri1 = m_TorusVertices.at(m_TorusIndices[i + 1]);
			Vector3 tri2 = m_TorusVertices.at(m_TorusIndices[i + 2]);

			float distance = 0.0f;
			if (ray.Intersects(tri0, tri1, tri2, distance))
			{
				return true;
			}
		}
	}

	return false;
}

float DebugGizmosNode::IntersectRayPlane(const Plane& plane, const Ray& ray)
{
	float distance = 0.0f;
	if (ray.Intersects(plane, distance))
	{
		if (distance <= 0.0f)
		{
			// ray direction vector and plan is coplanarity
			distance = Vector3::Distance(ray.direction, ray.position);
		}
	}
	return distance;
}

float DebugGizmosNode::ComputeAngleOnPlane(const Vector3& oldVector, const Vector3& newVector, const Vector3& normal)
{
	Vector3 dir1 = oldVector;
	dir1.Normalize();

	Vector3 dir2 = newVector;
	dir2.Normalize();

	float angle = acosf(boost::algorithm::clamp(dir1.Dot(dir2), -1.0f, 1.0f));
	if (dir1.Cross(dir2).Dot(normal) < 0.0f)
	{
		angle = -angle;
	}
	return angle;
}

void DebugGizmosNode::TranslatePicked(Scene* pScene)
{
	shared_ptr<ISceneNode> pPickedNode = pScene->FindActor(pScene->GetPickedActor());
	if (pPickedNode != nullptr && m_IsLButtonClick)
	{
		const Matrix& world = pPickedNode->VGet()->GetWorldMatrix();

		Vector3 axis;
		switch (m_Axis)
		{
		case DebugGizmosNode::TA_AxisX: axis = world.Right(); break;
		case DebugGizmosNode::TA_AxisY: axis = world.Up(); break;
		case DebugGizmosNode::TA_AxisZ: axis = world.Forward(); break;
		default: return;
		}

		Ray ray = CreateRay(pScene, world, false);
		Vector3 nodePos = world.Translation();
		Vector3 normal = ray.direction.Cross(axis).Cross(ray.direction);

		float numer = (ray.position - nodePos).Dot(normal);
		float denom = axis.Dot(normal);
		float distance = 0.0f;
		if (fabsf(denom) > FLT_EPSILON)
			distance = numer / denom;

		m_LastOffset = nodePos + axis * distance;
		m_LastTranslation = world.Translation();
	}

	if (pPickedNode != nullptr && m_IsLButtonDown)
	{
		const Matrix& world = pPickedNode->VGet()->GetWorldMatrix();

		Vector3 axis;
		switch (m_Axis)
		{
		case DebugGizmosNode::TA_AxisX: axis = world.Right(); break;
		case DebugGizmosNode::TA_AxisY: axis = world.Up(); break;
		case DebugGizmosNode::TA_AxisZ: axis = world.Forward(); break;
		default: return;
		}

		Ray ray = CreateRay(pScene, world, false);
		Vector3 nodePos = world.Translation();
		Vector3 normal = ray.direction.Cross(axis).Cross(ray.direction);

		float numer = (ray.position - nodePos).Dot(normal);
		float denom = axis.Dot(normal);
		float distance = 0.0f;
		if (fabsf(denom) > FLT_EPSILON)
			distance = numer / denom;

		Vector3 newOffset = nodePos + axis * distance;
		pPickedNode->VSetTransform(world * Matrix::CreateTranslation(newOffset - m_LastOffset));
		m_LastOffset = newOffset;
	}
}

void DebugGizmosNode::ScalePicked(Scene* pScene)
{
	shared_ptr<ISceneNode> pPickedNode = pScene->FindActor(pScene->GetPickedActor());
	if (pPickedNode != nullptr && m_IsLButtonClick)
	{
		const Matrix& world = pPickedNode->VGet()->GetWorldMatrix();

		Vector3 axis;
		switch (m_Axis)
		{
		case DebugGizmosNode::TA_AxisX: axis = world.Right(); break;
		case DebugGizmosNode::TA_AxisY: axis = world.Up(); break;
		case DebugGizmosNode::TA_AxisZ: axis = world.Forward(); break;
		default: return;
		}

		Ray ray = CreateRay(pScene, world, false);
		Vector3 nodePos = world.Translation();
		Vector3 normal = ray.direction.Cross(axis).Cross(ray.direction);

		float numer = (ray.position - nodePos).Dot(normal);
		float denom = axis.Dot(normal);
		float distance = 0.0f;
		if (fabsf(denom) > FLT_EPSILON)
			distance = numer / denom;

		m_LastOffset = nodePos + axis * distance;
		m_LastScale = Vector3(world._11, world._22, world._33);
	}

	if (pPickedNode != nullptr && m_IsLButtonDown)
	{
		const Matrix& world = pPickedNode->VGet()->GetWorldMatrix();

		Vector3 axis;
		Vector3 local;
		switch (m_Axis)
		{
		case DebugGizmosNode::TA_AxisX: axis = world.Right(); local = Vector3::Right; break;
		case DebugGizmosNode::TA_AxisY: axis = world.Up(); local = Vector3::Up; break;
		case DebugGizmosNode::TA_AxisZ: axis = world.Forward(); local = Vector3::Backward; break;
		default: return;
		}

		Ray ray = CreateRay(pScene, world, false);
		Vector3 nodePos = world.Translation();
		Vector3 normal = ray.direction.Cross(axis).Cross(ray.direction);

		float numer = (ray.position - nodePos).Dot(normal);
		float denom = axis.Dot(normal);
		float distance = 0.0f;
		if (fabsf(denom) > FLT_EPSILON)
			distance = numer / denom;

		Matrix object = world;
		Vector3 scale;
		Quaternion rotation;
		Vector3 translation;
		object.Decompose(scale, rotation, translation);

		Vector3 newOffset = nodePos + axis * distance;
		Vector3 offset = (newOffset - m_LastOffset) * local * 8.33f / Vector3::Distance(pScene->GetCamera()->GetPosition(), world.Translation());

		scale += m_LastScale * offset;
		if (scale.x > 0.001f && scale.y > 0.001f && scale.z > 0.001f)
		{
			pPickedNode->VSetTransform(
				Matrix::CreateScale(scale) * Matrix::Transform(Matrix::Identity, rotation) * Matrix::CreateTranslation(translation));
		}

		m_LastOffset = newOffset;
	}
}

void DebugGizmosNode::RotatePicked(Scene* pScene)
{
	shared_ptr<ISceneNode> pPickedNode = pScene->FindActor(pScene->GetPickedActor());
	if (pPickedNode != nullptr && m_IsLButtonClick)
	{
		const Matrix& world = pPickedNode->VGet()->GetWorldMatrix();
		Matrix cameraWorld = pScene->GetCamera()->VGet()->GetWorldMatrix();
		Ray ray = CreateRay(pScene, world, false);
		Plane plane(world.Translation(), cameraWorld.Forward());

		m_LastOffset = ray.position + ray.direction * IntersectRayPlane(plane, ray);
	}

	if (pPickedNode != nullptr && m_IsLButtonDown)
	{
		const Matrix& world = pPickedNode->VGet()->GetWorldMatrix();

		Vector3 axis;
		switch (m_Axis)
		{
		case DebugGizmosNode::TA_AxisX: axis = world.Right(); break;
		case DebugGizmosNode::TA_AxisY: axis = world.Up(); break;
		case DebugGizmosNode::TA_AxisZ: axis = world.Backward(); break;
		default: return;
		}

		Matrix cameraWorld = pScene->GetCamera()->VGet()->GetWorldMatrix();
		Ray ray = CreateRay(pScene, world, false);
		Plane plane(world.Translation(), cameraWorld.Forward());
		Vector3 newOffset = ray.position + ray.direction * IntersectRayPlane(plane, ray);

		Matrix object = world;
		Vector3 scale;
		Quaternion rotation;
		Vector3 translation;
		object.Decompose(scale, rotation, translation);

		Quaternion newRot = Quaternion::CreateFromAxisAngle(
			axis, ComputeAngleOnPlane(m_LastOffset - translation, newOffset - translation, axis));
		newRot.Normalize();
		pPickedNode->VSetTransform(
			Matrix::CreateScale(scale) * Matrix::Transform(Matrix::Identity, rotation * newRot) * Matrix::CreateTranslation(translation));

		m_LastOffset = newOffset;
	}
}
