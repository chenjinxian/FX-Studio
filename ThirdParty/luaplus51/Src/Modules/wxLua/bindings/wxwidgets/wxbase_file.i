// ===========================================================================
// Purpose:     wxFile, wxDir, wxFileName and file functions
// Author:      J Winwood, John Labenski
// Created:     14/11/2001
// Copyright:   (c) 2001-2002 Lomtick Software. All rights reserved.
// Licence:     wxWidgets licence
// wxWidgets:   Updated to 2.8.4
// ===========================================================================

#include "wx/filefn.h"
#include "sys/stat.h"

// global functions from the wxWindow's functions docs

bool wxDirExists(const wxString& dirname );
bool wxFileExists(const wxString& filename );

// %override [new Lua string] wxDos2UnixFilename(Lua string );
// C++ Func: void wxDos2UnixFilename(wxChar *s );
!%wxchkver_2_9_0 wxString wxDos2UnixFilename(const wxString& s );
// %override wxDateTime wxFileModificationTime(const wxString& filename) (not overridden, just return wxDateTime );
// C++ Func: time_t wxFileModificationTime(const wxString& filename );
wxDateTime wxFileModificationTime(const wxString& filename );
//wxString wxFileNameFromPath(const wxString& path); // obsolete use wxFileName::SplitPath
wxString wxFindFirstFile(const wxString& spec, int flags = 0 );
wxString wxFindNextFile( );
// bool wxGetDiskSpace(const wxString& path, wxLongLong *total = NULL, wxLongLong *free = NULL );
//wxFileKind wxGetFileKind(FILE* fd );
wxString wxGetOSDirectory( );
bool wxIsAbsolutePath(const wxString& filename );
wxString wxPathOnly(const wxString& path );
// %override [new Lua string] wxUnix2DosFilename(Lua string );
// C++ Func: void wxUnix2DosFilename(wxChar *s );
!%wxchkver_2_9 wxString wxUnix2DosFilename(const wxString& s );
bool wxConcatFiles(const wxString& file1, const wxString& file2,const wxString& file3 );
bool wxCopyFile(const wxString& file1, const wxString& file2, bool overwrite = true );
wxString wxGetCwd( );
//char* wxGetTempFileName(const wxString& prefix); // obsolete use wxFileName::CreateTempFileName
bool wxIsWild(const wxString& pattern );
bool wxMatchWild(const wxString& pattern, const wxString& text, bool dot_special );
bool wxMkdir(const wxString& dir, int perm = 0777 );
//int wxParseCommonDialogsFilter(const wxString& wildCard, wxArrayString& descriptions, wxArrayString& filters );
%wxchkver_2_8  wxString wxRealPath(const wxString& path );
bool wxRemoveFile(const wxString& file );
!%wxchkver_2_8 bool wxRenameFile(const wxString& file1, const wxString& file2 );
%wxchkver_2_8  bool wxRenameFile(const wxString& file1, const wxString& file2, bool overwrite = true );
bool wxRmdir(const wxString& dir, int flags=0 );
bool wxSetWorkingDirectory(const wxString& dir );

%wxchkver_2_8 bool wxIsWritable(const wxString &path );
%wxchkver_2_8 bool wxIsReadable(const wxString &path );
%wxchkver_2_8 bool wxIsExecutable(const wxString &path );

// These two methods are for wxLua
// %override long wxFileSize(const wxString& fileName) - gets the filesize
long wxFileSize(const wxString& fileName );

// wxLua only has storage for wxChar* in bindings, wxFILE_SEP_XXX are #defined
//   as wxChar wxT('.'), so we just redefine them to be wxT(".") or wxChar*
#define_wxstring wxFILE_SEP_EXT       wxT("." );
#define_wxstring wxFILE_SEP_DSK       wxT(":" );
#define_wxstring wxFILE_SEP_PATH_DOS  wxT("\\" );
#define_wxstring wxFILE_SEP_PATH_UNIX wxT("/" );
#define_wxstring wxFILE_SEP_PATH_MAC  wxT(":" );
#define_wxstring wxFILE_SEP_PATH_VMS  wxT("."); // VMS also uses '[' and ']'

#define_wxstring wxFILE_SEP_PATH wxLua_FILE_SEP_PATH // hack to convert from wxChar wxT('') to wxChar* wxT("" );

#define_wxstring wxPATH_SEP_DOS       // wxT(";" );
#define_wxstring wxPATH_SEP_UNIX      // wxT(":" );
#define_wxstring wxPATH_SEP_MAC       // wxT(";" );
#define_wxstring wxPATH_SEP           // wxPATH_SEP_XXX

#define wxARE_FILENAMES_CASE_SENSITIVE // bool 1/0

//bool wxIsPathSeparator(wxChar c) FIXME
bool wxEndsWithPathSeparator(const wxString& pszFileName );


// ---------------------------------------------------------------------------
// wxStandardPaths

#if %wxchkver_2_8 && wxLUA_USE_wxStandardPaths

#include "wx/stdpaths.h"

enum wxStandardPaths::ResourceCat
{
    ResourceCat_None,     // no special category
    ResourceCat_Messages, // message catalog resources
    ResourceCat_Max      // end of enum marker
};


class wxStandardPaths // we ignore wxStandardPathsBase
{
    // No constructor - use static Get() function

    // return the global standard paths object
    // %override static wxStandardPaths& Get();
    // C++ Func: static wxStandardPathsBase& Get();
    // We pretend that there is no wxStandardPathsBase and just use the wxStandardPaths name
    static wxStandardPaths& Get();

    // These are only for the generic version, probably not ever needed
    //void SetInstallPrefix(const wxString& prefix );
    //wxString GetInstallPrefix() const;

