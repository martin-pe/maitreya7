/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/GenericDasa.cpp
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
#pragma implementation "GenericDasa.h"
#endif

#include "GenericDasa.h"

#include "astrobase.h"
#include "Calculator.h"
#include "Dasa.h"
#include "Horoscope.h"
#include "Lang.h"
#include "mathbase.h"
#include "Planet.h"

/*****************************************************
**
**   GenericNakshatraDasaExpert   ---   getPortion
**
******************************************************/
double GenericNakshatraDasaExpert::getPortion( const Horoscope *h, int &nak )
{
	// Generic Calculation based upon the Moon, overwritten by Lagna Vimsottari Dasa
	Calculator *calculator = CalculatorFactory().getCalculator();
	double mlen = h->getObjectPosition( OMOON, true ).length;
	nak = (int)( mlen / NAKSHATRA_LEN );
	return calculator->calcNakshatraPortion( h->getDataSet(), mlen, false );
}

/*****************************************************
**
**   GenericNakshatraDasaExpert   ---   getNextLevel
**
******************************************************/
vector<Dasa*> GenericNakshatraDasaExpert::getNextLevel( Dasa *dasa )
{
	vector<Dasa*> ret;

	double start_jd = dasa->getStartJD();
	double end_jd = dasa->getEndJD();
	double dasa_len = end_jd - start_jd;
	double antar_len = 0;
	int lord = -1;
	for ( int i = 0; i < getNbLords(); i++ )
	{
		if ( getDasaLordByIndex( i ) == dasa->getDasaLord() )
		{
			lord = i;
			break;
		}
	}
	assert( lord != -1 );

	for ( int i = 0; i < getNbLords(); i++ )
	{
		antar_len = ( dasa_len * getDasaDuration( lord )) / getTotalLength();
		end_jd = start_jd + antar_len;

		ret.push_back( new DasaImpl( this, getDasaLordByIndex( lord ), start_jd, end_jd,
		                             getExtraText( lord ), ( dasa->getParent() == dasa ? 0 : dasa ) ));

		lord = ( lord + 1 ) % getNbLords();
		start_jd = end_jd;
	}
	return ret;
}

/*****************************************************
**
**   GenericNakshatraDasaExpert   ---   getFirstLevel
**
******************************************************/
vector<Dasa*> GenericNakshatraDasaExpert::getFirstLevel( const Horoscope *h, const int varga )
{
	vector<Dasa*> ret, tret;
	double nportion;
	int total_length = 0, nak;

	nportion = getPortion( h, nak );
	int pindex = getDasaIndex4Nakshatra( nak );
	int pl = getDasaLordByIndex( pindex );

	double start_jd = h->getJD() - nportion * getDasaDuration( pindex ) * getYearLength( true );
	double end_jd = start_jd + getTotalLength() * getYearLength( true );

	while ( total_length < 100 )
	{
		DasaImpl d( this, pl, start_jd, end_jd, 0, 0 );
		d.setParent( &d );
		tret = getNextLevel( &d );
		for ( unsigned j = 0; j < tret.size(); j++ )
		{
			ret.push_back( tret[j] );
		}
		tret.clear();
		total_length += getTotalLength();
		start_jd = end_jd;
		end_jd += getTotalLength() * getYearLength( true );
	}
	return ret;
}

