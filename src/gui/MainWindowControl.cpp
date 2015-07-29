/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/MainWindowControl.cpp
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
#pragma implementation "MainWindowControl.h"
#endif

#include "MainWindowControl.h"

#include "BasicView.h"
#include "ChildWindow.h"
#include "ColorConfig.h"
#include "Conf.h"
#include "DataSet.h"
#include "Document.h"
#include "FontConfig.h"
#include "IconProvider.h"
#include "Lang.h"
#include "MenuProvider.h"

#include <wx/imaglist.h>
#include <wx/listctrl.h>
#include <wx/log.h>
#include <wx/menu.h>
#include <wx/string.h>

extern Config *config;

IMPLEMENT_CLASS( MainWindowControl, wxWindow )

/*****************************************************
**
**   CLASS MainWindowControlEvtHandler
**
******************************************************/
class MainWindowControlEvtHandler : public wxEvtHandler
{
	DECLARE_CLASS( MainWindowControlEvtHandler )
public:
	MainWindowControlEvtHandler( MainWindowControl *listctrl ) {
		this->listctrl = listctrl;
	}

private:
	virtual void OnContextMenuEvent( wxMouseEvent& event) {
		listctrl->OnRightDown( event );
	}
	MainWindowControl *listctrl;
	DECLARE_EVENT_TABLE()
};

IMPLEMENT_CLASS( MainWindowControlEvtHandler, wxEvtHandler )
BEGIN_EVENT_TABLE( MainWindowControlEvtHandler, wxEvtHandler)
	EVT_RIGHT_DOWN( MainWindowControlEvtHandler::OnContextMenuEvent )
END_EVENT_TABLE()

/*****************************************************
**
**   MainWindowControl   ---   Constructor
**
******************************************************/
MainWindowControl::MainWindowControl( wxWindow* parent, wxWindowID id )
		: wxWindow( parent, id )
{
	listctrl = 0;
	activeitem = -1;

	smallIconList = IconProvider::createAppWindowImageList( true );
	bigIconList = IconProvider::createAppWindowImageList( false );


	Connect( wxEVT_SIZE, wxSizeEventHandler( MainWindowControl::OnSize ));
	Connect( LISTCTRL_ID, wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler( MainWindowControl::OnListItemActivated ));

	recreate();
}

/*****************************************************
**
**   MainWindowControl   ---   Destructor
**
******************************************************/
MainWindowControl::~MainWindowControl()
{
	listctrl->PopEventHandler( true );
	delete listctrl;
	delete smallIconList;
	delete bigIconList;
}

