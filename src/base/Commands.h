/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/base/Commands.h
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


#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#ifdef __GNUG__
#pragma interface "Commands.h"
#endif

#include <wx/hashmap.h>
#include <wx/string.h>

class MaitreyaCommand;
class wxKeyEvent;

WX_DECLARE_HASH_MAP( int, MaitreyaCommand*, wxIntegerHash, wxIntegerEqual, CommandListHash );

/*************************************************//**
*
* \brief keyboard shortcut
*
******************************************************/
class MaitreyaShortcut
{
public:

	MaitreyaShortcut( const int &scKey, const int &scShift, const int &scCtrl );

	wxString toString() const;

	bool matches( const wxKeyEvent& ) const;

private:
	int scKey, scShift, scCtrl, scMeta, scAlt;
};

/*************************************************//**
*
* \brief encapsulation of a command, used for construction of menu and toolbar items
*
******************************************************/
class MaitreyaCommand
{
public:
	MaitreyaCommand( const int &bmpId, const wxChar *text, const MaitreyaShortcut *shortcut = 0 )
			: bmpId( bmpId ),
			text( text ),
			shortcut( shortcut ) {}

	~MaitreyaCommand() {
		if ( shortcut ) delete shortcut;
	}

	wxString getMenuRepresentation();

	int getBitmapId() const {
		return bmpId;
	}

	const MaitreyaShortcut *getShortcut() const {
		return shortcut;
	}

	const wxChar *getText() const {
		return text;
	}

private:
	const int bmpId;
	const wxChar *text;
	const MaitreyaShortcut *shortcut;
};

/*************************************************//**
*
* \brief holds the list of commands and handles lookups for key events
*
******************************************************/
class CommandHandler
{
public:
	static CommandHandler *get();

	MaitreyaCommand *getCommand( const int &id );
	int lookupCommand4KeyEvent( const wxKeyEvent& );
	int lookupCommand4CharEvent( wxKeyEvent );

protected:
	void init();

	CommandListHash cmds;

private:
	CommandHandler();
	static CommandHandler* ego;
};

#endif

