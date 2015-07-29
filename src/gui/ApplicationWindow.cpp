/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/ApplicationWindow.cpp
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

#ifdef __GNUG__
#pragma implementation "ApplicationWindow.h"
#endif

#include "ApplicationWindow.h"
#include "config.h"

#include <wx/string.h>

#include <wx/cmdline.h>
#include <wx/config.h>
#include <wx/filedlg.h>
#include <wx/filename.h>
#include <wx/image.h>
#include <wx/listbase.h>
#include <wx/mimetype.h>
#include <wx/log.h>
#include <wx/sizer.h>
#include <wx/sysopt.h>
#include <wx/textctrl.h>

#include "AtlasDialog.h"
#include "ChildWindow.h"
#include "Document.h"
#include "IconProvider.h"
#include "MainWindowControl.h"
#include "MenuProvider.h"
#include "SingletonLifeline.h"
#include "Statusbar.h"
#include "ToolPanel.h"

#include "AboutDialog.h"
#include "ConfigDialog.h"

#include "constants.h"
#include "Calculator.h"
#include "Conf.h"
#include "FileConfig.h"
#include "guibase.h"
#include "Varga.h"

Config *config;

IMPLEMENT_APP( MaitreyaApp )
IMPLEMENT_CLASS( ApplicationWindow, wxFrame )

/*****************************************************
**
**   MaitreyaApp   ---   Constructor
**
******************************************************/
MaitreyaApp::MaitreyaApp()
		: wxApp()
{
}

/*****************************************************
**
**   MaitreyaApp   ---   OnInit
**
******************************************************/
bool MaitreyaApp::OnInit()
{
	wxString s;
	int screenx, screeny;
	int startarg = 1;

	SetAppName( APP_NAME );
	SetVendorName( VENDOR_NAME );

	s = wxT( "." );

#if defined( __WXMSW__)

	// Nothing to do

#elif defined( __WXMAC__ )
	wxSystemOptions::SetOption( wxT( "mac.listctrl.always_use_generic" ), 1 );
	s = MAC_RESOURCE_DIRECTORY;
#else  // Gtk, universal, expecting parameter for data directory
	if ( argc <= 1 )
	{
		printf( "WARN: missing parameter for datadir, assuming '..'\n" );
		printf( "Please call shell script wrapper 'maitreya6' from the same directory\n" );
	}
	else
	{
		s = argv[1];
		startarg = 2;
	}
#endif
	FileConfig::get()->init( s );

	::wxInitAllImageHandlers(); // maybe without gif handler

	/* Adjust main window size and position if necessary.
	   Strange coordinates can occur if main window is minimized on close etc.
	*/
	wxDisplaySize( &screenx, &screeny );
	if ( config->xMasterWindow <= 0 || config->xMasterWindow > screenx
	        || config->yMasterWindow <= 0 || config->yMasterWindow > screeny
	        || config->xPosMasterWindow <= 0 || config->xPosMasterWindow > screenx
	        || config->yPosMasterWindow <= 0 || config->yPosMasterWindow > screeny )
	{
		// Set default coordinates
		config->xMasterWindow = 720;
		config->yMasterWindow = 200;
		config->xPosMasterWindow = 50;
		config->yPosMasterWindow = 50;
	}
	frame = new ApplicationWindow( GUI_APP_NAME, config->xPosMasterWindow, config->yPosMasterWindow,
	                               config->xMasterWindow, config->yMasterWindow );

	frame->Show( true );
	//SetTopWindow(frame);

	if ( argc <= startarg )
	{
		if ( config->openNewDocOnStart ) frame->NewFile( config->useMultipleViews, -1 );
	}
	for ( int i = startarg; i < argc; i++ )
	{
		frame->OpenFile( argv[i] );
	}
#ifdef __WXMSW__
	frame->DragAcceptFiles( true );
#endif
	return true;
}

/*****************************************************
**
**   MaitreyaApp   ---   OnExit
**
******************************************************/
int MaitreyaApp::OnExit()
{
	SingletonLifeline().cleanup();
	return 0;
}

