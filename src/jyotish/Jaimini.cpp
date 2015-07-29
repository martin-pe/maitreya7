/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/Jaimini.cpp
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
#pragma implementation "Jaimini.h"
#endif

#include "Jaimini.h"

#include <wx/string.h>

#include "astrobase.h"
#include "Conf.h"
#include "Horoscope.h"
#include "Lang.h"
#include "mathbase.h"
#include "PlanetList.h"
#include "Varga.h"
#include "VargaHoroscope.h"
#include "Writer.h"

extern Config *config;

/*****************************************************
**
**   JaiminiExpert   ---   update
**
******************************************************/
void JaiminiExpert::update()
{
	int i;
	const int inode = config->vLunarNodeMode == LUNAR_NODE_TRUE ? OTRUENODE : OMEANNODE;
	const int descnode = config->vLunarNodeMode == LUNAR_NODE_TRUE ? OTRUEDESCNODE : OMEANDESCNODE;

	for ( i = 0; i < 12; i++ ) nbplanets[i] = 0;
	vector<int> obs = PlanetList().getDefaultVedicObjectList();
	for ( unsigned int p = 0; p < obs.size(); p++ ) {
		nbplanets[chart->getRasi(obs[p])]++;
	}

	aspSat = calcPlanetAspects( OSATURN );
	aspRah = calcPlanetAspects( inode );
	aspMar = calcPlanetAspects( OMARS );
	aspKet = calcPlanetAspects( descnode );

	calcNodeStrength();

	for ( i = 0; i < 12; i++ )
	{
		nbAspectsOnRasi[i] = calcSignAspects( i );
	}
	calcYears();
	calcCharaKarakas();

}

/*****************************************************
**
**   JaiminiExpert   ---   calcPada
**
******************************************************/
int JaiminiExpert::calcPada( const int &bhava )
{
	assert(( bhava < 12 ) && ( bhava >= 0 ));

	int rasi = red12( chart->getRasi( OASCENDANT ) + bhava );
	int lrd = getJaiminiLord( rasi );
	int diff = red12( chart->getRasi( lrd ) - rasi );
	int pada = red12( rasi + 2 * diff );
	if ( ! config->vArudhaMode && ! ( diff % 3 )) pada = red12( pada + 9 );
	return pada;
}


/*****************************************************
**
**   JaiminiExpert   ---   getStrongerRasi
**
******************************************************/
int JaiminiExpert::getStrongerRasi( const int &rasi1, const int &rasi2 )
{
	// RULES IMPLEMENTIEREN !!

	// Rule 1a
	if ( nbplanets[rasi1] > nbplanets[rasi2] )
	{
		return rasi1;
	}
	// Rule 1b
	else if ( nbplanets[rasi1] < nbplanets[rasi2] )
	{
		return rasi2;
	}
	// Rule 2a
	else if ( nbAspectsOnRasi[rasi1] > nbAspectsOnRasi[rasi2] )
	{
		return rasi1;
	}
	// Rule 2b
	else if ( nbAspectsOnRasi[rasi1] < nbAspectsOnRasi[rasi2] )
	{
		return rasi2;
	}
	// **** TODO RULE3
	// Rule 4a
	else if ( chart->getRasiLength(getJaiminiLord( rasi1 )) > chart->getRasiLength(getJaiminiLord(rasi2 )) )
	{
		return rasi1;
	}
	// Rule 4b
	else if ( chart->getRasiLength(getJaiminiLord( rasi1 )) < chart->getRasiLength(getJaiminiLord(rasi2 )) )
	{
		return rasi2;
	}
	else assert( false );
	return 0;
}

/*****************************************************
**
**   JaiminiExpert   ---   getJaiminiLord
**
******************************************************/
int JaiminiExpert::getJaiminiLord( const int &rasi )
{
	const int inode = config->vLunarNodeMode == LUNAR_NODE_TRUE ? OTRUENODE : OMEANNODE;
	const int descnode = config->vLunarNodeMode == LUNAR_NODE_TRUE ? OTRUEDESCNODE : OMEANDESCNODE;
	int lrd = ::getLord( rasi );
	if (( rasi == SCORPIO ) && ( ketuStrongerThanMars )) lrd = descnode;
	if (( rasi == AQUARIUS ) && ( rahuStrongerThanSaturn )) lrd = inode;

	return lrd;
}

