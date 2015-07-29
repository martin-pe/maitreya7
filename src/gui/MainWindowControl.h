/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/MainWindowControl.h
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
#ifndef MAINWINDOWCONTROL_H_
#define MAINWINDOWCONTROL_H_

#ifdef __GNUG__
#pragma interface "MainWindowControl.h"
#endif

#include <wx/window.h>
#include <vector>

#include "guibase.h"

class ChildWindow;
class Document;
class wxImageList;
class wxListCtrl;
class wxListEvent;
class wxMouseEvent;
class wxSizeEvent;

#define LISTCTRL_ID 12000

using namespace std;

/*************************************************//**
*
* \brief list control for main window supporting icons and several view types
*
******************************************************/
class MainWindowControl : public wxWindow
{
	DECLARE_CLASS( MainWindowControl )
public:
	MainWindowControl( wxWindow* parent, wxWindowID id );
	~MainWindowControl();
	void recreate();
	void addEntry( ChildWindow* );
	void deleteEntry( ChildWindow* );
	int getNumberOfEntries();
	ChildWindow *getChild( const int& );
	void selectItem( const int & );
	void setActiveItem( const int & );
	void OnRightDown( wxMouseEvent &event );
	void updateItemLabels();
	ChildWindow *getActiveChild();
	void arrangeIcons();
	void onConfigChanged();

private:
	int activeitem;
	void insertListItem( ChildWindow *item );
	void updateItemLabel( ChildWindow *item, const int &pos );
	void OnSize( wxSizeEvent& );
	wxListCtrl *listctrl;
	vector<ChildWindow*> windowlist;
	wxImageList *smallIconList, *bigIconList;
	int getIconIndex( ChildWindow *item );
	void OnListItemActivated( wxListEvent& );
	void OnListItemSelected( wxListEvent& );

};


#endif

