/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/WesternChart.cpp
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
#pragma implementation "WesternChart.h"
#endif

#include "WesternChart.h"

#include <math.h>
#include <wx/brush.h>
#include <wx/image.h>

#include "ArabicParts.h"
#include "Aspect.h"
#include "ChartProperties.h"
#include "ColorConfig.h"
#include "Conf.h"
#include "constants.h"
#include "GraphConfig.h"
#include "guibase.h"
#include "Horoscope.h"
#include "IconProvider.h"
#include "mathbase.h"
#include "Lang.h"
#include "Painter.h"
#include "PlanetList.h"
#include "WesternChartConfig.h"
#include "Writer.h"

extern Config *config;

/*****************************************************
**
**   WesternChart   ---   Constructor
**
******************************************************/
WesternChart::WesternChart( const Horoscope *horoscope, const Horoscope* htransit, const ChartProperties *chartprops )
		: GraphicalChart( chartprops ),
		horoscope( horoscope ),
		htransit( htransit )
{
	assert( chartprops );
	aexpert = new AspectExpert( horoscope );
	transitmode = ( htransit != horoscope );
	writer = WriterFactory().getWriter( WRITER_TEXT );
}

/*****************************************************
**
**   WesternChart   ---   Destructor
**
******************************************************/
WesternChart::~WesternChart()
{
	delete aexpert;
	delete writer;
}

/*****************************************************
**
**   WesternChart   ---   setTransitChart
**
******************************************************/
void WesternChart::setTransitChart( Horoscope *h )
{
	htransit = h;
	transitmode = ( htransit != horoscope );
}

/*****************************************************
**
**   WesternChart   ---   paintInternal
**
******************************************************/
void WesternChart::paintInternal( ChartConfig *cfg )
{
	assert( cfg );
	assert( cfg->GetClassInfo()->IsKindOf( CLASSINFO( WesternChartConfig )));
	wconf = (WesternChartConfig*)cfg;

	if ( horoscope == 0 )
	{
		printf( "Horosocpe NULL, return\n" );
		return;
	}
	if ( htransit == 0 )
	{
		printf( "HTransit NULL, return\n" );
		return;
	}

	// reduce radius if second chart has planets outside outerZodiac
	// transit style 0 = outside, 1 = inside
	if ( transitmode && chartprops->getWesternGraphicStyle().transitStyle == 0 )
	{
		rmax *= .9;
		rxmax *= .9;
		rymax *= .9;
		transitsoutside =  true;
	}
	else
	{
		transitsoutside =  false;
	}

	// scalingfactor for house numbers
	textzoom *= .6;

	// TODO
	psize = rmax/20;

	switch( chartprops->getWesternChartOrientation())
	{
		// Aries 0
		case 1:
			aries = 180;
		break;

		// Aries 15
		case 2:
			aries = 165;
		break;

		// Cancer 0
		case 3:
			aries = 90;
		break;

		// Cancer 15
		case 4:
			aries = 75;
		break;

		// Libra 0
		case 5:
			aries = 0;
		break;

		// Libra 15
		case 6:
			aries = 345;
		break;

		// Capricorn 0
		case 7:
			aries = 270;
		break;

		// Capricorn 15
		case 8:
			aries = 255;
		break;

		// Ascendant on left side
		case 0:
		default:
			if ( horoscope )
			{
				aries = red_deg( 180 - horoscope->getHouse( HOUSE1, false ) );
			}
			else aries = 0;
		break;
	}
	
	paintChartFrame( wconf->outerZodiac );
	if ( ! chartprops->isBlank())
	{
		if ( chartprops->getWesternGraphicStyle().showHouses ) paintHouses();
	}
	paintZodiac();
	if ( ! chartprops->isBlank())
	{
		if ( chartprops->getWesternGraphicStyle().showHouses ) paintHouseBordersAndAngles();
		if ( chartprops->getWesternGraphicStyle() .showAspects ) paintAspects();
		paintAllPlanets( horoscope );
		if ( transitmode )
		{
			paintAllPlanets( htransit );
		}
	}
}

