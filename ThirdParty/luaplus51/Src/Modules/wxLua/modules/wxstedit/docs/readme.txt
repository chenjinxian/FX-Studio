

****** README for wxStEditor ******
Copyright : 2005, John Labenski, Otto Wyss.
License : wxWindows license.

This library/program is free software; you can redistribute it and/or modify it
under the terms of the wxWindows License; either version 3 of the License, or
(at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the wxWindows License for more details.

If you use this program/library and find any bugs or have added any features
that you feel may be generally useful, please feel free to contact the author
by e-mail at jrl1[at]sourceforge[dot]net.

***** Summary : *****
wxStEdit is a library and sample program for the wxWidgets's wxStyledTextCtrl
wrapper around the Scintilla text editor widget. It provides a number of
convenience functions and added capabilities, including the necessary
preferences, styles, language management, menu creation and updating, a
splitter, notebook, and frame component. Additionally it provides a find/
replace, editor settings, and property dialogs. It is designed to be easily
integrated into a larger program and while it tries to manage as much as
possible, it's fairly extensible as well. Individual features and "helper"
functionality can be turned off or overridden if desired.

The bottom line; this editor system builds upon the wxStyledTextCtrl by adding
all the necessary code to ease the burden of providing a full featured editor
or sets of identically styled editors in a notebook or frame.

What does the ST in wxSTEditor stand for? Either styled text or Scintilla text,
your choice.

It has been tested in MS Windows OS >= 95 and *nix using GTK 1.2 or 2.0.
The recommended version of wxWidgets is 2.8 (>= 2.6).
The core for the editor was originally based on Otto Wyss's wxGuide editor.

***** References : *****
http://wxcode.sourceforge.net
http://www.wxwidgets.org
http://www.scintilla.org
http://bakefile.sourceforge.net

***** Provides : (at a glance) *****
**** The main GUI classes : ****
    * wxSTEditor : a subclassed wxStyledTextControl with additional
      functionality.
    * wxSTEditorSplitter : a subclassed wxSplitterWindow to split a wxSTEditor
      either horizontally or vertically.
    *     o Simplified splitter class that automatically handles splitting and
            unsplitting
    * wxSTEditorNotebook : a subclassed wxNotebook to manage multiple
      wxSTEditorSplitters.
    * wxSTEditorFrame : a subclassed wxFrame
    *     o Can create it's own wxMenuBar, wxToolBar, wxStatusBar or not
          o Create wxSTEditorNotebook or a single wxSTEditorSplitter as a child
            or neither
          o Can have typical IDE layout, vertical splitter with wxTreeCtrl on
            left for files opened, notebook on right for editing
**** Helper classes : ****
    * wxSTEditorEvent : alerts on status change to keep GUI updated
    * wxSTEditorRefData : easily allows splitting editors (used internally)
    * wxSTEditorPrefs : stores string/int/bool preferences for the editors
    * wxSTEditorStyles : stores colour/font styles for the editor
    * wxSTEditorLangs : stores lexer information, keywords, brackets
    * wxSTEditorOptions : options to control behavior of editor/splitter/frame
      and store data to be shared among editors
    * wxSTEditorMenuManager : create popup menu/menubar/toolbar, enable/check
      items
    * wxSTEditorFindReplacePanel : an embeddable find/replace panel
    * wxSTEditorFindReplaceDialog : full featured find/replace dialog
    * wxSTEditorFindReplaceData : complete find replace data
    * wxSTEditorPrintout : Managed wxPrintout, provides printing
    * wxSTEditorPrintOptionsDialog : Display Scintilla's print options
    * wxSTEditorExporter : Export file to RTF, HTML, PDF, TEX, XML
    * wxSTFrameFileDropTarget : a wxFileDrop target for the wxSTEditorFrame
    * wxSTEditorPropertiesDialog : file or editor properties dialog
    * wxSTEditorInsertTextDialog : insert text dialog (prepend, append, insert)
    * wxSTEditorPrefPageData : data for the prefs/styles/langs setup
    * wxSTEditorPrefDialog : complete preference, style, langs dialog
    * wxSTEditorPrefDialogPagePrefs : a generic page for any assortment of
      wxSTEditorPrefs
    * wxSTEditorPrefDialogPageStyles : colour styles page
    * wxSTEditorPrefDialogPageLangs : languages page
    * wxSTEditorShell : A console type interactive shell for user entry, eg. a
      DOS prompt.
**** Others : ****
    * Creation and display of right click menu for editor, splitter, notebook
      (or not).
    * Printing dialogs and print data management.
    * wxConfig saving and loading of prefs, styles, and langs in wxConfig::Get
      (false) is not NULL
    * Additional event notification events and more...

Each component of the library can be used independently of the higher level
controls. In other words you can use the wxSTEditor class independently of the
others. The wxSTEditorSplitter only depends on the wxSTEditor, the
wxSTEditorNotebook depends on the splitter and editor, and finally the
wxSTEditorFrame on any one or all of the above.
You can subclass each or any of the controls and use them in conjunction with
the others. The order of what control is parent to what is not important so
long as the virtual functions like GetEditor() work appropriately.

***** Example uses : (see also notes and code in the wxstedit sample program)
*****
You're making an IDE type program, use the notebook to manage your editor pages
keeping the styles and preferences between all of them in sync. Your output
window can also be assigned the same styles to make sure that it's colours are
the same as the editors, but not the preferences since the user shouldn't be
allowed to change anything. All editors should probably share the same
languages.
Maybe you simply need a syntax highlighting widget for the user to occasionally
edit/view some text in, just use the wxSTEditor widget itself.
The user needs to edit a file in a frame, just create a wxSTEditorFrame, create
your options for it, and set the text or load a file. Thats it!

You can build it as a library and link to it or just add the files to your
existing project. You typically need only include "wx/stedit/stedit.h".

***** Compilation : *****
**** NOTE:_You_must_copy_include/wx/stedit/setup0.h_to_setup.h ****
Compiles for wxWidgets version 2.6 and up, recommended version >= 2.6
Tested in Linux using gcc and wxGTK and wxMSW using MSVS 6&7.
**** Unix type systems using GCC and gmake : ****
*** Using the Makefiles ***
Use the Makefile in the src or samples/stedit directory which depends on the
wxWidget's 'wx-config' script being in the path. The Makefile in the /src
directory will create a library in the wxWidgets/lib directory using the
wxWidgets naming conventions. To create the library elsewhere, change the
variable $(WXLIBDIR) in the Makefile.

The Makefile in the sample/stedit dir will automatically try to build the
library first and so you can simply run make in the samples/stedit dir to build
both.

There is no "make install" for the sample, just copy or create a link to it
someplace in your path. You can use &#8220;make clean&#8221; or &#8220;make
cleanall&#8221; in the sample directory to rebuild the sources. The sample will
create a ~/.wxStEdit ini file.
*** Using configure ***
The configure script in the root directory can also be used to generate a
Makefile. For reference, the configure script is generated using bakefile and
autoconf files located in the build directory that you should not have to
modify. Run "./configure --help" to display the options for the configure
script.

The configure script looks for wx-config in the path and tries to adjust the
settings for compiling wxStEdit to match. To use multiple builds of wxWidgets
and therefore wxStEdit you can create a directory, set the path to the
appropriate wx-config, and run "/path/to/wxstedit/configure --prefix=/path/to/
created/wxstedit/build_dir".

After successfully running configure you then merely type "make" to build the
library. The output lib will be built into ./lib and the sample program in ./
samples/stedit in the directory you ran configure from.
**** MSW using MS Visual Studio >= 6 : ****
You must have the WXWIN environment variable set to the root directory of your
copy of the wxWidgets source to use the project files. In MS Win 9x you must
edit your c:\config.sys file and reboot. In 2k/XP you can right click on
&#8220;My Computer->Properties->Advanced->Environment Variables&#8221; and add
a user variable, you must restart MSVS for the change to take effect.

Open the project file build/wxstedit.dsw to open both build files
wxstedit_stedit.dsp for the lib and wxstedit_wxstedit.dsp for the sample.

Set the active project to wxstedit (the sample) and the build type that
corresponds to the settings you used to build wxWidgets or use batch build to
build.

The output library is placed in the ./lib dir and the sample program into the
samples/stedit/dir.

MSVC 2003 is missing the libs msvcprt.lib and msvcprtd.lib. You can get more
information from the website below about the 2003 compiler and also get the
release msvcprt lib. Just copy it to msvcprtd.lib for debug builds. http://
root.cern.ch/root/Procedure/
Procedure%20to%20install%20the%20free%20Microsoft%20Visual%20C.htm
**** Bakefile : ****
I am unable to test all of the configurations that bakefile can generate, use
the make and project files in the build directory the same way you would use
them in the wxWidget's samples directory.

Normally, you do not have to regenerate these... In the build directory is a
bakefile called &#8220;stedit .bkl&#8221;. To regenerate the make and project
files run &#8220;bakefile_gen" stedit.bkl&#8221;, to regenerate a single type,
like the Visual Studio project files for example, use &#8220;bakefile_gen -
f msvs stedit.bkl&#8221;.
The base files for bakefile are, stedit.bkl (the main bakefile), Bakefile.bkgen
(edit to point to stedit.bkl), and for gcc in linux aclocal.m4, acregen.sh,
config.guess, config.sub, configure.as.

***** Documentation : *****
The "full documentation" is included in the headers with a description of the
public functions. There is also a doxygen.cfg file in the "docs" directory that
you can use to create cross referenced html docs, please review and adjust the
file to your liking.

*** wxstedit.cpp : samples/stedit/wxstedit.cpp (wxstedit.exe for MSW, wxstedit
for *nix) ***
Sample program that uses the wxSTEditorFrame to create a standalone editor
executable. Its shows how little work is required to have a full featured text
editor. The wxstedit program uses a wxFileConfig meaning that NO registry
entries in MSW are created. Therefore no (un)installer needed. It only creates
a "Documents and Settings/user/wxStEdit.ini" in MSW's NT OSs and ~/.wxStEdit
*nix that you can delete if desired.

*** #defines and enums : include/wx/stedit/stedefs.h ***
**** NOTE:You must copy include/wx/stedit/setup0.h to setup.h ****
The include/wx/stedit/setup.h file is used to not compile in some of the data
for the different Scintialla lexers. Since there are quite a few of them and
each has their own keywords the amount of data is fairly sizeable, see
stelangs.cpp. By default all of them are turned on.
*** wxSTEditor : include/wx/stedit/stedit.h ***
Subclassed from the wxStyledTextControl it provides some simplified methods and
many additional features. The internal data it uses is the ref counted
wxSTEditorData so that you can ref an editor (splitting for example) and the
rest of the data goes with it.
    * The editor can be used independently of the other gui classes
    * Has wxTextCtrl compatibility functions so it can be used as a drop-in
      replacement in some cases
    * Adds to the wxStyledTextCtrl a number of functions including, but not
      limited to
    *     o PasteRectangular() - rectangular pasting
          o AppendTextGotoEnd() - useful for log type windows
          o GetWordCount(), GetEOLCount(), ...
          o Code folding simplification functions
          o ConvertTabsToSpaces() or reverse, RemoveTrailingWhitespace()
          o Columize() - align text to be in columns
          o Dialogs to set eol mode, columize, zoom, goto line, properties
          o Load/Save that is unicode aware
          o Export to a variety of formats
          o IsAlteredOnDisk() - check if file has been modified
          o Indicator convenience functions
          o Printing and dialogs associated with it
          o Updates all menu/toolbar items assocated with it when it has focus
          o Highlight matching braces
    * Alerts parent of changes to Cut/Copy/Paste/Save... ability (for menus)
    * Event at the end of the constructor to allow parent classes to add/change
      features and styles to the editor without having to subclass it.
    * Focus event so parent can know who's focused and update GUI items.
    * Set Toolbar and MenuBar for the editor to manage and update as necessary.
    * Find/Replace dialog management that automatically finds a splitter or
      notebook parent so that it works in both splitter panes or on all the
      notebook pages.
    * Customizeable popup right click dialog, or set your own
    * Function to handle menu events, returning if anything was done.
    * Load/Save config to a wxConfigBase class
    * Abililty to have preferences/styles/languages managed by wxSTEditorPrefs/
      Styles/Langs (or some or none)

*** wxSTEditorSplitter : include/wx/stedit/stesplit.h ***
wxSplitter window to split editor document horizontally or vertically. Can have
it's own popup menu to aid in (un)splitting. Additionaly, it gets rid of gtk
errors when wxSTC is in a notebook, so it's a good idea to always use this even
if you don't want to allow splitting.

*** wxSTEditorNotebook : include/wx/stedit/stenoteb.h ***
A notebook that can manage the tab names, adds a right click menu to add and
remove pages. Also handles loading/saving multiple files and searching though
all files if the find replace dialog has this as a parent. It's intended to
have wxSTEditorSplitter pages, but wxDynamicCast is used to verify each page
before any action is taken and if you have used some other type of window it
will be left alone.

*** wxSTEditorFrame : include/wx/stedit/steframe.h ***
A wxFrame that can have a toolbar or not and either a single page editor or a
notebook for them. As with the notebook, it only adjusts the children that it
has created or that your subclassed frame returns in the virtual functions to
get the notebook, splitter, or editor.

*** wxSTEditorFindReplaceData : include/wx/stedit/stefindr.h ***
*** wxSTEditorFindReplacePanel : ***
*** wxSTEditorFindReplaceDialog : ***
An enhanced find/replace dialog that uses all the find/replace functionality of
Scintilla. It stores multiple find/replace strings and shows them in
wxComboBoxes. It operates in the same way as the wxFindReplaceDialog by sending
events back to the parent. Use wxSTEditor::ShowFindReplaceDialog and it will
create one for you. If the editor's parent is a splitter then the splitter will
be the parent of the dialog, if grandparent is the notebook then that will be
the parent. That way you can split/unsplit without loosing the dialog and
search through all the notebook pages.

*** wxSTEditorOptions : include/wx/stedit/steopts.h ***
A simple class that stores different options for the editor, splitter,
notebook, frame, config (wxConfig), and menu items. These are options to
enable/disable features provided by this library, as opposed to the
wxSTEditorPrefs that only store prefs for the wxStyledTextCtrl. It also stores
popup menus for the editor, splitter, notebook and a menubar and toolbar that
you'll want to be updated.

The options class is passed from parent to child so that once you set it up the
way you want any children will use the same options. You can (of course) turn
off all the options and then you're back to the wxStyledTextCtrl's behavior.

*** wxSTEditorMenuManager : include/wx/stedit/stemenum.h ***
This is a class to create a full set of menus and add items to a toolbar. You
can control what menus are created by adjusting ored together enum values. The
functions can either create new menus for you or just add items to the menu you
send in.

The menu manager class also has static functions to update menu and toolbar
items that won't fail if the item doesn't exist.

*** wxSTEditorPrintout : include/wx/stedit/steprint.h ***
Print management/preview window. You can use your own wxPrintData and/or
wxPageSetupData or this will create it's own copy and delete it when done.

*** wxSTEditorPrintOptionsDialog : include/wx/stedit/steprint.h ***
A print options dialog to adjust how Scintilla will print the pages. Set the
font zoom, wrap lines, and colour mode.

*** wxSTEditorPrefDialog : include/wx/stedit/stedlgs.h ***
*** wxSTEditorPrefDialogPageBase... ***
Shows all the preferences that the wxSTC can have in a compact dialog. Also
lets you setup the styles (colouring for the editor) and view how the lexer
handles different languages.

*** wxSTEditorPropertiesDialog : include/wx/stedit/stedlgs.h ***
Gives for the properties for the editor page, word count, eol, etc.

*** wxSTEditorPrefs : include/wx/stedit/steprefs.h ***
A ref counted preference class for the wxSTEditor that controls ALL of
Scintilla's (wxSTEditor's) bool/int settings. This can get's attached to one or
many editors by calling wxSTEditor::RegisterPrefs(myPrefs). It updates all the
editors to have the same preferences automatically. This stores all the bool/
int wxSTC preferences like tab width, wraplines, etc... Has functions to update
menu items and load/save to a wxConfigBase.