/*****************************************************
**
**   ApplicationWindow   ---   Constructor
**
******************************************************/
ApplicationWindow::ApplicationWindow(const wxChar *title, int xpos, int ypos, int width, int height)
		: wxFrame( 0, -1, title, wxPoint(xpos, ypos), wxSize(width, height))
{
	wxString rfiles, s;

	docommands = true;
	listctrl = new MainWindowControl( this, -1 );
	listctrl->Show( true );
	if ( config->logMode == 1 )
	{
		m_text = new wxTextCtrl(this, wxID_ANY, wxT("Log window.\n"),
		                        wxPoint(0, 250), wxSize(100, 150), wxTE_MULTILINE);
		m_logTargetOld = wxLog::SetActiveTarget(new wxLogTextCtrl(m_text));
		wxBoxSizer* sizer = new wxBoxSizer( wxVERTICAL );
		sizer->Add( listctrl, 1, wxEXPAND | wxALL, 3 );
		sizer->Add( m_text, 1, wxEXPAND, 0 );
		SetSizer( sizer );
		sizer->Fit( this );
		//Layout();
		SetSize( config->xMasterWindow, config->yMasterWindow );
	}
	else if ( config->logMode == 2 )
	{
		m_logTargetOld = wxLog::SetActiveTarget(new wxLogWindow( this, wxT( "Log Window" )));
		//wxLogWindow *log = new wxLogWindow( this );
	}
	else
	{
		(void)new wxLogNull();
		//wxLog *log = new wxLogStream( &cout );
		//wxLog::SetActiveTarget( log );
	}
	SetIcon( IconProvider::getIcon( BITMAP_MAITREYA  ));

	docmanager = new DocumentManager;
	menubar = new AppMenuBar( wxMB_DOCKABLE );
	SetMenuBar( menubar );
	maintoolbar = ToolbarFactory::get()->createMainToolbar( this );
	maintoolbar->Realize();
	SetToolBar( maintoolbar );
	showToolBars();
	statusbar = (MainWindowStatusbar*)CreateStatusBar();
	showStatusBar();

#if defined( __WXMAC__ )
	// Otherwise window will grow on each startup
	SetSize( width, height );
#endif

	Connect( CMD_CHILD_FIRST+1, CMD_CHILD_GRAPHIC_STYLE+30, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ApplicationWindow::OnChildCommand ));
	Connect( APP_FILENEW, APP_FILE_NEW_MULTIPLE + 100, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ApplicationWindow::OnFileNew ));
	Connect( APP_FILEOPEN, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ApplicationWindow::OnFileOpen ));
	Connect( APP_FILEQUIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ApplicationWindow::OnFileQuit ));
	Connect( APP_SHOWSTATUSBAR, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ApplicationWindow::OnToggleStatusBar ));
	Connect( APP_SHOWMAINTOOLBAR, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ApplicationWindow::OnToggleToolBars ));
	Connect( APP_SAVECONFIG, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ApplicationWindow::OnSaveConfig ));
	Connect( APP_CONFIGURE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ApplicationWindow::OnConfigure ));
	Connect( APP_LOCATION, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ApplicationWindow::OnLocationDialog ));
	Connect( APP_ABOUT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ApplicationWindow::OnAppAbout ));
	Connect( APP_HELP, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ApplicationWindow::OnAppHelp ));

	Connect( APP_LIST_BIGICONS, APP_LIST_TABLE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ApplicationWindow::OnToggleIcons ));
	Connect( APP_SHOWHORA, APP_SHOWEPHEM, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ApplicationWindow::OnNewChild ));
	Connect( APP_FILERECENT, APP_FILERECENT+35, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ApplicationWindow::OnOpenRecent ));

	Connect( wxID_ANY, CONFIG_CHANGED, wxCommandEventHandler( ApplicationWindow::OnConfigChanged ));
	Connect( wxID_ANY, CONFIG_TOOLBAR_CHANGED, wxCommandEventHandler( ApplicationWindow::OnToolbarConfigChanged ));
	Connect( wxID_ANY, CHILD_SETACTIVE, wxCommandEventHandler( ApplicationWindow::OnChildFocusEvent ));
	Connect( wxID_ANY, CHILD_CLOSED, wxCommandEventHandler( ApplicationWindow::OnChildCloseEvent ));
	Connect( wxID_ANY, CREATE_ENTRY_CHART, wxCommandEventHandler( ApplicationWindow::OnCreateEntryChart ));
	Connect( wxID_ANY, DOC_SAVED, wxCommandEventHandler( ApplicationWindow::OnDocSavedEvent ));
	Connect( wxID_ANY, DOC_UPDATED, wxCommandEventHandler( ApplicationWindow::OnDocUpdateEvent ));

	Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( ApplicationWindow::OnClose ));
	Connect( wxEVT_DROP_FILES, wxDropFilesEventHandler( ApplicationWindow::OnDropFilesEvent ));
	Connect( LISTCTRL_ID, wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( ApplicationWindow::OnListItemSelected ));
	Connect( wxEVT_IDLE, wxIdleEventHandler( ApplicationWindow::OnIdle ));

	updateTools = true;
}

