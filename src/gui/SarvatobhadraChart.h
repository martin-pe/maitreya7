/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/SarvatobhadraChart.h
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

#ifndef SARVATOBHADRACHART_H_
#define SARVATOBHADRACHART_H_

#ifdef __GNUG__
#pragma interface "SarvatobhadraChart.h"
#endif

#include <wx/string.h>

#include "Nakshatra.h"
#include "BasicVedicChart.h"

class ChartProperties;
class NakshatraHoroscope;

/*************************************************//**
*
* \brief creates GUI represenation of SBC
*
******************************************************/
class SarvatobhadraChart : public BasicVedicChart
{
public:

	SarvatobhadraChart( const Horoscope*, const ChartProperties* );
	SarvatobhadraChart( const Horoscope*, const Horoscope*, const ChartProperties* );
	SarvatobhadraChart( const NakshatraHoroscope*, const ChartProperties* );
	SarvatobhadraChart( const NakshatraHoroscope*, const NakshatraHoroscope*, const ChartProperties* );
	~SarvatobhadraChart();

	virtual void paintChart();
	bool handleMouseLeftEvent( int x, int y, wxRect& );
	void setCharts( const NakshatraHoroscope*, const NakshatraHoroscope* );

private:
	double xstep, ystep;
	int mark[9][9];
	const NakshatraHoroscope *nh1, *nh2;
	vector<NakshatraProperties> nakshatraProps;

	// Coordinate methods
	bool getFieldIndex( int *x, int *y );
	MyRect getFieldCoord( const int &x, const int &y );
	MyRect getWeekdayCoord( const int& );
	MyRect getRasiCoord( const int& );
	void getNakshatraIndex( const int &i, int &x, int &y );
	void init();

	virtual void getRasiIndex( const int &r, int &x, int &y );

	void getWeekdayIndex( const int &w, int &x, int &y );
	MyRect getNakshatraCoord( const int &i );
	void markField( int x, int y );

	virtual int getPlanetField( const int &planet, const int &chart_id );
	virtual bool getPlanetRetro( const int &planet, const int &chart_id );

	void drawAspects();
};


#endif

