/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/astrobase.cpp
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
#pragma implementation "astrobase.h"
#endif

#include "astrobase.h"

#include "Conf.h"
#include "mathbase.h"

#include <wx/string.h>
#include <math.h>

extern Config *config;

/**************************************************************
***
**
***
***************************************************************/
double getYearLength( const bool vedic )
{
	int yl = ( vedic ? config->vYearLength : config->wYearLength );
	double custom_yl = ( vedic ? config->vCustomYearLength : config->wCustomYearLength );

	switch ( yl )
	{
	case 0:
		return 365.25;
		break;
	case 1:
		return 365.2422;
		break;
	case 2:
		return 365;
		break;
	case 3:
		return 360;
		break;
	case 4:
		return custom_yl;
		break;
	default:
		printf( "Warning: Wrong year length index %d taking default 365.25", yl );
		return 365.25;
		break;
	}
}

/*****************************************************
**
**   calcYearLength
**
******************************************************/
double calcYearLength( const int sel, const bool vedic )
{
	double custom_yl = ( vedic ? config->vCustomYearLength : config->wCustomYearLength );

	switch ( sel )
	{
	case 0:
		return 365.25;
		break;
	case 1:
		return 365.2422;
		break;
	case 2:
		return 365;
		break;
	case 3:
		return 360;
		break;
	case 4:
		return custom_yl;
		break;
	default:
		printf( "Warning: Wrong year length index %d taking default 365.25", sel );
		return 365.25;
		break;
	}
}

/**************************************************************
***
**
***
***************************************************************/
double getNavamsaPortion( const double &len, const int &pindex, const bool &retro )
{
	double navamsavastha = 0;
	double nav_portion;
	int start_nav;

	assert(( pindex <= OSATURN ) && ( pindex >= 0 ));

	if ( ! retro )
	{
		nav_portion = a_red( len, 30 ) / 3.3333333333;
		start_nav = red12( getRasi( len ) * 9 );
	}
	else
	{
		nav_portion = ( 30 - a_red( len, 30 )) / 3.3333333333;
		start_nav = red12( getRasi( len ) * 9 + 8 );
	}

	int inav_portion = (int)nav_portion;
	int nav_rasi;
	double portion;
	navamsavastha = 0;

	for ( int i = 0; i <= inav_portion; i++ )
	{
		if ( i < inav_portion ) portion = 1;
		else portion = nav_portion - (int)nav_portion;

		if ( ! retro )
		{
			nav_rasi = red12( start_nav + i );
		}
		else
		{
			nav_rasi = red12( start_nav - i );
		}

		if ( hasExaltationRasi( pindex, nav_rasi ))
		{
			navamsavastha += 2 * portion;
		}
		else if ( getLord( nav_rasi ) == pindex )
		{
			navamsavastha += 2 * portion;
		}
		else if ( hasDebilationRasi( pindex, nav_rasi ))
		{
			navamsavastha -= portion;
		}
		else if ( isFriend( pindex, getLord( nav_rasi )) == ENEMY )
		{
			navamsavastha -= portion;
		}
		else
		{
			navamsavastha += portion;
		}
	}
	if ( navamsavastha  < 0 ) navamsavastha = 0;
	return navamsavastha;
}

/**************************************************************
***
**
***
***************************************************************/
void getDasaDurationInts( int &year, int &month, int &day, double length )
{
	double yeard, monthd, dayd;

	yeard = length/getYearLength( true );
	year = (int)yeard;
	monthd = ( yeard - (double)year ) * 12;
	month = (int)monthd;
	dayd = ( monthd - (double)month ) * 30;
	dayd += .00001;
	day = (int)dayd;
}