/*****************************************************
**
**   GenericNakshatraDasaExpert   ---   getKPLordRecursive
**
******************************************************/
void GenericNakshatraDasaExpert::getKPLordRecursive( int &lord_index, double &elapsed, double &total_len )
{
	int count = 0;
	const int max_loop = 10; // maximum number of antardasas in cycle
	double antar_len; // in years

	if ( elapsed >= total_len ) printf( "ERROR GenericNakshatraDasaExpert::getKPLordRecursive: elapsed time too big, %f total was %f\n", elapsed, total_len );
	assert ( elapsed < total_len );

	antar_len = getDasaDuration( lord_index ) * total_len;
	antar_len /= getTotalLength();
	//printf( "Elapsed 1 %f antar_len %f total_len %f dur1 %d dur2 %d\n", elapsed, antar_len, total_len, getDasaDuration( lord_index ), getDasaDuration( lord_index ) );

	// count so many antardasas that elapsed time is more than antarlen
	while (( elapsed > antar_len ) && ( count++ < max_loop  ))
	{
		lord_index++;
		if ( lord_index >= getNbLords() ) lord_index -= getNbLords();
		elapsed -= antar_len;

		antar_len = getDasaDuration( lord_index ) * total_len;
		antar_len /= getTotalLength();
		//printf( "Elapsed 2 %f antar_len %f\n", elapsed, antar_len );
	}
	if ( count >= max_loop ) printf( "ERROR GenericNakshatraDasaExpert::getKPLordRecursive: count %d max_loop %d\n", count, max_loop );
	assert( count < max_loop );

	total_len = antar_len;
}

/*****************************************************
**
**   GenericNakshatraDasaExpert   ---   getKPLords
**
******************************************************/
KpData GenericNakshatraDasaExpert::getKPLords( const double &len )
{
	KpData kp;

	// 0...27 - ASWINI = 0 etc
	int nak = getNakshatra( len, N27 );

	// index of the first lord for that Nakshatra in the sequence of lords, e.g. Vimsottari has Sun, Moon, Mars, Rahu, Jupiter, Saturn, Mercury, Ketu, Venus
	int lord_index = getDasaIndex4Nakshatra( nak );

	// elapsed portion in the Nakshatra (0...1)
	double nakshatraPortion = getNakshatraLength( len, N27 ) / NAKSHATRA_LEN;

	// elapsed time in years
	double elapsed = nakshatraPortion * getDasaDuration( lord_index );

	// total length in years (e.g. 120 for Vimsottari)
	double total_len = getTotalLength();

	//printf( "START ------  CAll 1 KP len %f lord_index %d, portion %f elapsed %f total_len %f\n", len, lord_index, nakshatraPortion, elapsed, total_len );
	getKPLordRecursive( lord_index, elapsed, total_len );
	kp.lord = getDasaLordByIndex( lord_index );
	//printf( "CAll 2 KP len %f lord_index %d KPLORD %d, elapsed %f total_len %f\n", len, lord_index, kp.lord, elapsed, total_len );

	getKPLordRecursive( lord_index, elapsed, total_len );
	kp.sublord = getDasaLordByIndex( lord_index );
	//printf( "CAll 3 KP len %f lord_index %d KPLORD %d, elapsed %f total_len %f\n\n", len, lord_index,  kp.sublord, elapsed, total_len );

	getKPLordRecursive( lord_index, elapsed, total_len );
	kp.subsublord = getDasaLordByIndex( lord_index );
	//printf( "FINISHED   ------  CAll 4 KP len %f lord_index %d KPLORD %d, elapsed %f total_len %f\n\n", len, lord_index,  kp.subsublord, elapsed, total_len );

	return kp;
}

/*****************************************************
**
**   GenericNakshatraDasaExpert   ---   getKPEventList
**
******************************************************/
vector<KPEvent> GenericNakshatraDasaExpert::getKPEventList( const double &startlen, const double &endlen,
        const double &startjd )
{
	vector<KPEvent> events;
	int currentnak, currentlord, i, j, antarlord, antardasatime;
	double dasastartlen, antarstartlen, dist;
	bool append;

	int startnak = getNakshatra( startlen, N27 );

	for ( i = 0; i < 32; i++ )
	{
		currentnak = (startnak + i ) % 27;
		currentlord = getDasaIndex4Nakshatra( currentnak );
		dasastartlen = red_deg( ( startnak + i ) * 13.33333333 );
		antarstartlen = dasastartlen;

		for ( j = 0; j < getNbLords(); j++ )
		{
			append = true;
			antarlord = ( currentlord  + j ) % getNbLords();
			antardasatime = getDasaDuration( antarlord ); // years

			if ( i == 0 ) // cut off at the beginning (antardasas before starting point)
			{
				dist = antarstartlen - startlen;
				if ( dist < -300 ) dist += 360;  // correct if 360-deg shift
				if ( dist < 0 ) append = false;
			}
			if ( i >= 28 )  // cut off at the end (if event belongs to next time interval)
			{
				dist = antarstartlen - endlen;
				if ( dist > 300 ) dist -= 360;
				if ( dist < -300 ) dist += 360;
				if ( dist > 0 ) append = false;     // could also be return ?
			}
			if ( append )
				events.push_back( KPEvent( getDasaLordByIndex( currentlord ), getDasaLordByIndex( antarlord ),
				                           antarstartlen + .0000001, startjd + i + (double)j/getNbLords(), i ));
			antarstartlen += 13.333333333 * (double)antardasatime / getTotalLength();
		}
	}

	return events;
}

