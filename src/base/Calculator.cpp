/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/base/Calculator.cpp
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
#pragma implementation "Calculator.h"
#endif

#include "Calculator.h"

#include<wx/log.h>
#include <wx/string.h>

#ifndef TRUE
#define TRUE 1
#endif
#define TRUE 1

#ifdef __WXMSW__
#undef TRUE
#endif
#ifndef _cplusplus
extern "C" {
#endif
#include "swephexp.h"
#include "swepcalc.h"
#ifndef _cplusplus
}
#endif

#include "Conf.h"
#include "Lang.h"
#include "mathbase.h"

extern Config *config;

/*****************************************************
**
**   EclipseData   ---   Constructor
**
******************************************************/
EclipseData::EclipseData()
{
	jd = sunpos = moonpos = 0;
	planet = type = 0;
	description = wxT( "" );
}

Calculator *CalculatorFactory::calculator = 0;
/*****************************************************
**
**   CalculatorFactory   ---   getCalculator
**
******************************************************/
Calculator *CalculatorFactory::getCalculator()
{
	if ( calculator == 0 )
	{
		calculator = new CalculatorSwe();
		calculator->init();
	}
	return calculator;
}

/*****************************************************
**
**   CalculatorSwe   ---   Constructor
**
******************************************************/
CalculatorSwe::CalculatorSwe()
{
	pedantic = false;
	//pedantic = true;
	init();
}

/*****************************************************
**
**   CalculatorSwe   ---   Destructor
**
******************************************************/
CalculatorSwe::~CalculatorSwe()
{
	swe_close();
}

/*****************************************************
**
**   CalculatorSwe   ---   init
**
******************************************************/
void CalculatorSwe::init()
{
	if ( ! config->sePath.IsEmpty() )
	{
		swe_set_ephe_path( (char*)str2char( config->sePath ));
		if ( pedantic )	printf( "Ephem path: %s\n", str2char( config->sePath ));
	}
}

/*************************************************//**
*
* \brief constants for built-in Ayanamsas
*
******************************************************/
struct AyaInit {
	double t0, ayan_t0;
};
static const struct AyaInit Ayanamsa[NB_AYANAMSAS] = {
	{2415020.0, 360 - 337.53953},   /* 1: Lahiri (Robert Hand) */
	{2415020.0, 360 - 338.98556},   /* 3: Raman (Robert Hand) */
	{2415020.0, 360 - 337.636111}  /* 5: Krishnamurti (Robert Hand) */
};

/*****************************************************
**
**   CalculatorSwe   ---   calcAyanamsa
**
******************************************************/
double CalculatorSwe::calcAyanamsa( const double &jd, const int &type )
{
	double t, t0, ayan_t0;
	assert( type >= 0 && type <= NB_AYANAMSAS+1 );
	if ( type == 0 ) return 0;

	if ( type <= NB_AYANAMSAS && type > 0 )
	{
		t0 = Ayanamsa[type-1].t0;
		ayan_t0 = Ayanamsa[type-1].ayan_t0;
	}
	else
	{
		t0 = config->custom_t0;
		ayan_t0 = config->custom_ayan_t0;
	}
	if ( config->custom_aya_constant )
	{
		t = jd - t0;
		double years = t/365.25;
		double portion = years/config->custom_aya_period;
		double aya = portion * 360;
		//return red_deg( config->custom_ayan_t0 + 360 * ( jd - config->custom_t0 ) / ( config->custom_aya_period * 365.25 ));

		// bugfix 6.0: forgot ayan_t0
		//return red_deg( aya );
		return red_deg( ayan_t0 + aya );
	}
	else
	{
		swe_set_sid_mode( 255, t0, ayan_t0 );
		return swe_get_ayanamsa( calcJd( jd ));
	}
}

/*****************************************************
**
**   CalculatorSwe   ---   calcSiderealTime
**
******************************************************/
double CalculatorSwe::calcSiderealTime( const double &jd , const double &longitude )
{
	return swe_sidtime( jd + longitude / 360 );
}

