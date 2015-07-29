/**************************************************************************:.SETp

 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/Painter.cpp
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
#pragma implementation "Painter.h"
#endif

#include "Painter.h"

#include "ColorConfig.h"
#include "Conf.h"
#include "FontConfig.h"
#include "IconProvider.h"
#include "Lang.h"
#include "mathbase.h"
#include "FileConfig.h"

#include <wx/dc.h>
#include <wx/filename.h>

extern Config *config;

/*****************************************************
**
**   MyPoint   ---   toWxPoint
**
******************************************************/
wxPoint MyPoint::toWxPoint()
{
  return wxPoint( a_rund( x ), a_rund( y ));
}

/*****************************************************
**
**   MyRect   ---   toString
**
******************************************************/
wxString MyRect::toString()
{
	return wxString::Format( wxT( "x %f y %f w %f h %f" ), x, y, width, height );
}

/*****************************************************
**
**   Painter   ---   setTransparentBrush
**
******************************************************/
void Painter::setTransparentBrush()
{
	setBrush( *wxTRANSPARENT_BRUSH );
}

/*****************************************************
**
**   Painter   ---   setTransparentPen
**
******************************************************/
void Painter::setTransparentPen()
{
	setPen( *wxTRANSPARENT_PEN );
}

/*****************************************************
**
**   Painter   ---   drawRectangle
**
******************************************************/
void Painter::drawRectangle( const MyRect &r, const double rnd )
{
	drawRectangle( r.x, r.y, r.width, r.height, rnd );
}

/*****************************************************
**
**   Painter   ---   setDefaultTextColor
**
******************************************************/
void Painter::setDefaultTextColor()
{
	setTextColor( getDefaultTextColor());
}

/*****************************************************
**
**   Painter   ---   drawLine
**
******************************************************/
void Painter::drawLine( const MyPoint &p1, const MyPoint &p2 )
{
	drawLine( p1.x, p1.y, p2.x, p2.y );
}

/*****************************************************
**
**   Painter   ---   drawSimpleText
**
******************************************************/
void Painter::drawSimpleText( const MyRect &r, const wxString &s )
{
	drawSimpleText( r.x, r.y, r.width, r.height, s );
}

/*****************************************************
**
**   Painter   ---   drawSimpleText
**
******************************************************/
void Painter::drawSimpleText( const double &x, const double &y, const double &w, const double &h, const wxString &t )
{
	drawTextFormatted( x, y, w, h, t, Align::Center );
}

/*****************************************************
**
**   Painter   ---   drawTextFormatted
**
******************************************************/
void Painter::drawTextFormatted( const MyRect &r, const wxString &t, const int& align )
{
	drawTextFormatted( r.x, r.y, r.width, r.height, t, align );
}

/*****************************************************
**
**   GraphicalDasaPaintHelper   ---   drawDasaBar
**
******************************************************/
void GraphicalDasaPaintHelper::drawDasaBar( const int &dasalord, const int &isRasiLord, const DasaColorConfig *cfg,
	const int &x1, const int &y1, const int &w, const int &h, const int &cornerRadius )
{
	int format = TLARGE;
	wxString s;
	Lang lang;

	painter->setPen( wxPen( cfg->fgColor, cfg->fgWidth, cfg->fgStyle ));

	painter->setTextColor( cfg->fgColor );
	if ( cfg->bgStyle == wxSTIPPLE ) painter->setBrush( *IconProvider::getBitmap( cfg->bgImage ));
	else painter->setBrush( wxBrush ( cfg->bgColor, cfg->bgStyle ));

	painter->drawRectangle( x1, y1, w, h, cornerRadius );

	if ( w < 80 ) format = TMEDIUM;
	if ( isRasiLord )
	{
		assert( dasalord >= 0 && dasalord < 12 );
		if ( config->useSignSymbols )
		{
			painter->setSymbolFont();
			s = lang.getSignSymbolCode( dasalord );
		}
		else
		{
			painter->setGraphicFont();
			s = lang.getSignName( dasalord, format );
		}
	}
	else // Lord is a planet
	{
		if ( config->usePlanetSymbols )
		{
			painter->setSymbolFont();
			s = lang.getPlanetSymbolCode( dasalord );
		}
		else
		{
			painter->setGraphicFont();
			s = lang.getObjectName( dasalord, format, true );
		}
	}
	painter->drawTextFormatted( x1, y1, w, h,	s, Align::Center );
}

/*****************************************************
**
**   GraphicalDasaPaintHelper   ---   setDasaColors
**
******************************************************/
void GraphicalDasaPaintHelper::setDasaColors( const int &object, const int &rasitype )
{
	setDasaColors( config->colors->getDasaColorConfigId( object, rasitype ));
}

