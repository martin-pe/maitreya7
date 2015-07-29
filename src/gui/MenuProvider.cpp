/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/MenuProvider.cpp
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
#pragma implementation "MenuProvider.h"
#endif

#include "MenuProvider.h"

#include "BasicView.h"
#include "Conf.h"
#include "Commands.h"
#include "ChildWindow.h"
#include "Document.h"
#include "guibase.h"
#include "IconProvider.h"
#include "MultipleViewConfig.h"
#include "VedicChartConfig.h"
#include "WesternChartConfig.h"

extern Config *config;

IMPLEMENT_CLASS( MyMenu, wxMenu )

/**************************************************************
***
**    MyMenu   ---   Constructor
***
***************************************************************/
MyMenu::MyMenu( const wxString title, const BasicView *view )
		: wxMenu( title ),
		view( view )
{
	handler = CommandHandler::get();
}

/**************************************************************
***
**    MyMenu   ---   addItem
***
***************************************************************/
void MyMenu::addItem( const int &id )
{
	MaitreyaCommand *command = handler->getCommand( id );
	if ( command )
	{
		wxMenuItem *item = new wxMenuItem( this, id, command->getMenuRepresentation());
		if ( command->getBitmapId() != 0 ) item->SetBitmap( IconProvider::getMenuBitmap( command->getBitmapId() ));
		Append( item );
	}
	else assert( 0 );
}

/**************************************************************
***
**    MyMenu   ---   addCheckItem
***
***************************************************************/
void MyMenu::addCheckItem( const int &id, const bool checked )
{
	MaitreyaCommand *command = handler->getCommand( id );
	if ( command )
	{
		AppendCheckItem( id, command->getMenuRepresentation());
		Check( id, checked );
	}
	else assert( 0 );
}

/**************************************************************
***
**    MyMenu   ---   addCheckItem
***
***************************************************************/
void MyMenu::addCheckItem( const int &id, const wxString title, const bool checked )
{
	AppendCheckItem( id, title );
	Check( id, checked );
}

/**************************************************************
***
**    MyMenu   ---   addArray
***
***************************************************************/
void MyMenu::addArray( const int a[] )
{
	while ( *a != -1 )
	{
		if ( *a ) addItem( *a );
		else AppendSeparator();
		*a++;
	}
}

/**************************************************************
***
**    MyMenu   ---   addWindowListMenu
***
***************************************************************/
void MyMenu::addWindowListMenu()
{
	wxMenu *wlmenu = new wxMenu();
	wlmenu->AppendRadioItem( APP_LIST_BIGICONS, _( "Big Icons" ) );
	wlmenu->AppendRadioItem( APP_LIST_SMALLICONS, _( "Small Icons" ) );
	wlmenu->AppendRadioItem( APP_LIST_TABLE, _( "Table" ) );
	switch ( config->listStyle )
	{
	case 1:
		wlmenu->Check( APP_LIST_SMALLICONS, true );
		break;
	case 2:
		wlmenu->Check( APP_LIST_TABLE, true );
		break;
	default:
		wlmenu->Check( APP_LIST_BIGICONS, true );
		break;
	}
	Append( APP_LISTCTRL, _("Window List"), wlmenu );
}

/**************************************************************
***
**    MyMenu   ---   addVargaMenu
***
***************************************************************/
void MyMenu::addVargaMenu()
{
	MyMenu *menu = new MyMenu( wxEmptyString, view );
	for ( int i = 0; i < NB_VARGAS; i++ ) menu->addItem( CMD_CHILD_NEW_RASI + i );
	Append( -1, _( "Varga"), menu );
}

