/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/ChildWindow.cpp
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
#pragma implementation "ChildWindow.h"
#endif

#include "ChildWindow.h"
#include "BasicView.h"
#include "SplitterWidget.h"

#include <wx/string.h>

#include <wx/app.h>
#include <wx/imaglist.h>
#include <wx/notebook.h>
#include <wx/log.h>
#include <wx/menu.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/splitter.h>

#include "Commands.h"
#include "Conf.h"
#include "Document.h"
#include "guibase.h"
#include "IconProvider.h"
#include "MenuProvider.h"
#include "ViewNode.h"

extern Config *config;

IMPLEMENT_CLASS( ChildWindow, wxFrame )
IMPLEMENT_CLASS( SimpleChildWindow, ChildWindow )

DEFINE_EVENT_TYPE( CHILD_ADDTORECENT )
DEFINE_EVENT_TYPE( CHILD_SETACTIVE )
DEFINE_EVENT_TYPE( CHILD_CLOSED )
DEFINE_EVENT_TYPE( CHILD_ADDED )
DEFINE_EVENT_TYPE( SHOW_EPHEM_FILE_WARNING )

/*****************************************************
**
**   ChildWindow   ---   Constructor
**
******************************************************/
ChildWindow::ChildWindow( wxFrame *parent, Document *document, const wxSize &size, const bool ismain )
		: wxFrame( parent, -1, wxT( "childwindow" ), wxDefaultPosition, size ), doc( document )
{
	isvalid = true;
	ismainwindow = false;
	showEphemFileWarning = false;
	setMainWindow( ismain );
	if ( doc && ! ismain ) doc->addChildWindow( this );
	SetMinSize( wxSize( 200, 200 ));

	Connect( wxEVT_ACTIVATE, wxActivateEventHandler( ChildWindow::OnActivate ));
	Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( ChildWindow::OnMouseWheelEvent ));
	Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( ChildWindow::OnClose ));
	Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( ChildWindow::OnKeyDown ));
	Connect( wxEVT_CHAR, wxKeyEventHandler( ChildWindow::OnChar ));
	Connect( wxID_ANY, SHOW_EPHEM_FILE_WARNING, wxCommandEventHandler( ChildWindow::OnEphemFileWarning ));
	Connect( CMD_CHILD_FIRST+1, CMD_CHILD_GRAPHIC_STYLE+100, wxCommandEventHandler( ChildWindow::OnCommand ));
}

/*****************************************************
**
**   ChildWindow   ---   Destructor
**
******************************************************/
ChildWindow::~ChildWindow()
{
	if ( ismainwindow ) delete doc;
}

/*****************************************************
**
**   ChildWindow   ---   OnKeyDown
**
******************************************************/
void ChildWindow::OnKeyDown( wxKeyEvent &event )
{
	int i = CommandHandler::get()->lookupCommand4KeyEvent( event );
	if ( i )
	{
		wxMenuEvent evt( wxEVT_COMMAND_MENU_SELECTED, i );
		wxPostEvent( GetParent(), evt );
	}
	else event.Skip();
}

/*****************************************************
**
**   ChildWindow   ---   OnEphemFileWarning
**
******************************************************/
void ChildWindow::OnEphemFileWarning( wxCommandEvent &event )
{
	// show only once a session
	static bool shown = false;

	if ( ! shown )
	{
		if ( ! config->doNotShowAgainWarningEphemFiles )
		{
			doNotShowAgainMessage( this, DO_NOT_SHOW_AGAIN_WARNING_EPHEM_FILES );
		}
		shown = true;
	}
}

/*****************************************************
**
**   ChildWindow   ---   OnChar
**
******************************************************/
void ChildWindow::OnChar( wxKeyEvent &event )
{
	int i = CommandHandler::get()->lookupCommand4CharEvent( event );
	if ( i )
	{
		wxMenuEvent evt( wxEVT_COMMAND_MENU_SELECTED, i );
		wxPostEvent( GetParent(), evt );
	}
	else event.Skip();
}

/*****************************************************
**
**   ChildWindow   ---   OnMouseWheelEvent
**
******************************************************/
void ChildWindow::OnMouseWheelEvent( wxMouseEvent &event )
{
	handleMouseWheelEvent( event );
}

/*****************************************************
**
**   ChildWindow   ---   OnDataChanged
**
******************************************************/
void ChildWindow::OnDataChanged()
{
	setTitle();
}

/*****************************************************
**
**   ChildWindow   ---   OnClose
**
******************************************************/
void ChildWindow::OnClose( wxCloseEvent &event )
{
	wxLogMessage( wxT( "ChildWindow::OnClose classname %s" ), GetClassInfo()->GetClassName());

	if ( ismainwindow )
	{
		if ( ! doc->queryClose())
		{
			event.Veto();
			return;
		}
		doc->closeDependentChildWindows();
	}
	wxCommandEvent e( CHILD_CLOSED, GetId() );
	e.SetEventObject( this );

	// Issue 3.0
	//GetParent()->ProcessWindowEvent( e );
	//GetParent()->ProcessEvent( e );
	wxPostEvent( GetParent(), e );

	if ( doc ) doc->releaseChildWindow( this );
	Destroy();
	isvalid = false;
}

