/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/base/FontConfig.h
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

#ifndef _FONTCONFIG_H_
#define _FONTCONFIG_H_

#ifdef __GNUG__
#pragma interface "FontConfig.h"
#endif

#include <wx/font.h>
#include <wx/string.h>

#define MAX_FONTS 9
enum { FONT_DEFAULT = 0, FONT_HEADER, FONT_GRAPHIC, FONT_TEXT, FONT_SYMBOL,
	FONT_PDFDEFAULT, FONT_PDFHEADER, FONT_PDFGRAPHIC, FONT_PDFSYMBOL };

/*************************************************//**
*
* \brief encapsulates the needed parameters of a font
*
******************************************************/
class FontDescription
{
public:
	FontDescription();
	bool isEmpty();
	wxString toString();

	int pointSize, family, style, weight, underline;
	wxString facename;
};

/*************************************************//**
*
* \brief Provides access to fonts, singleton
*
******************************************************/
class FontConfig
{
public:

	/**
	 * \brief static access method, constructor is private
	 */
	static FontConfig *get();

	wxFont *getDefaultFont( const int zoom = 100 ) { return getFont( FONT_DEFAULT, zoom ); }
	wxFont *getHeaderFont( const int zoom = 100 ) { return getFont( FONT_HEADER, zoom ); }
	wxFont *getTextFont( const int zoom = 100 ) { return getFont( FONT_TEXT, zoom ); }
	wxFont *getGraphicFont( const int zoom = 100 ) { return getFont( FONT_GRAPHIC, zoom ); }
	wxFont *getSymbolFont( const int zoom = 100, const int tempFontSize = -1 ) { return getFont( FONT_SYMBOL, zoom, tempFontSize ); }

	wxFont *getPdfDefaultFont( const int zoom = 100 ) { return getFont( FONT_PDFDEFAULT, zoom ); }
	wxFont *getPdfHeaderFont( const int zoom = 100 ) { return getFont( FONT_PDFHEADER, zoom ); }
	wxFont *getPdfGraphicFont( const int zoom = 100 ) { return getFont( FONT_PDFGRAPHIC, zoom ); }
	wxFont *getPdfSymbolFont( const int zoom = 100, const int tempFontSize = -1 ) { return getFont( FONT_PDFSYMBOL, zoom, tempFontSize ); }

	/**
	 * \brief called by config FontPanel
	 */
	void setFont( const int &i, const wxFont& );

	/**
	 * \brief returns the default font if no config entry is set or default button in FontPanel is pressed
	 */
	wxFont *getFallbackFont( const int &f, const int zoom = 100 );

	/**
	 * called when configuration is stored
	 */
	void writeConfigEntries();

	wxString getSymbolFontName();

private:

	FontConfig();
	static FontConfig *ego;

	/**
	 * \brief init font description for a single font from a given config string
	 */
	void initFontDescription( const int &, const wxString& );

	/**
	 * \brief currently set font descriptions
	 */
	FontDescription fd[MAX_FONTS];

	/**
	 * \brief system default font descriptions
	 */
	FontDescription defaultfd[MAX_FONTS];

	int getSymbolFontSize( const int &scale );

	wxFont fbFont[MAX_FONTS];

	/**
	 * \brief returns pointer to the desired font
	 */
	wxFont *getFont( const int&, const int = 100, const int tempFontSize = -1 );
};

#endif


