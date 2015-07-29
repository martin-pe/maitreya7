/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/BasicView.cpp
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
#pragma implementation "BasicView.h"
#endif

#include "BasicView.h"

#include <wx/dcclient.h>
#include <wx/event.h>
#include <wx/layout.h>
#include <wx/menu.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/textctrl.h>
#include <wx/toolbar.h>

#include "BasicWidget.h"
#include "ChildWindow.h"
#include "Commands.h"
#include "Conf.h"
#include "Document.h"
#include "MenuProvider.h"
#include "SplitterWidget.h"
#include "Statusbar.h"
#include "ToolPanel.h"
#include "ViewNode.h"

extern Config *config;

IMPLEMENT_CLASS( BasicView, wxWindow )
IMPLEMENT_CLASS( EmptyView, BasicView )

/*****************************************************
**
**   BasicView   ---   Constructor
**
******************************************************/
BasicView::BasicView( wxWindow *parent, ChildWindow *frame, Document *doc, const int &viewId, const bool &showToolbar,
                      const bool &vedic, const long style )
		: wxWindow( parent, -1 ),
		ChartProperties( vedic ),
		frame( frame ),
		doc( doc ),
		viewId( viewId ),
		showtoolbar( showToolbar )
{
	supportEWToggle = supportGraphicExport = supportTextExport
		= supportFullGraphicStyleToggle = supportRestrictedGraphicStyleToggle = supportObjectToggle
		= supportSbcStyleToggle = supportSkinToggle = supportRestrictedObjectToggle = mainView = false;

	toolbar = 0;
	widget = 0;
	detailpanel = 0;
	statusbar = 0;

	sizer = new wxBoxSizer( wxVERTICAL );
	SetSizer( sizer );

	toolbar = ( showtoolbar ? ToolbarFactory::get()->createToolbar( this, viewId ) : 0 );

	Connect( wxEVT_SIZE, wxSizeEventHandler( BasicView::OnSize ));
	Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( BasicView::OnContextMenuEvent ));
	Connect( CMD_CHILD_FIRST+1, CMD_CHILD_GRAPHIC_STYLE+100, wxCommandEventHandler( BasicView::OnCommand ));
	Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( BasicView::OnKeyDown ));
	Connect( wxEVT_CHAR, wxKeyEventHandler( BasicView::OnChar ));

	Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( BasicView::OnMouseWheelEvent ));
	Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( BasicView::OnSetFocus ));

	// toolbar events
	Connect( wxID_ANY, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( BasicView::OnChoiceCommand ));
	Connect( wxID_ANY, wxEVT_COMMAND_SPINCTRL_UPDATED, wxCommandEventHandler( BasicView::OnChoiceCommand ));
	Connect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( BasicView::OnMenuCommand ));
}

/*****************************************************
**
**   BasicView   ---   OnCommand
**
******************************************************/
void BasicView::OnCommand( wxCommandEvent &event )
{
	//printf( "BasicView::OnCommand\n" );
	if ( dispatchCommand( event.GetId())) OnDataChanged();
	else event.Skip();
}

/*****************************************************
**
**   BasicView   ---   OnMenuCommand
**
******************************************************/
void BasicView::OnMenuCommand( wxCommandEvent &event )
{
	//printf( "BasicView::OnMenuCommand\n" );
	event.SetEventObject( this );
	wxPostEvent( frame->GetParent(), event );
}

/*****************************************************
**
**   BasicView   ---   DoGetBestSize
**
******************************************************/
wxSize BasicView::DoGetBestSize() const
{
	return wxSize( 200, 200 );
}

/*****************************************************
**
**   BasicView   ---   OnChoiceCommand
**
******************************************************/
void BasicView::OnChoiceCommand( wxCommandEvent &event )
{
	OnToolbarCommand();
	widget->SetFocus();
}

/*****************************************************
**
**   BasicView   ---   OnEnterPressed
**
******************************************************/
void BasicView::OnEnterPressed( wxKeyEvent& )
{
	OnToolbarCommand();
}

/*****************************************************
**
**   BasicView   ---   OnSpinCommand
**
******************************************************/
void BasicView::OnSpinCommand( wxSpinEvent &event )
{
	OnToolbarCommand();
}

