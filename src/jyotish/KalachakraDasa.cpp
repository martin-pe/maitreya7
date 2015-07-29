/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/KalachakraDasa.cpp
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
#pragma implementation "KalachakraDasa.h"
#endif

#include "KalachakraDasa.h"

#include "astrobase.h"
#include "Calculator.h"
#include "Conf.h"
#include "Horoscope.h"
#include "Lang.h"
#include "Planet.h"

extern Config *config;

const wxChar KalachakraDasaExpert::K_KALA[4][4][9] =
{
	{
		{ ARIES, TAURUS, GEMINI, CANCER, LEO, VIRGO, LIBRA, SCORPIO, SAGITTARIUS },
		{ CAPRICORN, AQUARIUS, PISCES, SCORPIO, LIBRA, VIRGO, CANCER, LEO, GEMINI },
		{ TAURUS, ARIES, PISCES, AQUARIUS, CAPRICORN, SAGITTARIUS, ARIES, TAURUS, GEMINI },
		{ CANCER, LEO, VIRGO, LIBRA, SCORPIO, SAGITTARIUS, CAPRICORN, AQUARIUS, PISCES },
	},
	{
		{ SCORPIO, LIBRA, VIRGO, CANCER, LEO, GEMINI, TAURUS, ARIES, PISCES },
		{ AQUARIUS, CAPRICORN, SAGITTARIUS, ARIES, TAURUS, GEMINI, CANCER, LEO, VIRGO },
		{ LIBRA, SCORPIO, SAGITTARIUS, CAPRICORN, AQUARIUS, PISCES, SCORPIO, LIBRA, VIRGO },
		{ CANCER, LEO, GEMINI, TAURUS, ARIES, PISCES, AQUARIUS, CAPRICORN, SAGITTARIUS },
	},
	{
		{ SAGITTARIUS, CAPRICORN, AQUARIUS, PISCES, ARIES, TAURUS, GEMINI, LEO, CANCER },
		{ VIRGO, LIBRA, SCORPIO, PISCES, AQUARIUS, CAPRICORN, SAGITTARIUS, SCORPIO, LIBRA },
		{ VIRGO, LEO, CANCER, GEMINI, TAURUS, ARIES, SAGITTARIUS, CAPRICORN, AQUARIUS },
		{ PISCES, ARIES, TAURUS, GEMINI, LEO, CANCER, VIRGO, LIBRA, SCORPIO }
	},
	{
		{ PISCES, AQUARIUS, CAPRICORN, SAGITTARIUS, SCORPIO, LIBRA, VIRGO, LEO, CANCER },
		{ GEMINI, TAURUS, ARIES, SAGITTARIUS, CAPRICORN, AQUARIUS, PISCES, ARIES, TAURUS },
		{ GEMINI, LEO, CANCER, VIRGO, LIBRA, SCORPIO, PISCES, AQUARIUS, CAPRICORN },
		{ SAGITTARIUS, SCORPIO, LIBRA, VIRGO, LEO, CANCER, GEMINI, TAURUS, ARIES }
	}
};

const int KalachakraDasaExpert::K_RASIYEARS[12] = {
	7, 16, 9, 21, 5, 9,
	16, 7, 10, 4, 4, 10
};

const int KalachakraDasaExpert::K_TOTALYEARS[4][4] =
{
	{ 100, 85, 83,  86 },
	{ 100, 85, 83,  86 },
	{  86, 83, 85, 100 },
	{  86, 83, 85, 100 }
};

const int KalachakraDasaExpert::K_NAKSHATRA_GROUP[27] = {
	0, 1, 0, 2, 3, 3, 0, 1, 0,
	2, 3, 3, 0, 1, 0, 2, 3, 3,
	0, 1, 0, 2, 3, 3, 0, 1 ,0
};

const int KalachakraDasaExpert::K_APSAVYA_AMSA[12] = {
	SCORPIO, LIBRA, GEMINI, LEO, CANCER, GEMINI,
	TAURUS, ARIES, PISCES, AQUARIUS, CAPRICORN, SAGITTARIUS
};

