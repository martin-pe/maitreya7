/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/MultipleChildWindow.cpp
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
#pragma implementation "MultipleChildWindow.h"
#endif

#include "MultipleChildWindow.h"
#include "BasicView.h"
#include "SplitterWidget.h"

#include <wx/string.h>

#include <wx/app.h>
#include <wx/notebook.h>
#include <wx/log.h>
#include <wx/menu.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/splitter.h>

#include "Conf.h"
#include "Document.h"
#include "guibase.h"
#include "IconProvider.h"
#include "MenuProvider.h"

#include "NotebookNodes.h"
#include "MultipleViewConfig.h"

extern Config *config;

IMPLEMENT_CLASS( MultipleChildWindow, ChildWindow )

/*****************************************************
**
**   MultipleChildWindow   ---   Constructor
**
******************************************************/
MultipleChildWindow::MultipleChildWindow( wxFrame *parent, Document *doc, const wxSize &size, const bool ismain )
		: ChildWindow( parent, doc, size, ismain )
{
	setTitle();
	viewconfig = 0;

	Connect( wxEVT_SIZE, wxSizeEventHandler( MultipleChildWindow::OnSize ));
	Connect( wxEVT_ACTIVATE, wxActivateEventHandler( MultipleChildWindow::OnActivate ));
}

/*****************************************************
**
**   MultipleChildWindow   ---   Destructor
**
******************************************************/
MultipleChildWindow::~MultipleChildWindow()
{
	for ( unsigned int i = 0; i < viewlist.size(); i++ ) delete viewlist[i];
	viewlist.clear();
	//if ( container ) delete container;
}

/*****************************************************
**
**   MultipleChildWindow   ---   OnDataChanged
**
******************************************************/
void MultipleChildWindow::OnDataChanged()
{
	//printf( "MultipleChildWindow::OnDataChanged\n" );
	for ( unsigned int i = 0; i < viewlist.size(); i++ ) viewlist[i]->OnDataChanged();
}

/**************************************************************
***
**  MultipleChildWindow   ---   getContextMenu
***
***************************************************************/
wxMenu *MultipleChildWindow::getContextMenu()
{
	BasicView *view = viewlist[0];
	return ContextMenuProvider().getContextMenu( this, view );
}

/*****************************************************
**
**   MultipleChildWindow   ---   getPreferVedic
**
******************************************************/
bool MultipleChildWindow::getPreferVedic()
{
	return true;
}

/*****************************************************
**
**   MultipleChildWindow   ---   supportsEWToggle
**
******************************************************/
bool MultipleChildWindow::supportsEWToggle() const
{
	return false;
}

/*****************************************************
**
**   MultipleChildWindow   ---   OnActivate
**
******************************************************/
void MultipleChildWindow::OnActivate( wxActivateEvent &event )
{
	if ( viewlist.size() > 0 ) viewlist[0]->SetFocus();
	ChildWindow::OnActivate( event );
}

/*****************************************************
**
**   MultipleChildWindow   ---   handleMouseWheelEvent
**
******************************************************/
void MultipleChildWindow::handleMouseWheelEvent( wxMouseEvent &event )
{
	//printf( "WHEEL in MultipleChildWindow::handleMouseWheelEvent %d - %d\n", event.GetX(), event.GetY() );
}

/*****************************************************
**
**   MultipleChildWindow   ---   setTitle
**
******************************************************/
void MultipleChildWindow::setTitle()
{
	if ( doc ) SetTitle( doc->getHName() );
}

/**************************************************************
***
**   MultipleChildWindow   ---   insertNode
***
***************************************************************/
wxWindow *MultipleChildWindow::insertNode( wxWindow *parent, ViewNode *node )
{
	//int id = 0;
	BasicView *view = 0;
	wxWindow *window1 = 0, *window2 = 0;
	ViewFactory f;
	wxSize size = wxDefaultSize;
	wxString pagename;

	if ( ! node ) return 0;
	if ( node && node->GetClassInfo()->IsKindOf( CLASSINFO( WidgetNode )))
	{
		WidgetNode* wn = (WidgetNode*)node;
		// TODO
		//tp = wn->toolpanel;
		view = f.createView( parent, this, getDoc(), wn );
		wn->widget = view;
		view->doLayout();
		viewlist.push_back( view );
		return view;
	}
	else if ( node && node->GetClassInfo()->IsKindOf( CLASSINFO( SplitterNode )))
	{
		SplitterNode *sn = (SplitterNode*)node;
		SplitterWidget *splitter = new SplitterWidget( parent, -1, wxDefaultPosition, size, wxSP_LIVE_UPDATE|wxSP_NOBORDER );
		sn->widget = splitter;
		splitter->SetSashGravity( 0.5 );
		if ( sn->window1 ) {
			window1 = insertNode( splitter, sn->window1 );
		}
		if ( sn->window2 ) {
			window2 = insertNode( splitter, sn->window2 );
		}

		if ( sn->dir ) {
			splitter->SplitVertically( window1, window2 );
		}
		else {
			splitter->SplitHorizontally( window1, window2 );
		}
		return splitter;
	}
	else if ( node && node->GetClassInfo()->IsKindOf( CLASSINFO( NotebookNode )))
	{
		NotebookNode *nn = (NotebookNode*)node;
		wxBookCtrlBase *notebook = NotebookNodeContainerFactory().createNotebookNodeContainer( parent, this, nn );
		return notebook;
	}
	return 0;
}

