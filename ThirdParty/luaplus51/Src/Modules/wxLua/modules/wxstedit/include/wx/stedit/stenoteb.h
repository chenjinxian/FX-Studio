///////////////////////////////////////////////////////////////////////////////
// Name:        stenoteb.h
// Purpose:     wxSTEditorNotebook
// Author:      John Labenski, parts taken from wxGuide by Otto Wyss
// Modified by:
// Created:     11/05/2002
// Copyright:   (c) John Labenski, Otto Wyss
// Licence:     wxWidgets licence
///////////////////////////////////////////////////////////////////////////////

/// @file stenoteb.h
/// @brief wxSTEditorNotebook, a wxNotebook for managing wxSTEditorSplitters.

#ifndef _STENOTEB_H_
#define _STENOTEB_H_

#include <wx/notebook.h>
#include "wx/stedit/stedefs.h"

class WXDLLIMPEXP_FWD_STEDIT wxArrayFileName;
class WXDLLIMPEXP_FWD_STEDIT wxSTEditorTreeCtrl;

//-----------------------------------------------------------------------------
/// @class wxSTEditorNotebook
/// @brief A wxNotebook of wxSTEditorSplitters.
///
/// Updates the tab names and can keep them sorted provides a menu to add,
/// close, goto, save, pages.
//-----------------------------------------------------------------------------

class WXDLLIMPEXP_STEDIT wxSTEditorNotebook : public wxNotebook
{
public:
    wxSTEditorNotebook() : wxNotebook() { Init(); }
    wxSTEditorNotebook( wxWindow *parent, wxWindowID id,
                        const wxPoint& pos = wxDefaultPosition,
                        const wxSize& size = wxDefaultSize,
                        long style = 0,
                        const wxString& name = wxT("wxSTEditorNotebook") ) : wxNotebook()
    {
        Init();
        Create( parent, id, pos, size, style, name );
    }

    virtual ~wxSTEditorNotebook();
    virtual bool Destroy();

    bool Create( wxWindow *parent, wxWindowID id,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = 0,
                 const wxString& name = wxT("wxSTEditorNotebook") );

    /// Create and set the wxSTEditorOptions, call this after creation.
    virtual void CreateOptions(const wxSTEditorOptions& options);
    /// Get the editor option values.
    const wxSTEditorOptions& GetOptions() const { return m_options; }
    /// Get the editor option values.
    wxSTEditorOptions& GetOptions() { return m_options; }
    /// Set the options, the options will now be refed copies of the ones you send in.
    /// This can be used to detach the options for a particular editor from
    /// the rest of them.
    void SetOptions(const wxSTEditorOptions& options) { m_options = options; }

    /// Get the wxSTEditorTreeCtrl to manage.
    wxSTEditorTreeCtrl* GetSTEditorTreeCtrl() { return m_editorTreeCtrl; }
    /// Set a wxSTEditorTreeCtrl to manage.
    void SetSTEditorTreeCtrl(wxSTEditorTreeCtrl* editorTreeCtrl) { m_editorTreeCtrl = editorTreeCtrl; }

    /// enable/disable sending wxSTEditor events from children editors.
    void SetSendSTEEvents(bool send);

    /// Get the editor at the page (last focused), if -1 then get the current editor, else NULL.
    wxSTEditor *GetEditor(int page = -1);
    /// Get the splitter at this page, if -1 then get current splitter, else NULL.
    wxSTEditorSplitter *GetEditorSplitter(int page = -1);

    /// Find the page that this editor is in or -1 for none.
    int FindEditorPage(const wxSTEditor* editor);
    /// Find the page who's editor has this filename, returns first found or -1 for not found.
    int FindEditorPageByFileName(const wxFileName& fileName);
    /// Find the page who's editor has this id, returns first found or -1 for not found.
    int FindEditorPageById(wxWindowID win_id);

    /// Insert a blank wxSTEditorSplitter into the notebook.
    /// nPage = -1 means at end or if sorted then alphabetically.
    wxSTEditorSplitter* InsertEditorSplitter(int nPage, wxWindowID win_id,
                                             const wxString& title,
                                             bool bSelect = false);
    /// Insert a splitter of your own creation.
    bool InsertEditorSplitter(int nPage, wxSTEditorSplitter* splitter,
                              bool bSelect = false);

    /// Get/Set the maximum number of pages to allow, default is STN_NOTEBOOK_PAGES_MAX_DEFAULT.
    /// Absolute max is STN_NOTEBOOK_PAGES_MAX, menu IDs will confict if greater.
    size_t GetMaxPageCount() const { return m_stn_max_page_count; }
    void SetMaxPageCount(size_t count) { m_stn_max_page_count = (int)count; }

    /// Strip the path off the wxSTEditor::GetFileName() to use as tab name.
    wxString FileNameToTabName(const wxSTEditor* editor) const;

    /// Delete a page and if query_save_if_modified & modified call
    ///   wxSTEditor::QuerySaveIfModified().
    /// If !STN_ALLOW_NO_PAGES then add back a new blank page.
    /// @returns success - not canceled and anything done.
    bool ClosePage(int n, bool query_save_if_modified = true);
    /// Delete all pages.
    /// @param query_save_if_modified & modified call wxSTEditor::QuerySaveIfModified().
    /// @param except_this_page If > 0 then keep this page open, else close all.
    /// If !STN_ALLOW_NO_PAGES then add back a new blank page.
    /// @returns success - not canceled and anything done.
    bool CloseAllPages(bool query_save_if_modified = true, int except_this_page = -1);