/*****************************************************
**
**   GraphicalDasaPaintHelper   ---   setDasaColors
**
******************************************************/
void GraphicalDasaPaintHelper::setDasaColors( const int &objectid )
{
	DasaColorConfig &dasa = config->colors->dasa[ objectid];

	painter->setPen( wxPen( dasa.fgColor, dasa.fgWidth ));

	if ( dasa.bgStyle == wxSTIPPLE ) painter->setBrush( *IconProvider::getBitmap( dasa.bgImage ));
	else painter->setBrush( wxBrush ( dasa.bgColor, dasa.bgStyle ));

	painter->setTextColor( dasa.fgColor );
}

/*****************************************************
**
**   DcPainter   ---   Constructor
**
******************************************************/
DcPainter:: DcPainter( wxDC *d )
{
	dc = d;
}

/*****************************************************
**
**   DcPainter   ---   setDefaults
**
******************************************************/
void DcPainter::setDefaults()
{
	setDefaultPen();
	setDefaultBrush();
	setTextColor( getDefaultTextColor());
}

/*****************************************************
**
**   DcPainter   ---   setBrushColor
**
******************************************************/
void DcPainter::setBrushColor( const wxColour &c )
{
	wxBrush b = dc->GetBrush();
	b.SetStyle( wxSOLID );
	b.SetColour( c );
	dc->SetBrush( b );
}

/*****************************************************
**
**   DcPainter   ---   setBrush
**
******************************************************/
void DcPainter::setBrush( const wxBrush &b )
{
	dc->SetBrush( b );
}

/*****************************************************
**
**   DcPainter   ---   getDefaultBrush
**
******************************************************/
wxBrush DcPainter::getDefaultBrush()
{
	return wxBrush( config->colors->bgColor );
}

/*****************************************************
**
**   DcPainter   ---   setPen
**
******************************************************/
void DcPainter::setPen( const wxPen &p )
{
	dc->SetPen( p );
}

/*****************************************************
**
**   DcPainter   ---   getDefaultPen
**
******************************************************/
wxPen DcPainter::getDefaultPen()
{
	return wxPen( config->colors->fgColor, 1, wxSOLID );
}

/*****************************************************
**
**   DcPainter   ---   drawRectangle
**
******************************************************/
void DcPainter::drawRectangle( const double &x, const double &y, const double &w, const double &h, const double rnd )
{
	if ( rnd == 0 )
		dc->DrawRectangle( a_rund( x ), a_rund( y ), a_rund( w ), a_rund( h ));
	else
		dc->DrawRoundedRectangle( a_rund( x ), a_rund( y ), a_rund( w ), a_rund( h ), rnd );
}

/*****************************************************
**
**   DcPainter   ---   drawPolygon
**
******************************************************/
void DcPainter::drawPolygon(int n, MyPoint points[], wxCoord xoffset, wxCoord yoffset )
{
	wxPoint *wxpoints = new wxPoint[n];
	for( int i  = 0; i < n; i++ )
	{
		wxpoints[i] = points[i].toWxPoint();
	}
	dc->DrawPolygon( n, wxpoints, xoffset, yoffset );
	delete[] wxpoints;
}

/*****************************************************
**
**   DcPainter   ---   drawLine
**
******************************************************/
void DcPainter::drawLine( const double &x1, const double &y1, const double &x2, const double &y2 )
{
	dc->DrawLine( a_rund( x1 ), a_rund( y1 ), a_rund( x2 ), a_rund( y2 ));
}

/*****************************************************
**
**   DcPainter   ---   drawTextFormatted
**
******************************************************/
void DcPainter::drawTextFormatted( const double &left, const double &top, const double &width, const double &height,
                                   const wxString &t, const int& align )
{
	//printf( "DcPainter::drawTextFormatted x %f y %f w %f h %f s %s\n", left, top, width, height, str2char( t ));
	wxCoord w, h;
	dc->GetTextExtent( t, &w, &h );
	int xmitte, ymitte, x, y;

	ymitte = 2 * (int)top + (int)height;
	ymitte /= 2;
	if ( align & Align::VCenter ) y = ymitte - h /2;
	else if ( align & Align::Top ) y = top;
	else y = top + height - h;

	xmitte = 2 * left + width;
	xmitte /= 2;
	if ( align & Align::HCenter ) x = xmitte - w /2;
	else if ( align & Align::Left ) x = left;
	else x = left + width - w;
	dc->DrawText( t, a_rund( x ), a_rund( y ));
}

/*****************************************************
**
**   DcPainter   ---   getTextExtent
**
******************************************************/
void DcPainter::getTextExtent( const wxString &s, double *w, double *h )
{
	wxCoord w1, h1;
	dc->GetTextExtent( s, &w1, &h1 );
	*h = h1;
	*w = w1;
}