    virtual wxString GetExecutablePath() const;
    virtual wxString GetConfigDir() const;
    virtual wxString GetUserConfigDir() const;
    virtual wxString GetDataDir() const;
    virtual wxString GetLocalDataDir() const;
    virtual wxString GetUserDataDir() const;
    virtual wxString GetUserLocalDataDir() const;
    virtual wxString GetPluginsDir() const;
    virtual wxString GetResourcesDir() const;
    virtual wxString GetLocalizedResourcesDir(const wxString& lang, wxStandardPaths::ResourceCat category = wxStandardPaths::ResourceCat_None) const;
    virtual wxString GetDocumentsDir() const;
    virtual wxString GetTempDir() const;
};

#endif // %wxchkver_2_8 && wxLUA_USE_wxStandardPaths


// ---------------------------------------------------------------------------
// wxPathList

#include "wx/filefn.h"

class %delete wxPathList : public wxArrayString
{
    wxPathList( );
    //wxPathList(const wxArrayString &arr );

    // Adds all paths in environment variable
    void AddEnvList(const wxString& envVariable );
    // Adds given path to this list
    !%wxchkver_2_8 void Add(const wxString& path );
    %wxchkver_2_8 bool Add(const wxString& path );
    %wxchkver_2_8 void Add(const wxArrayString& paths );
    // Find the first full path for which the file exists
    wxString FindValidPath(const wxString& filename) const;
    // Find the first full path for which the file exists; ensure it's an
    // absolute path that gets returned.
    wxString FindAbsoluteValidPath(const wxString& filename) const;
    // Given full path and filename, add path to list
    %not_overload !%wxchkver_2_8 void EnsureFileAccessible(const wxString& path );
    %not_overload %wxchkver_2_8 bool EnsureFileAccessible(const wxString& path );
};

// ---------------------------------------------------------------------------
// wxFileName

#if wxLUA_USE_wxFileName

#include "wx/filename.h"

#define wxPATH_GET_VOLUME
#define wxPATH_GET_SEPARATOR
#define wxPATH_MKDIR_FULL

#define wxFILE
#define wxDIR

enum wxPathFormat
{
    wxPATH_NATIVE,
    wxPATH_UNIX,
    wxPATH_MAC,
    wxPATH_DOS,
    wxPATH_VMS,
    wxPATH_BEOS,
    wxPATH_WIN,
    wxPATH_OS2,
    wxPATH_MAX
};

enum wxPathNormalize
{
    wxPATH_NORM_ENV_VARS,
    wxPATH_NORM_DOTS,
    wxPATH_NORM_TILDE,
    wxPATH_NORM_CASE,
    wxPATH_NORM_ABSOLUTE,
    wxPATH_NORM_LONG,
    wxPATH_NORM_SHORTCUT,
    wxPATH_NORM_ALL
};

class %delete wxFileName
{
    wxFileName( );
    wxFileName(const wxFileName& filename );
    wxFileName(const wxString& fullpath, wxPathFormat format = wxPATH_NATIVE );
    wxFileName(const wxString& path, const wxString& name, wxPathFormat format = wxPATH_NATIVE );
    wxFileName(const wxString& volume, const wxString& path, const wxString& name, const wxString& ext, wxPathFormat format = wxPATH_NATIVE );

    void AppendDir(const wxString& dir );
    void Assign(const wxFileName& filepath );
    void Assign(const wxString& fullpath, wxPathFormat format = wxPATH_NATIVE );
    void Assign(const wxString& volume, const wxString& path, const wxString& name, const wxString& ext, wxPathFormat format = wxPATH_NATIVE );
    void Assign(const wxString& path, const wxString& name, wxPathFormat format = wxPATH_NATIVE );
    void Assign(const wxString& path, const wxString& name, const wxString& ext, wxPathFormat format = wxPATH_NATIVE );
    void AssignCwd(const wxString& volume = "" );
    void AssignDir(const wxString& dir, wxPathFormat format = wxPATH_NATIVE );
    void AssignHomeDir( );
    !%wxchkver_2_8 void AssignTempFileName(const wxString& prefix, wxFile *fileTemp = NULL );
    %wxchkver_2_8&&(wxUSE_FILE||wxUSE_FFILE) void AssignTempFileName(const wxString& prefix );
    %wxchkver_2_8&&wxUSE_FILE void AssignTempFileName(const wxString& prefix, wxFile *fileTemp );
    //%wxchkver_2_8&&wxUSE_FFILE void AssignTempFileName(const wxString& prefix, wxFFile *fileTemp );
    void Clear( );
    void ClearExt( );

    // Use AssignTempFileName(...) equivalents
    //!%wxchkver_2_8 static wxString CreateTempFileName(const wxString& prefix, wxFile *fileTemp = NULL );
    //%wxchkver_2_8&&(wxUSE_FILE||wxUSE_FFILE) static wxString CreateTempFileName(const wxString& prefix );
    //%wxchkver_2_8&&wxUSE_FILE static wxString CreateTempFileName(const wxString& prefix, wxFile *fileTemp );
    //%wxchkver_2_8&&wxUSE_FFILE static wxString CreateTempFileName(const wxString& prefix, wxFFile *fileTemp);

    bool DirExists( );
    static bool DirExists(const wxString& dir );
    static wxFileName DirName(const wxString& dir );
    bool FileExists( );
    static bool FileExists(const wxString& file );
    static wxFileName FileName(const wxString& file );
    static wxString GetCwd(const wxString& volume = "" );
    int GetDirCount() const;

    // %override [Lua string table] wxFileName::GetDirs( );
    // C++ Func: const wxArrayString& GetDirs() const;
    const wxArrayString& GetDirs() const;

