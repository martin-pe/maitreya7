/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/Ashtakavarga.cpp
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
#pragma implementation "Ashtakavarga.h"
#endif

#include "Ashtakavarga.h"

#include<wx/log.h>
#include<math.h>

#include "constants.h"
#include "Conf.h"
#include "Lang.h"
#include "mathbase.h"
#include "PlanetList.h"
#include "Varga.h"
#include "VargaHoroscope.h"
#include "Writer.h"

extern Config *config;

/*****************************************************
**
**   AshtakavargaExpert   ---   update
**
******************************************************/
void AshtakavargaExpert::update()
{
	int i, j;

	for ( i = 0; i < 8; i++ )
		for ( j = ARIES; j <= PISCES; j++ )
			rekha[i][j] = trikona[i][j] = ekadhi[i][j] = 0;

	for ( i = ARIES; i <= PISCES; i++ ) planetNumber[i] = 0;

	calcRekha();
	calcTrikonaShodana();
	calcEkadhipatyaShodana();
	calcSarva();
	calcPinda();
}

/*****************************************************
**
**   AshtakavargaExpert   ---   getPinda
**
******************************************************/
int AshtakavargaExpert::getPinda( const int &type, const int &rasi ) const
{
	switch ( type )
	{
	case GRAHAPINDA:
		return grahaPinda[rasi];
		break;
	case RASIPINDA:
		return rasiPinda[rasi];
		break;
	case YOGAPINDA:
		return yogaPinda[rasi];
		break;
	default:
		wxLogError( wxT( "Error  AshtakavargaExpert::getPinda, type is %d" ), type );
		exit(1);
		break;
	}
	return 0;
}

/*****************************************************
**
**   AshtakavargaExpert   ---   getItem
**
******************************************************/
int AshtakavargaExpert::getItem( const int &type, const int &p, const int &rasi )
{
	assert(( p >= OSUN && p <= OSATURN ) || p == OASCENDANT );
	int planet = ( p <= OSATURN ? p : 7 );
	switch ( type )
	{
	case REKHA:
		return rekha[planet][rasi];
		break;
	case TRIKONA:
		return trikona[planet][rasi];
		break;
	case EKADHI:
		return ekadhi[planet][rasi];
		break;
	default:
		wxLogError( wxT( "Error  AshtakavargaExpert::getItem, type is %d" ), type );
		exit(1);
		break;
	}
	return 0;
}

/*****************************************************
**
**   AshtakavargaExpert   ---   getSarva
**
******************************************************/
int AshtakavargaExpert::getSarva( const int &type, const int &rasi ) const
{
	assert( rasi >= ARIES && rasi <= PISCES );
	switch ( type )
	{
	case REKHA:
		return sarvaRekha[rasi];
		break;
	case TRIKONA:
		return sarvaTrikona[rasi];
		break;
	case EKADHI:
		return sarvaEkadhi[rasi];
		break;
	default:
		wxLogError( wxT( "Error  AshtakavargaExpert::getSarva, type is %d" ), type );
		exit(1);
		break;
	}
	return 0;
}

/*****************************************************
**
**   AshtakavargaExpert   ---   getPlanetSarva
**
******************************************************/
int AshtakavargaExpert::getPlanetSarva( const int &type, const int &p ) const
{
	assert(( p >= OSUN && p <= OSATURN ) || p == OASCENDANT );
	int planet = ( p <= OSATURN ? p : 7 );
	switch ( type )
	{
	case REKHA:
		return psarvaRekha[planet];
		break;
	case TRIKONA:
		return psarvaTrikona[planet];
		break;
	case EKADHI:
		return psarvaEkadhi[planet];
		break;
	default:
		wxLogError( wxT( "Error  AshtakavargaExpert::getPlanetSarva, type is %d" ), type );
		exit(1);
		break;
	}
	return 0;
}