/*****************************************************
**
**   YoginiDasaExpert   ---   getDasaLordByIndex
**
******************************************************/
int YoginiDasaExpert::getDasaLordByIndex( const int &index )
{
	assert( index >= 0 && index < 8 );
	static const int dasa_lord[8] = { OMOON, OSUN, OJUPITER, OMARS, OMERCURY, OSATURN, OVENUS, OMEANNODE };
	return dasa_lord[index];
}

/*****************************************************
**
**   YoginiDasaExpert   ---   getDasaIndex4Nakshatra
**
******************************************************/
int YoginiDasaExpert::getDasaIndex4Nakshatra( const int &nak )
{
	return (int)a_red( nak + 3, 8 );
}

/*****************************************************
**
**   YoginiDasaExpert   ---   getDasaDuration
**
******************************************************/
int YoginiDasaExpert::getDasaDuration( const int &index )
{
	assert( index >= 0 && index < 8 );
	return index+1;
}

/*****************************************************
**
**   YoginiDasaExpert   ---   getExtraText
**
******************************************************/
const wxChar *YoginiDasaExpert::getExtraText( const int &i )
{
	const wxChar* k_yogini[8] = { _( "Mangala" ), _( "Pingala" ), _( "Dhanya" ), _( "Bhramari" ),
	                              _( "Bhadrika" ), _( "Ulka" ), _( "Siddha" ), _( "Sankata" )
	                            };
	assert( i >= 0 && i < 8 );
	return k_yogini[i];
}

/*****************************************************
**
**   YoginiDasaExpert   ---   getDasaLordName
**
******************************************************/
wxString YoginiDasaExpert::getDasaLordName( Writer*, const int &lord, const int format )
{
	wxString s;

	for( int i = 0; i < 8; i++ )
	{
		if ( getDasaLordByIndex( i ) == lord )
		{
			s = getExtraText( i );
			if ( format == TSHORT ) return s.Left( 2 );
			else if ( format == TMEDIUM ) return s.Left( 3 );
			else return s;
		}
	}
	printf( "Error YoginiDasaExpert::getDasaLordName lord %d\n", lord );
	return wxT( "Error" );
}

/*****************************************************
**
**   ShodshottariDasaExpert   ---   getDasaLordByIndex
**
******************************************************/
int ShodshottariDasaExpert::getDasaLordByIndex( const int &index )
{
	assert( index >= 0 && index < 8 );
	static const int dasa_lord[8] = { OSUN, OMARS, OJUPITER, OSATURN, OMEANDESCNODE, OMOON, OMERCURY, OVENUS };
	return dasa_lord[index];
}

/*****************************************************
**
**   ShodshottariDasaExpert   ---   getDasaIndex4Nakshatra
**
******************************************************/
int ShodshottariDasaExpert::getDasaIndex4Nakshatra( const int &nak )
{
	int diff = (int)a_red( nak - PUSHYAMI, 27 );
	return (int)a_red( diff, 8 );
}

/*****************************************************
**
**   ShodshottariDasaExpert   ---   getDasaDuration
**
******************************************************/
int ShodshottariDasaExpert::getDasaDuration( const int &index )
{
	assert( index >= 0 && index < 8 );
	static const int dasa_dur[8] = { 11, 12, 13, 14, 15, 16, 17, 18 };
	return dasa_dur[index];
}