    wxString GetExt() const;
    static wxString GetForbiddenChars(wxPathFormat format = wxPATH_NATIVE );
    static wxPathFormat GetFormat(wxPathFormat format = wxPATH_NATIVE );
    wxString GetFullName() const;
    wxString GetFullPath(wxPathFormat format = wxPATH_NATIVE) const;
    static wxString GetHomeDir( );
    %wxchkver_2_8 wxString GetHumanReadableSize(const wxString &nullsize = "Not available", int precision = 1) const;
    //%wxchkver_2_8 wxString GetHumanReadableSize(const wxString &nullsize = wxGetTranslation(_T("Not available")), int precision = 1) const;
    //%wxchkver_2_8 static wxString GetHumanReadableSize(const wxULongLong &sz, const wxString &nullsize = wxGetTranslation(_T("Not available")), int precision = 1 );
    wxString GetLongPath() const;
    wxDateTime GetModificationTime() const;
    wxString GetName() const;
    wxString GetPath(int flags = 0, wxPathFormat format = wxPATH_NATIVE) const;
    static int GetPathSeparator(wxPathFormat format = wxPATH_NATIVE );
    static wxString GetPathSeparators(wxPathFormat format = wxPATH_NATIVE );
    static wxString  GetPathTerminators(wxPathFormat format = wxPATH_NATIVE );
    wxString GetPathWithSep(wxPathFormat format = wxPATH_NATIVE ) const;
    wxString GetShortPath() const;

    #if %wxchkver_2_8
        wxULongLong GetSize() const;
        static wxULongLong GetSize(const wxString &file );
    #endif // %wxchkver_2_8

    // %override [bool, wxDateTime dtAccess, wxDateTime dtMod, wxDateTime dtCreate] wxFileName::GetTimes( );
    // C++ Func: bool GetTimes(wxDateTime* dtAccess, wxDateTime* dtMod, wxDateTime* dtCreate) const;
    bool GetTimes() const;

    wxString GetVolume() const;
    static wxString GetVolumeSeparator(wxPathFormat format = wxPATH_NATIVE );
    bool HasExt() const;
    bool HasName() const;
    bool HasVolume() const;
    void InsertDir(int before, const wxString& dir );
    bool IsAbsolute(wxPathFormat format = wxPATH_NATIVE );
    static bool IsCaseSensitive(wxPathFormat format = wxPATH_NATIVE );
    bool IsOk() const;
    static bool IsPathSeparator(int ch, wxPathFormat format = wxPATH_NATIVE );
    bool IsRelative(wxPathFormat format = wxPATH_NATIVE );
    bool IsDir() const;

    #if %wxchkver_2_8
        bool IsDirWritable() const;
        static bool IsDirWritable(const wxString &path );
        bool IsDirReadable() const;
        static bool IsDirReadable(const wxString &path );
        bool IsFileWritable() const;
        static bool IsFileWritable(const wxString &path );
        bool IsFileReadable() const;
        static bool IsFileReadable(const wxString &path );
        bool IsFileExecutable() const;
        static bool IsFileExecutable(const wxString &path );
    #endif // %wxchkver_2_8

    //static bool MacFindDefaultTypeAndCreator(const wxString& ext, wxUint32* type, wxUint32* creator );
    //bool MacSetDefaultTypeAndCreator( );
    bool MakeAbsolute(const wxString& cwd = "", wxPathFormat format = wxPATH_NATIVE );
    bool MakeRelativeTo(const wxString& pathBase = "", wxPathFormat format = wxPATH_NATIVE );
    bool Mkdir(int perm = 4095, int flags = 0 );
    static bool Mkdir(const wxString& dir, int perm = 4095, int flags = 0 );
    bool Normalize(int flags = wxPATH_NORM_ALL, const wxString& cwd = wxEmptyString, wxPathFormat format = wxPATH_NATIVE );
    void PrependDir(const wxString& dir );
    void RemoveDir(int pos );
    void RemoveLastDir( );
    bool Rmdir( );
    static bool Rmdir(const wxString& dir );
    bool SameAs(const wxFileName& filepath, wxPathFormat format = wxPATH_NATIVE) const;
    bool SetCwd( );
    static bool SetCwd(const wxString& cwd );
    void SetExt(const wxString& ext );
    void SetEmptyExt( );
    void SetFullName(const wxString& fullname );
    void SetName(const wxString& name );
    bool SetTimes(const wxDateTime* dtAccess, const wxDateTime* dtMod, const wxDateTime* dtCreate );
    void SetVolume(const wxString& volume );

    // %override [wxString path, wxString name, wxString ext] wxFileName::SplitPath(const wxString& fullpath, wxPathFormat format = wxPATH_NATIVE );
    // C++ Func: static void SplitPath(const wxString& fullpath, wxString* path, wxString* name, wxString* ext, wxPathFormat format = wxPATH_NATIVE );
    static void SplitPath(const wxString& fullpath, wxPathFormat format = wxPATH_NATIVE );

    // %override [wxString volume, wxString path, wxString name, wxString ext] wxFileName::SplitPathVolume(const wxString& fullpath, wxPathFormat format = wxPATH_NATIVE );
    // C++ Func: static void SplitPath(const wxString& fullpath, wxString* volume, wxString* path, wxString* name, wxString* ext, wxPathFormat format = wxPATH_NATIVE );
    %rename SplitPathVolume static void SplitPath(const wxString& fullpath, wxPathFormat format = wxPATH_NATIVE );

    // %override [wxString volume, wxString path] wxFileName::SplitVolume(const wxString& fullpath, wxPathFormat format = wxPATH_NATIVE );
    // C++ Func: static void SplitVolume(const wxString& fullpath, wxString* volume, wxString* path, wxPathFormat format = wxPATH_NATIVE );
    static void SplitVolume(const wxString& fullpath, wxPathFormat format = wxPATH_NATIVE );

    bool Touch( );

    wxFileName& operator=(const wxFileName& filename );
    bool operator==(const wxFileName& filename) const;
};

#endif //wxLUA_USE_wxFileName

// ---------------------------------------------------------------------------
// wxFile

#if wxLUA_USE_wxFile && wxUSE_FILE

#include "wx/file.h"

enum wxFile::OpenMode
{
    read,
    write,
    read_write,
    write_append,
    write_excl
};

