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

	void LoadModer(const std::string& filePath, ProgressCallback callback);

protected:
	ModelImporter();
	virtual ~ModelImporter();

	virtual bool Update(float percentage = -1.f) override;

private:
	void WriteNode(const aiNode* sceneNode, tinyxml2::XMLDocument* xmlDoc, tinyxml2::XMLElement* xmlNode, uint32_t depth);

	std::unique_ptr<Assimp::Importer> m_AssimpImporter;
	ProgressCallback m_Callback;
};

