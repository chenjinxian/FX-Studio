///////////////////////////////////////////////////////////////////////////////
// Name:        steopts.cpp
// Purpose:     wxSTEditorOptions
// Author:      John Labenski, parts taken from wxGuide by Otto Wyss
// Modified by:
// Created:     11/05/2002
// RCS-ID:
// Copyright:   (c) John Labenski, Otto Wyss
// Licence:     wxWidgets licence
///////////////////////////////////////////////////////////////////////////////

#include "precomp.h"

#include "wx/stedit/stedit.h"

wxString STE_DefaultFileName( wxT("untitled.txt") );
wxString STE_DefaultFileExtensions(
                                    wxT("All Files|")wxALL_FILES_PATTERN wxT("|")
                                #if !STE_FILEOPENEXTRA
                                    wxT("UTF8 Text Files|")wxALL_FILES_PATTERN wxT("|")
                                    wxT("Unicode Text Files|")wxALL_FILES_PATTERN wxT("|")
                                #ifdef __WXMSW__
                                    wxT("OEM Text Files|")wxALL_FILES_PATTERN wxT("|")
                                #endif
                                #endif
                                    wxT("Text Files (txt text)|*.txt;*.text")wxT("|")
                                    wxT("C/C++ Files (c cpp cxx)|*.c;*.cpp;*.cxx")wxT("|")
                                    wxT("H Files (h)|*.h")wxT("|")
                                    wxT("Html Files (htm html)|*.htm;*.html")wxT("|")
                                    wxT("XML Files (xml)|*.xml")wxT("|")
                                    wxT("Lua Files (lua)|*.lua")wxT("|")
                                    wxT("Python Files (py)|*.py")//wxT("|")
                                    );

//-----------------------------------------------------------------------------
// wxSTEditorOptions
//-----------------------------------------------------------------------------
class wxSTEditorOptions_RefData : public wxObjectRefData, public wxClientDataContainer
{
public:
    wxSTEditorOptions_RefData() :
                                    m_steFRData(&wxSTEditorFindReplaceData::sm_findReplaceData),
                                    m_steFRData_static(true),
                                    m_steMM(NULL),
                                    m_steMM_static(false),
                                    m_fileHistory(NULL),
                                    m_fileHistory_static(false),
                                    m_menuBar(NULL),
                                    m_toolBar(NULL),
                                    m_statusBar(NULL),
                                    m_editorPopupMenu(NULL),
                                    m_splitterPopupMenu(NULL),
                                    m_notebookPopupMenu(NULL),
                                    m_editorPopupMenu_static(false),
                                    m_splitterPopupMenu_static(false),
                                    m_notebookPopupMenu_static(false),
                                    m_displayPathSeparator(wxPATH_NATIVE)
    {
        m_optionNames.Alloc(STE_OPTION__MAX);
        m_optionNames.Add(wxT("STE_OPTION_EDITOR"));
        m_optionNames.Add(wxT("STE_OPTION_SPLITTER"));
        m_optionNames.Add(wxT("STE_OPTION_NOTEBOOK"));
        m_optionNames.Add(wxT("STE_OPTION_FRAME"));
        m_optionNames.Add(wxT("STE_OPTION_CONFIG"));
        m_optionNames.Add(wxT("STE_OPTION_FINDREPLACE"));
        m_optionNames.Add(wxT("STE_OPTION_DEFAULT_FILENAME"));
        m_optionNames.Add(wxT("STE_OPTION_DEFAULT_FILEPATH"));
        m_optionNames.Add(wxT("STE_OPTION_DEFAULT_FILEEXTS"));
        m_optionNames.Add(wxT("STE_OPTION_CFGPATH_BASE"));
        m_optionNames.Add(wxT("STE_OPTION_CFGPATH_PREFS"));
        m_optionNames.Add(wxT("STE_OPTION_CFGPATH_STYLES"));
        m_optionNames.Add(wxT("STE_OPTION_CFGPATH_LANGS"));
        m_optionNames.Add(wxT("STE_OPTION_CFGPATH_FRAME"));
        m_optionNames.Add(wxT("STE_OPTION_CFGPATH_FILEHISTORY"));
        m_optionNames.Add(wxT("STE_OPTION_CFGPATH_FINDREPLACE"));

        m_optionValues.Add(wxEmptyString, STE_OPTION__MAX);
    }

