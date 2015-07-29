/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/base/ChartConfig.cpp
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
#pragma implementation "ChartConfig.h"
#endif

#include "ChartConfig.h"
#include "ColorConfig.h"
#include "Conf.h"
#include "IconProvider.h"
#include "mathbase.h"

#include <wx/image.h>
#include <wx/tokenzr.h>
#include <wx/xml/xml.h>

extern Config *config;

IMPLEMENT_CLASS( ChartConfig, BaseXmlConfig )

/*****************************************************
**
**   ChartConfig   ---   Constructor
**
******************************************************/
ChartConfig::ChartConfig()
{
	name = wxT( "Default" );
	frameType = CHART_FRAME_TYPE_CIRCLE;
}

/*****************************************************
**
**   ChartConfig   ---   setDefaultConfig
**
******************************************************/
void ChartConfig::setDefaultConfig()
{
	signs.symbolZoomFactor = 100;
	signs.rotateSymbols = false;
	signs.symbolType = 0;

	useTextColors = true;
	useSignColors = false;
	useHouseColors = false;
}

/*****************************************************
**
**   ChartConfig   ---   getPlanetColor
**
******************************************************/
wxColour &ChartConfig::getPlanetColor( const int &i )
{
	// TODO
	return config->colors->getPlanetColor( i );
	/*
	if ( planetColor.size() == 0 ) return config->colors->getPlanetColor( i );
	else return planetColor[i % planetColor.size()];
	*/
}

/*****************************************************
**
**   ChartConfigLoader   ---   Constructor
**
******************************************************/
ChartConfigLoader::ChartConfigLoader( const int &topic )
 : XmlConfigLoader( topic )
{
}

/*****************************************************
**
**   ChartConfigLoader   ---   parseFrameType
**
******************************************************/
int ChartConfigLoader::parseFrameType( wxString key )
{
	int type = CHART_FRAME_TYPE_CIRCLE;
	if ( ! key.CmpNoCase( XML_NODE_CIRCLE)) type = CHART_FRAME_TYPE_CIRCLE;
	else if ( ! key.CmpNoCase( XML_NODE_ELLIPSE)) type = CHART_FRAME_TYPE_ELLIPSE;
	else if ( ! key.CmpNoCase( XML_NODE_SQUARE)) type = CHART_FRAME_TYPE_SQUARE;
	else if ( ! key.CmpNoCase( XML_NODE_RECTANGLE)) type = CHART_FRAME_TYPE_RECTANGLE;
	return type;
}

/*****************************************************
**
**   ChartConfigLoader   ---   parseAlign
**
******************************************************/
int ChartConfigLoader::parseAlign( wxString key )
{
	int align = Align::Center;
	if ( ! key.CmpNoCase( XML_NODE_ALIGN_LEFT_TOP)) align = Align::Left + Align::Top;
	else if ( ! key.CmpNoCase( XML_NODE_ALIGN_LEFT_BOTTOM )) align = Align::Left + Align::Bottom;
	else if ( ! key.CmpNoCase( XML_NODE_ALIGN_LEFT_CENTER )) align = Align::Left + Align::VCenter;
	else if ( ! key.CmpNoCase( XML_NODE_ALIGN_RIGHT_TOP )) align = Align::Right + Align::Top;
	else if ( ! key.CmpNoCase( XML_NODE_ALIGN_RIGHT_BOTTOM )) align = Align::Right + Align::Bottom;
	else if ( ! key.CmpNoCase( XML_NODE_ALIGN_RIGHT_CENTER )) align = Align::Right + Align::VCenter;
	else if ( ! key.CmpNoCase( XML_NODE_ALIGN_TOP_CENTER )) align = Align::Top + Align::HCenter;
	else if ( ! key.CmpNoCase( XML_NODE_ALIGN_BOTTOM_CENTER )) align = Align::Bottom + Align::HCenter;
	else if ( ! key.CmpNoCase( XML_NODE_ALIGN_CENTER )) align = Align::Center;
	else reportWrongKey( key );

	return align;
}