/*****************************************************
**
**   BasicView   ---   OnGeneralCommand
**
******************************************************/
void BasicView::OnGeneralCommand( wxCommandEvent &event )
{
	OnToolbarCommand();
}

/*****************************************************
**
**   BasicView   ---   OnSize
**
******************************************************/
void BasicView::OnSize( wxSizeEvent &event )
{
	Layout(); // Needed because the widget will not resize
	Refresh();
}

/*****************************************************
**
**   BasicView   ---   OnSetFocus
**
******************************************************/
void BasicView::OnSetFocus( wxFocusEvent &event )
{
	handleSetFocusEvent();
}

/*****************************************************
**
**   BasicView   ---   handleSetFocusEvent
**
******************************************************/
void BasicView::handleSetFocusEvent()
{
	if ( widget ) widget->SetFocus();
}

/*****************************************************
**
**   BasicView   ---   dispatchCommand
**
******************************************************/
bool BasicView::dispatchCommand( const int &command )
{
	return dispatchWidgetPropertyCommand( command );
}

/*****************************************************
**
**   BasicView   ---   OnDataChanged
**
******************************************************/
void BasicView::OnDataChanged()
{
	// Nesessary on Lunar Node True/Mean Changes
	//printf( "BasicView::OnDataChanged\n" );
	dispatchWidgetPropertyCommand( 0 );
	Refresh();
}

/*****************************************************
**
**   BasicView   ---   OnContextMenuEvent
**
******************************************************/
void BasicView::OnContextMenuEvent( wxMouseEvent& event)
{
	int x, y;
	x = event.m_x;
	y = event.m_y;
	wxWindow *window = (wxWindow*)event.GetEventObject();
	window->ClientToScreen( &x, &y );
	this->ScreenToClient( &x, &y );
	wxMenu *menu = getContextMenu();
	PopupMenu( menu, x, y );
	delete menu;
}

/*****************************************************
**
**   BasicView   ---   OnMouseWheelEvent
**
******************************************************/
void BasicView::OnMouseWheelEvent( wxMouseEvent &event )
{
	handleMouseWheelEvent( event );
}

/*****************************************************
**
**   BasicView   ---   OnKeyDown
**
******************************************************/
void BasicView::OnKeyDown( wxKeyEvent &event )
{
	//printf( "BasicView::OnKeyDown\n" );
	event.ResumePropagation( wxEVENT_PROPAGATE_MAX );
	event.Skip();
}

/*****************************************************
**
**   BasicView   ---   OnChar
**
******************************************************/
void BasicView::OnChar( wxKeyEvent &event )
{
	int i = CommandHandler::get()->lookupCommand4CharEvent( event );
	//printf( "BasicView::OnChar ONCHAR %d control %d i %d\n", event.GetKeyCode(), event.m_controlDown, i );
	if ( i )
	{
		wxMenuEvent evt( wxEVT_COMMAND_MENU_SELECTED, i );
		wxPostEvent( frame->GetParent(), evt );
	}

	// eigentlich obsolet
	else event.Skip();
}

/**************************************************************
***
**  BasicView   ---   getContextMenu
***
***************************************************************/
wxMenu *BasicView::getContextMenu()
{
	return ContextMenuProvider().getContextMenu( frame, this );
}

/*****************************************************
**
**   BasicView   ---   doLayout
**
******************************************************/
void BasicView::doLayout()
{
	sizer->Clear();

	// Toolbar
	if ( toolbar ) sizer->Add( toolbar, 0, wxEXPAND );

	// contents
	sizer->Add( widget, 1, wxEXPAND | wxALL, config->childViewFrameWidth );

	// detail panel
	if ( detailpanel ) sizer->Add( detailpanel, 0, wxEXPAND, 0 );

	// status bar
	if ( statusbar ) sizer->Add( statusbar, 0, wxEXPAND, 0 );

	sizer->Fit( this ); // Needed by Windows (view change in ephemeris and partner view)
	// TODO verify, problem in partner view obsolet
	frame->SendSizeEvent(); // Needed by GTK + Windows
	Layout();
}

/*****************************************************
**
**   BasicView   ---   getTitle
**
******************************************************/
wxString BasicView::getTitle()
{
	wxString label, s = wxT( "empty" );
	if ( doc )
	{
		label = getWindowLabel( true );
		if ( ! label.IsEmpty() ) s.Printf( wxT( "%s <%s>" ), (const wxChar*)doc->getHName(), (const wxChar*)getWindowLabel( false ) );
		else s = doc->getHName();
	}
	else {
		s = getWindowLabel();
	}
	return s;
}