/*****************************************************
**
**   MainWindowControl   ---   recreate
**
******************************************************/
void MainWindowControl::recreate()
{
	wxListItem itemCol;
	FontConfig *f = FontConfig::get();

	// delete old listctrl after creating the new one
	// this is a bug of 7.0.1 on GTK (GTK event loop crash on style change)
	wxListCtrl *old_listctrl = listctrl;

	if ( false && listctrl )
	{
		listctrl->PopEventHandler( true );
		delete listctrl;
	}
	int width = GetParent()->GetSize().GetWidth() / 4 - 2;
	switch ( config->listStyle )
	{
	case 1:
		listctrl = new wxListCtrl( this, LISTCTRL_ID, wxDefaultPosition, wxDefaultSize,
		                           wxLC_SMALL_ICON|wxLC_SINGLE_SEL|wxSUNKEN_BORDER );
		listctrl->SetImageList( smallIconList, wxIMAGE_LIST_SMALL );
		break;
	case 2:
		listctrl = new wxListCtrl( this, LISTCTRL_ID, wxDefaultPosition, wxDefaultSize,
		                           wxLC_REPORT|wxLC_SINGLE_SEL|wxSUNKEN_BORDER );
		listctrl->SetImageList( smallIconList, wxIMAGE_LIST_SMALL );
		itemCol.m_mask = wxLIST_MASK_TEXT | wxLIST_MASK_IMAGE;
		itemCol.m_text = _( "Type" );
		//itemCol.m_image = -1;
		listctrl->InsertColumn(0, itemCol);
		listctrl->SetColumnWidth( 0, width );

		itemCol.m_text = _( "Name" );
		listctrl->InsertColumn(1, itemCol);
		listctrl->SetColumnWidth( 1, width );

		itemCol.m_text = _( "Date" );
		listctrl->InsertColumn(2, itemCol);
		listctrl->SetColumnWidth( 2, width );

		itemCol.m_text = _( "Location" );
		listctrl->InsertColumn(3, itemCol);
		listctrl->SetColumnWidth( 3, width );
		break;
	default:
		listctrl = new wxListCtrl( this, LISTCTRL_ID, wxDefaultPosition, wxDefaultSize,
		                           wxLC_ICON|wxLC_SINGLE_SEL|wxSUNKEN_BORDER );
		listctrl->SetImageList( bigIconList, wxIMAGE_LIST_NORMAL );
		break;
	}
	for ( vector<ChildWindow*>::iterator iter = windowlist.begin(); iter != windowlist.end(); iter++ )
		insertListItem( *iter );

	listctrl->SetFont( *f->getDefaultFont() );
	listctrl->SetSize( GetSize() );
	listctrl->Arrange();
	if ( activeitem != -1 ) listctrl->SetItemState( activeitem ,wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
	listctrl->PushEventHandler( new MainWindowControlEvtHandler( this ));
	listctrl->Show( true );

	listctrl->SetBackgroundColour( config->colors->bgColor );
	listctrl->SetForegroundColour( config->colors->fgColor );

	if ( old_listctrl )
	{
		old_listctrl->PopEventHandler( true );
		delete old_listctrl;
	}
}

/*****************************************************
**
**   MainWindowControl   ---   getNumberOfEntries
**
******************************************************/
int MainWindowControl::getNumberOfEntries()
{
	return windowlist.size();
}

/***********************************************
***
**  MainWindowControl   ---   setActiveItem
***
***********************************************/
void MainWindowControl::setActiveItem( const int &i )
{
	activeitem = i;
}

/***********************************************
***
**  MainWindowControl   ---   getActiveChild
***
***********************************************/
ChildWindow *MainWindowControl::getActiveChild()
{
	if ( activeitem == -1 ) return 0;
	else return windowlist[activeitem];
}

/*****************************************************
**
**   MainWindowControl   ---   arrangeIcons
**
******************************************************/
void MainWindowControl::arrangeIcons()
{
	listctrl->SetSize( listctrl->GetSize() );
}

/*****************************************************
**
**   MainWindowControl   ---   updateItemLabel
**
******************************************************/
void MainWindowControl::updateItemLabel( ChildWindow *child, const int &pos )
{
	Formatter *formatter = Formatter::get();
	wxString name, s;

	Document *doc = child->getDoc();

	if ( config->listStyle == 2 )
	{
		listctrl->SetItem( pos, 0, child->getWindowLabel() );
		if ( doc )
		{
			Document *doc = child->getDoc();
			listctrl->SetItem( pos, 1, doc->getHName() );
			Location *location = doc->getLocation();
			listctrl->SetItem( pos, 2,
			                   formatter->getFullDateStringFromJD( doc->getJD() + ( location->getTimeZone() + location->getDST())/24 ) );
			listctrl->SetItem( pos, 3, doc->getLocation()->getLocName() );
		}
	}
	else
	{
		if ( doc )
		{
			name = doc->getHName();
			if ( child->isMainWindow() )
			{
				if ( name.Len() > 12 ) s.Printf( wxT( "%s..." ), (const wxChar*)name.Left( 12 ));
				else s = doc->getHName();
			}
			else
			{
				if ( name.Len() > 10 ) s.Printf( wxT( "%s...<%s>" ), (const wxChar*)name.Left( 10 ),
					                                 (const wxChar*)child->getWindowLabel( true ));
				else s.Printf( wxT( "%s <%s>" ), (const wxChar*)doc->getHName(), (const wxChar*)child->getWindowLabel( true ));
			}
		}
		else
		{
			s = child->getWindowLabel( true );
		}
		listctrl->SetItemText( pos, s );
	}
}

/*****************************************************
**
**   MainWindowControl   ---   insertListItem
**
******************************************************/
void MainWindowControl::insertListItem( ChildWindow *item )
{
	int i = getIconIndex( item );
	int pos = listctrl->InsertItem( listctrl->GetItemCount(), wxT( "--" ), i );
	updateItemLabel( item, pos );
	listctrl->SetItemState( pos, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
}

/*****************************************************
**
**   MainWindowControl   ---   onConfigChanged
**
******************************************************/
// doc based views are update by DocManager in ApplcationWindows
void MainWindowControl::onConfigChanged()
{
	for ( vector<ChildWindow*>::iterator iter = windowlist.begin(); iter != windowlist.end(); iter++ )
	{
		ChildWindow *child = *iter;
		if ( ! child->getDoc())
		{
			child->OnDataChanged();
		}
	}

	// colors may have changed
	listctrl->SetBackgroundColour( config->colors->bgColor );
	listctrl->SetForegroundColour( config->colors->fgColor );
	recreate();
}

/*****************************************************
**
**   MainWindowControl   ---   updateItemLabels
**
******************************************************/
void MainWindowControl::updateItemLabels()
{
	int i = 0;
	for ( vector<ChildWindow*>::iterator iter = windowlist.begin(); iter != windowlist.end(); iter++ )
	{
		updateItemLabel( *iter, i );
		i++;
	}
}

/*****************************************************
**
**   MainWindowControl   ---   getIconIndex
**
******************************************************/
int MainWindowControl::getIconIndex( ChildWindow *child )
{
	wxString cname = child->GetClassInfo()->GetClassName();
	if ( cname != wxT( "SimpleChildWindow" )) return 1;
	else if ( cname == wxT( "MultipleChildWindow" )) return 2;

	BasicView *view = ((SimpleChildWindow*)child)->getView();


	int id = view->getViewId();

	switch ( id )
	{
	case VIEW_PLANETLIST:
		return 1;
		break;
	case VIEW_DASA:
		return 3;
		break;
	case VIEW_TRANSIT:
		return 4;
		break;
	case VIEW_EPHEM:
		return 5;
		break;
	case VIEW_URANIAN:
		return 6;
		break;
	case VIEW_GRAPHIC:
		return 7;
		break;
	case VIEW_PARTNER:
		return 10;
		break;
	case VIEW_SBC:
		return 11;
		break;
	case VIEW_GRAPHICALDASA:
		return 12;
		break;
	case VIEW_ECLIPSE:
		return 13;
		break;
	case VIEW_YOGA:
		return 14;
		break;
	case VIEW_SOLAR:
		return 15;
		break;
	case VIEW_VARGA:
		return 16;
		break;
	case VIEW_GRAPHICGRID:
		return 16;
		break;
	case VIEW_ASHTAKAVARGA:
		return 18;
		break;
	case VIEW_YOGA_EDITOR:
		return 19;
		break;
	case VIEW_TEXT:
		return 20;
		break;

	case VIEW_HORA:
	default:
		return 0;
		break;
	}
	return 0;
}

/*****************************************************
**
**   MainWindowControl   ---   addEntry
**
******************************************************/
void MainWindowControl::addEntry( ChildWindow *child )
{
	windowlist.push_back( child );
	insertListItem( child );
}

/*****************************************************
**
**   MainWindowControl   ---   deleteEntry
**
******************************************************/
void MainWindowControl::deleteEntry( ChildWindow *child )
{
	int i = 0;
	for ( vector<ChildWindow*>::iterator iter = windowlist.begin(); iter != windowlist.end(); iter++ )
	{
		ChildWindow *current = *iter;
		if ( current == child )
		{
			windowlist.erase( iter );
			listctrl->DeleteItem( i );
			//recreate();
			break;
		}
		i++;
	}
	if ( windowlist.size() == 0 )
	{
		activeitem = -1;
	}
	else
	{
		if ( activeitem == i ) selectItem( i == 0 ? 0 : i - 1 );
	}
	listctrl->Arrange();
}

/*****************************************************
**
**   MainWindowControl   ---   OnSize
**
******************************************************/
void MainWindowControl::OnSize( wxSizeEvent &event )
{
	listctrl->SetSize( event.GetSize() );
	listctrl->Arrange();
}

/***********************************************
***
**  MainWindowControl   ---   getChild
***
***********************************************/
ChildWindow *MainWindowControl::getChild( const int &i )
{
	return windowlist[i];
}

/*****************************************************
**
**   MainWindowControl   ---   selectItem
**
******************************************************/
void MainWindowControl::selectItem( const int &i )
{
	//printf( "MainWindowControl::selectItem -->  Active item %d -- i %d\n", activeitem, i );
	if ( activeitem == i ) return;
	activeitem = i;
	listctrl->SetItemState( i, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED );
}

/*****************************************************
**
**   MainWindowControl   ---   OnListItemActivated
**
******************************************************/
void MainWindowControl::OnListItemActivated( wxListEvent& event )
{
	ChildWindow *child = windowlist[event.GetIndex()];
	if ( ! child )
	{
		wxLogError( wxT( "Error: can't find child  %ld in list"),  event.GetIndex() );
		return;
	}
	child->Raise();
}

/*****************************************************
**
**   MainWindowControl   ---   OnListItemSelected
**
******************************************************/
void MainWindowControl::OnListItemSelected( wxListEvent& event )
{
	ChildWindow *child = windowlist[event.GetIndex()];
	if ( ! child )
	{
		wxLogError( wxT( "Error: can't find child %ld in list" ), event.GetIndex() );
		return;
	}
}

/*****************************************************
**
**   MainWindowControl   ---   OnRightDown
**
******************************************************/
void MainWindowControl::OnRightDown( wxMouseEvent &event )
{
	int flag = wxLIST_HITTEST_ONITEM;
	wxMenu *menu = 0;

	long int i = listctrl->HitTest( event.GetPosition(), flag );

	if ( i == -1 ) {
		menu = ContextMenuProvider().getAppWindowContextMenu();
	}
	else
	{
		selectItem( i );
		ChildWindow *child = windowlist[i];
		menu  = child->getContextMenu();
	}
	PopupMenu( menu, event.GetX(), event.GetY() );
	delete menu;
}