/*****************************************************
**
**   WesternChart   ---   OnDataChanged
**
******************************************************/
void WesternChart::OnDataChanged()
{
	if ( htransit && horoscope ) aexpert->update( htransit, chartprops );
}

/*****************************************************
**
**   WesternChart   ---   paintSingleAspect
**
******************************************************/
void WesternChart::paintSingleAspect( const double &len1, const double &len2, const int &type )
{
	double glen1, glen2;
	wxString s;
	const double lpart1 = .2;
	const double lpart2 = .3;

	glen1 = red_deg( aries + len1 );
	MyPoint p1 = frameBorderPointToXy( glen1, wconf->aspects );

	glen2 = red_deg( aries + len2 );
	MyPoint p2 = frameBorderPointToXy( glen2, wconf->aspects );

	if ( chartprops->getWesternGraphicStyle().showAspectSymbols && type != 0 )
	{
		painter->drawLine( p1.x, p1.y, p1.x + lpart1 * ( p2.x - p1.x ), p1.y + lpart1 * ( p2.y - p1.y ));
		painter->drawLine( p1.x + lpart2 * ( p2.x - p1.x ), p1.y + lpart2 * ( p2.y - p1.y ), p2.x, p2.y );

		MyRect rect( p1.x + lpart1 * ( p2.x - p1.x ), p1.y + lpart1 * ( p2.y - p1.y ), ( lpart2 - lpart1 ) * ( p2.x - p1.x ),
			( lpart2 - lpart1 ) * ( p2.y - p1.y ));
		s = AspectExpert::getAspectSymbol( type );
		if ( s.IsEmpty())
		{
			painter->setGraphicFont( textzoom );
			painter->drawTextFormatted( rect, AspectExpert::getAspectShortDescription( type ), Align::Center );
		}
		else
		{
			painter->setSymbolFont( symbolzoom );
			painter->drawTextFormatted( rect, s, Align::Center );
		}
	}
	else
	{
		painter->drawLine( p1, p2 );
	}
	painter->setGraphicFont( textzoom );
}

/*****************************************************
**
**   WesternChart   ---   paintAspects
**
******************************************************/
void WesternChart::paintAspects()
{
	assert( htransit );
	assert( horoscope );

	ObjectPosition pos1, pos2;
	list<AspectEvent>::iterator iter;
	const Horoscope *hbase = transitmode ? htransit : horoscope;
	AspectConfigLoader *loader = AspectConfigLoader::get();
	AspectConfig *cfg;
	lastaspect = -1;
	wxColour color;

	paintChartFrame( wconf->aspects );
	painter->setPen( defaultPen );
	painter->setTextColor( defaultPen.GetColour());

	list<AspectEvent> al = aexpert->getAspectList();
	for ( iter = al.begin(); iter != al.end(); iter++ )
	{
		paintLengthMarker( aries + hbase->getWesternLength( (*iter).planet1 ), wconf->aspects.radius );
		paintLengthMarker( aries + horoscope->getWesternLength( (*iter).planet2 ), wconf->aspects.radius );
	}

	color = config->colors->fgColor;
	for ( iter = al.begin(); iter != al.end(); iter++ )
	{
		cfg = loader->getAspectConfig( (*iter).aspectType );
		if ( chartprops->getWesternGraphicStyle().showAspectColors )
		{
			color = cfg->color;
		}
		painter->setPen( wxPen( color, cfg->width, convertInt2WxPen( cfg->style )));
		painter->setTextColor( color );
		/*
		if ( chartprops->getWesternGraphicStyle().showAspectColors )
		{
			painter->setPen( wxPen( color, cfg->width, convertInt2WxPen( cfg->style )));
			painter->setTextColor( color );
			painter->setPen( wxPen( cfg->color, cfg->width, convertInt2WxPen( cfg->style )));
			painter->setTextColor( cfg->color );
		}
			*/

		paintSingleAspect( hbase->getWesternLength( (*iter).planet1 ),
			horoscope->getWesternLength( (*iter).planet2 ), (*iter).aspectType );
	}
}

