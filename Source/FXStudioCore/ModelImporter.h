#pragma once
#include "assimp/Importer.hpp"
#include "assimp/ProgressHandler.hpp"

typedef bool(*ProgressCallback)(float, const char*);

class ModelImporter : public Assimp::ProgressHandler
{
public:
	static ModelImporter* GetImporter();
	static void DeleteImporter();

	void LoadModer(const std::string& filePath, ProgressCallback callback);

protected:
	ModelImporter();
	~ModelImporter();

	virtual bool Update(float percentage = -1.f) override;

private:
	static ModelImporter* g_Importer;
	ProgressCallback m_Callback;
	Assimp::Importer* m_AssimpImporter;
};

