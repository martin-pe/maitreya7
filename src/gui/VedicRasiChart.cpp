/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/VedicRasiChart.cpp
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
#pragma implementation "VedicRasiChart.h"
#endif

#include "VedicRasiChart.h"

#include "Lang.h"
#include "ChartProperties.h"
#include "Lang.h"
#include "mathbase.h"
#include "Painter.h"
#include "VedicChartConfig.h"
#include "Writer.h"

/*****************************************************
**
**   VedicRasiChart   ---   Constructor
**
******************************************************/
VedicRasiChart::VedicRasiChart( const ChartProperties *chartprops, const int chartcount )
		: BasicVedicChart( chartprops, 12, chartcount )
{
	centerstring_graphic = false;
}

/*****************************************************
**
**   VedicRasiChart   ---   paintChart
**
******************************************************/
void VedicRasiChart::paintChart()
{
	painter->setPen( defaultPen );
	painter->setBrush( defaultBrush );
	setDefaultTextColor();

	switch( chartprops->getVedicGraphicStyle().indianChartType )
	{
		case VGRAPHIC_NORTH_INDIAN:
			paintNorth();
		break;
		case VGRAPHIC_EAST_INDIAN:
			paintEast();
		break;
		case VGRAPHIC_SOUTH_INDIAN:
			paintSouth();
		break;
		default:
			printf( "ERROR chartprops->getVedicGraphicStyle().indianChartType %d\n", chartprops->getVedicGraphicStyle().indianChartType );
			paintSouth();
			//assert( false );
		break;
	}
}

