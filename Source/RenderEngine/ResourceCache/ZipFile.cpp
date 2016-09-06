#include <cctype>

#include "ZipFile.h"

#include <zlib.h>
#include <string.h>


// --------------------------------------------------------------------------
// Basic types.
// --------------------------------------------------------------------------
typedef unsigned long dword;
typedef unsigned short word;
typedef unsigned char byte;

// --------------------------------------------------------------------------
// ZIP file structures. Note these have to be packed.
// --------------------------------------------------------------------------

#pragma pack(1)
// --------------------------------------------------------------------------
// struct ZipFile::TZipLocalHeader					- Chapter 8, page 215
// --------------------------------------------------------------------------
struct ZipFile::TZipLocalHeader
{
  enum
  {
	SIGNATURE = 0x04034b50,
  };
  dword   sig;
  word    version;
  word    flag;
  word    compression;      // Z_NO_COMPRESSION or Z_DEFLATED
  word    modTime;
  word    modDate;
  dword   crc32;
  dword   cSize;
  dword   ucSize;
  word    fnameLen;         // Filename string follows header.
  word    xtraLen;          // Extra field follows filename.
};

// --------------------------------------------------------------------------
// struct ZipFile::TZipDirHeader					- Chapter 8, page 215
// --------------------------------------------------------------------------
struct ZipFile::TZipDirHeader
{
  enum
  {
	SIGNATURE = 0x06054b50
  };
  dword   sig;
  word    nDisk;
  word    nStartDisk;
  word    nDirEntries;
  word    totalDirEntries;
  dword   dirSize;
  dword   dirOffset;
  word    cmntLen;
};

// --------------------------------------------------------------------------
// struct ZipFile::TZipDirFileHeader					- Chapter 8, page 215
// --------------------------------------------------------------------------
struct ZipFile::TZipDirFileHeader
{
  enum
  {
	SIGNATURE   = 0x02014b50
  };
  dword   sig;
  word    verMade;
  word    verNeeded;
  word    flag;
  word    compression;      // COMP_xxxx
  word    modTime;
  word    modDate;
  dword   crc32;
  dword   cSize;            // Compressed size
  dword   ucSize;           // Uncompressed size
  word    fnameLen;         // Filename string follows header.
  word    xtraLen;          // Extra field follows filename.
  word    cmntLen;          // Comment field follows extra field.
  word    diskStart;
  word    intAttr;
  dword   extAttr;
  dword   hdrOffset;

  char *GetName   () const { return (char *)(this + 1);   }
  char *GetExtra  () const { return GetName() + fnameLen; }
  char *GetComment() const { return GetExtra() + xtraLen; }
};

#pragma pack()

// --------------------------------------------------------------------------
// Function:      Init
// Purpose:       Initialize the object and read the zip file directory.
// Parameters:    A stdio FILE* used for reading.
// --------------------------------------------------------------------------
bool ZipFile::Init(const std::wstring &resFileName)
{
  End();

  _wfopen_s(&m_pFile, resFileName.c_str(), _T("rb"));
  if (!m_pFile)
	return false;

  // Assuming no extra comment at the end, read the whole end record.
  TZipDirHeader dh;

  fseek(m_pFile, -(int)sizeof(dh), SEEK_END);
  long dhOffset = ftell(m_pFile);
  memset(&dh, 0, sizeof(dh));
  fread(&dh, sizeof(dh), 1, m_pFile);

  // Check
  if (dh.sig != TZipDirHeader::SIGNATURE)
	return false;

  // Go to the beginning of the directory.
  fseek(m_pFile, dhOffset - dh.dirSize, SEEK_SET);

  // Allocate the data buffer, and read the whole thing.
  m_pDirData = GCC_NEW char[dh.dirSize + dh.nDirEntries*sizeof(*m_papDir)];
  if (!m_pDirData)
	return false;
  memset(m_pDirData, 0, dh.dirSize + dh.nDirEntries*sizeof(*m_papDir));
  fread(m_pDirData, dh.dirSize, 1, m_pFile);

  // Now process each entry.
  char *pfh = m_pDirData;
  m_papDir = (const TZipDirFileHeader **)(m_pDirData + dh.dirSize);

  bool success = true;

  for (int i = 0; i < dh.nDirEntries && success; i++)
  {
	TZipDirFileHeader &fh = *(TZipDirFileHeader*)pfh;

	// Store the address of nth file for quicker access.
	m_papDir[i] = &fh;

	// Check the directory entry integrity.
	if (fh.sig != TZipDirFileHeader::SIGNATURE)
	  success = false;
	else
	{
	  pfh += sizeof(fh);

	  // Convert UNIX slashes to DOS backlashes.
	  for (int j = 0; j < fh.fnameLen; j++)
		if (pfh[j] == '/')
		  pfh[j] = '\\';

	  char fileName[_MAX_PATH];
	  memcpy(fileName, pfh, fh.fnameLen);
	  fileName[fh.fnameLen]=0;
	  _strlwr_s(fileName, _MAX_PATH);
	  std::string spath = fileName;
	  m_ZipContentsMap[spath] = i;

	  // Skip name, extra and comment fields.
	  pfh += fh.fnameLen + fh.xtraLen + fh.cmntLen;
	}
  }
  if (!success)
  {
	SAFE_DELETE_ARRAY(m_pDirData);
  }
  else
  {
	m_nEntries = dh.nDirEntries;
  }

  return success;
}