    virtual ~wxSTEditorOptions_RefData()
    {
        if (m_steFRData && !m_steFRData_static)
            delete m_steFRData;
        if (m_steMM && !m_steMM_static)
            delete m_steMM;
        if (m_fileHistory && !m_fileHistory_static)
            delete m_fileHistory;
        if (m_editorPopupMenu && !m_editorPopupMenu_static)
            delete m_editorPopupMenu;
        if (m_splitterPopupMenu && !m_splitterPopupMenu_static)
            delete m_splitterPopupMenu;
        if (m_notebookPopupMenu && !m_notebookPopupMenu_static)
            delete m_notebookPopupMenu;
    }

    wxArrayString m_optionNames;
    wxArrayString m_optionValues;

    wxSTEditorPrefs  m_prefs;
    wxSTEditorStyles m_styles;
    wxSTEditorLangs  m_langs;

    wxSTEditorFindReplaceData *m_steFRData;
    bool m_steFRData_static;

    wxSTEditorMenuManager *m_steMM;
    bool m_steMM_static;

    wxFileHistory *m_fileHistory;
    bool m_fileHistory_static;

    wxMenuBar *m_menuBar;
    wxToolBar *m_toolBar;
    wxStatusBar *m_statusBar;
    wxMenu *m_editorPopupMenu;
    wxMenu *m_splitterPopupMenu;
    wxMenu *m_notebookPopupMenu;
    bool m_editorPopupMenu_static;
    bool m_splitterPopupMenu_static;
    bool m_notebookPopupMenu_static;

    wxPathFormat m_displayPathSeparator;
};

#define STEO_REFDATA ((wxSTEditorOptions_RefData*)m_refData)

wxSTEditorOptions::wxSTEditorOptions()
{
    m_refData = new wxSTEditorOptions_RefData(); // always created
}

wxSTEditorOptions::wxSTEditorOptions(long editor_opt,
                                     long splitter_opt,
                                     long notebook_opt,
                                     long frame_opt,
                                     long config_opt,
                                     const wxString& defaultFileName,
                                     const wxString& defaultFilePath,
                                     const wxString& defaultFileExt )
{
    m_refData = new wxSTEditorOptions_RefData();

    SetOptionInt(STE_OPTION_EDITOR,      editor_opt);
    SetOptionInt(STE_OPTION_SPLITTER,    splitter_opt);
    SetOptionInt(STE_OPTION_NOTEBOOK,    notebook_opt);
    SetOptionInt(STE_OPTION_FRAME,       frame_opt);
    SetOptionInt(STE_OPTION_CONFIG,      config_opt);
    SetOptionInt(STE_OPTION_FINDREPLACE, STE_FR_DEFAULT_OPTIONS);

    SetOption(STE_OPTION_DEFAULT_FILENAME, defaultFileName);
    SetOption(STE_OPTION_DEFAULT_FILEPATH, defaultFilePath);
    SetOption(STE_OPTION_DEFAULT_FILEEXTS, defaultFileExt);

    SetOption(STE_OPTION_CFGPATH_BASE,        wxT("/wxSTEditor"));
    SetOption(STE_OPTION_CFGPATH_PREFS,       wxT("Preferences"));
    SetOption(STE_OPTION_CFGPATH_STYLES,      wxT("Styles"));
    SetOption(STE_OPTION_CFGPATH_LANGS,       wxT("Languages"));
    SetOption(STE_OPTION_CFGPATH_FRAME,       wxT("Frame"));
    SetOption(STE_OPTION_CFGPATH_FILEHISTORY, wxT("RecentFiles"));
    SetOption(STE_OPTION_CFGPATH_FINDREPLACE, wxT("FindReplace"));

    SetUseGlobalPrefsStylesLangs();

    SetFindReplaceData(&wxSTEditorFindReplaceData::sm_findReplaceData, true);
    SetMenuManager(new wxSTEditorMenuManager(0), false);
}

size_t wxSTEditorOptions::GetOptionCount() const
{
    return STEO_REFDATA->m_optionValues.GetCount();
}

wxString wxSTEditorOptions::GetOption(size_t option_n) const
{
    return STEO_REFDATA->m_optionValues[option_n];
}
void wxSTEditorOptions::SetOption(size_t option_n, const wxString& value)
{
    STEO_REFDATA->m_optionValues[option_n] = value;
}

wxString wxSTEditorOptions::GetOptionName(size_t option_n) const
{
    return STEO_REFDATA->m_optionNames[option_n];
}
void wxSTEditorOptions::SetOptionName(size_t option_n, const wxString& name)
{
    STEO_REFDATA->m_optionNames[option_n] = name;
}
int wxSTEditorOptions::FindOptionByName(const wxString& name) const
{
    return STEO_REFDATA->m_optionNames.Index(name);
}

