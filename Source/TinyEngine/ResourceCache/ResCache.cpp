#include "ResCache.h"
#include "boost/optional.hpp"
#include <cctype>

Resource::Resource(const std::string &name)
{
	m_name = name;
	std::transform(m_name.begin(), m_name.end(), m_name.begin(), (int(*)(int)) std::tolower);
}

ResourceZipFile::ResourceZipFile()
	: m_pZipFile(nullptr),
	m_ResFileName(L"Assets.zip")
{

}

ResourceZipFile::~ResourceZipFile()
{

}

bool ResourceZipFile::VOpen()
{
	m_pZipFile = unique_ptr<ZipFile>(DEBUG_NEW ZipFile);
	if (m_pZipFile != nullptr)
	{
		return m_pZipFile->Init(m_ResFileName.c_str());
	}
	return false;
}

int ResourceZipFile::VGetRawResourceSize(const Resource &r)
{
	int resourceNum = m_pZipFile->Find(r.m_name.c_str());
	if (resourceNum == -1)
		return -1;

	return m_pZipFile->GetFileLen(resourceNum);
}

int ResourceZipFile::VGetRawResource(const Resource &r, char *buffer)
{
	int size = 0;
	boost::optional<int> resourceNum = m_pZipFile->Find(r.m_name.c_str());
	if (resourceNum.is_initialized())
	{
		size = m_pZipFile->GetFileLen(resourceNum.get());
		m_pZipFile->ReadFile(resourceNum.get(), buffer);
	}
	return size;
}

int ResourceZipFile::VGetNumResources() const
{
	return (m_pZipFile == nullptr) ? 0 : m_pZipFile->GetNumFiles();
}

std::string ResourceZipFile::VGetResourceName(int num) const
{
	std::string resName = "";
	if (m_pZipFile != nullptr && num >= 0 && num < m_pZipFile->GetNumFiles())
	{
		resName = m_pZipFile->GetFilename(num);
	}
	return resName;
}

DevelopmentResourceZipFile::DevelopmentResourceZipFile(const std::string assetDir, const Mode mode)
	: ResourceZipFile(),
	m_Mode(mode),
	m_AssetsDir(assetDir)
{

}

int DevelopmentResourceZipFile::Find(const std::string &name)
{
// 	std::string lowerCase = name;
// 	std::transform(lowerCase.begin(), lowerCase.end(), lowerCase.begin(), (int(*)(int)) std::tolower);
	ZipContentsMap::const_iterator i = m_DirectoryContentsMap.find(name);
	if (i == m_DirectoryContentsMap.end())
	{
		// In editor mode, allowed to add new resource file
		if (AddNewResFile(name))
		{
			return m_AssetFileInfo.size() - 1;
		}
		else
			return -1;
	}

	return i->second;
}

bool DevelopmentResourceZipFile::VOpen()
{
	if (m_Mode != Editor)
	{
		ResourceZipFile::VOpen();
	}

	// open the asset directory and read in the non-hidden contents
	if (m_Mode == Editor)
	{
		ReadAssetsDirectory(L"*");
	}
	else
	{
		// FUTURE WORK - iterate through the ZipFile contents and go grab WIN32_FIND_DATA 
		//   elements for each one. Then it would be possible to compare the dates/times of the
		//   asset in the Zip file with the source asset.
		DEBUG_ASSERT(0 && "Not implemented yet");
	}

	return true;
}


void DevelopmentResourceZipFile::VRemoveRawResource(const Resource &r)
{
	ZipContentsMap::const_iterator i = m_DirectoryContentsMap.find(r.m_name);
	if (i != m_DirectoryContentsMap.end())
	{
		m_AssetFileInfo.erase(m_AssetFileInfo.begin() + i->second - 1);
	}
	m_DirectoryContentsMap.erase(r.m_name);
}