/*****************************************************
**
**   CalculatorSwe   ---   calcEps
**
******************************************************/
void CalculatorSwe::calcEps( double &eps, const double &jd )
{
	char err[256];
	double x[6];

	// x[0] enthaelt eps
	if ( swe_calc( calcJd( jd ), SE_ECL_NUT, 0, x, err) == -1 )
	{
		assert( false );
	}
	PRINT_PEDANTIC
	eps = x[0];
}

/*****************************************************
**
**   CalculatorSwe   ---   calcNextSolarEvent
**
******************************************************/
double CalculatorSwe::calcNextSolarEvent( const int &type, const double &jd, const double &lat, const double &lon )
{
	char err[256];
	double rsmi[3], tret[3];

	int flag = 0;
	if ( ! config->sunrise_def ) flag = SE_BIT_DISC_CENTER;
	if ( ! config->sunrise_refrac ) flag |= SE_BIT_NO_REFRACTION;

	//int event_flag = 0;
	switch ( type )
	{
	case SOLAR_EVENT_SUNRISE:
		flag |= SE_CALC_RISE;
		break;
	case SOLAR_EVENT_SUNSET:
		flag |= SE_CALC_SET;
		break;
	case SOLAR_EVENT_NOON:
		flag |= SE_CALC_ITRANSIT;
		break;
	case SOLAR_EVENT_MIDNIGHT:
		flag |= SE_CALC_MTRANSIT;
		break;
	default:
		assert( false );
		break;
	}

	rsmi[0] = lon;
	rsmi[1] = lat;
	rsmi[2] = 0;

	swe_rise_trans( calcJd( jd ), SE_SUN, 0, 0, flag, rsmi, 0, 0, tret, err );
	PRINT_PEDANTIC
	return tret[0];
}

/*****************************************************
**
**   CalculatorSwe   ---   calcSunRiseSunSet
**
******************************************************/
bool CalculatorSwe::calcSunRiseSunSet( DataSet *d, double *sunrise, double *sunset, double *noon, double *midnight )
{
	char err[256];
	bool daybirth = true;
	double r[6];
	double cusp[13], ascmc[10];
	double rsmi[3], tret[3];
	double startjdrise, startjdset, startjdnoon, startjdmidnight;
	int flag = 0;

	if ( ! config->sunrise_def ) flag = SE_BIT_DISC_CENTER;
	if ( ! config->sunrise_refrac ) flag |= SE_BIT_NO_REFRACTION;
	double jd = calcJd( d->getJD() );

	swe_calc( jd, OSUN, 0, r,err );
	PRINT_PEDANTIC
	swe_houses( jd, d->getLocation()->getLatitude(), d->getLocation()->getLongitude(), 'E', cusp, ascmc );

	double diff_ascsun = red_deg( ascmc[0] -  r[0] );  // Sun and AC
	if ( diff_ascsun > 180 ) daybirth = false;

	double diff_icsun = red_deg( ascmc[1] + 180 - r[0] );  // Sun and IC

	startjdrise = startjdset = startjdnoon = startjdmidnight = jd;

	if ( daybirth )
	{
		if ( diff_icsun < 180 )
		{
			// forenoon
			startjdrise--;
		}
		else
		{
			// afternoon
			startjdrise--;
			startjdnoon--;
		}
	}
	else
	{
		if ( diff_icsun < 180 )
		{
			// morning before sunrise
			startjdrise--;
			startjdset--;
			startjdnoon--;
			startjdmidnight--;
		}
		else
		{
			// evening after sunset
			startjdrise--;
			startjdset--;
			startjdnoon--;
		}
	}

	rsmi[0] = d->getLocation()->getLongitude();
	rsmi[1] = d->getLocation()->getLatitude();
	rsmi[2] = 0;

	if ( sunrise )
	{
		swe_rise_trans( startjdrise, SE_SUN, 0, 0, SE_CALC_RISE|flag, rsmi, 0, 0, tret, err );
		PRINT_PEDANTIC
		*sunrise = tret[0];
	}
	if ( sunset )
	{
		swe_rise_trans( startjdset, SE_SUN, 0, 0, SE_CALC_SET|flag, rsmi, 0, 0, tret, err );
		PRINT_PEDANTIC
		*sunset = tret[0];
	}

	if ( noon )
	{
		swe_rise_trans( startjdnoon, SE_SUN, 0, 0, SE_CALC_MTRANSIT|flag, rsmi, 0, 0, tret, err );
		PRINT_PEDANTIC
		*noon = tret[0];
	}

	if ( midnight )
	{
		swe_rise_trans( startjdmidnight, SE_SUN, 0, 0, SE_CALC_ITRANSIT|flag, rsmi, 0, 0, tret, err );
		PRINT_PEDANTIC
		*midnight = tret[0];
	}
	return daybirth;
}

