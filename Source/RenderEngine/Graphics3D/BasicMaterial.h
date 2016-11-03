#pragma once
#include "../RenderEngineBase.h"
#include "../RenderEngineInterface.h"
#include "Material.h"

typedef struct _BasicMaterialVertex
{
	XMFLOAT4 Position;
	XMFLOAT4 Color;

	_BasicMaterialVertex() { }

	_BasicMaterialVertex(XMFLOAT4 position, XMFLOAT4 color)
		: Position(position), Color(color) { }
} BasicMaterialVertex;

class BasicMaterial : public Material
{
public:
	BasicMaterial();

	virtual void Initialize(Effect* effect) override;
	virtual void CreateVertexBuffer(const Mesh* pMesh, ID3D11Buffer** ppVertexBuffer) const override;
	void CreateVertexBuffer(BasicMaterialVertex* vertices, UINT vertexCount, ID3D11Buffer** ppVertexBuffer) const;
	virtual uint32_t VertexSize() const override;

	Variable& GetWorldViewProjection() { return *m_pWorldViewProjection; }

private:
	Variable* m_pWorldViewProjection;
};
