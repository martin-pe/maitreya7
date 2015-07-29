/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/Planet.cpp
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
#pragma implementation "Planet.h"
#endif

#include "Planet.h"

#include "astrobase.h"
#include "Conf.h"
#include "constants.h"
#include "Horoscope.h"
#include "Lang.h"
#include "Varga.h"

extern Config *config;

/*****************************************************
**
**   Planet   ---   Constructor
**
******************************************************/
Planet::Planet( const Horoscope *h, const int &pindex )
		: horoscope( h ),
		index( pindex )
{
	length = 0;
	for ( int i = 0; i < 4; i++ )
	{
		vimsopakaBala[i] = 0;
		vimsopakaBalaGoodVargas[i] = 0;
	}
	averageVimsopakaBala = 0;
	for ( int division = 0; division < NB_VARGAS; division++ ) vargadata[division] = new VargaData( this, division );
}

/*****************************************************
**
**   Planet   ---   Destructor
**
******************************************************/
Planet::~Planet()
{
	for ( int i = 0; i < NB_VARGAS; i++ ) delete vargadata[i];
}

/*****************************************************
**
**   Planet   ---   getVargaData
**
******************************************************/
VargaData *Planet::getVargaData( const int &i ) const
{
	assert( i >= 0 && i < NB_VARGAS );
	return vargadata[i];
}

/*****************************************************
**
**   Planet   ---   getVimsopakaBalaGoodVargas
**
******************************************************/
int Planet::getVimsopakaBalaGoodVargas( const int &i ) const
{
	assert( i >= 0 && i < 4 );
	return vimsopakaBalaGoodVargas[i];
}

/*****************************************************
**
**   Planet   ---   getRasi
**
******************************************************/
int Planet::getRasi()
{
	return ::getRasi( length );
}

/*****************************************************
**
**   Planet   ---   getVimsopakaBala
**
******************************************************/
double Planet::getVimsopakaBala( const int &i ) const
{
	assert( i >= 0 && i < 4 );
	return vimsopakaBala[i];
}

/*****************************************************
**
**   Planet   ---   update
**
******************************************************/
void Planet::update( const double &len )
{
	updateVargaData( len );
}

/*****************************************************
**
**   Planet   ---   updateVargaData
**
******************************************************/
void Planet::updateVargaData( const double &len )
{
	VargaExpert expert( horoscope );
	length = len;

	for ( int division = 0; division < NB_VARGAS; division++ )
	{
		vargadata[division]->rasi = expert.calcVarga( length, division );
	}
}