/*****************************************************
**
**   CalculatorSwe   ---   CalcDateForAscendantPos
**
******************************************************/
double CalculatorSwe::CalcDateForAscendantPos( DataSet *d, const double &dest )
{
	double diff, alen, speed;
	int count = 0;
	DataSet dataset( *d );
	Location *loc = dataset.getLocation();
	double lat = loc->getLatitude();
	double lon = loc->getLongitude();
	speed = 720;
	do
	{
		//calcPositionSpeed( &dataset, 1, alen, dummy, mspeed, true, true );
		alen = calcAscendantAya( dataset.getJD(), lat, lon, true );

		diff = alen - dest;
		if ( diff > 200 ) diff -= 360;
		if ( diff < -200 ) diff += 360;
		dataset.setDate( dataset.getJD() - diff / speed );
	}
	while (( fabs( diff ) > 0.0001 ) && ( count++ < 40 ));

	if ( count > 39 ) printf("Too many iterations for calculation of ascecndant length\n" );
	return dataset.getJD();
}

/*****************************************************
**
**   CalculatorSwe   ---   calcAscendantPortion
**
******************************************************/
double CalculatorSwe::calcAscendantPortion( DataSet *d, const double &alen )
{
	int nakshatra;
	double nak_len, jd1, jd2, len1, len2;

	nakshatra = (int)( alen / NAKSHATRA_LEN );
	nak_len = alen - nakshatra * NAKSHATRA_LEN;
	if ( config->vNakshatraPortionMode ) // according to entry time
	{
		len1 = nakshatra * NAKSHATRA_LEN;
		len2 = ( nakshatra + 1 ) * NAKSHATRA_LEN;
		jd1 = CalcDateForAscendantPos( d, len1 );
		jd2 = CalcDateForAscendantPos( d, len2 );
		return ( d->getJD() - jd1 ) / ( jd2 - jd1 );
	}
	else // according to length
	{
		return nak_len / NAKSHATRA_LEN;
	}
}

/*****************************************************
**
**   CalculatorSwe   ---   calcNakshatraPortion
**
******************************************************/
double CalculatorSwe::calcNakshatraPortion( DataSet *d, const double &mlen, const bool usepada )
{
	int nakshatra, pada;
	double nak_len, p_len, len1, len2, jd1, jd2;

	nakshatra = (int)( mlen / NAKSHATRA_LEN );
	nak_len = mlen - nakshatra * NAKSHATRA_LEN;
	pada = (int)(nak_len / PADA_LEN );
	p_len = mlen - ( nakshatra * NAKSHATRA_LEN + pada * PADA_LEN );
	if ( usepada )
	{
		len1 = nakshatra * NAKSHATRA_LEN + pada * PADA_LEN;
		len2 = nakshatra * NAKSHATRA_LEN + ( pada + 1 ) * PADA_LEN;
	}
	else
	{
		len1 = nakshatra * NAKSHATRA_LEN;
		len2 = ( nakshatra + 1 ) * NAKSHATRA_LEN;
	}

	if ( config->vNakshatraPortionMode ) // according to entry time
	{
		jd1 = CalcDateForMoonPos( d, len1 );
		jd2 = CalcDateForMoonPos( d, len2 );
		return ( d->getJD() - jd1 ) / ( jd2 - jd1 );
	}
	else // according to length
	{
		if ( ! usepada ) return nak_len / NAKSHATRA_LEN;
		else return p_len / PADA_LEN;
	}
}

