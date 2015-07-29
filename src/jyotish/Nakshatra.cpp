/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/Nakshatra.cpp
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
#pragma implementation "Nakshatra.h"
#endif

#include "Nakshatra.h"

#include "astrobase.h"
#include "ChartProperties.h"
#include "Conf.h"
#include "Horoscope.h"
#include "Lang.h"
#include "mathbase.h"
#include "Writer.h"

#include <wx/string.h>
#include <math.h>
#include <assert.h>

extern Config *config;

/*****************************************************
**
**   NakshatraExpert   ---   getYoni
**
******************************************************/
int NakshatraExpert::getYoni( const double& len ) const
{
	//const wxChar *k_yoni_name[14] = { _( "Horse" ), _( "Buffalo" ), _( "Lion" ), _( "Elephant" ), _( "Goat" ), _( "Monkey" ),
	//_( "Mongoose" ), _( "Serpent" ), _( "Deer" ), _( "Dog" ), _( "Cat" ), _( "Rat" ), _( "Tiger" ), _( "Cow"  ) };
	assert( len >= 0 && len <= 360 );
	int nakshatra = getNakshatra( len, N27 );
	const int yoni_map[27] =
	{
		0, 3, 4, 7, 7, 9, 10, 4, 10,
		11, 11, 13, 1, 12, 1, 12, 8, 8,
		9, 5, 6, 5, 2, 0, 2, 13, 3
	};
	return yoni_map[nakshatra];
}

/*****************************************************
**
**   NakshatraExpert   ---   getNadi
**
******************************************************/
int NakshatraExpert::getNadi( const double& len ) const
{
	assert( len >= 0 && len <= 360 );
	int nakshatra = getNakshatra( len, N27 );

	const int nadi_map[27] =
	{
		0, 1, 2, 2, 1, 0, 0, 1, 2,
		2, 1, 0, 0, 1, 2, 2, 1, 0,
		0, 1, 2, 2, 1, 0, 0, 1, 2
	};
	return nadi_map[nakshatra];
}

/*****************************************************
**
**   NakshatraExpert   ---   getGana
**
******************************************************/
int NakshatraExpert::getGana( const double& len ) const
{
	assert( len >= 0 && len <= 360 );
	int nakshatra = getNakshatra( len, N27 );

	const int gana_map[27] =
	{
		0, 1, 2, 1, 0, 1, 0, 0, 2,
		2, 1, 1, 0, 2, 0, 2, 0, 2,
		2, 1, 1, 0, 2, 2, 1, 1, 0
	};
	return gana_map[nakshatra];
}

/*****************************************************
**
**   NakshatraExpert   ---   getTara
**
******************************************************/
int NakshatraExpert::getTara( const int& nak ) const
{
	int n = nak;
	if ( n < 0 ) n += 27;
	assert( n >= 0 && n < 27 );
	return n % 9;
}

/*****************************************************
**
**   NakshatraExpert   ---   getSbcNakshatraProperties
**
******************************************************/
void NakshatraExpert::getSbcNakshatraProperties( const int &birthStar, vector<NakshatraProperties> &props )
{
	props.clear();
	int nak27 = 0;  // Aswini

	for( int nak28 = 0; nak28 < 28; nak28++ )
	{
		NakshatraProperties p;
		if ( nak28 == ABHIJIT_INDEX )
		{
			p.tara = -1;
			p.taraQuality = TARA_PROP_NEUTRAL;
		}
		else
		{
			p.tara = getTara( nak27 - birthStar );
			//printf( "Birth %d nak28 %d nak27 %d tara %d\n", birthStar, nak28, nak27, p.tara );
			assert( p.tara >= 0 && p.tara < 9 );
			switch( p.tara )
			{
				case 1:
				case 3:
				case 5:
				case 7:
					p.taraQuality = TARA_PROP_BENEFIC;
				break;
				case 0:
				case 2:
				case 4:
				case 6:
				case 8:
					p.taraQuality = TARA_PROP_MALEFIC;
				break;
				default:
					assert( 0 );
				break;
			}
			nak27++;
		}
		props.push_back( p );
	}
}