/*****************************************************
**
**   ChartConfigLoader   ---   parseImage
**
******************************************************/
GcImage *ChartConfigLoader::parseImage( wxXmlNode *cur )
{
	wxString key;
	GcImage *image = new GcImage;
	cur = cur->GetChildren();
	while ( cur )
	{
		key = cur->GetName();
		if ( ! key.CmpNoCase( XML_NODE_INPUTFILE ))
		{
			image->filename = cur->GetNodeContent();
		}
		else if ( ! key.CmpNoCase( XML_NODE_MIRROR_X )) XMLNODE2BOOLEAN( cur->GetNodeContent(), image->mirrorX )
		else if ( ! key.CmpNoCase( XML_NODE_MIRROR_Y )) XMLNODE2BOOLEAN( cur->GetNodeContent(), image->mirrorY )
		else if ( ! key.CmpNoCase( XML_NODE_ALIGN )) image->align = parseAlign( cur->GetNodeContent());
		else reportWrongKey( key );
		cur = cur->GetNext();
	}
	return image;
}

/*****************************************************
**
**   ChartConfigLoader   ---   parseWidgetBackground
**
******************************************************/
void ChartConfigLoader::parseWidgetBackground( wxXmlNode *cur, GcWidgetBackground &bg )
{
	wxString key;
	cur = cur->GetChildren();
	while ( cur )
	{
		key = cur->GetName();
		if ( ! key.CmpNoCase( XML_NODE_BRUSH )) bg.brush = parseBrush( cur );
		else if ( ! key.CmpNoCase( XML_NODE_PEN )) bg.pen = parsePen( cur );
		else if ( ! key.CmpNoCase( XML_NODE_IMAGE )) bg.images.push_back( *parseImage( cur ));
		else reportWrongKey( key );
		cur = cur->GetNext();
	}
}

/*****************************************************
**
**   ChartConfigLoader   ---   parseStyle
**
******************************************************/
int ChartConfigLoader::parseStyle( wxString key )
{
	int style = wxSOLID;
	if ( ! key.CmpNoCase( XML_NODE_STYLE_SOLID )) style = wxSOLID;
	else if ( ! key.CmpNoCase( XML_NODE_STYLE_TRANSPARENT )) style = wxTRANSPARENT;
	else if ( ! key.CmpNoCase( XML_NODE_STYLE_DOT )) style = wxDOT;
	else if ( ! key.CmpNoCase( XML_NODE_STYLE_LONG_DASH )) style = wxLONG_DASH;
	else if ( ! key.CmpNoCase( XML_NODE_STYLE_SHORT_DASH )) style = wxSHORT_DASH;
	else if ( ! key.CmpNoCase( XML_NODE_STYLE_DOT_DASH )) style = wxDOT_DASH;
	else if ( ! key.CmpNoCase( XML_NODE_STYLE_STIPPLE )) style = wxSTIPPLE;
	else if ( ! key.CmpNoCase( XML_NODE_STYLE_USER_DASH )) style = wxUSER_DASH;
	else if ( ! key.CmpNoCase( XML_NODE_STYLE_BDIAGONAL_HATCH )) style = wxBDIAGONAL_HATCH;
	else if ( ! key.CmpNoCase( XML_NODE_STYLE_CROSSDIAG_HATCH )) style = wxCROSSDIAG_HATCH;
	else if ( ! key.CmpNoCase( XML_NODE_STYLE_FDIAGONAL_HATCH )) style = wxFDIAGONAL_HATCH;
	else if ( ! key.CmpNoCase( XML_NODE_STYLE_CROSS_HATCH )) style = wxCROSS_HATCH;
	else if ( ! key.CmpNoCase( XML_NODE_STYLE_HORIZONTAL_HATCH )) style = wxHORIZONTAL_HATCH;
	else if ( ! key.CmpNoCase( XML_NODE_STYLE_VERTICAL_HATCH )) style = wxVERTICAL_HATCH;
	else reportWrongKey( key );

	return style;
}

