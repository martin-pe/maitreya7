/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/base/WesternChartConfig.cpp
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
#pragma implementation "WesternChartConfig.h"
#endif

#include "WesternChartConfig.h"

#include "Conf.h"
#include "GraphConfig.h"
#include "mathbase.h"

#include <wx/brush.h>
#include <wx/xml/xml.h>

extern Config *config;

IMPLEMENT_CLASS( WesternChartConfig, ChartConfig )

/*****************************************************
**
**   WesternChartConfig   ---   Constructor
**
******************************************************/
WesternChartConfig::WesternChartConfig()
		: ChartConfig()
{
	ChartConfig::setDefaultConfig();
	setDefaultConfig();
}

/*****************************************************
**
**   WesternChartConfig   ---   dump
**
******************************************************/
wxString WesternChartConfig::dump()
{
	// TODO
	printf( "Not implemented: WesternChartConfig::dump\n" );
	wxString s;
	return s;
}

/*****************************************************
**
**   WesternChartConfig   ---   dumpXml
**
******************************************************/
wxString WesternChartConfig::dumpXml()
{
	// TODO
	printf( "Not implemented: WesternChartConfig::dumpXml\n" );
	wxString s;
	return s;
}

/*****************************************************
**
**   WesternChartConfig   ---   setDefaultConfig
**
******************************************************/
void WesternChartConfig::setDefaultConfig()
{
	ChartConfig::setDefaultConfig();

	zodiac.radius        = 79.0;
	zodiac1Degree.radius = 81.0;
	zodiac1Degree.show   = false;
	zodiac5Degree.radius = 83.0;
	zodiac5Degree.show   = false;
	zodiacSymbols.radius = 86;
	zodiacSymbols.show   = false;
	outerZodiac.radius   = 92.0;

	innerHouse.radius = 50.0;
	outerHouse.radius = 79.0;

	planets.radius = 73.0;
	aspects.radius = 50.0;

	innerTransitZodiac.radius  = 50.0;
	innerTransitZodiac1Degree.radius = 52.0;
	innerTransitZodiac5Degree.radius = 54.0;
	innerTransitPlanets.radius = 58.0;

	outerTransitZodiac.radius  = 96.0;
	outerTransitZodiac1Degree.radius = 98.0;
	outerTransitZodiac5Degree.radius = 100.0;
	outerTransitPlanets.radius = 106.0;

	planetMarkerLength = 2;
}

WesternChartConfigLoader *WesternChartConfigLoader::ego = 0;

/*****************************************************
**
**   WesternChartConfigLoader   ---   Constructor
**
******************************************************/
WesternChartConfigLoader::WesternChartConfigLoader()
 : ChartConfigLoader( XML_TOPIC_WESTERN_CHART )
{
}

/*****************************************************
**
**   WesternChartConfigLoader   ---   getConfig
**
******************************************************/
WesternChartConfig *WesternChartConfigLoader::getConfig( const int &i )
{
	return (WesternChartConfig*)getConfigInternal( i );
}

/*****************************************************
**
**   WesternChartConfigLoader   ---   createConfig
**
******************************************************/
BaseXmlConfig *WesternChartConfigLoader::createConfig( BaseXmlConfig* base )
{
	if ( base )
	{
		WesternChartConfig *cfg = (WesternChartConfig*) base;
		return new WesternChartConfig( *cfg );
	}
	else return new WesternChartConfig;
}

