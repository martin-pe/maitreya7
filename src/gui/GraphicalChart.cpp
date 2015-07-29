/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/GraphicalChart.cpp
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
#pragma implementation "GraphicalChart.h"
#endif

#include "GraphicalChart.h"

#include "ChartProperties.h"
#include "ColorConfig.h"
#include "Conf.h"
#include "constants.h"
#include "IconProvider.h"
#include "mathbase.h"
#include "Painter.h"
#include "VedicChartConfig.h"
#include "WesternChartConfig.h"
#include "Writer.h"

#include <wx/image.h>

extern Config *config;

/*****************************************************
**
**   GraphicalChart   ---   Constructor
**
******************************************************/
GraphicalChart::GraphicalChart( const ChartProperties *chartprops )
 : chartprops( chartprops )
{
	extrazoom = 1.0;
}

/*****************************************************
**
**   GraphicalChart   ---   paint
**
******************************************************/
void GraphicalChart::paint( Painter *painter, const MyRect &r, const wxRect *rrect )
{
	this->painter = painter;
	rect = r;
	refreshRect = (wxRect*)rrect;

	rxmax = .5 * rect.width;
	rymax = .5 * rect.height;
	rmax = Min( rxmax, rymax );
	xcenter = rect.x  + rect.width / 2;
	ycenter = rect.y + rect.height / 2;

	if (chartprops->isVedic())
		cfg =	(ChartConfig*)VedicChartConfigLoader::get()->getConfig( chartprops->getVedicSkin());
	else
		cfg = (ChartConfig*)WesternChartConfigLoader::get()->getConfig( chartprops->getWesternSkin());

	// set pen and brush of config, otherwise default resources
	defaultPen = ( cfg->pen.IsOk() ? cfg->pen : painter->getDefaultPen());
  defaultBrush = ( cfg->brush.IsOk() ? cfg->brush : painter->getDefaultBrush() );

	paintBackground( &cfg->widgetBackground );

	double painterzoom = extrazoom * painter->getTextZoomFactor( Min( rect.width, rect.height ));

	textzoom = painterzoom;
	symbolzoom = painterzoom;
	painter->setSymbolFont( symbolzoom );
	painter->getTextExtent( wxT( "A" ), &text_width,  &symbol_height );
	painter->setGraphicFont( textzoom );
	painter->getTextExtent( wxT( "A" ), &text_width,  &text_height );

	paintInternal( cfg );
}

/*****************************************************
**
**   GraphicalChart   ---   setDefaultTextColor
**
******************************************************/
void GraphicalChart::setDefaultTextColor()
{
	painter->setTextColor( defaultPen.GetColour().IsOk() ?  defaultPen.GetColour() : painter->getDefaultTextColor());
}

/*****************************************************
**
**   GraphicalChart   ---   paintBackground
**
******************************************************/
void GraphicalChart::paintBackground( GcWidgetBackground *bg )
{
	if ( bg->brush.IsOk() )
	{
		painter->setBrush( bg->brush );
		if( bg->brush.GetColour().IsOk()) painter->setPen( wxPen( bg->brush.GetColour() ));
		else painter->setTransparentPen();
	}
	else
	{
		wxBrush brush = painter->getDefaultBrush();
		painter->setBrush( brush );
		painter->setPen( wxPen( brush.GetColour() ));
	}
	painter->drawRectangle( rect );

	for( unsigned int i = 0; i < bg->images.size(); i++ )
	{
		GcImage	img = bg->images[i];
		wxBitmap* bitmap = IconProvider::getBitmap( img.filename );
		int w = bitmap->GetWidth();
		int h = bitmap->GetHeight();

		double x0 = .5 * rect.width - .5 * w;
		double y0 = .5 * rect.height - .5 * h;

		if ( img.align & Align::Left ) x0 = 0;
		else if ( img.align & Align::Right ) x0 = rect.width - w;

		if ( img.align & Align::Top ) y0 = 0;
		else if ( img.align & Align::Bottom ) y0 = rect.height - h;

		if ( img.mirrorX || img.mirrorY )
		{
			wxImage image = bitmap->ConvertToImage();
			if ( img.mirrorX ) image = image.Mirror( true );
			if ( img.mirrorY ) image = image.Mirror( false );
			painter->drawBitmap( wxBitmap( image ), rect.x + x0, rect.y + y0, true);
		}
		else
		{
			painter->drawBitmap( *bitmap, rect.x + x0, rect.y + y0, true);
		}
	}
}