/**************************************************************
***
**
***
***************************************************************/
void getDasaDuration( wxString &buf, double length )
{
// YEAR_LEN is 365.25;
#define MY_NULL .00000001
	int years, months, days, hours, minutes, seconds;
	double yeard, monthd, dayd, hourd, minuted, secondd;

	buf = wxT( " --- " );
	wxChar yd, md = 'M', dd;
	yd = ( config->lang == wxT( "de" ) ? 'J' : 'Y' );
	dd = ( config->lang == wxT( "de" ) ? 'T' : 'D' );

	yeard = length/getYearLength( true );
	years = (int)yeard;
	monthd = ( yeard - (double)years ) * 12;
	if ( ::fabs( monthd ) < MY_NULL )
	{
		buf.Printf( wxT( "%d%c" ), years, yd );
		//if ( years > 1 ) strcat( buf, "s" );
		return;
	}
	months = (int)monthd;
	dayd = ( monthd - (double)months ) * 30;
	dayd += .00001;
	if ( ::fabs( dayd ) < MY_NULL )
	{
		buf.Printf( wxT( "%d%c %d%c" ), years, yd, months, md );
		return;
	}
	days = (int)dayd;
	hourd = ( dayd - (double)days ) * 24;
	if ( years > 0 || months > 0 )
	{
		buf.Printf( wxT( "%d%c %d%c %d%c" ), years, yd, months, md, days, dd );
		return;
	}
	hours = (int)hourd;
	minuted = ( hourd - (double)hours ) * 60;
	minutes = (int)minuted;
	secondd = ( minuted - (double)minutes ) * 60;
	seconds = (int)secondd;
	buf.Printf( wxT( "%d%c %02d:%02d:%02d" ), days, dd, hours, minutes, seconds );
}

/**************************************************************
***
**
***
***************************************************************/
double getOpposition( const double &v )
{
	return red_deg( v + 180.0 );
}

/**************************************************************
***
**
***
***************************************************************/
int getRasi( const double &len )
{
	return( (int)( red_deg( len ) / 30 )  );
}

#define USHADHA_BEGIN 266.6666666667
#define ABHIJIT_BEGIN 276.6666666667
#define SRAVANA_BEGIN 280.8888888889
#define DHANISHTA_BEGIN 293.6666666667

#define USHADHA_LEN 10
#define ABHIJIT_LEN 4.2222222222 // = 4:13:20;
#define SRAVANA_LEN 12.444444444 // = 12:26:40
/**************************************************************
***
**
***
***************************************************************/
double getCorrectedNakshatraLength( const double &len )
{
	// incl. P. Shadha
	if ( len <= USHADHA_BEGIN ) return len;

	// U. Shadha
	else if (( len > USHADHA_BEGIN ) && ( len <= ABHIJIT_BEGIN  ))
		return USHADHA * NAKSHATRA_LEN + NAKSHATRA_LEN / USHADHA_LEN * ( len - USHADHA_BEGIN );

	// Abhijit: upto 280:53:20
	else if (( len > ABHIJIT_BEGIN ) && ( len <= SRAVANA_BEGIN  ))
		return ABHIJIT * NAKSHATRA_LEN + NAKSHATRA_LEN / ABHIJIT_LEN * ( len - ABHIJIT_BEGIN );

	// Sravana
	else if (( len > SRAVANA_BEGIN ) && ( len <= DHANISHTA_BEGIN  ))
		return SRAVANA * NAKSHATRA_LEN + 1.071428571 * ( len - SRAVANA_BEGIN );

	// rest add one nakshatra
	else return len + NAKSHATRA_LEN;
}
/**************************************************************
***
**
***
***************************************************************/
int getNakshatra( const double &len, const int nsys )
{
	if ( nsys == N27 ) return( (int)a_red( red_deg( len ) / NAKSHATRA_LEN, 27 ));
	else return( (int)( getCorrectedNakshatraLength( len ) / NAKSHATRA_LEN ));
}

/**************************************************************
***
**
***
***************************************************************/
double getNakshatraLength( const double &len, const int nsys )
{
	if ( nsys == N27 ) return a_red( len, NAKSHATRA_LEN );
	else return a_red( getCorrectedNakshatraLength( len ), NAKSHATRA_LEN );
}

