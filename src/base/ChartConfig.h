/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/base/ChartConfig.h
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

#ifndef _CHARTCONFIG_H_
#define _CHARTCONFIG_H_

#ifdef __GNUG__
#pragma interface "ChartConfig.h"
#endif

#include "GraphConfig.h"

#include "XmlConfigLoader.h"

#include <wx/colour.h>

#include <vector>

class wxBitmap;

using namespace std;

/*************************************************//**
*
* \brief common class for chart configuration (western and Vedic)
*
******************************************************/
class ChartConfig : public BaseXmlConfig
{
	DECLARE_CLASS( ChartConfig )

public:
	ChartConfig();

	virtual void setDefaultConfig();

	int frameType;

	GcWidgetBackground widgetBackground;
	GcZodiacalSigns signs;

	wxPen pen;
	wxBrush brush;

	bool useTextColors;
	bool useHouseColors;
	bool useSignColors;

	wxColour &getPlanetColor( const int &i );

protected:

};

/*************************************************//**
*
*
*
******************************************************/
class ChartConfigLoader : public XmlConfigLoader
{
protected:

	ChartConfigLoader( const int &topic );

	GcImage *parseImage( wxXmlNode* );

	void parseWidgetBackground( wxXmlNode*, GcWidgetBackground& );

	int parseAlign( wxString );

	int parseFrameType( wxString );

	int parseStyle( wxString );

	wxPen parsePen( wxXmlNode* );

	void parsePenList( wxXmlNode*, vector<wxPen>& );

	wxBrush parseBrush( wxXmlNode *cur );

	void parseBrushList( wxXmlNode*, vector<wxBrush>& );

	void parseColorList( wxXmlNode*, vector<wxColour>& );

	void parseCircle( wxXmlNode*, GcCircle& );

	void parseChartFrame( wxXmlNode*, GcChartFrame& );

	void parseRectangle( wxXmlNode*, GcRectangle& );

	void parseArrow( wxXmlNode*, GcArrow& );

	void parseRegion( wxXmlNode*, GcRegion& );

	void parseZodiacalSigns( wxXmlNode*, GcZodiacalSigns& );

	void parseHouses( wxXmlNode*, GcHouses& );
};

#endif

