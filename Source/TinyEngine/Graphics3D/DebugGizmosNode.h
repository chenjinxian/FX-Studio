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

	enum PickedTransform
	{
		PT_None = 0,
		PT_TranslateX,
		PT_TranslateY,
		PT_TranslateZ,
		PT_RotateX,
		PT_RotateY,
		PT_RotateZ,
		PT_ScaleX,
		PT_ScaleY,
		PT_ScaleZ
	};

	void SetTransformType(int type) { m_Transform = (DebugGizmosNode::TransformType)type; }
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

	bool IsXAxisPicked(Scene* pScene, const Matrix& world);
	Vector3 IntersectRayPlane(Scene* pScene, const Matrix& world);

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

	TransformType m_Transform;
	Vector2 m_MousePos;
	PickedTransform m_PickedTransform;
	Vector3 m_Translate;
	bool m_IsLButtonClick;
	bool m_IsLButtonDown;
};