size_t wxSTEditorOptions::AddOption(const wxString& name, const wxString& value)
{
    STEO_REFDATA->m_optionNames.Add(name);
    STEO_REFDATA->m_optionValues.Add(value);
    return STEO_REFDATA->m_optionValues.GetCount() - 1;
}

wxSTEditorPrefs&  wxSTEditorOptions::GetEditorPrefs() const             { return STEO_REFDATA->m_prefs; }
wxSTEditorStyles& wxSTEditorOptions::GetEditorStyles() const            { return STEO_REFDATA->m_styles; }
wxSTEditorLangs&  wxSTEditorOptions::GetEditorLangs() const             { return STEO_REFDATA->m_langs; }
void wxSTEditorOptions::SetEditorPrefs(const wxSTEditorPrefs& prefs)    { STEO_REFDATA->m_prefs  = prefs; }
void wxSTEditorOptions::SetEditorStyles(const wxSTEditorStyles& styles) { STEO_REFDATA->m_styles = styles; }
void wxSTEditorOptions::SetEditorLangs(const wxSTEditorLangs& langs)    { STEO_REFDATA->m_langs  = langs; }
void wxSTEditorOptions::SetUseGlobalPrefsStylesLangs()
{
    STEO_REFDATA->m_prefs  = wxSTEditorPrefs::GetGlobalEditorPrefs();
    STEO_REFDATA->m_styles = wxSTEditorStyles::GetGlobalEditorStyles();
    STEO_REFDATA->m_langs  = wxSTEditorLangs::GetGlobalEditorLangs();
}

wxSTEditorFindReplaceData* wxSTEditorOptions::GetFindReplaceData() const
{
    return STEO_REFDATA->m_steFRData;
}
void wxSTEditorOptions::SetFindReplaceData(wxSTEditorFindReplaceData* steFRdata, bool is_static)
{
    if (STEO_REFDATA->m_steFRData && !STEO_REFDATA->m_steFRData_static)
        delete STEO_REFDATA->m_steFRData;

    STEO_REFDATA->m_steFRData        = steFRdata;
    STEO_REFDATA->m_steFRData_static = is_static;
}

wxSTEditorMenuManager* wxSTEditorOptions::GetMenuManager() const
{
    return STEO_REFDATA->m_steMM;
}
void wxSTEditorOptions::SetMenuManager(wxSTEditorMenuManager* steMM, bool is_static)
{
    if (STEO_REFDATA->m_steMM && !STEO_REFDATA->m_steMM_static)
        delete STEO_REFDATA->m_steMM;

    STEO_REFDATA->m_steMM        = steMM;
    STEO_REFDATA->m_steMM_static = is_static;
}

wxFileHistory* wxSTEditorOptions::GetFileHistory() const
{
    return STEO_REFDATA->m_fileHistory;
}
void wxSTEditorOptions::SetFileHistory(wxFileHistory* fileHistory, bool is_static)
{
    if (STEO_REFDATA->m_fileHistory && !STEO_REFDATA->m_fileHistory_static)
        delete STEO_REFDATA->m_fileHistory;

    STEO_REFDATA->m_fileHistory        = fileHistory;
    STEO_REFDATA->m_fileHistory_static = is_static;
}

wxMenuBar* wxSTEditorOptions::GetMenuBar() const        { return STEO_REFDATA->m_menuBar; }
wxToolBar* wxSTEditorOptions::GetToolBar() const        { return STEO_REFDATA->m_toolBar; }
wxStatusBar* wxSTEditorOptions::GetStatusBar() const    { return STEO_REFDATA->m_statusBar; }
wxMenu* wxSTEditorOptions::GetEditorPopupMenu() const   { return STEO_REFDATA->m_editorPopupMenu; }
wxMenu* wxSTEditorOptions::GetSplitterPopupMenu() const { return STEO_REFDATA->m_splitterPopupMenu; }
wxMenu* wxSTEditorOptions::GetNotebookPopupMenu() const { return STEO_REFDATA->m_notebookPopupMenu; }
wxPathFormat wxSTEditorOptions::GetDisplayPathSeparator() const  { return STEO_REFDATA->m_displayPathSeparator; } // maybe use GetOptionInt() instead
void wxSTEditorOptions::SetDisplayPathSeparator(wxPathFormat display_fmt) { STEO_REFDATA->m_displayPathSeparator = display_fmt; } // maybe use SetOptionInt() instead

