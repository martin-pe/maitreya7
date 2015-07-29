/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/base/IconProvider.cpp
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
#pragma implementation "IconProvider.h"
#endif

#include "IconProvider.h"

#include "constants.h"
#include "FileConfig.h"

#include <wx/filename.h>
#include <wx/hashmap.h>
#include <wx/image.h>
#include <wx/imaglist.h>
#include <wx/log.h>

#include "ashtakavarga.xpm"
#include "chart.xpm"
#include "chart2.xpm"
#include "chart3.xpm"
#include "chart4.xpm"
#include "chart6.xpm"
#include "chart7.xpm"
#include "chart8.xpm"
#include "chart9.xpm"
#include "chart10.xpm"
#include "chart12.xpm"
#include "chart16.xpm"
#include "chart20.xpm"
#include "chart24.xpm"
#include "chart27.xpm"
#include "chart30.xpm"
#include "chart40.xpm"
#include "chart45.xpm"
#include "chart60.xpm"
#include "chart108.xpm"
#include "chart144.xpm"
#include "chartb.xpm"
#include "close.xpm"
#include "config.xpm"
#include "copy.xpm"
#include "dasa.xpm"
#include "dasa_folder.xpm"
#include "dasa_folder_e.xpm"
#include "dasatree1.xpm"
#include "dasatree1e.xpm"
#include "dasatree2.xpm"
#include "dasatree2e.xpm"
#include "dasatree3.xpm"
#include "dasatree3e.xpm"
#include "dasatree4.xpm"
#include "dasatree4e.xpm"
#include "dasatree5.xpm"
#include "dasatree5e.xpm"
#include "eclipse.xpm"
#include "edit.xpm"
#include "ephem.xpm"
#include "goto.xpm"
#include "graphicaldasa.xpm"
#include "help.xpm"
#include "hora.xpm"
#include "info.xpm"
#include "mahadasa.xpm"
#include "mahadasa_e.xpm"
#include "maitreya.xpm"
#include "mview.xpm"

#include "nav_down.xpm"
#include "nav_left.xpm"
#include "nav_right.xpm"
#include "nav_up.xpm"

#include "new.xpm"
#include "now.xpm"
#include "open.xpm"
#include "partner.xpm"
#include "print.xpm"
#include "quickprint.xpm"
#include "quit.xpm"
#include "save.xpm"
#include "saveas.xpm"
#include "sbc.xpm"
#include "separator.xpm"
#include "shadbala.xpm"
#include "solar.xpm"
#include "sview.xpm"
#include "text.xpm"
#include "transit.xpm"
#include "uranian.xpm"
#include "varga.xpm"
#include "wchart.xpm"
#include "world.xpm"
#include "yoga.xpm"
#include "yogaeditor.xpm"

WX_DECLARE_STRING_HASH_MAP( wxBitmap*, ImageHashMap );

