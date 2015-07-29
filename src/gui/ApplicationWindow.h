/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/ApplicationWindow.h
 *  Release    7.0
 *  Author     Martin Pettau
 *  Copyright  2003-2012 by the author

 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
****************************************************************************/

#ifndef _APPLICATIONWINDOW_H_
#define _APPLICATIONWINDOW_H_

#ifdef __GNUG__
#pragma interface "ApplicationWindow.h"
#endif

#include <wx/app.h>
#include <wx/frame.h>

class ApplicationWindow;
class AppMenuBar;
class ChildWindow;
class Document;
class DocumentManager;
class MainWindowControl;
class MainWindowStatusbar;
class MaitreyaToolbar;

class wxEvent;
class wxLog;
class wxListEvent;
class wxTextCtrl;


/*************************************************//**
*
* \brief application class managing life cycle of ApplicationWindow
*
******************************************************/
class MaitreyaApp : public wxApp
{
public:
	MaitreyaApp();

protected:
	ApplicationWindow *frame;
	virtual bool OnInit();
	virtual int OnExit();
};

DECLARE_APP( MaitreyaApp )

/*************************************************//**
*
* \brief main window of appilication
*
******************************************************/
class ApplicationWindow : public wxFrame
{
	DECLARE_CLASS( ApplicationWindow )
public:

	ApplicationWindow( const wxChar *title, int xpos, int ypos, int width, int height );
	~ApplicationWindow();
	void NewFile( const bool &useMultipleView, const int viewId = 0 );
	bool OpenFile( wxString );
	void CreateNewChild( Document* );

protected:
	void AddChild( ChildWindow* );
	void SetActiveChild( ChildWindow* );
	void OnChildClose( ChildWindow* );
	void OnOpenRecent( wxCommandEvent& );
	void OnFileNew( wxCommandEvent& );
	void OnFileOpen( wxCommandEvent& );
	void OnFileQuit( wxCommandEvent& );
	void OnNewChild( wxCommandEvent& );
	void OnChildCommand( wxCommandEvent& );
	void OnSaveConfig( wxCommandEvent& );
	void OnConfigure( wxCommandEvent& );
	void OnLocationDialog( wxCommandEvent& );
	void OnImportLocationDialog( wxCommandEvent& );
	void OnAppAbout( wxCommandEvent& );
	void OnAppHelp( wxCommandEvent& );
	void OnClose( wxCloseEvent& );
	void showStatusBar();
	void showToolBars();
	void OnToggleToolBars( wxCommandEvent& );
	void OnToggleStatusBar( wxCommandEvent& );
	void OnToggleIcons( wxCommandEvent& );
	void OnConfigChanged( wxCommandEvent& );
	void OnToolbarConfigChanged( wxCommandEvent& );
	void OnDropFilesEvent( wxDropFilesEvent& );
	wxStatusBar* OnCreateStatusBar(int number, long style, wxWindowID id, const wxString& name);
	void saveConfig();
	void OnEventTest( wxCommandEvent& );
	void OnListItemSelected( wxListEvent& );
	void OnChildFocusEvent( wxCommandEvent& );
	void OnChildCloseEvent( wxCommandEvent& );
	void OnNewChildEvent( wxCommandEvent& );
	void OnDocSavedEvent( wxCommandEvent& );
	void OnDocUpdateEvent( wxCommandEvent& );
	void OnCreateEntryChart( wxCommandEvent& );
	void OnIdle( wxIdleEvent& );

	void echo( const wxChar* );

	DocumentManager *docmanager;
	MainWindowStatusbar *statusbar;
	MainWindowControl *listctrl;
	wxLog *m_logTargetOld;
	wxTextCtrl *m_text;
	AppMenuBar *menubar;
	MaitreyaToolbar *maintoolbar;
	bool docommands, updateTools;
};

#endif