/**************************************************************
***
**
***
***************************************************************/
int getPada( const double &len )
{
	return( (int)a_red( red_deg( len ) * 4 / NAKSHATRA_LEN, 108 ));
}

/**************************************************************
***
**
***
***************************************************************/
int isOddFootedRasi( const int &rasi )
{
	assert(( rasi >= 0 ) && ( rasi < 12 ));

	if (( rasi == ARIES )
	        || ( rasi == TAURUS )
	        || ( rasi == GEMINI )
	        || ( rasi == LIBRA )
	        || ( rasi == SCORPIO )
	        || ( rasi == SAGITTARIUS )
	   )
		return true;

	return false;
}

/**************************************************************
***
**   getVarna
***
***************************************************************/
int getVarna( const double &len )
{
	assert( len >= 0 && len <= 360 );
	switch ( getRasi( len ) % 4 )
	{
	case 0:
		return 2; // Kshattriya
		break;
	case 1:
		return 1; // Vaishya
		break;
	case 2:
		return 0; // Shudra
		break;
	case 3:
		return 3; // Brahmin
		break;
	default:
		assert( 0 );
		return -1;
		break;
	}
	return -1;
}

/**************************************************************
***
**
***
***************************************************************/
bool hasJaiminiAspect( const int &pos1, const int &pos2 )
{
	int diff = red12( pos2 - pos1 );

	// CHARA
	if ( ! ( pos1 % 3 ))
	{
		if (( diff == 4 ) || ( diff == 7 ) || ( diff == 10 )) return true;
	}
	// STHIRA
	else if ( ( pos1 % 3 ) == 1 )
	{
		if (( diff == 2 ) || ( diff == 5 ) || ( diff == 8 )) return true;
	}
	// DWISWA
	else
	{
		if (( diff == 3 ) || ( diff == 3 ) || ( diff == 9 )) return true;
	}

	return false;
}


/**************************************************************
***
**
***
***************************************************************/
int isOddRasi( const double &len )
{
	return( getRasi( len + 30 ) % 2 );
}

/**************************************************************
***
**
***
***************************************************************/
int isEvenRasi( const double &len )
{
	return( getRasi( len ) % 2 );
}

/**************************************************************
***
** isSampadaRasi
***
***************************************************************/
bool isSampadaRasi( const int &rasi )
{
	if ( rasi == CANCER ) return true;
	if ( rasi == LEO ) return true;
	if ( rasi == VIRGO ) return true;
	if ( rasi == CAPRICORN ) return true;
	if ( rasi == AQUARIUS ) return true;
	if ( rasi == PISCES ) return true;
	return false;
}

/**************************************************************
***
** isVishamapadaRasi
***
***************************************************************/
bool isVishamapadaRasi( const int &rasi )
{
	if ( rasi == ARIES ) return true;
	if ( rasi == TAURUS ) return true;
	if ( rasi == GEMINI ) return true;
	if ( rasi == LIBRA ) return true;
	if ( rasi == SCORPIO ) return true;
	if ( rasi == SAGITTARIUS ) return true;
	return false;
}

/**************************************************************
***
**
***
***************************************************************/
double getRasiLen( const double &len, const bool &reverse )
{
	if ( ! reverse )	return( a_red( len, 30 ));
	else return( 30 - a_red( len, 30 ));
}

/**************************************************************
***
**
***
***************************************************************/
int between( const double &len, const double &min, const double &max )
{
	return( ( len >= min ) && ( len <= max )  );
}

/**************************************************************
***
**
***
***************************************************************/
double planetDistance( const double &len1, const double &len2 )
{
	double d = red_deg( fabs( len2 - len1 ));
	if ( d > 180 ) return( 360 - d );
	return d;
}

