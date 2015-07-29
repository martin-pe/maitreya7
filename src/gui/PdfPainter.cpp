/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/PdfPainter.cpp
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
#pragma implementation "PdfPainter.h"
#endif

#include "PdfPainter.h"
#include "PdfBase.h"

#include "Conf.h"
#include "guibase.h"
#include "IconProvider.h"
#include "Lang.h"
#include "Writer.h"

#include <wx/dc.h>

extern Config *config;

#define PDF_TEXT_ZOOMFACTOR 30
#define PDF_DEFAULT_FGCOLOR *wxBLACK
#define PDF_DEFAULT_BGCOLOR *wxWHITE
#define PDF_PEN_SCALINGFACTOR 5.0

/*****************************************************
**
**   PdfPainter   ---   Constructor
**
******************************************************/
PdfPainter::PdfPainter( BasePdfDocument *pdfdoc )
{
	pdf = pdfdoc;
	drawmode = wxPDF_STYLE_DRAW;
	textwriter = WriterFactory().getWriter( WRITER_TEXT );

	defaultLineStyle = new wxPdfLineStyle( pdf->GetLineStyle());
	defaultTextColor = new wxPdfColour( pdf->GetTextColour() );
}

/*****************************************************
**
**   PdfPainter   ---   Destructor
**
******************************************************/
PdfPainter::~PdfPainter()
{
	delete defaultLineStyle;
	delete defaultTextColor;
	delete textwriter;
}

/*****************************************************
**
**   PdfPainter   ---   setBrush
**
******************************************************/
void PdfPainter::setBrush( const wxBrush &b )
{
	// TODO howto scale width und height 
	if ( b.GetStyle() == wxSTIPPLE )
	{
		wxImage image;
		wxBitmap *bitmap = b.GetStipple();
		if ( bitmap && bitmap->IsOk())
		{
			image = bitmap->ConvertToImage();

			double width = image.GetWidth() / 3;
			double height = image.GetHeight() / 3;

			// setup a hash value for the image
			wxString s = createImageHash( &image );

			pdf->AddPattern( s, image, width, height );
			drawmode = wxPDF_STYLE_FILL;
			pdf->SetFillPattern( s );
			pdf->SetDrawPattern( s );
		}
	}
	else if ( b.GetStyle() != wxTRANSPARENT && b.GetColour().IsOk() ) setBrushColor( b.GetColour() );
	else
	{
		drawmode = wxPDF_STYLE_DRAW;
	}
}

wxColour colorToGreyScale( const wxColour&c )
{
	int g = ( c.Red() + c.Green() + c.Blue()) / 3.0;
	return wxColour( g, g, g );
}

/*****************************************************
**
**   PdfPainter   ---   setBrushColor
**
******************************************************/
void PdfPainter::setBrushColor( const wxColour &c )
{
	drawmode = wxPDF_STYLE_FILL;
	pdf->SetFillColour( c );
}

/*****************************************************
**
**   PdfPainter   ---   getDefaultBrush
**
******************************************************/
wxBrush PdfPainter::getDefaultBrush()
{
	return wxBrush( PDF_DEFAULT_BGCOLOR, wxTRANSPARENT );
}

/*****************************************************
**
**   PdfPainter   ---   setPen
**
******************************************************/
void PdfPainter:: setPen( const wxPen &p )
{
	const double defaultWidth = 1;
	wxPdfLineStyle style;
	switch ( p.GetStyle() )
	{
		case wxDOT:
		{
			wxPdfArrayDouble dash1;
			dash1.Add( .5 );
			style = wxPdfLineStyle( -1, wxPDF_LINECAP_NONE, wxPDF_LINEJOIN_MITER, dash1, 10.);
		}
		break;
		case wxLONG_DASH:
		{
			wxPdfArrayDouble dash2;
			dash2.Add( 3 );
			style  = wxPdfLineStyle( -1, wxPDF_LINECAP_BUTT, wxPDF_LINEJOIN_MITER, dash2, 0.);
			break;
		}
		break;
		case wxSHORT_DASH:
		{
			wxPdfArrayDouble dash3;
			dash3.Add( 1.5 );
			style  = wxPdfLineStyle( -1, wxPDF_LINECAP_BUTT, wxPDF_LINEJOIN_MITER, dash3, 0.);
		}
		case wxDOT_DASH:
		{
			wxPdfArrayDouble dash4;
			dash4.Add( 1.5 );
			dash4.Add( 3 );
			style  = wxPdfLineStyle( -1, wxPDF_LINECAP_BUTT, wxPDF_LINEJOIN_MITER, dash4, 0.);
		}
		break;
		default:
		// Noting to do
		break;
	}

	style.SetWidth( defaultWidth * p.GetWidth() / PDF_PEN_SCALINGFACTOR );
	if ( p.GetColour().IsOk() )
	{
		style.SetColour( p.GetColour() );
		pdf->SetDrawColour( p.GetColour());
	}
	pdf->SetLineStyle( style );
}

/*****************************************************
**
**   PdfPainter   ---   setPenColor
**
******************************************************/
void PdfPainter::setPenColor( const wxColour &c )
{
	wxPdfLineStyle style = pdf->GetLineStyle();
	style.SetColour( c );
	pdf->SetLineStyle( style );
}

/*****************************************************
**
**   PdfPainter   ---   getDefaultPen
**
******************************************************/
wxPen PdfPainter::getDefaultPen()
{
	return wxPen( PDF_DEFAULT_FGCOLOR, 1, wxSOLID );
}

/*****************************************************
**
**   PdfPainter   ---   setDefaults
**
******************************************************/
void PdfPainter::setDefaults()
{
	setPen( getDefaultPen());
	setBrush( getDefaultBrush());
	pdf->SetTextColour( PDF_DEFAULT_FGCOLOR );
}