/*****************************************************
**
**   Planet   ---   calculateDignities
**
******************************************************/
void Planet::calculateDignities()
{
	if ( index > OSATURN ) return;

	assert( horoscope );
	VargaExpert expert( horoscope );
	int dignity = 0;
	int vargaViswa = 0;
	int bala = 0;
	int isTempFriend, rasi, tfrasi, tflordrasi, isfriend;


	for ( int division = 0; division < NB_VARGAS; division++ )
	{
		dignity = DIGNITY_NONE;
		rasi = horoscope->getVargaData( index, division )->getRasi();
		tfrasi = horoscope->getVargaData( index, config->tempFriendBasedOnVarga ? division : VARGA_RASI )->getRasi();
		tflordrasi = horoscope->getVargaData( getLord( rasi ), config->tempFriendBasedOnVarga ? division : VARGA_RASI )->getRasi();
		isTempFriend = isRasiPosTempFriend( tfrasi, tflordrasi );
		//printf( "Planet %d Varga: %d rasi %d tfrasi %d tflord %d tflordrasi %d, isTempFriend %d config %d\n",
		//pindex, division, rasi, tfrasi, tflord, tflordrasi, isTempFriend, config->tempFriendBasedOnVarga );
		isfriend = isFriend( index, getLord( rasi));

		// Adhi Mitra
		if ( isfriend == FRIEND && isTempFriend == FRIEND )
		{
			dignity = DIGNITY_ADHIMITRA;
			vargaViswa = 18;
			bala = 20;
		}

		// Mitra
		else if ( isfriend == NEUTRAL && isTempFriend == FRIEND )
		{
			dignity = DIGNITY_MITRA;
			vargaViswa = 15;
			bala = 15;
		}

		// Sama
		else if (( isfriend == FRIEND && isTempFriend == ENEMY ) || ( isfriend == ENEMY && isTempFriend == FRIEND ))
		{
			dignity = DIGNITY_SAMA;
			vargaViswa = 10;
			bala = 10;
		}

		// Satru
		else if ( isfriend == NEUTRAL && isTempFriend == ENEMY )
		{
			dignity = DIGNITY_SATRU;
			vargaViswa = 7;
			bala = 4;
		}

		// Adhi Satru
		else if ( isfriend == ENEMY && isTempFriend == ENEMY )
		{
			dignity = DIGNITY_ADHISATRU;
			vargaViswa = 5;
			bala = 2;
		}

		/*
		* NB: Moon in Taurus is exalted AND in Moolatrikona (Varga Chart, e.g. D-9). So exaltation will prevail because of order below.
		* Same for Saturn in Aquarius (own and Moolatrikona): Moolatrikona will prevail.
		*/
		// Own sign
		if ( index == getLord( rasi ))
		{
			dignity = DIGNITY_SVAKSHETRA;
			vargaViswa = 20;
		}

		if ( hasExaltationRasi( index, rasi )) {
			dignity = DIGNITY_EXALTED;
		}

		// Special handling for Moolatrikona: Rasi depends on exact degree, other Vargas only on sign position
		if ( division == VARGA_RASI )
		{
			if ( ::isInMulatrikona( index, horoscope->getVedicLength( index )))
			{
				dignity = DIGNITY_MOOLATRIKONA;
				bala = 45;
			}
		}
		else
		{
			if ( horoscope->getVargaData( index, division )->getRasi() == getMulatrikonaRasi( index ))
			{
				dignity = DIGNITY_MOOLATRIKONA;
				bala = 45;
			}
		}

		// Debilation
		if ( hasDebilationRasi( index, rasi )) {
			dignity = DIGNITY_DEBILATED;
		}

		assert( dignity != DIGNITY_NONE );
		vargadata[division]->dignity = dignity;
		vargadata[division]->vargaViswa = vargaViswa;
		vargadata[division]->saptaVargajaBala = bala;
	}
	calculateVimsopakaBala();
}

/*****************************************************
**
**   Planet   ---   calculateVimsopakaBala
**
******************************************************/
void Planet::calculateVimsopakaBala()
{
	int i;
	for ( i = 0; i < 4; i++ )
	{
		vimsopakaBala[i] = 0;
		vimsopakaBalaGoodVargas[i] = 0;
	}

	for ( i = 0; i < 6; i++ )  addVimsopakaValue( shadvarga_weight[i], shadvarga_vargas[i], 0 );
	for ( i = 0; i < 7; i++ )  addVimsopakaValue( saptavarga_weight[i], saptavarga_vargas[i], 1 );
	for ( i = 0; i < 10; i++ )	addVimsopakaValue( dasavarga_weight[i], dasavarga_vargas[i], 2 );
	for ( i = 0; i < 16; i++ )	addVimsopakaValue( shodasavarga_weight[i], shodasavarga_vargas[i], 3 );

	averageVimsopakaBala = .25 * ( vimsopakaBala[0] + vimsopakaBala[1] + vimsopakaBala[2] + vimsopakaBala[3] );
}

/*****************************************************
**
**   Planet   ---   addVimsopakaValue
**
******************************************************/
void Planet::addVimsopakaValue( const double &weight, const int& varga, const int &vindex )
{
	double bala;
	const double minbala = 20;

	assert( vindex >= 0 && vindex < 4 );
	assert( varga >= 0 && varga < NB_VARGAS );
	assert( weight > 0 && weight < 10 );

	bala = weight * getVargaData( varga )->getVargaViswa();
	vimsopakaBala[vindex] += .05 * bala;
	if ( getVargaData( varga )->getVargaViswa() >= minbala ) vimsopakaBalaGoodVargas[vindex]++;
}
