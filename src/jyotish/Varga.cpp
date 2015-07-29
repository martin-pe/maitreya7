/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/Varga.cpp
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
#pragma implementation "Varga.h"
#endif

#include "Varga.h"

#include <wx/log.h>
#include <wx/string.h>
#include <math.h>
#include <vector>

#include "astrobase.h"
#include "Conf.h"
#include "constants.h"
#include "Horoscope.h"
#include "mathbase.h"
#include "Planet.h"
#include "PlanetList.h"
#include "Lang.h"
#include "VargaHoroscope.h"
#include "Writer.h"

using namespace std;

extern Config *config;

const int VargaExpert::vargaDivisions[NB_VARGAS] = {
	1, 9, 2, 3, 4, 6, 7, 8, 10, 12, 16, 20, 24, 27, 30, 40, 45, 60, 108, 144, 0
};

/*****************************************************
**
**   VargaData   ---   Constructor
**
******************************************************/
VargaData::VargaData( const Planet *iplanet, const int &d )
		:	planet( iplanet ),
		division( d )
{
	rasi = RASI_NONE;
	dignity = DIGNITY_NONE;
	vargaViswa = saptaVargajaBala = -1;
}

/**************************************************************
***
**  VargaExpert   ---   getVargaName
***
***************************************************************/
const wxChar *VargaExpert::getVargaName( const int &i ) const
{
	ASSERT_VALID_VARGA( i );
	const wxChar *vargaNames[NB_VARGAS] =
	{
		_( "Rasi" ), _( "Navamsa" ), _( "Hora" ), _( "Drekkana" ), _( "Chaturtamsa" ), _( "Sastamsa" ),
		_( "Saptamamsa" ), _( "Ashtamsa" ), _( "Dasamsa" ), _( "Dvadasamsa" ), _( "Shodasamsa" ),
		_( "Vimsamsa" ), _( "Siddhamsa" ), _( "Bhamsa" ), _( "Trimsamsa" ),
		_( "Chatvarimsamsa" ), _( "Akshavedamsa" ), _( "Shastiamsa" ),
		_( "Ashtottaramsa" ), _( "Dvadas-Davadamsa" ), _( "Bhava" )
	};
	return vargaNames[i];
}

/*****************************************************
**
**   VargaExpert   ---   getVargaIndexByDivision
**
******************************************************/
int VargaExpert::getVargaIndexByDivision( const int &div ) const
{
	int j = -1;

	// nonsense or non-initialized
	assert(( div >= 0 ) || ( div <= 144 ));

	while ( ++j < NB_VARGAS )
	{
		if ( vargaDivisions[j] == div ) return j;
	}
	assert( false );
	return 0;
}

/*****************************************************
**
**   VargaExpert   ---   getVargaDivision
**
******************************************************/
const int VargaExpert::getVargaDivision( const int &i ) const
{
	ASSERT_VALID_VARGA( i );
	return vargaDivisions[i];
}

/*****************************************************
**
**   VargaExpert   ---   getDvadasamsaLength
**
******************************************************/
double VargaExpert::getDvadasamsaLength( const double& len ) const
{
	return red_deg( getRasi( len ) * 30 + getRasiLen( len ) * 12 );
}