/*****************************************************
**
**   AppMenuBar   ---   Constructor
**
******************************************************/
AppMenuBar::AppMenuBar( int style )
		: wxMenuBar( style )
{
	const int docitems[] = { CMD_CHILD_EDITDATA, 0, CMD_CHILD_CLOSE_DOC, CMD_CHILD_SAVE, CMD_CHILD_SAVEAS, 0,
		CMD_CHILD_PRINT, CMD_CHILD_QUICKPRINT, 0, -1 };
	const int vedicitems[12] = { CMD_CHILD_NEW_VARGA, CMD_CHILD_NEW_SBC, CMD_CHILD_NEW_TEXT, 0,
		CMD_CHILD_NEW_DASA, CMD_CHILD_NEW_GRAPHICALDASA, CMD_CHILD_NEW_TRANSIT, 0, CMD_CHILD_NEW_YOGA,
		CMD_CHILD_NEW_ASHTAKAVARGA, CMD_CHILD_NEW_SOLAR, -1 };
	const int westernitems[8] = { CMD_CHILD_NEW_WCHART, CMD_CHILD_NEW_TEXT, 0, CMD_CHILD_NEW_URANIAN,
		CMD_CHILD_NEW_TRANSIT, 0, CMD_CHILD_NEW_SOLAR, -1 };
	const int extraitems[11] = { APP_SHOWEPHEM, APP_SHOWECLIPSE, APP_SHOWHORA, APP_NEW_PARTNER, 0,
		APP_CONFIGURE, APP_LOCATION, APP_YOGAEDITOR, APP_SAVECONFIG, 0, -1 };

	newmenu = new MyMenu( wxEmptyString, 0 );
	vector<BaseXmlConfig*> v = MultipleViewConfigLoader::get()->getConfigs();
	for ( unsigned int i = 0; i < v.size(); i++ )
	{
		newmenu->Append( APP_FILE_NEW_MULTIPLE + i, v[i]->name );
	}

	recentmenu = new MyMenu( wxEmptyString, 0 );
	wxString rfiles = config->recentFileList;
	int i = rfiles.Find( '\t' );
	while ( i != -1 )
	{
		recentfiles.Add( rfiles.Left( i ));
		rfiles = rfiles.Right( rfiles.Len() - i - 1 );
		i = rfiles.Find( '\t' );
	}
	if ( rfiles.Len() > 0 ) recentfiles.Add( rfiles );
	for ( size_t n = 0; n < recentfiles.GetCount(); n++ ) recentmenu->Append( APP_FILERECENT + n, recentfiles[n] );

	filemenu = new MyMenu( wxEmptyString, 0 );
	filemenu->addItem( APP_FILENEW );

	if ( ! config->useMultipleViews ) filemenu->addItem( APP_FILE_NEW_SIMPLE );
	newChildSimpleChildCommandIncluded = ! config->useMultipleViews;

	filemenu->Append( APP_FILE_NEW_MULTIPLE + 99, _( "New Multiple View" ), newmenu );
	filemenu->addItem( APP_FILEOPEN );
	filemenu->Append( APP_FILERECENT, _("Open Recent"), recentmenu );
	filemenu->AppendSeparator();
	filemenu->addItem( APP_FILEQUIT );
	Append( filemenu, _( "&File" ));

	docmenu = new MyMenu( wxEmptyString, 0 );
	docmenu->addArray( docitems );
	docmenu->AppendCheckItem( CMD_CHILD_ANIMATE, _("Animate" ));
	Append( docmenu, _( "&Document" ));

	vedicmenu = new MyMenu( wxEmptyString, 0 );
	vedicmenu->addVargaMenu();
	vedicmenu->addArray( vedicitems );
	Append( vedicmenu, _( "&Vedic"));

	westernmenu = new MyMenu( wxEmptyString, 0 );
	westernmenu->addArray( westernitems );
	Append( westernmenu, _( "&Western"));

	extramenu = new MyMenu( wxEmptyString, 0 );
	extramenu->addArray( extraitems );
	extramenu->addWindowListMenu();
	extramenu->addCheckItem( APP_SHOWMAINTOOLBAR, config->showMainToolbar );
	extramenu->addCheckItem( APP_SHOWSTATUSBAR, config->showStatusBar );
	Append( extramenu, _( "&Extras"));

	helpmenu = new MyMenu( wxEmptyString, 0 );
	helpmenu->addItem( APP_HELP );
	helpmenu->addItem( APP_ABOUT );
	//helpmenu->addItem( wxID_ABOUT );
	Append( helpmenu, _( "&Help"));

}

/*****************************************************
**
**   AppMenuBar   ---   updateMenus
**
******************************************************/
void AppMenuBar::updateMenus( const bool &isDoc, const bool &animated )
{
	EnableTop( 1, isDoc );
	EnableTop( 2, isDoc );
	EnableTop( 3, isDoc );

	filemenu->Enable( APP_FILERECENT, recentfiles.GetCount() > 0  );
	if ( isDoc )
	{
		docmenu->Check( CMD_CHILD_ANIMATE, animated );
	}
	
	if ( config->useMultipleViews )
	{
		if ( ! newChildSimpleChildCommandIncluded )
		{
			// Insert Item for new single view
			MaitreyaCommand *command = CommandHandler::get()->getCommand( APP_FILE_NEW_SIMPLE );
			assert( command );
			wxMenuItem *item = new wxMenuItem( filemenu, APP_FILE_NEW_SIMPLE, command->getMenuRepresentation());
			if ( command->getBitmapId() != 0 ) item->SetBitmap( IconProvider::getMenuBitmap( command->getBitmapId() ));
			filemenu->Insert( 1, item );
			newChildSimpleChildCommandIncluded  = true;
		}
	}
	else
	{
		if ( newChildSimpleChildCommandIncluded )
		{
			filemenu->Delete( APP_FILE_NEW_SIMPLE );
			newChildSimpleChildCommandIncluded  = false;
		}
	}
}