/*****************************************************
**
**   ApplicationWindow   ---   Destructor
**
******************************************************/
ApplicationWindow::~ApplicationWindow()
{
	delete docmanager;
	delete config;
}

/*****************************************************
**
**   ApplicationWindow   ---   OnOpenRecent
**
******************************************************/
void ApplicationWindow::OnOpenRecent( wxCommandEvent &event )
{
	wxLogMessage( wxT( "ApplicationWindow::OnOpenRecent eventid %d" ), event.GetId() );
	OpenFile( menubar->getRecentFilesEntry( event.GetId()) );
	updateTools = true;
}


/*****************************************************
**
**   ApplicationWindow   ---   SetActiveChild
**
******************************************************/
void ApplicationWindow::SetActiveChild( ChildWindow *child )
{
	updateTools = true;
	if ( ! listctrl->getNumberOfEntries() ) return;
	for ( long i = 0; i < listctrl->getNumberOfEntries(); i++ )
	{
		if ( listctrl->getChild(i) == child )
		{
			listctrl->selectItem(i );
			return;
		}
	}
//	assert( 0 );
}

/*****************************************************
**
**   ApplicationWindow   ---   OnIdle
**
******************************************************/
void ApplicationWindow::OnIdle( wxIdleEvent &event )
{
	if ( ! updateTools ) return;

	bool isDoc, animated;
	isDoc = animated = false;

	docommands = false;
	ChildWindow *child = listctrl->getActiveChild();
	if ( child )
	{
		if ( child->getDoc() ) isDoc = true;
		if ( isDoc && child->getDoc()->isAnimated() ) animated = true;
	}
	menubar->updateMenus( isDoc, animated );
	maintoolbar->updateTools( isDoc );
	docommands = true;
	updateTools = false;
}

/*****************************************************
**
**   ApplicationWindow   ---   OnChildClose
**
******************************************************/
void ApplicationWindow::OnChildClose( ChildWindow *child )
{
	listctrl->deleteEntry( child );
	Document *doc = child->getDoc();

	//printf( "ApplicationWindow::OnChildClose\n" );

	if ( doc && child == doc->getMainWindow() ) docmanager->deleteDoc( doc );

	if ( listctrl->getNumberOfEntries() > 0 ) listctrl->selectItem( 0 );
	updateTools = true;
}

/*****************************************************
**
**   ApplicationWindow   ---   OnFileNew
**
******************************************************/
void ApplicationWindow::OnFileNew( wxCommandEvent &event )
{
	int viewId = -1;

	bool useMultipleView = config->useMultipleViews;
	switch ( event.GetId() )
	{
	case APP_FILENEW:
		break;
	case APP_FILE_NEW_SIMPLE:
		useMultipleView = false;
		break;
	case APP_FILE_NEW_MULTIPLE_DEFAULT:
		useMultipleView = true;
		viewId = -1;
		break;
	default:
		useMultipleView = true;
		viewId = event.GetId() - APP_FILE_NEW_MULTIPLE;
		break;
	}
	NewFile( useMultipleView, viewId );
}