/*****************************************************
**
**   WesternChart   ---   paintHouses
**
******************************************************/
void WesternChart::paintHouses()
{
	double len, houselen;
	MyPoint p;
	wxString s;
	Lang lang;
	int i;
	double h0, h1;

	bool showColors = chartprops->getWesternGraphicStyle().showHouseColors;

	// house backgrounds
	//if ( showColors && wconf->houses.needsBgPaint() )
	if ( wconf->houses.needsBgPaint() )
	{
		painter->setTransparentPen();
		for ( i = 0; i < 12; i++ )
		{
			painter->setBrush( wconf->houses.getBrush( i ));
			h0 = aries + horoscope->getHouse( i, false );
			h1 = aries + horoscope->getHouse(  i < 11 ? i + 1: HOUSE1, false );
			if ( h1 < h0 ) h1 += 360;
			drawChartFrameField( h0, h1, wconf->innerHouse, wconf->outerHouse );
		}
	}

	painter->setPen( defaultPen );
	painter->setGraphicFont( textzoom );
	paintChartFrame( wconf->innerHouse );

	// House numbers: leave them if transits are on inner house circle
	bool paintHousenumbers = wconf->houses.paintHouseNumbers;
	if ( paintHousenumbers )
	{
		if ( transitmode && ! transitsoutside ) paintHousenumbers = false;
	}
	if ( paintHousenumbers )
	{
		wxColour textColor;
		if ( ! showColors )
		{
			setDefaultTextColor();
		}

		GcChartFrame frame( 1.05 * wconf->innerHouse.radius );
		for ( int i = HOUSE1; i <= HOUSE12; i++ )
		{
			houselen = red_deg( horoscope->getHouse( i == HOUSE12 ? HOUSE1 : i + 1, false ) - horoscope->getHouse( i, false ));
			len = red_deg( aries + horoscope->getHouse( i, false ) + .5 * houselen );
			s.Printf( wxT( "%d" ), i+1 );

			p = frameBorderPointToXy( len, frame );
			if ( showColors )
			{
				painter->setTextColor( wconf->houses.getTextColor(i));
			}
			painter->drawTextFormatted( MyRect( p.x-psize/2, p.y-psize/2, psize, psize), s, Align::Center );
		}
	}
}

/*****************************************************
**
**   WesternChart   ---   paintHouseBordersAndAngles
**
******************************************************/
void WesternChart::paintHouseBordersAndAngles()
{
	double len;
	MyPoint p;
	const int degOffset = 3;
	Lang lang;

	if ( ! transitsoutside )
	{
		paintArrow( red_deg( aries + horoscope->getHouse( HOUSE1, false )), wconf->angles );
		len = red_deg( degOffset + aries + horoscope->getHouse( HOUSE1, false ));
		p = frameBorderPointToXy( len, wconf->angles.outerFrame.radius );

		painter->setSymbolFont( symbolzoom );
		if ( chartprops->getWesternGraphicStyle().showPlanetColors )
			painter->setTextColor( wconf->getPlanetColor( OASCENDANT ) );

		painter->drawTextFormatted( MyRect( p.x-psize/2, p.y-psize/2, psize, psize),
			lang.getPlanetSymbolCode( OASCENDANT), Align::Center );

		paintArrow( red_deg( aries + horoscope->getHouse( HOUSE10, false )), wconf->angles );
		len = red_deg( degOffset + aries + horoscope->getHouse( HOUSE10, false ));
		p = frameBorderPointToXy( len, wconf->angles.outerFrame );

		if ( chartprops->getWesternGraphicStyle().showPlanetColors )
			painter->setTextColor( wconf->getPlanetColor( OMERIDIAN ) );

		painter->drawTextFormatted( MyRect( p.x-psize/2, p.y-psize/2, psize, psize),
			lang.getPlanetSymbolCode( OMERIDIAN), Align::Center );
	}

	painter->setPen( defaultPen );
	for ( int i = HOUSE1; i <= HOUSE12; i++ )
	{
		if ( ! transitsoutside && ( i == HOUSE1 || i == HOUSE4 || i == HOUSE7 || i == HOUSE10 )) continue;
		len = horoscope->getHouse( i, false );
		paintMarkerLine( aries + len, wconf->innerHouse, wconf->outerHouse );
	}
}

