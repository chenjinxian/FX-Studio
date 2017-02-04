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

	void LoadModer(const std::string& importPath, const std::string& exportPath, ProgressCallback callback);

protected:
	ModelImporter();
	virtual ~ModelImporter();

	virtual bool Update(float percentage = -1.f) override;

private:
	void WriteNode(const aiNode* pSceneNode, tinyxml2::XMLDocument* pXmlDoc, tinyxml2::XMLElement* pXmlNode, uint32_t depth);
	void WriteAnimation(const aiScene* pScene, tinyxml2::XMLDocument* pXmlDoc, tinyxml2::XMLElement* pXmlNode);
	void WriteMesh(const aiScene* pScene, tinyxml2::XMLDocument* pXmlDoc, tinyxml2::XMLElement* pXmlNode);

	std::unique_ptr<Assimp::Importer> m_AssimpImporter;
	ProgressCallback m_Callback;
};