/*****************************************************
**
**   AshtakavargaExpert   ---   write
**
******************************************************/
void AshtakavargaExpert::write( Writer *writer, const bool show_header )
{
	VargaExpert expert;
	wxString s;
	int i;

	if ( show_header )
	{
		writer->writeHeader1( _( "Ashtakavarga" ));
		//s << _T( "Varga" ) << wxT( ": " ) << expert.getVargaName(d) << wxT( " D" ) << expert.getVargaDivision(d);
		//writer->writeLine( s );
	}

	writer->writeHeader2( _( "Rekha" ));
	writeSingleAv( writer, REKHA );
	writer->writeHeader2( _( "Trikona Shodana" ));
	writeSingleAv( writer, TRIKONA );
	writer->writeHeader2( _( "Ekadhipatya Shodana" ));
	writeSingleAv( writer, EKADHI );

	Table table( 4, 8 );
	writer->writeHeader2( _( "Pinda" ));
	table.setHeader( 0,  _( "Planet" ));
	table.setHeader( 1,  _( "Rasi" ));
	table.setHeader( 2,  _( "Graha" ));
	table.setHeader( 3,  _( "Sodhya" ));

	int line = 1;
	vector<int> obs = PlanetList().getDefaultVedicObjectList();
	for ( unsigned int p = 0; p < obs.size(); p++ )
	{
		i = obs[p];
		if ( i > OSATURN && i != OASCENDANT ) continue;
		table.setEntry( 0, line, writer->getObjectName( i, TLARGE ), config->usePlanetSymbols );
		s.Printf( wxT( "%3d" ), getPinda( RASIPINDA, i ));
		table.setEntry( 1, line, s );
		s.Printf( wxT( "%3d" ), getPinda( GRAHAPINDA, i ));
		table.setEntry( 2, line, s );
		s.Printf( wxT( "%3d" ), getPinda( YOGAPINDA, i ));
		table.setEntry( 3, line, s );
		line++;
	}
	writer->writeTable( table );
}

/*****************************************************
**
**   AshtakavargaExpert   ---   writeSingleAv
**
******************************************************/
void AshtakavargaExpert::writeSingleAv( Writer *writer, const int&w )
{
	int i, j, line = 1;
	wxString s1;
	Table table( 13, 10 );
	table.setHeader( 0, wxEmptyString);
	for ( i = ARIES; i <= PISCES; i++ )
	{
		table.setHeader( i+1, writer->getSignName( i, TMEDIUM), config->usePlanetSymbols );
	}

	vector<int> obs = PlanetList().getDefaultVedicObjectList();
	obs.push_back( OASCENDANT );
	for ( unsigned int p = 0; p < obs.size(); p++ )
	{
		i = obs[p];
		if ( i > OSATURN && i != OASCENDANT ) continue;
		table.setEntry( 0, line, writer->getObjectName( i, TLARGE ), config->usePlanetSymbols );
		for ( j = ARIES; j <= PISCES; j++ )
		{
			//s1.Printf( wxT( "%02d "), getItem( w, i, j ));
			s1.Printf( wxT( "%2d "), getItem( w, i, j ));
			table.setEntry( j+1, line, s1 );
		}
		line++;
	}
	table.setEntry( 0, 9, _( "Sarva" ));
	for ( j = ARIES; j <= PISCES; j++ )
	{
		//s1.Printf( wxT( "%02d " ), getSarva( w, j ));
		s1.Printf( wxT( "%2d " ), getSarva( w, j ));
		table.setEntry( j+1, 9, s1 );
	}

	writer->writeTable( table );
}