/*****************************************************
**
**   AppMenuBar   ---   addToRecentFiles
**
******************************************************/
void AppMenuBar::addToRecentFiles( const wxString &filename )
{
	unsigned int maxrecent = config->maxRecentFiles;
	unsigned int recentcount = recentmenu->GetMenuItemCount();
	size_t n;

	// clear recent file menu
	for ( n = 0; n < recentcount; n++ ) recentmenu->Delete( APP_FILERECENT + n );

	// Clear obsolete items ( if number of files has changed via ConfigDialog)
	if ( recentfiles.GetCount() > maxrecent )
	{
		for ( n = 0; n < recentfiles.GetCount() - maxrecent; n++ )
			recentfiles.RemoveAt( maxrecent + n );
	}

	// update file list
	for ( n = 0; n < recentfiles.GetCount(); n++ )
	{
		if ( recentfiles[n] == filename )
		{
			recentfiles.RemoveAt( n );
			break;
		}
	}
	if ( recentfiles.GetCount() == (unsigned int)config->maxRecentFiles ) recentfiles.RemoveAt( recentfiles.GetCount() - 1 );
	recentfiles.Insert( filename, 0 );
	for ( n = 0; n < recentfiles.GetCount(); n++ ) recentmenu->Append( APP_FILERECENT + n, recentfiles[n] );
}

/*****************************************************
**
**   AppMenuBar   ---   getRecentFilesEntry
**
******************************************************/
wxString AppMenuBar::getRecentFilesEntry( const int &index )
{
	return recentfiles[ index - APP_FILERECENT ];
}

/*****************************************************
**
**   AppMenuBar   ---   saveRecentFiles
**
******************************************************/
void AppMenuBar::saveRecentFiles()
{
	wxString rfiles = wxT( "" );
	size_t n;

	for ( n = 0; n < recentfiles.GetCount(); n++ )
	{
		if ( n > 0 ) rfiles += wxT( "\t" );
		rfiles += recentfiles[n];
	}
	config->recentFileList = rfiles;
}

/**************************************************************
***
**   ContextMenuProvider   ---   getContextMenu
***
***************************************************************/
wxMenu *ContextMenuProvider::getContextMenu( ChildWindow *child, BasicView *basicview )
{
	assert( basicview );
	int count = 0;
	view = basicview;
	MyMenu *menu = new MyMenu( child->GetTitle(), view );

	bool ismainview = view->isMainView();

	menu->Append( -1, _( "File" ), getChartMenu());
	if ( view->getDoc()) menu->addItem( CMD_CHILD_EDITDATA );

	if ( view->supportsEWToggle() )
	{
		menu->AppendSeparator();
		menu->addCheckItem( CMD_CHILD_VEDICMODE, view->isVedic());
		menu->addCheckItem( CMD_CHILD_WESTERNMODE, ! view->isVedic());
	}

	menu->AppendSeparator();
	if ( view->supportsObjectToggle() || view->supportsRestrictedObjectToggle())
	{
		menu->AppendSubMenu( getObjectSubMenu(), _( "Objects"));
		count++;
	}
	if ( view->supportsFullGraphicStyleToggle() || view->supportsRestrictedGraphicStyleToggle())
	{
		menu->AppendSubMenu( getGraphicStyleMenu(), _( "Graphic Options"));
		count++;
	}
	if ( view->supportsSbcStyleToggle() ) // SBC
	{
		menu->AppendSubMenu( getSarvatobhadraMenu(), _( "Sarvatobhadra"));
		count++;
	}
	if ( view->supportsSkinToggle())
	{
		menu->AppendSubMenu( getSkinMenu(), _( "Skin" ));
		count++;
	}
	if ( ismainview )
	{
		menu->AppendSubMenu( getMainViewColumnMenu(), _( "Columns"));
		count++;
	}
	if ( count > 0 ) menu->AppendSeparator();

	menu->AppendSubMenu( getNewViewMenu(), _( "New View"));
	if ( view->getDoc()) // TODO auch für non doc
	{
		menu->AppendSeparator();
		menu->Append( CMD_CHILD_ANIMATE, _( "Animate"), wxT( "" ), true );
		menu->Check( CMD_CHILD_ANIMATE, view->getDoc()->isAnimated());
	}
	if ( view->supportsGraphicExport() )
	{
		menu->AppendSeparator();
		menu->addItem( CMD_CHILD_EXPORT );
	}
	if ( view->supportsTextExport() )
	{
		menu->AppendSeparator();
		//menu->addItem( CMD_CHILD_SELECT_ALL );
		//menu->addItem( CMD_CHILD_COPY );
		menu->addItem( CMD_CHILD_SAVETEXT );
	}
	return menu;
}