/*****************************************************
**
**   DvadashottariDasaExpert   ---   getDasaLordByIndex
**
******************************************************/
int DvadashottariDasaExpert::getDasaLordByIndex( const int &index )
{
	assert( index >= 0 && index < 8 );
	static const int dasa_lord[8] = { OSUN, OJUPITER, OMEANDESCNODE, OMERCURY, OMEANNODE, OMARS, OSATURN, OMOON };
	return dasa_lord[index];
}

/*****************************************************
**
**   DvadashottariDasaExpert   ---   getDasaIndex4Nakshatra
**
******************************************************/
int DvadashottariDasaExpert::getDasaIndex4Nakshatra( const int &nak )
{
	int diff = (int)a_red( 26 - nak, 27 ); // 26 is Revati
	return (int)a_red( diff, 8 );
}

/*****************************************************
**
**   DvadashottariDasaExpert   ---   getDasaDuration
**
******************************************************/
int DvadashottariDasaExpert::getDasaDuration( const int &index )
{
	assert( index >= 0 && index < 8 );
	static const int dasa_dur[8] = { 7, 9, 11, 13, 15, 17, 19, 21 };
	return dasa_dur[index];
}

/*****************************************************
**
**   PanchottariDasaExpert   ---   getDasaLordByIndex
**
******************************************************/
int PanchottariDasaExpert::getDasaLordByIndex( const int &index )
{
	assert( index >= 0 && index < 7 );
	static const int dasa_lord[7] = { OSUN, OMERCURY, OSATURN, OMARS, OVENUS, OMOON, OJUPITER };
	return dasa_lord[index];
}

/*****************************************************
**
**   PanchottariDasaExpert   ---   getDasaIndex4Nakshatra
**
******************************************************/
int PanchottariDasaExpert::getDasaIndex4Nakshatra( const int &nak )
{
	int diff = (int)a_red( nak - ANURADHA, 27 );
	return (int)a_red( diff, 7 );
}

/*****************************************************
**
**   PanchottariDasaExpert   ---   getDasaDuration
**
******************************************************/
int PanchottariDasaExpert::getDasaDuration( const int &index )
{
	assert( index >= 0 && index < 7 );
	static const int dasa_dur[7] = { 12, 13, 14, 15, 16, 17, 18 };
	return dasa_dur[index];
}

/*****************************************************
**
**   ShatabdikaDasaExpert   ---   getDasaLordByIndex
**
******************************************************/
int ShatabdikaDasaExpert::getDasaLordByIndex( const int &index )
{
	assert( index >= 0 && index < 7 );
	static const int dasa_lord[7] = { OSUN, OMOON, OVENUS, OMERCURY, OJUPITER, OMARS, OSATURN };
	return dasa_lord[index];
}

/*****************************************************
**
**   ShatabdikaDasaExpert   ---   getDasaIndex4Nakshatra
**
******************************************************/
int ShatabdikaDasaExpert::getDasaIndex4Nakshatra( const int &nak )
{
	int diff = (int)a_red( nak - 26, 27 ); // 26 is Revati
	return (int)a_red( diff, 7 );
}

/*****************************************************
**
**   ShatabdikaDasaExpert   ---   getDasaDuration
**
******************************************************/
int ShatabdikaDasaExpert::getDasaDuration( const int &index )
{
	assert( index >= 0 && index < 7 );
	static const int dasa_dur[7] = { 5, 5, 10, 10, 20, 20, 30 };
	return dasa_dur[index];
}

/*****************************************************
**
**   ChaturashitiSamaDasaExpert   ---   getDasaLordByIndex
**
******************************************************/
int ChaturashitiSamaDasaExpert::getDasaLordByIndex( const int &index )
{
	assert( index >= 0 && index < 7 );
	static const int dasa_lord[7] = { OSUN, OMOON, OMARS, OMERCURY, OJUPITER, OVENUS, OSATURN };
	return dasa_lord[index];
}