void wxSTEditorOptions::SetMenuBar(wxMenuBar* menuBar)
{
    if (menuBar == STEO_REFDATA->m_menuBar) return;
    // if setting new menubar, remove filehistory from old
    if (STEO_REFDATA->m_menuBar && STEO_REFDATA->m_fileHistory)
    {
        for (size_t n = 0; n < STEO_REFDATA->m_menuBar->GetMenuCount(); n++)
            STEO_REFDATA->m_fileHistory->RemoveMenu(STEO_REFDATA->m_menuBar->GetMenu(n));
    }

    STEO_REFDATA->m_menuBar = menuBar;
}
void wxSTEditorOptions::SetToolBar(wxToolBar* toolBar)       { STEO_REFDATA->m_toolBar = toolBar; }
void wxSTEditorOptions::SetStatusBar(wxStatusBar* statusBar) { STEO_REFDATA->m_statusBar = statusBar; }
void wxSTEditorOptions::SetEditorPopupMenu(wxMenu* menu, bool is_static)
{
    if (STEO_REFDATA->m_editorPopupMenu && STEO_REFDATA->m_fileHistory)
        STEO_REFDATA->m_fileHistory->RemoveMenu(STEO_REFDATA->m_editorPopupMenu);
    if (STEO_REFDATA->m_editorPopupMenu && !STEO_REFDATA->m_editorPopupMenu_static)
        delete STEO_REFDATA->m_editorPopupMenu;

    STEO_REFDATA->m_editorPopupMenu = menu;
    STEO_REFDATA->m_editorPopupMenu_static = is_static;
}
void wxSTEditorOptions::SetSplitterPopupMenu(wxMenu* menu, bool is_static)
{
    if (STEO_REFDATA->m_splitterPopupMenu && STEO_REFDATA->m_fileHistory)
        STEO_REFDATA->m_fileHistory->RemoveMenu(STEO_REFDATA->m_splitterPopupMenu);
    if (STEO_REFDATA->m_splitterPopupMenu && !STEO_REFDATA->m_splitterPopupMenu_static)
        delete STEO_REFDATA->m_splitterPopupMenu;

    STEO_REFDATA->m_splitterPopupMenu = menu;
    STEO_REFDATA->m_splitterPopupMenu_static = is_static;
}
void wxSTEditorOptions::SetNotebookPopupMenu(wxMenu* menu, bool is_static)
{
    if (STEO_REFDATA->m_notebookPopupMenu && STEO_REFDATA->m_fileHistory)
        STEO_REFDATA->m_fileHistory->RemoveMenu(STEO_REFDATA->m_notebookPopupMenu);
    if (STEO_REFDATA->m_notebookPopupMenu && !STEO_REFDATA->m_notebookPopupMenu_static)
        delete STEO_REFDATA->m_notebookPopupMenu;

    STEO_REFDATA->m_notebookPopupMenu = menu;
    STEO_REFDATA->m_notebookPopupMenu_static = is_static;
}

/*static*/ void wxSTEditorOptions::RegisterIds()
{
    wxRegisterId(ID_STE__LAST); // TODO: how to do this right?
}

void wxSTEditorOptions::SetClientObject( wxClientData *data )
{
    wxCHECK_RET(STEO_REFDATA, wxT("invalid wxSTEditorOptions"));
    STEO_REFDATA->SetClientObject(data);
}
wxClientData *wxSTEditorOptions::GetClientObject() const
{
    wxCHECK_MSG(STEO_REFDATA, NULL, wxT("invalid wxSTEditorOptions"));
    return STEO_REFDATA->GetClientObject();
}
void wxSTEditorOptions::SetClientData( void *data )
{
    wxCHECK_RET(STEO_REFDATA, wxT("invalid wxSTEditorOptions"));
    STEO_REFDATA->SetClientData(data);
}
void *wxSTEditorOptions::GetClientData() const
{
    wxCHECK_MSG(STEO_REFDATA, NULL, wxT("invalid wxSTEditorOptions"));
    return STEO_REFDATA->GetClientData();
}

// static
wxString wxSTEditorOptions::GetGlobalDefaultFileName() { return STE_DefaultFileName; }
// static
void wxSTEditorOptions::SetGlobalDefaultFileName(const wxString& fileName) { STE_DefaultFileName = fileName; }

// static
wxString wxSTEditorOptions::GetGlobalDefaultExtensions() { return STE_DefaultFileExtensions; }
// static
void wxSTEditorOptions::SetGlobalDefaultFileExtensions(const wxString& fileExt) { STE_DefaultFileExtensions = fileExt; }