/**************************************************************
***
**    ContextMenuProvider   ---   getMainViewColumnMenu
***
***************************************************************/
wxMenu *ContextMenuProvider::getMainViewColumnMenu()
{
	wxMenu *menu = new wxMenu( _( "Columns" ));
	if ( view->isVedic())
	{
		menu->Append( CMD_CHILD_MAIN_SHOW_DECLINATION, _( "Declinations" ), wxT( "" ), true );
		menu->Check( CMD_CHILD_MAIN_SHOW_DECLINATION, view->getVedicMainStyle() & MAIN_CHILD_SHOW_DECLINATION );
		menu->Append( CMD_CHILD_MAIN_SHOW_LORD, _( "Lord" ), wxT( "" ), true );
		menu->Check( CMD_CHILD_MAIN_SHOW_LORD, view->getVedicMainStyle() & MAIN_CHILD_SHOW_LORD );
		menu->Append( CMD_CHILD_MAIN_SHOW_DIGNITY, _( "Dignity" ), wxT( "" ), true );
		menu->Check( CMD_CHILD_MAIN_SHOW_DIGNITY, view->getVedicMainStyle() & MAIN_CHILD_SHOW_DIGNITY );
		menu->Append( CMD_CHILD_MAIN_SHOW_NAVAMSA, _( "Navamsa (D-9)" ), wxT( "" ), true );
		menu->Check( CMD_CHILD_MAIN_SHOW_NAVAMSA, view->getVedicMainStyle() & MAIN_CHILD_SHOW_NAVAMSA );
		menu->Append( CMD_CHILD_MAIN_SHOW_NAKSHATRA, _( "Nakshatra" ), wxT( "" ), true );
		menu->Check( CMD_CHILD_MAIN_SHOW_NAKSHATRA, view->getVedicMainStyle() & MAIN_CHILD_SHOW_NAKSHATRA );
		menu->Append( CMD_CHILD_MAIN_SHOW_PADA, _( "Nakshatra Padas" ), wxT( "" ), true );
		menu->Check( CMD_CHILD_MAIN_SHOW_PADA, view->getVedicMainStyle() & MAIN_CHILD_SHOW_PADA );

		menu->Append( CMD_CHILD_MAIN_SHOW_KARAKA, _( "Jaimini Karaka" ), wxT( "" ), true );
		menu->Check( CMD_CHILD_MAIN_SHOW_KARAKA, view->getVedicMainStyle() & MAIN_CHILD_SHOW_KARAKA );
		menu->Append( CMD_CHILD_MAIN_SHOW_KP, _( "Krishnamurti Lords" ), wxT( "" ), true );
		menu->Check( CMD_CHILD_MAIN_SHOW_KP, view->getVedicMainStyle() & MAIN_CHILD_SHOW_KP );

		menu->Append( CMD_CHILD_MAIN_SHOW_SHASTIAMSA, _( "Shastiamsa (D-60)" ), wxT( "" ), true );
		menu->Check( CMD_CHILD_MAIN_SHOW_SHASTIAMSA, view->getVedicMainStyle() & MAIN_CHILD_SHOW_SHASTIAMSA );
		menu->Append( CMD_CHILD_MAIN_SHOW_DASAVARGA, _( "Dasa Varga Dignity" ), wxT( "" ), true );
		menu->Check( CMD_CHILD_MAIN_SHOW_DASAVARGA, view->getVedicMainStyle() & MAIN_CHILD_SHOW_DASAVARGA );
		menu->Append( CMD_CHILD_MAIN_SHOW_HOUSEPOS, _( "Bhava" ), wxT( "" ), true );
		menu->Check( CMD_CHILD_MAIN_SHOW_HOUSEPOS, view->getVedicMainStyle() & MAIN_CHILD_SHOW_HOUSEPOS );
		menu->Append( CMD_CHILD_MAIN_SHOW_ASHTAKA, _( "Ashtakavarga" ), wxT( "" ), true );
		menu->Check( CMD_CHILD_MAIN_SHOW_ASHTAKA, view->getVedicMainStyle() & MAIN_CHILD_SHOW_ASHTAKA );
	}
	else
	{
		menu->Append( CMD_CHILD_MAIN_SHOW_HOUSEPOS, _( "House Positions" ), wxT( "" ), true );
		menu->Check( CMD_CHILD_MAIN_SHOW_HOUSEPOS, view->getWesternMainStyle() & MAIN_CHILD_SHOW_HOUSEPOS );
		menu->Append( CMD_CHILD_MAIN_SHOW_DECLINATION, _( "Declinations" ), wxT( "" ), true );
		menu->Check( CMD_CHILD_MAIN_SHOW_DECLINATION, view->getWesternMainStyle() & MAIN_CHILD_SHOW_DECLINATION );
	}
	return menu;
}

