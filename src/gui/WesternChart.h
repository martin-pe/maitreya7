/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/WesternChart.h
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

#ifndef WESTERNCHART_H_
#define WESTERNCHART_H_

#ifdef __GNUG__
#pragma interface "WesternChart.h"
#endif

#include "GraphicalChart.h"

class AspectExpert;
class GcCircle;
class Horoscope;
class WesternChartConfig;
class Writer;

/*************************************************//**
*
*  western representation of graphical chart
*
******************************************************/
class WesternChart : public GraphicalChart
{
public:

	WesternChart( const Horoscope*, const Horoscope*, const ChartProperties* );
	~WesternChart();
	virtual void OnDataChanged();

	WesternChartConfig *getChartConfig() const { return wconf; }

	void setTransitChart( Horoscope *h );

private:

  virtual void paintInternal( ChartConfig* );

	const Horoscope *horoscope, *htransit;

	AspectExpert *aexpert;
	WesternChartConfig *wconf;
	Writer *writer;

	int lastaspect;
	bool transitmode;
	bool transitsoutside;
 	double psize, aries;

	void paintZodiac();
	void paintAllPlanets( const Horoscope *h );

	// get coordinates for a planet
	MyPoint getCoordinatesForPlanet( const int &planet, const Horoscope *h );

	// get radius for planet. Can vary if other planets occupy the same place
	double getPRadius( const int &planet, const Horoscope *h );

	void paintAspects();
	void paintSingleAspect( const double &len1, const double &len2, const int &type );

	void paintLengthMarker( const double &alfa, const GcChartFrame& );

	void paintHouses();
	void paintHouseBordersAndAngles();
};

#endif