/**************************************************************
***
**
***
***************************************************************/
int inAscendant( const double &len, const double &asc )
{
	return( a_red( getRasi( len ) - getRasi( asc ), 12 ) == 0 );

	return 0;
}

/**************************************************************
***
**
***
***************************************************************/
int inTrikona( const double &len, const double &asc )
{
	int b = (int)a_red( getRasi( len ) - getRasi( asc ), 12 );

	if (( b == HOUSE5 ) || ( b == HOUSE9 )) return 1;
	else return 0;
}

/**************************************************************
***
**
***
***************************************************************/
int inKendra( const double &len, const double &asc )
{

	int b = (int)a_red( getRasi( len ) - getRasi( asc ), 12 );

	if (( b == HOUSE4 ) || ( b == HOUSE7 ) || ( b == HOUSE10 )) return 1;
	else return 0;
}

/**************************************************************
***
**
***
***************************************************************/
int inMovableSign( const double &len )
{
	return( getRasi( len ) % 3 ) == 0;
}

/**************************************************************
***
**
***
***************************************************************/
int inFixedSign( const double &len )
{
	return( getRasi( len ) % 3 ) == 1;
}

/**************************************************************
***
**
***
***************************************************************/
int inDualSign( const double &len )
{
	return( getRasi( len ) % 3 ) == 2;
}

/**************************************************************
***
** getAvasthaName
***
***************************************************************/
const wxChar *getAvasthaName( const int &i , const int &avastha )
{
	const wxChar *sayanadi[12] = { wxT( "Sayana" ), wxT( "Upavesana" ), wxT( "Netrapani" ),
	                               wxT( "Prakasana" ), wxT( "Gamana" ), wxT( "Aagamana" ), wxT( "Sabha" ), wxT( "Agama" ),
	                               wxT( "Bhojama" ), wxT( "Nrityalipsa" ), wxT( "Kautuka" ), wxT( "Nidra" )
	                             };

	if ( i > 1 ) return sayanadi[i - 1];
	return sayanadi[11];
}

/**************************************************************
***
**
***
***************************************************************/
int getLord( const int &sign, const bool vedic )
{
	assert(( sign >= ARIES ) && ( sign <= PISCES ));
	const int k_vlord[12] = { OMARS, OVENUS, OMERCURY, OMOON, OSUN, OMERCURY,
	                          OVENUS, OMARS, OJUPITER, OSATURN, OSATURN, OJUPITER
	                        };
	const int k_wlord[12] = { OMARS, OVENUS, OMERCURY, OMOON, OSUN, OMERCURY,
	                          OPLUTO, OMARS, OJUPITER, OSATURN, OURANUS, ONEPTUNE
	                        };
	return vedic ? k_vlord[sign] : k_wlord[sign];
}

/**************************************************************
***
**
***
***************************************************************/
int isFriend( const int &planet1, const int &planet2 )
{
	assert(( planet1 >= OSUN ) && ( planet1 <= OSATURN ));
	assert(( planet2 >= OSUN ) && ( planet2 <= OSATURN ));

	/********************************************************
	       Friends         Enemies        Neutrals
	---------------------------------------------------------
	 Sun   Moo, Mar, Jup   Ve, Sat        Mer
	 Moo   Sun, Mer        -              Mar, Jup, Ven, Sat
	 Mar   Sun, Moo, Jup   Mer            Ven, Sat
	 Mer   Sun, Ven        Moo            Mar, Jup, Sat
	 Jup   Sun, Moo, Mar   Mer, Ven       Sat
	 Ven   Mer, Sat        Moo, Sun       Mar, Jup
	 Sat   Mer, Ven        Sun, Moo, Mar  Jup
	*********************************************************/

	const int k_friend[7][7] = {
		{ FRIEND, FRIEND, NEUTRAL, ENEMY, FRIEND, FRIEND, ENEMY },      // Sun
		{ FRIEND, FRIEND, FRIEND, NEUTRAL, NEUTRAL, NEUTRAL, NEUTRAL }, // Moon
		{ FRIEND, ENEMY, FRIEND, FRIEND, NEUTRAL, NEUTRAL, NEUTRAL },   // Mercury
		{ ENEMY, ENEMY, FRIEND, FRIEND, NEUTRAL, NEUTRAL, FRIEND },     // Venus
		{ FRIEND, FRIEND, ENEMY, NEUTRAL, FRIEND, FRIEND, NEUTRAL },    // Mars
		{ FRIEND, FRIEND, ENEMY, ENEMY, FRIEND, FRIEND, NEUTRAL },      // Jupiter
		{ ENEMY, ENEMY, FRIEND, FRIEND, ENEMY, NEUTRAL, FRIEND }        // Saturn
	};
	return k_friend[planet1][planet2];
}

