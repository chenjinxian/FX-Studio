#pragma once
#include "Material.h"

typedef struct _TextureMappingMaterialVertex
{
	XMFLOAT4 Position;
	XMFLOAT2 TextureCoordinates;

	_TextureMappingMaterialVertex() { }

	_TextureMappingMaterialVertex(XMFLOAT4 position, XMFLOAT2 textureCoordinates)
		: Position(position), TextureCoordinates(textureCoordinates) { }
} TextureMappingMaterialVertex;

class TextureMappingMaterial : public Material
{
public:
	TextureMappingMaterial();

	virtual void Initialize(Effect* pEffect) override;
	virtual void CreateVertexBuffer(const Mesh* pMesh, ID3D11Buffer** ppVertexBuffer) const override;
	void CreateVertexBuffer(TextureMappingMaterialVertex* vertices, uint32_t vertexCount, ID3D11Buffer** ppVertexBuffer) const;
	virtual UINT VertexSize() const override;

	Variable& GetWorldViewProjection() { return *m_pWorldViewProjection; }
	Variable& GetColorTexture() { return *m_pColorTexture; }

private:
	Variable* m_pWorldViewProjection;
	Variable* m_pColorTexture;
};