/**************************************************************
***
**    ContextMenuProvider   ---   getSkinMenu
***
***************************************************************/
wxMenu *ContextMenuProvider::getSkinMenu()
{
	unsigned int i = 0;
	MyMenu *menu = new MyMenu( _( "Skin" ), view );

	if ( view->isVedic())
	{
		vector<BaseXmlConfig*> list =  VedicChartConfigLoader::get()->getConfigs();
		for ( unsigned i = 0; i < list.size(); i++ )
		{
			menu->addCheckItem( CMD_CHILD_GRAPHIC_STYLE + i, list[i]->name, i == (unsigned int)view->getSkin());
		}
	}
	else
	{
		vector<BaseXmlConfig*> list =  WesternChartConfigLoader::get()->getConfigs();
		for ( i = 0; i < list.size(); i++ )
		{
			menu->addCheckItem( CMD_CHILD_GRAPHIC_STYLE + i, list[i]->name, i == (unsigned int) view->getSkin());
		}
	}
	return menu;
}

/**************************************************************
***
**   ContextMenuProvider   ---   getObjectSubMenu
***
***************************************************************/
wxMenu *ContextMenuProvider::getObjectSubMenu()
{
	MyMenu *menu = new MyMenu( _( "Objects" ), view );
	menu->addCheckItem( CMD_CHILD_SHOWOUTER, _( "3 Outer Planets"), view->getObjectStyle() & OBJECTS_INCLUDE_OUTER );
	menu->addCheckItem( CMD_CHILD_SHOWDRAGONHEAD, _( "Dragon Head"), view->getObjectStyle() & OBJECTS_INCLUDE_DRAGONHEAD );
	menu->addCheckItem( CMD_CHILD_SHOWDRAGONTAIL, _( "Dragon Tail"), view->getObjectStyle() & OBJECTS_INCLUDE_DRAGONTAIL );
	if ( ! view->supportsRestrictedObjectToggle() )
	{
		menu->addCheckItem( CMD_CHILD_SHOWASCENDANT, _( "Ascendant"), view->getObjectStyle() & OBJECTS_INCLUDE_ASCENDANT );
		menu->addCheckItem( CMD_CHILD_SHOWMERIDIAN, _( "Meridian"), view->getObjectStyle() & OBJECTS_INCLUDE_MERIDIAN );
	}
	menu->addCheckItem( CMD_CHILD_SHOWURANIAN, _( "8 Uranian"), view->getObjectStyle() & OBJECTS_INCLUDE_URANIAN );
	menu->addCheckItem( CMD_CHILD_SHOWCHIRON, _( "Chiron"), view->getObjectStyle() & OBJECTS_INCLUDE_CHIRON );
	menu->addCheckItem( CMD_CHILD_SHOWPHOLUS, _( "Pholus"), view->getObjectStyle() & OBJECTS_INCLUDE_PHOLUS );
	menu->addCheckItem( CMD_CHILD_SHOWPLANETOIDS, _( "4 Planetoids"), view->getObjectStyle() & OBJECTS_INCLUDE_PLANETOIDS );
	menu->addCheckItem( CMD_CHILD_SHOWLILITH, _( "Lilith (Black Moon)"), view->getObjectStyle() & OBJECTS_INCLUDE_LILITH );

	if ( ! view->supportsRestrictedObjectToggle() )
	{
		menu->addCheckItem( CMD_CHILD_SHOWARIES, _( "Aries"), view->getObjectStyle() & OBJECTS_INCLUDE_ARIES );
		menu->addCheckItem( CMD_CHILD_SHOWUPAGRAHAS, _( "5 Upagrahas"), view->getObjectStyle() & OBJECTS_INCLUDE_UPAGRAHAS );
		menu->addCheckItem( CMD_CHILD_SHOWKALAVELAS, _( "Kalavelas"), view->getObjectStyle() & OBJECTS_INCLUDE_KALAVELAS );
		menu->addCheckItem( CMD_CHILD_SHOWSPECIALLAGNAS, _( "3 Special Lagnas"), view->getObjectStyle() & OBJECTS_INCLUDE_SPECIALLAGNAS );
		menu->addCheckItem( CMD_CHILD_SHOWD9LAGNA, _( "Sphuta Navamsa Lagna"), view->getObjectStyle() & OBJECTS_INCLUDE_D9_LAGNA );

		size_t size = ( view->isVedic() ? config->vArabicParts.Count() : config->wArabicParts.Count() );
		if ( size > 0 )
		{
			menu->addCheckItem( CMD_CHILD_SHOWARABICPARTS, _( "Arabic Parts"), view->getObjectStyle() & OBJECTS_INCLUDE_ARABICPARTS );
		}
		// show houses only for text views, not in graphical charts
		if ( ! view->supportsRestrictedObjectToggle() )
		{
			menu->addCheckItem( CMD_CHILD_MAIN_SHOW_HOUSES, _( "Houses" ), view->getObjectStyle() & MAIN_CHILD_SHOW_HOUSES );
		}
	}
	return menu;
}

