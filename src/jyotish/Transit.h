/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/Transit.h
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
#ifndef _TRANSIT_H_
#define _TRANSIT_H_

#ifdef __GNUG__
#pragma interface "Transit.h"
#endif

#include <wx/string.h>

#include "Expert.h"
#include "Horoscope.h"

class AspectExpert;
class ChartProperties;
class Writer;

/*************************************************//**
*
* \brief Horoscope subclass providing transit calculations
*
******************************************************/
class TransitHoroscope : public Horoscope
{
public:

	TransitHoroscope();
	double calcTransitPositions( const Horoscope *hbase, const double &jd, const bool &vedic,
	                             const double &yl, const int &mode );

protected:
	double calcdate;
	DECLARE_CLASS( TransitHoroscope )
};


/*************************************************//**
*
* \brief encapsulation for calculation of transits, directions, constant + Solar + Lunar arc
*
******************************************************/
class TransitExpert : public Expert
{
public:

	TransitExpert( Horoscope *h, const ChartProperties *chartprops );
	~TransitExpert();
	void update();
	const Horoscope *getTransitChart() {
		return htransit;
	}
	void setTransitDate( const double &jd, const double tzoffset = 0.0 );
	double getTransitDate() {
		return transitJD;
	}
	double getTzOffset() { return tzoffset; }

	void setTransitMode( const int &tmode ) {
		transitmode = tmode;
	}
	void setSortOrder( const int &order ) {
		sortorder = order;
	}
	void setYearLength( const double &yl ) {
		yearlength = yl;
	}
	void setCircleDegrees( const int &cd ) {
		degrees = cd;
	}
	void setOrbis( const double &orb ) {
		orbis = orb;
	}
	void setFilter( const int &filter ) {
		this->filter = filter;
	}
	void writeTransitData( Writer*, const bool &uranian );

private:
	const ChartProperties *chartprops;
	int transitmode, sortorder, degrees, filter;
	double yearlength, orbis, transitJD, posdelta;
	double tzoffset;

	TransitHoroscope *htransit;
	AspectExpert *aexpert;
	void writeStandardPlanetList( Writer *writer, const ChartProperties* );
	void writeUranianPlanetList( Writer *writer, const ChartProperties* );
};

#endif