/*****************************************************
**
**   BasicView   ---   handleMouseWheelEvent
**
******************************************************/
void BasicView::handleMouseWheelEvent( wxMouseEvent &event )
{
	if ( widget->GetClassInfo()->IsKindOf( CLASSINFO( BasicWidget )))
	{
		((BasicWidget*)widget)->HandleMouseWheelEvent( event );
	}
	else if ( widget->GetClassInfo()->IsKindOf( CLASSINFO( SplitterWidget )))
	{
		((SplitterWidget*)widget)->handleMouseWheelEvent( event );
	}
}

/*****************************************************
**
**   BasicView   ---   echo
**
******************************************************/
void BasicView::echo( wxString message )
{
	if ( statusbar ) statusbar->echo( message );
}

/*****************************************************
**
**   BasicView   ---   setStatusText
**
******************************************************/
void BasicView::setStatusText( wxString message, const int &field )
{
	if ( statusbar ) statusbar->SetStatusText( message, field );
}

/*****************************************************
**
**   EmptyView   ---   Constructor
**
******************************************************/
EmptyView::EmptyView( wxWindow *parent, ChildWindow *frame )
		: BasicView( parent, frame, 0, VIEW_EMPTY, false, false )
{
	widget = new EmptyWidget( this, this );
}

/**************************************************************
***
**   ViewFactory   ---   createEmptyView
***
***************************************************************/
BasicView *ViewFactory::createEmptyView( wxWindow *parent, ChildWindow *frame )
{
	return new EmptyView( parent, frame );
}

/**************************************************************
***
**   ViewFactory   ---   createView
***
***************************************************************/
BasicView *ViewFactory::createView( wxWindow *parent, ChildWindow *frame, Document *doc, WidgetNode *node )
{
	BasicView * view = 0;
	switch ( node->viewtype )
	{
	case VIEW_TEXT:
		view = createDefaultTextView( parent, frame, doc, node->toolpanel, node->vedic, node->textmode );
		break;
	case VIEW_YOGA:
		view = createYogaView( parent, frame, doc );
		break;
	case VIEW_GRAPHICALDASA:
		view = createGraphicalDasaView( parent, frame, doc );
		break;
	case VIEW_PLANETLIST:
		view = createMainView( parent, frame, doc, node->vedic );
		break;
	case VIEW_GRAPHIC:
		view = createGraphicView( parent, frame, doc, node->varga, node->vedic );
		break;
	case VIEW_DASA:
		view = createDasaView( parent, frame, doc );
		break;
	case VIEW_URANIAN:
		view = createUranianView( parent, frame, doc, node->vedic );
		break;
	case VIEW_TRANSIT:
		view = createTransitView( parent, frame, doc, node->vedic );
		break;
	case VIEW_VARGA:
		view = createVargaView( parent, frame, doc );
		break;
	case VIEW_ASHTAKAVARGA:
		view = createAshtakaVargaView( parent, frame, doc );
		break;
	case VIEW_VIMSOPAKABALA:
		view = createVimsopakaBalaView( parent, frame, doc );
		break;
	case VIEW_SOLAR:
		view = createSolarView( parent, frame, doc, node->vedic );
		break;
	case VIEW_SBC:
		view = createSbcView( parent, frame, doc );
		break;
	case VIEW_HORA:
		view = createHoraView( parent, frame );
		break;
	case VIEW_ECLIPSE:
		view = createEclipseView( parent, frame, node->vedic );
		break;
	case VIEW_EPHEM:
		view = createEphemView( parent, frame, node->vedic );
		break;
	case VIEW_YOGA_EDITOR:
		view = createYogaEditorView( parent, frame );
		break;
	case VIEW_GRAPHICGRID:
		view = createGraphicGridView( parent, frame, doc, node->rows, node->cols, node->vargas, node->vedic );
		break;
	default:
		printf( "ViewFactory::createView unknown id %d\n", node->viewtype );
		assert( false );
		view = createEmptyView( parent, frame );
		break;
	}
	assert( view );
	return view;
}


