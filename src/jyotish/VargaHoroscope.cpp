/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/VargaHoroscope.cpp
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

#ifdef __GNUG__
#pragma implementation "VargaHoroscope.h"
#endif

#include "VargaHoroscope.h"

#include "astrobase.h"
#include "Conf.h"
#include "Horoscope.h"
#include "mathbase.h"
#include "Varga.h"

extern Config *config;

/*****************************************************
**
**   AbstractVargaHoroscope   ---   AbstractVargaHoroscope
**
******************************************************/
int AbstractVargaHoroscope::getBhava( const int &planet ) const
{
	assert( planet >= 0 && planet < MAX_EPHEM_OBJECTS );
	int r1 = this->getRasi( planet );
	int r2 = this->getRasi( OASCENDANT );
	if ( r1 < 0 || r2 < 0 ) return -1;
	return red12( r1 - r2 );
}

/*****************************************************
**
**   VargaHoroscope   ---   Constructor
**
******************************************************/
VargaHoroscope::VargaHoroscope( const Horoscope *h, const int div )
		: horoscope( h ),	division( div ) {}

/*****************************************************
**
**   VargaHoroscope   ---   setDivision
**
******************************************************/
void VargaHoroscope::setDivision( const int &div )
{
	VargaExpert expert;
	assert( div >= 0 && div <= NB_VARGAS );
	division = div;
}

/*****************************************************
**
**   VargaHoroscope   ---   getRasiLength
**
******************************************************/
double VargaHoroscope::getRasiLength( const int &planet ) const
{
	assert( planet >= 0 && planet < MAX_EPHEM_OBJECTS && horoscope );
	const int inode = config->vLunarNodeMode == LUNAR_NODE_TRUE ? OTRUENODE : OMEANNODE;
	const int descnode = config->vLunarNodeMode == LUNAR_NODE_TRUE ? OTRUEDESCNODE : OMEANDESCNODE;
	// reverse for rahu and ketu
	return ::getRasiLen( horoscope->getVedicLength( planet ), planet == inode || planet == descnode );
}

/*****************************************************
**
**   VargaHoroscope   ---   getRasi
**
******************************************************/
int VargaHoroscope::getRasi( const int &planet ) const
{
	ASSERT_VALID_OBJECT( planet )
	assert( horoscope );
	return horoscope->getVargaData( planet, division )->getRasi();
}

/*****************************************************
**
**   VargaHoroscope   ---   getNakshatra
**
******************************************************/
int VargaHoroscope::getNakshatra( const int &planet, const int &naksystem ) const
{
	ASSERT_VALID_OBJECT( planet )
	assert( naksystem == N27 || naksystem == N28 );
	assert( horoscope );

	return ::getNakshatra( horoscope->getObjectPosition( planet, true ).length, naksystem );
}

/*****************************************************
**
**   VargaHoroscope   ---   isRetrograde
**
******************************************************/
bool VargaHoroscope::isRetrograde( const int &planet ) const
{
	return horoscope->isRetrograde( planet );
}

/*****************************************************
**
**   VargaHoroscope   ---   isBenefic
**
******************************************************/
bool VargaHoroscope::isBenefic( const int &p ) const
{
	return horoscope->isBenefic( p );
}

/*****************************************************
**
**   MinimalVargaHoroscope   ---   Constructor
**
******************************************************/
MinimalVargaHoroscope::MinimalVargaHoroscope()
{
	cleanup();
}

/*****************************************************
**
**   MinimalVargaHoroscope   ---   cleanup
**
******************************************************/
void MinimalVargaHoroscope::cleanup()
{
	int i;
	for ( i = 0; i < MAX_EPHEM_OBJECTS; i++ ) ephem_objects_rasi[i] = -1;
	for ( i = 0; i < MAX_EXTENDED_OBJECTS; i++ ) extended_objects_rasi[i] = -1;
	for ( i = 0; i < MAX_ARABIC_OBJECTS; i++ ) arabic_objects_rasi[i] = -1;
}

/*****************************************************
**
**   MinimalVargaHoroscope   ---   setRasi
**
******************************************************/
void MinimalVargaHoroscope::setRasi( const int &planet, const int &rasi )
{
	ASSERT_VALID_EPHEM_OBJECT( planet );
	assert( rasi >= -1 && rasi <= PISCES );
	ephem_objects_rasi[planet] = rasi;
}

/*****************************************************
**
**   MinimalVargaHoroscope   ---   getRasi
**
******************************************************/
int MinimalVargaHoroscope::getRasi( const int &planet ) const
{
	ASSERT_VALID_EPHEM_OBJECT( planet );
	return ephem_objects_rasi[planet];
}

