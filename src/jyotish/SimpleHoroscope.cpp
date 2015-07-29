/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/SimpleHoroscope.cpp
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
#pragma implementation "SimpleHoroscope.h"
#endif

#include "SimpleHoroscope.h"

#include "Calculator.h"
#include "Conf.h"
#include "constants.h"
#include "mathbase.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <wx/string.h>

extern Config *config;

IMPLEMENT_CLASS( SimpleHoroscope, wxEvtHandler )

/*****************************************************
**
**   ObjectPosition   ---   Constructor
**
******************************************************/
ObjectPosition::ObjectPosition()
{
	length = 0;
	latitude = 0;
	speed = 0;
	retro = false;
}

/*****************************************************
**
**   ObjectPosition   ---   Constructor
**
******************************************************/
ObjectPosition::ObjectPosition( const double &l, const double &b, const double &s )
{
	length = l;
	latitude = b;
	speed = s;
	retro = false;
}

/*****************************************************
**
**   SimpleHoroscope   ---   Constructor
**
******************************************************/
SimpleHoroscope::SimpleHoroscope()
{
	int i;
	TzUtil tzu;
	for ( i = 0; i < MAX_EPHEM_OBJECTS; i++ )
	{
		object_len[i] = 0;
		object_lat[i] = 0;
		object_speed[i] = 1;
	}
	for ( i = HOUSE1; i <= HOUSE12; i++ ) whousecusp[i] = ihousecusp[i] = ihousesandhi[0] = 0;

	TzInfo info = tzu.getCurrentTzInfo();

	dataSet = new FileDataSet;
	Location location = config->defaultLocation;
	location.setTimeZone( info.tzhours );
	location.setDST( info.dsthours );
	dataSet->setLocation( location );
}

/*****************************************************
**
**   SimpleHoroscope   ---   Destructor
**
******************************************************/
SimpleHoroscope::~SimpleHoroscope()
{
	delete dataSet;
}

/*****************************************************
**
**   SimpleHoroscope   ---   updatePlanets
**
** update of Ayanamsa, object_len etc and house length
**
******************************************************/
void SimpleHoroscope::updatePlanets()
{
	int i;
	double r[12], houselen;
	Calculator *calculator = CalculatorFactory().getCalculator();

	// Ayanamsa
	iayanamsa_type = config->vAyanamsa;
	wayanamsa_type = config->wAyanamsa;
	iayanamsa = calculator->calcAyanamsa( getJD(), iayanamsa_type );
	wayanamsa = calculator->calcAyanamsa( getJD(), wayanamsa_type );

	// Planetary positions
	for ( i = 0; i < MAX_EPHEM_OBJECTS; i++ )
	{
		if ( i == OMEANDESCNODE || i == OTRUEDESCNODE || i == OASCENDANT || i == OMERIDIAN ) continue;
		calculator->calcPositionSpeed( dataSet, i, object_len[i], object_lat[i], object_speed[i] );
	}
	object_len[OMEANDESCNODE] = red_deg( object_len[OMEANNODE] + 180 );
	object_len[OTRUEDESCNODE] = red_deg( object_len[OTRUENODE] + 180 );
	object_speed[OMEANDESCNODE] = object_speed[OMEANNODE];
	object_speed[OTRUEDESCNODE] = object_speed[OTRUENODE];

	// House calculation
	calculator->calcHouses( getJD(), getLongitude(), getLatitude(), config->wHouseSystem, r );
	for ( i = HOUSE1; i <= HOUSE12; i++ ) whousecusp[i] = red_deg( r[i] - wayanamsa );

	if ( config->vHouseSystem != config->wHouseSystem )
		calculator->calcHouses( getJD(), getLongitude(), getLatitude(), config->vHouseSystem, r );

	for ( i = HOUSE1; i <= HOUSE12; i++ ) ihousecusp[i] = red_deg( r[i] - iayanamsa );
	for ( i = HOUSE1; i <= HOUSE12; i++ )
	{
		houselen = red_deg( ihousecusp[i<11 ? i+1 : 0] - ihousecusp[i] );
		ihousesandhi[i] = red_deg( ihousecusp[i] + .5 * houselen );
	}
	object_len[OASCENDANT] = r[0];
	object_len[OMERIDIAN] = r[9];

	siderealTime = calculator->calcSiderealTime( getJD(), getLongitude() );

	updatePositionArrays();
}

/*****************************************************
**
**   SimpleHoroscope   ---   updatePositionArrays
**
** updates the arrays vpos and wpos
** integrated in updatePlanets, only once called by TransitHoroscope
**
******************************************************/
void SimpleHoroscope::updatePositionArrays()
{
	// Setup Object position structs
	for ( int i = 0; i < MAX_EPHEM_OBJECTS; i++ )
	{
		vpos[i].length   = wpos[i].length   = object_len[i];
		vpos[i].latitude = wpos[i].latitude = object_lat[i];
		vpos[i].speed    = wpos[i].speed    = object_speed[i];
		if ( i != OMEANNODE && i != OMEANDESCNODE )
			vpos[i].retro = wpos[i].retro = ( object_speed[i] < 0 );

		// p.length =0 can happen for planetoids etc. without data files.
		// Do not adjust Ayanamsa in that case, so 0 Aries position will be quite obvious.
		if ( object_len[i] != 0 )
		{
			vpos[i].length = red_deg( vpos[i].length - iayanamsa );
			wpos[i].length = red_deg( wpos[i].length - wayanamsa );
		}
	}
}