/*****************************************************
**
**   KalachakraDasaImpl   ---   Constructor
**
******************************************************/
KalachakraDasaImpl::KalachakraDasaImpl( DasaExpert *e, const int &lord, const double &startjd, const double &endjd,
                                        const int &pada, const int &group, const wxChar *extra_text, Dasa *parent )
		: DasaImpl( e, lord, startjd, endjd, extra_text, parent )
{
	this->pada  = pada;
	this->group = group;
}

/*****************************************************
**
**   KalachakraDasaExpert   ---   Constructor
**
******************************************************/
KalachakraDasaExpert::KalachakraDasaExpert()
{
	deha = jeeva = 0;
}

/*****************************************************
**
**   KalachakraDasaExpert   ---   getNextLevel
**
******************************************************/
vector<Dasa*> KalachakraDasaExpert::getNextLevel( Dasa *d )
{
	vector<Dasa*> ret;
	int i, ad_lord;;
	double ad_dur;

	KalachakraDasaImpl *dasa = (KalachakraDasaImpl*)d;
	int group = dasa->getGroup();
	int pada = dasa->getPada();
	int lord = dasa->getDasaLord();
	double start_jd = dasa->getStartJD();
	double end_jd = dasa->getEndJD();
	double dasa_dur = end_jd - start_jd;
	//const int level = dasa->getLevel() + 1;

	// get correct column
	int spalte = 0;
	int count = 0;
	while ( K_KALA[group][pada][spalte] != lord )
	{
		if ( ++spalte > 9 ) spalte = 0;

		if ( ++count > 10 )
		{
			assert( 0 );
		}
	}
	double total_years = K_TOTALYEARS[group][pada];

	for ( i = 0; i < 9; i++ )
	{
		ad_lord = K_KALA[group][pada][(spalte+i)%9];
		ad_dur = dasa_dur * (double)K_RASIYEARS[ad_lord] / total_years;
		end_jd = start_jd + ad_dur;
		ret.push_back( new KalachakraDasaImpl( this, ad_lord, start_jd, end_jd, pada, group, wxT( "" ), d ));

		start_jd = end_jd;
	}

	return ret;
}

/*****************************************************
**
**   KalachakraDasaExpert   ---   getFirstLevel
**
******************************************************/
vector<Dasa*> KalachakraDasaExpert::getFirstLevel( const Horoscope *h, int varga )
{
	Calculator *calculator = CalculatorFactory().getCalculator();
	vector<Dasa*> ret;
	wxChar etext[256];
	int mnak, pada, group, amsa;
	double mlen, portion;
	Lang lang;

	mlen = h->getObjectPosition( OMOON, true ).length;
	mnak = (int)( mlen / NAKSHATRA_LEN );
	group = K_NAKSHATRA_GROUP[mnak];
	savya = ( group <= 1 );

	pada = (int)( mlen / PADA_LEN );
	portion = ( config->vKalachakraMode == 3 ? 0 :  calculator->calcNakshatraPortion( h->getDataSet(), mlen, true ));
	pada %= 4;

	amsa = ( mnak % 3  ) * 4 + pada;
	if ( ! savya ) amsa = K_APSAVYA_AMSA[amsa];

	double start_jd;
	double end_jd;

	int totalyears = K_TOTALYEARS[group][pada];

	deha = savya ? K_KALA[group][pada][0] : K_KALA[group][pada][8];
	jeeva = savya ? K_KALA[group][pada][8] : K_KALA[group][pada][0];

	double elapsed = portion * totalyears;
	int spalte = 0;
	while ( elapsed - K_RASIYEARS[(int)K_KALA[group][pada][spalte]] > 0 )
	{
		elapsed -= K_RASIYEARS[(int)K_KALA[group][pada][spalte]];
		spalte++;
		if ( spalte > 8 )
		{
			assert( 0 );
		}
	}
	start_jd =  h->getJD() - elapsed  * getYearLength( true );

	for ( int i = 0; i <= 9; i++ )
	{
		*etext = 0;
		if ( spalte == 0 || i == 0 )
			wxSprintf( etext, _( "Paramayus %d years, %s Deha, %s Jeeva, Amsa %s, %s" ), totalyears,
			           (const wxChar*)lang.getSignName( deha ), (const wxChar*)lang.getSignName( jeeva ),
			           (const wxChar*)lang.getSignName( amsa ), ( savya ? _( "Savya" ) : _( "Apsavya" )));

		end_jd = start_jd + K_RASIYEARS[K_KALA[group][pada][spalte]] * getYearLength( true );

		ret.push_back( new KalachakraDasaImpl( this, K_KALA[group][pada][spalte], start_jd, end_jd,
		                                       pada, group, etext, 0 ));
		spalte++;
		if ( spalte > 8 )
		{
			spalte = 0;

			// iKalachakraMode == 1 doesn's jump to next row
			// mode 0 goes to next row or next nakshatra if last pada.
			// Last pada of savya or apsavya goes to first nakshatra of same group
			if ( config->vKalachakraMode == 0 )
			{
				if ( pada < 3 ) pada++;
				else
				{
					pada  = 0;
					mnak++;
					if ( mnak > 27 ) mnak = 0;

					// On change of savya/apsava -> don't add one nakshatra, but go back to first nakshatra of same cycle
					if (( savya && K_NAKSHATRA_GROUP[mnak] > 1 ) || ( ! savya && K_NAKSHATRA_GROUP[mnak] <= 1 )) mnak -= 3;
					if ( mnak < 0 ) mnak += 27;
					group = K_NAKSHATRA_GROUP[mnak];
				}
			}
			// Mode 2 stays in the same nakshatra
			else if ( config->vKalachakraMode == 2 )
			{
				if ( pada < 3 ) pada++;
				else pada  = 0;
			}
			deha  = savya ? K_KALA[group][pada][0] : K_KALA[group][pada][8];
			jeeva = savya ? K_KALA[group][pada][8] : K_KALA[group][pada][0];
		}
		start_jd = end_jd;
	}
	return ret;
}

