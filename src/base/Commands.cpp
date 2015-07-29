/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/base/Commands.cpp
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
#pragma implementation "Commands.h"
#endif

#include "Commands.h"
#include "constants.h"
#include "IconProvider.h"

#include <wx/event.h>

CommandHandler *CommandHandler::ego = 0;

/*****************************************************
**
**   MaitreyaShortcut   ---   Constructor
**
******************************************************/
MaitreyaShortcut::MaitreyaShortcut( const int &scKey, const int &scShift, const int &scCtrl )
{
	this->scKey = scKey;
	this->scShift = scShift;
	this->scCtrl = scCtrl;
	scMeta = scAlt = 0;

	/*
	   MacOS workaround: simple shortcuts like '0' are caught in spin boxes, date input fields etc.
		 So these Shortcuts get additional 'Alt' modifier.

	  Relevant are all shortcuts without shift and ctrl
		letters (l, p ) are caught in the Yoga Editor text fields
	*/
#if defined( __WXMAC__ )
	if ( scShift == 0 && scCtrl == 0 )
	{
		scAlt = 1;
	}
#endif
}

/*****************************************************
**
**   MaitreyaShortcut   ---   toString
**
******************************************************/
wxString MaitreyaShortcut::toString() const
{
	wxString s;

	if ( scShift ) s << wxT( "Shift+" );
	if ( scCtrl ) s << wxT( "Ctrl+" );
	if ( scMeta ) s << wxT( "Meta+" );
	if ( scAlt ) s << wxT( "Alt+" );
	switch ( scKey )
	{
	case WXK_F1:
		s << wxT( "F1" );
		break;
	case WXK_ESCAPE:
		s << wxT( "Esc" );
		break;
	default:
		s << (wxChar)wxToupper( scKey );
		break;
	}
	return s;
}

/*****************************************************
**
**   MaitreyaShortcut   ---   matches
**
******************************************************/
bool MaitreyaShortcut::matches( const wxKeyEvent &event ) const
{
	return( event.GetKeyCode() == scKey
	        && event.m_shiftDown == (bool)scShift
	        && event.m_controlDown == (bool)scCtrl
	        && event.m_metaDown == (bool)scMeta
	        && event.m_altDown == (bool)scAlt);
}

/*****************************************************
**
**   MaitreyaCommand   ---   getMenuRepresentation
**
******************************************************/
wxString MaitreyaCommand::getMenuRepresentation()
{
	wxString s = text;
	if ( shortcut != 0 ) s << wxT( "\t" ) << shortcut->toString();
	return s;
}

/*****************************************************
**
**   CommandHandler   ---   Constructor
**
******************************************************/
CommandHandler::CommandHandler()
{
	init();
}

/*****************************************************
**
**   CommandHandler   ---   get
**
******************************************************/
CommandHandler *CommandHandler::get()
{
	if ( ! ego )
	{
		ego = new CommandHandler();
	}
	return ego;
}

/*****************************************************
**
**   CommandHandler   ---   getCommand
**
******************************************************/
MaitreyaCommand *CommandHandler::getCommand( const int &id )
{
	return cmds[id];
}

/*****************************************************
**
**   CommandHandler   ---   lookupCommand4CharEvent
**
******************************************************/
int CommandHandler::lookupCommand4CharEvent( wxKeyEvent event )
{
	int code = event.GetKeyCode();
	//printf( "CODE 1 %d %c\n", code, code );

	// Control pressed
	if ( code < 27 )
	{
		code = 'A' + code - 1;
	}
	code = toupper( code );
	event.m_keyCode = code;

	if ( code == WXK_ESCAPE ) return CMD_CHILD_CLOSE;
	for ( CommandListHash::iterator it = cmds.begin(); it != cmds.end(); it++ )
	{
		if ( it->second && it->second->getShortcut() && it->second->getShortcut()->matches( event )) return it->first;
	}
	return false;
}

/*****************************************************
**
**   CommandHandler   ---   lookupCommand4KeyEvent
**
******************************************************/
int CommandHandler::lookupCommand4KeyEvent( const wxKeyEvent &event )
{
	if ( event.GetKeyCode() == WXK_ESCAPE ) return CMD_CHILD_CLOSE;

	for ( CommandListHash::iterator it = cmds.begin(); it != cmds.end(); it++ )
	{
		if ( it->second && it->second->getShortcut() && it->second->getShortcut()->matches( event )) return it->first;
	}
	return false;
}