/*****************************************************
**
**   IconProvider   ---   getIcon
**
******************************************************/
wxIcon IconProvider::getIcon( const int &index )
{
#ifdef __WXMSW__
	switch ( index )
	{
	case BITMAP_MAITREYA:
		return wxIcon( wxT( "aa_maitreya_icon" ), wxBITMAP_TYPE_ICO_RESOURCE );
		break;
	case BITMAP_MVIEW:
		return wxIcon( wxT( "mview_icon" ), wxBITMAP_TYPE_ICO_RESOURCE );
		break;
	case BITMAP_SVIEW:
		return wxIcon( wxT( "sview_icon" ), wxBITMAP_TYPE_ICO_RESOURCE );
		break;
	case BITMAP_TEXT:
		return wxIcon( wxT( "text_icon" ), wxBITMAP_TYPE_ICO_RESOURCE );
		break;
	case BITMAP_CHART:
		return wxIcon( wxT( "chart_icon" ), wxBITMAP_TYPE_ICO_RESOURCE );
		break;
	case BITMAP_CHART9:
		return wxIcon( wxT( "chart9_icon" ), wxBITMAP_TYPE_ICO_RESOURCE );
		break;
	case BITMAP_WCHART:
		return wxIcon( wxT( "wchart_icon" ), wxBITMAP_TYPE_ICO_RESOURCE );
		break;
	case BITMAP_DASA:
		return wxIcon( wxT( "dasa_icon" ), wxBITMAP_TYPE_ICO_RESOURCE );
		break;
	case BITMAP_HORA:
		return wxIcon( wxT( "hora_icon" ), wxBITMAP_TYPE_ICO_RESOURCE );
		break;
	case BITMAP_URANIAN:
		return wxIcon( wxT( "uranian_icon" ), wxBITMAP_TYPE_ICO_RESOURCE );
		break;
	case BITMAP_TRANSIT:
		return wxIcon( wxT( "transit_icon" ), wxBITMAP_TYPE_ICO_RESOURCE );
		break;
	case BITMAP_EPHEM:
		return wxIcon( wxT( "ephem_icon" ), wxBITMAP_TYPE_ICO_RESOURCE );
		break;
	case BITMAP_GRAPHICALDASA:
		return wxIcon( wxT( "graphicaldasa_icon" ), wxBITMAP_TYPE_ICO_RESOURCE );
		break;
	case BITMAP_ECLIPSE:
		return wxIcon( wxT( "eclipse_icon" ), wxBITMAP_TYPE_ICO_RESOURCE );
		break;
	case BITMAP_PARTNER:
		return wxIcon( wxT( "partner_icon" ), wxBITMAP_TYPE_ICO_RESOURCE );
		break;
	case BITMAP_SBC:
		return wxIcon( wxT( "sbc_icon" ), wxBITMAP_TYPE_ICO_RESOURCE );
		break;
	case BITMAP_SOLAR:
		return wxIcon( wxT( "solar_icon" ), wxBITMAP_TYPE_ICO_RESOURCE );
		break;
	case BITMAP_WORLD:
		return wxIcon( wxT( "world_icon" ), wxBITMAP_TYPE_ICO_RESOURCE );
		break;
	case BITMAP_EDIT:
		return wxIcon( wxT( "edit_icon" ), wxBITMAP_TYPE_ICO_RESOURCE );
		break;
	case BITMAP_CONFIG:
		return wxIcon( wxT( "config_icon" ), wxBITMAP_TYPE_ICO_RESOURCE );
		break;
	case BITMAP_YOGA:
		return wxIcon( wxT( "yoga_icon" ), wxBITMAP_TYPE_ICO_RESOURCE );
		break;
	case BITMAP_VARGA:
		return wxIcon( wxT( "varga_icon" ), wxBITMAP_TYPE_ICO_RESOURCE );
		break;
	case BITMAP_ASHTAKAVARGA:
		return wxIcon( wxT( "ashtakavarga_icon" ), wxBITMAP_TYPE_ICO_RESOURCE );
		break;
	case BITMAP_SHADBALA:
		return wxIcon( wxT( "shadbala_icon" ), wxBITMAP_TYPE_ICO_RESOURCE );
		break;
	case BITMAP_YOGAEDITOR:
		return wxIcon( wxT( "yogaeditor_icon" ), wxBITMAP_TYPE_ICO_RESOURCE );
		break;
	default:
		return wxIcon( wxT( "aa_maitreya_icon" ), wxBITMAP_TYPE_ICO_RESOURCE );
		break;
	}
#else
	switch ( index )
	{
	case BITMAP_MAITREYA:
		return wxIcon( maitreya_xpm );
		break;
	case BITMAP_MVIEW:
		return wxIcon( mview_xpm );
		break;
	case BITMAP_SVIEW:
		return wxIcon( sview_xpm );
		break;
	case BITMAP_TEXT:
		return wxIcon( text_xpm );
		break;
	case BITMAP_CHART:
		return wxIcon( chart_xpm );
		break;
	case BITMAP_CHART9:
		return wxIcon( chart9_xpm );
		break;
	case BITMAP_WCHART:
		return wxIcon( wchart_xpm );
		break;
	case BITMAP_DASA:
		return wxIcon( dasa_xpm );
		break;
	case BITMAP_HORA:
		return wxIcon( hora_xpm );
		break;
	case BITMAP_TRANSIT:
		return wxIcon( transit_xpm );
		break;
	case BITMAP_URANIAN:
		return wxIcon( uranian_xpm );
		break;
	case BITMAP_EPHEM:
		return wxIcon( ephem_xpm );
		break;
	case BITMAP_GRAPHICALDASA:
		return wxIcon( graphicaldasa_xpm );
		break;
	case BITMAP_ECLIPSE:
		return wxIcon( eclipse_xpm );
		break;
	case BITMAP_PARTNER:
		return wxIcon( partner_xpm );
		break;
	case BITMAP_SBC:
		return wxIcon( sbc_xpm );
		break;
	case BITMAP_SOLAR:
		return wxIcon( solar_xpm );
		break;
	case BITMAP_WORLD:
		return wxIcon( world_xpm );
		break;
	case BITMAP_EDIT:
		return wxIcon( edit_xpm );
		break;
	case BITMAP_CONFIG:
		return wxIcon( config_xpm );
		break;
	case BITMAP_YOGA:
		return wxIcon( yoga_xpm );
		break;
	case BITMAP_ASHTAKAVARGA:
		return wxIcon( ashtakavarga_xpm );
		break;
	case BITMAP_VARGA:
		return wxIcon( varga_xpm );
		break;
	case BITMAP_SHADBALA:
		return wxIcon( shadbala_xpm );
		break;
	case BITMAP_YOGAEDITOR:
		return wxIcon( yogaeditor_xpm );
		break;
	default:
		wxLogError( wxT( "Cannot find bitmap #%d, using default" ), index );
		return wxIcon( maitreya_xpm );
		break;
	}
#endif
}