/*****************************************************
**
**   ApplicationWindow   ---   NewFile
**
******************************************************/
void ApplicationWindow::NewFile( const bool &useMultipleView, const int viewId )
{
	//printf( "ApplicationWindow::NewFile\n" );
	ChildWindowFactory factory;
	Document *doc = new Document();
	doc->update();
	docmanager->addDoc( doc );
	ChildWindow *child = factory.createMainWindow( this, doc, useMultipleView, viewId );
	AddChild( child );
	updateTools = true;
}

/*****************************************************
**
**   ApplicationWindow   ---   OnFileOpen
**
******************************************************/
void ApplicationWindow::OnFileOpen( wxCommandEvent &event )
{
	wxString filetypes = wxT( "Maitreya (*.mtx)|*.mtx| Maitreya 2.x (*.atx)|*.atx|Jagannatha Hora (*.jhd)|*.jhd| All files (*.*)|*.*" );
	wxFileDialog openFileDialog( this, _( "Open file"), config->defOpenPath, wxT( "" ),
	                             filetypes, wxFD_OPEN, wxDefaultPosition );

	openFileDialog.SetFilterIndex( config->defOpenFiletype );
	if (openFileDialog.ShowModal() == wxID_OK )
	{
		// Bugfix 7.0.3 GetDirectory returns home directory on some Linux distros instead of selected dir
		//if ( OpenFile( openFileDialog.GetDirectory() + wxFileName::GetPathSeparator()  + openFileDialog.GetFilename() ))
		if ( OpenFile( openFileDialog.GetPath() ))
		{
			config->defOpenPath = openFileDialog.GetDirectory();
			config->defOpenFiletype = openFileDialog.GetFilterIndex();
		}
	}
	updateTools = true;
}

/*****************************************************
**
**   ApplicationWindow   ---   OpenFile
**
******************************************************/
bool ApplicationWindow::OpenFile( wxString filename )
{
	wxLogMessage( wxT( "ApplicationWindow::OpenFile %s" ), (const wxChar*)filename );

	Document *doc = new Document();
	if ( ! doc->openFile( filename ))
	{
		delete doc;
		doMessageBox( this,
		              wxString::Format( wxT( "%s %s" ), _( "Cannot open file" ), (const wxChar*)filename ),
		              wxOK | wxICON_ERROR );
		return false;
	}
	menubar->addToRecentFiles( filename );
	CreateNewChild( doc );
	updateTools = true;
	return true;
}

/*****************************************************
**
**   ApplicationWindow   ---   CreateNewChild
**
******************************************************/
void ApplicationWindow::CreateNewChild( Document *doc )
{
	doc->update();
	docmanager->addDoc( doc );
	ChildWindow *child = ChildWindowFactory().createMainWindow( this, doc, config->useMultipleViews, -1 );
	AddChild( child );
	updateTools = true;
}

/*****************************************************
**
**   ApplicationWindow   ---   OnFileQuit
**
******************************************************/
void ApplicationWindow::OnFileQuit( wxCommandEvent &event )
{
	Close( true );
}

/*****************************************************
**
**   ApplicationWindow   ---   AddChild
**
******************************************************/
void ApplicationWindow::AddChild( ChildWindow *child )
{
	if ( child == 0 )
	{
		printf( "Error: child is null!\n" );
		return;
	}
	listctrl->addEntry( child );
	SetActiveChild( child );
	child->Show( true );
	child->postCreate();
	updateTools = true;
}

/*****************************************************
**
**   ApplicationWindow   ---   OnNewChild
**
******************************************************/
void ApplicationWindow::OnNewChild( wxCommandEvent &event )
{
	ChildWindow *child = 0;
	//printf( "ApplicationWindow::OnNewChild %d\n", event.GetId() );
	ChildWindowFactory factory;

	switch ( event.GetId() )
	{
	case APP_NEW_PARTNER:
		child = factory.createPartnerWindow( this, docmanager );
		break;
	default:
		child = factory.createChild( this, 0, event.GetId(), true );
		break;
	}
	AddChild( child );
	updateTools = true;
}