int ZipFile::Find(const std::string &path) const
{
	std::string lowerCase = path;
	std::transform(lowerCase.begin(), lowerCase.end(), lowerCase.begin(), (int(*)(int)) std::tolower);
	ZipContentsMap::const_iterator i = m_ZipContentsMap.find(lowerCase);
	if (i==m_ZipContentsMap.end())
		return -1;

	return i->second;
}



// --------------------------------------------------------------------------
// Function:      End
// Purpose:       Finish the object
// Parameters:    
// --------------------------------------------------------------------------
void ZipFile::End()
{
	m_ZipContentsMap.clear();
	SAFE_DELETE_ARRAY(m_pDirData);
	m_nEntries = 0;
}

// --------------------------------------------------------------------------
// Function:      GetFilename
// Purpose:       Return the name of a file
// Parameters:    The file index and the buffer where to store the filename
// --------------------------------------------------------------------------
std::string ZipFile::GetFilename(int i)  const
{
	std::string fileName = "";
	if (i >=0 && i < m_nEntries)
	{
	  char pszDest[_MAX_PATH];
	  memcpy(pszDest, m_papDir[i]->GetName(), m_papDir[i]->fnameLen);
	  pszDest[m_papDir[i]->fnameLen] = '\0';
	  fileName = pszDest;
	}
	return fileName;
}


// --------------------------------------------------------------------------
// Function:      GetFileLen
// Purpose:       Return the length of a file so a buffer can be allocated
// Parameters:    The file index.
// --------------------------------------------------------------------------
int ZipFile::GetFileLen(int i) const
{
  if (i < 0 || i >= m_nEntries)
	return -1;
  else
	return m_papDir[i]->ucSize;
}