*** wxSTEditorStyles : include/wx/stedit/steprefs.h ***
A ref counted styles class to stores all the styles that the wxSTC uses for
colourizing a document. Comes with predefined values for styles 0-37 which you
can (of course) change. Also stores some GUI styles like cursor colour and what
not as well as the indicator colours and can load/save to a wxConfigBase.

A note on Scintilla's styles: Scintilla uses styles numbered 0-127, with 33-37
used for gui colouring, see STE_StyleType. Different lexers (languages) use
different styles with different style numbers, they're unfortunately almost
random, but they do start from 0 and are (mostly) contiguous. To simplify this
a fixed set of styles are created numbered 0-32 that reasonably cover most of
the styles used by the lexers. For example, any language that has a comment
style will use STE_STYLE_COMMENT for that. Therefore you need only setup the
styles 0-32 and any language should be appropriately styled. This way when a
user wants to do something as simple as change the colors to white on black
they only have to change it once and it works for any language.

*** wxSTEditorLangs : include/wx/stedit/steprefs.h ***
A ref counted language class for the wxSTC lexer to use. It uses the
wxSTEditorStyles to set the colours and stores it's own words for each
language. You probably only need one of these since the only thing that you can
currently change is what file extensions to associate with what files. It
contains information about every one of Scintilla's lexers, some may be more
complete than others however and I welcome any help understanding the meaning
of the styles for the more esoteric languages.

