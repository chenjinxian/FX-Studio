#pragma once
#include "../TinyEngine/TinyEngine.h"

class MaterialNode : public boost::noncopyable
{
public:
	MaterialNode(const std::string& materialName);
	~MaterialNode();

	HRESULT OnInitSceneNode();
	HRESULT OnDeleteSceneNode();
	HRESULT OnUpdate(const GameTime& gameTime);
	HRESULT Render(const GameTime& gameTime);

private:
	Effect* m_pEffect;
	Pass* m_pCurrentPass;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	uint32_t m_IndexCount;
	std::unique_ptr<Mesh> m_Mesh;

	Matrix m_World;
	Matrix m_View;
	Matrix m_Project;
	std::string m_MaterialName;
};

