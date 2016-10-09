#pragma once
#include "../RenderEngineBase.h"
#include "../RenderEngineInterface.h"

typedef std::map<std::string, int> ZipContentsMap;		// maps path to a zip content id

class ZipFile
{
  public:
	ZipFile() { m_nEntries=0; m_pFile=NULL; m_pDirData=NULL; }
	virtual ~ZipFile() { End(); fclose(m_pFile); }

	bool Init(const std::wstring &resFileName);
	void End();

	int GetNumFiles()const { return m_nEntries; }
	std::string GetFilename(int i) const;	
	int GetFileLen(int i) const;
	bool ReadFile(int i, void *pBuf);

	// Added to show multi-threaded decompression
	bool ReadLargeFile(int i, void *pBuf, void (*progressCallback)(int, bool &));

	int Find(const std::string &path) const;

	ZipContentsMap m_ZipContentsMap;

  private:
	struct TZipDirHeader;
	struct TZipDirFileHeader;
	struct TZipLocalHeader;

	FILE *m_pFile;		// Zip file
	char *m_pDirData;	// Raw data buffer.
	int  m_nEntries;	// Number of entries.

	// Pointers to the dir entries in pDirData.
	const TZipDirFileHeader **m_papDir;   
};