enum wxFile::dummy
{
    fd_invalid, // = -1
    fd_stdin,
    fd_stdout,
    fd_stderr
};

enum wxSeekMode
{
    wxFromStart,
    wxFromCurrent,
    wxFromEnd,
    wxInvalidOffset
};

enum wxFileKind
{
    wxFILE_KIND_UNKNOWN,
    wxFILE_KIND_DISK,
    wxFILE_KIND_TERMINAL,
    wxFILE_KIND_PIPE
};

#define wxS_IRUSR
#define wxS_IWUSR
#define wxS_IXUSR
#define wxS_IRGRP
#define wxS_IWGRP
#define wxS_IXGRP
#define wxS_IROTH
#define wxS_IWOTH
#define wxS_IXOTH
#define wxS_DEFAULT

class %delete wxFile
{
    wxFile( );
    wxFile(const wxString& filename, wxFile::OpenMode mode = wxFile::read );

    static bool Access(const wxString& name, wxFile::OpenMode mode );
    void Attach(int fd );
    void Close( );
    bool Create(const wxString& filename, bool overwrite = false, int access = wxS_DEFAULT );
    void Detach( );
    int fd() const;
    bool Eof() const;
    static bool Exists(const wxString& name );
    bool Flush( );
    wxFileKind GetKind() const;
    bool IsOpened() const;
    wxFileOffset Length() const;
    bool Open(const wxString& filename, wxFile::OpenMode mode = wxFile::read );

    // %override [size_t count, Lua string] wxFile::Read(unsigned int count );
    // C++ Func: size_t Read(void* buffer, unsigned int count );
    size_t Read(unsigned int count );

    wxFileOffset Seek(wxFileOffset offset, wxSeekMode mode = wxFromStart );
    wxFileOffset SeekEnd(wxFileOffset offset = 0 );
    wxFileOffset Tell() const;

    // %override size_t wxFile::Write(Lua string, unsigned int count );
    // C++ Func: size_t Write(const void* buffer, unsigned int count );
    size_t Write(const wxString& buffer, unsigned int count );

    size_t Write(const wxString &str); //, const wxMBConv& conv = wxConvUTF8 );
};

// ---------------------------------------------------------------------------
// wxTempFile

#include "wx/file.h"

class %delete wxTempFile
{
    wxTempFile( );
    // associates the temp file with the file to be replaced and opens it
    wxTempFile(const wxString& strName );

    // open the temp file (strName is the name of file to be replaced );
    bool Open(const wxString& strName );

    // is the file opened?
    bool IsOpened() const;
    // get current file length
    wxFileOffset Length() const;
    // move ptr ofs bytes related to start/current offset/end of file
    wxFileOffset Seek(wxFileOffset ofs, wxSeekMode mode = wxFromStart );
    // get current offset
    wxFileOffset Tell() const;

    // I/O (both functions return true on success, false on failure );
    //bool Write(const void *p, size_t n );
    bool Write(const wxString& str); //, const wxMBConv& conv = wxConvUTF8 );

    // validate changes and delete the old file of name m_strName
    bool Commit( );
    // discard changes
    void Discard();
};

#endif //wxLUA_USE_wxFile && wxUSE_FILE

// ---------------------------------------------------------------------------
// wxDir

#if wxLUA_USE_wxDir

#include "wx/dir.h"

#define wxDIR_FILES
#define wxDIR_DIRS
#define wxDIR_HIDDEN
#define wxDIR_DOTDOT
%wxchkver_2_9_5 #define wxDIR_NO_FOLLOW
#define wxDIR_DEFAULT


#if %wxchkver_2_9_4

// these constants are possible return value of wxDirTraverser::OnDir()
enum wxDirTraverseResult
{
    wxDIR_IGNORE,           // ignore this directory but continue with others
    wxDIR_STOP,             // stop traversing
    wxDIR_CONTINUE          // continue into this directory
};

#endif //%wxchkver_2_9_4


class %delete wxDir
{
    wxDir( );
    wxDir(const wxString& dir );

    static bool Exists(const wxString& dir );

    // %override [unsigned int, Lua string table] wxDir::GetAllFiles(const wxString& dirname, const wxString& filespec = "", int flags = wxDIR_DEFAULT );
    // C++ Func: static unsigned int GetAllFiles(const wxString& dirname, wxArrayString *files, const wxString& filespec = "", int flags = wxDIR_DEFAULT );
    static unsigned int GetAllFiles(const wxString& dirname, const wxString& filespec = "", int flags = wxDIR_DEFAULT );

    // %override [bool, string filename] wxDir::GetFirst(const wxString& filespec = "", int flags = wxDIR_DEFAULT );
    // C++ Func: bool GetFirst(wxString * filename, const wxString& filespec = "", int flags = wxDIR_DEFAULT) const;
    bool GetFirst(const wxString& filespec = "", int flags = wxDIR_DEFAULT) const;

    wxString GetName() const;

    // %override [bool, string filename] wxDir::GetNext( );
    // C++ Func: bool GetNext(wxString * filename) const;
    bool GetNext() const;

    bool HasFiles(const wxString& filespec = "" );
    bool HasSubDirs(const wxString& dirspec = "" );
    bool IsOpened() const;
    bool Open(const wxString& dir );

    #if %wxchkver_2_8
        static wxString FindFirst(const wxString& dirname, const wxString& filespec, int flags = wxDIR_DEFAULT );
        static wxULongLong GetTotalSize(const wxString &dir); //, wxArrayString *filesSkipped = NULL) FIXME override
    #endif // %wxchkver_2_8

    // We don't need wxDirTraverser, just use wxDir methods GetFirst, GetNext.
    //size_t Traverse(wxDirTraverser& sink, const wxString& filespec = wxEmptyString, int flags = wxDIR_DEFAULT );
};

#endif //wxLUA_USE_wxDir

