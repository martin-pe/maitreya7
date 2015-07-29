/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/Horoscope.cpp
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
#pragma implementation "Horoscope.h"
#endif

#include "Horoscope.h"

#include "ArabicParts.h"
#include "astrobase.h"
#include "Calculator.h"
#include "Conf.h"
#include "Dasa.h"
#include "Lang.h"
#include "Planet.h"
#include "mathbase.h"

extern Config *config;

IMPLEMENT_CLASS( Horoscope, SimpleHoroscope )
IMPLEMENT_CLASS( TajakaHoroscope, Horoscope )

/*****************************************************
**
**   Horoscope   ---   Constructor
**
******************************************************/
Horoscope::Horoscope()
		: SimpleHoroscope()
{
	int i;

	for ( i = 0; i < NUM_LAGNA; i++ ) lagna_len[i] = 0;
	for ( i = 0; i < NUM_UPA; i++ ) upagraha_len[i] = 0;
	for ( i = FIRST_EPHEM_OBJECT; i <= LAST_EPHEM_OBJECT; i++ ) ephem_planets[i] = new Planet( this, i );

	for ( i = 0; i < MAX_EXTENDED_OBJECTS; i++ )
	{
		evpos[i] = new ObjectPosition;
		ewpos[i] = new ObjectPosition;
		extended_planets[i] = new Planet( this, FIRST_EXTENDED_OBJECT + i );
	}
	arabic_expert = new ArabicPartsExpert( this );
	arieslen = 0;
}

/*****************************************************
**
**   Horoscope   ---   Destructor
**
******************************************************/
Horoscope::~Horoscope()
{
	int i;
	for ( i = 0; i < MAX_EPHEM_OBJECTS; i++ ) delete ephem_planets[i];
	for ( i = 0; i < MAX_EXTENDED_OBJECTS; i++ ) delete extended_planets[i];
	for ( i = 0; i < MAX_EXTENDED_OBJECTS; i++ )
	{
		delete evpos[i];
		delete ewpos[i];
	}
	delete arabic_expert;
}