/*****************************************************
**
**   VedicRasiChart   ---   paintSouth
**
******************************************************/
void VedicRasiChart::paintSouth()
{
	int i;

	const double xstep = .5 * xr;
	const double ystep = .5 * yr;

	painter->setPen( defaultPen );

	// Rasi coordinates, single mode only
	const MyRect rasi_rect[12] = {
		MyRect( xcenter - xstep, ycenter - 2 * ystep, xstep, ystep ),
		MyRect( xcenter, ycenter - 2 * ystep, xstep, ystep ),
		MyRect( xcenter + xstep, ycenter - 2 * ystep, xstep, ystep ),
		MyRect( xcenter + xstep, ycenter - ystep, xstep, ystep ),
		MyRect( xcenter + xstep, ycenter, xstep, ystep ),
		MyRect( xcenter + xstep, ycenter + ystep, xstep, ystep ),

		MyRect( xcenter, ycenter + ystep, xstep, ystep ),
		MyRect( xcenter - xstep, ycenter + ystep, xstep, ystep ),
		MyRect( xcenter - 2 * xstep, ycenter + ystep, xstep, ystep  ),
		MyRect( xcenter - 2 * xstep, ycenter, xstep, ystep ),
		MyRect( xcenter - 2 * xstep, ycenter-ystep, xstep, ystep ),
		MyRect( xcenter - 2 * xstep, ycenter - 2 * ystep, xstep, ystep )
	};

	// Rasi coordinates for 1st chart (double mode only)
	const MyRect planet1_rect[12] = {
		MyRect( xcenter - xstep, ycenter - 1.5 * ystep, xstep, ystep / 2 ),
		MyRect( xcenter, ycenter - 1.5 * ystep, xstep, ystep / 2 ),
		MyRect( xcenter + xstep, ycenter - 1.5 * ystep, xstep / 2, ystep / 2 ),
		MyRect( xcenter + xstep, ycenter - ystep, xstep / 2, ystep ),
		MyRect( xcenter + xstep, ycenter, xstep / 2, ystep ),
		MyRect( xcenter + xstep, ycenter + ystep, xstep / 2, ystep / 2 ),

		MyRect( xcenter, ycenter + ystep, xstep, ystep / 2 ),
		MyRect( xcenter - xstep, ycenter + ystep, xstep, ystep / 2 ),
		MyRect( xcenter - 1.5 * xstep, ycenter + ystep, xstep / 2, ystep / 2 ),
		MyRect( xcenter - 1.5 * xstep, ycenter, xstep / 2, ystep ),
		MyRect( xcenter - 1.5 * xstep, ycenter-ystep, xstep / 2, ystep ),
		MyRect( xcenter - 1.5 * xstep, ycenter - 1.5 * ystep, xstep / 2, ystep / 2 )
	};

	// Rasi coordinates for 2nd chart (double mode only)
	const MyRect planet2_rect[12] = {
		MyRect( xcenter - xstep, ycenter - 2 * ystep, xstep, ystep / 2 ),
		MyRect( xcenter, ycenter - 2 * ystep, xstep, ystep / 2 ),
		MyRect( xcenter + xstep, ycenter - 2 * ystep, xstep, ystep / 2 ),
		MyRect( xcenter + 1.5 * xstep, ycenter - ystep, xstep / 2, ystep ),
		MyRect( xcenter + 1.5 * xstep, ycenter, xstep / 2, ystep ),
		MyRect( xcenter + xstep, ycenter + 1.5 * ystep, xstep, ystep / 2 ),

		MyRect( xcenter, ycenter + 1.5 * ystep, xstep, ystep / 2 ),
		MyRect( xcenter - xstep, ycenter + 1.5 * ystep, xstep, ystep / 2 ),
		MyRect( xcenter - 2 * xstep, ycenter + 1.5 * ystep, xstep, ystep / 2 ),
		MyRect( xcenter - 2 * xstep, ycenter, xstep / 2, ystep ),
		MyRect( xcenter - 2 * xstep, ycenter-ystep, xstep / 2, ystep ),
		MyRect( xcenter - 2 * xstep, ycenter - 2 * ystep, xstep, ystep / 2 )
	};

	// rectangle for separation of double charts
	const MyRect transit_rect( xcenter - 1.5 * xstep, ycenter - 1.5 * ystep, 3 * xstep, 3 * ystep );

  GcZodiacalSigns *signs = &vconf->signs;
	if ( signs->needsBgPaint() )
	{
		for ( i = ARIES; i <= PISCES; i++ )
		{
			painter->setBrush( signs->getBrush( i ));
			painter->setTransparentPen();
			//painter->setDefaultPen();
			painter->drawRectangle( rasi_rect[i] );
		}
	}

	paintOuterRectangle();

	painter->setPen( defaultPen );
	painter->setTransparentBrush();
	painter->drawLine( xcenter - xr + xstep, ycenter - yr, xcenter - xr + xstep, ycenter + yr  );
	painter->drawLine( xcenter - xr + 3 * xstep, ycenter - yr, xcenter - xr + 3 * xstep, ycenter + yr  );

	painter->drawLine( xcenter - xr + 2 * xstep, ycenter - yr, xcenter-xr + 2 * xstep, ycenter - yr + ystep  );
	painter->drawLine( xcenter - xr + 2 * xstep, ycenter + yr - ystep, xcenter - xr + 2 * xstep, ycenter + yr  );

	painter->drawLine( xcenter - xr, ycenter - yr + ystep, xcenter + xr, ycenter - yr + ystep  );
	painter->drawLine( xcenter - xr, ycenter - yr + 3 * ystep, xcenter + xr, ycenter - yr + 3 * ystep  );

	painter->drawLine( xcenter - xr, ycenter - yr + 2 * ystep, xcenter - xr + xstep, ycenter - yr + 2 * ystep  );
	painter->drawLine( xcenter + xr - xstep, ycenter - yr + 2 * ystep, xcenter + xr, ycenter - yr + 2 * ystep  );

	// TODO soll das so bleiben, 3-mal
	if ( chart_count > 1 ) painter->drawRectangle( transit_rect );

	if ( ! chartprops->isBlank() )
	{
		for ( i = ARIES; i <= PISCES; i++ )
		{
			if ( chart_count == 1 ) drawFieldText( rasi_rect[i], i, Align::Center, 0 );
			else
			{
				drawFieldText( planet1_rect[i], i, Align::Center, 0 );
				drawFieldText( planet2_rect[i], i, Align::Center, 1 );
			}
		}
		paintCenterString();
	}
	painter->setGraphicFont( textzoom );
}