/*****************************************************
**
**   GraphicalChart   ---   paintMarkerLine
**
******************************************************/
void GraphicalChart::paintMarkerLine( const double &alfa, const GcChartFrame &c1, const GcChartFrame &c2 )
{
	painter->setPen( defaultPen );
	const MyPoint p1 = frameBorderPointToXy( alfa, c1 );
	const MyPoint p2 = frameBorderPointToXy( alfa, c2 );
	painter->drawLine( p1, p2 );
}

/*****************************************************
**
**   GraphicalChart   ---   frameBorderPointToXy
**
******************************************************/
MyPoint GraphicalChart::frameBorderPointToXy( const double &angle, const GcChartFrame &frame )
{
	const double alfa = red_deg( angle );
	const double a = alfa * PI / 180;

	int type = ( frame.frameType != CHART_FRAME_TYPE_INHERITED ) ? frame.frameType : cfg->frameType;
	switch( type )
	{
		case CHART_FRAME_TYPE_CIRCLE:
		{
			const double radius = rmax * frame.radius / 100.0;
			return MyPoint( xcenter + radius * cos( alfa * PI / 180 ), ycenter - radius * sin( alfa * PI / 180 ) );
		}
		break;
		case CHART_FRAME_TYPE_ELLIPSE:
		{
			const double xradius = rxmax * frame.radius / 100.0;
			const double yradius = rymax * frame.radius / 100.0;
			return MyPoint( xcenter + xradius * cos( alfa * PI / 180 ), ycenter - yradius * sin( alfa * PI / 180 ) );
		}
		break;
		case CHART_FRAME_TYPE_SQUARE:
		{
			const double radius = rmax * frame.radius / 100.0;
			if ( alfa > 315 || alfa <= 45 )
			{
				return MyPoint( xcenter + radius, ycenter - SQRT2 * radius * sin( a ) );
			}
			else if ( alfa < 135 )
			{
				return MyPoint( xcenter + SQRT2 * radius * cos( a ), ycenter - radius );
			}
			else if ( alfa < 225 )
			{
				return MyPoint( xcenter - radius, ycenter - SQRT2 * radius * sin( a ) );
			}
			else // 225 < alfa < 315
			{
				return MyPoint( xcenter + SQRT2 * radius * cos( a ), ycenter + radius );
			}
		}
		break;

		case CHART_FRAME_TYPE_RECTANGLE:
		{
			const double xradius = rxmax * frame.radius / 100.0;
			const double yradius = rymax * frame.radius / 100.0;
			if ( alfa > 315 || alfa <= 45 )
			{
				return MyPoint( xcenter + xradius, ycenter - SQRT2 * yradius * sin( a ) );
			}
			else if ( alfa < 135 )
			{
				return MyPoint( xcenter + SQRT2 * xradius * cos( a ), ycenter - yradius );
			}
			else if ( alfa < 225 )
			{
				return MyPoint( xcenter - xradius, ycenter - SQRT2 * yradius * sin( a ) );
			}
			else // 225 < alfa < 315
			{
				return MyPoint( xcenter + SQRT2 * xradius * cos( a ), ycenter + yradius );
			}
		}
		break;

		default:
			printf( "Invalid chart frame type %d\n", cfg->frameType );
			assert( false );
		break;
	}
}