/*****************************************************
**
**   MultipleChildWindow   ---   initViews
**
******************************************************/
void MultipleChildWindow::initViews( MultipleViewConfig *cfg )
{
	viewconfig = cfg;
	assert( viewconfig->window );
	viewconfig->window->widget = insertNode( this, viewconfig->window );
}

/**************************************************************
***
**   MultipleChildWindow   ---   getWindowLabel
***
***************************************************************/
wxString MultipleChildWindow::getWindowLabel( const bool shortname )
{
	if ( doc ) return doc->getHName();
	else return wxT( "dummy" );
}

/*****************************************************
**
**   MultipleChildWindow   ---   postCreateNode
**
******************************************************/
void MultipleChildWindow::postCreateNode( ViewNode *node )
{
	assert( node->widget );
	if ( node && node->GetClassInfo()->IsKindOf( CLASSINFO( SplitterNode )))
	{
		SplitterNode *snode = (SplitterNode*)node;
		SplitterWidget *splitter = (SplitterWidget*)node->widget;
		wxSize size = splitter->GetSize();
		int sash = 100;
		if ( snode->sashpos_rel > 0 )
		{
			sash = ( snode->dir ? size.GetX() : size.GetY() ) * snode->sashpos_rel / 100;
		}
		else if ( snode->sashpos > 0 )
		{
			sash = snode->sashpos;
		}
		else
		{
			sash = ( snode->dir ? size.GetX() : size.GetY() ) / 2;    // emulate 50%
		}
		splitter->SetSashPosition( sash );
		if ( snode->window1 ) postCreateNode( snode->window1 );
		if ( snode->window2 ) postCreateNode( snode->window2 );
	}
	else if ( node && node->GetClassInfo()->IsKindOf( CLASSINFO( NotebookNode )))
	{
		NotebookNode *nnode = (NotebookNode*)node;
		wxBookCtrlBase *notebook = (wxBookCtrlBase*)nnode->widget;
		notebook->SetSelection( 0 );
		postCreateNode( nnode->pages[0] );
	}
	else if ( node && node->GetClassInfo()->IsKindOf( CLASSINFO( MultipleViewConfig )))
	{
		MultipleViewConfig *vnode = (MultipleViewConfig*)node;
		if ( vnode ) postCreateNode( vnode->window );
	}
	else if ( node && node->GetClassInfo()->IsKindOf( CLASSINFO( PageNode )))
	{
		PageNode *pnode = (PageNode*)node;
		if ( pnode ) postCreateNode( pnode->window );
	}
	else if ( node && node->GetClassInfo()->IsKindOf( CLASSINFO( WidgetNode )))
	{
		WidgetNode *wnode = (WidgetNode*)node;
		if ( wnode->widget->GetClassInfo()->IsKindOf( CLASSINFO( BasicView )))
		{
			BasicView *v = (BasicView*)wnode->widget;
			v->postCreate();
		}
	}
	else assert( false );
}

/*****************************************************
**
**   MultipleChildWindow   ---   postCreate
**
******************************************************/
void MultipleChildWindow::postCreate()
{
	if ( viewconfig->window ) postCreateNode( viewconfig->window );
	for ( unsigned int i = 0; i < viewlist.size(); i++ ) viewlist[i]->postCreate();
}

/*****************************************************
**
**   MultipleChildWindow   ---   OnSize
**
******************************************************/
void MultipleChildWindow::OnSize( wxSizeEvent &event )
{
	//printf( "MultipleChildWindow ->>OnSize %d -- %d\n", event.GetSize().x, event.GetSize().y  );
	config->xMultipleView = event.GetSize().x;
	config->yMultipleView = event.GetSize().y;
	event.Skip();
}

/**************************************************************
***
**   ChildWindowFactory   ---   createMainWindow
***
***************************************************************/
ChildWindow *ChildWindowFactory::createMainWindow( wxFrame *parent, Document *doc, const bool &useMultipleView, const int viewId )
{
	return useMultipleView ? createMultipleMainWindow( parent, doc, viewId ) : createSingleMainWindow( parent, doc );
}

/**************************************************************
***
**   ChildWindowFactory   ---   createMultipleMainWindow
***
***************************************************************/
ChildWindow *ChildWindowFactory::createMultipleMainWindow( wxFrame *parent, Document *doc, const int viewId )
{
	ChildWindow *child;
	wxString s;
	MultipleViewConfigLoader *cfgloader = MultipleViewConfigLoader::get();

	wxSize size = getSizeForViewId( VIEW_MULTIPLE );

	int id = viewId;
	if ( id < 0 || id >= cfgloader->getCount()) id = config->defaultMultipleView;

	MultipleViewConfig *viewconfig = MultipleViewConfigLoader::get()->getConfig( id );
	if ( viewconfig->window )
	{
		child = new MultipleChildWindow( parent, doc, size, true );
		((MultipleChildWindow*)child)->initViews( viewconfig );
	}
	else
	{
		doMessageBox( parent, _( "Internal error. Could not initialize view, falling back to single mode." ), wxOK | wxICON_ERROR );
		return createSingleMainWindow( parent, doc );
	}
	child->SetIcon( IconProvider::getWindowIcon( VIEW_MULTIPLE ));
	return child;
}


