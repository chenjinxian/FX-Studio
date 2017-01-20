#pragma once
#include "../TinyEngineBase.h"
#include "../TinyEngineInterface.h"
#include "ZipFile.h"

class ResHandle;
class ResCache;

class IResourceExtraData
{
public:
	virtual std::string VToString() = 0;
};

class Resource
{
public:
	Resource(const std::string &name);
	std::string m_name;
};

class ResourceZipFile : public IResourceFile
{
public:
	ResourceZipFile();
	virtual ~ResourceZipFile();

	virtual bool VOpen() override;
	virtual int VGetRawResourceSize(const Resource &r) override;
	virtual int VGetRawResource(const Resource &r, char *buffer) override;
	virtual int VGetNumResources() const override;
	virtual std::string VGetResourceName(int num) const override;
	virtual bool VIsUsingDevelopmentDirectories(void) const  override { return false; }

private:
	unique_ptr<ZipFile> m_pZipFile;
	std::wstring m_ResFileName;
};

class DevelopmentResourceZipFile : public ResourceZipFile
{
public:
	enum Mode
	{
		Development,
		Editor
	};

	DevelopmentResourceZipFile(const std::string assetDir, const Mode mode);

	virtual bool VOpen();
	virtual int VGetRawResourceSize(const Resource &r);
	virtual int VGetRawResource(const Resource &r, char *buffer);
	virtual int VGetNumResources() const;
	virtual std::string VGetResourceName(int num) const;
	virtual bool VIsUsingDevelopmentDirectories(void) const { return true; }

	int Find(const std::string &path);

	Mode m_Mode;
	std::string m_AssetsDir;
	std::vector<WIN32_FIND_DATA> m_AssetFileInfo;
	ZipContentsMap m_DirectoryContentsMap;

protected:
	void ReadAssetsDirectory(std::wstring fileSpec);

private:
	bool AddNewResFile(const std::string& filePath);
};

class ResHandle
{
	friend class ResCache;

public:
	ResHandle(Resource& resource, char* buffer, uint32_t size, ResCache* pResCache);

	virtual ~ResHandle();

	const std::string GetName() { return m_Resource.m_name; }
	uint32_t Size() const { return m_Size; }
	char* Buffer() const { return m_pBuffer; }
	char* WritableBuffer() { return m_pBuffer; }

	shared_ptr<IResourceExtraData> GetExtraData() { return m_pExtraData; }
	void SetExtraData(shared_ptr<IResourceExtraData> extra) { m_pExtraData = extra; }

protected:
	Resource m_Resource;
	char* m_pBuffer;
	uint32_t m_Size;
	shared_ptr<IResourceExtraData> m_pExtraData;
	ResCache* m_pResCache;
};

class DefaultResourceLoader : public IResourceLoader
{
public:
	virtual bool VUseRawFile() { return true; }
	virtual bool VDiscardRawBufferAfterLoad() { return true; }
	virtual uint32_t VGetLoadedResourceSize(char* rawBuffer, uint32_t rawSize) { return rawSize; }
	virtual bool VLoadResource(char* rawBuffer, uint32_t rawSize, shared_ptr<ResHandle> handle) { return true; }
	virtual std::string VGetPattern() { return "*"; }

};

typedef std::list< shared_ptr <ResHandle > > ResHandleList;
typedef std::map<std::string, shared_ptr < ResHandle  > > ResHandleMap;
typedef std::list< shared_ptr < IResourceLoader > > ResourceLoaders;

class ResCache
{
	friend class ResHandle;

public:
	ResCache(const uint32_t sizeInMb, const std::string& assetDir, bool isZipResource);
	virtual ~ResCache();

	bool Init();

	void RegisterLoader(shared_ptr<IResourceLoader> loader);

	shared_ptr<ResHandle> GetHandle(Resource * r);

	int Preload(const std::string pattern, void(*progressCallback)(int, bool &));
	std::vector<std::string> Match(const std::string pattern);

	void Flush(void);

	bool IsUsingDevelopmentDirectories(void) const { DEBUG_ASSERT(m_pResFile); return m_pResFile->VIsUsingDevelopmentDirectories(); }

protected:

	bool MakeRoom(uint32_t size);
	char *Allocate(uint32_t size);
	void Free(shared_ptr<ResHandle> gonner);

	shared_ptr<ResHandle> Load(Resource * r);
	shared_ptr<ResHandle> Find(Resource * r);
	void Update(shared_ptr<ResHandle> handle);

	void FreeOneResource();
	void MemoryHasBeenFreed(uint32_t size);

private:
	ResHandleList m_ResList;
	ResHandleMap m_ResMap;
	ResourceLoaders m_ResourceLoaders;

	unique_ptr<IResourceFile> m_pResFile;

	uint32_t m_CacheSize;
	uint32_t m_Allocated;
};

shared_ptr<IResourceLoader> CreateDdsResourceLoader();
shared_ptr<IResourceLoader> CreateJpgResourceLoader();
shared_ptr<IResourceLoader> CreatePngResourceLoader();
shared_ptr<IResourceLoader> CreateBmpResourceLoader();
shared_ptr<IResourceLoader> CreateTiffResourceLoader();
shared_ptr<IResourceLoader> CreateXmlResourceLoader();
shared_ptr<IResourceLoader> CreateFxEffectResourceLoader();
shared_ptr<IResourceLoader> CreateCsoEffectResourceLoader();