/**************************************************************
***
**
***
***************************************************************/
int isRasiPosTempFriend( const int &rasi1, const int &rasi2 )
{
	assert( rasi1 >= ARIES && rasi1 <= PISCES );
	assert( rasi2 >= ARIES && rasi2 <= PISCES );
	int diff = red12( rasi2 - rasi1 );
	if ( diff == 1 || diff == 2 || diff == 3 || diff == 11 || diff == 10 || diff == 9 ) return FRIEND;
	else return ENEMY;
}

/**************************************************************
***
**
***
***************************************************************/
bool isInMulatrikona( const int &planet, const double &len )
{
	if ( planet == OSUN )
	{
		if (( len >= 120 ) && ( len <= 140 )) return true;
		else return false;
	}
	else if ( planet == OMOON )
	{
		if (( len >= 34 ) && ( len <= 60 )) return true;
		else return false;
	}
	else if ( planet == OMARS )
	{
		if (( len >= 0 ) && ( len <= 12 )) return true;
		else return false;
	}
	else if ( planet == OMERCURY )
	{
		if (( len >= 165 ) && ( len <= 170 )) return true;
		else return false;
	}
	else if ( planet == OJUPITER )
	{
		if (( len >= 240 ) && ( len <= 250 )) return true;
		else return false;
	}
	else if ( planet == OVENUS )
	{
		if (( len >= 180 ) && ( len <= 195 )) return true;
		else return false;
	}
	else if ( planet == OSATURN )
	{
		if (( len >= 300 ) && ( len <= 320 )) return true;
		else return false;
	}
	else
	{
		printf( "ERROR isInMulatrikona planet " );
		exit(1);
	}
	return false;
}

/**************************************************************
***
**
***
***************************************************************/
int getMulatrikonaRasi( const int &planet )
{
	int p = planet;
	if ( planet == OMEANNODE || planet == OTRUENODE ) p = 7;
	if ( planet == OMEANDESCNODE || planet == OTRUEDESCNODE ) p = 8;
	assert( p >= OSUN && p < 9 );

	// su=LEO, mo=TAURUS, ma=ARIES, me=VIRGO, Ju=SAGITTARIUS, ve=LIBRA, sa=AQUARIUS, ra=AQUARIUS, ke=SCORPIO
	const int k_mulatrikona_sign[9] = { LEO, TAURUS, VIRGO, LIBRA, ARIES, SAGITTARIUS, AQUARIUS, AQUARIUS, SCORPIO };
	return k_mulatrikona_sign[planet];
}