/*****************************************************
**
**   CalculatorSwe   ---   calcIFlag
**
******************************************************/
long CalculatorSwe::calcIFlag( DataSet *d )
{
	long iflag = 0;
	if ( config->useTruePositions ) iflag |= SEFLG_TRUEPOS;
	switch ( config->ephemCoordinateSystem )
	{
	case EPHEM_COORDINATES_GEOCENTRIC:
		// do nothing
		break;
	case EPHEM_COORDINATES_TOPOCENTRIC:
		swe_set_topo( d->getLocation()->getLongitude(), d->getLocation()->getLatitude(), 0 );
		iflag |= SEFLG_TOPOCTR;
		break;
	case EPHEM_COORDINATES_EQUATORIAL:
		iflag |= SEFLG_EQUATORIAL;
		break;
	case EPHEM_COORDINATES_BARYCENTRIC:
		iflag |= SEFLG_BARYCTR;
		break;
	case EPHEM_COORDINATES_HELIOCENTRIC:
		iflag |= SEFLG_HELCTR ;
		break;
	default:
		assert( false );
		break;
	}
	return iflag;
}

/*****************************************************
**
**   CalculatorSwe   ---   calcPositionSpeed
**
******************************************************/
void CalculatorSwe::calcPositionSpeed( DataSet *d, const int &planet, double &slen, double &slat, double &sspeed,
	const bool aya, const bool indian )
{

	double r[6];
	char err[256];

	if ( planet == OMEANDESCNODE )
	{
		swe_calc( calcJd( d->getJD() ), mapOToS( OMEANNODE ), calcIFlag( d ) | SEFLG_SPEED, r, err );
		r[0] += 180;
	}
	else if ( planet == OTRUEDESCNODE )
	{
		swe_calc( calcJd( d->getJD() ), mapOToS( OTRUENODE ), calcIFlag( d ) | SEFLG_SPEED, r, err );
		r[0] += 180;
	}
	else swe_calc( calcJd( d->getJD() ), mapOToS( planet ), calcIFlag( d ) | SEFLG_SPEED, r, err );
	PRINT_PEDANTIC

	if ( r[0] != 0.0 )
	{
		slen = red_deg( r[0] - ( aya ? calcAyanamsa( d->getJD(), ( indian ? config->vAyanamsa : config->wAyanamsa )) : 0 ));
		slat = r[1];
		sspeed = r[3];
		if ( sspeed == 0 )
		{
			printf( "WARN CalculatorSwe::calcPositionSpeed speed is null planet %d len %.12f\n", planet, slen );
		}
	}
	else
	{
		// happens if data files are missing (planetoids, Chiron)
		slen = 0;
		slat = 0;
		sspeed = 0;
	}
}

/*****************************************************
**
**   CalculatorSwe   ---   calcPosition
**
******************************************************/
void CalculatorSwe::calcPosition( DataSet *d, const int &planet, double &slen, double &slat,
                                  const bool aya, const bool indian )
{
	double r[6];
	char err[256];
	swe_calc( calcJd( d->getJD() ), mapOToS( planet ), calcIFlag( d ), r, err );
	PRINT_PEDANTIC
	slen = red_deg( r[0] - ( aya ? calcAyanamsa( d->getJD(), ( indian ? config->vAyanamsa : config->wAyanamsa )) : 0 ));
	slat = r[1];
}

/*****************************************************
**
**   CalculatorSwe   ---   getHouseSystemCode
**
******************************************************/
int CalculatorSwe::getHouseSystemCode( const int &hsys )
{
	int c = 'P';
	switch ( hsys )
	{
	case HOUSE_SYSTEM_NONE:
	case HOUSE_SYSTEM_PLACIDUS:
		c = 'P';
		break;
	case HOUSE_SYSTEM_KOCH:
		c = 'K';
		break;
	case HOUSE_SYSTEM_REGIOMONTANUS:
		c = 'R';
		break;
	case HOUSE_SYSTEM_CAMPANUS:
		c = 'C';
		break;
	case HOUSE_SYSTEM_PORPHYRY:
		c = 'O';
		break;
	case HOUSE_SYSTEM_EQUAL:
		c = 'E';
		break;
	case HOUSE_SYSTEM_VEHLOW:
		c = 'V';
		break;
	case HOUSE_SYSTEM_AXIAL:
		c = 'X';
		break;
	case HOUSE_SYSTEM_AZIMUTAL:
		c = 'H';
		break;
	case HOUSE_SYSTEM_POLICH:
		c = 'T';
		break;
	case HOUSE_SYSTEM_ALCABITUS:
		c = 'B';
		break;
	case HOUSE_SYSTEM_MORINUS:
		c = 'M';
		break;
	case HOUSE_SYSTEM_KRUSINSKI:
		c = 'U';
		break;
	default:
		assert( false );
		break;
	}
	return c;
}

