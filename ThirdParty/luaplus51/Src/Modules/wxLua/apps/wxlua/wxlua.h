/////////////////////////////////////////////////////////////////////////////
// Purpose:     Standalone wxLua application
// Author:      John Labenski, Francis Irving, J. Winwood
// Created:     16/01/2002
// Copyright:   (c) 2012 John Labenski
// Copyright:   (c) 2002 Creature Labs. All rights reserved.
// Copyright:   (c) 2002 Lomtick Software. All rights reserved.
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef WX_LUAAPP_H
#define WX_LUAAPP_H

#include <wx/app.h>

#include "wxlua/debugger/wxldtarg.h"
#include "wxlua/wxlstate.h"
#include "wxlua/wxlconsole.h"

// ----------------------------------------------------------------------------
// wxLuaStandaloneApp
// ----------------------------------------------------------------------------

class wxLuaStandaloneApp : public wxApp
{
public:
    // Override the base class virtual functions
    virtual bool OnInit();
    virtual int  OnExit();

    void DisplayMessage(const wxString &msg, bool is_error,
                        const wxLuaState& wxlState = wxNullLuaState);

    void OnLua(wxLuaEvent &event);

    wxString            m_programName;
    wxLuaState          m_wxlState;
    bool                m_print_stdout;
    bool                m_dos_stdout;
    bool                m_print_msgdlg;
    bool                m_want_console;
    bool                m_mem_bitmap_added;
    wxLuaDebugTarget*   m_wxlDebugTarget;

private:
    DECLARE_EVENT_TABLE();
};

DECLARE_APP(wxLuaStandaloneApp)

#endif // WX_LUAAPP_H
