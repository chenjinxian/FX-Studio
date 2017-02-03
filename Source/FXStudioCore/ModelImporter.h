#pragma once
#include "assimp/Importer.hpp"
#include "assimp/ProgressHandler.hpp"
#include "boost/noncopyable.hpp"
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
	Assimp::Importer m_AssimpImporter;
	ProgressCallback m_Callback;
};