/*****************************************************
**
**   VedicRasiChart   ---   paintNorth
**
******************************************************/
void VedicRasiChart::paintNorth()
{
	Lang lang;
	wxString s;
	int current, i;

	// used for sign number/symbol display
	const double rtol = xmax / 15.0;

	// used for free space in the middle of the chart (leave free for center string if required)
	const double mtol = xmax / 100.0;

	// 1/4 of the chart
	const double xi = .5 * xr;
	const double yi = .5 * yr;

	// middle square for separation of 2 charts (transit or partner mode)
	const double ri_prop = .4;
	const double xm = ri_prop * xi + ( 1 - ri_prop ) * xr;
	const double ym = ri_prop * yi + ( 1 - ri_prop ) * yr;

	// text separation from chart border
	const double text_border = xr / 30.0;

	// Rasi coordinates in single mode
	const MyRect rasi_rect[12] = {
		MyRect( xcenter - xi, ycenter - yr, xr, yr ),
		MyRect( xcenter - xr, ycenter - yr, xr, yi ),
		MyRect( xcenter - xr, ycenter - yr, xr, yr ),
		MyRect( xcenter - xr, ycenter - yi, xr, yr ),
		MyRect( xcenter - xr, ycenter, xi, yr ),
		MyRect( xcenter - xr, ycenter + yi, xr, yi ),

		MyRect( xcenter - xi, ycenter, xr, yr ),
		MyRect( xcenter, ycenter + yi, xr, yi ),
		MyRect( xcenter, ycenter, xr, yr ),
		MyRect( xcenter, ycenter - yi, xr, yr ),
		MyRect( xcenter + xi, ycenter - yr, xi, yr ),
		MyRect( xcenter, ycenter - yr, xr, yi )
	};

	// Rasi coordinates for 2nd chart (only in double mode)
	const MyRect planet1_rect[12] = {
		MyRect( xcenter - xi, ycenter - ym, xr, xm ),
		MyRect( xcenter - xm, ycenter - ym, xi, ym ),
		MyRect( xcenter - xm, ycenter - yr, xr - xm, yr ),
		MyRect( xcenter - xm, ycenter - yi, xm, yr ),
		MyRect( xcenter - xm, ycenter, xr - xm, yr ),
		MyRect( xcenter - xr, ycenter + yi, xr, ym - yi ),

		MyRect( xcenter - xi, ycenter, xr, ym ),
		MyRect( xcenter + xi / 2, ycenter + yi, xi, ym - yi ),
		MyRect( xcenter + xi, ycenter, xm - xi, yr ),
		MyRect( xcenter, ycenter - yi, xm, yr ),
		MyRect( xcenter + xi, ycenter - ym, xm - xi, ym ),
		MyRect( xcenter + xi / 2, ycenter - ym, xi, ym - xi )
	};

	// Rasi coordinates for 2nd chart (only in double mode)
	const MyRect planet2_rect[12] = {
		MyRect( xcenter - xi, ycenter - yr, xr, yr - ym ),
		MyRect( xcenter - xr, ycenter - yr, xr, yr - ym ),
		MyRect( xcenter - xr, ycenter - yr, xr - xm, yr ),
		MyRect( xcenter - xr, ycenter - yi, xr - xm, yr ),
		MyRect( xcenter - xr, ycenter, xr - xm, yr ),
		MyRect( xcenter - xr, ycenter + ym, xr, yr - ym ),

		MyRect( xcenter - xi, ycenter + ym, xr, yr  - ym ),
		MyRect( xcenter, ycenter + ym, xr, yr - ym ),
		MyRect( xcenter + xm, ycenter, xr - xm, yr ),
		MyRect( xcenter + xm, ycenter - yi, xr - xm, yr ),
		MyRect( xcenter + xm, ycenter - yr, xr - xm, yr ),
		MyRect( xcenter, ycenter - yr, xr, yr - ym )
	};

	// Rectangles for sign names resp. numbers
	const MyRect sign_rect[12] = {
		MyRect( xcenter - rtol, ycenter - rtol, 2 * rtol, rtol ),
		MyRect( xcenter - xi - rtol / 2, ycenter - yi - rtol, rtol, rtol ),
		MyRect( xcenter - xi - rtol, ycenter - yi - rtol / 2, rtol, rtol ),
		MyRect( xcenter - 2 * rtol, ycenter - rtol, 2 * rtol, rtol ),
		MyRect( xcenter - xi - rtol, ycenter + yi - rtol / 2, rtol, rtol ),
		MyRect( xcenter - xi - rtol / 2, ycenter + yi, rtol, rtol ),

		MyRect( xcenter - rtol, ycenter, 2 * rtol, rtol ),
		MyRect( xcenter + xi - rtol / 2, ycenter + yi, rtol, rtol ),
		MyRect( xcenter + xi, ycenter + yi - rtol / 2, rtol, rtol ),
		MyRect( xcenter, ycenter - rtol, 2 * rtol, rtol ),
		MyRect( xcenter + xi, ycenter - yi - rtol / 2, rtol, rtol ),
		MyRect( xcenter + xi - rtol / 2, ycenter - yi - rtol, rtol, rtol )
	};

	// Text alignment for rasis
	const int rasi_alignment[12] = {
		Align::Center,
		Align::Top + Align::HCenter,
		Align::Left + Align::VCenter,
		Align::Center,
		Align::Left + Align::VCenter,
		Align::Bottom + Align::HCenter,

		Align::Center,
		Align::Bottom + Align::HCenter,
		Align::Right + Align::VCenter,
		Align::Center,
		Align::Right + Align::VCenter,
		Align::Top + Align::HCenter
	};

	// polygon coordinates for signs
	const MyPoint rasi_polygons[][12] = {
		{ MyPoint( xcenter-xi, ycenter - yi ), MyPoint( xcenter, ycenter ), MyPoint( xcenter+xi, ycenter - yi ), MyPoint( xcenter, ycenter - yr ) },
		{ MyPoint( xcenter-xr, ycenter - yr ), MyPoint( xcenter, ycenter-yr ), MyPoint( xcenter-xi, ycenter - yi ) },
		{ MyPoint( xcenter-xr, ycenter - yr ), MyPoint( xcenter-xr, ycenter ), MyPoint( xcenter-xi, ycenter - yi ) },
		{ MyPoint( xcenter-xr, ycenter ), MyPoint( xcenter-xi, ycenter+yi ), MyPoint( xcenter, ycenter ), MyPoint( xcenter-xi, ycenter - yi ) },
		{ MyPoint( xcenter-xr, ycenter + yr ), MyPoint( xcenter-xr, ycenter ), MyPoint( xcenter-xi, ycenter + yi ) },
		{ MyPoint( xcenter-xr, ycenter + yr ), MyPoint( xcenter, ycenter+yr ), MyPoint( xcenter-xi, ycenter + yi ) },

		{ MyPoint( xcenter-xi, ycenter+yi ), MyPoint( xcenter, ycenter+yr ), MyPoint( xcenter+xi, ycenter+yi ), MyPoint( xcenter, ycenter ) },
		{ MyPoint( xcenter, ycenter + yr ), MyPoint( xcenter+xr, ycenter+yr ), MyPoint( xcenter+xi, ycenter + yi ) },
		{ MyPoint( xcenter+xr, ycenter + yr ), MyPoint( xcenter+xr, ycenter ), MyPoint( xcenter+xi, ycenter + yi ) },
		{ MyPoint( xcenter, ycenter ), MyPoint( xcenter+xi, ycenter+yi ), MyPoint( xcenter+xr, ycenter ), MyPoint( xcenter+xi, ycenter-yi ) },
		{ MyPoint( xcenter+xr, ycenter - yr ), MyPoint( xcenter+xr, ycenter ), MyPoint( xcenter+xi, ycenter - yi ) },
		{ MyPoint( xcenter, ycenter - yr ), MyPoint( xcenter+xr, ycenter-yr ), MyPoint( xcenter+xi, ycenter - yi ) },
	};

	// rectangle for separation of double charts
	const MyRect transit_rect( xcenter - xm, ycenter - ym, 2 * xm, 2 * ym );

	// draw filled polygons
  GcZodiacalSigns *signs = &vconf->signs;
	if ( signs->needsBgPaint() )
	{
		painter->setTransparentPen();
		for ( i = ARIES; i <= PISCES; i++ )
		{
			painter->setBrush( signs->getBrush(redRasi( getAscendant() + i )));
			painter->drawPolygon( i % 3 ? 3 : 4, (MyPoint*)rasi_polygons[i] );
		}
	}

	paintOuterRectangle();

	painter->setPen( defaultPen );

	// diagonal from egde to egde paint one line or two depending on chart center
	if ( ! ( chartprops->getVedicGraphicStyle().centerInfoType == VGRAPHIC_CHART_CENTER_NOTHING ))
	{
		painter->drawLine( xcenter - xr, ycenter - yr, xcenter - mtol, ycenter - mtol );
		painter->drawLine( xcenter + mtol, ycenter + mtol, xcenter + xr, ycenter + yr );

		painter->drawLine( xcenter - xr, ycenter + yr, xcenter - mtol, ycenter + mtol );
		painter->drawLine( xcenter + mtol , ycenter - mtol, xcenter + xr, ycenter - yr );
	}
	else
	{
		painter->drawLine( xcenter - xr, ycenter - yr, xcenter + xr, ycenter + yr );
		painter->drawLine( xcenter - xr, ycenter + yr, xcenter + xr, ycenter - yr );
	}

	painter->drawLine( xcenter, ycenter + yr, xcenter + xr, ycenter );
	painter->drawLine( xcenter, ycenter + yr, xcenter - xr, ycenter );
	painter->drawLine( xcenter, ycenter - yr, xcenter - xr, ycenter );
	painter->drawLine( xcenter, ycenter - yr, xcenter + xr, ycenter );

	if ( chart_count == 2 ) painter->drawRectangle( transit_rect );

	/*********************************************************
	**  draw number of houses
	*********************************************************/
	if ( ! ( chartprops->getVedicGraphicStyle().northIndianSignDisplayType == VGRAPHIC_NORTH_INDIAN_SYMBOL )) painter->setGraphicFont( textzoom );
	else painter->setSymbolFont( symbolzoom );

	for ( i = ARIES; i <= PISCES; i++ )
	{
		current = redRasi( getAscendant() + i );
		switch( chartprops->getVedicGraphicStyle().northIndianSignDisplayType )
		{
			case VGRAPHIC_NORTH_INDIAN_ASC:
				s.Printf( wxT( "%d" ),  current + 1 );
			break;
			case VGRAPHIC_NORTH_INDIAN_NUMBER:
				s.Printf( wxT( "%d" ),  current + 1 );
			break;
			case VGRAPHIC_NORTH_INDIAN_SHORT:
			{
				s = lang.getSignName( current, TSHORT );
			}
			break;
			default:
				s = lang.getSignSymbolCode( current );
			break;
		}
		painter->drawSimpleText( sign_rect[i], s );

		// leave after ascendant for style == 0
		if ( chartprops->getVedicGraphicStyle().northIndianSignDisplayType == VGRAPHIC_NORTH_INDIAN_ASC ) break;
	}
	painter->setGraphicFont( textzoom );

	/*********************************************************
	**  draw planets
	*********************************************************/
	if ( ! chartprops->isBlank() )
	{

		for ( i = ARIES; i <= PISCES; i++ )
		{
			if ( chart_count == 1 ) drawFieldText( rasi_rect[i], redRasi( i + getAscendant() ), rasi_alignment[i], 0, text_border );
			else
			{
				drawFieldText( planet1_rect[i], redRasi( i + getAscendant() ), rasi_alignment[i], 0, text_border );
				drawFieldText( planet2_rect[i], redRasi( i + getAscendant() ), rasi_alignment[i], 1, text_border );
			}
		}

		paintCenterString();
	}
	painter->setGraphicFont( textzoom );
}