/*****************************************************
**
**   IconProvider   ---   IconProvider::getScaledBitmap
**
******************************************************/
wxBitmap IconProvider::getScaledBitmap( const int &index, const int &size )
{
	return wxBitmap( getBitmap( index ).ConvertToImage().Scale( size, size ));
}

/*****************************************************
**
**   IconProvider   ---   getMenuBitmap
**
******************************************************/
wxBitmap IconProvider::getMenuBitmap( const int &index )
{
	return getScaledBitmap( index, 16 );
}

/*****************************************************
**
**   IconProvider   ---   getImage
**
******************************************************/
wxImage IconProvider::getImage( const wxString &s )
{
	return getBitmap( s )->ConvertToImage();
}

/*****************************************************
**
**   IconProvider   ---   getBitmap
**
******************************************************/
wxBitmap *IconProvider::getBitmap( const wxString &s )
{
	static ImageHashMap map;
	if ( map[s] != 0 )
	{
		return map[s];
	}
	else
	{
		wxString filename =  FileConfig::get()->getPicDir() + s;
		if ( ! wxFileName::IsFileReadable( filename ))
		{
			printf( "ERROR: bitmap %s not found in directory %s\n", str2char( s ), str2char( FileConfig::get()->getPicDir()));
			return &wxNullBitmap;
		}
		wxBitmap *bmp = new wxBitmap( FileConfig::get()->getPicDir() + s, wxBITMAP_TYPE_ANY );
		//bmp->SetName( s );
		map[s] = bmp;
		return bmp;
	}
}

