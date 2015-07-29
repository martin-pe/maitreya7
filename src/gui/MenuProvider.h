/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/MenuProvider.h
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

#ifndef _MENUPROVIDER_H_
#define _MENUPROVIDER_H_

#ifdef __GNUG__
#pragma interface "MenuProvider.h"
#endif

#include <wx/menu.h>
#include <wx/string.h>

class BasicView;
class ChildWindow;

/*************************************************//**
*
* \brief specific menu class, transporting View details
*
******************************************************/
class MyMenu : public wxMenu
{
public:

	MyMenu( const wxString title, const BasicView* );
	void addItem( const int &id );
	void addCheckItem( const int &id, const bool checked = false );
	void addCheckItem( const int &id, const wxString title, const bool checked = false );
	void addArray( const int a[] );
	void addWindowListMenu();
	void addVargaMenu();

	const BasicView *getView() const {
		return view;
	}

private:
	class CommandHandler *handler;
	const class BasicView *view;

	DECLARE_CLASS( MyMenu )
};


/*************************************************//**
*
* \brief main menu bar of the appliation
*
******************************************************/
class AppMenuBar : public wxMenuBar
{
public:
	AppMenuBar( int style );
	void updateMenus( const bool &isDoc, const bool &animated );
	void addToRecentFiles( const wxString& );
	wxString getRecentFilesEntry( const int& );
	void saveRecentFiles();

private:
	MyMenu *filemenu, *docmenu, *westernmenu, *vedicmenu, *extramenu, *helpmenu, *recentmenu, *newmenu;
	wxArrayString recentfiles;
	bool newChildSimpleChildCommandIncluded;
};

/*************************************************//**
*
* \brief creates the context menu according to view options
*
******************************************************/
class ContextMenuProvider : public wxObject
{
public:
	wxMenu *getContextMenu( ChildWindow*, BasicView* );
	wxMenu *getAppWindowContextMenu();

private:
	wxMenu *getSkinMenu();
	wxMenu *getSarvatobhadraMenu();
	wxMenu *getObjectSubMenu();
	wxMenu *getChartMenu();
	wxMenu *getNewViewMenu();
	wxMenu *getMainViewColumnMenu();
	wxMenu *getGraphicStyleMenu();

	BasicView *view;
};


#endif