// ---------------------------------------------------------------------------
//  wxFileTypeInfo

#include "wx/mimetype.h"

class %delete wxFileTypeInfo
{
    //  the ... parameters form a NULL terminated list of extensions
    //wxFileTypeInfo(const wxChar *mimeType, const wxChar *openCmd, const wxChar *printCmd, const wxChar *desc, ... );
    // the array elements correspond to the parameters of the ctor above in the same order
    wxFileTypeInfo(const wxArrayString& sArray );

    // invalid item - use this to terminate the array passed to wxMimeTypesManager::AddFallbacks
    wxFileTypeInfo( );

    bool IsValid() const;

    void SetIcon(const wxString& iconFile, int iconIndex = 0 );
    void SetShortDesc(const wxString& shortDesc );

    wxString GetMimeType() const;
    wxString GetOpenCommand() const;
    wxString GetPrintCommand() const;
    wxString GetShortDesc() const;
    wxString GetDescription() const;
    wxArrayString GetExtensions() const;
    size_t GetExtensionsCount() const;
    wxString GetIconFile() const;
    int GetIconIndex() const;
};

// ---------------------------------------------------------------------------
// wxIconLocation

#include "wx/iconloc.h"

class %delete wxIconLocation
{
    // ctor takes the name of the file where the icon is
    !%msw wxIconLocation(const wxString& filename = "" );
    %msw wxIconLocation(const wxString& file = "", int num = 0 );

    // returns true if this object is valid/initialized
    bool IsOk() const;

    // set/get the icon file name
    void SetFileName(const wxString& filename );
    const wxString& GetFileName() const;

    // set/get the icon index
    %msw void SetIndex(int num );
    %msw int GetIndex() const;
};

// ---------------------------------------------------------------------------
//  wxFileType::MessageParameters

class %delete wxFileType::MessageParameters
{
    //wxFileType::MessageParameters( );
    wxFileType::MessageParameters(const wxString& filename, const wxString& mimetype = "" );

    // accessors (called by GetOpenCommand );
    wxString GetFileName() const;
    wxString GetMimeType() const;

    // override this function in derived class
    virtual wxString GetParamValue(const wxString& name) const;
};

// ---------------------------------------------------------------------------
//  wxFileType

class %delete wxFileType
{
    wxFileType(const wxFileTypeInfo& ftInfo );

    // accessors: all of them return true if the corresponding information
    // could be retrieved/found, false otherwise (and in this case all [out] parameters are unchanged );

    // return the MIME type for this file type
    //bool GetMimeType(wxString *mimeType) const;
    bool GetMimeTypes(wxArrayString& mimeTypes) const;

    bool GetExtensions(wxArrayString& extensions);

    // get the icon corresponding to this file type and of the given size
    bool GetIcon(wxIconLocation *iconloc) const;
    //bool GetIcon(wxIconLocation *iconloc, const wxFileType::MessageParameters& params) const;

    // get a brief file type description ("*.txt" => "text document" );
    // %override [bool Lua string] wxFileType::GetDescription() const;
    // C++ Func: bool GetDescription(wxString *desc) const;
    bool GetDescription() const;

    // get the command to be used to open/print the given file.
    //bool GetOpenCommand(wxString *openCmd, const wxFileType::MessageParameters& params) const;
    // a simpler to use version of GetOpenCommand() -- it only takes the
    // filename and returns an empty string on failure
    wxString GetOpenCommand(const wxString& filename) const;

    // get the command to print the file of given type
    // %override [bool Lua string] wxFileType::GetPrintCommand(const wxFileType::MessageParameters& params) const;
    // C++ Func: bool GetPrintCommand(wxString *printCmd, const wxFileType::MessageParameters& params) const;
    bool GetPrintCommand(const wxFileType::MessageParameters& params) const;

    // return the number of commands defined for this file type, 0 if none
    size_t GetAllCommands(wxArrayString *verbs, wxArrayString *commands, const wxFileType::MessageParameters& params) const;

    // set an arbitrary command, ask confirmation if it already exists and overwriteprompt is true
    bool SetCommand(const wxString& cmd, const wxString& verb, bool overwriteprompt = true );

    bool SetDefaultIcon(const wxString& cmd = "", int index = 0 );

    // remove the association for this filetype from the system MIME database:
    // notice that it will only work if the association is defined in the user
    // file/registry part, we will never modify the system-wide settings
    bool Unassociate();

    // expand a string in the format of GetOpenCommand (which may contain
    // '%s' and '%t' format specificators for the file name and mime type
    // and %{param} constructions).
    static wxString ExpandCommand(const wxString& command, const wxFileType::MessageParameters& params);
};

// ---------------------------------------------------------------------------
//  wxMimeTypesManager

class wxMimeTypesManager
{
    #define_pointer wxTheMimeTypesManager

    // wxMimeTypesManager(); - Use pointer wxTheMimeTypesManager

    // check if the given MIME type is the same as the other one: the
    // second argument may contain wildcards ('*'), but not the first. If
    // the types are equal or if the mimeType matches wildcard the function
    // returns true, otherwise it returns false
    static bool IsOfType(const wxString& mimeType, const wxString& wildcard);

    // NB: the following 2 functions are for Unix only and don't do anything elsewhere

    // loads data from standard files according to the mailcap styles
    // specified: this is a bitwise OR of wxMailcapStyle values
    //
    // use the extraDir parameter if you want to look for files in another
    // directory
    void Initialize(int mailcapStyle = wxMAILCAP_ALL, const wxString& extraDir = "");
    // and this function clears all the data from the manager
    void ClearData();

    // Database lookup: all functions return a pointer to wxFileType object
    // whose methods may be used to query it for the information you're
    // interested in. If the return value is !NULL, caller is responsible for
    // deleting it.
    // get file type from file extension
    wxFileType *GetFileTypeFromExtension(const wxString& ext);
    // get file type from MIME type (in format <category>/<format> );
    wxFileType *GetFileTypeFromMimeType(const wxString& mimeType);