/*****************************************************
**
**   ApplicationWindow   ---   OnChildCommand
**
******************************************************/
void ApplicationWindow::OnChildCommand( wxCommandEvent &event )
{
	updateTools = true;
	int id = event.GetId();
	//printf( "ApplicationWindow::OnChildCommand %d object %ld\n", id, (long)event.GetEventObject() );
	if ( ! docommands ) return;
	ChildWindow *child = listctrl->getActiveChild();

	if ( ! child )
	{
		wxLogMessage( wxT( "ApplicationWindow::OnChildCommand No active child" ));
		return;
	}
	if ( id < CMD_CHILD_NEW_TEXT || id >= CMD_CHILD_GRAPHIC_STYLE )
	{
		child->OnCommand( event );
		if ( id == CMD_CHILD_EDITDATA ) listctrl->updateItemLabels();
	}
	else
	{
		// avoid that document dependent view is opened based upon non doc view, ugly but works
		if ( child->getDoc() == 0 &&
		        ( id == CMD_CHILD_NEW_TEXT || id == CMD_CHILD_NEW_TRANSIT || id == CMD_CHILD_NEW_SBC || id == CMD_CHILD_NEW_SOLAR
		          || id == CMD_CHILD_NEW_YOGA || id == CMD_CHILD_NEW_GRAPHICALDASA || id == CMD_CHILD_NEW_DASA
		          || id == CMD_CHILD_NEW_URANIAN || id == CMD_CHILD_NEW_WCHART || id == CMD_CHILD_NEW_MAINVIEW
		          || id >= CMD_CHILD_NEW_RASI )) return;

		// may be done now
		AddChild( ChildWindowFactory().createChild( this, child->getDoc(), id, true ));
	}
}

/*****************************************************
**
**   ApplicationWindow   ---   saveConfig
**
******************************************************/
void ApplicationWindow::saveConfig()
{
	wxLogMessage( wxT( "ApplicationWindow::saveConfig" ));

	wxRect rect = GetRect();
	config->xPosMasterWindow = rect.x;
	config->yPosMasterWindow = rect.y;
	config->xMasterWindow = rect.width;
	config->yMasterWindow = rect.height;
	menubar->saveRecentFiles();
	config->writeAll();
	updateTools = true;
}

/*****************************************************
**
**   ApplicationWindow   ---   OnClose
**
******************************************************/
void ApplicationWindow::OnClose( wxCloseEvent &event )
{
	wxLogMessage( wxT( "Start ApplicationWindow::OnClose" ));
	long i;
	Document *doc;

	updateTools = true;
	if ( config->askOnQuit )
	{
		if ( doMessageBox( this, _( "Do you want to quit?" ), wxYES_NO | wxCENTRE | wxICON_QUESTION ) == wxID_NO ) return;
	}
	wxLogMessage( wxT( "ApplicationWindow::OnClose -- Closing" ));

	for ( i = 0; i < docmanager->getNbDocuments(); i++ )
	{
		doc = docmanager->getDocument( i );
		if ( ! doc->queryClose() ) return;
	}
	for ( i = 0; i < listctrl->getNumberOfEntries(); i++ )
	{
		if ( ! listctrl->getChild( i )->Close()) return;
	}
	Destroy();
	saveConfig();
}

/*****************************************************
**
**   ApplicationWindow   ---   OnSaveConfig
**
******************************************************/
void ApplicationWindow::OnSaveConfig( wxCommandEvent &event )
{
	saveConfig();
	doMessageBox( this, _( "Configuration saved." ));
	updateTools = true;
}

/*****************************************************
**
**   ApplicationWindow   ---   OnToggleIcons
**
******************************************************/
void ApplicationWindow::OnToggleIcons( wxCommandEvent &event )
{
	config->listStyle = event.GetId() - APP_LIST_BIGICONS;
	listctrl->recreate();
	updateTools = true;
}