/**************************************************************
***
**   ContextMenuProvider   ---   getGraphicStyleMenu
***
***************************************************************/
wxMenu *ContextMenuProvider::getGraphicStyleMenu()
{
	wxMenu *menu = new MyMenu( _( "Graphic Options" ), view );
	if ( view->isVedic())
	{
		menu->Append( CMD_CHILD_VSHOWSOUTHINDIAN, _("South Indian Style"), wxT( "" ), true );
		menu->Check( CMD_CHILD_VSHOWSOUTHINDIAN, view->getVedicGraphicStyle().indianChartType == VGRAPHIC_SOUTH_INDIAN );
		menu->Append( CMD_CHILD_VSHOWNORTHINDIAN, _("North Indian Style"), wxT( "" ), true );
		menu->Check( CMD_CHILD_VSHOWNORTHINDIAN, view->getVedicGraphicStyle().indianChartType == VGRAPHIC_NORTH_INDIAN );
		menu->Append( CMD_CHILD_VSHOWEASTINDIAN, _("East Indian Style"), wxT( "" ), true );
		menu->Check( CMD_CHILD_VSHOWEASTINDIAN, view->getVedicGraphicStyle().indianChartType == VGRAPHIC_EAST_INDIAN );

		if ( view->getVedicGraphicStyle().indianChartType == VGRAPHIC_NORTH_INDIAN )
		{
			wxMenu *northmenu = new wxMenu();
			northmenu->Append( CMD_CHILD_VSHOWNORTHASC, _("Ascendant Only"), wxT( "" ), true );
			northmenu->Check( CMD_CHILD_VSHOWNORTHASC, view->getVedicGraphicStyle().northIndianSignDisplayType == VGRAPHIC_NORTH_INDIAN_ASC );
			northmenu->Append( CMD_CHILD_VSHOWNORTHNUMBER, _("Number"), wxT( "" ), true );
			northmenu->Check( CMD_CHILD_VSHOWNORTHNUMBER, view->getVedicGraphicStyle().northIndianSignDisplayType == VGRAPHIC_NORTH_INDIAN_NUMBER );
			northmenu->Append( CMD_CHILD_VSHOWNORTHSHORT, _("Short String"), wxT( "" ), true );
			northmenu->Check( CMD_CHILD_VSHOWNORTHSHORT, view->getVedicGraphicStyle().northIndianSignDisplayType == VGRAPHIC_NORTH_INDIAN_SHORT );
			northmenu->Append( CMD_CHILD_VSHOWNORTHSYMBOL, _("Symbol"), wxT( "" ), true );
			northmenu->Check( CMD_CHILD_VSHOWNORTHSYMBOL, view->getVedicGraphicStyle().northIndianSignDisplayType == VGRAPHIC_NORTH_INDIAN_SYMBOL );
			menu->Append( -1, _( "Display Signs" ), northmenu );
		}
		if ( view->supportsFullGraphicStyleToggle())
		{
			MyMenu *centermenu = new MyMenu( _( "Chart Center" ), view );
			centermenu->Append( CMD_CHILD_VSHOWCENTERNOTHING, _("Nothing"), wxT( "" ), true );
			centermenu->Check( CMD_CHILD_VSHOWCENTERNOTHING, view->getVedicGraphicStyle().centerInfoType == VGRAPHIC_CHART_CENTER_NOTHING );
			centermenu->Append( CMD_CHILD_VSHOWCENTERVARGA, _("Varga"), wxT( "" ), true );
			centermenu->Check( CMD_CHILD_VSHOWCENTERVARGA, view->getVedicGraphicStyle().centerInfoType == VGRAPHIC_CHART_CENTER_VARGA );
			centermenu->Append( CMD_CHILD_VSHOWCENTERDIVISION, _("Division"), wxT( "" ), true );
			centermenu->Check( CMD_CHILD_VSHOWCENTERDIVISION, view->getVedicGraphicStyle().centerInfoType == VGRAPHIC_CHART_CENTER_DIVISION );
			centermenu->Append( CMD_CHILD_VSHOWCENTERBOTH, _("Both"), wxT( "" ), true );
			centermenu->Check( CMD_CHILD_VSHOWCENTERBOTH, view->getVedicGraphicStyle().centerInfoType == VGRAPHIC_CHART_CENTER_BOTH );
			menu->Append( -1, _( "Chart Center" ), centermenu );
		}
		menu->AppendSeparator();

		if ( view->supportsFullGraphicStyleToggle())
		{
			menu->Append( CMD_CHILD_VSHOWRETRO, _( "Retrograde Planets"), wxT( "" ), true );
			menu->Check( CMD_CHILD_VSHOWRETRO, view->getVedicGraphicStyle().showRetro );
			menu->Append( CMD_CHILD_VSHOWARUDHA, _( "Arudhas"), wxT( "" ), true );
			menu->Check( CMD_CHILD_VSHOWARUDHA, view->getVedicGraphicStyle().showArudhas );
		}
		menu->Append( CMD_CHILD_VSHOWPLANETCOLORS, _( "Colors for Planet Symbols"), wxT( "" ), true );
		menu->Check( CMD_CHILD_VSHOWPLANETCOLORS, view->getVedicGraphicStyle().showPlanetColors );
	}
	else
	{
		menu->Append( CMD_CHILD_WSHOWRETRO, _( "Show Retrograde Planets"), wxT( "" ), true );
		menu->Check( CMD_CHILD_WSHOWRETRO, view->getWesternGraphicStyle().showRetro );
		menu->Append( CMD_CHILD_WSHOWASPECTS, _( "Show Aspects"), wxT( "" ), true );
		menu->Check( CMD_CHILD_WSHOWASPECTS, view->getWesternGraphicStyle().showAspects );
		menu->Append( CMD_CHILD_WSHOWASPECTSYMBOLS, _( "Show Aspect Symbols"), wxT( "" ), true );
		menu->Check( CMD_CHILD_WSHOWASPECTSYMBOLS, view->getWesternGraphicStyle().showAspectSymbols );
		menu->Append( CMD_CHILD_WSHOWHOUSES, _( "Show Houses"), wxT( "" ), true );
		menu->Check( CMD_CHILD_WSHOWHOUSES, view->getWesternGraphicStyle().showHouses );

		if ( view->getViewId() == VIEW_TRANSIT )
		{
			menu->Append( CMD_CHILD_WTRANSIT_STYLE, _( "Transits Inside"), wxT( "" ), true );
			menu->Check( CMD_CHILD_WTRANSIT_STYLE, view->getWesternGraphicStyle().transitStyle );
		}

	}
	return menu;
}