/*****************************************************
**
**   IconProvider   ---   getBitmap
**
******************************************************/
wxBitmap IconProvider::getBitmap( const int &index )
{
#define ASSIGN_BITMAP( i, b ) case i: return wxBitmap( b ); break;
	switch ( index )
	{
		ASSIGN_BITMAP( BITMAP_NEW, new_xpm )

		// navigation
		ASSIGN_BITMAP( BITMAP_DOWN, nav_down_xpm )
		ASSIGN_BITMAP( BITMAP_LEFT, nav_left_xpm )
		ASSIGN_BITMAP( BITMAP_RIGHT, nav_right_xpm )
		ASSIGN_BITMAP( BITMAP_UP, nav_up_xpm )

	case BITMAP_OPEN:
		return wxBitmap( open_xpm );
		break;
	case BITMAP_SAVE:
		return wxBitmap( save_xpm );
		break;
	case BITMAP_SAVEAS:
		return wxBitmap( saveas_xpm );
		break;
	case BITMAP_CLOSE:
		return wxBitmap( close_xpm );
		break;
	case BITMAP_QUIT:
		return wxBitmap( quit_xpm );
		break;
	case BITMAP_NOW:
		return wxBitmap( now_xpm );
		break;
	case BITMAP_GOTO:
		return wxBitmap( goto_xpm );
		break;
	case BITMAP_TEXT:
		return wxBitmap( text_xpm );
		break;
	case BITMAP_HELP:
		return wxBitmap( help_xpm );
		break;
	case BITMAP_HORA:
		return wxBitmap( hora_xpm );
		break;
	case BITMAP_MAITREYA:
		return wxBitmap( maitreya_xpm );
		break;
	case BITMAP_EDIT:
		return wxBitmap( edit_xpm );
		break;
	case BITMAP_CHART:
		return wxBitmap( chart_xpm );
		break;
	case BITMAP_CHART2:
		return wxBitmap( chart2_xpm );
		break;
	case BITMAP_CHART3:
		return wxBitmap( chart3_xpm );
		break;
	case BITMAP_CHART4:
		return wxBitmap( chart4_xpm );
		break;
	case BITMAP_CHART6:
		return wxBitmap( chart6_xpm );
		break;
	case BITMAP_CHART7:
		return wxBitmap( chart7_xpm );
		break;
	case BITMAP_CHART8:
		return wxBitmap( chart8_xpm );
		break;
	case BITMAP_CHART9:
		return wxBitmap( chart9_xpm );
		break;
	case BITMAP_CHART10:
		return wxBitmap( chart10_xpm );
		break;
	case BITMAP_CHART12:
		return wxBitmap( chart12_xpm );
		break;
	case BITMAP_CHART16:
		return wxBitmap( chart16_xpm );
		break;
	case BITMAP_CHART20:
		return wxBitmap( chart20_xpm );
		break;
	case BITMAP_CHART24:
		return wxBitmap( chart24_xpm );
		break;
	case BITMAP_CHART27:
		return wxBitmap( chart27_xpm );
		break;
	case BITMAP_CHART30:
		return wxBitmap( chart30_xpm );
		break;
	case BITMAP_CHART40:
		return wxBitmap( chart40_xpm );
		break;
	case BITMAP_CHART45:
		return wxBitmap( chart45_xpm );
		break;
	case BITMAP_CHART60:
		return wxBitmap( chart60_xpm );
		break;
	case BITMAP_CHART108:
		return wxBitmap( chart108_xpm );
		break;
	case BITMAP_CHART144:
		return wxBitmap( chart144_xpm );
		break;
	case BITMAP_CHARTB:
		return wxBitmap( chartb_xpm );
		break;
	case BITMAP_CONFIG:
		return wxBitmap( config_xpm );
		break;
	case BITMAP_PRINT:
		return wxBitmap( print_xpm );
		break;
	case BITMAP_QUICKPRINT:
		return wxBitmap( quickprint_xpm );
		break;
	case BITMAP_TRANSIT:
		return wxBitmap( transit_xpm );
		break;
	case BITMAP_URANIAN:
		return wxBitmap( uranian_xpm );
		break;
	case BITMAP_PARTNER:
		return wxBitmap( partner_xpm );
		break;
	case BITMAP_WCHART:
		return wxBitmap( wchart_xpm );
		break;
	case BITMAP_ECLIPSE:
		return wxBitmap( eclipse_xpm );
		break;
	case BITMAP_EPHEM:
		return wxBitmap( ephem_xpm );
		break;
	case BITMAP_ASHTAKAVARGA:
		return wxBitmap( ashtakavarga_xpm );
		break;
	case BITMAP_VARGA:
		return wxBitmap( varga_xpm );
		break;
	case BITMAP_GRAPHICALDASA:
		return wxBitmap( graphicaldasa_xpm );
		break;
	case BITMAP_DASA:
		return wxBitmap( dasa_xpm );
		break;
	case BITMAP_SBC:
		return wxBitmap( sbc_xpm );
		break;
	case BITMAP_SHADBALA:
		return wxBitmap( shadbala_xpm );
		break;
	case BITMAP_SOLAR:
		return wxBitmap( solar_xpm );
		break;
	case BITMAP_WORLD:
		return wxBitmap( world_xpm );
		break;
	case BITMAP_ABOUT:
		return wxBitmap( info_xpm );
		break;
	case BITMAP_MVIEW:
		return wxBitmap( mview_xpm );
		break;
	case BITMAP_SVIEW:
		return wxBitmap( sview_xpm );
		break;
	case BITMAP_DASA_FOLDER:
		return wxBitmap( dasa_folder_xpm );
		break;
	case BITMAP_MAHADASA:
		return wxBitmap( mahadasa_xpm );
		break;
	case BITMAP_DASATREE1:
		return wxBitmap( dasatree1_xpm );
		break;
	case BITMAP_DASATREE2:
		return wxBitmap( dasatree2_xpm );
		break;
	case BITMAP_DASATREE3:
		return wxBitmap( dasatree3_xpm );
		break;
	case BITMAP_DASATREE4:
		return wxBitmap( dasatree4_xpm );
		break;
	case BITMAP_DASATREE5:
		return wxBitmap( dasatree5_xpm );
		break;
	case BITMAP_DASA_FOLDERE:
		return wxBitmap( dasa_folder_e_xpm );
		break;
	case BITMAP_MAHADASAE:
		return wxBitmap( mahadasa_e_xpm );
		break;
	case BITMAP_DASATREE1E:
		return wxBitmap( dasatree1e_xpm );
		break;
	case BITMAP_DASATREE2E:
		return wxBitmap( dasatree2e_xpm );
		break;
	case BITMAP_DASATREE3E:
		return wxBitmap( dasatree3e_xpm );
		break;
	case BITMAP_DASATREE4E:
		return wxBitmap( dasatree4e_xpm );
		break;
	case BITMAP_DASATREE5E:
		return wxBitmap( dasatree5e_xpm );
		break;
	case BITMAP_SEPARATOR:
		return wxBitmap( separator_xpm );
		break;
	case BITMAP_YOGA:
		return wxBitmap( yoga_xpm );
		break;
	case BITMAP_COPY:
		return wxBitmap( copy_xpm );
		break;
	case BITMAP_YOGAEDITOR:
		return wxBitmap( yogaeditor_xpm );
		break;
	default:
		wxLogError( wxT( "Cannot find bitmap #%d, using default" ), index );
		return wxBitmap( maitreya_xpm );
		break;
	}
#undef ASSIGN_BITMAP
}

