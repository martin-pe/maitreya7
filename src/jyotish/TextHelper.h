/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/TextHelper.h
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

#ifndef _TEXTHELPER_H_
#define _TEXTHELPER_H_

#ifdef __GNUG__
#pragma interface "TextHelper.h"
#endif

class ChartProperties;
class Horoscope;
class SimpleHoroscope;
class Writer;

/*************************************************//**
*
*  writes text output for text view and console output
*
******************************************************/
class TextHelper
{
public:

	TextHelper( const Horoscope*, const ChartProperties*, Writer* );

	void setShowHeader( const bool b = true ) {
		show_header = b;
	}

	int writeTextAnalysis( const int &mode, const int varga = 0, const int dasa = 0 );

	// standard lists in main view
	int writePlanets();
	int writeVedicPlanets();
	int writeWesternPlanets();
	int writeVedicPlanetReport();
	int writeWesternPlanetReport();

	// used in text view
	void writeBaseData();

	int writeAstronomicalData();
	void writeBhavas();
	int writeKp( const int &dasaindex );

private:
	const Horoscope *horoscope;
	const ChartProperties *chartprops;
	Writer *writer;
	bool show_header;

};

#endif

