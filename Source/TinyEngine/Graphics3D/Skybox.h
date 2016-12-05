#pragma once
#include "Material.h"
#include "SceneNode.h"

class Scene;

class SkyboxNode : public SceneNode
{
public:
	SkyboxNode(const std::wstring& textureFile);
	virtual ~SkyboxNode();

	virtual HRESULT VPreRender(Scene* pScene);
	virtual bool VIsVisible(Scene* pScene);

	virtual HRESULT VOnInitSceneNode(Scene* pScene) override;
	virtual HRESULT VOnDeleteSceneNode(Scene *pScene) override;
	virtual HRESULT VRender(Scene* pScene, const GameTime& gameTime) override;
	virtual HRESULT VOnUpdate(Scene* pScene, const GameTime& gameTime) override;

private:
	void Reset();

	Effect* m_pEffect;
	ID3D11ShaderResourceView* m_pCubeMapShaderResourceView;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	Matrix m_WorldMatrix;
	Matrix m_ScaleMatrix;
	std::wstring m_TextureFile;
	UINT m_IndexCount;
	bool m_IsActive;
};
