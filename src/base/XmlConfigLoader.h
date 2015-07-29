/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/base/XmlConfigLoader.h
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

#ifndef _XMLCONFIGLOADER_H_
#define _XMLCONFIGLOADER_H_

#ifdef __GNUG__
#pragma interface "XmlConfigLoader.h"
#endif

#include <wx/colour.h>
#include <wx/hashmap.h>

#include <vector>

WX_DECLARE_STRING_HASH_MAP( int, FileModTimeHash );

class wxBitmap;
class wxXmlNode;

using namespace std;

#define RELOAD_ON_FILE_CHANGE

// Xml topics
#define MAX_XML_TOPICS 5

enum {
	XML_TOPIC_MULTIPLE_VIEWS = 0,
	XML_TOPIC_WESTERN_CHART,
	XML_TOPIC_VEDIC_CHART,
	XML_TOPIC_PRINTOUT,
	XML_TOPIC_YOGA
};

// Root names for view containers (multiple views)
#define XML_ROOT_MULTIPLE_VIEWS wxT( "MultipleViewConfigs" )
#define XML_ROOT_VEDIC_CHART wxT( "VedicChartConfigs" )
#define XML_ROOT_WESTERN_CHART wxT( "WesternChartConfigs" )
#define XML_ROOT_PRINTOUT wxT( "PrintoutConfigs" )
#define XML_NODE_CONFIG wxT( "Config" )

// Root names for Yogas
#define XML_ROOT_YOGA wxT( "YogaConfigs" )
#define XML_NODE_YOGA wxT( "YogaConfig" )
#define XML_NODE_YOGA_GROUP wxT( "YogaConfigGroup" )

// Various element node names
#define XML_NODE_DESCRIPTION wxT( "Description" )
#define XML_NODE_GROUP wxT( "Group" )
#define XML_NODE_EXTENDS wxT( "Extends" )
#define XML_NODE_SOURCE wxT( "Source" )
#define XML_NODE_EFFECT wxT( "Effect" )
#define XML_NODE_NAME wxT( "Name" )
#define XML_NODE_SHORTNAME wxT( "ShortName" )
#define XML_NODE_RULESTR wxT( "Rule" )
#define XML_NODE_ALLOW_HIGHER_VARGAS wxT( "AllowHigherVargas" )

#define XML_NODE_PLANET_COLOR wxT( "PlanetColor" )
#define XML_NODE_SIGN_COLOR wxT( "SignColor" )
#define XML_NODE_SIGN_BGCOLOR wxT( "SignBgColor" )
#define XML_NODE_HOUSE_COLORS wxT( "HouseColors" )
#define XML_NODE_HOUSE_COLOR wxT( "HouseColor" )
#define XML_NODE_HOUSE_BGCOLOR wxT( "HouseBgColor" )
#define XML_NODE_MARKED_FIELDS wxT( "MarkedFields" )

#define XML_NODE_FG_COLOR wxT( "FgColor" )
#define XML_NODE_BG_COLOR wxT( "BgColor" )
#define XML_NODE_TEXT_COLOR wxT( "TextColor" )
#define XML_NODE_GLOBAL_TEXT_COLOR wxT( "GlobalTextColor" )
#define XML_NODE_USE_TEXT_COLORS wxT( "UseTextColors" )
#define XML_NODE_ASPECT_COLOR wxT( "AspectColor" )

#define XML_NODE_DO_FLOOD_FILL wxT( "DoFloodFill" )
#define XML_NODE_BG_IMAGE wxT( "BgImage" )

