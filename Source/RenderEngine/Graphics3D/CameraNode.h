#pragma once
#include "SceneNode.h"

class Frustum
{
	friend class CameraNode;

public:
	Frustum();

	const Plane& GetNearPlane() const { return m_Planes[FrustumPlane_Near]; }
	const Plane& GetFarPlane() const { return m_Planes[FrustumPlane_Far]; }
	const Plane& GetLeftPlane() const { return m_Planes[FrustumPlane_Left]; }
	const Plane& GetRightPlane() const { return m_Planes[FrustumPlane_Right]; }
	const Plane& GetTopPlane() const { return m_Planes[FrustumPlane_Top]; }
	const Plane& GetBottomPlane() const { return m_Planes[FrustumPlane_Bottom]; }

	void Init(float fov, float aspect, float nearDistance, float farDistance);
	void SetFOV(float fov) { m_Fov = fov; Init(m_Fov, m_Aspect, m_Near, m_Far); }
	void SetAspect(float aspect) { m_Aspect = aspect; Init(m_Fov, m_Aspect, m_Near, m_Far); }
	void SetNear(float nearClip) { m_Near = nearClip; Init(m_Fov, m_Aspect, m_Near, m_Far); }
	void SetFar(float farClip) { m_Far = farClip; Init(m_Fov, m_Aspect, m_Near, m_Far); }
	bool IsInside(const Vector3 &point) const;
	bool IsInside(const Vector3 &point, float radius) const;

	void Render();

private:
	enum FrustumPlane
	{
		FrustumPlane_Near = 0,
		FrustumPlane_Far,
		FrustumPlane_Left,
		FrustumPlane_Right,
		FrustumPlane_Top,
		FrustumPlane_Bottom,
		NumPlanes
	};

	Plane m_Planes[NumPlanes];
	Vector3 m_NearClip[4];
	Vector3 m_FarClip[4];

	float m_Fov;
	float m_Aspect;
	float m_Near;
	float m_Far;
};

class CameraNode : public SceneNode
{
public:
	CameraNode(const Frustum& frustum);
	virtual ~CameraNode();

	virtual HRESULT VOnRestore(Scene* pScene);
	virtual HRESULT VRender(Scene* pScene, double fTime, float fElapsedTime);
	virtual bool VIsVisible(Scene* pScene) const { return true; }

	const Frustum& GetFrustum() { return m_Frustum; }
	void SetTarget(shared_ptr<SceneNode> pTarget) { m_pTarget = pTarget; }
	shared_ptr<SceneNode> GetTarget() { return m_pTarget; }
	void ClearTarget() { m_pTarget = shared_ptr<SceneNode>(); }

	const Vector3& GetPosition() { return VGet()->GetPosition(); }
	const Matrix& GetViewMatrix() { return m_ViewMatrix; }
	const Matrix& GetProjectMatrix() { return m_ProjectionMatrix; }
	Matrix GetWorldViewProjection(Scene* pScene);
	void SetViewTransform(Scene* pScene);
	
	void SetCameraOffset(const Vector4& offsetVector) { m_OffsetVector = offsetVector; }

private:
	Frustum m_Frustum;
	bool m_IsDebugCamera;
	shared_ptr<SceneNode> m_pTarget;
	Vector4 m_OffsetVector;

	Matrix m_ViewMatrix;
	Matrix m_ProjectionMatrix;
};