/*****************************************************
**
**   ApplicationWindow   ---   OnConfigure
**
******************************************************/
void ApplicationWindow::OnConfigure( wxCommandEvent &event )
{
	wxLogMessage( wxT( "ApplicationWindow::OnConfigure" ));
	ConfigDialog( this ).ShowModal();
	updateTools = true;
}

/*****************************************************
**
**   ApplicationWindow   ---   OnLocationDialog
**
******************************************************/
void ApplicationWindow::OnLocationDialog( wxCommandEvent &event )
{
	AtlasDialogWrapper wrapper( this, ATLASDIALOG_CALLER_APPLICATIONWINDOW );
	wrapper.run();
	updateTools = true;
}

/*****************************************************
**
**   ApplicationWindow   ---   OnAppHelp
**
******************************************************/
void ApplicationWindow::OnAppHelp( wxCommandEvent &event )
{
	if ( ! wxLaunchDefaultBrowser( SARAVALI_HELP_URL )) doMessageBox( this, _( "Cannot launch Browser." ) );
	updateTools = true;
}

/*****************************************************
**
**   ApplicationWindow   ---   OnAppAbout
**
******************************************************/
void ApplicationWindow::OnAppAbout( wxCommandEvent &event )
{
	AboutDialog( this ).ShowModal();
	updateTools = true;
}

/**************************************************************
***
**  ApplicationWindow   ---   OnCreateStatusBar
***
***************************************************************/
wxStatusBar *ApplicationWindow::OnCreateStatusBar(int number, long style, wxWindowID id, const wxString& name)
{
	return new MainWindowStatusbar( this );
	updateTools = true;
}

/*****************************************************
**
**   ApplicationWindow   ---   showStatusBar
**
******************************************************/
void ApplicationWindow::showStatusBar()
{
	wxCoord w, h;
	statusbar->StartPlanetTimer( config->showStatusBar && config->showStatusInfo );
	statusbar->Show( config->showStatusBar );
	// Workaround
	GetSize( &w, &h );
	SetSize( w+1, h );
	SetSize( w, h );
	updateTools = true;
}

/*****************************************************
**
**   ApplicationWindow   ---   OnToggleStatusBar
**
******************************************************/
void ApplicationWindow::OnToggleStatusBar( wxCommandEvent &event )
{
	config->showStatusBar = event.IsChecked();
	showStatusBar();
	updateTools = true;
}

/*****************************************************
**
**   ApplicationWindow   ---   OnToggleToolBars
**
******************************************************/
void ApplicationWindow::OnToggleToolBars( wxCommandEvent &event )
{
	switch ( event.GetId() )
	{
	case APP_SHOWMAINTOOLBAR:
		config->showMainToolbar = event.IsChecked();
		break;
	default:
		assert( 0 );
		break;
	}
	showToolBars();
	updateTools = true;
}

/*****************************************************
**
**   ApplicationWindow   ---   showToolBars
**
******************************************************/
void ApplicationWindow::showToolBars()
{
	wxCoord w, h;
	maintoolbar->Show( config->showMainToolbar );
	// Workaround
	GetSize( &w, &h );
	SetSize( w+1, h );
	SetSize( w, h );
	updateTools = true;
}

/*****************************************************
**
**   ApplicationWindow   ---   OnDropFilesEvent
**
******************************************************/
void ApplicationWindow::OnDropFilesEvent( wxDropFilesEvent &event )
{
	wxLogMessage( wxT( "ApplicationWindow::OnDropFilesEvent" ));
	for ( int i = 0; i < event.GetNumberOfFiles(); i++ )
	{
		wxLogMessage( wxT( "ApplicationWindow::OnDropFilesEvent open %s"), (const wxChar*)((event.GetFiles())[i]) );
		OpenFile( (const wxChar*)((event.GetFiles())[i]) );
	}
}