#define XML_NODE_BRUSH wxT( "Brush" )
#define XML_NODE_GLOBAL_BRUSH wxT( "GlobalBrush" )
#define XML_NODE_BRUSH_SEQUENCE wxT( "BrushSequence" )
#define XML_NODE_PEN wxT( "Pen" )
#define XML_NODE_GLOBAL_PEN wxT( "GlobalPen" )
#define XML_NODE_PEN_SEQUENCE wxT( "PenSequence" )
#define XML_NODE_STYLE wxT( "Style" )
#define XML_NODE_IMAGE wxT( "Image" )
#define XML_NODE_INPUTFILE wxT( "Inputfile" )
#define XML_NODE_ROTATE_HUE wxT( "RotateHue" )
#define XML_NODE_LINEWIDTH wxT( "Linewidth" )
#define XML_NODE_ALIGN wxT( "Align" )
#define XML_NODE_ALIGN_LEFT_TOP wxT( "LeftTop" )
#define XML_NODE_ALIGN_LEFT_BOTTOM wxT( "LeftBottom" )
#define XML_NODE_ALIGN_LEFT_CENTER wxT( "LeftCenter" )
#define XML_NODE_ALIGN_RIGHT_TOP wxT( "RightTop" )
#define XML_NODE_ALIGN_RIGHT_BOTTOM wxT( "RightBottom" )
#define XML_NODE_ALIGN_RIGHT_CENTER wxT( "RightCenter" )
#define XML_NODE_ALIGN_TOP_CENTER wxT( "TopCenter" )
#define XML_NODE_ALIGN_BOTTOM_CENTER wxT( "BottomCenter" )
#define XML_NODE_ALIGN_CENTER wxT( "Center" )
#define XML_NODE_MIRROR_X wxT( "MirrorX" )
#define XML_NODE_MIRROR_Y wxT( "MirrorY" )

#define XML_NODE_WIDGET_BACKGROUND wxT( "WidgetBackground" )
#define XML_NODE_COLOR wxT( "Color" )
#define XML_NODE_CIRCLE wxT( "Circle" )
#define XML_NODE_RADIUS wxT( "Radius" )
#define XML_NODE_RECTANGLE wxT( "Rectangle" )
#define XML_NODE_SHOW wxT( "Show" )

#define XML_NODE_FRAME_TYPE wxT( "FrameType" )
#define XML_NODE_CIRCLE wxT( "Circle" )
#define XML_NODE_ELLIPSE wxT( "Ellipse" )
#define XML_NODE_SQUARE wxT( "Square" )
#define XML_NODE_RECTANGLE wxT( "Rectangle" )

#define XML_NODE_ANGLES wxT( "Angles" )
#define XML_NODE_INNER_CIRCLE wxT( "InnerCircle" )
#define XML_NODE_OUTER_CIRCLE wxT( "OuterCircle" )

// Brush and pen styles
#define XML_NODE_STYLE_SOLID wxT( "wxSOLID" )
#define XML_NODE_STYLE_TRANSPARENT wxT( "wxTRANSPARENT" )
#define XML_NODE_STYLE_DOT wxT( "wxDOT" )
#define XML_NODE_STYLE_LONG_DASH wxT( "wxLONG_DASH" )
#define XML_NODE_STYLE_SHORT_DASH wxT( "wxSHORT_DASH" )
#define XML_NODE_STYLE_DOT_DASH wxT( "wxDOT_DASH" )
#define XML_NODE_STYLE_STIPPLE wxT( "wxSTIPPLE" )
#define XML_NODE_STYLE_USER_DASH wxT( "wxUSER_DASH" )
#define XML_NODE_STYLE_BDIAGONAL_HATCH wxT( "wxBDIAGONAL_HATCH" )
#define XML_NODE_STYLE_CROSSDIAG_HATCH wxT( "wxCROSSDIAG_HATCH" )
#define XML_NODE_STYLE_FDIAGONAL_HATCH wxT( "wxFDIAGONAL_HATCH" )
#define XML_NODE_STYLE_CROSS_HATCH wxT( "wxCROSS_HATCH" )
#define XML_NODE_STYLE_HORIZONTAL_HATCH wxT( "wxHORIZONTAL_HATCH" )
#define XML_NODE_STYLE_VERTICAL_HATCH wxT( "wxVERTICAL_HATCH" )

#define XML_NODE_ZODIAC wxT( "Zodiac" )
#define XML_NODE_ZODIAC_1_DEGREE wxT( "Zodiac1Degree" )
#define XML_NODE_ZODIAC_5_DEGREE wxT( "Zodiac5Degree" )
#define XML_NODE_ZODIAC_SYMBOLS wxT( "ZodiacSymbols" )
#define XML_NODE_OUTER_ZODIAC wxT( "OuterZodiac" )