    !%wxchkver_2_9 bool ReadMailcap(const wxString& filename, bool fallback = false);
    // read in additional file in mime.types format
    !%wxchkver_2_9 bool ReadMimeTypes(const wxString& filename);

    // enumerate all known MIME types returns the number of retrieved file types
    size_t EnumAllFileTypes(wxArrayString& mimetypes);

    // The filetypes array should be terminated by either NULL entry or an
    // invalid wxFileTypeInfo (i.e. the one created with default ctor );
    //void AddFallbacks(const wxFileTypeInfo *filetypes);
    void AddFallback(const wxFileTypeInfo& ft );

    // create a new association using the fields of wxFileTypeInfo (at least
    // the MIME type and the extension should be set );
    // if the other fields are empty, the existing values should be left alone
    wxFileType *Associate(const wxFileTypeInfo& ftInfo );

    // undo Associate( );
    bool Unassociate(wxFileType *ft );
};

// ---------------------------------------------------------------------------
//  wxStreamBase

#if wxUSE_STREAMS

#include "wx/stream.h"
#include "wx/txtstrm.h"

enum wxEOL
{
    wxEOL_NATIVE,
    wxEOL_UNIX,
    wxEOL_MAC,
    wxEOL_DOS
};

enum wxStreamError
{
    wxSTREAM_NO_ERROR,
    wxSTREAM_EOF,
    wxSTREAM_WRITE_ERROR,
    wxSTREAM_READ_ERROR
};

// ---------------------------------------------------------------------------
//  wxStreamBase

class wxStreamBase
{
    // wxStreamBase() this is only a base class

    %wxchkver_2_6 wxFileOffset GetLength() const;
    wxStreamError GetLastError() const;
    size_t GetSize() const;
    bool IsOk() const;
    bool IsSeekable() const;
    void Reset( );
};

// ---------------------------------------------------------------------------
//  wxInputStream

class wxInputStream : public wxStreamBase
{
    // wxInputStream() this is only a base class

    bool CanRead() const;
    char GetC( );
    bool Eof( );
    size_t LastRead() const;
    char Peek( );

    // %override [Lua string] wxInputStream::Read(size_t size );
    // C++ Func: wxInputStream& Read(void *buffer, size_t size );
    wxString Read(size_t size );

    wxInputStream& Read(wxOutputStream& stream_in );
    wxFileOffset SeekI(wxFileOffset pos, wxSeekMode mode = wxFromStart );
    wxFileOffset TellI() const;

    // %override size_t wxInputStream::Ungetch(Lua string, size_t size );
    // C++ Func: size_t Ungetch(const char* buffer, size_t size );
    %override_name wxLua_wxInputStream_UngetchString size_t Ungetch(const wxString& str, size_t size );

    bool Ungetch(char c );
};

// ---------------------------------------------------------------------------
//  wxOutputStream

class wxOutputStream : public wxStreamBase
{
    // wxOutputStream() this is only a base class

    bool Close( );
    size_t LastWrite() const;
    void PutC(char c );
    wxFileOffset SeekO(wxFileOffset pos, wxSeekMode mode = wxFromStart );
    wxFileOffset TellO() const;

    // %override wxOutputStream& wxOutputStream::Write(Lua string, size_t size );
    // C++ Func: wxOutputStream& Write(const void *buffer, size_t size );
    wxOutputStream& Write(const wxString& buffer, size_t size );

    wxOutputStream& Write(wxInputStream& stream_in );
};

// ---------------------------------------------------------------------------
//  wxFileInputStream

#include "wx/wfstream.h"

class %delete wxFileInputStream : public wxInputStream
{
    wxFileInputStream(const wxString& fileName );
    wxFileInputStream(wxFile& file );
    //wxFileInputStream(int fd );

    bool Ok() const;
};

// ---------------------------------------------------------------------------
//  wxFileOutputStream

class %delete wxFileOutputStream : public wxOutputStream
{
    wxFileOutputStream(const wxString& fileName );
    wxFileOutputStream(wxFile& file );
    //wxFileOutputStream(int fd );

    bool Ok() const;
};

// ---------------------------------------------------------------------------
//  wxMemoryInputStream

#include "wx/mstream.h"

class %delete wxMemoryInputStream : public wxInputStream
{
    wxMemoryInputStream(const char *data, size_t length );
    //wxMemoryInputStream(const wxMemoryOutputStream& stream );

};

// ---------------------------------------------------------------------------
//  wxMemoryOutputStream

//%include "wx/mstream.h"

//class %delete wxMemoryInputStream : public wxInputStream
//{
//    wxMemoryOutputStream(void *data, size_t length );
//    wxMemoryInputStream(const wxMemoryOutputStream& stream );
//};

// ---------------------------------------------------------------------------
//  wxDataInputStream

#include "wx/datstrm.h"

class %delete wxDataInputStream
{
    // wxDataInputStream(wxInputStream& s, const wxMBConv& conv = wxConvAuto());
    wxDataInputStream(wxInputStream& s );

    bool IsOk( );

    //#if wxHAS_INT64
    //    wxUint64 Read64( );
    //#endif
    //#if wxUSE_LONGLONG
    //    wxLongLong ReadLL( );
    //#endif
    wxUint32 Read32( );
    wxUint16 Read16( );
    wxUint8 Read8( );
    double ReadDouble( );
    wxString ReadString( );

    //#if wxHAS_INT64
    //   void Read64(wxUint64 *buffer, size_t size );
    //    void Read64(wxInt64 *buffer, size_t size );
    //#endif
    //#if defined(wxLongLong_t) && wxUSE_LONGLONG
    //    void Read64(wxULongLong *buffer, size_t size );
    //    void Read64(wxLongLong *buffer, size_t size );
    //#endif
    //#if wxUSE_LONGLONG
    //  void ReadLL(wxULongLong *buffer, size_t size );
    //  void ReadLL(wxLongLong *buffer, size_t size );
    //#endif
    //void Read32(wxUint32 *buffer, size_t size );
    //void Read16(wxUint16 *buffer, size_t size );
    //void Read8(wxUint8 *buffer, size_t size );
    //void ReadDouble(double *buffer, size_t size );