/*****************************************************
**
**   CommandHandler   ---   init
**
******************************************************/
void CommandHandler::init()
{
	// App File
	cmds[APP_FILENEW] = new MaitreyaCommand( BITMAP_NEW, _( "New" ), new MaitreyaShortcut( 'N', 0, 1 ));
	cmds[APP_FILE_NEW_SIMPLE] = new MaitreyaCommand( BITMAP_SVIEW, _( "New Simple Child View" ), new MaitreyaShortcut( 'N', 1, 1 ));
	cmds[APP_FILE_NEW_MULTIPLE_DEFAULT] = new MaitreyaCommand( BITMAP_MVIEW, _( "New Multiple Child View" ), new MaitreyaShortcut( 'M', 1, 1 ));
	cmds[APP_FILEOPEN] = new MaitreyaCommand( BITMAP_OPEN, _( "Open" ), new MaitreyaShortcut( 'O', 0, 1 ));
	cmds[APP_FILEQUIT] = new MaitreyaCommand( BITMAP_QUIT, _( "Quit Application" ), new MaitreyaShortcut( 'Q', 0, 1 ));

	cmds[CMD_CHILD_CLOSE] = new MaitreyaCommand( BITMAP_CLOSE, _( "Close" ), new MaitreyaShortcut( 'W', 0, 1 ));
	cmds[CMD_CHILD_CLOSE_DOC] = new MaitreyaCommand( BITMAP_CLOSE, _( "Close Document" ), new MaitreyaShortcut( 'W', 1, 1 ));
	cmds[CMD_CHILD_SAVE] = new MaitreyaCommand( BITMAP_SAVE, _( "Save" ), new MaitreyaShortcut( 'S', 0, 1 ));
	cmds[CMD_CHILD_SAVEAS] = new MaitreyaCommand( BITMAP_SAVEAS, _( "Save As" ), new MaitreyaShortcut( 'A', 0, 1 ));
	cmds[CMD_CHILD_PRINT] = new MaitreyaCommand( BITMAP_PRINT, _( "Print Chart" ), new MaitreyaShortcut( 'P', 0, 1 ));
	cmds[CMD_CHILD_QUICKPRINT] = new MaitreyaCommand( BITMAP_QUICKPRINT, _( "Quick Print Chart" ), new MaitreyaShortcut( 'P', 1, 1 ));

	// child new view
	cmds[CMD_CHILD_NEW_TEXT] = new MaitreyaCommand( BITMAP_TEXT, _( "Text View" ), new MaitreyaShortcut( 'T', 0, 0 ));
	cmds[CMD_CHILD_NEW_TRANSIT] = new MaitreyaCommand( BITMAP_TRANSIT, _( "Transit View" ), new MaitreyaShortcut( 'R', 0, 0 ));
	cmds[CMD_CHILD_NEW_YOGA] = new MaitreyaCommand( BITMAP_YOGA, _( "Yoga View" ), new MaitreyaShortcut( 'Y', 0, 0 ));
	cmds[CMD_CHILD_NEW_SBC] = new MaitreyaCommand( BITMAP_SBC, _( "Sarvatobhadra View" ), new MaitreyaShortcut( 'S', 0, 0 ));
	cmds[CMD_CHILD_NEW_SOLAR] = new MaitreyaCommand( BITMAP_SOLAR, _( "Solar View" ), new MaitreyaShortcut( 'T', 0, 1 ));
	cmds[CMD_CHILD_NEW_URANIAN] = new MaitreyaCommand( BITMAP_URANIAN, _( "Uranian View" ), new MaitreyaShortcut( 'U', 0, 0 ));
	cmds[CMD_CHILD_NEW_VARGA] = new MaitreyaCommand( BITMAP_VARGA, _( "Varga Summary View" ), new MaitreyaShortcut( 'V', 0, 1 ));
	cmds[CMD_CHILD_NEW_WCHART] = new MaitreyaCommand( BITMAP_WCHART, _( "Western Chart" ), new MaitreyaShortcut( '0', 0, 0 ));
	cmds[CMD_CHILD_NEW_ASHTAKAVARGA] = new MaitreyaCommand( BITMAP_ASHTAKAVARGA, _( "Ashtakavarga View" ), new MaitreyaShortcut( 'A', 0, 0 ));
	cmds[CMD_CHILD_NEW_DASA] = new MaitreyaCommand( BITMAP_DASA, _( "Dasa View (Tree)" ), new MaitreyaShortcut( 'D', 0, 0 ));
	cmds[CMD_CHILD_NEW_GRAPHICALDASA] = new MaitreyaCommand( BITMAP_GRAPHICALDASA, _( "Dasa View (Graphical)" ), new MaitreyaShortcut( 'G', 0, 0 ));
	cmds[CMD_CHILD_NEW_RASI] = new MaitreyaCommand( BITMAP_CHART, _( "Rasi (D-1)" ), new MaitreyaShortcut( '1', 0, 0 ));
	cmds[CMD_CHILD_NEW_RASI+1] = new MaitreyaCommand( BITMAP_CHART9, _( "Navamsa (D-9)" ), new MaitreyaShortcut( '2', 0, 0 ));
	cmds[CMD_CHILD_NEW_RASI+2] = new MaitreyaCommand( BITMAP_CHART2, _( "Hora (D-2)" ), new MaitreyaShortcut( '3', 0, 0 ));
	cmds[CMD_CHILD_NEW_RASI+3] = new MaitreyaCommand( BITMAP_CHART3, _( "Drekkana (D-3)" ), new MaitreyaShortcut( '4', 0, 0 ));
	cmds[CMD_CHILD_NEW_RASI+4] = new MaitreyaCommand( BITMAP_CHART4, _( "Chaturthamsa (D-4)" ), new MaitreyaShortcut( '5', 0, 0 ));
	cmds[CMD_CHILD_NEW_RASI+5] = new MaitreyaCommand( BITMAP_CHART6, _( "Sastamsa (D-6)" ), new MaitreyaShortcut( '6', 0, 0 ));
	cmds[CMD_CHILD_NEW_RASI+6] = new MaitreyaCommand( BITMAP_CHART7, _( "Saptamamsa (D-7)" ), new MaitreyaShortcut( '7', 0, 0 ));
	cmds[CMD_CHILD_NEW_RASI+7] = new MaitreyaCommand( BITMAP_CHART8, _( "Ashtamsa (D-8)" ), new MaitreyaShortcut( '8', 0, 0 ));
	cmds[CMD_CHILD_NEW_RASI+8] = new MaitreyaCommand( BITMAP_CHART10, _( "Dasamsa (D-10)" ), new MaitreyaShortcut( '9', 0, 0 ));
	cmds[CMD_CHILD_NEW_RASI+9] = new MaitreyaCommand( BITMAP_CHART12, _( "Dvadasamsa (D-12)" ), new MaitreyaShortcut( '0', 0, 1 ));
	cmds[CMD_CHILD_NEW_RASI+10] = new MaitreyaCommand( BITMAP_CHART16, _( "Shodasamsa (D-16)" ), new MaitreyaShortcut( '1', 0, 1 ));
	cmds[CMD_CHILD_NEW_RASI+11] = new MaitreyaCommand( BITMAP_CHART20, _( "Vimsamsa (D-20)" ), new MaitreyaShortcut( '2', 0, 1 ));
	cmds[CMD_CHILD_NEW_RASI+12] = new MaitreyaCommand( BITMAP_CHART24, _( "Siddhamsa (D-24)" ), new MaitreyaShortcut( '3', 0, 1 ));
	cmds[CMD_CHILD_NEW_RASI+13] = new MaitreyaCommand( BITMAP_CHART27, _( "Bhamsa (D-27)" ), new MaitreyaShortcut( '4', 0, 1 ));
	cmds[CMD_CHILD_NEW_RASI+14] = new MaitreyaCommand( BITMAP_CHART30, _( "Trimsamsa (D-30)" ), new MaitreyaShortcut( '5', 0, 1 ));
	cmds[CMD_CHILD_NEW_RASI+15] = new MaitreyaCommand( BITMAP_CHART40, _( "Chatvarimsamsa (D-40)" ), new MaitreyaShortcut( '6', 0, 1 ));
	cmds[CMD_CHILD_NEW_RASI+16] = new MaitreyaCommand( BITMAP_CHART45, _( "Akshavedamsa (D-45)" ), new MaitreyaShortcut( '7', 0, 1 ));
	cmds[CMD_CHILD_NEW_RASI+17] = new MaitreyaCommand( BITMAP_CHART60, _( "Shashtiamsa (D-60)" ), new MaitreyaShortcut( '8', 0, 1 ));
	cmds[CMD_CHILD_NEW_RASI+18] = new MaitreyaCommand( BITMAP_CHART108, _( "Ashtottaramsa (D-108)" ), new MaitreyaShortcut( '9', 0, 1 ));
	cmds[CMD_CHILD_NEW_RASI+19] = new MaitreyaCommand( BITMAP_CHART144, _( "Dvadas-Davadamsa (D-144)" ), new MaitreyaShortcut( '1', 1, 1 ));
	cmds[CMD_CHILD_NEW_RASI+20] = new MaitreyaCommand( BITMAP_CHARTB, _( "Bhava Chart" ), new MaitreyaShortcut( '2', 1, 1 ));

	// App New View
	cmds[APP_SHOWHORA] = new MaitreyaCommand( BITMAP_HORA, _( "Hora View" ), new MaitreyaShortcut( 'H', 0, 0 ));
	cmds[APP_SHOWECLIPSE] = new MaitreyaCommand( BITMAP_ECLIPSE, _( "Eclipse View" ), new MaitreyaShortcut( 'E', 0, 1 ));
	cmds[APP_NEW_PARTNER] = new MaitreyaCommand( BITMAP_PARTNER, _( "Partner View" ), new MaitreyaShortcut( 'P', 0, 0 ));
	cmds[APP_SHOWEPHEM] = new MaitreyaCommand( BITMAP_EPHEM, _( "Ephemeris View" ), new MaitreyaShortcut( 'E', 0, 0 ));
	cmds[APP_CONFIGURE] = new MaitreyaCommand( BITMAP_CONFIG, _( "Configuration" ), new MaitreyaShortcut( 'C', 0, 0 ));
	cmds[APP_YOGAEDITOR] = new MaitreyaCommand( BITMAP_YOGAEDITOR, _( "Yoga Editor" ), new MaitreyaShortcut( 'Y', 0, 1 ));
	cmds[APP_LOCATION] = new MaitreyaCommand( BITMAP_WORLD, _( "Atlas" ), new MaitreyaShortcut( 'L', 0, 0 ));
	cmds[APP_ABOUT] = new MaitreyaCommand( BITMAP_ABOUT, _( "About" ), new MaitreyaShortcut( WXK_F1, 1, 0 ));
	cmds[APP_HELP] = new MaitreyaCommand( BITMAP_HELP, _( "Documentation (Web)" ), new MaitreyaShortcut( WXK_F1, 0, 0 ));

	// child varia
	cmds[CMD_CHILD_ANIMATE] = new MaitreyaCommand( 0, _( "Animate Chart" ), new MaitreyaShortcut( 'A', 1, 1 ));
	cmds[CMD_CHILD_VEDICMODE] = new MaitreyaCommand( 0, _( "Vedic Mode" ), new MaitreyaShortcut( 'V', 0, 0 ));
	cmds[CMD_CHILD_WESTERNMODE] = new MaitreyaCommand( 0, _( "Western Mode" ), new MaitreyaShortcut( 'W', 0, 0 ));
	cmds[CMD_CHILD_EDITDATA] = new MaitreyaCommand( BITMAP_EDIT, _( "Edit Chart Data" ), new MaitreyaShortcut( 'B', 0, 0 ));
	cmds[CMD_CHILD_SAVETEXT] = new MaitreyaCommand( BITMAP_SAVEAS, _( "Save Text As ..." ));
	cmds[CMD_CHILD_EXPORT] = new MaitreyaCommand( 0, _( "Save Picture As ..." ));

	// App Varia
	cmds[APP_SAVECONFIG] = new MaitreyaCommand( BITMAP_SAVE, _( "Save Configuration" ));
	cmds[APP_SHOWMAINTOOLBAR] = new MaitreyaCommand( 0, _( "Show Toolbar" ));
	cmds[APP_SHOWSTATUSBAR] = new MaitreyaCommand( 0, _( "Show Statusbar" ));
}