/*****************************************************
**
**   NakshatraExpert   ---   write
**
******************************************************/
void NakshatraExpert::write( Writer *writer, const ChartProperties *chartprops, const bool show_header )
{
	assert( chartprops );
	assert( horoscope );
	assert( writer );

	wxString s;
	int i;
	double len;
	int nakshatramode =  ( config->vNakshatraMode28 ? N28 : N27 );

	Lang lang;
	vector<int> obs = chartprops->getVedicPlanetList();

	if ( show_header ) writer->writeHeader1( _( "Nakshatras" ));
	Table table( nakshatramode == N27 ? 7 : 3 , obs.size() + 1 );
	table.setHeader( 0,  _( "Planet" ));
	table.setHeader( 1,  _( "Length" ));
	table.setHeader( 2,  _( "Nakshatra" ));
	if ( nakshatramode == N27 )
	{
		table.setHeader( 3,  _( "Yoni" ));
		table.setHeader( 4,  _( "Gana" ));
		table.setHeader( 5,  _( "Nadi" ));
		table.setHeader( 6,  _( "Tara" ));
	}

	int line = 1;
	for ( unsigned int p = 0; p < obs.size(); p++ )
	{
		i = obs[p];
		len = horoscope->getObjectPosition( i, true ).length;
		table.setEntry( 0, line, writer->getObjectName( i, TLARGE, true ));
		table.setEntry( 1, line, writer->getPosFormatted( len ));
		table.setEntry( 2, line, lang.getNakshatraName( ::getNakshatra( len, nakshatramode ), nakshatramode, TLARGE) );

		// This is not defined for 28 nakshatra mode, so leave it out
		if ( nakshatramode == N27 )
		{
			table.setEntry( 3, line, lang.getYoniName( getYoni( len )) );
			table.setEntry( 4, line, lang.getGanaName( getGana( len )) );
			table.setEntry( 5, line, lang.getNadiName( getNadi( len )) );
			table.setEntry( 6, line, lang.getTaraName(
			                    getTara(  ::getNakshatra( len, N27 ) - ::getNakshatra( horoscope->getObjectPosition( OMOON, true ).length, N27 ))));
		}
		line++;
	}
	writer->writeTable( table );
}

/*****************************************************
**
**   NakshatraHoroscope   ---   Constructor
**
******************************************************/
NakshatraHoroscope::NakshatraHoroscope( const Horoscope *h, const int mode )
		: horoscope( h ),
		mode( mode )
{
	assert( mode == N27 || mode == N28 );
}

/*****************************************************
**
**   NakshatraHoroscope   ---   getNakshatra
**
******************************************************/
int NakshatraHoroscope::getNakshatra( const int &planet ) const
{
	if ( horoscope )
	{
		return ::getNakshatra( horoscope->getVedicLength( planet ), mode );
	}
	else printf( "WARN: horoscope in NakshatraHoroscope::getNakshatra is NULL, returning Aswini\n" );
	return 0;
}

/*****************************************************
**
**   NakshatraHoroscope   ---   isRetrograde
**
******************************************************/
bool NakshatraHoroscope::isRetrograde( const int &planet ) const
{
	if ( horoscope ) return horoscope->isRetrograde( planet );
	else printf( "WARN: horoscope in NakshatraHoroscope::isRetrograde is NULL, returning false\n" );
	return false;
}