wxString wxSTEditorOptions::GetConfigPath(size_t path_option_n) const
{
    wxString basePath   = GetOption(STE_OPTION_CFGPATH_BASE);
    wxString optionPath = GetOption(path_option_n);

    // the optionPath is absolute
    if (optionPath.Length() && (optionPath[0] == wxT('/')))
        return optionPath;

    return FixConfigPath(basePath, true) + optionPath;
}

// static
wxString wxSTEditorOptions::FixConfigPath(const wxString& path, bool add_sep)
{
    if (add_sep && (!path.Length() || (path.Last() != wxT('/'))))
        return path + wxT("/");
    if (!add_sep && path.Length() && (path.Last() == wxT('/')))
        return path.Mid(0, path.Length()-1);

    return path;
}

void wxSTEditorOptions::LoadConfig(wxConfigBase &config)
{
    if (HasConfigOption(STE_CONFIG_PREFS) && GetEditorPrefs().IsOk())
        GetEditorPrefs().LoadConfig(config, GetConfigPath(STE_OPTION_CFGPATH_PREFS));
    if (HasConfigOption(STE_CONFIG_STYLES) && GetEditorStyles().IsOk())
        GetEditorStyles().LoadConfig(config, GetConfigPath(STE_OPTION_CFGPATH_STYLES));
    if (HasConfigOption(STE_CONFIG_LANGS) && GetEditorLangs().IsOk())
        GetEditorLangs().LoadConfig(config, GetConfigPath(STE_OPTION_CFGPATH_LANGS));
}
void wxSTEditorOptions::SaveConfig(wxConfigBase &config)
{
    if (HasConfigOption(STE_CONFIG_PREFS) && GetEditorPrefs().IsOk())
        GetEditorPrefs().SaveConfig(config, GetConfigPath(STE_OPTION_CFGPATH_PREFS));
    if (HasConfigOption(STE_CONFIG_STYLES) && GetEditorStyles().IsOk())
        GetEditorStyles().SaveConfig(config, GetConfigPath(STE_OPTION_CFGPATH_STYLES));
    if (HasConfigOption(STE_CONFIG_LANGS) && GetEditorLangs().IsOk())
        GetEditorLangs().SaveConfig(config, GetConfigPath(STE_OPTION_CFGPATH_LANGS));

    if (GetEditorPrefs().IsOk() || GetEditorStyles().IsOk() || GetEditorLangs().IsOk())
        config.Flush(true); // what is current only?
}

void wxSTEditorOptions::LoadFileConfig( wxConfigBase &config)
{
    const wxString oldpath = config.GetPath();
    wxFileHistory *fileHistory = GetFileHistory();
    if (!fileHistory)
        return;

    wxString configPath = FixConfigPath(GetConfigPath(STE_OPTION_CFGPATH_FILEHISTORY), false);
    wxString value, key = configPath+wxT("/LastDir");
    if (config.Read(key, &value) && wxDirExists(value))
        SetDefaultFilePath(value);

    config.SetPath(configPath);
    fileHistory->Load(config);
    config.SetPath(oldpath);
}

void wxSTEditorOptions::SaveFileConfig(wxConfigBase &config)
{
    const wxString oldpath = config.GetPath();
    wxFileHistory *fileHistory = GetFileHistory();
    if (!fileHistory)
        return;

    wxString configPath = FixConfigPath(GetConfigPath(STE_OPTION_CFGPATH_FILEHISTORY), false);
    config.Write(configPath+wxT("/LastDir"), GetDefaultFilePath());

    config.SetPath(configPath);
    fileHistory->Save(config);
    config.SetPath(oldpath);
}

//-----------------------------------------------------------------------------
// wxSTEditorRefDataObject
// Derives from wxObject, to satisfy DECLARE_DYNAMIC_CLASS() and
// the wxWidgets RTTI system, so that an wxSTEditorRefData instance
// can be created like this,
// CLASSINFO(wxSTEditorRefDataObject)->CreateObject()
//-----------------------------------------------------------------------------

class wxSTEditorRefDataObject : public wxObject, public wxSTEditorRefData
{
    DECLARE_DYNAMIC_CLASS(wxSTEditorRefDataObject)
};
IMPLEMENT_DYNAMIC_CLASS(wxSTEditorRefDataObject, wxObject)
const wxClassInfo* STE_GlobalRefDataClassInfo = CLASSINFO(wxSTEditorRefDataObject);