/**************************************************************
***
**   IconProvider   ---   createImageList
***
***************************************************************/
wxImageList *IconProvider::createAppWindowImageList( const bool &smallList )
{
	const int max_image = 21;
	const int imageIdList[max_image] = { BITMAP_HORA, BITMAP_SVIEW, BITMAP_MVIEW, BITMAP_DASA, BITMAP_TRANSIT,
	                                     BITMAP_EPHEM, BITMAP_URANIAN, BITMAP_CHART, BITMAP_CHART9, BITMAP_WCHART, BITMAP_PARTNER,
	                                     BITMAP_SBC, BITMAP_GRAPHICALDASA, BITMAP_ECLIPSE, BITMAP_YOGA, BITMAP_SOLAR, BITMAP_VARGA,
	                                     BITMAP_SHADBALA, BITMAP_ASHTAKAVARGA, BITMAP_YOGAEDITOR, BITMAP_TEXT
	                                   };
	wxImageList *imagelist;

	if ( smallList ) imagelist = new wxImageList( 16, 16, true );
	else imagelist = new wxImageList( 24, 24, true );

	for ( int i = 0; i < max_image; i++ )
	{
		if ( smallList ) imagelist->Add( getMenuBitmap( imageIdList[i] ));
		else imagelist->Add( getBitmap( imageIdList[i] ));
	}
	return imagelist;
}