/**************************************************************
***
**
***
***************************************************************/
int getVashya( const double &len )
{
	int	vashya = VASHYA_NONE;
	int rasi = ::getRasi( len );
	double rasilen = ::getRasiLen( len );
	//enum { VASHYA_QUADRUPED, VASHYA_HUMAN, VASHYA_JALACHARA, VASHYA_LEO, VASHYA_SCORPIO };
	switch ( rasi )
	{
	case ARIES:
	case TAURUS:
		vashya = VASHYA_QUADRUPED;
		break;
	case GEMINI:
	case VIRGO:
	case LIBRA:
	case AQUARIUS:
		vashya = VASHYA_HUMAN;
		break;
	case CANCER:
	case PISCES:
		vashya = VASHYA_JALACHARA;
		break;
	case LEO:
		vashya = VASHYA_LEO;
		break;
	case SCORPIO:
		vashya = VASHYA_SCORPIO;
		break;
	case SAGITTARIUS:
		vashya = rasilen < 15 ? VASHYA_HUMAN : VASHYA_QUADRUPED;
		break;
	case CAPRICORN:
		vashya = rasilen < 15 ? VASHYA_QUADRUPED : VASHYA_JALACHARA;
		break;
	default:
		assert( 0 );
		break;
	}
	assert( vashya != VASHYA_NONE );
	return vashya;
}

/**************************************************************
***
**
***
***************************************************************/
bool hasMulatrikonaRasi( const int &planet, const int &rasi )
{
	assert(( rasi >= ARIES ) && ( rasi <= PISCES ));
	return( getMulatrikonaRasi( planet ) == rasi );
}

/**************************************************************
***
**
***
***************************************************************/
int getExaltationRasi( const int &planet )
{
	const static int exalt_rasi[9] = { ARIES, TAURUS, VIRGO, PISCES, CAPRICORN, CANCER, LIBRA, GEMINI, SAGITTARIUS };
	int ret = 0;
	assert(  (( planet >= OSUN ) && ( planet <= OSATURN ))
	         || ( planet == OMEANNODE ) || ( planet == OTRUENODE )
	         || ( planet == OMEANDESCNODE ) || ( planet == OTRUEDESCNODE ));

	switch ( planet )
	{
	case OMEANNODE:
	case OTRUENODE:
		ret = 7;
		break;
	case OMEANDESCNODE:
	case OTRUEDESCNODE:
		ret = 8;
		break;
	default:
		ret = planet;
		break;
	}
	return exalt_rasi[ret];
}

/**************************************************************
***
**
***
***************************************************************/
bool hasExaltationRasi( const int &planet, const int &rasi )
{
	return ( getExaltationRasi( planet ) == rasi );
}

/**************************************************************
***
**
***
***************************************************************/
bool hasDebilationRasi( const int &planet, const int &rasi )
{
	int erasi = rasi + 6;
	if ( erasi > 11 ) erasi -= 12;
	return ( getExaltationRasi( planet ) == erasi );
}

/**************************************************************
***
**  Sphuta drishti
***
***************************************************************/
double getAspectValue( const int &aspectingPlanet, const int &aspectedPlanet, const double &len )
{
	assert( len >= 0 && len <= 360 );
	double ret = 0;
	double rasilen = a_red( len, 30 );
	//double antirasilen = 30 - rasilen;

	double rp = 2 * rasilen;
	double ap = 2 * ( 30 - rasilen );

	double fmars = .75;
	if ( aspectingPlanet == OMARS ) fmars = 1;

	double fjupiter = .5;
	if ( aspectingPlanet == OJUPITER ) fjupiter = 1;

	double fsaturn = .25;
	if ( aspectingPlanet == OSATURN ) fsaturn = 1;

	// Default 180
	if ( len >= 150 && len < 180 ) {
		ret += rp;
	}
	else if ( len >= 180 && len < 210 ) {
		ret += ap;
	}

	// Mars 90 + 210
	if (( len >= 60 && len < 90 ) || ( len >= 180 && len < 210 )) {
		ret += fmars * rp;
	}
	else if (( len >= 90 && len < 120 ) || ( len >= 210 && len < 240 )) {
		ret += fmars * ap;
	}

	// jupiter 120 + 240
	if (( len >= 90 && len < 120 ) || ( len >= 210 && len < 240 )) {
		ret += fjupiter * rp;
	}
	else if (( len >= 120 && len < 150 ) || ( len >= 240 && len < 270 )) {
		ret += fjupiter * ap;
	}

	// Saturn 60 + 270
	if (( len >= 30 && len < 60 ) || ( len >= 240 && len < 270 )) {
		ret += fsaturn * rp;
	}
	else if (( len >= 60 && len < 90 ) || ( len >= 270 && len < 300 )) {
		ret += fsaturn * ap;
	}

	assert( ret >= 0 && ret <= 60 );
	return ret;
}