/*****************************************************
**
**   JaiminiExpert   ---   isKetuStrongerThanMars
**
******************************************************/
bool JaiminiExpert::isKetuStrongerThanMars()
{
	return ketuStrongerThanMars;
}

/*****************************************************
**
**   JaiminiExpert   ---   isRahuStrongerThanSaturn
**
******************************************************/
bool JaiminiExpert::isRahuStrongerThanSaturn()
{
	return rahuStrongerThanSaturn;
}

/*****************************************************
**
**   JaiminiExpert   ---   write
**
******************************************************/
void JaiminiExpert::write( Writer *writer, const bool show_header )
{
	int i, k_id;
	Lang lang;
	VargaExpert expert;
	wxString s;

	if ( show_header )
	{
		writer->writeHeader1 ( _( "Jaimini" ));
		s.Printf( wxT( "%s: %s"), _( "Varga"), expert.getVargaName( chart->getDivision()) );
		writer->writeLine( s );
	}

	writer->writeHeader2( _( "Chara Karakas" ));
	Table t1( 2, 9 );
	t1.setHeader( 0, _( "Karaka" ));
	t1.setHeader( 1, _( "Planet" ));
	int line = 1;
	for ( i = ATMA_KARAKA; i <= DARA_KARAKA; i++ )
	{
		t1.setEntry( 0, line, lang.getKarakaName( i ) );
		k_id = getCharaKaraka(i);
		if ( k_id > OSATURN ) k_id = OMEANNODE;
		t1.setEntry( 1, line, writer->getObjectName( k_id, TLARGE ));
		line++;
	}
	writer->writeTable( t1 );

	writer->writeHeader2( _( "Stronger Planets" ));
	Table t2( 2, 3 );
	t2.setHeader( 0, _( "Pair" ));
	t2.setHeader( 1, _( "Stronger" ));
	t2.setEntry( 0, 1, _( "Rahu/Saturn" ));
	t2.setEntry( 0, 2, _( "Ketu/Mars" ));
	t2.setEntry( 1, 1, ( isRahuStrongerThanSaturn() ? writer->getObjectName( OMEANNODE, TLARGE, true )
	                     : writer->getObjectName( OSATURN, TLARGE )) );

	t2.setEntry( 1, 2, ( isKetuStrongerThanMars() ? writer->getObjectName( OMEANDESCNODE, TLARGE, true )
	                     : writer->getObjectName( OMARS, TLARGE )));
	writer->writeTable( t2 );

	writer->writeHeader2( _( "Arudha Padas" ));
	Table t3( 2, 13 );
	t3.setHeader( 0, _( "House" ));
	t3.setHeader( 1, _( "Rasi" ));
	for ( i = 0; i < 12; i++ )
	{
		s.Printf( wxT( "%02d " ), i+1 );
		t3.setEntry( 0, i+1, s );
		t3.setEntry( 1, i+1, writer->getSignName( calcPada( i ), TLARGE ) );
	}
	writer->writeTable( t3 );

	writer->writeHeader2( _( "Years" ));
	Table t4( 2, 13 );
	t4.setHeader( 0, _( "Rasi" ));
	t4.setHeader( 1, _( "Years" ));
	for ( i = 0; i < 12; i++ )
	{
		t4.setEntry( 0, i+1, writer->getSignName( i, TLARGE ));
		s.Printf( wxT( "%02d" ), getRasiYears( i ) );
		t4.setEntry( 1, i+1, s );
	}
	writer->writeTable( t4 );
}

/*****************************************************
**
**   JaiminiExpert   ---   calcPlanetAspects
**
******************************************************/
int JaiminiExpert::calcPlanetAspects( const int &planet )
{
	int ret = 0;

	if ( hasJaiminiAspect( chart->getRasi(OJUPITER), chart->getRasi(planet) )) ret++;
	if ( hasJaiminiAspect( chart->getRasi(OMERCURY), chart->getRasi(planet) )) ret++;

	int rasi1 = chart->getRasi(planet);
	int lord1 = getJaiminiLord( rasi1 );
	if ( hasJaiminiAspect( chart->getRasi(lord1), chart->getRasi(planet) )) ret++;

	return ret;
}