/*****************************************************
**
**   WesternChartConfigLoader   ---   readSpecificAttributes
**
******************************************************/
void WesternChartConfigLoader::readSpecificAttributes( BaseXmlConfig *cfg, wxXmlNode *cur )
{
	WesternChartConfig *wconf = (WesternChartConfig*)cfg;
	wxString key;

	while ( cur )
	{
		key = cur->GetName();
		key = key.Trim();
		if ( ! key.CmpNoCase( XML_NODE_USE_TEXT_COLORS )) XMLNODE2BOOLEAN( cur->GetNodeContent(), wconf->useTextColors )

		else if ( ! key.CmpNoCase( XML_NODE_WIDGET_BACKGROUND )) parseWidgetBackground( cur, wconf->widgetBackground );
		else if ( ! key.CmpNoCase( XML_NODE_PEN )) wconf->pen = parsePen( cur );
		else if ( ! key.CmpNoCase( XML_NODE_BRUSH )) wconf->brush = parseBrush( cur );
		else if ( ! key.CmpNoCase( XML_NODE_ANGLES )) parseArrow( cur, wconf->angles );

		else if ( ! key.CmpNoCase( XML_NODE_ZODIAC )) parseChartFrame( cur, wconf->zodiac );
		else if ( ! key.CmpNoCase( XML_NODE_ZODIAC_1_DEGREE )) parseChartFrame( cur, wconf->zodiac1Degree );
		else if ( ! key.CmpNoCase( XML_NODE_ZODIAC_5_DEGREE )) parseChartFrame( cur, wconf->zodiac5Degree );
		else if ( ! key.CmpNoCase( XML_NODE_ZODIAC_SYMBOLS )) parseChartFrame( cur, wconf->zodiacSymbols );
		else if ( ! key.CmpNoCase( XML_NODE_OUTER_ZODIAC )) parseChartFrame( cur, wconf->outerZodiac );

		else if ( ! key.CmpNoCase( XML_NODE_INNER_HOUSE )) parseChartFrame( cur, wconf->innerHouse );
		else if ( ! key.CmpNoCase( XML_NODE_OUTER_HOUSE )) parseChartFrame( cur, wconf->outerHouse );

		else if ( ! key.CmpNoCase( XML_NODE_PLANETS )) parseChartFrame( cur, wconf->planets );
		else if ( ! key.CmpNoCase( XML_NODE_ASPECTS )) parseChartFrame( cur, wconf->aspects );

		else if ( ! key.CmpNoCase( XML_NODE_INNER_TRANSIT_ZODIAC )) parseChartFrame( cur, wconf->innerTransitZodiac );
		else if ( ! key.CmpNoCase( XML_NODE_INNER_TRANSIT_ZODIAC_1_DEGREE )) parseChartFrame( cur, wconf->innerTransitZodiac1Degree );
		else if ( ! key.CmpNoCase( XML_NODE_INNER_TRANSIT_ZODIAC_5_DEGREE )) parseChartFrame( cur, wconf->innerTransitZodiac5Degree );
		else if ( ! key.CmpNoCase( XML_NODE_INNER_TRANSIT_PLANETS )) parseChartFrame( cur, wconf->innerTransitPlanets );

		else if ( ! key.CmpNoCase( XML_NODE_OUTER_TRANSIT_ZODIAC )) parseChartFrame( cur, wconf->outerTransitZodiac );
		else if ( ! key.CmpNoCase( XML_NODE_OUTER_TRANSIT_ZODIAC_1_DEGREE )) parseChartFrame( cur, wconf->outerTransitZodiac1Degree );
		else if ( ! key.CmpNoCase( XML_NODE_OUTER_TRANSIT_ZODIAC_5_DEGREE )) parseChartFrame( cur, wconf->outerTransitZodiac5Degree );
		else if ( ! key.CmpNoCase( XML_NODE_OUTER_TRANSIT_PLANETS )) parseChartFrame( cur, wconf->outerTransitPlanets );

		else if ( ! key.CmpNoCase( XML_NODE_ZODIACAL_SIGN )) parseZodiacalSigns( cur, wconf->signs );
		else if ( ! key.CmpNoCase( XML_NODE_HOUSES )) parseHouses( cur, wconf->houses );

		else if ( ! key.CmpNoCase( XML_NODE_FRAME_TYPE )) wconf->frameType = parseFrameType( cur->GetNodeContent());

		else if ( ! key.CmpNoCase( XML_NODE_PLANET_MARKER_LENGTH ))
			wconf->planetMarkerLength = myatof( cur->GetNodeContent());

		else reportWrongKey( key );
		cur = cur->GetNext();
	}
}