/**************************************************************
***
**   ContextMenuProvider   ---   getSarvatobhadraMenu
***
***************************************************************/
wxMenu *ContextMenuProvider::getSarvatobhadraMenu()
{
	MyMenu *menu = new MyMenu( _( "Sarvatobhadra" ), view );
	menu->Append( CMD_CHILD_VSHOWRETRO, _( "Retrograde Planets"), wxT( "" ), true );
	menu->Check( CMD_CHILD_VSHOWRETRO, view->getVedicGraphicStyle().showRetro );
	menu->Append( CMD_CHILD_VSHOWPLANETCOLORS, _( "Colors for Planet Symbols"), wxT( "" ), true );
	menu->Check( CMD_CHILD_VSHOWPLANETCOLORS, view->getVedicGraphicStyle().showPlanetColors );
	menu->Append( CMD_CHILD_VSHOWSANSKRITSYMBOLS, _( "Sanskrit Symbols"), wxT( "" ), true );
	menu->Check( CMD_CHILD_VSHOWSANSKRITSYMBOLS, view->getVedicGraphicStyle().showSbcSanskritSymbols );
	menu->Append( CMD_CHILD_VSHOWAFFLICTIONS, _( "Afflictions"), wxT( "" ), true );
	menu->Check( CMD_CHILD_VSHOWAFFLICTIONS, view->getVedicGraphicStyle().showSbcAfflictions );
	menu->Append( CMD_CHILD_VSHOW_SBC_NAKSHATRA_QUALITY, _( "Benefic/Malefic Nakshatras"), wxT( "" ), true );
	menu->Check( CMD_CHILD_VSHOW_SBC_NAKSHATRA_QUALITY, view->getVedicGraphicStyle().showSbcNakshatraQuality );
	return menu;
}

