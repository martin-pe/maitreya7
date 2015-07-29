/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/ToolPanel.h
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

#ifndef _TOOLPANEL_H_
#define _TOOLPANEL_H_

#ifdef __GNUG__
#pragma interface "ToolPanel.h"
#endif

#include <wx/toolbar.h>
#include <vector>

using namespace std;

/*************************************************//**
*
* \brief single command in tool bar
*
******************************************************/
class ToolBarCommand
{
public:
	ToolBarCommand() {
		commandId = 0;
		bitmapId = -1;
		description = wxEmptyString;
		isDefault = false;
		imageListId = 0;
	}
	ToolBarCommand( const int &id, const int& bmpId, const wxString &s, const bool &isdef, const int &imageId )
	{
		commandId = id;
		bitmapId = bmpId;
		description = s;
		isDefault = isdef;
		imageListId = imageId;
	}

	int commandId, bitmapId, imageListId;
	wxString description;
	bool isDefault;
};

/*************************************************//**
*
* \brief toolbar with config support
*
******************************************************/
class MaitreyaToolbar : public wxToolBar
{
public:
	MaitreyaToolbar( wxWindow* parent );
	void updateTools( const bool &isDoc );
	void recreate();

private:
	  void OnChar( wxKeyEvent& );
};

/*************************************************//**
*
* \brief encapsulates creation of toolbar
*
******************************************************/
class ToolbarFactory
{
public:
	static ToolbarFactory *get();

	wxToolBar *createToolbar( wxWindow*, const int &type );

	MaitreyaToolbar* createMainToolbar( wxWindow* );

	const vector<ToolBarCommand> &getFullCommandList() { return cmdlist; }

	vector<ToolBarCommand> getCommandListFromString( wxString toolbarConfig );
	vector<ToolBarCommand> getDefaultCommandList();

	ToolBarCommand getItemById( const int &i );

private:
	ToolbarFactory();
	void init();

	vector<ToolBarCommand> cmdlist;
	wxToolBar *toolbar;
	static ToolbarFactory *ego;
	void create( const int* );
	void createChoiceItem( const int& );
	void createButtonItem( const int& );
	void createTextField();
};

#endif