/*****************************************************
**
**   ApplicationWindow   ---   OnToolbarConfigChanged
**
******************************************************/
void ApplicationWindow::OnToolbarConfigChanged( wxCommandEvent &event )
{
	delete maintoolbar;
	SetToolBar( 0 );
	maintoolbar = ToolbarFactory::get()->createMainToolbar( this );
	maintoolbar->Realize();
	SetToolBar( maintoolbar );
	showToolBars();
	updateTools = true;
}

/*****************************************************
**
**   ApplicationWindow   ---   OnConfigChanged
**
******************************************************/
void ApplicationWindow::OnConfigChanged( wxCommandEvent &event )
{
	Document *doc;
	int i;
	Calculator *calculator = CalculatorFactory().getCalculator();

	calculator->init();
	wxString oldcfg = config->mainToolbarItems;
	wxString tb = config->mainToolbarItems;
	for ( i = 0; i < docmanager->getNbDocuments(); i++ )
	{
		doc = docmanager->getDocument( i );
		if ( doc )
		{
			doc->update();
			doc->updateAllChildWindows();
		}
	}
	listctrl->onConfigChanged();
	if ( config->showStatusBar )
		((MainWindowStatusbar*)GetStatusBar())->StartPlanetTimer( config->showStatusInfo );
	updateTools = true;
}

/*****************************************************
**
**   ApplicationWindow   ---   OnChildFocusEvent
**
******************************************************/
void ApplicationWindow::OnChildFocusEvent( wxCommandEvent &event )
{
	ChildWindow *child = (ChildWindow*)event.GetEventObject();
	SetActiveChild( child );
	updateTools = true;
}

/*****************************************************
**
**   ApplicationWindow   ---   OnChildCloseEvent
**
******************************************************/
void ApplicationWindow::OnChildCloseEvent( wxCommandEvent &event )
{
	ChildWindow *child = (ChildWindow*)event.GetEventObject();
	OnChildClose( child );
	updateTools = true;
}

/*****************************************************
**
**   ApplicationWindow   ---   OnNewChildEvent
**
******************************************************/
void ApplicationWindow::OnNewChildEvent( wxCommandEvent &event )
{
	assert( 0 );
	ChildWindow *child = (ChildWindow*)event.GetEventObject();
	AddChild( child );
	echo( wxT( "New child created" ));
	updateTools = true;
}

/*****************************************************
**
**   ApplicationWindow   ---   OnListItemSelected
**
******************************************************/
void ApplicationWindow::OnListItemSelected( wxListEvent& event )
{
	listctrl->setActiveItem( event.GetIndex() );
	updateTools = true;
}

/*****************************************************
**
**   ApplicationWindow   ---   OnDocSavedEvent
**
******************************************************/
void ApplicationWindow::OnDocSavedEvent( wxCommandEvent &event )
{
	Document *doc = (Document*)event.GetEventObject();
	menubar->addToRecentFiles( ((FileDataSet*)doc->getDataSet())->getFilename() );
	updateTools = true;
}

/*****************************************************
**
**   ApplicationWindow   ---   OnDocUpdateEvent
**
******************************************************/
void ApplicationWindow::OnDocUpdateEvent( wxCommandEvent &event )
{
	listctrl->arrangeIcons();
	updateTools = true;
}

/*****************************************************
**
**   ApplicationWindow   ---   OnCreateEntryChart
**
******************************************************/
void ApplicationWindow::OnCreateEntryChart( wxCommandEvent &event )
{
	Document *d = (Document*)event.GetEventObject();
	CreateNewChild( d );
	updateTools = true;
}

/*****************************************************
**
**   ApplicationWindow   ---   OnEventTest
**
******************************************************/
void ApplicationWindow::OnEventTest( wxCommandEvent &event )
{
	//wxString *s = (wxString*)event.GetEventObject();
}

/*****************************************************
**
**   ApplicationWindow   ---   echo
**
******************************************************/
void ApplicationWindow::echo( const wxChar *message )
{
	//((MainWindowStatusbar*)GetStatusBar())->echo( message );
	updateTools = true;
}