/*****************************************************
**
**   AshtakavargaExpert   ---   getSingleRekha
**
******************************************************/
int AshtakavargaExpert::getSingleRekha( const int &i, const int &j, const int &k )
{
	assert( i >= 0 && i < 8 );
	assert( j >= 0 && j < 8 );
	assert( k >= 0 && k < 12 );

	static const char REKHA_MAP[8][8][12] =
	{
		{ // Sun
			{ 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0 }, // Sun
			{ 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0 }, // Moon
			{ 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1 }, // Mercury
			{ 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1 }, // Venus
			{ 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0 }, // Mars
			{ 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0 }, // Jupiter
			{ 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0 }, // Saturn
			{ 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1 }  // Ascendant
		},
		{ // Moon
			{ 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0 }, // Sun
			{ 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0 }, // Moon
			{ 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0 }, // Mercury
			{ 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0 }, // Venus
			{ 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 0 }, // Mars
			{ 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0 }, // Jupiter
			{ 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0 }, // Saturn
			{ 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0 }  // Ascendant
		},
		{ // Mercury
			{ 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1 }, // Sun
			{ 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0 }, // Moon
			{ 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1 }, // Mercury
			{ 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 0 }, // Venus
			{ 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0 }, // Mars
			{ 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1 }, // Jupiter
			{ 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0 }, // Saturn
			{ 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0 }  // Ascendant
		},
		{  // Venus
			{ 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1 }, // Sun
			{ 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1 }, // Moon
			{ 0, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0 }, // Mercury
			{ 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0 }, // Venus
			{ 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1 }, // Mars
			{ 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0 }, // Jupiter
			{ 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0 }, // Saturn
			{ 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 0 }  // Ascendant
		},
		{ // Mars
			{ 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0 }, // Sun
			{ 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0 }, // Moon
			{ 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0 }, // Mercury
			{ 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1 }, // Venus
			{ 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0 }, // Mars
			{ 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1 }, // Jupiter
			{ 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0 }, // Saturn
			{ 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0 }  // Ascendant
		},
		{ // Jupiter
			{ 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0 }, // Sun
			{ 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0 }, // Moon
			{ 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0 }, // Mercury
			{ 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0 }, // Venus
			{ 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0 }, // Mars
			{ 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0 }, // Jupiter
			{ 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1 }, // Saturn
			{ 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0 }  // Ascendant
		},
		{  // Saturn
			{ 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0 }, // Sun
			{ 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0 }, // Moon
			{ 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1 }, // Mercury
			{ 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1 }, // Venus
			{ 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1 }, // Mars
			{ 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1 }, // Jupiter
			{ 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0 }, // Saturn
			{ 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0 }  // Ascendant
		},
		{  // Ascendant
			{ 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1 }, // Sun
			{ 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1 }, // Moon
			{ 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0 }, // Mercury
			{ 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0 }, // Venus
			{ 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0 }, // Mars
			{ 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0 }, // Jupiter
			{ 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0 }, // Saturn
			{ 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0 }  // Ascendant
		}
	};
	// Varaha Mihira
	if ( config->vAshtakavargaMode )
	{
		if ( i == OMOON )
		{
			if ( j == OMOON && k == HOUSE9 ) return 0;     // Moon malefic in  9 from Moon
			if ( j == OMARS && k == HOUSE9 ) return 1;     // Moon benefic in  9 from Mars
			if ( j == OJUPITER && k == HOUSE2 ) return 0;  // Moon malefic in  2 from Jupiter
			if ( j == OJUPITER && k == HOUSE12 ) return 1; // Moon benefic in 12 from Jupiter
		}
		else if ( i == OVENUS )
		{
			if ( j == OMARS && k == HOUSE4 ) return 0;     // Venus malefic in 4 from Mars
			if ( j == OMARS && k == HOUSE5 ) return 1;     // Venus benefic in 5 from Mars
		}
	}
	return REKHA_MAP[i][j][k];
}

/*****************************************************
**
**   AshtakavargaExpert   ---   calcRekha
**
******************************************************/
void AshtakavargaExpert::calcRekha()
{
	int i, j, k, house, p1, p2;

	for ( i = 0; i < 8; i++ )
	{
		i < 7 ? p1 = i : p1 = OASCENDANT;
		for ( j = 0; j < 8; j++ )
		{
			j < 7 ? p2 = j : p2 = OASCENDANT;
			for ( k = 0; k < 12; k++ )
			{
				house = red12( chart->getRasi(p2) + k );
				if ( getSingleRekha( i, j, k ) ) rekha[i][house]++;
			}
		}
	}
}