/*****************************************************
**
**   Horoscope   ---   update
**
******************************************************/
void Horoscope::update()
{
	int i, j, lord;
	double step_len, lsun, thejd, asc;
	Calculator *calculator = CalculatorFactory().getCalculator();
	ObjectPosition pos;
	double startjdrise, startjdset;

	// SimpleHoroscope
	updatePlanets();

	// daybirth, sunrise and sunset
	double diff_ascsun = red_deg( getTropicalLength( OASCENDANT ) -  getTropicalLength( OSUN ));
	isDayBirth = ( diff_ascsun < 180 );

	startjdrise = startjdset = getJD();
	if ( isDayBirth ) startjdrise--;
	else startjdset--;

	sunrise = calculator->calcNextSolarEvent( SOLAR_EVENT_SUNRISE, startjdrise, getLocation()->getLatitude(), getLocation()->getLongitude());
	sunset = calculator->calcNextSolarEvent( SOLAR_EVENT_SUNSET, startjdset, getLocation()->getLatitude(), getLocation()->getLongitude());

	// Upagraha Update
	lsun = object_len[OSUN];
	// Dhuma
	upagraha_len[CDHUMA] =  red_deg( lsun + 133.333333333 );
	// Vyatipata unused, ref Dhuma
	upagraha_len[CVYATIPATA] = 0;
	// Parivesha unsused, ref Dhuma
	upagraha_len[CPARIVESHA] = 0;
	// Chapa
	upagraha_len[CCHAPA] = red_deg( lsun + 313.333333333 );
	// Upaketu
	upagraha_len[CUPAKETU] = red_deg( lsun + 330 );

	// Kalavelas
	for ( i = 0; i <= 8; i++ )
	{
		if ( isDayBirth )
		{
			lord = ( getDataSet()->getWeekDay() + i ) % 8;
			step_len = .125 * fabs( sunset - sunrise );
			thejd = sunrise + i * step_len;
		}
		else
		{
			lord = ( getDataSet()->getWeekDay() + i + 4 ) % 8;
			step_len = .125 * fabs( sunrise - sunset );
			// FIXME
			if ( step_len < 0 ) step_len *= -1;
			thejd = sunset + i * step_len;
		}
		for ( j = 0; j < 6; j++ )
		{
			if ( config->kalaVelaLord[j] == lord )
			{
				// Default: config->kalaVelaPortion[j] == KALAVELA_PORTION_START, do nothing
				if ( config->kalaVelaPortion[j] == KALAVELA_PORTION_MIDDLE ) thejd += .5* step_len;
				else if ( config->kalaVelaPortion[j] == KALAVELA_PORTION_END ) thejd += step_len;

				asc = red_deg( CalculatorFactory().getCalculator()->calcAscendant( thejd, getLocation()->getLatitude(),
				               getLocation()->getLongitude() ));
				upagraha_len[CKALA+j] = asc;
			}
		}
	}

	// Special Lagnas
	lagna_len[CBHAVA_LAGNA] = red_deg( 360 * ( getJD() - sunrise ) + lsun);
	lagna_len[CHORA_LAGNA] = red_deg( 720 * ( getJD() - sunrise ) + lsun);
	lagna_len[CGHATIKA_LAGNA] = red_deg( 1800 * ( getJD() - sunrise ) + lsun);

	updateObjectPositions();

	double angle = red_deg( object_len[ OMOON ] - object_len[ OSUN ] );
	switch ( config->vMoonBenefic )
	{
	case 1: // angle < 180
		moonBenefic = ( angle < 180 );
		break;
	case 2: // 120 < angle < 240
		moonBenefic = ( angle > 120 && angle < 240 );
		break;
	case 3: // always
		moonBenefic =  true;
		break;
	case 0: // never
	default:
		moonBenefic = false;
		break;
	}

	mercuryBenefic = true;
	switch ( config->vMercuryBenefic )
	{
	case 1: // no malefic conjunction
	{
		const int mpos = getRasi( getVedicLength( OMERCURY ));
		if ( getRasi( getVedicLength( OSUN )) == mpos ) {
			mercuryBenefic = false;
			break;
		}
		if ( ! moonBenefic && getRasi( getVedicLength( OMOON )) == mpos ) {
			mercuryBenefic = false;
			break;
		}
		if ( getRasi( getVedicLength( OMARS )) == mpos ) {
			mercuryBenefic = false;
			break;
		}
		if ( getRasi( getVedicLength( OSATURN )) == mpos ) {
			mercuryBenefic = false;
			break;
		}
		if ( getRasi( getVedicLength(
		                  config->vLunarNodeMode == LUNAR_NODE_TRUE ? OTRUENODE : OMEANNODE )) == mpos ) {
			mercuryBenefic = false;
			break;
		}
		if ( getRasi( getVedicLength(
		                  config->vLunarNodeMode == LUNAR_NODE_TRUE ? OTRUEDESCNODE : OMEANDESCNODE )) == mpos ) {
			mercuryBenefic = false;
			break;
		}
	}
	case 2: // always
		mercuryBenefic = true;
		break;
	case 0: // never
	default:
		mercuryBenefic = false;
		break;
	}

	// Arabic parts
	arabic_expert->update();
}

