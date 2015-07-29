/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/base/VedicChartConfig.cpp
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
#pragma implementation "VedicChartConfig.h"
#endif

#include "VedicChartConfig.h"

#include "ColorConfig.h"
#include "Conf.h"
#include "FileConfig.h"
#include "mathbase.h"

#include <wx/xml/xml.h>

extern Config *config;

IMPLEMENT_CLASS( VedicChartConfig, ChartConfig )

/*****************************************************
**
**   VedicChartConfig   ---   Constructor
**
******************************************************/
VedicChartConfig::VedicChartConfig()
		: ChartConfig()
{
	ChartConfig::setDefaultConfig();
	setDefaultConfig();
}

/*****************************************************
**
**   VedicChartConfig   ---   dump
**
******************************************************/
wxString VedicChartConfig::dump()
{
	// TODO
	printf( "Not implemented: VedicChartConfig::dump\n" );
	wxString s;
	return s;
}

/*****************************************************
**
**   VedicChartConfig   ---   dumpXml
**
******************************************************/
wxString VedicChartConfig::dumpXml()
{
	// TODO
	printf( "Not implemented: VedicChartConfig::dumpXml\n" );
	wxString s;
	return s;
}

/*****************************************************
**
**   VedicChartConfig   ---   setDefaultConfig
**
******************************************************/
void VedicChartConfig::setDefaultConfig()
{
	ChartConfig::setDefaultConfig();

	useSymbols = true;
	markedFields.brush = wxBrush( config->colors->sbcMarkedFieldsBgColor, wxSOLID );
	/*
	markedFields.brush = new GcBrush();
	markedFields.brush->color = wxColour( config->colors->sbcMarkedFieldsBgColor );
	markedFields.brush->style = wxSOLID;
	*/
}

/*****************************************************
**
**   VedicChartConfig   ---   toString
**
******************************************************/
wxString VedicChartConfig::toString()
{
	wxString s;
	s << wxT( "VedicChart: " ) << name << Endl
	<< wxT( "  Description: " ) << description << Endl
	<< wxT( "  useTextColors: " ) << useTextColors << Endl

	<< wxT( "  useSignColors: " ) << useSignColors << Endl
	<< wxT( "  useHouseColors: " ) << useHouseColors << Endl
	<< wxT( "  useSymbols: " ) << useSymbols << Endl;
	return s;
}

VedicChartConfigLoader *VedicChartConfigLoader::ego = 0;

/*****************************************************
**
**   VedicChartConfigLoader   ---   Constructor
**
******************************************************/
VedicChartConfigLoader::VedicChartConfigLoader()
 : ChartConfigLoader( XML_TOPIC_VEDIC_CHART )
{
}

/*****************************************************
**
**   VedicChartConfigLoader   ---   getConfig
**
******************************************************/
VedicChartConfig *VedicChartConfigLoader::getConfig( const int &i )
{
	return (VedicChartConfig*)getConfigInternal( i );
}

/*****************************************************
**
**   VedicChartConfigLoader   ---   createConfig
**
******************************************************/
BaseXmlConfig *VedicChartConfigLoader::createConfig( BaseXmlConfig* base )
{
	if ( base )
	{
		VedicChartConfig *cfg = (VedicChartConfig*) base;
		return new VedicChartConfig( *cfg );
	}
	else return new VedicChartConfig;
}

/*****************************************************
**
**   VedicChartConfigLoader   ---   readSpecificAttributes
**
******************************************************/
void VedicChartConfigLoader::readSpecificAttributes( BaseXmlConfig *cfg, wxXmlNode *cur )
{
	VedicChartConfig *vconf = (VedicChartConfig*)cfg;
	wxString key;

	while ( cur )
	{
		key = cur->GetName();

		if ( key == XML_NODE_USE_TEXT_COLORS ) XMLNODE2BOOLEAN( cur->GetNodeContent(), vconf->useTextColors )
		else if ( ! key.CmpNoCase( XML_NODE_WIDGET_BACKGROUND )) parseWidgetBackground( cur, vconf->widgetBackground );
		else if ( ! key.CmpNoCase( XML_NODE_PEN )) vconf->pen = parsePen( cur );
		else if ( ! key.CmpNoCase( XML_NODE_BRUSH )) vconf->brush = parseBrush( cur );
		else if ( ! key.CmpNoCase( XML_NODE_MARKED_FIELDS )) parseRegion( cur, vconf->markedFields );
		else if ( ! key.CmpNoCase( XML_NODE_ZODIACAL_SIGN )) parseZodiacalSigns( cur, vconf->signs );

		else if ( ! key.CmpNoCase( XML_NODE_OUTER_RECTANGLE )) parseRectangle( cur, vconf->outerRectangle );

		else if ( ! key.CmpNoCase( XML_NODE_USE_SIGN_COLORS )) XMLNODE2BOOLEAN( cur->GetNodeContent(), vconf->useSignColors )
		else if ( ! key.CmpNoCase( XML_NODE_USE_HOUSE_COLORS )) XMLNODE2BOOLEAN( cur->GetNodeContent(), vconf->useHouseColors )
		else if ( ! key.CmpNoCase( XML_NODE_USE_SYMBOLS )) XMLNODE2BOOLEAN( cur->GetNodeContent(), vconf->useSymbols )

		else reportWrongKey( key );
		cur = cur->GetNext();
	}
}