/*****************************************************
**
**   SarvatobhadraExpert   ---   writeSbcText
**
******************************************************/
void SarvatobhadraExpert::writeSbcText( Writer *writer, const ChartProperties *props )
{
	Lang lang;
	int i;
	int nak_mode = ( config->vNakshatraMode28 ? N28 : N27 );
	double len;

	vector<int> obs = props->getVedicPlanetList();
	Table table( 3, obs.size() + 1 );
	table.setHeader( 0,  _( "Planet" ));
	table.setHeader( 1,  _( "Length" ));
	table.setHeader( 2,  _( "Nakshatra" ));
	int line = 1;

	for ( unsigned int p = 0; p < obs.size(); p++ )
	{
		i = obs[p];
		table.setEntry( 0, line,  writer->getObjectName( i, TMEDIUM, true ));
		len = horoscope->getVedicLength( i );
		table.setEntry( 1, line,  writer->getPosFormatted( len, horoscope->isRetrograde( i )));
		table.setEntry( 2, line,  lang.getNakshatraName( getNakshatra( len, nak_mode ), nak_mode ) );
		line++;
	}
	writer->writeTable( table );
}


/*****************************************************
**
**   SarvatobhadraExpert   ---   writeSbcAfflictions
**
******************************************************/
void SarvatobhadraExpert::writeSbcAfflictions( Writer *writer, const Horoscope *h2, const ChartProperties *props )
{
	Lang lang;
	int i, j, k, l, diff, star, rnak, nak, nak_index;
	const int special_nak[9] = { 0, 9, 15, 17, 18, 22, 24, 25, 26 };
	const wxChar *special_nak_name[9] = { _( "Janma" ), _( "Karmaksha" ), _( "Sanghatika" ),
	                                      _( "Samudayika" ), _( "Adhana" ), _( "Vinasana" ), _( "Jati" ), _( "Desa" ), _( "Abhisheka" )
	                                    };
	int vedas[4];
	wxString s1;

	int nakmode = config->vNakshatraMode28;

	writer->writeHeader1( _( "Afflictions" ));

	star = getNakshatra( horoscope->getVedicLength( OMOON ), nakmode );

	vector<int> obs = props->getVedicPlanetList();
	for ( unsigned int p = 0; p < obs.size(); p++ )
	{
		i = obs[p];
		nak = getNakshatra( h2->getVedicLength( i ), nakmode );
		s1.Printf( wxT( "%s (%s)" ), (const wxChar*)writer->getObjectName(i, TLARGE, true ),
		           (const wxChar*)lang.getNakshatraName( nak, nakmode, TMEDIUM ));
		writer->writeHeader3( s1 );
		vedas[0] = nak;
		nak_index = nak;
		if ( nakmode == N27 && nak > ABHIJIT ) nak_index++;
		vedas[1] = nak_aspects[nak_index][1];
		vedas[2] = nak_aspects[nak_index][2];
		vedas[3] = nak_aspects[nak_index][3];
		if ( nakmode == N27 )
		{
			for ( j = 1; j < 4; j++ )
			{
				if ( vedas[j] == ABHIJIT ) vedas[j] = -1;
				else if ( vedas[j] > ABHIJIT ) vedas[j]--;
			}
		}
		for ( j = 0; j < 4; j++ )
		{
			if ( vedas[j] == -1 ) continue;

			// affliction of special nakshatras
			//if ( nakmode == N27 )
			if ( false )
			{
				for ( l = 0; l < 9; l++ )
				{
					k = star + special_nak[l];
					if ( k >= ABHIJIT ) k++;
					diff = (int)a_red( k - vedas[j], 28 );
					if ( diff == 0 )
					{
						s1.Printf( wxT( "%6s: %s" ), (const wxChar*)lang.getNakshatraName( vedas[j], nakmode, TMEDIUM ), special_nak_name[l] );
						writer->writeLine( s1 );
					}
				}
			}

			// Radix Nakshatra afflictions
			for ( unsigned int q = 0; q < obs.size(); q++ )
			{
				k = obs[q];
				rnak = getNakshatra( horoscope->getVedicLength( k ), nakmode );
				if ( rnak != vedas[j] ) continue;
				if ( horoscope == h2 && i == k ) continue;
				s1.Printf( wxT( "%6s: %s" ), (const wxChar*)lang.getNakshatraName( vedas[j], nakmode, TMEDIUM ),
				           (const wxChar*)writer->getObjectName( k, TLARGE, true ));
				writer->writeLine( s1 );
			}
		}
	}
}


