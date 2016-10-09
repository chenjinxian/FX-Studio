#pragma once
#include "../RenderEngineBase.h"
#include "../RenderEngineInterface.h"

#include <Tlhelp32.h>
#include "dbghelp.h"
#include <tchar.h>

class MiniDumper
{
protected:
	static MiniDumper *gpDumper;
	static LONG WINAPI Handler( struct _EXCEPTION_POINTERS *pExceptionInfo );

	_EXCEPTION_POINTERS *m_pExceptionInfo;
	time_t m_lTime;

	TCHAR m_szDumpPath[_MAX_PATH];
	TCHAR m_szAppPath[_MAX_PATH];
	TCHAR m_szAppBaseName[_MAX_PATH];
	LONG WriteMiniDump(_EXCEPTION_POINTERS *pExceptionInfo );
	BOOL m_bHeadless;

	virtual void VSetDumpFileName(void);
	virtual MINIDUMP_USER_STREAM_INFORMATION *VGetUserStreamArray() { return NULL; }
	virtual const TCHAR *VGetUserMessage() { return _T(""); }

public:
	MiniDumper(bool headless);
};

class ModuleEntry
{
protected:
	MODULEENTRY32 m_me32;
	TCHAR *m_pFileVersionInfo;
	TCHAR *m_pProductVersion;

	void ReadFileVersionInfo(void);
	void FindProductVersion(void);

public:
	ModuleEntry(const MODULEENTRY32 &me);
	const MODULEENTRY32 *Get() const { return &m_me32; }
	const TCHAR *GetProductVersion() const { return m_pProductVersion; }
	virtual ~ModuleEntry();
};

#define USER_DATA_BUFFER_SIZE (4096)