/*****************************************************
**
**   ChartConfigLoader   ---   parsePen
**
******************************************************/
wxPen ChartConfigLoader::parsePen( wxXmlNode *cur )
{
	wxString key;
	wxColour color = config->colors->fgColor;
	int width = 1;
	int style = wxSOLID;

	cur = cur->GetChildren();
	while ( cur )
	{
		key = cur->GetName();
		if ( ! key.CmpNoCase( XML_NODE_COLOR )) color = wxColour( cur->GetNodeContent());
		else if ( ! key.CmpNoCase( XML_NODE_LINEWIDTH )) width = (int)myatof( cur->GetNodeContent());
		else if ( ! key.CmpNoCase( XML_NODE_STYLE )) style = (int)parseStyle( cur->GetNodeContent());
		else reportWrongKey( key );
		cur = cur->GetNext();
	}
	return wxPen( color, width, style );
}

/*****************************************************
**
**   ChartConfigLoader   ---   parsePenList
**
******************************************************/
void ChartConfigLoader::parsePenList( wxXmlNode *cur, vector<wxPen> &list )
{
	wxString key;
	cur = cur->GetChildren();
	while ( cur )
	{
		key = cur->GetName();
		if ( ! key.CmpNoCase( XML_NODE_PEN )) list.push_back( parsePen( cur ));
		else reportWrongKey( key );
		cur = cur->GetNext();
	}
}

/*****************************************************
**
**   ChartConfigLoader   ---   parseBrush
**
******************************************************/
wxBrush ChartConfigLoader::parseBrush( wxXmlNode *cur )
{
	wxString key;
	wxColour color;
	wxBitmap *bitmap = 0;
	int style = wxSOLID;
	int rotateHue = 0;

	cur = cur->GetChildren();
	while ( cur )
	{
		key = cur->GetName();
		if ( ! key.CmpNoCase( XML_NODE_COLOR ))
		{
			color = wxColour( cur->GetNodeContent());
			if ( ! color.IsOk() ) printf( "WARN ChartConfigLoader::parseBrush: color for %s is not okay\n", str2char( cur->GetNodeContent()));
		}
		else if ( ! key.CmpNoCase( XML_NODE_STYLE )) style = parseStyle( cur->GetNodeContent());
		else if ( ! key.CmpNoCase( XML_NODE_INPUTFILE ))
		{
			bitmap = IconProvider::getBitmap( cur->GetNodeContent());
			if ( bitmap == 0 ) printf( "WARN ChartConfigLoader::parseBrush: bitmap for file %s is null\n", str2char( cur->GetNodeContent()));
		}
		else if ( ! key.CmpNoCase( XML_NODE_ROTATE_HUE )) rotateHue =  (int)myatof( cur->GetNodeContent());
		else reportWrongKey( key );
		cur = cur->GetNext();
	}
	if ( bitmap )
	{
		if ( rotateHue != 0 )
		{
      wxImage image( bitmap->ConvertToImage() );
      image.RotateHue( rotateHue / 360.0 );
			return wxBrush( wxBitmap( image ));
		}
		return wxBrush( *bitmap );
	}
	else if ( ! color.IsOk() ) return *wxTRANSPARENT_BRUSH;
	return wxBrush( color, style );
}

/*****************************************************
**
**   ChartConfigLoader   ---   parseBrushList
**
******************************************************/
void ChartConfigLoader::parseBrushList( wxXmlNode *cur, vector<wxBrush> &bl )
{
	wxString key;
	cur = cur->GetChildren();
	while ( cur )
	{
		key = cur->GetName();
		if ( ! key.CmpNoCase( XML_NODE_BRUSH )) bl.push_back( parseBrush( cur ));
		else reportWrongKey( key );
		cur = cur->GetNext();
	}
}

/*****************************************************
**
**   ChartConfigLoader   ---   parseColorList
**
******************************************************/
void ChartConfigLoader::parseColorList( wxXmlNode *cur, vector<wxColour> &cl )
{
	wxString key;
	cur = cur->GetChildren();
	while ( cur )
	{
		key = cur->GetName();

		if ( ! key.CmpNoCase( XML_NODE_COLOR )) cl.push_back( wxColour( cur->GetNodeContent()));
		else reportWrongKey( key );
		cur = cur->GetNext();
	}
}