    void BigEndianOrdered(bool be_order );
};

// ---------------------------------------------------------------------------
//  wxDataOutputStream

#include "wx/datstrm.h"

class %delete wxDataOutputStream
{
    // wxDataOutputStream(wxOutputStream& s, const wxMBConv& conv = wxConvAuto());
    wxDataOutputStream(wxOutputStream& s);

    bool IsOk( );

    //#if wxHAS_INT64
    //  void Write64(wxUint64 i);
    //  void Write64(wxInt64 i);
    //#endif
    //#if wxUSE_LONGLONG
    //    void WriteLL(const wxLongLong &ll);
    //    void WriteLL(const wxULongLong &ll);
    //#endif
    void Write32(wxUint32 i );
    void Write16(wxUint16 i );
    void Write8(wxUint8 i );
    void WriteDouble(double d );
    void WriteString(const wxString& string );

    //#if wxHAS_INT64
    //    void Write64(const wxUint64 *buffer, size_t size);
    //    void Write64(const wxInt64 *buffer, size_t size);
    //#endif
    //#if defined(wxLongLong_t) && wxUSE_LONGLONG
    //    void Write64(const wxULongLong *buffer, size_t size);
    //    void Write64(const wxLongLong *buffer, size_t size);
    //#endif
    //#if wxUSE_LONGLONG
    //    void WriteLL(const wxULongLong *buffer, size_t size);
    //    void WriteLL(const wxLongLong *buffer, size_t size);
    //#endif
    //void Write32(const wxUint32 *buffer, size_t size);
    //void Write16(const wxUint16 *buffer, size_t size);
    //void Write8(const wxUint8 *buffer, size_t size);
    //void WriteDouble(const double *buffer, size_t size);

    void BigEndianOrdered(bool be_order );
};



// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//  wxFSFile

#if wxUSE_FILESYSTEM // already has wxUSE_STREAMS

#include "wx/filesys.h"


class %delete wxFSFile : public wxObject
{
    wxFSFile(%ungc wxInputStream *stream, const wxString& loc, const wxString& mimetype, const wxString& anchor, wxDateTime modif );

    // returns stream. This doesn't give away ownership of the stream object.
    wxInputStream *GetStream() const;
    // gives away the ownership of the current stream.
    %gc wxInputStream *DetachStream( );
    // deletes the current stream and takes ownership of another.
    void SetStream(%ungc wxInputStream *stream );

    // returns file's mime type
    wxString GetMimeType() const;
    // returns the original location (aka filename) of the file
    wxString GetLocation() const;
    wxString GetAnchor() const;
    wxDateTime GetModificationTime() const;
};


// ---------------------------------------------------------------------------
//  wxFileSystemHandler

class %delete wxFileSystemHandler : public wxObject
{
    // wxFileSystemHandler(); // no constructor since it has abstract functions

    // returns true if this handler is able to open given location
    virtual bool CanOpen(const wxString& location); //= 0;

    // opens given file and returns pointer to input stream.
    // Returns NULL if opening failed.
    // The location is always absolute path.
    virtual wxFSFile* OpenFile(wxFileSystem& fs, const wxString& location); //= 0;

    // Finds first/next file that matches spec wildcard. flags can be wxDIR for restricting
    // the query to directories or wxFILE for files only or 0 for either.
    // Returns filename or empty string if no more matching file exists
    virtual wxString FindFirst(const wxString& spec, int flags = 0);
    virtual wxString FindNext();
};


// ---------------------------------------------------------------------------
//  wxLocalFSHandler


class %delete wxLocalFSHandler : public wxFileSystemHandler
{
    wxLocalFSHandler( );

    // wxLocalFSHandler will prefix all filenames with 'root' before accessing
    // files on disk. This effectively makes 'root' the top-level directory
    // and prevents access to files outside this directory.
    // (This is similar to Unix command 'chroot'. );
    static void Chroot(const wxString& root );
};


// ---------------------------------------------------------------------------
//  wxFileSystem

enum
{
    wxFS_READ,        // Open for reading
    wxFS_SEEKABLE    // Returned stream will be seekable
};

class %delete wxFileSystem : public wxObject
{
    wxFileSystem( );

    // sets the current location. Every call to OpenFile is
    // relative to this location.
    // NOTE !!
    // unless is_dir = true 'location' is *not* the directory but
    // file contained in this directory
    // (so ChangePathTo("dir/subdir/xh.htm") sets m_Path to "dir/subdir/" );
    void ChangePathTo(const wxString& location, bool is_dir = false);

    wxString GetPath() const;

    // opens given file and returns pointer to input stream.
    // Returns NULL if opening failed.
    // It first tries to open the file in relative scope
    // (based on ChangePathTo()'s value) and then as an absolute
    // path.
    %gc wxFSFile* OpenFile(const wxString& location, int flags = wxFS_READ);

    // Finds first/next file that matches spec wildcard. flags can be wxDIR for restricting
    // the query to directories or wxFILE for files only or 0 for either.
    // Returns filename or empty string if no more matching file exists
    wxString FindFirst(const wxString& spec, int flags = 0);
    wxString FindNext();

    // find a file in a list of directories, returns false if not found
    // %override [bool, Lua string full_path] bool FindFileInPath(const wxString& path, const wxString& file);
    // C++ Func: bool FindFileInPath(wxString *pStr, const wxChar *path, const wxChar *file);
    bool FindFileInPath(const wxString& path, const wxString& file);

    // Adds FS handler.
    // In fact, this class is only front-end to the FS handlers :- );
    static void AddHandler(%ungc wxFileSystemHandler *handler);