/**************************************************************
***
**    Horoscope   ---   updateObjectPositions
***
***************************************************************/
void Horoscope::updateObjectPositions()
{
	int p, pid;

	for ( p = 0; p < MAX_EPHEM_OBJECTS; p++ ) ephem_planets[p]->update( getVedicLength( p ));

	// Attention: must be separated from updateVargaData because Vargas reference each other
	for ( p = 0; p <= OSATURN; p++ ) ephem_planets[p]->calculateDignities();

	for ( p = 0; p < MAX_EXTENDED_OBJECTS; p++ )
	{
		pid = p + FIRST_EXTENDED_OBJECT;
		switch ( pid )
		{
		case ODHUMA:
		case OVYATIPATA:
		case OPARIVESHA:
			evpos[p]->length = ewpos[p]->length = upagraha_len[CDHUMA];
			break;
		case OCHAPA:
			evpos[p]->length = ewpos[p]->length = upagraha_len[CCHAPA];
			break;
		case OUPAKETU:
			evpos[p]->length = ewpos[p]->length = upagraha_len[CUPAKETU];
			break;
		case OKALA:
			evpos[p]->length = ewpos[p]->length = upagraha_len[CKALA];
			break;
		case OMRITYU:
			evpos[p]->length = ewpos[p]->length = upagraha_len[CMRITYU];
			break;
		case OARDHAPRAHARA:
			evpos[p]->length = ewpos[p]->length = upagraha_len[CARDHAPRAHARA];
			break;
		case OYAMAGHANTAKA:
			evpos[p]->length = ewpos[p]->length = upagraha_len[CYAMAGHANTAKA];
			break;
		case OGULIKA:
			evpos[p]->length = ewpos[p]->length = upagraha_len[CGULIKA];
			break;
		case OMANDI:
			evpos[p]->length = ewpos[p]->length = upagraha_len[CMANDI];
			break;
		case OBHAVA_LAGNA:
			evpos[p]->length = ewpos[p]->length = lagna_len[CBHAVA_LAGNA];
			break;
		case OHORA_LAGNA:
			evpos[p]->length = ewpos[p]->length = lagna_len[CHORA_LAGNA];
			break;
		case OGHATIKA_LAGNA:
			evpos[p]->length = ewpos[p]->length = lagna_len[CGHATIKA_LAGNA];
			break;
		case OARIES:
			evpos[p]->length = ewpos[p]->length = arieslen;
			break;

			// Calculate D9 Lagna separately because Ayanamsa must be multiplied, too
		case OD9LAGNA:
			evpos[p]->length = red_deg( 9 * red_deg( object_len[OASCENDANT] - iayanamsa ));
			ewpos[p]->length = red_deg( 9 * ( object_len[OASCENDANT] - wayanamsa ));
			break;
		default:
			assert( false );
			break;
		}

		if ( pid != OD9LAGNA )
		{
			evpos[p]->length = red_deg( evpos[p]->length - iayanamsa );
			ewpos[p]->length = red_deg( ewpos[p]->length - wayanamsa );
		}
		if ( pid == OVYATIPATA )
		{
			evpos[p]->length = red_deg( 360 - evpos[p]->length );
			ewpos[p]->length = red_deg( 360 - ewpos[p]->length );
		}
		else if ( pid == OPARIVESHA )
		{
			evpos[p]->length = red_deg( 180 - evpos[p]->length );
			ewpos[p]->length = red_deg( 180 - ewpos[p]->length );
		}
		extended_planets[p]->update( evpos[p]->length );
	}
}

/**************************************************************
***
**    Horoscope   ---   getVargaData
***
***************************************************************/
VargaData *Horoscope::getVargaData( const int &planet, const int &varga) const
{
	ASSERT_VALID_OBJECT( planet )
	ASSERT_VALID_VARGA( varga )
	return getPlanet( planet )->getVargaData( varga );
}

/**************************************************************
***
**   Horoscope   ---   getPlanet
***
***************************************************************/
Planet *Horoscope::getPlanet( const int &planet ) const
{
	ASSERT_VALID_OBJECT( planet )
	if ( planet <= LAST_EPHEM_OBJECT )
	{
		return ephem_planets[planet];
	}
	else if ( planet >= FIRST_EXTENDED_OBJECT && planet <= LAST_EXTENDED_OBJECT )
	{
		return extended_planets[planet - FIRST_EXTENDED_OBJECT];
	}
	else
	{
		ASSERT_VALID_ARABIC_OBJECT( planet )
		return arabic_expert->getPlanet( planet );
	}
}

