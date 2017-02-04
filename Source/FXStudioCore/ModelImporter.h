#pragma once
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/ProgressHandler.hpp"
#include "boost/noncopyable.hpp"
#include "tinyxml2.h"
#include <memory>

typedef bool(*ProgressCallback)(float, const char*);

class ModelImporter : public Assimp::ProgressHandler, public boost::noncopyable
{
public:
	static ModelImporter& GetImporter();

	void LoadModel(const std::string& importPath, const std::string& exportPath, ProgressCallback callback);

protected:
	ModelImporter();
	virtual ~ModelImporter();

	virtual bool Update(float percentage = -1.f) override;

private:
	void ExportModel(const std::string& exportPath, const aiScene* pScene);
	void WriteNode(const aiNode* pSceneNode, std::ofstream& fs, uint32_t depth);
	void WriteAnimation(const aiScene* pScene, std::ofstream& fs);
	void WriteMesh(const aiScene* pScene, std::ofstream& fs);

	int FStreamPrintf(std::ofstream& fs, const char* format, ...);
	void ConvertName(aiString& out, const aiString& in);

	static const size_t BufferSize = 4096;

	std::unique_ptr<Assimp::Importer> m_AssimpImporter;
	ProgressCallback m_Callback;
};