#define XML_NODE_INNER_HOUSE wxT( "InnerHouse" )
#define XML_NODE_OUTER_HOUSE wxT( "OuterHouse" )

#define XML_NODE_PLANETS wxT( "Planets" )
#define XML_NODE_ASPECTS wxT( "Aspects" )

#define XML_NODE_OUTER_ANGLE_MARKER wxT( "OuterAngleMarker" )
#define XML_NODE_INNER_ANGLE_MARKER wxT( "InnerAngleMarker" )
#define XML_NODE_PLANET_MARKER_LENGTH wxT( "PlanetMarkerLength" )

#define XML_NODE_ZODIACAL_SIGN wxT( "ZodiacalSigns" )
#define XML_NODE_SYMBOL_ZOOM_FACTOR wxT( "SymbolZoomFactor" )
#define XML_NODE_ROTATE_SYMBOLS wxT( "RotateSymbols" )
#define XML_NODE_SYMBOL_TYPE wxT( "SymbolType" )

#define XML_NODE_HOUSES wxT( "Houses" )
#define XML_NODE_USE_STOCK_BRUSH_LIST wxT( "UseStockBrushList" )

#define XML_NODE_INNER_TRANSIT_ZODIAC wxT( "InnerTransitZodiac" )
#define XML_NODE_INNER_TRANSIT_ZODIAC_1_DEGREE wxT( "InnerTransitZodiac1Degree" )
#define XML_NODE_INNER_TRANSIT_ZODIAC_5_DEGREE wxT( "InnerTransitZodiac5Degree" )
#define XML_NODE_INNER_TRANSIT_PLANETS wxT( "InnerTransitPlanets" )

#define XML_NODE_OUTER_TRANSIT_ZODIAC wxT( "OuterTransitZodiac" )
#define XML_NODE_OUTER_TRANSIT_ZODIAC_1_DEGREE wxT( "OuterTransitZodiac1Degree" )
#define XML_NODE_OUTER_TRANSIT_ZODIAC_5_DEGREE wxT( "OuterTransitZodiac5Degree" )
#define XML_NODE_OUTER_TRANSIT_PLANETS wxT( "OuterTransitPlanets" )

#define XML_NODE_PAINT1_DEG_CIRCLE wxT( "Paint1DegCircle" )
#define XML_NODE_PAINT5_DEG_CIRCLE wxT( "Paint5DegCircle" )
#define XML_NODE_PAINT_OUTER_HOUSE_CIRCLE wxT( "PaintOuterHouseCircle" )
#define XML_NODE_PAINT_INNER_HOUSE_CIRCLE wxT( "PaintInnerHouseCircle" )
#define XML_NODE_PAINT_HOUSE_NUMBERS wxT( "PaintHouseNumbers" )

#define XML_NODE_OUTER_RECTANGLE wxT( "OuterRectangle" )

#define XML_NODE_USE_SIGN_COLORS wxT( "UseSignColors" )
#define XML_NODE_USE_HOUSE_COLORS wxT( "UseHouseColors" )
#define XML_NODE_DOUBLE_OUTER_LINE wxT( "DoubleOuterLine" )
#define XML_NODE_USE_SYMBOLS wxT( "UseSymbols" )

#define XML_NODE_WINDOW_NODE wxT( "Window" )
#define XML_NODE_WINDOW1 wxT( "Window1" )
#define XML_NODE_WINDOW2 wxT( "Window2" )
#define XML_NODE_WIDGET_NODE wxT( "Widget" )
#define XML_NODE_SPLITTER_NODE wxT( "Splitter" )
#define XML_NODE_PAGE_NODE wxT( "Page" )
#define XML_NODE_NOTEBOOK_NODE wxT( "Notebook" )
#define XML_NODE_VIEW_TYPE wxT( "ViewType" )
#define XML_NODE_TEXTMODE wxT( "TextMode" )
#define XML_NODE_VARGA wxT( "Varga" )
#define XML_NODE_SUB_SHOWTOOLBAR wxT( "ShowToolbar" )
#define XML_NODE_VEDIC wxT( "Vedic" )
#define XML_NODE_FALSE wxT( "false" )
#define XML_NODE_TRUE wxT( "true" )
#define XML_NODE_ROW_COUNT wxT( "RowCount" )
#define XML_NODE_COL_COUNT wxT( "ColCount" )
#define XML_NODE_VARGA_LIST wxT( "VargaList" )
#define XML_NODE_VARGA_LIST_ITEM wxT( "VargaItem" )