/*****************************************************
**
**   WesternChart   ---   getPRadius
**
******************************************************/
double WesternChart::getPRadius( const int &planet, const Horoscope *h )
{
	unsigned int i;
	int p;
	const int factor = 1;
	double ret = rmax * wconf->planets.radius / 100;
	if ( transitmode && h == htransit )
	{
		if ( chartprops->getWesternGraphicStyle().transitStyle > 0 )
			ret = rmax * wconf->innerTransitPlanets.radius / 100;
		else
			ret = rmax * wconf->outerTransitPlanets.radius / 100;
	}

	double len, dist;
	double mylen = h->getWesternLength( planet );

	for ( i = 0; i < chartprops->getPlanetList().size(); i++ )
	{
		p = chartprops->getPlanetList()[i];
		if ( p >= planet ) continue;
		if (( p == OASCENDANT || p == OMERIDIAN ) && ( chartprops->getWesternGraphicStyle().showHouses )) continue;
		len = h->getWesternLength( p );
		dist = ::fabs( mylen - len );
		if ( dist > 300 ) dist = fabs( dist - 360 );
		if ( dist < 5 )
		{
			if ( h == htransit ) ret -= Min( factor * psize, factor * psize * ( 5 - dist)/5 );
			else ret += Min( factor * psize, factor * psize * ( 5 - dist)/5 );
		}
	}
	return ret;
}

/*****************************************************
**
**   WesternChart   ---   getCoordinatesForPlanet
**
******************************************************/
MyPoint WesternChart::getCoordinatesForPlanet( const int &planet, const Horoscope *h )
{
	double len = h->getWesternLength( planet );
	double glen = red_deg( aries + len );
	double r = getPRadius( planet, h );
	return frameBorderPointToXy( glen, GcChartFrame( 100 * r / rmax ));
}

/*****************************************************
**
**   WesternChart   ---   paintLengthMarker
**
******************************************************/
void WesternChart::paintLengthMarker( const double &alfa, const GcChartFrame &frame )
{
	paintMarkerLine( alfa, GcChartFrame( frame.radius - wconf->planetMarkerLength ),
		GcChartFrame( frame.radius + wconf->planetMarkerLength ));
}

/*****************************************************
**
**   WesternChart   ---   paintAllPlanets
**
******************************************************/
void WesternChart::paintAllPlanets( const Horoscope *h )
{
	int planet, cindex, align;
	Lang lang;
	double ff = 15;
	wxString psymbol;
	MyPoint point;

	painter->setPen( defaultPen );
	painter->setTextColor( defaultPen.GetColour());

	vector<int> obs = chartprops->getWesternPlanetList();
	for ( unsigned int i = 0; i < chartprops->getWesternPlanetList().size(); i++ )
	{
		setDefaultTextColor();
		planet = obs[i];
		if (( planet == OASCENDANT || planet == OMERIDIAN )
			&& ( chartprops->getWesternGraphicStyle().showHouses ) && ! transitsoutside ) continue;

		// mark planet positions on zodiac
		GcChartFrame frame;
		if ( transitmode && h == htransit )
		{
			if ( transitsoutside ) frame = wconf->outerTransitZodiac;
			else frame = wconf->innerTransitZodiac;
		}
		else
		{
			frame = wconf->zodiac;
		}

		paintLengthMarker( aries + h->getWesternLength( planet ), frame );
		point = getCoordinatesForPlanet( planet, h );

		MyRect rr( point.x-ff, point.y-ff, 2*ff, 2*ff);

		if ( chartprops->getWesternGraphicStyle().showPlanetColors )
		{
			cindex = planet;
			if ( planet == OASCENDANT ) cindex = 0;
			else if ( planet == OMERIDIAN ) cindex = 1;
			painter->setTextColor( cindex <= MAX_EPHEM_OBJECTS ? wconf->getPlanetColor( cindex ) : defaultPen.GetColour());
		}

		psymbol = lang.getPlanetSymbolCode( planet );
		if ( psymbol.IsEmpty() )
		{
			painter->setGraphicFont( textzoom );
			align = Align::Center;
			painter->drawTextFormatted( rr, writer->getObjectName( planet, TLARGE ), align );
		}
		else
		{
			painter->setSymbolFont( symbolzoom );
			painter->drawTextFormatted( rr, psymbol, Align::Center );
		}

		// Retrogression
		if (( chartprops->getWesternGraphicStyle().showRetro ) && h->isRetrograde( planet ))
		{
			painter->setSymbolFont( symbolzoom );

			align = Align::Right+Align::Bottom;
			painter->drawTextFormatted( MyRect( point.x, point.y+psize/2, psize, psize), wxT( "_" ), align );
		}
	}
	painter->setGraphicFont( textzoom );
}