/*****************************************************
**
**   AshtakavargaExpert   ---   calcSarva
**
******************************************************/
void AshtakavargaExpert::calcSarva()
{
	int i, j, jmax;
	for ( i = 0; i < 12; i++ ) sarvaRekha[i] = sarvaTrikona[i] = sarvaEkadhi[i] = 0;
	for ( i = 0; i < 8; i++ ) psarvaRekha[i] = psarvaTrikona[i] = psarvaEkadhi[i] = 0;

	jmax = config->vAshtakavargaSarva ? 8 : 7;
	for ( i = 0; i < 12; i++ )
	{
		for ( j = 0; j < jmax; j++ )
		{
			sarvaRekha[i] += rekha[j][i];
			sarvaTrikona[i] += trikona[j][i];
			sarvaEkadhi[i] += ekadhi[j][i];

			psarvaRekha[j] += rekha[j][i];
			psarvaTrikona[j] += trikona[j][i];
			psarvaEkadhi[j] += ekadhi[j][i];
		}
	}
}

/*****************************************************
**
**   AshtakavargaExpert   ---   calcPinda
**
******************************************************/
void AshtakavargaExpert::calcPinda()
{
	int rasi, planet, p;

	// Numbers for virgo and Capricorn are not clear: some say 5,  others 6, others vi 6 and cp 5
	const int k_rasimana[12] = { 7, 10, 8, 4, 10, 6, 7, 8, 9, 5, 11, 12 };
	//const int k_rasimana[12] = { 7, 10, 8, 4, 10, 5, 7, 8, 9, 5, 11, 12 };

	// Grahamana
	// so=5 mo=5 ma=8 me=5 ju=10 ve=7 sa=5
	const int k_grahamana[7] = { 5, 5, 5, 7, 8, 10, 5 };

	for ( int i = ARIES; i <= PISCES; i++ )
		rasiPinda[i] = grahaPinda[i] = yogaPinda[i] = 0;

	for ( planet = 0; planet <= 8; planet++ )
	{
		for ( rasi = ARIES; rasi <= PISCES; rasi++ )
		{
			rasiPinda[planet] += ekadhi[planet][rasi] * k_rasimana[rasi];

			// Sodhya Pinda: Mantreswar
			if ( config->vAshtakavargaMantreswarMode ) yogaPinda[planet] += ekadhi[planet][rasi];
		}
		for ( p = 0; p < 7; p++ )
		{
			grahaPinda[planet] += ekadhi[planet][chart->getRasi(p)] * k_grahamana[p];
		}

		// Sodhya Pinda: Parasara
		if ( ! config->vAshtakavargaMantreswarMode )
			yogaPinda[planet] = grahaPinda[planet] + rasiPinda[planet];
	}
}

/*****************************************************
**
**   AshtakavargaExpert   ---   calcTrikonaShodana
**
******************************************************/
void AshtakavargaExpert::calcTrikonaShodana()
{
	wxChar minrekha;
	for ( int i = 0; i < 8; i++ )
	{
		for ( int j = 0; j < 4; j++ )
		{
			minrekha = Min( rekha[i][j], rekha[i][j+4] );
			minrekha = Min( rekha[i][j+8], minrekha );

			for ( int k = 0; k < 3; k++ ) trikona[i][j+4*k] = rekha[i][j+4*k] - minrekha;
		}
	}
}