/*****************************************************
**
**   Horoscope   ---   getObjectPosition
**
******************************************************/
ObjectPosition Horoscope::getObjectPosition( const int &planet, const bool &vedic ) const
{
	ASSERT_VALID_OBJECT( planet )
	if ( planet <= LAST_EPHEM_OBJECT ) return SimpleHoroscope::getObjectPosition( planet, vedic );
	else if ( planet <= LAST_EXTENDED_OBJECT ) return( vedic ? *evpos[planet - FIRST_EXTENDED_OBJECT] : *ewpos[planet - FIRST_EXTENDED_OBJECT] );
	else return arabic_expert->getObjectPosition( planet, vedic );
}

/*****************************************************
**
**   Horoscope   ---   isBenefic
**
******************************************************/
bool Horoscope::isBenefic( const int &planet ) const
{
	ASSERT_VALID_OBJECT( planet )
	switch ( planet )
	{
	case OMOON:
		return moonBenefic;
		break;
	case OMERCURY:
		return mercuryBenefic;
		break;
	case OVENUS:
	case OJUPITER:
		return true;
		break;
	default:
		return false;
		break;
	}
	return false;
}

/*****************************************************
**
**   Horoscope   ---   updateKP
**
******************************************************/
void Horoscope::updateKP( const int &dasaindex ) const
{
	int i;
	DasaExpert *expert = DasaExpertFactory::get()->getDasaExpert( dasaindex );

	for ( i = 0; i < MAX_EPHEM_OBJECTS; i++ )
		kp_lord[i] = expert->getKPLords( getVedicLength( i ));

	for ( i = 0; i < MAX_EXTENDED_OBJECTS; i++ )
		ekp_lord[i] = expert->getKPLords( getVedicLength( i + FIRST_EXTENDED_OBJECT ));

	for ( i = 0; i < MAX_ARABIC_OBJECTS; i++ )
		akp_lord[i] = expert->getKPLords( getVedicLength( i + FIRST_ARABIC_OBJECT ));

	for ( i = HOUSE1; i <= HOUSE12; i++ )
		hkp_lord[i] = expert->getKPLords( getHouse( i, true, false ));
}

/*****************************************************
**
**   Horoscope   ---   getHouseKPLords
**
******************************************************/
KpData Horoscope::getHouseKPLords( const int &house ) const
{
	ASSERT_VALID_HOUSE( house )
	return hkp_lord[house];
}

/*****************************************************
**
**   Horoscope   ---   getKPLords
**
******************************************************/
KpData Horoscope::getKPLords( const int &planet ) const
{
	ASSERT_VALID_OBJECT( planet );

	if ( planet <= LAST_EPHEM_OBJECT )
	{
		return kp_lord[planet];
	}
	else if ( planet <= LAST_EXTENDED_OBJECT )
	{
		return ekp_lord[planet - FIRST_EXTENDED_OBJECT];
	}
	else
	{
		ASSERT_VALID_ARABIC_OBJECT( planet )
		return kp_lord[planet - FIRST_ARABIC_OBJECT];
	}
}

/*****************************************************
**
**   TajakaHoroscope   ---   Constructor
**
******************************************************/
TajakaHoroscope::TajakaHoroscope( DataSet *dataset )
{
	birth_ds = dataset;
	setHName( _( "Tajaka" ));
	setLocation( *dataset->getLocation() );
}

/*****************************************************
**
**   TajakaHoroscope   ---   calcTajaka
**
******************************************************/
void TajakaHoroscope::calcTajaka( const int &syear, const bool &vedic )
{
	int dummy, month, year;
	Formatter::get()->getDateIntsFromJD( birth_ds->getJD(), dummy, month, year );
	Calculator *calculator = CalculatorFactory().getCalculator();
	double jd = calculator->calcTajakaJd( birth_ds, syear, vedic );
	setDate( jd );
}

