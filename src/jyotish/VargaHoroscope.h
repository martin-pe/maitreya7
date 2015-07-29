/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/VargaHoroscope.h
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

#ifndef _VARGAHOROSCOPE_H_
#define _VARGAHOROSCOPE_H_

#ifdef __GNUG__
#pragma interface "VargaHoroscope.h"
#endif

#include <constants.h>

class Horoscope;

/*************************************************//**
*
* \brief minimal abstract chart representation
*
******************************************************/
class AbstractVargaHoroscope
{
public:

	virtual int getRasi( const int &planet ) const = 0;
	virtual int getNakshatra( const int &planet, const int &naksystem ) const = 0;

	virtual bool isRetrograde( const int &planet ) const = 0;
	virtual bool isBenefic( const int& ) const = 0;

	int getBhava( const int &planet ) const;
	bool isMalefic( const int &p ) const {
		return ! isBenefic( p );
	}
};

/*************************************************//**
*
* \brief virtual Horoscope in Varga chart giving access to astrological properties
*
******************************************************/
class VargaHoroscope : public AbstractVargaHoroscope
{
public:

	VargaHoroscope( const Horoscope*, const int division = 0 );
	virtual ~VargaHoroscope() {}

	virtual int getRasi( const int &planet ) const;
	virtual int getNakshatra( const int &planet, const int &naksystem ) const;

	virtual bool isRetrograde( const int &planet ) const;
	virtual bool isBenefic( const int& ) const;

	const Horoscope *getHoroscope() const {
		return horoscope;
	}

	void setDivision( const int &division );
	int getDivision() const {
		return division;
	}

	double getRasiLength( const int &planet ) const;


private:

	const Horoscope *horoscope;
	int division;
};
/*************************************************//**
*
* \brief minimal concrete chart representation
*
******************************************************/
class MinimalVargaHoroscope : public AbstractVargaHoroscope
{
public:

	MinimalVargaHoroscope();
	virtual ~MinimalVargaHoroscope() {}

	void cleanup();

	void setRasi( const int &id, const int &rasi );

	virtual int getRasi( const int &planet ) const;
	virtual int getNakshatra( const int &planet, const int &naksystem ) const {
		return -1;
	}

	virtual bool isRetrograde( const int &planet ) const {
		return false;
	}
	virtual bool isBenefic( const int& ) const {
		return false;
	}

private:

	int ephem_objects_rasi[MAX_EPHEM_OBJECTS];
	int extended_objects_rasi[MAX_EXTENDED_OBJECTS];
	int arabic_objects_rasi[MAX_ARABIC_OBJECTS];

};

#endif