/**************************************************************
***
**   ContextMenuProvider   ---   getAppWindowContextMenu
***
***************************************************************/
wxMenu *ContextMenuProvider::getAppWindowContextMenu()
{
	const int appmenuitems[] = { APP_FILENEW, APP_FILEOPEN, 0, APP_SHOWEPHEM,
		APP_SHOWECLIPSE, APP_SHOWHORA, APP_NEW_PARTNER, 0, -1 };

	MyMenu *menu = new MyMenu( _( "Main Window Menu" ), view );
	menu->addArray( appmenuitems );
	menu->addWindowListMenu();
	menu->addItem( APP_CONFIGURE );
	menu->addItem( APP_LOCATION );
	menu->AppendSeparator();
	menu->addItem( APP_FILEQUIT );
	return menu;
}

/**************************************************************
***
**   ContextMenuProvider   ---   getChartMenu
***
***************************************************************/
wxMenu *ContextMenuProvider::getChartMenu()
{
	MyMenu *menu = new MyMenu( _( "File" ), view );
	menu->addItem( CMD_CHILD_CLOSE );
	if ( view->getDoc() )
	{
		menu->addItem( CMD_CHILD_CLOSE_DOC );
		menu->addItem( CMD_CHILD_SAVE );
		menu->addItem( CMD_CHILD_SAVEAS );
		menu->addItem( CMD_CHILD_PRINT );
		menu->addItem( CMD_CHILD_QUICKPRINT );
	}
	return menu;
}

/**************************************************************
***
**   ContextMenuProvider   ---   getNewViewMenu
***
***************************************************************/
wxMenu *ContextMenuProvider::getNewViewMenu()
{
	const int vedicitems[] = { 0, CMD_CHILD_NEW_DASA, CMD_CHILD_NEW_GRAPHICALDASA,
		CMD_CHILD_NEW_TRANSIT, 0, CMD_CHILD_NEW_YOGA, CMD_CHILD_NEW_ASHTAKAVARGA,
		CMD_CHILD_NEW_SOLAR, -1 };
	const int westernitems[5] = { 0, CMD_CHILD_NEW_URANIAN, CMD_CHILD_NEW_TRANSIT, CMD_CHILD_NEW_SOLAR, -1 };
	const int appitems[5] = { APP_SHOWEPHEM, APP_SHOWECLIPSE, APP_SHOWHORA, APP_NEW_PARTNER, -1 };

	MyMenu *menu = new MyMenu( _( "New View" ), view );
	bool vedicmode = view->isVedic();
	if ( wxGetKeyState( WXK_SHIFT )) vedicmode = ! vedicmode;

	if ( view->getDoc())
	{
		// Varga submenu or western chart
		if ( vedicmode ) menu->addVargaMenu();
		else menu->addItem( CMD_CHILD_NEW_WCHART );

		if ( vedicmode )
		{
			menu->addItem( CMD_CHILD_NEW_VARGA );
			menu->addItem( CMD_CHILD_NEW_SBC );
		}
		menu->addItem( CMD_CHILD_NEW_TEXT );
		menu->addArray( vedicmode ? vedicitems : westernitems );
	}
	else
	{
		menu->addArray( appitems );
	}
	return menu;
}