/*****************************************************
**
**   WesternChart   ---   paintZodiac
**
******************************************************/
void WesternChart::paintZodiac()
{
	int i;
	double a;
	MyPoint signpos;
	GcZodiacalSigns *signs = &wconf->signs;

	bool showColors = chartprops->getWesternGraphicStyle().showSignColors;

	// Sign background, if required
	a = aries;
	if ( signs->needsBgPaint() )
	{
		for ( i = 0; i < 12; i++ )
		{
			painter->setBrush( signs->getBrush( i ));
			drawChartFrameField( aries + i * 30, aries + (i + 1 ) * 30, wconf->zodiac, wconf->outerZodiac );
		}
	}

	if ( ! showColors )
	{
		setDefaultTextColor();
	}

	if ( signs->symbolType != 2 ) painter->setSymbolFont( symbolzoom * signs->symbolZoomFactor / 100 );
	a = aries + 15;
	for ( i = 0; i < 12; i++ )
	{
		signpos = frameBorderPointToXy( a, wconf->zodiacSymbols );
		if ( chartprops->getWesternGraphicStyle().showSignColors )
		{
			if ( showColors )
			{
				painter->setTextColor( signs->getTextColor(i));
			}
		}
		painter->drawSignSymbol( signpos.x, signpos.y, i, signs->symbolType, signs->symbolZoomFactor, signs->rotateSymbols ? a - 90 : 0 );
		a += 30.0;
	}

	paintChartFrame( wconf->zodiac );
	painter->setPen( defaultPen );
	a = aries;
	for ( i = 0; i < 12; i++ )
	{
		paintMarkerLine( a, wconf->zodiac, wconf->outerZodiac );
		a += 30.0;
	}

	a = aries;
	for ( i = 1; i < 72; i++ ) {
		if ( i % 6 )
		{
			paintMarkerLine( a + i * 5.0, wconf->zodiac, wconf->zodiac5Degree );
		}
	}
	for ( i = 1; i < 360; i++ ) {
		if ( i % 5 )
		{
			paintMarkerLine( a + i, wconf->zodiac, wconf->zodiac1Degree );
		}
	}
	paintChartFrame( wconf->zodiac1Degree );
	paintChartFrame( wconf->zodiac5Degree );

	const int tstyle = chartprops->getWesternGraphicStyle().transitStyle;
	if ( transitmode )
	{
		paintChartFrame( tstyle > 0 ? wconf->innerTransitZodiac : wconf->outerTransitZodiac );
		for ( i = 1; i < 72; i++ )
		{
			if ( tstyle > 0 )
				paintMarkerLine( a + i * 5.0, wconf->innerTransitZodiac5Degree, wconf->innerTransitZodiac );
			else
				paintMarkerLine( a + i * 5.0, wconf->outerTransitZodiac5Degree, wconf->outerTransitZodiac );
		}
		for ( i = 1; i < 360; i++ )
		{
			if ( i % 5 )
			{
				if ( tstyle > 0 )
					paintMarkerLine( a + i, wconf->innerTransitZodiac1Degree, wconf->innerTransitZodiac );
				else
					paintMarkerLine( a + i, wconf->outerTransitZodiac1Degree, wconf->outerTransitZodiac );
			}
		}
	}
	paintChartFrame( wconf->outerHouse );
}