/*****************************************************
**
**   VargaExpert   ---   calcVarga
**
******************************************************/
int VargaExpert::calcVarga( const double& l, const int &division )
{
	ASSERT_VALID_VARGA( division );
	double len = l;
	double ret = 0;
	double basepos, rs;
	int rasi;
	assert( horoscope );

	switch ( division )
	{
		// D1
	case VARGA_RASI:
		ret = len;
		break;

		// D9
	case VARGA_NAVAMSA:
		ret =  9 * len;
		break;

		// D2
	case VARGA_HORA:
		switch ( config->vVargaHoraMode )
		{
		case 0: // Normal mode: Parasara
			ret = a_red( len - 15, 60 ) + 90;
			break;
		case 1: // Continuous
			ret = 2 * len;
			break;
		default:
			assert( false );
			break;
		}
		break;

		// D3
	case VARGA_DREKKANA:
		switch ( config->vVargaDrekkanaMode )
		{
		case 0: // normal Mode: Parasara
			ret = ( floor( getRasiLen( len ) / 10 )  * 120 + getRasi( len ) * 30 + getRasiLen( 3 * len ));
			break;
		case 1: // Continuous
			ret = 3 * len;
			break;
		default:
			assert( false );
			break;
		}
		break;

		// D4
	case VARGA_CHATURTAMSA:
		switch ( config->vVargaChaturtamsaMode )
		{
		case 0: // Normal mode: Parasara
			ret = ( floor( getRasiLen( len ) / 7.5 )  * 90 + getRasi( len ) * 30 + getRasiLen( 4 * len ));
			break;
		case 1: // Continuous
			ret = 2 * len;
			break;
		default:
			assert( false );
			break;
		}
		break;

		// D6
	case VARGA_SASTAMSA:
		ret =  6 * len;
		//ret = ( getRasi( len ) * 30 + getRasiLen( len ) * 5 );
		break;

		// D7
	case VARGA_SAPTAMAMSA:
		basepos = getRasi( len ) * 30 + getRasiLen( len ) * 7;
		if ( isOddRasi( len ) == 1 ) ret = basepos;
		else ret = basepos + 180;
		break;

		// D8
	case VARGA_ASHTAMSA:
		ret = 8 * len;
		break;

		// D10
	case VARGA_DASAMSA:
		basepos = getRasi( len ) * 30 + getRasiLen( len ) * 10;
		if ( isOddRasi( len ) == 1 ) ret = basepos;
		else ret = basepos + 240;
		break;

		// D12
	case VARGA_DVADASAMSA:
		ret = getDvadasamsaLength( len );
		break;

		// D16
	case VARGA_SHODASAMSA:
		ret = 16 * len;
		break;

		// D20
	case VARGA_VIMSAMSA:
		ret = 20 * len;
		break;

		// D24
	case VARGA_SIDDHAMSA:
		basepos = getRasiLen( len ) * 24;
		if ( isOddRasi( len ) == 1 ) ret = basepos + 120;
		else ret = basepos + 90;
		break;

		// D27
	case VARGA_BHAMSA:
		ret =  27 * len;
		break;

		// D30
	case VARGA_TRIMSAMSA:
		rs = getRasiLen( len );
		if ( isOddRasi( len ) == 1 )
		{
			// Ar, Aq, Sa, Ge, Li
			if ( rs < 5 )
				ret = 30 * ARIES + rs*6;
			else if (( rs >=  5 ) && ( rs <= 10 ))
				ret = 30 * AQUARIUS + (rs - 5)*6;
			else if (( rs >= 10 ) && ( rs <= 18 ))
				ret = 30 * SAGITTARIUS + (rs - 10)/4 * 15;
			else if (( rs >=  18 ) && ( rs <= 25 ))
				ret = 30 * GEMINI + ( rs - 18 )/7 * 30;
			else if ( rs > 25 )
				ret = 30 * LIBRA + (rs - 25)*6;
		}
		else
		{
			// Ta, Vi, Pi, Cp, Sc
			if ( rs < 5 ) ret = 30 * TAURUS + (5-rs)*6;
			else if (( rs >=  5 ) && ( rs <= 10 )) ret = 30 * VIRGO + (10-rs)*6;
			else if (( rs >= 10 ) && ( rs <= 18 )) ret = 30 * PISCES + ( 18-rs)/4 * 15;
			else if (( rs >= 18 ) && ( rs <= 25 )) ret = 30 * CAPRICORN + (25-rs)/7 * 30;
			else if ( rs > 25 ) ret = 30 * SCORPIO + ( 30 -rs)*6;
		}
		break;

		// D40
	case VARGA_CHATVARIMSAMSA:
		basepos = getRasiLen( len ) * 40;
		if ( isOddRasi( len ) == 1 ) ret = basepos;
		else ret = basepos + 180;
		break;

		// D45
	case VARGA_AKSHAVEDAMSA:
		basepos = getRasiLen( len ) * 45;
		if ( inMovableSign( len )) ret = basepos;
		else if ( inFixedSign( len )) ret = basepos + 120;
		else ret = basepos + 240;
		break;

		// D60
	case VARGA_SHASTIAMSA:
		ret = 60 * getRasiLen( len ) + getRasi( len ) * 30;
		break;

		// D108
	case VARGA_ASHTOTTARAMSA:
		ret = getDvadasamsaLength( 9 * len );
		break;

		// D144
	case VARGA_DVADASDVADASAMSA:
		ret = getDvadasamsaLength( getDvadasamsaLength ( len ));
		break;

		// Bhava
	case VARGA_BHAVA:
		for ( int i = HOUSE1; i <= HOUSE12; i++ )
		{
			assert( horoscope );
			double hstart = 0, hend = 0;
			if ( config->vHouseWestern )
			{
				hstart = horoscope->getHouse( i, true, false );
				hend = horoscope->getHouse( red12(i+1), true, false );
			}
			else
			{
				hstart = horoscope->getHouse( red12(i-1), true, true );
				hend = horoscope->getHouse( i, true, true );
			}
			if (( hstart <= len && len < hend ) || ( hstart > hend && ( len >= hstart || len < hend )))
			{
				ret = red_deg( 30 * ( i + getRasi( horoscope->getHouse( HOUSE1, true ))) + 15 );
				break;
			}
		}
		break;

	default: // invalid varga
		assert( 0 );
		break;
	}

	// ret has degree value, so get rasi
	rasi = getRasi( red_deg( ret ));
	assert( rasi >= ARIES && rasi <= PISCES );
	return rasi;
}

