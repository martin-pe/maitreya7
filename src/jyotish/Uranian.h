/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/Uranian.h
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
#ifndef _URANIAN_H_
#define _URANIAN_H_

#ifdef __GNUG__
#pragma interface "Uranian.h"
#endif

#include <list>

#include "Expert.h"
#include "UranianBase.h"

using namespace std;

class ChartProperties;
class DataSet;
class Horoscope;
class UranianHelper;
class Writer;

/*************************************************//**
*
* \brief encapsulates calculations in Uranian astrology
*
******************************************************/
class UranianExpert : public Expert
{
	friend class UranianHelper;
public:

	UranianExpert( const ChartProperties*, Writer*, const UranianConfig &cfg );
	~UranianExpert();

	void writeTransitAnalysis( const Horoscope*, const Horoscope* );

	void writePartnerAnalysis( const Horoscope*, const Horoscope* );

	void writeRadixAnalysis( const Horoscope* );

	void writeYearlyPreviewAnalysis( const Horoscope*, const int &year );

private:

	void calculateEvents( const Horoscope *h1, const Horoscope *h2 );
	int calculateMidpointMatch( const double &len1, const double &len2, double *dist );

	double calcEventDateForSolarArcEvent( DataSet *basedate, double aprox_jd, const double &sun_radixlen,
	                                      const double &radixpos, const double &targetpos, const int &eventtype );

	const ChartProperties *chartprops;
	Writer *writer;
	UranianConfig cfg;
	UranianHelper *helper;

	bool show_header;

	class Calculator *calculator;
	list <UranianMidpointEvent> midpoint_events;
	list <UranianConjunctionEvent> conjunction_events;
	list <UranianMidpointValue> midpoint_values;
};


#endif

