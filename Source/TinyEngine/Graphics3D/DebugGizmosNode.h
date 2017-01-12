#pragma once
#include "SceneNode.h"

class DebugGizmosNode : public SceneNode
{
public:
	DebugGizmosNode();
	~DebugGizmosNode();

	virtual HRESULT VOnInitSceneNode(Scene* pScene) override;
	virtual HRESULT VOnDeleteSceneNode(Scene *pScene) override;
	virtual HRESULT VOnUpdate(Scene* pScene, const GameTime& gameTime) override;
	virtual HRESULT VRender(Scene* pScene, const GameTime& gameTime) override;
	virtual bool VIsVisible(Scene* pScene) const override;

	enum TransformType
	{
		TT_None = 0,
		TT_Translation,
		TT_Rotation,
		TT_Scale
	};

	enum TransformAxis
	{
		TA_None = 0,
		TA_AxisX,
		TA_AxisY,
		TA_AxisZ
	};

	bool IsPicked() { return (m_Axis != TA_None); }

	void SetTransformType(int type) { m_Type = (DebugGizmosNode::TransformType)type; }
	void PointerLeftClick(const Vector2& pos);
	void PointerMove(const Vector2 &pos, bool leftButtonDown);

private:
	HRESULT RenderBoundingBox(Scene* pScene, const BoundingBox& aaBox, const Matrix& world);
	HRESULT RenderTranslateAxes(Scene* pScene, const BoundingBox& aaBox, const Matrix& world);
	HRESULT RenderRotateRings(Scene* pScene, const BoundingBox& aaBox, const Matrix& world);
	HRESULT RenderScaleAxes(Scene* pScene, const BoundingBox& aaBox, const Matrix& world);

	void CreateAABox(std::vector<Vector3>& vertices, std::vector<uint16_t>& indices);
	void AddVertex(std::vector<Vector3>& vertices, const std::vector<struct VertexPositionNormalTexture>& inputVertices);
	void CreateGeometryBuffers();

	Ray CreateRay(Scene* pScene, const Matrix& world, bool inLocal = true);
	bool IsAxisPicked(const Ray& ray);
	bool IsTorusPicked(const Ray& ray);
	float IntersectRayPlane(const Plane& plane, const Ray& ray);
	Vector3 ComputeMouseOffset(Scene* pScene, const Matrix& world);
// 	Vector3 ComputeMouseOffset(Scene* pScene, const Plane& plane, const Ray& ray);
	float ComputeAngleOnPlane(const Vector3& newOffset, const Vector3& translation);

	Effect* m_pEffect;
	Pass* m_pCurrentPass;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;

	int32_t m_AABoxVertexOffset;
	int32_t m_CylinderVertexOffset;
	int32_t m_ConeVertexOffset;
	int32_t m_TorusVertexOffset;
	int32_t m_CubeVertexOffset;

	uint32_t m_AABoxIndexOffset;
	uint32_t m_CylinderIndexOffset;
	uint32_t m_ConeIndexOffset;
	uint32_t m_TorusIndexOffset;
	uint32_t m_CubeIndexOffset;

	uint32_t m_AABoxIndexCount;
	uint32_t m_CylinderIndexCount;
	uint32_t m_ConeIndexCount;
	uint32_t m_TorusIndexCount;
	uint32_t m_CubeIndexCount;

	std::vector<Vector3> m_TorusVertices;
	std::vector<uint16_t> m_TorusIndices;

	TransformType m_Type;
	TransformAxis m_Axis;
	Vector2 m_MousePos;
	Vector3 m_LastOffset;
	Vector3 m_LastTranslation;
	Vector3 m_LastScale;
	bool m_IsLButtonClick;
	bool m_IsLButtonDown;
};