/*****************************************************
**
**   CalculatorSwe   ---   calcHouses
**
******************************************************/
void CalculatorSwe::calcHouses( const double &jd, const double &lon, const double &lat, const int &hsys, double *r )
{
	double cusp[13], ascmc[10];
	swe_houses( jd, lat, lon, getHouseSystemCode( hsys ), cusp, ascmc );
	for ( int i = 0; i < 12; i++ ) r[i] = cusp[1+i];
}

/*****************************************************
**
**   CalculatorSwe   ---   CalcDateForMoonPos
**
******************************************************/
double CalculatorSwe::CalcDateForMoonPos( DataSet *d, const double &dest )
{
	double diff, mlen, dummy, mspeed;
	int count = 0;
	DataSet dataset( *d );
	do
	{
		calcPositionSpeed( &dataset, 1, mlen, dummy, mspeed, true, true );

		diff = mlen - dest;
		if ( diff > 200 ) diff -= 360;
		if ( diff < -200 ) diff += 360;
		dataset.setDate( dataset.getJD() - diff / mspeed );
	}
	while (( fabs( diff ) > 0.0000001 ) && ( count++ < 20 ));

	if ( count > 19 ) printf("Too many iterations for calculation of moon length\n" );
	return dataset.getJD();
}

/*****************************************************
**
**   CalculatorSwe   ---   calc_jd
**
******************************************************/
double CalculatorSwe::calc_jd( const int &year, const int &month, const int &day, const double &time )
{
	return swe_julday( year, month, day, time, 1 );
}

/*****************************************************
**
**   CalculatorSwe   ---   calcTajakaJd
**
******************************************************/
double CalculatorSwe::calcTajakaJd( const DataSet *dataset, const int &tyear, const bool &vedic )
{
	double diff, slen, dummy, speed, birthlen;
	int count = 0;
	int dummy2, dummy3, year;
	Formatter::get()->getDateIntsFromJD( dataset->getJD(), dummy2, dummy3, year );

	DataSet d( *dataset );
	calcPosition( &d, 0, birthlen, dummy, true, vedic );
	d.setDate( dataset->getJD() + ( tyear - year ) * 365.25 );
	do
	{
		calcPositionSpeed( &d, 0, slen, dummy, speed, true, vedic );

		diff = slen - birthlen;
		if ( diff > 200 ) diff -= 360;
		if ( diff < -200 ) diff += 360;
		d.setDate( d.getJD() - diff / speed );
	}
	while (( fabs( diff ) > 0.0000001 ) && ( count++ < 20 ));

	if ( count > 19 ) printf( "Too many iterations for calculation of Sun length\n" );
	return d.getJD();
}

/***************************************************************
**
** CalculatorSwe   ----  getDateIntsFromJD
**
***************************************************************/
bool CalculatorSwe:: getDateIntsFromJD( const double &jd, int &year, int &month, int &day, double &ut )
{
	swe_revjul ( jd, 1, &year, &month, &day, &ut );
	return true;
}

/*****************************************************
**
**   CalculatorSwe   ---   calcAscendant
**
******************************************************/
double CalculatorSwe::calcAscendant( const double &jd, const double &lat, const double &lon )
{
	double cusp[13], ascmc[10];
	swe_houses( jd, lat, lon, 'P', cusp, ascmc );

	return ascmc[0];
}

/*****************************************************
**
**   CalculatorSwe   ---   calcAscendantAya
**
******************************************************/
double CalculatorSwe::calcAscendantAya( const double &jd, const double &lat, const double &lon, bool indian )
{
	double ayanamsa = 0;
	double cusp[13], ascmc[10];
	swe_houses( jd, lat, lon, 'P', cusp, ascmc );

	ayanamsa = calcAyanamsa( calcJd( jd ), ( indian ? config->vAyanamsa : config->wAyanamsa ));
	return red_deg( ascmc[0] - ayanamsa );
}