    // Removes FS handler
    static %gc wxFileSystemHandler* RemoveHandler(wxFileSystemHandler *handler);

    // Returns true if there is a handler which can open the given location.
    static bool HasHandlerForPath(const wxString& location);

    // remove all items from the m_Handlers list
    static void CleanUpHandlers();

    // Returns the native path for a file URL
    static wxFileName URLToFileName(const wxString& url);

    // Returns the file URL for a native path
    static wxString FileNameToURL(const wxFileName& filename);
};


// ---------------------------------------------------------------------------
//  wxArchiveFSHandler

#include "wx/fs_arc.h"

class %delete wxArchiveFSHandler : public wxFileSystemHandler
{
    wxArchiveFSHandler( );
};

// ---------------------------------------------------------------------------
//  wxZipFSHandler - is just a typedef to wxArchiveFSHandler

//%include "wx/fs_zip.h"

//#if wxUSE_FS_ZIP
//    typedef wxArchiveFSHandler wxZipFSHandler;
//#endif

// ---------------------------------------------------------------------------
//  wxFilterFSHandler

#include "wx/fs_filter.h"

class %delete wxFilterFSHandler : public wxFileSystemHandler
{
    wxFilterFSHandler( );
};

// ---------------------------------------------------------------------------
//  wxInternetFSHandler

#if wxUSE_FS_INET && wxUSE_SOCKETS // already has wxUSE_STREAMS && wxUSE_FILESYSTEM
#include "wx/fs_inet.h"

class %delete wxInternetFSHandler : public wxFileSystemHandler
{
    wxInternetFSHandler( );
};
#endif //wxUSE_FS_INET && wxUSE_SOCKETS

// ---------------------------------------------------------------------------
//  wxMemoryFSHandler - See wxcore_core.i for this since it requires wxImage & wxBitmap.


#endif // wxUSE_FILESYSTEM


#endif // wxUSE_STREAMS

// ---------------------------------------------------------------------------
//  wxFileSystemWatcher classes

#if wxUSE_FSWATCHER && %wxchkver_2_9_4

#include "wx/fswatcher.h"

enum
{
    wxFSW_EVENT_CREATE, // = 0x01,
    wxFSW_EVENT_DELETE, // = 0x02,
    wxFSW_EVENT_RENAME, // = 0x04,
    wxFSW_EVENT_MODIFY, // = 0x08,
    wxFSW_EVENT_ACCESS, // = 0x10,
    wxFSW_EVENT_ATTRIB, // = 0x20, // Currently this is wxGTK-only

    // error events
    wxFSW_EVENT_WARNING, // = 0x40,
    wxFSW_EVENT_ERROR, // = 0x80,
    wxFSW_EVENT_ALL, // = wxFSW_EVENT_CREATE | wxFSW_EVENT_DELETE |
                     //    wxFSW_EVENT_RENAME | wxFSW_EVENT_MODIFY |
                     //    wxFSW_EVENT_ACCESS | wxFSW_EVENT_ATTRIB |
                     //    wxFSW_EVENT_WARNING | wxFSW_EVENT_ERROR
#if defined(wxHAS_INOTIFY)
    wxFSW_EVENT_UNMOUNT, // = 0x2000
#endif
};

// Type of the path watched, used only internally for now.
enum wxFSWPathType
{
    wxFSWPath_None,     // Invalid value for an initialized watch.
    wxFSWPath_File,     // Plain file.
    wxFSWPath_Dir,      // Watch a directory and the files in it.
    wxFSWPath_Tree      // Watch a directory and all its children recursively.
};

// Type of the warning for the events notifying about them.
enum wxFSWWarningType
{
    wxFSW_WARNING_NONE,
    wxFSW_WARNING_GENERAL,
    wxFSW_WARNING_OVERFLOW
};

// ---------------------------------------------------------------------------
// wxFileSystemWatcherEvent

class %delete wxFileSystemWatcherEvent: public wxEvent
{
public:
    %wxEventType wxEVT_FSWATCHER   // EVT_FSWATCHER(winid, func);

    wxFileSystemWatcherEvent(int changeType = 0, int watchid = wxID_ANY);
    wxFileSystemWatcherEvent(int changeType, wxFSWWarningType warningType, const wxString& errorMsg = "", int watchid = wxID_ANY);
    wxFileSystemWatcherEvent(int changeType, const wxFileName& path, const wxFileName& newPath, int watchid = wxID_ANY);

    const wxFileName& GetPath() const;
    void SetPath(const wxFileName& path);
    const wxFileName& GetNewPath() const;
    void SetNewPath(const wxFileName& path);
    int GetChangeType() const;
    //virtual wxEvent* Clone() const;
    //virtual wxEventCategory GetEventCategory() const;
    bool IsError() const;
    wxString GetErrorDescription() const;
    wxFSWWarningType GetWarningType() const;
    wxString ToString() const;
};

// ---------------------------------------------------------------------------
// wxFileSystemWatcher

class wxFileSystemWatcher : public wxEvtHandler
{
public:
    wxFileSystemWatcher();

    virtual bool Add(const wxFileName& path, int events = wxFSW_EVENT_ALL);
    bool AddAny(const wxFileName& path, int events, wxFSWPathType type, const wxString& filespec = "");
    virtual bool AddTree(const wxFileName& path, int events = wxFSW_EVENT_ALL, const wxString& filespec = wxEmptyString);
    virtual bool Remove(const wxFileName& path);
    virtual bool RemoveTree(const wxFileName& path);
    virtual bool RemoveAll();
    int GetWatchedPathsCount() const;
    int GetWatchedPaths(wxArrayString* paths) const;
    wxEvtHandler* GetOwner() const;
    void SetOwner(wxEvtHandler* handler);
};

#endif // wxUSE_FSWATCHER && %wxchkver_2_9_4