/**************************************************************
***
**   IconProvider   ---   createDasaImageList
***
***************************************************************/
wxImageList *IconProvider::createDasaImageList()
{
	const int max_image = 15;
	const int imageIdList[max_image] = { BITMAP_DASA, BITMAP_DASA_FOLDER, BITMAP_MAHADASA, BITMAP_DASATREE1,
	                                     BITMAP_DASATREE2, BITMAP_DASATREE3, BITMAP_DASATREE4, BITMAP_DASATREE5, BITMAP_DASA_FOLDERE,
	                                     BITMAP_MAHADASAE, BITMAP_DASATREE1E, BITMAP_DASATREE2E, BITMAP_DASATREE3E, BITMAP_DASATREE4E,
	                                     BITMAP_DASATREE5E
	                                   };
	wxImageList *imagelist;

	imagelist = new wxImageList( 24, 24, true );

	for ( int i = 0; i < max_image; i++ )
	{
		imagelist->Add( getBitmap( imageIdList[i] ));
	}
	return imagelist;
}

/*****************************************************
**
**   IconProvider   ---   getWindowIcon
**
******************************************************/
wxIcon IconProvider::getWindowIcon( const int id )
{
	switch ( id )
	{
	case VIEW_TEXT:
		return getIcon( BITMAP_TEXT );
		break;
	case VIEW_TRANSIT:
		return getIcon( BITMAP_TRANSIT );
		break;
	case VIEW_SBC:
		return getIcon( BITMAP_SBC );
		break;
	case VIEW_SOLAR:
		return getIcon( BITMAP_SOLAR );
		break;
	case VIEW_DASA:
		return getIcon( BITMAP_DASA );
		break;
	case VIEW_GRAPHICALDASA:
		return getIcon( BITMAP_GRAPHICALDASA );
		break;
	case VIEW_URANIAN:
		return getIcon( BITMAP_URANIAN );
		break;
	case VIEW_GRAPHIC:
		return getIcon( BITMAP_CHART );
		break;
	case VIEW_PLANETLIST:
		return getIcon( BITMAP_SVIEW );
		break;
	case VIEW_HORA:
		return getIcon( BITMAP_HORA );
		break;
	case VIEW_ECLIPSE:
		return getIcon( BITMAP_ECLIPSE );
		break;
	case VIEW_PARTNER:
		return getIcon( BITMAP_PARTNER );
		break;
	case VIEW_EPHEM:
		return getIcon( BITMAP_EPHEM );
		break;
	case VIEW_YOGA:
		return getIcon( BITMAP_YOGA );
		break;
	case VIEW_VARGA:
		return getIcon( BITMAP_VARGA );
		break;
	case VIEW_ASHTAKAVARGA:
		return getIcon( BITMAP_ASHTAKAVARGA );
		break;
	case VIEW_SHADBALA:
		return getIcon( BITMAP_SHADBALA );
		break;
	case VIEW_YOGA_EDITOR:
		return getIcon( BITMAP_YOGAEDITOR );
		break;
	case VIEW_MULTIPLE:
		return getIcon( BITMAP_MVIEW );
		break;
	default:
		return getIcon( BITMAP_HORA );
		break;
	}
}




