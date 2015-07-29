/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/base/Lang.h
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

#ifndef _LANG_H_
#define _LANG_H_

#ifdef __GNUG__
#pragma interface "Lang.h"
#endif

#include "constants.h"

class Config;

/*************************************************//**
*
* \brief encapsulation of language specific names for astrological entities
*
******************************************************/
class Lang
{
public:

	Lang();
	Lang( Config* );

	// get weekday name (0..6, sunday is 0)
	const wxString getWeekdayName( const int& ) const;
	const wxString getMonthName( const int &i ) const;

	const wxString getYoniName( const int& ) const;
	const wxString getGanaName( const int& ) const;
	const wxString getNadiName( const int& ) const;
	const wxString getVarnaName( const int& ) const;
	const wxString getTaraName( const int& ) const;
	const wxString getTithiName( const int& ) const;
	const wxString getBhavaName( const int &i ) const;
	const wxString getDignityName( const int &i, const int& format ) const;

	const wxString getVimsopakaDignityName( const int &index, const int &dignity ) const;

	// Uranian event symbols
	const wxString getCircleName( const int &i ) const;

	const wxChar getSignSymbolCode( const int &i, const int type = 0 ) const;
	const wxString getPlanetSymbolCode( const int &i ) const;

	const wxString getNakshatraName( const int &nak, const int &nsys, const int &format = TSHORT ) const;

	const wxString getKarakaName( const int &i, const int format = TLARGE ) const;
	const wxString getShastiamsaName( const int &i ) const;

	const wxString getObjectName( const int &num, const int &format, const bool vedic = false ) const;

	const wxString getSignName( const int &i, const int format = TLARGE ) const;

	// name of ayanamsa
	const wxString getAyanamsaName( const int &ayatype );

	void setConfig( Config *c ) {
		cfg = c;
	}

private:
	Config *cfg;
};

/*************************************************//**
*
*  \brief Formatting of date, time and degree values
*
******************************************************/
class Formatter
{
	DECLARE_SINGLETON( Formatter )

public:

	// get the integers for day, month and year from jd
	void getDateIntsFromJD( const double &jd, int &day, int &month, int &year, const int format = 0 );

	// parses date string and calculates day, month and year
	bool getDateIntsFromString( const wxChar *value, int &day, int &month, int &year, const int format = 0 );

	// formats a date which is given by jd
	const wxString getDateStringFromJD( const double &jd, int format = 0 );

	// formats a date which is given by jd (including time in GT)
	const wxString getFullDateStringFromJD( const double &jd, const int format = 0 );

	// calculates degrees, minutes and seconds from a given length from a (double) degree value
	void getDegMinSecInts( const double &len, int &deg, int &min, int &sec );
	void getDegMinSecInts2( const double &len, int &deg, int &min, double &sec );

	const wxString getDegMinSecFormatted( double t, const int &format, const double &minvalue, const double &maxvalue );

	// formats a length like DD:MM:SS
	const wxString getLenFormatted( const double &len, const int format = DEG_PRECISION_FLEXIBLE );

	// formats a latitude like DD:MM:SS  [N|S]
	const wxString getLatitudeFormatted( const double&, const int format = DEG_PRECISION_FLEXIBLE );

	// formats a longitude like DD:MM:SS  [E|W]
	const wxString getLongitudeFormatted( const double&, const int format = DEG_PRECISION_FLEXIBLE );

	// formats a time value like HH:MM:SS
	const wxString getTimeFormatted( const double &t, const int format = DEG_PRECISION_FLEXIBLE ) { return getDegMinSecFormatted( t, format, 0.0, 24.0 ); }

	// formats a degree value like DD:MM:SS
	const wxString getDegreesFormatted( const double &t, const int format = DEG_PRECISION_FLEXIBLE ) { return getDegMinSecFormatted( t, format, 0.0, 360.0 ); }

	wxString parseDegMinSecAndFormat( wxString, const int = DEG_PRECISION_FLEXIBLE );

	// parses a degree string DD:MM:SS
	bool getDegreeIntsFromString( const wxChar *value, int &deg, int &minute, int &second, int format = 0 );

	// Calculates a length string
	const wxString getLenString( const double &len, const int format = 0, const int dir = 0 );

	wxString getIntOrEmtpy( const int &a );
};

#endif