/*****************************************************
**
**   JaiminiExpert   ---   calcNodeStrength
**
******************************************************/
void JaiminiExpert::calcNodeStrength()
{
	ketuStrongerThanMars = false;
	rahuStrongerThanSaturn = false;
	const int inode = config->vLunarNodeMode == LUNAR_NODE_TRUE ? OTRUENODE : OMEANNODE;
	const int descnode = config->vLunarNodeMode == LUNAR_NODE_TRUE ? OTRUEDESCNODE : OMEANDESCNODE;

	/**********************************************
	** RAHU -- SATURN
	**********************************************/
	// ** RULE 1: Jupiter aspects rahu -> rahu in fall
	if ( hasJaiminiAspect( chart->getRasi(OJUPITER), chart->getRasi(inode) ))
	{
		rahuStrongerThanSaturn = false;
	}
	// RULE 2a
	else if (( chart->getRasi(OSATURN) == AQUARIUS ) && ( chart->getRasi(inode) != AQUARIUS ))
	{
		rahuStrongerThanSaturn = true;
	}
	// RULE 2b
	else if (( chart->getRasi(OSATURN) != AQUARIUS ) && ( chart->getRasi(inode) == AQUARIUS ))
	{
		rahuStrongerThanSaturn = false;
	}
	// RULE 3a
	else if ( nbplanets[chart->getRasi(OSATURN)] > nbplanets[chart->getRasi(inode)] )
	{
		rahuStrongerThanSaturn = false;
	}
	// RULE 3b
	else if ( nbplanets[chart->getRasi(OSATURN)] < nbplanets[chart->getRasi(inode)])
	{
		rahuStrongerThanSaturn = true;
	}
	// RULE 4a
	else if ( aspSat > aspRah )
	{
		rahuStrongerThanSaturn = false;
	}
	// RULE 4b
	else if ( aspSat < aspRah )
	{
		rahuStrongerThanSaturn = true;
	}
	// RULE 5a
	else if ( hasExaltationRasi( OSATURN, chart->getRasi(OSATURN) ) && ! hasExaltationRasi( inode, chart->getRasi(inode) ))
	{
		rahuStrongerThanSaturn = false;
	}
	// RULE 5b
	else if ( ! hasExaltationRasi( OSATURN, chart->getRasi(OSATURN) ) && hasExaltationRasi( inode, chart->getRasi(inode) ))
	{
		rahuStrongerThanSaturn = true;
	}
	// RULE 8a
	else if ( chart->getRasiLength(OSATURN) > chart->getRasiLength(inode) )
	{
		rahuStrongerThanSaturn = false;
	}
	// RULE 8b
	else if ( chart->getRasiLength(OSATURN) < chart->getRasiLength(inode) )
	{
		rahuStrongerThanSaturn = true;
	}
	else
	{
		rahuStrongerThanSaturn = false;
		//exit(1);
	}

	/**********************************************
	** KETU -- MARS
	**********************************************/
	//** RULE 1: Jupiter aspects ketu -> ketu in fall
	if ( hasJaiminiAspect( chart->getRasi(OJUPITER), chart->getRasi(descnode) ))
	{
		ketuStrongerThanMars = false;
	}
	// RULE 2a
	else if (( chart->getRasi(OMARS) == SCORPIO ) && ( chart->getRasi(descnode) != SCORPIO ))
	{
		ketuStrongerThanMars = true;
	}
	// RULE 2b
	else if (( chart->getRasi(OMARS) != SCORPIO ) && ( chart->getRasi(descnode) == SCORPIO ))
	{
		ketuStrongerThanMars = false;
	}
	// RULE 3a
	else if ( nbplanets[chart->getRasi(OMARS)] > nbplanets[chart->getRasi(descnode)] )
	{
		ketuStrongerThanMars = false;
	}
	// RULE 3b
	else if ( nbplanets[chart->getRasi(OMARS)] < nbplanets[chart->getRasi(descnode)] )
	{
		ketuStrongerThanMars = true;
	}
	// RULE 4a
	else if ( aspMar > aspKet )
	{
		ketuStrongerThanMars = false;
	}
	// RULE 4b
	else if ( aspMar < aspKet )
	{
		ketuStrongerThanMars = true;
	}
	// RULE 5a
	else if ( hasExaltationRasi( OMARS, chart->getRasi(OMARS) ) && ! hasExaltationRasi( descnode, chart->getRasi(descnode) ))
	{
		ketuStrongerThanMars = false;
	}
	// RULE 5b
	else if ( ! hasExaltationRasi( OMARS, chart->getRasi(OMARS) ) && hasExaltationRasi( descnode, chart->getRasi(descnode) ))
	{
		ketuStrongerThanMars = true;
	}
	// **** TODO: RULE 6 + 7
	// RULE 8a
	else if ( chart->getRasiLength(OMARS) > chart->getRasiLength(descnode) )
	{
		ketuStrongerThanMars = false;
	}
	// RULE 8b
	else if ( chart->getRasiLength(OMARS) < chart->getRasiLength(descnode) )
	{
		ketuStrongerThanMars = true;
	}
	else
	{
		ketuStrongerThanMars = false;
		//exit(1);
	}
}

