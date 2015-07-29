/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/GraphicalChart.h
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

#ifndef GRAPHICALCHART_H_
#define GRAPHICALCHART_H_

#ifdef __GNUG__
#pragma interface "GraphicalChart.h"
#endif

#include <wx/brush.h>
#include <wx/gdicmn.h>
#include <wx/pen.h>

#include "GraphConfig.h"
#include "Painter.h"

class ChartConfig;
class ChartProperties;
class Painter;
class Writer;

/*************************************************//**
*
*  
*
******************************************************/
class GraphicalChart
{
public:

	GraphicalChart( const ChartProperties* );
	virtual ~GraphicalChart() {}

	void paint( Painter*, const MyRect&, const wxRect* = 0 );

	virtual void OnDataChanged() = 0;

protected:

  const ChartProperties *chartprops;

	virtual void paintInternal( ChartConfig* ) = 0;

	void paintBackground( GcWidgetBackground* );

	void drawChartFrameField( const double &a0, const double &a1, const GcChartFrame&, const GcChartFrame& );

	void paintChartFrame( const GcChartFrame& );
	void paintMarkerLine( const double &alfa, const GcChartFrame&, const GcChartFrame& );
	MyPoint frameBorderPointToXy( const double &alfa, const GcChartFrame& );
	void paintArrow( const double &alfa, GcArrow& );

	void setDefaultTextColor();

	MyRect rect;
	wxRect *refreshRect;
	double rmax;
	double rxmax, rymax;
	double xcenter, ycenter;

	double symbolzoom;
	double textzoom;
	double extrazoom;
	double text_width, text_height, symbol_height;

	wxPen defaultPen;
	wxBrush defaultBrush;

	Painter *painter;

private:

	// used only here. Western/Vedic chart has its own configs
	ChartConfig *cfg;
};


#endif