/*****************************************************
**
**   VargaExpert   ---   write
**
******************************************************/
void VargaExpert::write( Writer *writer, const int &style, const bool show_header )
{
	assert( horoscope );

	int p, v, gv;
	unsigned int i;
	wxString s;
	Planet *planet;
	VargaHoroscope chart( horoscope );
	Lang lang;
	PlanetList planetlist;

	if ( show_header ) writer->writeHeader1( _( "Vargas" ));

	vector<int> obs = planetlist.getVedicObjectList( style );
	vector<int> obsp = planetlist.getVedicPlanetOnlyList();

	Table t1( NB_VARGAS, obs.size()+2 );
	Table t2( NB_VARGAS, 8 );
	Table t3( NB_VARGAS, 8 );
	t1.setHeader( 0, wxT( "" ));
	for ( v = 0; v < NB_VARGAS-1; v++ )
	{
		s.Printf( wxT( "D-%02d" ), getVargaDivision(v));
		t1.setHeader( v+1, s );
		t2.setHeader( v+1, s );
		t3.setHeader( v+1, s );
	}
	int line = 1;
	for ( i = 0; i < obs.size(); i++ )
	{
		p = obs[i];
		t1.setEntry( 0, line, writer->getObjectName( p, TMEDIUM, true ));

		for ( v = 0; v < NB_VARGAS-1; v++ )
		{
			chart.setDivision( v );
			t1.setEntry( v+1, line, writer->getSignName( chart.getRasi( p )));
		}
		line++;
	}
	writer->writeTable( t1 );

	/*
	* Dignity
	*/
	writer->writeHeader2( _( "Dignities" ));
	t2.setHeader( 0, wxT( "" ));

	line = 1;
	for ( i = 0; i < obsp.size(); i++ )
	{
		p = obsp[i];
		t2.setEntry( 0, line, writer->getObjectName( p, TMEDIUM ));
		planet = horoscope->getPlanet( p );

		for ( v = 0; v < NB_VARGAS-1; v++ )
		{
			//s.Printf( wxT( "%02d" ), (int)horoscope->getVargaData( p, v )->getVargaViswa() );
			t2.setEntry( v+1, line, lang.getDignityName( horoscope->getVargaData( p, v )->getDignity(), TSHORT ) );
		}
		line++;
	}
	writer->writeTable( t2 );

	/*
	* Varga Viswa
	*/
	writer->writeHeader2( _( "Varga Viswa" ));
	t3.setHeader( 0, wxT( "" ));

	line = 1;
	for ( i = 0; i < obsp.size(); i++ )
	{
		p = obsp[i];
		t3.setEntry( 0, line, writer->getObjectName( p, TMEDIUM ));
		planet = horoscope->getPlanet( p );

		for ( v = 0; v < NB_VARGAS-1; v++ )
		{
			s.Printf( wxT( "%d" ), (int)horoscope->getVargaData( p, v )->getVargaViswa() );
			t3.setEntry( v+1, line, s );
		}
		line++;
	}
	writer->writeTable( t3 );

	writer->writeHeader2( _( "Vimsopaka Bala" ));
	Table t4( 15, 8 );
	t4.setHeader( 0, wxT( "" ));
	t4.setHeader( 1, _( "V-6" ));
	t4.setHeader( 2, _( "Own Vargas" ));
	t4.setHeader( 3, wxEmptyString );
	t4.setHeader( 4, _( "V-7" ));
	t4.setHeader( 5, _( "Own Vargas" ));
	t4.setHeader( 6, wxEmptyString );
	t4.setHeader( 7, _( "V-10" ));
	t4.setHeader( 8, _( "Own Vargas" ));
	t4.setHeader( 9, wxEmptyString );
	t4.setHeader( 10, _( "V-16" ));
	t4.setHeader( 11, _( "Own Vargas" ));
	t4.setHeader( 12, wxEmptyString );
	t4.setHeader( 13, _( "Average" ));
	t4.setHeader( 14, _( "Percent" ));
	line = 1;
	for ( i = 0; i < obsp.size(); i++ )
	{
		p = obsp[i];
		t4.setEntry( 0, line,  writer->getObjectName( p, TMEDIUM ));
		planet = horoscope->getPlanet( p );

		s.Printf( wxT( "%02.1f" ), planet->getVimsopakaBala( 0 ));
		t4.setEntry( 1,  line, s );

		gv = planet->getVimsopakaBalaGoodVargas( 0 );
		s.Printf( wxT( "%d" ), gv );
		if ( gv > 1 ) s << wxT( " - " ) << lang.getVimsopakaDignityName( 0, gv );
		t4.setEntry( 2,  line, s );

		s.Printf( wxT( "%02.1f" ), planet->getVimsopakaBala( 1 ));
		t4.setEntry( 4,  line, s );

		gv = planet->getVimsopakaBalaGoodVargas( 1 );
		s.Printf( wxT( "%d" ), gv );
		if ( gv > 1 ) s << wxT( " - " ) << lang.getVimsopakaDignityName( 1, gv );
		t4.setEntry( 5,  line, s );

		s.Printf( wxT( "%02.1f" ), planet->getVimsopakaBala( 2 ));
		t4.setEntry( 7,  line, s );

		gv = planet->getVimsopakaBalaGoodVargas( 2 );
		s.Printf( wxT( "%d" ), gv );
		if ( gv > 1 ) s << wxT( " - " ) << lang.getVimsopakaDignityName( 2, gv );
		t4.setEntry( 8,  line, s );

		s.Printf( wxT( "%02.1f" ), planet->getVimsopakaBala( 3 ));
		t4.setEntry( 10,  line, s );

		gv = planet->getVimsopakaBalaGoodVargas( 3 );
		s.Printf( wxT( "%d" ), gv );
		if ( gv > 1 ) s << wxT( " - " ) << lang.getVimsopakaDignityName( 3, gv );
		t4.setEntry( 11,  line, s );

		s.Printf( wxT( "%02.1f" ), planet->getAverageVimsopakaBala());
		t4.setEntry( 13,  line, s );

		s.Printf( wxT( "%02.1f%%" ), 5.0 * planet->getAverageVimsopakaBala());
		t4.setEntry( 14,  line, s );

		line++;
	}
	writer->writeTable( t4 );
}