/*****************************************************
**
**   VedicRasiChart   ---   paintEast
**
******************************************************/
void VedicRasiChart::paintEast()
{
	int i;

	// inner square of chart
	const double xi = xr*0.28;
	const double yi = yr*0.28;

	// middle square for separation of 2 charts
	const double ri_prop = .4;
	const double xm = ri_prop * xi + ( 1 - ri_prop ) * xr;
	const double ym = ri_prop * yi + ( 1 - ri_prop ) * yr;

	const double text_border = xr / 30.0;

	// Rasi coordinates in single mode
	const MyRect rasi_rect[12] = {
		MyRect( xcenter - xi, ycenter - yr, 2 * xi, yr - yi ),
		MyRect( xcenter - xr, ycenter - yr, xr - xi, yr - yi ),
		MyRect( xcenter - xr, ycenter - yr, xr - xi, yr - yi ),
		MyRect( xcenter - xr, ycenter - yi, xr - xi, 2 * yi ),
		MyRect( xcenter - xr, ycenter + yi, xr - xi, yr - yi ),
		MyRect( xcenter - xr, ycenter + yi, xr - xi, yr - yi ),

		MyRect( xcenter - xi, ycenter + yi, 2 * xi, yr - yi ),
		MyRect( xcenter + xi, ycenter + yi, xr - xi, yr - yi ),
		MyRect( xcenter + xi, ycenter + yi, xr - xi, yr - yi ),
		MyRect( xcenter + xi, ycenter - yi, xr - xi, 2 * yi ),
		MyRect( xcenter + xi, ycenter - yr, xr - xi, yr - yi ),
		MyRect( xcenter + xi, ycenter - yr, xr - xi, yr - yi ),
	};

	// Rasi coordinates for 1st chart (only in double mode)
	const MyRect planet1_rect[12] = {
		MyRect( xcenter - xi, ycenter - ym, 2 * xi, yr - ym ),
		MyRect( xcenter - xm, ycenter - ym, xm - xi, ym - yi ),
		MyRect( xcenter - xm, ycenter - ym, xm - xi, ym - yi ),
		MyRect( xcenter - xm, ycenter - yi, xm - xi, 2 * yi ),
		MyRect( xcenter - xm, ycenter + yi, xm - xi, ym - yi ),
		MyRect( xcenter - xm, ycenter + yi, xm - xi, ym - yi ),

		MyRect( xcenter - xi, ycenter + yi, 2 * xi, ym - yi ),
		MyRect( xcenter + xi, ycenter + yi, xm - xi, ym - yi ),
		MyRect( xcenter + xi, ycenter + yi, xm - xi, ym - yi ),
		MyRect( xcenter + xi, ycenter - yi, xm - xi, 2 * yi ),
		MyRect( xcenter + xi, ycenter - ym, xm - xi, ym - yi ),
		MyRect( xcenter + xi, ycenter - ym, xm - xi, ym - yi )
	};

	// Rasi coordinates for 2nd chart (only in double mode)
	const MyRect planet2_rect[12] = {
		MyRect( xcenter - xi, ycenter - yr, 2 * xi, yr - ym ),
		MyRect( xcenter - xm, ycenter - yr, xm - xi, yr - ym ),
		MyRect( xcenter - xr, ycenter - yr, xr - xi, yr - yi ),
		MyRect( xcenter - xr, ycenter - yi, xr - xm, 2 * yi ),
		MyRect( xcenter - xr, ycenter + yi, xr - xm, yr - yi ),
		MyRect( xcenter - xm, ycenter + ym, xr - xm, yr - ym ),

		MyRect( xcenter - xi, ycenter + ym, 2 * xi, yr - ym ),
		MyRect( xcenter + xi, ycenter + ym, xr - xm, yr - ym ),
		MyRect( xcenter + xm, ycenter + yi, xr - xm, yr - yi ),
		MyRect( xcenter + xm, ycenter - yi, xr - xm, 2 * yi ),
		MyRect( xcenter + xm, ycenter - ym, xr - xm, yr - ym ),
		MyRect( xcenter + xi, ycenter - yr, xr - xi, yr - ym ),
	};

	// Text alignment for rasis
	const int rasi_alignment[12] = {
		Align::Center,
		Align::Right + Align::Top,
		Align::Left + Align::Bottom,
		Align::Center,
		Align::Left + Align::Top,
		Align::Right + Align::Bottom,

		Align::Center,
		Align::Left + Align::Bottom,
		Align::Right + Align::Top,
		Align::Center,
		Align::Right + Align::Bottom,
		Align::Left + Align::Top
	};

	// rectangle for separation of double charts
	const MyRect transit_rect( xcenter - xm, ycenter - ym, 2 * xm, 2 * ym );

	painter->setPen( defaultPen );

	// draw filled polygons resp. rectangles if required
  GcZodiacalSigns *signs = &vconf->signs;
	if ( signs->needsBgPaint() )
	{
		painter->setTransparentPen();
		int a = 0;

		painter->setBrush( signs->getBrush( a ));
		painter->drawRectangle( xcenter - xi, ycenter - yr, 2 * xi, yr - yi );

		painter->setBrush( signs->getBrush( a + 1 ));
		MyPoint ptaurus[3] = { MyPoint( xcenter - xr, ycenter - yr ), MyPoint( xcenter - xi, ycenter - yr ), MyPoint( xcenter-xi, ycenter - yi ) };
		painter->drawPolygon( 3, ptaurus );

		painter->setBrush( signs->getBrush( a + 2 ));
		MyPoint pgem[3] = { MyPoint( xcenter - xr, ycenter - yr ), MyPoint( xcenter-xr, ycenter - yi ), MyPoint( xcenter - xi, ycenter - yi ) };
		painter->drawPolygon( 3, pgem );

		painter->setBrush( signs->getBrush( a + 3 ));
		painter->drawRectangle( xcenter - xr, ycenter - yi, xr - xi, 2 * yi );

		painter->setBrush( signs->getBrush( a + 4 ));
		MyPoint pleo[3] = { MyPoint( xcenter - xr, ycenter + yi ), MyPoint( xcenter - xi, ycenter + yi ), MyPoint( xcenter - xr, ycenter + yr ) };
		painter->drawPolygon( 3, pleo );

		painter->setBrush( signs->getBrush( a + 5 ));
		MyPoint pvirgo[3] = { MyPoint( xcenter - xr, ycenter + yr ), MyPoint( xcenter - xi, ycenter + yr ),
			MyPoint( xcenter - xi, ycenter + yi ) };
		painter->drawPolygon( 3, pvirgo );

		painter->setBrush( signs->getBrush( a + 6 ));
		painter->drawRectangle( xcenter - xi, ycenter + yi, 2 * xi, yr - yi );

		painter->setBrush( signs->getBrush( a + 7 ));
		MyPoint pscorp[3] = { MyPoint( xcenter + xi, ycenter + yr ), MyPoint( xcenter + xr, ycenter + yr ),
		                      MyPoint( xcenter + xi, ycenter + yi ) };
		painter->drawPolygon( 3, pscorp );

		painter->setBrush( signs->getBrush( a + 8 ));
		MyPoint p2[3] = { MyPoint( xcenter + xi, ycenter + yi ), MyPoint( xcenter + xr, ycenter + yi ), MyPoint( xcenter + xr, ycenter + yr ) };
		painter->drawPolygon( 3, p2 );

		painter->setBrush( signs->getBrush( a + 9 ));
		painter->drawRectangle( xcenter + xi, ycenter - yi, xr - xi, 2 * yi );

		painter->setBrush( signs->getBrush( a + 10 ));
		MyPoint paqua[3] = { MyPoint( xcenter + xi, ycenter - yi ), MyPoint( xcenter + xr, ycenter - yi ), MyPoint( xcenter + xr, ycenter - yr ) };
		painter->drawPolygon( 3, paqua );

		painter->setBrush( signs->getBrush( a + 11 ));
		MyPoint ppisc[3] = { MyPoint( xcenter + xi, ycenter - yr ), MyPoint( xcenter + xr, ycenter - yr ),
			MyPoint( xcenter + xi, ycenter - yi ) };
		painter->drawPolygon( 3, ppisc );
	}

	paintOuterRectangle();

	painter->setPen( defaultPen );
	painter->drawLine( xcenter - xi, ycenter - yr, xcenter - xi, ycenter + yr );

	painter->drawLine( xcenter + xi, ycenter-yr, xcenter+xi, ycenter+yr );

	painter->drawLine( xcenter - xr, ycenter + yi, xcenter + xr, ycenter + yi );
	painter->drawLine( xcenter - xr, ycenter - yi, xcenter + xr, ycenter - yi );

	painter->drawLine( xcenter - xr, ycenter + yr, xcenter - xi, ycenter + yi );
	painter->drawLine( xcenter - xr, ycenter - yr, xcenter - xi, ycenter - yi );

	painter->drawLine( xcenter + xr, ycenter + yr, xcenter + xi, ycenter + yi );
	painter->drawLine( xcenter + xr, ycenter - yr, xcenter + xi, ycenter - yi );

	if ( chart_count == 2 ) painter->drawRectangle( transit_rect );

	painter->setBrush( defaultBrush );
	if ( ! chartprops->isBlank() )
	{
		for ( i = ARIES; i <= PISCES; i++ )
		{
			if ( chart_count == 1 ) drawFieldText( rasi_rect[i], i, rasi_alignment[i], 0, text_border );
			else
			{
				drawFieldText( planet1_rect[i], i, rasi_alignment[i], 0, text_border );
				drawFieldText( planet2_rect[i], i, rasi_alignment[i], 1, text_border );
			}
		}
		paintCenterString();
	}
	painter->setGraphicFont( textzoom );
}

/*****************************************************
**
**   VedicRasiChart   ---   paintCenterString
**
******************************************************/
void VedicRasiChart::paintCenterString()
{
	if ( chartprops->getVedicGraphicStyle().centerInfoType == VGRAPHIC_CHART_CENTER_NOTHING ) return;

	setDefaultTextColor();

	if ( centerstring_graphic ) painter->setSymbolFont( symbolzoom );
	else painter->setGraphicFont( textzoom );

	painter->drawSimpleText( MyRect( xcenter - xr, ycenter - yr, 2 * xr, 2 * yr ), getCenterString() );
}

