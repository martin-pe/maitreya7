/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/BasicVedicChart.h
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

#ifndef _BASICVEDICCHART_H_
#define _BASICVEDICCHART_H_

#ifdef __GNUG__
#pragma interface "BasicVedicChart.h"
#endif

#include <wx/dynarray.h>
#include <wx/string.h>
#include <vector>

#include "GraphicalChart.h"

using namespace std;

class ChartProperties;
class Horoscope;
class Painter;
class VedicChartConfig;
class Writer;
class wxRect;

/*************************************************//**
*
* \brief Holds the text items for a field of a chart
*
******************************************************/
class ChartTextItem
{
public:
	ChartTextItem( const wxString lname, const wxString sname, const bool r = false )
	{
		longname = lname;
		shortname = sname;
		retro = r;
	}
	wxString shortname, longname;
	bool retro;
};

/*************************************************//**
*
* \brief Holds graphical items for a field of a chart
*
******************************************************/
class ChartGraphicItem
{
public:
	ChartGraphicItem( const wxString n, const int &index, const bool r = false ) {
		name = n;
		pindex = index;
		retro = r;
	}
	ChartGraphicItem( const ChartGraphicItem &a ) {
		name = a.name;
		pindex = a.pindex;
		retro = a.retro;
	}
	wxString name;
	int pindex;
	bool retro;
};

/*************************************************//**
*
* \brief Contains the graphical items and text items for one field (1..12 or 1..28) of a chart
*
******************************************************/
class ChartContents
{
public:
	vector<ChartTextItem> textitems;
	vector<ChartGraphicItem> graphicitems;
	wxArrayInt planets;
	void clear();
};

/*************************************************//**
*
* \brief Basic class for all Vedic charts including Rasi and Sarvatobhadra
*
*
* This class provides basic functionality for Vedic charts:
* - holding contents for Rasi/Nakshatra fields
* - drawing methods for graphical items and text items
* - styles for Vedic charts
* - Writer and Painter references
* - Paint method, calling abstract method paintChart
* - Public update method OnDataChanged
*
******************************************************/
class BasicVedicChart : public GraphicalChart
{
public:

	BasicVedicChart( const ChartProperties*, const int &field_count, const int chartcount = 1 );

	void OnDataChanged();

	VedicChartConfig *getChartConfig() const { return vconf; }


protected:

	virtual void paintInternal( ChartConfig* );

	/**
	 * \brief 12 or 28 supported
	 */
	const int field_count;

	/**
	 * \brief Number of horoscopes included in graphical display (default: 1, transit or partner mode: 2)
	 */
	int chart_count;

	/**
	 * \brief Graphical style of chart (colors, background etc.)
	 */
	VedicChartConfig *vconf;

	/**
	 * \brief max widget extension
	 */
	double xmax, ymax;

	/**
	 * \brief radius from center to end of the chart (not end of widget)
	 */
	double xr, yr;

	/**
	 * \brief Rasi/Nakshatra contents
	 */
	vector<ChartContents> contents, tcontents;

	/**
	 * \brief get chart contents for chart number hid
	 */
	vector<ChartContents> &getChartContents( const int &hid );

	/**
	 * \brief Write astrological objects into contents arrays
	 */
	virtual void writeChartContents( const int &chart_id );

	/**
	*  \brief Abstract method. Is overwritten by Sarvatobhadra chart (28 fields) and Vedic Rasi chart (12 fields in South/East/North Indian style)
	*/
	virtual void paintChart() = 0;

	/**
	*  \brief Returns the field index (1..12 or 1..28) for a given planet in radix or transit mode. Must be overwritten.
	*/
	virtual int getPlanetField( const int &planet, const int &chart_id )  {
		return 0;
	}

	/**
	*  \brief Returns a boolean value for retrogression of a given planet in radic or transit mode. Must be overwritten.
	*/
	virtual bool getPlanetRetro( const int &planet, const int &chart_id )  {
		return false;
	}

	/**
	 * \brief Draw Rasi or Nakshatra field contents for field number r in the given rectangle
	 */
	void drawFieldText( const double &x, const double &y, const double &w, const double &h, const int &r, const int& align, const int &chart_id, const int border = 0 );

	/**
	 * \brief Draw Rasi or Nakshatra field contents for field number r in the given rectangle
	 */
	void drawFieldText( MyRect rect, const int &r, const int& align, const int &chart_id, const int border = 0 );

	/**
	 * \brief Draw a single graphical item
	 */
	void drawSingleGraphicItem( const MyRect &rect, const ChartGraphicItem &g );

	/**
	 * \brief Draw one line of text in the given rectangle
	 */
	void drawTextItemLine( MyRect rect, const wxString &s, const int &align );

	/**
	 * \brief Draw one line of graphical items in the given rectangle
	 */
	void drawGraphicItemLine( MyRect rect, const vector<ChartGraphicItem> g, const int &align );

	/**
	 * \brief Draw text in the given rectangle. Adds support for text align.
	 */
	void drawTextFormatted( const MyRect &rect, const wxString t, const int& align );

	void paintOuterRectangle();

private:

};

#endif