/*****************************************************
**
**   CalculatorSwe   ---   calcAscendantByArmc
**
******************************************************/
double CalculatorSwe::calcAscendantByArmc( const double &armc, const double &eps, const double &lat, double *r, const int hsys )
{
	int i;
	double cusp[13], ascmc[10];
	swe_houses_armc( armc, lat, eps, getHouseSystemCode( hsys ), cusp, ascmc );
	if ( r != 0 )
	{
		for ( i = 0; i < 12; i++ ) r[i] = cusp[1+i];
	}
	return ascmc[0];
}

/*****************************************************
**
**   CalculatorSwe   ---   calcMC
**
******************************************************/
double CalculatorSwe::calcMC( const double &jd, const double &lat, const double &lon )
{
	double cusp[13], ascmc[10];
	swe_houses( jd, lat, lon, 'P', cusp, ascmc );

	return ascmc[1];
}

/*****************************************************
**
**   CalculatorSwe   ---   calcMCAya
**
******************************************************/
double CalculatorSwe::calcMCAya( const double &jd, const double &lat, const double &lon, bool indian )
{
	double cusp[13], ascmc[10];
	swe_houses( jd, lat, lon, 'P', cusp, ascmc );

	return red_deg( ascmc[1] - calcAyanamsa( jd, ( indian ? config->vAyanamsa : config->wAyanamsa )  ));
}

/*****************************************************
**
**   CalculatorSwe   ---   calcNextEclipse
**
******************************************************/
EclipseData CalculatorSwe::calcNextEclipse( const double &jd, const int &planet, const int &type, const bool &vedic )
{
	EclipseData data;
	double tret[10];
	char err[256];
	int ret;
	double dummy;
	DataSet d( config->defaultLocation );
	d.setDate( jd );

	if ( planet == OSUN ) ret = swe_sol_eclipse_when_glob( calcJd( jd ), 0, 0, tret, false, err );
	else ret = swe_lun_eclipse_when( calcJd( jd ), 0, 0, tret, false, err );
	PRINT_PEDANTIC
	data.jd = tret[0];
	d.setDate( data.jd );
	data.type = ret;
	data.planet = planet;
	calcPosition( &d, 0, data.sunpos, dummy, true, vedic );
	calcPosition( &d, 1, data.moonpos, dummy, true, vedic );
	if ( ret & SE_ECL_TOTAL ) data.description = _( "Total" );
	else if ( ret & SE_ECL_ANNULAR ) data.description = _( "Annular" );
	else if ( ret & SE_ECL_PARTIAL ) data.description = _( "Partial" );
	else if ( ret & SE_ECL_PENUMBRAL ) data.description = _( "Penumbral" );
	else if ( ret & SE_ECL_ANNULAR_TOTAL ) data.description = _( "Annular Total" );
	else data.description = _( "unknown" );

	return data;
}

/*****************************************************
**
**   CalculatorSwe   ---   calcMcFromAsc
**
******************************************************/
double CalculatorSwe::calcMcFromAsc( const double &ascpos, const double &jd, const double &lon, const double &lat )
{
	double pos[3], xin[2], xaz[3];
	pos[0] = lon;
	pos[1] = lat;
	pos[2] = 0;

	xin[0] = ascpos;
	xin[1] = 0;
	//xin[0] = 0;

	swe_azalt( jd, 	SE_ECL2HOR, pos, 0, 10, xin, xaz );
	return 0;
}