/*****************************************************
**
**   ChildWindow   ---   showContextMenu
**
******************************************************/
void ChildWindow::showContextMenu( wxMenu *menu, const int &x, const int &y )
{
	PopupMenu( menu, x, y);
	delete menu;
}

/*****************************************************
**
**   ChildWindow   ---   OnActivate
**
******************************************************/
void ChildWindow::OnActivate( wxActivateEvent &event )
{
	setTitle();
	wxCommandEvent e( CHILD_SETACTIVE, GetId() );
	e.SetEventObject( this );
	wxPostEvent( GetParent(), e );
}

/*****************************************************
**
**   ChildWindow   ---   dispatchCommand
**
******************************************************/
bool ChildWindow::dispatchCommand( const int& command, BasicView *view )
{
	bool ret = false;
	if ( view ) ret = view->dispatchCommand( command );

	if ( ret )
	{
		view->OnDataChanged();
		view->Refresh();
		return ret;
	}

	switch ( command )
	{
	case CMD_CHILD_CLOSE:
		Close();
		break;
	case CMD_CHILD_CLOSE_DOC:
		doc->getMainWindow()->Close();
		break;
	case CMD_CHILD_SAVE:
		if ( doc ) doc->save();
		break;
	case CMD_CHILD_SAVEAS:
		if ( doc ) doc->saveAs();
		break;
	case CMD_CHILD_QUICKPRINT:
		if ( doc ) doc->doQuickPrint();
		break;
	case CMD_CHILD_PRINT:
		if ( doc ) doc->doPrint();
		break;
	case CMD_CHILD_EDITDATA:
		if ( doc ) doc->editData();
		break;
	case CMD_CHILD_ANIMATE:
		if ( doc ) doc->StartTimer( ! doc->isAnimated());
		break;
	default:
		return false;
		break;
	}
	return true;
}

/*****************************************************
**
**   ChildWindow   ---   OnCommand
**
******************************************************/
void ChildWindow::OnCommand( wxCommandEvent &event )
{
	if ( ! isvalid ) return; // nothing to do for closing views

	BasicView *bview = 0;
	wxObject *oo = event.GetEventObject();

	if ( oo )
	{
		if ( oo->GetClassInfo()->IsKindOf( CLASSINFO( MyMenu )))
		{
			MyMenu *menu = (MyMenu*)oo;
			bview = (BasicView*)menu->getView();
		}
	}

	if ( event.GetId() >= CMD_CHILD_NEW_TEXT && event.GetId() <= CMD_CHILD_NEW_RASI+30 )
	{
		event.Skip(); // Delegate to parent
	}
	else dispatchCommand( event.GetId(), bview );
}

/*****************************************************
**
**   ChildWindow   ---   setMainWindow
**
******************************************************/
void ChildWindow::setMainWindow( const bool &b )
{
	ismainwindow = b;
	if ( b ) doc->setMainWindow( this );
}

/*****************************************************
**
**   SimpleChildWindow   ---   Constructor
**
******************************************************/
SimpleChildWindow::SimpleChildWindow( wxFrame *parent, Document *doc, const wxSize &size, const bool ismain )
		: ChildWindow( parent, doc, size, ismain )
{
	view = 0;
	Connect( wxEVT_ACTIVATE, wxActivateEventHandler( SimpleChildWindow::OnActivate ));
	Connect( wxEVT_SIZE, wxSizeEventHandler( SimpleChildWindow::OnSize ));
	Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( SimpleChildWindow::OnClose ));
}

/*****************************************************
**
**   SimpleChildWindow   ---   Destructor
**
******************************************************/
SimpleChildWindow::~SimpleChildWindow()
{
}

/*****************************************************
**
**   SimpleChildWindow   ---   insertView
**
******************************************************/
void SimpleChildWindow::insertView( BasicView *v )
{
	view = v;
	setTitle();
}

/*****************************************************
**
**   SimpleChildWindow   ---   postCreate
**
******************************************************/
void SimpleChildWindow::postCreate()
{
	assert( view );
	view->postCreate();
	//view->Layout();
}

/*****************************************************
**
**   SimpleChildWindow   ---   getViewId
**
******************************************************/
int SimpleChildWindow::getViewId()
{
	assert( view );
	return view->getViewId();
}

/*****************************************************
**
**   SimpleChildWindow   ---   OnClose
**
******************************************************/
void SimpleChildWindow::OnClose( wxCloseEvent &event )
{
	assert( view );
	if ( ! view->queryClose() )
	{
		event.Veto();
	}
	else ChildWindow::OnClose( event );
}