/*****************************************************
**
**   AshtakavargaExpert   ---   calcEkadhipatyaPair
**
******************************************************/
void AshtakavargaExpert::calcEkadhipatyaPair( const int &rasi1, const int &rasi2 )
{
	int p;

	for ( p = 0; p < 12; p++ )
	{
		/*********************************************************
		*** (0)
		** one rasi has got a number in Trikona Shodana
		** and the other ist bereft of
		** any number -> nothing to be done
		***
		*********************************************************/
		if ( ! trikona[p][rasi1] || ! trikona[p][rasi2])
		{
		}

		/*********************************************************
		*** (1)
		** both rasis without a planet and Trikona Shodana different
		** -> both are given the smaller number
		***
		***  BUG FIX 5.0:
		***  -> No test for Trikona different, so condition (5) was never reached
		*********************************************************/
		else if (( ! planetNumber[rasi1] ) && ( ! planetNumber[rasi2] ) && ( trikona[p][rasi1] != trikona[p][rasi2]) )
		{
			ekadhi[p][rasi1] = ekadhi[p][rasi2] = Min( trikona[p][rasi1], trikona[p][rasi2] );
		}

		/*********************************************************
		*** (2)
		** both rasis with planets -> no shodana
		***
		*********************************************************/
		else if (( planetNumber[rasi1] ) && ( planetNumber[rasi2] ))
		{
		}

		/*********************************************************
		*** (3)
		** one rasi is with a planet and a smaller trikona number
		** an the other is without a planet and a bigger trikona number
		** -> deduct the smaller number from the bigger number
		**  and the number of the rasi with planet should be kept unchanged
		***
		*********************************************************/
		else if (( planetNumber[rasi1] ) && ( ! planetNumber[rasi2] )
		         && ( trikona[p][rasi1] < trikona[p][rasi2]))
		{
			ekadhi[p][rasi2] -= trikona[p][rasi1];
		}

		else if (( planetNumber[rasi2] ) && ( ! planetNumber[rasi1] )
		         && ( trikona[p][rasi2] < trikona[p][rasi1]))
		{
			ekadhi[p][rasi1] -= trikona[p][rasi2];
		}

		/*********************************************************
		*** (4)
		** if the rasi with the planet has a bigger number than that
		** of the rasi without planet
		** -> shodana with the number of the rasi without planet
		** and the number of the rasi with planet schould be kept unchaged
		***
		*********************************************************/
		else if (( planetNumber[rasi1] ) && ( ! planetNumber[rasi2] )
		         && ( trikona[p][rasi1] > trikona[p][rasi2]))
		{
			ekadhi[p][rasi2] = 0;
		}
		else if (( planetNumber[rasi2] ) && ( ! planetNumber[rasi1] )
		         && ( trikona[p][rasi2] > trikona[p][rasi1]))
		{
			ekadhi[p][rasi1] = 0;
		}

		/*********************************************************
		*** (5)
		**  if both the rasis are without a planet an possess the
		**  same numbers
		**  -> shodana with both the numbers
		***
		*********************************************************/
		else if (( ! planetNumber[rasi1] ) && ( ! planetNumber[rasi2] )
		         && ( trikona[p][rasi1] == trikona[p][rasi2]))
		{
			ekadhi[p][rasi1] = ekadhi[p][rasi2] = 0;
		}

		/*********************************************************
		*** (6)
		** if one rasi is with an planet and the other is without
		** any planet
		** -> the number of the latter should be reduced to zero
		***
		*********************************************************/
		else if (( planetNumber[rasi1] ) && ( ! planetNumber[rasi2] ))
		{
			ekadhi[p][rasi2] = 0;
		}
		else if (( planetNumber[rasi2] ) && ( ! planetNumber[rasi1] ))
		{
			ekadhi[p][rasi1] = 0;
		}
	}
}

/*****************************************************
**
**   AshtakavargaExpert   ---   calcEkadhipatyaShodana
**
******************************************************/
void AshtakavargaExpert::calcEkadhipatyaShodana()
{
	int i, j;

	// calculate number of planets in rasi
	for ( i = OSUN; i < OSATURN; i++ ) planetNumber[chart->getRasi(i)]++;

	for ( i = 0; i < 8; i++ )
		for ( j = 0; j < 12; j++ )
			ekadhi[i][j] = trikona[i][j];

	calcEkadhipatyaPair( ARIES, SCORPIO );
	calcEkadhipatyaPair( TAURUS, LIBRA );
	calcEkadhipatyaPair( GEMINI, VIRGO );
	calcEkadhipatyaPair( SAGITTARIUS, PISCES );
	calcEkadhipatyaPair( CAPRICORN, AQUARIUS );
}