bool DevelopmentResourceZipFile::AddNewResFile(const std::string& filePath)
{
	bool success = false;
	HANDLE fileHandle;
	WIN32_FIND_DATA findData;

	std::wstring pathSpec = Utility::S2WS(m_AssetsDir + filePath);
	fileHandle = FindFirstFile(pathSpec.c_str(), &findData);
	if (fileHandle != INVALID_HANDLE_VALUE)
	{
		std::string lower = filePath;
		std::transform(lower.begin(), lower.end(), lower.begin(), (int(*)(int)) std::tolower);
		wcscpy_s(&findData.cFileName[0], MAX_PATH, Utility::S2WS(lower).c_str());
		m_DirectoryContentsMap[lower] = m_AssetFileInfo.size();
		m_AssetFileInfo.push_back(findData);
		success = true;
	}

	FindClose(fileHandle);

	return success;
}

int DevelopmentResourceZipFile::VGetRawResourceSize(const Resource &r)
{
	int size = 0;

	if (m_Mode == Editor)
	{
		int num = Find(r.m_name.c_str());
		if (num == -1)
			return -1;

		return m_AssetFileInfo[num].nFileSizeLow;
	}

	return ResourceZipFile::VGetRawResourceSize(r);
}

int DevelopmentResourceZipFile::VGetRawResource(const Resource &r, char *buffer)
{
	if (m_Mode == Editor)
	{
		int num = Find(r.m_name.c_str());
		if (num == -1)
			return -1;

		std::string fullFileSpec = m_AssetsDir + r.m_name.c_str();
		FILE *f = nullptr;
		fopen_s(&f, fullFileSpec.c_str(), "rb");
		size_t bytes = fread(buffer, 1, m_AssetFileInfo[num].nFileSizeLow, f);
		fclose(f);
		return bytes;
	}

	return ResourceZipFile::VGetRawResource(r, buffer);
}

int DevelopmentResourceZipFile::VGetNumResources() const
{
	return (m_Mode == Editor) ? m_AssetFileInfo.size() : ResourceZipFile::VGetNumResources();
}

std::string DevelopmentResourceZipFile::VGetResourceName(int num) const
{
	if (m_Mode == Editor)
	{
		std::wstring wideName = m_AssetFileInfo[num].cFileName;
		return Utility::WS2S(wideName);
	}

	return ResourceZipFile::VGetResourceName(num);
}


