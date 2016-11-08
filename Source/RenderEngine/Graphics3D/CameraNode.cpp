#include "CameraNode.h"
#include "Scene.h"

Frustum::Frustum()
	: m_Fov(XM_PIDIV4),
	m_Aspect(1.0f),
	m_Near(1.0f),
	m_Far(1000.0f)
{

}

void Frustum::Init(float fov, float aspect, float nearDistance, float farDistance)
{
	m_Fov = fov;
	m_Aspect = aspect;
	m_Near = nearDistance;
	m_Far = farDistance;

	float tanFovOver2 = tanf(m_Fov / 2.0f);
	Vector3 nearRight = (m_Near * tanFovOver2) * m_Aspect * Vector3::Right;
	Vector3 nearUp = (m_Near * tanFovOver2) * Vector3::Up;
	Vector3 farRight = (m_Far * tanFovOver2) * m_Aspect * Vector3::Right;
	Vector3 farUp = (m_Far * tanFovOver2)  * Vector3::Up;

	m_NearClip[0] = (m_Near * Vector3::Backward) - nearRight + nearUp;
	m_NearClip[1] = (m_Near * Vector3::Backward) + nearRight + nearUp;
	m_NearClip[2] = (m_Near * Vector3::Backward) + nearRight - nearUp;
	m_NearClip[3] = (m_Near * Vector3::Backward) - nearRight - nearUp;

	m_FarClip[0] = (m_Far * Vector3::Backward) - farRight + farUp;
	m_FarClip[1] = (m_Far * Vector3::Backward) + farRight + farUp;
	m_FarClip[2] = (m_Far * Vector3::Backward) + farRight - farUp;
	m_FarClip[3] = (m_Far * Vector3::Backward) - farRight - farUp;

	Vector3 origin(0.0f, 0.0f, 0.0f);
	m_Planes[FrustumPlane_Near] = Plane(m_NearClip[2], m_NearClip[1], m_NearClip[0]);
	m_Planes[FrustumPlane_Far] = Plane(m_FarClip[0], m_FarClip[1], m_FarClip[2]);
	m_Planes[FrustumPlane_Right] = Plane(m_FarClip[2], m_FarClip[1], origin);
	m_Planes[FrustumPlane_Top] = Plane(m_FarClip[1], m_FarClip[0], origin);
	m_Planes[FrustumPlane_Left] = Plane(m_FarClip[0], m_FarClip[3], origin);
	m_Planes[FrustumPlane_Bottom] = Plane(m_FarClip[3], m_FarClip[2], origin);

	for (auto& plane : m_Planes)
	{
		plane.Normalize();
	}
}

bool Frustum::IsInside(const Vector3 &point) const
{
	for (auto plane : m_Planes)
	{
		if (plane.DotCoordinate(point) < 0.0f)
			return false;
	}

	return true;
}

bool Frustum::IsInside(const Vector3 &point, float radius) const
{
	for (auto plane : m_Planes)
	{
		if (plane.DotCoordinate(point) < -radius)
			return false;
	}

	return true;
}

void Frustum::Render()
{

}

CameraNode::CameraNode(const Frustum& frustum)
	: SceneNode(INVALID_ACTOR_ID, WeakBaseRenderComponentPtr(), RenderPass_0, Matrix::CreateTranslation(Vector3(0.0f, 0.0f, 20.0f))),
	m_Frustum(frustum),
	m_IsDebugCamera(false),
	m_pTarget(nullptr),
	m_OffsetVector(0.0f, 1.0f, -10.0f, 0.0f),
	m_ViewMatrix(Matrix::Identity),
	m_ProjectionMatrix(Matrix::Identity)
{

}

CameraNode::~CameraNode()
{

}

HRESULT CameraNode::VOnRestore(Scene* pScene)
{
	m_Frustum.SetAspect((float)DXUTGetWindowWidth() / DXUTGetWindowHeight());
	m_ProjectionMatrix = Matrix::CreatePerspectiveFieldOfView(m_Frustum.m_Fov, m_Frustum.m_Aspect, m_Frustum.m_Near, m_Frustum.m_Far);
	return S_OK;
}

HRESULT CameraNode::VRender(Scene* pScene, double fTime, float fElapsedTime)
{
	if (m_IsDebugCamera)
	{
		pScene->PopMatrix();

		m_Frustum.Render();

		pScene->PushAndSetMatrix(m_Properties.GetWorldMatrix());
	}

	return S_OK;
}

Matrix CameraNode::GetWorldViewProjection(Scene* pScene)
{
	Matrix world = pScene->GetTopMatrix();
	Matrix worldView = world * m_ViewMatrix;
	return worldView * m_ProjectionMatrix;
}

void CameraNode::SetViewTransform(Scene* pScene)
{
	if (m_pTarget != nullptr)
	{
		Matrix worldTarget = m_pTarget->VGet()->GetWorldMatrix();
		Vector3 position = worldTarget.Translation() + Vector4::Transform(m_OffsetVector, worldTarget);
		worldTarget.Translation(position);
		VSetTransform(worldTarget);
	}

	m_ViewMatrix = VGet()->GetWorldMatrix().Invert();
}