/*****************************************************
**
**   JaiminiExpert   ---   calcSignAspects
**
******************************************************/
int JaiminiExpert::calcSignAspects( const int &rasi )
{
	int ret = 0;

	if ( hasJaiminiAspect( chart->getRasi(OJUPITER), rasi )) ret++;
	if ( hasJaiminiAspect( chart->getRasi(OMERCURY), rasi )) ret++;

	int lrd = getJaiminiLord( rasi );

	if ( hasJaiminiAspect( chart->getRasi(lrd), rasi )) ret++;

	return ret;
}


/*****************************************************
**
**   JaiminiExpert   ---   calcYears
**
******************************************************/
void JaiminiExpert::calcYears()
{
	for ( int rasi = 0; rasi < 12; rasi++ )
	{

		int ll = getJaiminiLord( rasi );
		int pos = chart->getRasi( ll );

		if ( isOddFootedRasi( rasi ))
		{
			rasiYears[rasi] = red12( pos - rasi );
		}
		else
		{
			rasiYears[rasi] = red12( rasi - pos );
		}

		if ( rasiYears[rasi] == 0 )
		{
			rasiYears[rasi] = 12;
		}
		else
		{
			if (( hasExaltationRasi( ll, pos  )) && ( ll != OMERCURY  ))
			{
				rasiYears[rasi]++;
			}
			else if ( hasDebilationRasi( ll, pos  ))
			{
				rasiYears[rasi]--;
			}
		}
	}
}

/*****************************************************
**
**   JaiminiExpert   ---   getRasiYears
**
******************************************************/
int JaiminiExpert::getRasiYears( const int &rasi )
{
	assert(( rasi >= 0 ) && ( rasi < 12 ));
	return rasiYears[rasi];
}

/*****************************************************
**
**   JaiminiExpert   ---   getCharaKaraka
**
******************************************************/
int JaiminiExpert::getCharaKaraka( const int &index )
{
	assert( index >= ATMA_KARAKA && index <= DARA_KARAKA );
	return chara_karaka[index];
}

/*****************************************************
**
**   JaiminiExpert   ---   getCharaKarakaProperty
**
******************************************************/
int JaiminiExpert::getCharaKarakaProperty( const int &planet )
{
	assert(( planet >= 0 && planet < 8 ));
	return p_chara_karaka[planet];
}

/*****************************************************
**
**   JaiminiExpert   ---   calcCharaKarakas
**
******************************************************/
void JaiminiExpert::calcCharaKarakas()
{
	int i, j, p;
	double rl[8], max_len;

	const int inode = config->vLunarNodeMode == LUNAR_NODE_TRUE ? OTRUENODE : OMEANNODE;

	for ( i = 0; i < 8; i++ )
	{
		p = ( i <= OSATURN ? i : inode );
		rl[i] = chart->getRasiLength(p);
	}
	for ( i = 0; i < 8; i++ )
	{
		max_len = rl[0];
		chara_karaka[i] = OSUN;
		for ( j = OMOON; j < 8; j++ )
		{
			if ( rl[j] > max_len )
			{
				max_len = rl[j];
				chara_karaka[i] = j;
			}
		}
		rl[chara_karaka[i]] = 0;
		p_chara_karaka[chara_karaka[i]] = i;
	}
}