/*****************************************************
**
**   PdfPainter   ---   setGraphicFont
**
******************************************************/
void PdfPainter::setGraphicFont( const int zoom )
{
	pdf->setGraphicFont();
}

/*****************************************************
**
**   PdfPainter   ---   setSymbolFont
**
******************************************************/
void PdfPainter::setSymbolFont( const int zoom )
{
	pdf->setSymbolFont( zoom );
}

/*****************************************************
**
**   PdfPainter   ---  setTextColor
**
******************************************************/
void PdfPainter::setTextColor( const wxColour &c )
{
	pdf->SetTextColour( c );
}

/*****************************************************
**
**   PdfPainter   ---  getDefaultTextColor
**
******************************************************/
wxColour PdfPainter::getDefaultTextColor()
{
	return PDF_DEFAULT_FGCOLOR;
}

/*****************************************************
**
**   PdfPainter   ---  getTextExtent
**
******************************************************/
void PdfPainter::getTextExtent( const wxString &s, double *w, double *h )
{
	*w = pdf->GetStringWidth( s );
	//if ( s.Len() == 1 ) *w *= 1.7; // make single symbol items larger

	*h = .4 * pdf->GetFontSize();
}

/*****************************************************
**
**   PdfPainter   ---   getTextZoomFactor
**
******************************************************/
double PdfPainter::getTextZoomFactor( const double &totalsize )
{
	return PDF_TEXT_ZOOMFACTOR;
}

/*****************************************************
**
**   PdfPainter   ---  drawRectangle
**
******************************************************/
void PdfPainter::drawRectangle( const double &x, const double &y, const double &w, const double &h, const double rnd )
{
	// TODO round not implemented
	pdf->Rect( x, y, w, h, drawmode );
}

/*****************************************************
**
**   PdfPainter   ---  drawRectangle
**
******************************************************/
void PdfPainter::drawRectangle( const wxRect &rect )
{
	drawRectangle( rect.x, rect.y, rect.width, rect.height );
}

/*****************************************************
**
**   PdfPainter   ---   drawPolygon
**
******************************************************/
void PdfPainter::drawPolygon(int n, MyPoint points[], wxCoord xoffset, wxCoord yoffset )
{
	wxPdfArrayDouble x, y;
	for ( int i = 0; i < n; i++ )
	{
		x.Add( points[i].x + xoffset );
		y.Add( points[i].y + yoffset );
	}
	pdf->Polygon( x, y, drawmode );
}

/*****************************************************
**
**   PdfPainter   ---  drawLine
**
******************************************************/
void PdfPainter::drawLine( const double &x1, const double &y1, const double &x2, const double &y2 )
{
	pdf->Line( x1, y1, x2, y2 );
}

/*****************************************************
**
**   PdfPainter   ---  drawTextFormatted
**
******************************************************/
void PdfPainter::drawTextFormatted( const double &x, const double &y, const double &w, const double &h,
                                    const wxString &t, const int& align )
{
	double sw, sh;
	double oldx = pdf->GetX();
	double oldy = pdf->GetY();
	double x1 = x;
	double y1 = y;

	pdf->SetXY( x, y1 );

	getTextExtent( t, &sw, &sh );

	if ( align & Align::Top )
	{
		y1 = y + h; // + .5 * sh;
	}
	else if ( align & Align::Bottom )
	{
		y1 = y + h; // + .5 * sh;
	}
	else if ( align & Align::VCenter )
	{
		y1 = y + .5 * ( h + sh );
	}
	if ( align & Align::HCenter )
	{
		x1 = x + .5 * ( w - sw );
	}
	else if ( align & Align::Right )
	{
		x1 = x + w - sw;
	}
	pdf->Text( x1, y1, t );
	//drawEllipse( x1, y1, 2, 2 );
	pdf->SetXY( oldx, oldy );
}

/*****************************************************
**
**   PdfPainter   ---   drawEllipse
**
******************************************************/
void PdfPainter::drawEllipse( const double &x0, const double &y0, const double &xmax, const double &ymax )
{
	pdf->Ellipse( x0 + xmax/2 , y0 + ymax/2, xmax/2, ymax/2 ); // Cannot be filled
}

/*****************************************************
**
**   PdfPainter   ---   drawArc
**
******************************************************/
void PdfPainter::drawArc( const double &x0, const double &y0, const double &xmax, const double &ymax,
                          const double &w1, const double &w2 )
{
	pdf->Ellipse( x0 + xmax/2 , y0 + ymax/2, xmax/2, ymax/2, 0, w1, w2 );
}

/*****************************************************
**
**   PdfPainter   ---   drawPoint
**
******************************************************/
void PdfPainter::drawPoint( const double &x, const double &y )
{
	pdf->Line( x, y, x, y );
}

/*****************************************************
**
**   PdfPainter   ---   drawSignSymbol
**
******************************************************/
void PdfPainter::drawSignSymbol( const int&x, const int &y, const int &sign, const int type, const int zoom, const double angle )
{
	Lang lang;
	double w, h;
	wxString s;

	if ( type == 2 ) s = textwriter->getSignName( sign, TSHORT );
	else s = lang.getSignSymbolCode( sign, type );
	getTextExtent( s, &w, &h );

	/*
	setTransparentBrush();
	setDefaultPen();
	drawRectangle( x-3, y-3, 6, 6 );
	drawRectangle( x-10, y-10, 20, 20 );
	*/

	double x4 = x - .5 * w * cos( .25 * PI + angle * DEG2RAD );
	double y4 = y + .5 * h * sin( .25 * PI + angle * DEG2RAD );

	pdf->RotatedText( x4, y4, s, angle );
}