/*****************************************************
**
**   ChartConfigLoader   ---   parseCircle
**
******************************************************/
void ChartConfigLoader::parseCircle( wxXmlNode *cur, GcCircle &circle )
{
	wxString key;
	cur = cur->GetChildren();
	while ( cur )
	{
		key = cur->GetName();
		if ( ! key.CmpNoCase( XML_NODE_BRUSH )) circle.brush = parseBrush( cur );
		else if ( ! key.CmpNoCase( XML_NODE_PEN )) circle.pen = parsePen( cur );
		else if ( ! key.CmpNoCase( XML_NODE_RADIUS )) XMLNODE2INT( cur->GetNodeContent(), circle.radius )
		else if ( ! key.CmpNoCase( XML_NODE_SHOW )) XMLNODE2BOOLEAN( cur->GetNodeContent(), circle.show )
		else if ( ! key.CmpNoCase( XML_NODE_RECTANGLE )) XMLNODE2BOOLEAN( cur->GetNodeContent(), circle.isRectangle )
		else reportWrongKey( key );
		cur = cur->GetNext();
	}
}

/*****************************************************
**
**   ChartConfigLoader   ---   parseChartFrame
**
******************************************************/
void ChartConfigLoader::parseChartFrame( wxXmlNode *cur, GcChartFrame &frame )
{
	wxString key;
	cur = cur->GetChildren();
	while ( cur )
	{
		key = cur->GetName();
		if ( ! key.CmpNoCase( XML_NODE_BRUSH )) frame.brush = parseBrush( cur );
		else if ( ! key.CmpNoCase( XML_NODE_PEN )) frame.pen = parsePen( cur );
		else if ( ! key.CmpNoCase( XML_NODE_RADIUS )) XMLNODE2INT( cur->GetNodeContent(), frame.radius )
		else if ( ! key.CmpNoCase( XML_NODE_SHOW )) XMLNODE2BOOLEAN( cur->GetNodeContent(), frame.show )
		else if ( ! key.CmpNoCase( XML_NODE_FRAME_TYPE )) frame.frameType = parseFrameType( cur->GetNodeContent());
		else reportWrongKey( key );
		cur = cur->GetNext();
	}
}

/*****************************************************
**
**   ChartConfigLoader   ---   parseRectangle
**
******************************************************/
void ChartConfigLoader::parseRectangle( wxXmlNode *cur, GcRectangle &rectangle )
{
	wxString key;
	cur = cur->GetChildren();
	while ( cur )
	{
		key = cur->GetName();
		if ( ! key.CmpNoCase( XML_NODE_BRUSH )) rectangle.brush = parseBrush( cur );
		else if ( ! key.CmpNoCase( XML_NODE_PEN )) rectangle.pen = parsePen( cur );
		else if ( ! key.CmpNoCase( XML_NODE_DOUBLE_OUTER_LINE )) XMLNODE2BOOLEAN( cur->GetNodeContent(), rectangle.doubleOuterLine )
		else if ( ! key.CmpNoCase( XML_NODE_RADIUS )) rectangle.radius = myatof( cur->GetNodeContent());
		else if ( ! key.CmpNoCase( XML_NODE_SHOW )) XMLNODE2BOOLEAN( cur->GetNodeContent(), rectangle.show )
		else reportWrongKey( key );
		cur = cur->GetNext();
	}
}

/*****************************************************
**
**   ChartConfigLoader   ---   parseArrow
**
******************************************************/
void ChartConfigLoader::parseArrow( wxXmlNode *cur, GcArrow &arrow )
{
	wxString key;
	cur = cur->GetChildren();
	while ( cur )
	{
		key = cur->GetName();
		if ( ! key.CmpNoCase( XML_NODE_PEN )) arrow.pen = parsePen( cur );
		else if ( ! key.CmpNoCase( XML_NODE_INNER_CIRCLE )) parseChartFrame( cur, arrow.innerFrame );
		else if ( ! key.CmpNoCase( XML_NODE_OUTER_CIRCLE )) parseChartFrame( cur, arrow.outerFrame );
		else reportWrongKey( key );
		cur = cur->GetNext();
	}
}