/**************************************************************
***
**
***
***************************************************************/
double getAspectValue2( const int &planet1, const double &len1, const int &planet2, const double &len2 )
{
	assert( ( planet1 >= OSUN ) && ( planet1 <= OSATURN ));
	assert( ( planet2 >= OSUN ) && ( planet2 <= OSATURN ));

	double ret = 0;
	double asplen = red_deg( len2 - len1 );
	//printf( " -->> p1 %d len %f ####  p2 %d len %f ####  Asplen: %f\n", planet1, len1, planet2, len2, asplen );

	if (( asplen >= 30 ) && ( asplen <= 60 ))
	{
		ret = ( asplen - 30 ) * .5;
		/* if ( planet1 == OSATURN ) { ret = ( asplen - 30 ) * 2; } */
	}
	else if (( asplen > 60 ) && ( asplen <= 90 ))
	{
		ret = asplen - 45;
		if ( planet1 == OSATURN )
		{
			//ret = 45 + ( 90 - asplen ) * .5;
			ret += 45;
		}
	}
	else if (( asplen > 90 ) && ( asplen <= 120 ))
	{
		//ret = ( 120 - asplen ) * .5 + 30;
		ret = 90 - asplen * .5; // the same
		if ( planet1 == OMARS )
		{
			//ret = 45 + ( asplen - 90 ) * .5;
			ret += 15;
		}
		/* if ( planet1 == OJUPITER ) { ret = 45 + ( asplen - 90 ) * .5; } */
	}
	else if (( asplen > 120 ) && ( asplen <= 150 ))
	{
		ret = 150 - asplen;
		/* if ( planet1 == OMARS ) { ret = ( 150 - asplen ) * 2; } */
		if ( planet1 == OJUPITER )
		{
			//ret = ( 150 - asplen ) * 2;
			ret += 30;
		}
	}
	else if (( asplen > 150 ) && ( asplen <= 180 ))
	{
		ret = ( asplen - 150 ) * 2;
	}
	else if (( asplen > 180 ) && ( asplen <= 300 ))
	{
		ret = ( 300 - asplen ) * .5;
		if ( planet1 == OMARS )
		{
			/* if (( asplen >= 180 ) && ( asplen <=210 )) { ret = 60; } */
			if (( asplen >= 210 ) && ( asplen <=240 ))
			{
				//ret = 270 - asplen;
				ret += 15;
			}
		}
		if ( planet1 == OJUPITER )
		{
			/* if (( asplen >= 210 ) && ( asplen <=240 )) { ret = 45 + ( asplen - 210 ) * .5; } else*/
			if (( asplen >= 240 ) && ( asplen <=270 ))
			{
				//ret = 15 + ( 270 - asplen ) * 1.5;
				ret += 30;
			}
		}
		if ( planet1 == OSATURN )
		{
			/* if (( asplen >= 240 ) && ( asplen <=270 )) { //ret = 30 + asplen - 240; ret = asplen - 210; } else */
			if (( asplen >= 270 ) && ( asplen <=300 ))
			{
				//ret = ( 300 - asplen ) * 2;
				ret += 45;
			}
		}
	}
	//assert(( ret >= 0 ) && ( ret <= 60 ));
	return ret;
}

/**************************************************************
***
**   Calculate geometrical midpoint for composite horoscope
***
***************************************************************/
double calcGeometricalMidpoint( const double &len1, const double &len2 )
{
	double dist = red_deg( len2 - len1 );
	return( dist < 180 ? red_deg( len1 + dist/2 ) : red_deg( len2 + ( 360 - dist ) /2 ) );
}