void DevelopmentResourceZipFile::ReadAssetsDirectory(std::wstring fileSpec)
{
	HANDLE fileHandle;
	WIN32_FIND_DATA findData;

	std::wstring pathSpec = Utility::S2WS(m_AssetsDir) + fileSpec;
	fileHandle = FindFirstFile(pathSpec.c_str(), &findData);
	if (fileHandle != INVALID_HANDLE_VALUE)
	{
		while (FindNextFile(fileHandle, &findData))
		{
			if (findData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
				continue;

			std::wstring fileName = findData.cFileName;
			if (findData.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY)
			{
				if (fileName != L".." && fileName != L".")
				{
					fileName = fileSpec.substr(0, fileSpec.length() - 1) + fileName + L"\\*";
					ReadAssetsDirectory(fileName);
				}
			}
			else
			{
				fileName = fileSpec.substr(0, fileSpec.length() - 1) + fileName;
				std::wstring lower = fileName;
				std::transform(lower.begin(), lower.end(), lower.begin(), (int(*)(int)) std::tolower);
				wcscpy_s(&findData.cFileName[0], MAX_PATH, lower.c_str());
				m_DirectoryContentsMap[Utility::WS2S(lower)] = m_AssetFileInfo.size();
				m_AssetFileInfo.push_back(findData);
			}
		}
	}

	FindClose(fileHandle);
}

ResHandle::ResHandle(Resource& resource, char* buffer, uint32_t size, ResCache* pResCache)
	: m_Resource(resource),
	m_pBuffer(buffer),
	m_Size(size),
	m_pExtraData(nullptr),
	m_pResCache(pResCache)
{

}

ResHandle::~ResHandle()
{
	SAFE_DELETE_ARRAY(m_pBuffer);
	m_pResCache->MemoryHasBeenFreed(m_Size);
}

ResCache::ResCache(const uint32_t sizeInMb, const std::string& assetDir, bool isZipResource)
	: m_CacheSize(sizeInMb * 1024 * 1024),
	m_Allocated(0)
{
	if (isZipResource)
	{
		m_pResFile = unique_ptr<IResourceFile>(DEBUG_NEW ResourceZipFile());
	}
	else
	{
		m_pResFile = unique_ptr<IResourceFile>(DEBUG_NEW DevelopmentResourceZipFile(assetDir + "\\", DevelopmentResourceZipFile::Editor));
	}
}

ResCache::~ResCache()
{
	while (!m_ResList.empty())
	{
		FreeOneResource();
	}
}

bool ResCache::Init()
{
	bool retValue = false;
	if (m_pResFile->VOpen())
	{
		RegisterLoader(shared_ptr<IResourceLoader>(DEBUG_NEW DefaultResourceLoader()));
		retValue = true;
	}
	return retValue;
}

void ResCache::RegisterLoader(shared_ptr<IResourceLoader> loader)
{
	m_ResourceLoaders.push_front(loader);
}

shared_ptr<ResHandle> ResCache::GetHandle(Resource* r)
{
	shared_ptr<ResHandle> handle(Find(r));
	if (handle == nullptr)
	{
		handle = Load(r);
		DEBUG_ASSERT(handle);
	}
	else
	{
		Update(handle);
	}
	return handle;
}

void ResCache::RemoveHandle(Resource* r)
{
	shared_ptr<ResHandle> handle(Find(r));
	if (handle != nullptr)
	{
		m_ResList.remove(handle);
		m_ResMap.erase(handle->m_Resource.m_name);
		m_pResFile->VRemoveRawResource(*r);
	}
}

shared_ptr<ResHandle> ResCache::Load(Resource* r)
{
	shared_ptr<IResourceLoader> loader;
	shared_ptr<ResHandle> handle;

	for (ResourceLoaders::iterator it = m_ResourceLoaders.begin(); it != m_ResourceLoaders.end(); ++it)
	{
		shared_ptr<IResourceLoader> testLoader = *it;

		if (Utility::WildcardMatch(testLoader->VGetPattern().c_str(), r->m_name.c_str()))
		{
			loader = testLoader;
			break;
		}
	}

	if (!loader)
	{
		DEBUG_ASSERT(loader && _T("Default resource loader not found!"));
		return handle;		// Resource not loaded!
	}

	int rawSize = m_pResFile->VGetRawResourceSize(*r);
	if (rawSize < 0)
	{
		DEBUG_ASSERT(rawSize > 0 && "Resource size returned -1 - Resource not found");
		return shared_ptr<ResHandle>();
	}

	int allocSize = rawSize + ((loader->VAddNullZero()) ? (1) : (0));
	char *rawBuffer = loader->VUseRawFile() ? Allocate(allocSize) : DEBUG_NEW char[allocSize];
	memset(rawBuffer, 0, allocSize);

	if (rawBuffer == nullptr || m_pResFile->VGetRawResource(*r, rawBuffer) == 0)
	{
		// resource cache out of memory
		return shared_ptr<ResHandle>();
	}

	char *buffer = nullptr;
	uint32_t size = 0;

	if (loader->VUseRawFile())
	{
		buffer = rawBuffer;
		handle = shared_ptr<ResHandle>(DEBUG_NEW ResHandle(*r, buffer, rawSize, this));
	}
	else
	{
		size = loader->VGetLoadedResourceSize(rawBuffer, rawSize);
		buffer = Allocate(size);
		if (rawBuffer == nullptr || buffer == nullptr)
		{
			// resource cache out of memory
			return shared_ptr<ResHandle>();
		}
		handle = shared_ptr<ResHandle>(DEBUG_NEW ResHandle(*r, buffer, size, this));
		bool success = loader->VLoadResource(rawBuffer, rawSize, handle);

		if (loader->VDiscardRawBufferAfterLoad())
		{
			SAFE_DELETE_ARRAY(rawBuffer);
		}

		if (!success)
		{
			return shared_ptr<ResHandle>();
		}
	}

	if (handle)
	{
		m_ResList.push_front(handle);
		m_ResMap[r->m_name] = handle;
	}

	DEBUG_ASSERT(loader && _T("Default resource loader not found!"));
	return handle;		// ResCache is out of memory!
}

shared_ptr<ResHandle> ResCache::Find(Resource * r)
{
	ResHandleMap::iterator i = m_ResMap.find(r->m_name);
	if (i == m_ResMap.end())
		return shared_ptr<ResHandle>();

	return i->second;
}

void ResCache::Update(shared_ptr<ResHandle> handle)
{
	// Move most recently used resource to the front, let least used resource clear first
	m_ResList.remove(handle);
	m_ResList.push_front(handle);
}

char *ResCache::Allocate(uint32_t size)
{
	if (!MakeRoom(size))
		return nullptr;

	char *mem = DEBUG_NEW char[size];
	if (mem)
	{
		m_Allocated += size;
	}

	return mem;
}

void ResCache::FreeOneResource()
{
	ResHandleList::iterator gonner = m_ResList.end();
	gonner--;

	shared_ptr<ResHandle> handle = *gonner;

	m_ResList.pop_back();
	m_ResMap.erase(handle->m_Resource.m_name);
}

void ResCache::Flush()
{
	while (!m_ResList.empty())
	{
		shared_ptr<ResHandle> handle = *(m_ResList.begin());
		Free(handle);
		m_ResList.pop_front();
	}
}

bool ResCache::MakeRoom(uint32_t size)
{
	if (size > m_CacheSize)
	{
		return false;
	}

	while (size > (m_CacheSize - m_Allocated))
	{
		if (m_ResList.empty())
			return false;

		FreeOneResource();
	}

	return true;
}

void ResCache::Free(shared_ptr<ResHandle> gonner)
{
	m_ResList.remove(gonner);
	m_ResMap.erase(gonner->m_Resource.m_name);
}

void ResCache::MemoryHasBeenFreed(uint32_t size)
{
	m_Allocated -= size;
}

std::vector<std::string> ResCache::Match(const std::string pattern)
{
	std::vector<std::string> matchingNames;
	if (m_pResFile == nullptr)
		return matchingNames;

	int numFiles = m_pResFile->VGetNumResources();
	for (int i = 0; i < numFiles; ++i)
	{
		std::string name = m_pResFile->VGetResourceName(i);
		std::transform(name.begin(), name.end(), name.begin(), (int(*)(int)) std::tolower);
		if (Utility::WildcardMatch(pattern.c_str(), name.c_str()))
		{
			matchingNames.push_back(name);
		}
	}
	return matchingNames;
}

int ResCache::Preload(const std::string pattern, void(*progressCallback)(int, bool &))
{
	if (m_pResFile == nullptr)
		return 0;

	int numFiles = m_pResFile->VGetNumResources();
	int loaded = 0;
	bool cancel = false;
	for (int i = 0; i < numFiles; ++i)
	{
		Resource resource(m_pResFile->VGetResourceName(i));

		if (Utility::WildcardMatch(pattern.c_str(), resource.m_name.c_str()))
		{
			shared_ptr<ResHandle> handle = GetHandle(&resource);
			++loaded;
		}

		if (progressCallback != nullptr)
		{
			progressCallback(i * 100 / numFiles, cancel);
		}
	}
	return loaded;
}
