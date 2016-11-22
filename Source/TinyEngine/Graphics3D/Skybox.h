#pragma once
#include "Material.h"
#include "SceneNode.h"

class Scene;

class SkyboxMaterial : public Material
{
public:
	SkyboxMaterial();
	virtual ~SkyboxMaterial();

	virtual void Initialize(Effect* pEffect) override;
	virtual void CreateVertexBuffer(const Mesh* pMesh, ID3D11Buffer** ppVertexBuffer) const override;
	virtual void CreateVertexBuffer(XMFLOAT4* vertices, uint32_t vertexCount, ID3D11Buffer** ppVertexBuffer) const;
	virtual UINT VertexSize() const override;

	Variable& WorldViewProjection() { return *m_pWorldViewProjection; }
	Variable& SkyboxTexture() { return *m_pSkyboxTexutre; }

private:
	Variable* m_pWorldViewProjection;
	Variable* m_pSkyboxTexutre;
};

class SkyboxNode : public SceneNode
{
public:
	SkyboxNode(const std::wstring& textureFile);
	virtual ~SkyboxNode();

	virtual HRESULT VPreRender(Scene* pScene);
	virtual bool VIsVisible(Scene* pScene);

	virtual HRESULT VOnRestore(Scene* pScene);
	virtual HRESULT VRender(Scene* pScene, const GameTime& gameTime);
	virtual HRESULT VOnUpdate(Scene* pScene, const GameTime& gameTime);

private:
	void Reset();

	Effect* m_pEffect;
	SkyboxMaterial* m_pMaterial;
	ID3D11ShaderResourceView* m_pCubeMapShaderResourceView;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	Matrix m_WorldMatrix;
	Matrix m_ScaleMatrix;
	std::wstring m_TextureFile;
	UINT m_IndexCount;
	bool m_IsActive;
};