/*****************************************************
**
**   SimpleChildWindow   ---   dispatchCommand
**
******************************************************/
bool SimpleChildWindow::dispatchCommand( const int& command, BasicView *v )
{
	return ChildWindow::dispatchCommand( command, view );
}

/*****************************************************
**
**   SimpleChildWindow   ---   handleMouseWheelEvent
**
******************************************************/
void SimpleChildWindow::handleMouseWheelEvent( wxMouseEvent &event )
{
	view->handleMouseWheelEvent( event );
}

/*****************************************************
**
**   SimpleChildWindow   ---   OnDataChanged
**
******************************************************/
void SimpleChildWindow::OnDataChanged()
{
	view->OnDataChanged();
	ChildWindow::OnDataChanged();
}

/*****************************************************
**
**   SimpleChildWindow   ---   OnActivate
**
******************************************************/
void SimpleChildWindow::OnActivate( wxActivateEvent &event )
{
	if ( view )	view->SetFocus();
	ChildWindow::OnActivate( event );
}

/*****************************************************
**
**   SimpleChildWindow   ---   OnSize
**
******************************************************/
void SimpleChildWindow::OnSize( wxSizeEvent &event )
{
	wxSize size = event.GetSize();
	if ( view ) setConfigSizeForViewId( view->getViewId(), size, view->isVedic());
	event.Skip();
}

/**************************************************************
***
**   SimpleChildWindow   ---   getContextMenu
***
***************************************************************/
wxMenu *SimpleChildWindow::getContextMenu()
{
	assert( view );
	return ContextMenuProvider().getContextMenu( this, view );
}

/*****************************************************
**
**   SimpleChildWindow   ---   getPreferVedic
**
******************************************************/
bool SimpleChildWindow::getPreferVedic()
{
	if ( ! view ) return true;
	else return view->isVedic();
}

/*****************************************************
**
**   SimpleChildWindow   ---   supportsEWToggle
**
******************************************************/
bool SimpleChildWindow::supportsEWToggle() const
{
	if ( ! view ) return false;
	else return view->supportsEWToggle();
}

/*****************************************************
**
**   SimpleChildWindow   ---   setTitle
**
******************************************************/
void SimpleChildWindow::setTitle()
{
	if ( view ) SetTitle( view->getTitle() );
	else SetTitle( wxT( "empty" ));
}

/**************************************************************
***
**   SimpleChildWindow   ---   getWindowLabel
***
***************************************************************/
wxString SimpleChildWindow::getWindowLabel( const bool shortname )
{
	if ( ! view ) return wxT( "empty" );
	else return view->getWindowLabel( shortname );
}

/**************************************************************
***
**   ChildWindowFactory   ---   createPartnerWindow
***
***************************************************************/
ChildWindow *ChildWindowFactory::createPartnerWindow( wxFrame *parent, DocumentManager *manager )
{
	int viewid = VIEW_PARTNER;
	wxSize size = getSizeForViewId( viewid );
	SimpleChildWindow *child = new SimpleChildWindow( parent, 0, size , false );
	BasicView *mview = ViewFactory().createPartnerView( child, child, manager, config->preferVedic );
	mview->doLayout();
	child->insertView( mview );
	child->SetIcon( IconProvider::getIcon( BITMAP_PARTNER ));
	return child;
}

/**************************************************************
***
**   ChildWindowFactory   ---   createSingleMainWindow
***
***************************************************************/
ChildWindow *ChildWindowFactory::createSingleMainWindow( wxFrame *parent, Document *doc )
{
	WidgetNode node( CMD_CHILD_NEW_MAINVIEW );
	wxSize size = getSizeForViewId( node.viewtype );
	SimpleChildWindow *child = new SimpleChildWindow( parent, doc, size, true );

	BasicView *view = ViewFactory().createView( child, child, doc, &node );
	assert( view );
	child->insertView( view );
	view->doLayout();
	doc->setMainWindow( child );
	child->SetIcon( IconProvider::getWindowIcon( node.viewtype ));
	return child;
}

/**************************************************************
***
**   ChildWindowFactory   ---   createChild
***
***************************************************************/
ChildWindow *ChildWindowFactory::createChild( wxFrame *parent, Document *doc, const int id, const bool &vedic )
{
	WidgetNode node( id );
	wxSize size = getSizeForViewId( node.viewtype, node.vedic );
	SimpleChildWindow *child = new SimpleChildWindow( parent, doc, size, false );
	assert( child );

	BasicView *view = ViewFactory().createView( child, child, doc, &node );
	child->insertView( view );
	view->doLayout();

	child->SetIcon( IconProvider::getWindowIcon( node.viewtype ));
	return child;
}