/*****************************************************
**
**   SimpleHoroscope   ---   getObjectPosition
**
******************************************************/
ObjectPosition SimpleHoroscope::getObjectPosition( const int &planet, const bool &vedic ) const
{
	ASSERT_VALID_EPHEM_OBJECT( planet )
	return( vedic ? vpos[planet] : wpos[planet] );
}

/*****************************************************
**
**   SimpleHoroscope   ---   getLength
**
******************************************************/
double SimpleHoroscope::getLength( const int &planet, const bool &vedic ) const
{
	ASSERT_VALID_OBJECT( planet );
	return getObjectPosition( planet, vedic ).length;
}

/*****************************************************
**
**   SimpleHoroscope   ---   isRetrograde
**
******************************************************/
bool SimpleHoroscope::isRetrograde( const int &planet ) const
{
	ASSERT_VALID_OBJECT( planet );
	return getObjectPosition( planet, true ).retro;
}

/*****************************************************
**
**   SimpleHoroscope   ---   getTropicalLength
**
******************************************************/
double SimpleHoroscope::getTropicalLength( const int &planet ) const
{
	ASSERT_VALID_EPHEM_OBJECT( planet )
	return object_len[planet];
}

/*****************************************************
**
**   SimpleHoroscope   ---   getLatitude
**
******************************************************/
double SimpleHoroscope::getLatitude( const int &planet ) const
{
	ASSERT_VALID_OBJECT( planet );
	return getObjectPosition( planet, true ).latitude;
}

/*****************************************************
**
**   SimpleHoroscope   ---   getSpeed
**
******************************************************/
double SimpleHoroscope::getSpeed( const int &planet ) const
{
	ASSERT_VALID_EPHEM_OBJECT( planet )
	return object_speed[planet];
}

/*****************************************************
**
**   SimpleHoroscope   ---   getHouse
**
******************************************************/
double SimpleHoroscope::getHouse( const int& housenb, const bool& vedic, const bool getsandhi ) const
{
	ASSERT_VALID_HOUSE( housenb )
	if ( vedic ) {
		return( getsandhi ? ihousesandhi[housenb] : ihousecusp[housenb] );
	}
	else {
		return whousecusp[housenb];
	}
}

/*****************************************************
**
**   SimpleHoroscope   ---   getHouseForLength
**
******************************************************/
double SimpleHoroscope::getHouse4Length( const double &len, const bool &vedic ) const
{
	double hstart, hend, hlen, hpart, ret;
	for ( int i = HOUSE1; i <= HOUSE12; i++ )
	{
		if ( ! vedic )
		{
			hstart = whousecusp[i];
			hend = whousecusp[red12(i+1)];
		}
		else
		{
			if ( config->vHouseWestern )
			{
				hstart = ihousecusp[i];
				hend = ihousecusp[red12(i+1)];
			}
			else
			{
				hstart = ihousesandhi[red12( i -1)];
				hend = ihousesandhi[red12(i)];
			}
		}
		if (( hstart <= len && len < hend ) || ( hstart > hend && ( len >= hstart || len < hend )))
		{
			hlen = red_deg( hend - hstart );
			hpart = red_deg( len - hstart );
			ret = i + hpart / hlen;
			//printf( "SimpleHoroscope::getHouse4Length len %f vedic %d hstart %f hend %f hlen %f hpart %f ret %f\n", len, vedic, hstart, hend, hlen, hpart, ret );
			assert( ret >= 0 && ret < 12 );
			return ret;
		}
	}
	//assert( 0 );

	printf( "WARN: SimpleHoroscope::getHouse4Length target not reached, len %f vedic %d\n", len, vedic );
	return 0;
}

/*****************************************************
**
**   SimpleHoroscope   ---   getHousePos
**
******************************************************/
int SimpleHoroscope::getHousePos( const int &planet, const bool &vedic ) const
{
	// TODO store house lengths on update() and return that instead of repeated calculation
	return (int)getHouse4Length( getLength( planet, vedic), vedic );
}

/*****************************************************
**
**   SimpleHoroscope   ---   openFile
**
******************************************************/
bool SimpleHoroscope::openFile( const wxChar *filename )
{
	return dataSet->openFile( filename );
}

/*****************************************************
**
**   SimpleHoroscope   ---   saveAs
**
******************************************************/
bool SimpleHoroscope::saveAs( const wxChar *filename )
{
	return dataSet->saveAs( filename );
}

/*****************************************************
**
**   SimpleHoroscope   ---   save
**
******************************************************/
bool SimpleHoroscope::save()
{
	return dataSet->save();
}


