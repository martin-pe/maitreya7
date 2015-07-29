/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/base/IconProvider.h
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

#ifndef _ICONPROVIDER_H_
#define _ICONPROVIDER_H_

#ifdef __GNUG__
#pragma interface "IconProvider.h"
#endif

#include <wx/bitmap.h>
#include <wx/icon.h>

class wxImageList;

enum { BITMAP_NEW = 1, BITMAP_OPEN, BITMAP_SAVE, BITMAP_SAVEAS, BITMAP_TEXT, BITMAP_HORA,
       BITMAP_DOCUMENT, BITMAP_MAITREYA, BITMAP_EDIT,
       BITMAP_CHART,
       BITMAP_CHART2,
       BITMAP_CHART3,
       BITMAP_CHART4,
       BITMAP_CHART6,
       BITMAP_CHART7,
       BITMAP_CHART8,
       BITMAP_CHART9,
       BITMAP_CHART10,
       BITMAP_CHART12,
       BITMAP_CHART16,
       BITMAP_CHART20,
       BITMAP_CHART24,
       BITMAP_CHART27,
       BITMAP_CHART30,
       BITMAP_CHART40,
       BITMAP_CHART45,
       BITMAP_CHART60,
       BITMAP_CHART108,
       BITMAP_CHART144,
       BITMAP_CHARTB,

       BITMAP_MVIEW, BITMAP_SVIEW,
       BITMAP_CONFIG, BITMAP_QUIT, BITMAP_PRINT,
       BITMAP_NOW, BITMAP_URANIAN, BITMAP_TRANSIT, BITMAP_PARTNER, BITMAP_WCHART, BITMAP_SOLAR,
       BITMAP_CLOSE, BITMAP_EPHEM, BITMAP_DASA, BITMAP_WORLD, BITMAP_LOGO, BITMAP_SBC,
       BITMAP_ECLIPSE, BITMAP_ABOUT,
       BITMAP_COPY, BITMAP_YOGAEDITOR,
       BITMAP_DASA_FOLDER, BITMAP_MAHADASA, BITMAP_GRAPHICALDASA,
       BITMAP_DASATREE1, BITMAP_DASATREE2, BITMAP_DASATREE3, BITMAP_DASATREE4, BITMAP_DASATREE5,
       BITMAP_DASA_FOLDERE, BITMAP_MAHADASAE, BITMAP_GOTO, BITMAP_QUICKPRINT, BITMAP_DASATREE1E,
       BITMAP_DASATREE2E, BITMAP_DASATREE3E, BITMAP_DASATREE4E, BITMAP_DASATREE5E, BITMAP_HELP,
       BITMAP_YOGA, BITMAP_ASHTAKAVARGA, BITMAP_VARGA, BITMAP_SHADBALA,
       BITMAP_RIGHT, BITMAP_LEFT, BITMAP_UP, BITMAP_DOWN, BITMAP_SEPARATOR
     };

/*************************************************//**
*
* \brief encapsulates access to pictures and icons
*
******************************************************/
class IconProvider
{
public:
	static wxBitmap *getBitmap( const wxString& );
	static wxBitmap getBitmap( const int& );

	static wxImage getImage( const wxString &s );

	static wxBitmap getScaledBitmap( const int &index, const int &size );
	static wxBitmap getMenuBitmap( const int& );

	static wxIcon getIcon( const int & );
	static wxIcon getWindowIcon( const int );

	static wxImageList *createAppWindowImageList( const bool& );
	static wxImageList *createDasaImageList();

};

#endif