/*****************************************************
**
**   ChartConfigLoader   ---   parseRegion
**
******************************************************/
void ChartConfigLoader::parseRegion( wxXmlNode *cur, GcRegion &region )
{
	wxString key;
	cur = cur->GetChildren();
	while ( cur )
	{
		key = cur->GetName();
		if ( ! key.CmpNoCase( XML_NODE_BRUSH )) region.brush = parseBrush( cur );
		else if ( ! key.CmpNoCase( XML_NODE_PEN )) region.pen = parsePen( cur );
		else if ( ! key.CmpNoCase( XML_NODE_SHOW )) XMLNODE2BOOLEAN( cur->GetNodeContent(), region.show )
		else if ( ! key.CmpNoCase( XML_NODE_TEXT_COLOR )) region.textColor = wxColour( cur->GetNodeContent());
		else reportWrongKey( key );
		cur = cur->GetNext();
	}
}

/*****************************************************
**
**   ChartConfigLoader   ---   parseZodiacalSigns
**
******************************************************/
void ChartConfigLoader::parseZodiacalSigns( wxXmlNode *cur, GcZodiacalSigns &signs )
{
	wxString key;
	cur = cur->GetChildren();
	while ( cur )
	{
		key = cur->GetName();
		if ( ! key.CmpNoCase( XML_NODE_SYMBOL_ZOOM_FACTOR )) XMLNODE2INT( cur->GetNodeContent(), signs.symbolZoomFactor )
		else if ( ! key.CmpNoCase( XML_NODE_SYMBOL_TYPE )) XMLNODE2INT( cur->GetNodeContent(), signs.symbolType )
		else if ( ! key.CmpNoCase( XML_NODE_ROTATE_SYMBOLS )) XMLNODE2BOOLEAN( cur->GetNodeContent(), signs.rotateSymbols )
		else if ( ! key.CmpNoCase( XML_NODE_GLOBAL_BRUSH )) signs.globalBrush = parseBrush( cur );
		else if ( ! key.CmpNoCase( XML_NODE_BRUSH_SEQUENCE )) parseBrushList( cur, signs.brushList );
		else if ( ! key.CmpNoCase( XML_NODE_GLOBAL_PEN )) signs.globalPen = parsePen( cur );
		else if ( ! key.CmpNoCase( XML_NODE_PEN_SEQUENCE )) parsePenList( cur, signs.penList );
		else if ( ! key.CmpNoCase( XML_NODE_GLOBAL_TEXT_COLOR )) signs.globalTextColor = wxColour( cur->GetNodeContent());
		else if ( ! key.CmpNoCase( XML_NODE_USE_STOCK_BRUSH_LIST)) XMLNODE2BOOLEAN( cur->GetNodeContent(), signs.useStockBrushList )
		else reportWrongKey( key );

		cur = cur->GetNext();
	}
}

/*****************************************************
**
**   ChartConfigLoader   ---   parseHouses
**
******************************************************/
void ChartConfigLoader::parseHouses( wxXmlNode *cur, GcHouses &houses )
{
	wxString key;
	cur = cur->GetChildren();
	while ( cur )
	{
		key = cur->GetName();
		if ( ! key.CmpNoCase( XML_NODE_USE_STOCK_BRUSH_LIST)) XMLNODE2BOOLEAN( cur->GetNodeContent(), houses.useStockBrushList )
		else if ( ! key.CmpNoCase( XML_NODE_GLOBAL_BRUSH )) houses.globalBrush = parseBrush( cur );
		else if ( ! key.CmpNoCase( XML_NODE_BRUSH_SEQUENCE )) parseBrushList( cur, houses.brushList );
		else if ( ! key.CmpNoCase( XML_NODE_GLOBAL_PEN )) houses.globalPen = parsePen( cur );
		else if ( ! key.CmpNoCase( XML_NODE_PEN_SEQUENCE )) parsePenList( cur, houses.penList );

		// TODO
		//else if ( ! key.CmpNoCase( XML_NODE_TEXT_SEQUENCE )) parseTextList( cur, housess.textList );
		else if ( ! key.CmpNoCase( XML_NODE_GLOBAL_TEXT_COLOR )) houses.globalTextColor = wxColour( cur->GetNodeContent());
		else if ( ! key.CmpNoCase( XML_NODE_PAINT_HOUSE_NUMBERS)) XMLNODE2BOOLEAN( cur->GetNodeContent(), houses.paintHouseNumbers )
		else reportWrongKey( key );
		cur = cur->GetNext();
	}
}



