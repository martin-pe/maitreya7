/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/Nakshatra.h
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

#ifndef _NAKSHATRA_H_
#define _NAKSHATRA_H_

#ifdef __GNUG__
#pragma interface "Nakshatra.h"
#endif

#include "Expert.h"
#include "constants.h"
#include <vector>

using namespace std;

class ChartProperties;
class Writer;

enum { TARA_PROP_BENEFIC, TARA_PROP_MALEFIC, TARA_PROP_NEUTRAL }; 

/*************************************************//**
*
* 
*
******************************************************/
class NakshatraProperties
{
public:

	// benefic, malefic, neutral (Abhijit)
	int taraQuality;

	int tara; 

};


/*************************************************//**
*
* \brief Encapsulates calculation of Nakshatra releated details
*
******************************************************/
class NakshatraExpert : public Expert
{
public:

	NakshatraExpert() : Expert() {}
	NakshatraExpert( const Horoscope *h ) : Expert( h ) {}

	int getVedhaNakshatra( const int &nak ) const;
	int getYoni( const double& ) const;
	int getNadi( const double& ) const;
	int getGana( const double& ) const;
	int getTara( const int& ) const;

	void getSbcNakshatraProperties( const int &birthStar, vector<NakshatraProperties>& );

	void write( Writer *writer, const ChartProperties *chartprops, const bool show_header = true );
};

/*************************************************//**
*
* \brief Provides Nakshatra view on a Horoscope
*
******************************************************/
class NakshatraHoroscope
{
public:

	NakshatraHoroscope( const Horoscope*, const int mode = N27 );

	const Horoscope *getHoroscope() const {
		return horoscope;
	}

	int getNakshatra( const int &planet ) const;

	bool isRetrograde( const int &planet ) const;

	//bool isBenefic( const int& ) const;

	//bool isMalefic( const int &p ) const { return ! isBenefic( p ); }

protected:
	const Horoscope *horoscope;
	const int mode;
};

/*************************************************//**
*
* \brief Encapsulates astrological details for Sarvatobhadra charts
*
******************************************************/
class SarvatobhadraExpert : public Expert
{
public:

	SarvatobhadraExpert() : Expert() {}
	SarvatobhadraExpert( const Horoscope *h ) : Expert( h ) {}

	void writeSbcText( Writer*, const ChartProperties* );
	void writeSbcAfflictions( Writer*, const Horoscope*, const ChartProperties* );

protected:

};

#endif