/*****************************************************
**
**   DcPainter   ---   drawEllipse
**
******************************************************/
void DcPainter::drawEllipse( const double &x0, const double &y0, const double &xmax, const double &ymax )
{
	dc->DrawEllipse( a_rund(x0), a_rund(y0), a_rund(xmax), a_rund(ymax) );
}

/*****************************************************
**
**   DcPainter   ---   drawArc
**
******************************************************/
void DcPainter::drawArc( const double &x0, const double &y0, const double &xmax, const double &ymax,
                         const double &w1, const double &w2 )
{
	dc->DrawEllipticArc( a_rund(x0), a_rund(y0), a_rund(xmax), a_rund(ymax), a_rund(w1), a_rund( w2) );
}

/*****************************************************
**
**   DcPainter   ---   drawPoint
**
******************************************************/
void DcPainter::drawPoint( const double &x, const double &y )
{
	dc->DrawPoint( a_rund(x), a_rund(y) );
}

/*****************************************************
**
**   DcPainter   ---   setPenColor
**
******************************************************/
void DcPainter::setPenColor( const wxColour &c )
{
	wxPen p = dc->GetPen();
	p.SetStyle( wxSOLID );
	p.SetColour( c );
	dc->SetPen( p );
}

/*****************************************************
**
**   DcPainter   ---   setPenWidth
**
******************************************************/
void DcPainter::setPenWidth( const int &i )
{
	wxPen p = dc->GetPen();
	p.SetWidth( i );
	dc->SetPen( p );
}

/*****************************************************
**
**   DcPainter   ---   setSymbolFont
**
******************************************************/
void DcPainter::setSymbolFont( const int zoom )
{
	FontConfig *f = FontConfig::get();
	dc->SetFont( *f->getSymbolFont( zoom ));
}

/*****************************************************
**
**   DcPainter   ---   setGraphicFont
**
******************************************************/
void DcPainter::setGraphicFont( const int zoom )
{
	FontConfig *f = FontConfig::get();
	dc->SetFont( *f->getGraphicFont( zoom ));
}

/*****************************************************
**
**   DcPainter   ---   setFontSize
**
******************************************************/
void DcPainter::setFontSize( const int &size )
{
	wxFont font = dc->GetFont();
	font.SetPointSize( size );
	dc->SetFont( font );
}

/*****************************************************
**
**   DcPainter   ---   setTextColor
**
******************************************************/
void DcPainter::setTextColor( const wxColour& c )
{
	dc->SetTextForeground( c );
}
/*****************************************************
**
**   DcPainter   ---   getDefaultTextColor
**
******************************************************/
wxColour DcPainter::getDefaultTextColor()
{
	return wxColour( config->colors->fgColor );
}

/*****************************************************
**
**   DcPainter   ---   setTextColor
**
******************************************************/
void DcPainter::setTextBackgroundColor( const wxColour &c )
{
	dc->SetTextBackground( c );
}

/*****************************************************
**
**   DcPainter   ---   drawSignSymbol
**
******************************************************/
void DcPainter::drawSignSymbol( const int&x, const int &y, const int &sign, const int type, const int zoom, const double angle )
{
	Lang lang;
	double w, h;
	wxString s;

	if ( type == 2 ) s = lang.getSignName( sign, TSHORT );
	else s = lang.getSignSymbolCode( sign, type );
	getTextExtent( s, &w, &h );
	//printf( "DcPainter::drawSignSymbol Text extent w %f h %f\n", w, h );

	/*
	setTransparentBrush();
	setDefaultPen();
	drawRectangle( x-3, y-3, 6, 6 );
	drawRectangle( x-10, y-10, 20, 20 );
	*/

	double x3 = x - .5 * w * cos( 1.75 * PI + angle * DEG2RAD );
	double y3 = y + .5 * h * sin( 1.75 * PI+ angle * DEG2RAD );

	dc->DrawRotatedText( s, a_rund( x3 ), a_rund( y3 ), angle );
}

/*****************************************************
**
**   DcPainter   ---   drawBitmap
**
******************************************************/
void DcPainter::drawBitmap( const wxBitmap &bitmap, const double &x0, const double &y0, const bool &transparent )
{
	dc->DrawBitmap( bitmap, a_rund( x0 ), a_rund( y0 ), transparent );
}

/*****************************************************
**
**   DcPainter   ---   getTextZoomFactor
**
******************************************************/
double DcPainter::getTextZoomFactor( const double &totalsize )
{
	// 400 (minimum of width and height) pixel is 100%, means e.g. 12pt font will have 12pt
	double scalingfactor = 400.0 / 100.0;
	return totalsize / scalingfactor;
}