// --------------------------------------------------------------------------
// Function:      ReadFile
// Purpose:       Uncompress a complete file
// Parameters:    The file index and the pre-allocated buffer
// --------------------------------------------------------------------------
bool ZipFile::ReadFile(int i, void *pBuf)
{
  if (pBuf == NULL || i < 0 || i >= m_nEntries)
	return false;

  // Quick'n dirty read, the whole file at once.
  // Ungood if the ZIP has huge files inside

  // Go to the actual file and read the local header.
  fseek(m_pFile, m_papDir[i]->hdrOffset, SEEK_SET);
  TZipLocalHeader h;

  memset(&h, 0, sizeof(h));
  fread(&h, sizeof(h), 1, m_pFile);
  if (h.sig != TZipLocalHeader::SIGNATURE)
	return false;

  // Skip extra fields
  fseek(m_pFile, h.fnameLen + h.xtraLen, SEEK_CUR);

  if (h.compression == Z_NO_COMPRESSION)
  {
	// Simply read in raw stored data.
	fread(pBuf, h.cSize, 1, m_pFile);
	return true;
  }
  else if (h.compression != Z_DEFLATED)
	return false;

  // Alloc compressed data buffer and read the whole stream
  char *pcData = GCC_NEW char[h.cSize];
  if (!pcData)
	return false;

  memset(pcData, 0, h.cSize);
  fread(pcData, h.cSize, 1, m_pFile);

  bool ret = true;

  // Setup the inflate stream.
  z_stream stream;
  int err;

  stream.next_in = (Bytef*)pcData;
  stream.avail_in = (uInt)h.cSize;
  stream.next_out = (Bytef*)pBuf;
  stream.avail_out = h.ucSize;
  stream.zalloc = (alloc_func)0;
  stream.zfree = (free_func)0;

  // Perform inflation. wbits < 0 indicates no zlib header inside the data.
  err = inflateInit2(&stream, -MAX_WBITS);
  if (err == Z_OK)
  {
	err = inflate(&stream, Z_FINISH);
	inflateEnd(&stream);
	if (err == Z_STREAM_END)
	  err = Z_OK;
	inflateEnd(&stream);
  }
  if (err != Z_OK)
	ret = false;

  delete[] pcData;
  return ret;
}



// --------------------------------------------------------------------------
// Function:      ReadLargeFile
// Purpose:       Uncompress a complete file with callbacks.
// Parameters:    The file index and the pre-allocated buffer
// --------------------------------------------------------------------------
bool ZipFile::ReadLargeFile(int i, void *pBuf, void (*progressCallback)(int, bool &))
{
  if (pBuf == NULL || i < 0 || i >= m_nEntries)
	return false;

  // Quick'n dirty read, the whole file at once.
  // Ungood if the ZIP has huge files inside

  // Go to the actual file and read the local header.
  fseek(m_pFile, m_papDir[i]->hdrOffset, SEEK_SET);
  TZipLocalHeader h;

  memset(&h, 0, sizeof(h));
  fread(&h, sizeof(h), 1, m_pFile);
  if (h.sig != TZipLocalHeader::SIGNATURE)
	return false;

  // Skip extra fields
  fseek(m_pFile, h.fnameLen + h.xtraLen, SEEK_CUR);

  if (h.compression == Z_NO_COMPRESSION)
  {
	// Simply read in raw stored data.
	fread(pBuf, h.cSize, 1, m_pFile);
	return true;
  }
  else if (h.compression != Z_DEFLATED)
	return false;

  // Alloc compressed data buffer and read the whole stream
  char *pcData = GCC_NEW char[h.cSize];
  if (!pcData)
	return false;

  memset(pcData, 0, h.cSize);
  fread(pcData, h.cSize, 1, m_pFile);

  bool ret = true;

  // Setup the inflate stream.
  z_stream stream;
  int err;

  stream.next_in = (Bytef*)pcData;
  stream.avail_in = (uInt)h.cSize;
  stream.next_out = (Bytef*)pBuf;
  stream.avail_out = (128 * 1024); //  read 128k at a time h.ucSize;
  stream.zalloc = (alloc_func)0;
  stream.zfree = (free_func)0;

  // Perform inflation. wbits < 0 indicates no zlib header inside the data.
  err = inflateInit2(&stream, -MAX_WBITS);
  if (err == Z_OK)
  {
	  uInt count = 0;
	  bool cancel = false;
		while (stream.total_in < (uInt)h.cSize && !cancel)
		{
			err = inflate(&stream, Z_SYNC_FLUSH);
			if (err == Z_STREAM_END)
			{
				err = Z_OK;
				break;
			}
			else if (err != Z_OK)
			{
				GCC_ASSERT(0 && "Something happened.");
				break;
			}

			stream.avail_out = (128 * 1024); 
			stream.next_out += stream.total_out;

			progressCallback(count * 100 / h.cSize, cancel);
		}
		inflateEnd(&stream);
  }
  if (err != Z_OK)
	ret = false;

  delete[] pcData;
  return ret;
}