/*****************************************************
**
**   KalachakraDasaExpert   ---   getKPEventList
**
******************************************************/
vector<KPEvent> KalachakraDasaExpert::getKPEventList( const double &startlen, const double &endlen,
        const double &startjd )
{
	vector<KPEvent> events;
	int i, j, pada, spalte, currentnak, group, lord, mahayears, paramayus, antarspalte, sublord;
	int antaryears;
	double len, mahalen, approxjd, inc_antarlen, dist;
	bool append;

	int index = 0;
	int startnak = getNakshatra( startlen, N27 );
	for ( i = 0; i < 32; i++ )
	{
		currentnak = (startnak + i ) % 27;
		for ( pada = 0; pada < 4; pada++ )
		{
			group = K_NAKSHATRA_GROUP[currentnak];
			len = currentnak * NAKSHATRA_LEN + pada * PADA_LEN;
			paramayus = K_TOTALYEARS[group][pada];
			for ( spalte = 0; spalte < 9; spalte++ )
			{
				index++;
				lord = K_KALA[group][pada][spalte];
				mahayears = K_RASIYEARS[lord];
				mahalen = (double)mahayears * PADA_LEN / (double)paramayus;
				inc_antarlen = 0;
				for ( j = 0; j < 9; j++ )
				{
					append = true;
					antarspalte = spalte + j;
					if ( antarspalte >= 9 ) antarspalte -= 9;
					sublord = K_KALA[group][pada][antarspalte];
					antaryears = K_RASIYEARS[sublord];
					approxjd = startjd + i + pada/4;
					if ( i == 0 ) // cut off at the beginning (antardasas before starting point)
					{
						dist = len + inc_antarlen - startlen;
						if ( dist < -300 ) dist += 360;  // correct if 360-deg shift
						if ( dist < 0 ) append = false;
					}
					if ( i >= 28 )  // cut off at the end (if event belongs to next time interval)
					{
						dist = len + inc_antarlen - endlen;
						if ( dist > 300 ) dist -= 360;
						if ( dist < -300 ) dist += 360;
						if ( dist > 0 ) append = false;     // could also be return ?
					}
					if ( append == true )
					{
						events.push_back( KPEvent( lord, sublord, len + inc_antarlen, approxjd, index ));
					}

					inc_antarlen += (double)antaryears * mahalen / (double)paramayus;
				}
				len += mahalen;
			}

		}
	}
	return events;
}

/*****************************************************
**
**   DasaExpertFactory   ---   create
**
******************************************************/
DasaExpert *DasaExpertFactory::createKalachakraDasaExpert() {
	return new KalachakraDasaExpert();
}