/**************************************************************
***
**   return number of degrees for a given circle type
***
***************************************************************/
double getDegrees4CircleType( const int &circle )
{
	assert( circle >= CIRCLE_TYPE_360 && circle <= CIRCLE_TYPE_5625 );
	switch ( circle )
	{
	case CIRCLE_TYPE_360:
		return 360;
		break;
	case CIRCLE_TYPE_180:
		return 180;
		break;
	case CIRCLE_TYPE_90:
		return 90;
		break;
	case CIRCLE_TYPE_45:
		return 45;
		break;
	case CIRCLE_TYPE_225:
		return 22.5;
		break;
	case CIRCLE_TYPE_1125:
		return 11.25;
		break;
	case CIRCLE_TYPE_5625:
		return 5.625;
		break;
	default:
		assert( 0 );
		break;
	}
	return 0;
}

/**************************************************************************
***
**  Monatslaenge berechnen
***
**************************************************************************/
int calcMonthLength( int monat, int jahr )
{
	if ( monat == 1 || monat == 3 || monat == 5 || monat == 7 || monat == 8 || monat == 10 || monat == 12 ) return 31;
	if ( monat == 4 || monat == 6 || monat == 9 || monat == 11 ) return 30;
	assert( monat == 2 );
	if ( jahr % 4 )  return 28;
	else
	{
		if (( jahr % 100 ) != 0 ) return 29;
		if (( jahr % 100 ) == 0 && ( jahr % 400 ) != 0 ) return 28;
		return 29;
	}
}

/*****************************************************
**
**   getDegMinSecInts2
**
******************************************************/
void getDegMinSecInts2( const double &len, int &deg, int &min, double &sec )
{
	double mylen = fabs( len );
	deg = (int)mylen;
	double mind = (double)((mylen - (double)deg ) * 60);
	min = (int)mind;
	sec = ( mind - (double)min ) * 60;
	if ( len < 0 ) deg *= -1;
}

/*****************************************************
**
**   getDegMinSecInts
**
******************************************************/
void getDegMinSecInts( const double &len, int &deg, int &min, int &sec )
{
#define ROUND_DEGREES .005 / 3600
	double mylen = fabs( len ) + ROUND_DEGREES;
	deg = (int)mylen;
	double mind = (double)((mylen - (double)deg ) * 60);
	min = (int)mind;
	sec = (int)(( mind - (double)min ) * 60 );
	if ( len < 0 ) deg *= -1;
}

/*****************************************************
**
**   getGrahaDrishti
**
******************************************************/
bool getGrahaDrishti( const int &aspectingPlanet, const int &rasidiff )
{
	if ( rasidiff == 6 ) return true;
	else if ( aspectingPlanet == OMARS && ( rasidiff == 3 || rasidiff == 7 )) return true;
	else if ( aspectingPlanet == OJUPITER && ( rasidiff == 4 || rasidiff == 8 )) return true;
	else if ( aspectingPlanet == OSATURN && ( rasidiff == 2 || rasidiff == 9 )) return true;
	else return false;
}

/*****************************************************
**
**   getGrahaDrishtiValue
**
******************************************************/
double getGrahaDrishtiValue( const int &aspectingPlanet, const int &rasidiff )
{
	if ( rasidiff == 6 ) return 1;
	else if ( rasidiff == 3 || rasidiff == 7 )
	{
		return ( aspectingPlanet == OMARS ? 1 : .75 );
	}
	else if ( rasidiff == 4 || rasidiff == 8 )
	{
		return ( aspectingPlanet == OJUPITER ? 1 : .5 );
	}
	else if ( rasidiff == 2 || rasidiff == 9 )
	{
		return ( aspectingPlanet == OSATURN ? 1 : .25 );
	}
	return 0;
}


