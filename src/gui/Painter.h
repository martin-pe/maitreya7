/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/Painter.h
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

#ifndef _PAINTER_H_
#define _PAINTER_H_

#ifdef __GNUG__
#pragma interface "Painter.h"
#endif

#include "GraphConfig.h"
#include <wx/gdicmn.h>
#include <wx/string.h>

class DasaColorConfig;
class wxBitmap;
class wxColour;
class wxDC;

/*************************************************//**
*
*  
*
******************************************************/
class MyPoint
{
public:
	MyPoint() { x = 0; y = 0; }
	MyPoint( const double &x0, const double &y0 ) { x = x0; y = y0; }

	wxPoint toWxPoint();

	double x, y;
};

/*************************************************//**
*
*  
*
******************************************************/
class MyRect
{
public:
	MyRect() { x = y = width = height = 0; }
	MyRect( const double &x0, const double &y0, const double &w, const double &h ) { x = x0; y = y0; width = w; height = h; }

	wxRect toWxRect() { return wxRect( (int)x, (int)y, (int)width, (int)height ); }
	wxString toString();

	double x, y, width, height;
};

/*************************************************//**
*
* \brief abstract class for paiting graphical items and text on a GUI
*
******************************************************/
class Painter
{
public:

	virtual ~Painter() {}

	virtual void setDefaults() = 0;

	// brush
	virtual void setTransparentBrush();
	virtual wxBrush getDefaultBrush() = 0;
	virtual void setBrushColor( const wxColour& ) = 0;
	virtual void setBrush( const wxBrush& ) = 0;
	void setDefaultBrush() { setBrush( getDefaultBrush()); }

	// pen
	virtual void setPen( const wxPen& ) = 0;
	void setTransparentPen();
	virtual void setPenColor( const wxColour& ) = 0;
	virtual void setPenWidth( const int& ) {}
	void setDefaultPen() { setPen( getDefaultPen()); }
	virtual wxPen getDefaultPen() = 0;

	// drawing methods
	virtual void drawRectangle( const double &x, const double &y, const double &w, const double &h, const double = 0 ) = 0;
	void drawRectangle( const MyRect&, const double = 0 );
	virtual void drawPolygon(int n, MyPoint points[], wxCoord xoffset = 0, wxCoord yoffset = 0 ) = 0;
	virtual void drawLine( const double &x1, const double &y1, const double &x2, const double &y2 ) = 0;
	void drawLine( const MyPoint &p1, const MyPoint &p2 );
	virtual void drawEllipse( const double &x0, const double &y0, const double &xmax, const double &ymax ) = 0;
	virtual void drawArc( const double &x0, const double &y0, const double &xmax, const double &ymax,
		const double &w1, const double &w2 ) = 0;
	virtual void drawPoint( const double &x, const double &y ) = 0;

	// TODO not implemented by PdfPainter, required for chart drawing (background images)
	virtual void drawBitmap( const wxBitmap &bitmap, const double &x0, const double &y0, const bool &transparent ) {}

	// text
	virtual void drawTextFormatted( const double &x, const double &y, const double &w, const double &h,
		const wxString &t, const int& align ) = 0;
	void drawTextFormatted( const MyRect &rect, const wxString &t, const int& align );
	virtual void getTextExtent( const wxString &s, double *w, double *h ) = 0;
	void drawSimpleText( const MyRect &r, const wxString &s );
	void drawSimpleText( const double &x, const double &y, const double &w, const double &h, const wxString &s );
	virtual void drawSignSymbol( const int&x, const int &y, const int &sign, const int type = 0, const int zoom = 100,
		const double angle = 0 ) = 0;

	// fonts and text properties
	void setDefaultTextColor();
	virtual void setTextColor( const wxColour& ) = 0;
	virtual wxColour getDefaultTextColor() = 0;
	virtual void setTextBackgroundColor( const wxColour& ) = 0;
	virtual void setSymbolFont( const int = 100 ) = 0;
	virtual void setGraphicFont( const int = 100 ) = 0;
	virtual void setFontSize( const int& ) = 0;
	virtual double getTextZoomFactor( const double &totalsize ) = 0;
};


/*************************************************//**
*
* 
*
******************************************************/
class GraphicalDasaPaintHelper
{
public:
	GraphicalDasaPaintHelper( Painter *painter ) { this->painter = painter; }

	void setDasaColors( const int &object, const int &rasitype );
	void setDasaColors( const int &objectid );
	void drawDasaBar( const int &dasalord, const int &isRasiLord, const DasaColorConfig *cfg, const int &x1, const int &y1,
		const int &w, const int &h, const int &cornerRadius );

private:
	Painter *painter;

};

/*************************************************//**
*
* \brief Painter implementation for wx device context (GUI and file export via memory DC)
*
******************************************************/
class DcPainter: public Painter
{
public:
	DcPainter( wxDC* );

	virtual void setDefaults();

	// brush
	virtual void setBrush( const wxBrush& );
	virtual void setBrushColor( const wxColour& );
	virtual wxBrush getDefaultBrush();

	// pen
	virtual void setPen( const wxPen& );
	virtual void setPenColor( const wxColour& );
	virtual void setPenWidth( const int& );
	virtual wxPen getDefaultPen();

	// drawing methods
	virtual void drawRectangle( const double &x, const double &y, const double &w, const double &h, const double = 0 );
	virtual void drawPolygon(int n, MyPoint points[], wxCoord xoffset = 0, wxCoord yoffset = 0 );
	virtual void drawLine( const double &x1, const double &y1, const double &x2, const double &y2 );
	virtual void drawEllipse( const double &x0, const double &y0, const double &xmax, const double &ymax );
	virtual void drawArc( const double &x0, const double &y0, const double &xmax, const double &ymax,
		const double &w1, const double &w2 );
	virtual void drawPoint( const double &x, const double &y );
	virtual void drawBitmap( const wxBitmap &bitmap, const double &x0, const double &y0, const bool &transparent );

	// text
	virtual void drawTextFormatted( const double &x, const double &y, const double &w, const double &h,
	                                const wxString &t, const int& align );
	virtual void getTextExtent( const wxString &s, double *w, double *h );
	virtual void drawSignSymbol( const int&x, const int &y, const int &sign, const int type = 0, const int zoom = 100,
		const double angle = 0 );

	// fonts and text properties
	virtual void setSymbolFont( const int = 100 );
	virtual void setGraphicFont( const int = 100 );
	virtual void setTextColor( const wxColour& );
	virtual wxColour getDefaultTextColor();
	virtual void setTextBackgroundColor( const wxColour& );
	virtual void setFontSize( const int& );
	virtual double getTextZoomFactor( const double &totalsize );

private:
	wxDC *dc;
};


#endif