/*****************************************************
**
**   CalculatorSwe   ---   calcPlanetaryEvent
**
******************************************************/
double CalculatorSwe::calcPlanetaryEvent( const DataSet *d, const double &targetlen, const int &planet,
        const bool &vedic )
{
	double diff, len, dummy, speed, tlen;
	DataSet dataset( *d );

	int count = 0;
	int pp = planet;

	tlen = targetlen;
	// map both rahu and ketu to the same node (depending on conf for inode)
	if ( planet == OMEANDESCNODE )
	{
		pp = OMEANNODE;
		tlen = red_deg( tlen + 180 );
	}
	else if ( planet == OTRUEDESCNODE )
	{
		pp = OTRUENODE;
		tlen = red_deg( tlen + 180 );
	}

	do
	{
		calcPositionSpeed( &dataset, pp, len, dummy, speed, true, vedic );

		// bugfix 6.0: found ingress event with speed=0 (Month 1/11, planet Jupiter, western ephemeris)
		if ( speed == 0 )
		{
			printf( "CalculatorSwe::calcPlanetaryEvent WARN: planetary speed is null: planet %d, targetlen %f date %f calculated len %.12f\n",
			        planet, targetlen, dataset.getJD(), len );
			speed = .0000000001;
		}

		//if ( planet == OTRUEDESCNODE || planet == OMEANDESCNODE ) len = red_deg( len + 180 ); // ketu gets len of rahu + 180

		diff = len - tlen;
		if ( diff > 200 ) diff -= 360;
		if ( diff < -200 ) diff += 360;
		dataset.setDate( dataset.getJD() - diff / speed );
	}
	while (( fabs( diff ) > 0.0000001 ) && ( count++ < 20 ));

	if ( count > 19 )
	{
		printf( "Too many iterations for calculation of planetary length\n" );
	}
	return dataset.getJD();
}

/*****************************************************
**
**   CalculatorSwe   ---   calcSunMoonEvent
**
******************************************************/
double CalculatorSwe::calcSunMoonEvent( DataSet *d, const double &angle, double &slen, double &mlen )
{
	double mspeed, sspeed, dummy, a, diff;
	int count = 0;
	const double targettol = .000001;
	do
	{
		calcPositionSpeed( d, OSUN, slen, dummy, sspeed, true, false );
		calcPositionSpeed( d, OMOON, mlen, dummy, mspeed, true, false );
		a = red_deg( mlen - slen );
		diff = angle - a;
		if ( diff > 200 ) diff -= 360;
		if ( diff < -200 ) diff += 360;
		d->setDate( d->getJD() + diff / ( mspeed - sspeed ));

		count++;
	}
	while ( fabs( diff ) > targettol && count < 20 );

	return d->getJD();
}

/*****************************************************
**
**   CalculatorSwe   ---   calcJd
**
******************************************************/
double CalculatorSwe::calcJd( const double &jd )
{
	return( config->useEphemerisTime ? jd : jd + swe_deltat( jd ));
}

/*****************************************************
**
**   CalculatorSwe   ---   mapOToS
**
******************************************************/
int CalculatorSwe::mapOToS( const int &i )
{
	switch ( i )
	{
	case OSUN      :
		return SE_SUN;
		break;
	case OMOON     :
		return SE_MOON;
		break;
	case OMERCURY  :
		return SE_MERCURY;
		break;
	case OVENUS    :
		return SE_VENUS;
		break;
	case OMARS     :
		return SE_MARS;
		break;
	case OJUPITER  :
		return SE_JUPITER;
		break;
	case OSATURN   :
		return SE_SATURN;
		break;
	case OURANUS   :
		return SE_URANUS;
		break;
	case ONEPTUNE  :
		return SE_NEPTUNE;
		break;
	case OPLUTO    :
		return SE_PLUTO;
		break;
	case OMEANNODE :
		return SE_MEAN_NODE;
		break;
	case OTRUENODE :
		return SE_TRUE_NODE;
		break;
	case OCUPIDO   :
		return SE_CUPIDO;
		break;
	case OHADES    :
		return SE_HADES;
		break;
	case OZEUS     :
		return SE_ZEUS;
		break;
	case OKRONOS   :
		return SE_KRONOS;
		break;
	case OAPOLLON  :
		return SE_APOLLON;
		break;
	case OADMETOS  :
		return SE_ADMETOS;
		break;
	case OVULKANUS :
		return SE_VULKANUS;
		break;
	case OPOSEIDON :
		return SE_POSEIDON;
		break;
	case OCHIRON   :
		return SE_CHIRON;
		break;
	case OPHOLUS   :
		return SE_PHOLUS;
		break;
	case OCERES    :
		return SE_CERES;
		break;
	case OPALLAS   :
		return SE_PALLAS;
		break;
	case OJUNO     :
		return SE_JUNO;
		break;
	case OVESTA    :
		return SE_VESTA;
		break;
	case OLILITH   :
		return SE_MEAN_APOG;
		break;
	default        :
		assert( 0 );
		break;
	}
	return -1;
}

