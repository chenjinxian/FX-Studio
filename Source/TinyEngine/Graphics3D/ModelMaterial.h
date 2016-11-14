#pragma once
#include "../TinyEngineBase.h"
#include "../TinyEngineInterface.h"

struct aiMaterial;

enum TextureType
{
	TextureTypeDifffuse = 0,
	TextureTypeSpecularMap,
	TextureTypeAmbient,
	TextureTypeEmissive,
	TextureTypeHeightmap,
	TextureTypeNormalMap,
	TextureTypeSpecularPowerMap,
	TextureTypeDisplacementMap,
	TextureTypeLightMap,
	TextureTypeEnd
};

class ModelMaterial : public boost::noncopyable
{
	friend class Model;

public:
	ModelMaterial(Model& model, aiMaterial* material);
	~ModelMaterial();

	Model& GetModel();
	const std::string& Name() const;
	const std::map<TextureType, std::vector<std::wstring>*> Textures() const;

private:
	static void InitializeTextureTypeMappings();
	static std::map<TextureType, UINT> sTextureTypeMappings;

	Model& mModel;
	std::string mName;
	std::map<TextureType, std::vector<std::wstring>*> mTextures;
};