***** Notes : *****
*** IMPORTANT! ***
Some systems may send focus events when closing the app. This can happen if,
for example, a modal dialog is shown to ask the user to save a file before
closing and the user very quickly and incessantly clicks to close the dialog
and then on a window in the closing frame. Since the wxSTEditor sends it's own
focus event to the parents to help you update toolbars and whatnot, the window
gets destroyed behind your back and causes a segfault. The benefits of tracking
the focus (instead of using UpdateUI events) outweighs the extra care required.
The solution is simple however! Call the topmost window's wxSTEditor/Splitter/
Notebook/Frame::SetSendSTEEvents(false) in your wxApp::OnExit or in your
wxFrame's EVT_CLOSE handler to block all wxSTEditor event processing for the
window and all it's children.

All wxSTEditorOptions are passed down to the children of each window. For
example, when you create a wxSTEditorFrame and call frame->CreateOptions all
the children will be created and then CreateOptions with the parent's options
called on them.

Events are passed to each child, eg. frame gets an event and calls bool
HandleEvent for the notebook which calls HandleEvent for the splitter which
calls HandleEvent for the current editor. Each HandleEvent function returns
true if it knows about the event ID and has dealt with it. All the known event
IDs are in stedefs.h

There are global versions of the preferences, styles, and languages that are
always created. You should use these for at least one "set" of editors and
create new ones for other editors if you want to allow different prefs/styles
for different editors. (you probably don't need to create multiple languages)


This file was edited using the Nvu composer, tidied with "tidy -i -w 79 -
o readme.h readme.htm" and converted to text using "html2text -nobs -ascii -
o readme.txt readme.htm".

