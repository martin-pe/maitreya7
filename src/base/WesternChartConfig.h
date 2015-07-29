/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/base/WesternChartConfig.h
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

#ifndef _WESTERNCHARTCONFIG_H_
#define _WESTERNCHARTCONFIG_H_

#ifdef __GNUG__
#pragma interface "WesternChartConfig.h"
#endif

#include "ChartConfig.h"
#include "constants.h"
#include "XmlConfigLoader.h"

using namespace std;

/*************************************************//**
*
* \brief configuration for a single western chart
*
******************************************************/
class WesternChartConfig : public ChartConfig
{
	DECLARE_CLASS( WesternChartConfig )
public:

	WesternChartConfig();

	virtual void setDefaultConfig();
	virtual wxString dump();
	virtual wxString dumpXml();

	GcChartFrame zodiac;
	GcChartFrame zodiac1Degree;
	GcChartFrame zodiac5Degree;
	GcChartFrame zodiacSymbols;
	GcChartFrame outerZodiac;

	GcChartFrame innerHouse;
	GcChartFrame outerHouse;

	GcChartFrame planets;
	GcChartFrame aspects;

	GcChartFrame innerTransitZodiac;
	GcChartFrame innerTransitZodiac1Degree;
	GcChartFrame innerTransitZodiac5Degree;
	GcChartFrame innerTransitPlanets;

	GcChartFrame outerTransitZodiac;
	GcChartFrame outerTransitZodiac1Degree;
	GcChartFrame outerTransitZodiac5Degree;
	GcChartFrame outerTransitPlanets;

	double planetMarkerLength;

	GcHouses houses;

	GcArrow angles;
};

/*************************************************//**
*
* \brief contains configs for western charts
*
******************************************************/
class WesternChartConfigLoader : public ChartConfigLoader
{
	DECLARE_SINGLETON_WITHOUT_CONSTRUCTOR( WesternChartConfigLoader )

public:
	WesternChartConfig *getConfig( const int& );

protected:
	virtual BaseXmlConfig *createConfig( BaseXmlConfig* = 0 );

	virtual void readSpecificAttributes( BaseXmlConfig*, wxXmlNode* );

private:

	WesternChartConfigLoader();

};


#endif

