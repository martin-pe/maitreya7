/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/PdfBase.cpp
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
#pragma implementation "PdfBase.h"
#endif

#include "PdfBase.h"

#include "Conf.h"
#include "config.h"
#include "FontConfig.h"

extern Config *config;

#define PDF_UNIT wxT( "mm" )
#define PDF_TINYFONT_SCALINGFACTOR .6
#define PDF_SYMBOLFONT_SCALINGFACTOR 4

/*****************************************************
**
**   BasePdfDocument   ---   Constructor
**
******************************************************/
BasePdfDocument::BasePdfDocument()
 : wxPdfDocument( wxPORTRAIT, PDF_UNIT, (wxPaperSize)config->printPaperFormat )
{
	setDefaultFont();
}

/*****************************************************
**
**   BasePdfDocument   ---   Footer
**
******************************************************/
void BasePdfDocument::Footer()
{
	wxString s;
	setTinyFont();
	if ( PageNo() == 1 )
	{
		if ( config->printCustomFooter )
		{
			wxString s = config->printCustomFooterText;
			s.Replace( wxT( "$date" ), wxDateTime().Now().FormatDate());
			s.Replace( wxT( "$version" ), wxConvertMB2WX( VERSION ));
			SetY( -20 );
			Cell( 0, 10, s, 0, 0, wxPDF_ALIGN_CENTER );
		}
	}
	else
	{
		SetY(-15);
		s << _( "Page" ) << wxT( " " ) << PageNo();
		Cell( 0, 10, s, 0, 0, wxPDF_ALIGN_CENTER );

	}
	setDefaultFont();
}

/*****************************************************
**
**   BasePdfDocument   ---   setSymbolFont
**
******************************************************/
void BasePdfDocument::setSymbolFont( const int zoom )
{
	wxFont symbolFont = *FontConfig::get()->getPdfSymbolFont( PDF_SYMBOLFONT_SCALINGFACTOR * zoom, config->symbolFontSize );
	SetFont( symbolFont );
}

/*****************************************************
**
**   BasePdfDocument   ---   setDefaultFont
**
******************************************************/
void BasePdfDocument::setDefaultFont()
{
	SetFont( *FontConfig::get()->getPdfDefaultFont());
}

/*****************************************************
**
**   BasePdfDocument   ---   setGraphicFont
**
******************************************************/
void BasePdfDocument::setGraphicFont()
{
	SetFont( *FontConfig::get()->getPdfGraphicFont());
}

/*****************************************************
**
**   BasePdfDocument   ---   setDefaultBoldFont
**
******************************************************/
void BasePdfDocument::setDefaultBoldFont()
{
	SetFont( *FontConfig::get()->getPdfHeaderFont());
}

/*****************************************************
**
**   BasePdfDocument   ---   setHeader1Font
**
******************************************************/
void BasePdfDocument::setHeader1Font()
{
	SetFont( *FontConfig::get()->getPdfHeaderFont());
}

/*****************************************************
**
**   BasePdfDocument   ---   setHeader2Font
**
******************************************************/
void BasePdfDocument::setHeader2Font()
{
	SetFont( *FontConfig::get()->getPdfHeaderFont());
}

/*****************************************************
**
**   BasePdfDocument   ---   setHeader3Font
**
******************************************************/
void BasePdfDocument::setHeader3Font()
{
	SetFont( *FontConfig::get()->getPdfHeaderFont());
}

/*****************************************************
**
**   BasePdfDocument   ---   setTinyFont
**
******************************************************/
void BasePdfDocument::setTinyFont()
{
	wxFont tinyFont = *FontConfig::get()->getPdfDefaultFont();
	tinyFont.SetPointSize( PDF_TINYFONT_SCALINGFACTOR * tinyFont.GetPointSize() );
	SetFont( tinyFont );
}