#define XML_NODE_SPLIT_DIRECTION wxT( "SplitDirection" )
#define XML_NODE_SPLIT_HORIZONTAL wxT( "Horizontal" )
#define XML_NODE_SPLIT_VERTICAL wxT( "Vertical" )
#define XML_NODE_SASH_POS wxT( "SashPos" )
#define XML_NODE_SASH_POS_PERCENT wxT( "SashPosPercent" )

// Printout
#define XML_NODE_PAGES wxT( "Pages" )
#define XML_NODE_ITEM wxT( "Item" )
#define XML_NODE_STYLE wxT( "Style" )
#define XML_NODE_DASA_TYPE wxT( "DasaType" )
#define XML_PRINTOUT_TYPE_TITLE wxT( "Title" )
#define XML_PRINTOUT_TYPE_BASE_DATA wxT( "ChartData" )
#define XML_PRINTOUT_TYPE_VEDIC_CHART_PAIR wxT( "VedicChartPair" )
#define XML_PRINTOUT_TYPE_WESTERN_CHART    wxT( "WesternChart" )
#define XML_PRINTOUT_TYPE_DASA wxT( "Dasa" )
#define XML_PRINTOUT_TYPE_ASHTAKAVARGA wxT( "Ashtakavarga" )
#define XML_PRINTOUT_TYPE_SBC wxT( "Sarvatobhadra" )
#define XML_PRINTOUT_TYPE_URANIAN wxT( "Uranian" )
#define XML_PRINTOUT_TYPE_ASPECTARIUM wxT( "Aspectarium" )
#define XML_PRINTOUT_TYPE_EPHEM wxT( "Ephemeris" )
#define XML_NODE_VARGA1 wxT( "Varga1" )
#define XML_NODE_VARGA2 wxT( "Varga2" )

#define XMLNODE2INT( k, v ) { long l; v = ( k.ToLong( &l ) ? (int)l : 0 ); }
#define XMLNODE2BOOLEAN( k, v ) { v = ( k.Lower() == wxT( "true" )); }
#define XMLNODE2DOUBLE( k, v ) { v = myatof( k ); }

/*************************************************//**
*
* 
*
******************************************************/
class BaseXmlConfig : public wxObject
{
	DECLARE_CLASS( BaseXmlConfig )
public:

	BaseXmlConfig();

	virtual wxString dump() = 0;
	virtual wxString dumpXml() = 0;

	wxString name;
	wxString description;
};

/*************************************************//**
*
*
*
******************************************************/
class XmlConfigLoader
{
public:

	bool needsReload();

	bool dumpFile( wxString filename );

	bool saveFile( wxString name, vector<BaseXmlConfig*> &v );

	vector<BaseXmlConfig*> &getConfigs();

	int getCount() { return (int)configs.size(); }

	bool parseFile( wxString filenname,  vector<BaseXmlConfig*>& );

protected:

	XmlConfigLoader( const int &topic );
	virtual ~XmlConfigLoader() {}

	BaseXmlConfig *getConfigInternal( const int& );

	virtual BaseXmlConfig *createConfig( BaseXmlConfig* = 0 ) = 0;

	virtual void readSpecificAttributes( BaseXmlConfig*, wxXmlNode* ) = 0;

	BaseXmlConfig *copyConfig( wxString name );

	void readConfig();

	void clearConfigs();

	void initConfigs();

	vector <BaseXmlConfig*> configs;

	void reportWrongKey( wxString );

	void reportWrongRootNode( wxString );

	class wxXmlDocument *doc;

	FileModTimeHash modtime;

private:

	const int topic;
};


#endif