    /// Add a new page with the given name, if name is "" popup dialog to ask name.
    virtual bool NewPage( const wxString& title = wxEmptyString );
    /// Load a single file into a new page, if fileName is empty use wxFileSelector.
    virtual bool LoadFile( const wxFileName&,
                           const wxString &extensions = wxEmptyString,
                           const wxString& encoding = wxEmptyString);
    /// Load file(s) into new page(s), if filenames is NULL, use wxFileDialog.
    virtual bool LoadFiles( wxArrayString *fileNames = NULL,
                            const wxString &extensions = wxEmptyString  );
    bool LoadFiles( const wxArrayFileName *fileNames,
                    const wxString &extensions = wxEmptyString  );
    /// Save all the opened files if modified.
    virtual void SaveAllFiles();

    /// Runs through all pages and if IsModified() popup a message box asking if the user wants to save the file.
    /// If the user presses wxID_YES the file is automatically saved.
    /// Note: Use EVT_CLOSE in frame before hiding the frame.
    ///       Check for wxCloseEvent::CanVeto and if it can't be vetoed use the
    ///       style wxYES_NO only since it can't be canceled.
    /// @returns false if wxCANCEL was pressed, else true.
    bool QuerySaveIfModified(int style = wxYES_NO|wxCANCEL);

    /// Tests wxSTEditor::CanSave() for each page.
    bool CanSaveAll();

    /// Update all the menu/tool items in the wxSTEditorOptions.
    virtual void UpdateAllItems();
    /// Update popupmenu, menubar, toolbar if any.
    virtual void UpdateItems(wxMenu *menu=NULL, wxMenuBar *menuBar=NULL, wxToolBar *toolBar=NULL);

    /// Find a string starting at the current page and incrementing pages
    ///   until one is found or wxNOT_FOUND (-1) for none found.
    /// Action is of type STE_FindStringType selects, goto, or do nothing.
    /// @returns The position in the page, use GetSelection to get new page number.
    int FindString(const wxString &str, STE_TextPos start_pos, int flags, int action);
    /// Replace all occurances of the find string with the replace string in all pages.
    /// If flags = -1 uses wxSTEditor::GetFindFlags().
    /// Pages will be filled with the number of different pages that have been modified.
    /// @returns The number of replacements.
    int ReplaceAllStrings(const wxString &findString,
                          const wxString &replaceString,
                          int flags = -1, int *pages = NULL);

    // -----------------------------------------------------------------------
    // implementation

    /// Create a wxSTEditorSplitter as needed for new pages, override if desired.
    virtual wxSTEditorSplitter *CreateSplitter(wxWindowID id = wxID_ANY);

    /// Sort the tabs, currently only STN_ALPHABETICAL_TABS is understood.
    void SortTabs(int style = STN_ALPHABETICAL_TABS);

    /// Update the goto and close submenus of the notebook tab popup menu.
    void UpdateGotoCloseMenu(wxMenu* menu, int startID);

    /// @name Overridden wxNotebook methods.
    /// These send EVT_STNOTEBOOK_PAGE_CHANGED events to help update UI (these just call base class).
    /// @{

    virtual bool AddPage(wxWindow *page, const wxString& text,
                         bool bSelect = false, int imageId = -1);
    virtual bool InsertPage(size_t nPage, wxNotebookPage *pPage,
                            const wxString& strText, bool bSelect = false,
                            int imageId = -1);
    virtual int  GetSelection() const;
    virtual int  SetSelection(size_t nPage);
    virtual bool DeletePage(size_t nPage);
    virtual bool RemovePage(size_t nPage);
    virtual bool DeleteAllPages();

    // Override for 2.6 compatibility
    //virtual bool InsertPage(int nPage, wxNotebookPage *pPage,
    //                        const wxString& strText, bool bSelect = false,
    //                        int imageId = -1) { return InsertPage((size_t)nPage, pPage, strText, bSelect, imageId); }
    //virtual int  SetSelection(int nPage) { return SetSelection((size_t)nPage); } /// Override for 2.6 compatibility
    //virtual bool DeletePage(int nPage)   { return DeletePage((size_t)nPage); } /// Override for 2.6 compatibility
    //virtual bool RemovePage(int nPage)   { return RemovePage((size_t)nPage); } /// Override for 2.6 compatibility

    /// @}

    void OnMenu(wxCommandEvent &event);
    virtual bool HandleMenuEvent(wxCommandEvent &event);
    void OnSTEState(wxSTEditorEvent &event);
#if defined(__WXMSW__) && (wxVERSION_NUMBER >= 2900)
    void OnLeftUp(wxMouseEvent &event);
#endif
    void OnRightUp(wxMouseEvent &event);
    void OnMiddleUp(wxMouseEvent &event);
    void OnPageChanged(wxNotebookEvent &event);
    void OnFindDialog(wxFindDialogEvent& event);

    void UpdatePageState();

protected:
    wxSTEditorOptions m_options;

    wxSTEditorTreeCtrl* m_editorTreeCtrl;

    int m_stn_selection;
    int m_stn_page_count;
    int m_stn_max_page_count;

    wxSTERecursionGuardFlag m_rGuard_OnMenu;
    wxSTERecursionGuardFlag m_rGuard_HandleMenuEvent;
    wxSTERecursionGuardFlag m_rGuard_OnFindDialog;
    wxSTERecursionGuardFlag m_rGuard_UpdatePageState;

private:
    void Init();
    DECLARE_EVENT_TABLE()
    DECLARE_DYNAMIC_CLASS(wxSTEditorNotebook)
};

#endif  // _STENOTEB_H_