/*****************************************************
**
**   ChaturashitiSamaDasaExpert   ---   getDasaIndex4Nakshatra
**
******************************************************/
int ChaturashitiSamaDasaExpert::getDasaIndex4Nakshatra( const int &nak )
{
	int diff = (int)a_red( nak - SWATI, 27 );
	return (int)a_red( diff, 7 );
}

/*****************************************************
**
**   ChaturashitiSamaDasaExpert   ---   getDasaDuration
**
******************************************************/
int ChaturashitiSamaDasaExpert::getDasaDuration( const int &index )
{
	return 12;
}

/*****************************************************
**
**   DvisaptatiSamaDasaExpert   ---   getDasaLordByIndex
**
******************************************************/
int DvisaptatiSamaDasaExpert::getDasaLordByIndex( const int &index )
{
	assert( index >= 0 && index < 8 );
	static const int dasa_lord[8] = { OSUN, OMOON, OMARS, OMERCURY, OJUPITER, OVENUS, OSATURN, OMEANNODE };
	return dasa_lord[index];
}

/*****************************************************
**
**   DvisaptatiSamaDasaExpert   ---   getDasaIndex4Nakshatra
**
******************************************************/
int DvisaptatiSamaDasaExpert::getDasaIndex4Nakshatra( const int &nak )
{
	int diff = (int)a_red( nak - 18, 27 );
	return (int)a_red( diff, 8 );
}

/*****************************************************
**
**   DvisaptatiSamaDasaExpert   ---   getDasaDuration
**
******************************************************/
int DvisaptatiSamaDasaExpert::getDasaDuration( const int &index )
{
	assert( index >= 0 && index < 8 );
	return 9;
}

/*****************************************************
**
**   ShatTrimshatSamaDasaExpert   ---   getDasaLordByIndex
**
******************************************************/
int ShatTrimshatSamaDasaExpert::getDasaLordByIndex( const int &index )
{
	assert( index >= 0 && index < 8 );
	static const int dasa_lord[8] = { OMOON, OSUN, OJUPITER, OMARS, OMERCURY, OSATURN, OVENUS, OMEANNODE };
	return dasa_lord[index];
}

/*****************************************************
**
**   ShatTrimshatSamaDasaExpert   ---   getDasaIndex4Nakshatra
**
******************************************************/
int ShatTrimshatSamaDasaExpert::getDasaIndex4Nakshatra( const int &nak )
{
	int diff = (int)a_red( nak - 21, 27 );
	return (int)a_red( diff, 8 );
}

/*****************************************************
**
**   ShatTrimshatSamaDasaExpert   ---   getDasaDuration
**
******************************************************/
int ShatTrimshatSamaDasaExpert::getDasaDuration( const int &index )
{
	assert( index >= 0 && index < 8 );
	return index + 1;
}


/*****************************************************
**
**   DasaExpertFactory   ---   create
**
******************************************************/
DasaExpert *DasaExpertFactory::createYoginiDasaExpert() {
	return new YoginiDasaExpert();
}
DasaExpert *DasaExpertFactory::createShodshottariDasaExpert() {
	return new ShodshottariDasaExpert();
}
DasaExpert *DasaExpertFactory::createDvadashottariDasaExpert() {
	return new DvadashottariDasaExpert();
}
DasaExpert *DasaExpertFactory::createPanchottariDasaExpert() {
	return new PanchottariDasaExpert();
}
DasaExpert *DasaExpertFactory::createShatabdikaDasaExpert() {
	return new ShatabdikaDasaExpert();
}
DasaExpert *DasaExpertFactory::createChaturashitiSamaDasaExpert() {
	return new ChaturashitiSamaDasaExpert();
}
DasaExpert *DasaExpertFactory::createDvisaptatiSamaDasaExpert() {
	return new DvisaptatiSamaDasaExpert();
}
DasaExpert *DasaExpertFactory::createShatTrimshatSamaDasaExpert() {
	return new ShatTrimshatSamaDasaExpert();
}