/*****************************************************
**
**   GraphicalChart   ---   drawChartFrameField
**
******************************************************/
void GraphicalChart::drawChartFrameField( const double &alfa0, const double &alfa1, const GcChartFrame &frame0, const GcChartFrame &frame1 )
{ 
  int i;
	const double a0 = red_deg( alfa0 );
	const double a1 = red_deg( alfa1 );
	ASSERT_VALID_DEGREE( a0 );
	ASSERT_VALID_DEGREE( a1 );

	switch( cfg->frameType )
	{
		case CHART_FRAME_TYPE_CIRCLE:
		case CHART_FRAME_TYPE_ELLIPSE:
		{
			const int nbsteps = 6;
			const int ntotal =  2 * ( nbsteps + 1 );
			MyPoint points[ntotal];
			double a = a0;
			const double astep = red_deg( a1 - a0 ) / nbsteps;

			for ( i = 0; i <= nbsteps; i++ )
			{
				points[i] = frameBorderPointToXy( a, frame0 );
				a += astep;
			}
			a = a1;
			for ( i = nbsteps + 1; i < ntotal; i++ )
			{
				points[i] = frameBorderPointToXy( a, frame1 );
				a -= astep;
			}
			painter->drawPolygon( ntotal, points );
		}
		break;

		case CHART_FRAME_TYPE_SQUARE:
		case CHART_FRAME_TYPE_RECTANGLE:
		{
			const int quadrant0 = (int)((a0 + 45 ) / 90 );
			const int quadrant1 = (int)((a1 + 45 ) / 90 );
			const bool qdrchange = (bool)(quadrant0 != quadrant1 );
			const double qdrcorner = 90 * (int)( Max( a0, a1 ) / 90 ) + 45;

			//const int ntotal =  qdrchange ? 6 : 4;
			const int ntotal =  6;
			MyPoint points[ntotal];
			int i = 0;

			points[i++] = frameBorderPointToXy( a0, frame0 );
			if ( qdrchange ) points[i++] = frameBorderPointToXy( qdrcorner, frame0 );
			points[i++] = frameBorderPointToXy( a1, frame0 );
			points[i++] = frameBorderPointToXy( a1, frame1 );
			if ( qdrchange ) points[i++] = frameBorderPointToXy( qdrcorner, frame1 );
			points[i++] = frameBorderPointToXy( a0, frame1 );
			//painter->drawPolygon( ntotal, points );
			painter->drawPolygon( i, points );
		}
		break;
		default:
			printf( "Invalid chart frame type %d\n", cfg->frameType );
			assert( false );
		break;
	}

} 

/*****************************************************
**
**   GraphicalChart   ---   paintChartFrame
**
******************************************************/
void GraphicalChart::paintChartFrame( const GcChartFrame &frame )
{
  if ( ! frame.show ) return;

  if ( frame.pen.IsOk() ) painter->setPen( frame.pen );
  else painter->setPen( defaultPen );

  if ( frame.brush.IsOk() ) painter->setBrush( frame.brush );
  else painter->setTransparentBrush();

	int type = ( frame.frameType != CHART_FRAME_TYPE_INHERITED ) ? frame.frameType : cfg->frameType;
	switch( type )
	{
		case CHART_FRAME_TYPE_CIRCLE:
		{
			const double radius = rmax * frame.radius / 100;
			painter->drawEllipse( xcenter - radius, ycenter - radius, 2 * radius, 2 * radius );
		}
		break;
		case CHART_FRAME_TYPE_ELLIPSE:
		{
			const double xradius = rxmax * frame.radius / 100;
			const double yradius = rymax * frame.radius / 100;
			painter->drawEllipse( xcenter - xradius, ycenter - yradius, 2 * xradius, 2 * yradius );
		}
		break;
		case CHART_FRAME_TYPE_SQUARE:
		{
			const double radius = rmax * frame.radius / 100;
			painter->drawRectangle( xcenter - radius, ycenter - radius, 2 * radius, 2 * radius );
		}
		break;
		case CHART_FRAME_TYPE_RECTANGLE:
		{
			const double xradius = rxmax * frame.radius / 100;
			const double yradius = rymax * frame.radius / 100;
			painter->drawRectangle( xcenter - xradius, ycenter - yradius, 2 * xradius, 2 * yradius );
		}
		break;
		default:
			printf( "Invalid chart frame type %d\n", cfg->frameType );
			assert( false );
		break;
	}
}

/*****************************************************
**
**   GraphicalChart   ---   paintArrow
**
******************************************************/
void GraphicalChart::paintArrow( const double &a, GcArrow &arrow )
{
	MyPoint p = frameBorderPointToXy( a, arrow.outerFrame );

	if ( arrow.pen.IsOk() ) painter->setPen( arrow.pen );
	else painter->setPen( defaultPen );
	if ( arrow.innerFrame.radius == 0 )
	{
		MyPoint q = frameBorderPointToXy( a+180, arrow.outerFrame );
		painter->drawLine( p.x, p.y, q.x, q.y );
	}
	else
	{
		paintMarkerLine( a, arrow.innerFrame, arrow.outerFrame );
		paintMarkerLine( a+180, arrow.innerFrame, arrow.outerFrame );
	}
	GcChartFrame frame( arrow.outerFrame.radius * .97 );
	double ra = 1;
	MyPoint q1 = frameBorderPointToXy( a - ra, frame );
	painter->drawLine( p.x, p.y, q1.x, q1.y );
	MyPoint q2 = frameBorderPointToXy( a + ra, frame );
	painter->drawLine( p.x, p.y, q2.x, q2.y );
}

