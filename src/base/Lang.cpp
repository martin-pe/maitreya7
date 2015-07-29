/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/base/Lang.cpp
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
#pragma implementation "Lang.h"
#endif

#include "Lang.h"

#include <math.h>

#include <wx/log.h>
#include <wx/string.h>

#include "Calculator.h"
#include "Conf.h"
#include "constants.h"
#include "LanguageConfig.h"
#include "mathbase.h"

extern Config *config;

IMPLEMENT_SINGLETON( Formatter )

/**************************************************************
***
**   Lang   ---   Constructor
***
***************************************************************/
Lang::Lang()
{
	cfg = config;
}

/**************************************************************
***
**   Lang   ---   Constructor
***
***************************************************************/
Lang::Lang( Config *c )
{
	cfg = c;
}

/**************************************************************
***
** Lang   ---   getNakshatraName
***
***************************************************************/
const wxString Lang::getNakshatraName( const int &nak, const int &nsys, const int &format ) const
{
	assert(( nsys == N27 ) || ( nsys == N28 ));
	assert(( nsys > 0 ) || ( nsys < ( nsys == N27 ? 27 : 28 ) ));

	const static wxChar *k_nakshatra_name_large[28]  = { _( "Aswini" ), _( "Bharani" ), _( "Krittika" ), _( "Rohini" ),
		_( "Mrigasira" ), _( "Ardra" ), _( "Punarvasu" ), _( "Pushyami" ), _( "Aslesha" ), _( "Magha" ),
		_( "Poorvaphalguni" ), _( "Uttaraphalguni" ), _( "Hasta" ), _( "Chitra" ), _( "Swati" ), _( "Visakha" ),
		_( "Anuradha" ), _( "Jyeshta" ), _( "Moola" ), _( "Poorvashadha" ), _( "Uttarashadha" ), _( "Abhijit" ),
		_( "Sravana" ), _( "Dhanista" ), _( "Satabhisha" ), _( "Poorvabhadra" ), _( "Uttarabhadra" ), _( "Revati" )
	};
	const static wxChar *k_nakshatra_name_medium[28]  = {
		_( "Aswini" ), _( "Bharani" ), _( "Krittika" ), _( "Rohini" ), _( "Mrigasira" ), _( "Ardra" ),
		_( "Punarvasu" ), _( "Pushyami" ), _( "Aslesha" ), _( "Magha" ), _( "P.Phalguni" ), _( "U.Phalguni" ),
		_( "Hasta" ), _( "Chitra" ), _( "Swati" ), _( "Visakha" ), _( "Anuradha" ), _( "Jyeshta" ),
		_( "Moola" ), _( "P.Shadha" ), _( "U.Shadha" ), _( "Abhijit" ), _( "Sravana" ), _( "Dhanista" ),
		_( "Satabisha" ), _( "P.Bhadra" ), _( "U.Bhadra" ), _( "Revati" )
	};
	const static wxChar *k_nakshatra_name_short[28]  = { _( "Asw" ), _( "Bhr" ), _( "Kri" ), _( "Roh" ),
		_( "Mri" ), _( "Ard" ), _( "Pun" ), _( "Pus" ), _( "Asl" ), _( "Mak" ), _( "PPl" ),
		_( "UPl" ), _( "Hst" ), _( "Cit" ), _( "Sva" ), _( "Vis" ), _( "Anu" ), _( "Jye" ),
		_( "Mul" ), _( "PSa" ), _( "USa" ), _( "Abh" ), _( "Sra" ), _( "Dha" ), _( "Sat" ),
		_( "PBa" ), _( "UBa" ), _( "Rev" )
	};
	// shift index after  U. shadha for system of 27 nakshatras
	const int index = (( nsys == N27 ) && ( nak > 20 )) ? nak+1 : nak;

	if ( format == TSHORT ) return k_nakshatra_name_short[index];
	if ( format == TMEDIUM ) return k_nakshatra_name_medium[index];
	else return k_nakshatra_name_large[index];
}

/**************************************************************
***
** Lang   ---   getObjectName
***
***************************************************************/
const wxString Lang::getObjectName( const int &num, const int &format, const bool vedic ) const
{
	assert( num >= 0 );
	wxString ret;
	const static wxChar *k_object_large[MAX_EPHEM_OBJECTS] =
	{
		_("Sun "), _("Moon"), _("Mercury"), _( "Venus" ), _( "Mars" ), _("Jupiter" ), _("Saturn"),
		_("Uranus"), _("Neptune"), _("Pluto"),
		_("Dragon Head"), _("Dragon Head"), _( "Dragon Tail" ), _( "Dragon Tail" ),
		_( "Ascendant" ), _("Meridian"),
		_("Cupido"), _("Hades"), _("Zeus"), _("Kronos"), _("Apollon"), _("Admetos"), _("Vulkanus"), _("Poseidon"),
		_( "Chiron" ), _( "Pholus" ), _( "Ceres" ), _( "Pallas" ), _( "Juno" ), _( "Vesta" ), _( "Lilith" )
	};
	const static wxChar *k_object_large_vedic[8] =
	{
		_("Surya"), _("Chandra"), _("Budha"), _("Shukra"), _("Kuja"), _("Guru"), _("Shani"), _("Lagna")
	};
	const static wxChar *k_special_object[MAX_EXTENDED_OBJECTS] =
	{
		_("Aries"),
		_("Dhuma"), _("Vyatipata"), _("Parivesha"), _("Chapa"), _("Upaketu"),
		_("Kala"), _("Mrityu"), _("Ardhaprahara"), _("Yamaghantaka"), _("Gulika"), _( "Mandi" ),
		_("Bhava Lagna"), _("Hora Lagna"), _("Ghatika Lagna"), _( "D9Lagna" )
	};

	if ( num < MAX_EPHEM_OBJECTS )
	{
		ret = k_object_large[num];
		if ( vedic || cfg->useVedicSignNames )
		{
			if ( num == OTRUENODE || num == OMEANNODE ) ret = _( "Rahu" );
			if ( num == OTRUEDESCNODE || num == OMEANDESCNODE ) ret = _( "Ketu" );
		}
		if ( cfg->useVedicPlanetNames  )
		{
			if ( num <= OSATURN ) ret = k_object_large_vedic[num];
			else if ( num == OASCENDANT ) ret = k_object_large_vedic[7];
		}
	}
	else if ( num >= OARIES && num <= OARIES + MAX_EXTENDED_OBJECTS )
	{
		ret = k_special_object[num - OARIES];
	}
	else assert( false );

	if ( format == TLARGE ) {
		return ret;
	}
	else if ( format == TMEDIUM )
	{
		if ( num == OMERIDIAN ) return _( "MC" );
		else if ( ! vedic )
		{
			if ( num == OTRUENODE || num == OMEANNODE ) ret = _( "NNo" );
			if ( num == OTRUEDESCNODE || num == OMEANDESCNODE ) ret = _( "SNo" );
		}
		return ret.Left( 3 );
	}
	else {
		return ret.Left( 2 );
	}
}

/**************************************************************
***
**    Lang   ---   getYoniName
***
***************************************************************/
const wxString Lang::getYoniName( const int &index ) const
{
	assert( index >= 0 && index < 14 );
	const wxChar *k_yoni_name_sk[14] = { wxT( "Ashwa"), wxT( "Mahish" ), wxT( "Simha" ), wxT( "Gaja" ),
		wxT( "Mesha" ), wxT( "Vanar" ), wxT( "Nakul" ), wxT( "Sarpa" ), wxT( "Mriga" ), wxT( "Swan" ),
		wxT( "Marjar" ), wxT( "Mushak" ), wxT( "Vyaghra" ), wxT( "Gow" ) };
	const wxChar *k_yoni_name[14] = { _( "Horse" ), _( "Buffalo" ), _( "Lion" ), _( "Elephant" ),
		_( "Goat" ), _( "Monkey" ), _( "Mongoose" ), _( "Serpent" ), _( "Deer" ), _( "Dog" ), _( "Cat" ),
		_( "Rat" ), _( "Tiger" ), _( "Cow"  ) };
	return cfg->useVedicPlanetNames ? k_yoni_name_sk[index] : k_yoni_name[index];
}

/**************************************************************
***
** Lang   ---   getGanaName
***
***************************************************************/
const wxString Lang::getGanaName( const int &index ) const
{
	assert( index >= 0 && index < 3 );
	const wxChar *k_gana_name[3] = { _( "Deva" ), _( "Manuj" ), _( "Rakshas" ) };
	return k_gana_name[index];
}

/**************************************************************
***
** Lang   ---   getTaraName
***
***************************************************************/
const wxString Lang::getTaraName( const int &index ) const
{
	assert( index >= -1 && index < 9 );

	if ( index == -1 ) return _( "none" );

	const wxChar *k_tara_name[9] = { _( "Janma" ), _( "Sampat" ), _( "Vipat" ), _( "Kshema" ),
		_( "Pratyak" ), _( "Sadhaka" ), _( "Vadha" ), _( "Mitra" ), _( "Parma Mitra" ) };

	return k_tara_name[index];
}

/**************************************************************
***
** Lang   ---   getNadiName
***
***************************************************************/
const wxString Lang::getNadiName( const int &index ) const
{
	assert( index >= 0 && index < 3 );
	const wxChar *k_nadi_name[3] = { _( "Adi (Vata)" ), _( "Madhya (Pitta)" ), _( "Antya (Kapha)" ) };
	return k_nadi_name[index];
}

/**************************************************************
***
** Lang   ---   getVarnaName
***
***************************************************************/
const wxString Lang::getVarnaName( const int &index ) const
{
	assert( index >= 0 && index < 4 );
	const wxChar *k_varna_name[4] = { _( "Shudra" ), _("Vaishya" ), _( "Kshattriya" ), _( "Brahmin" ) };
	return k_varna_name[index];
}

/**************************************************************
***
** Lang   ---   getCircleName
***
***************************************************************/
const wxString Lang::getCircleName( const int &i ) const
{
	assert( i >= 0 && i < 7 );
	wxString gradkreis_name[7] = { wxT( "360" ), wxT( "180" ), wxT( "90" ), wxT( "45" ), wxT( "22.5" ), wxT( "11.25" ), wxT( "5.625" ) };
	return gradkreis_name[i];
}

/**************************************************************
***
** Lang   ---   getDignityName
***
***************************************************************/
const wxString Lang::getDignityName( const int &index, const int& format ) const
{
	assert( index >= DIGNITY_EXALTED && index <= DIGNITY_ADHISATRU );

	const static wxChar *k_dignity_name_sk[9] = { _( "Uchcha" ), _( "Nicha" ), _( "Moolatrikona" ),
		_( "Svakshetra" ), _( "Adhimitra" ), _( "Mitra" ), _( "Sama" ), _( "Satru" ), _( "Adhisatru" ) }; 
	const static wxChar *k_dignity_name[9] = { _( "Exalted" ), _( "Debilated" ), _( "Moolatrikona" ),
		_( "Own" ), _( "Great friend" ), _( "Friend" ), _( "Neutral" ), _( "Enemy" ), _( "Great enemy" ) }; 
	const static wxChar *k_dignity_short[9] = { _( "Exal" ), _( "Deb" ), _( "Mool" ), _( "Own" ),
		_( "Gr.Fr" ), _( "Frnd" ), _( "Neutr" ), _( "Enem" ), _( "Gr.En" ) };

	if ( format == TSHORT )	return k_dignity_short[index];
	else return cfg->useVedicPlanetNames ? k_dignity_name_sk[index] : k_dignity_name[index];
}

/**************************************************************
***
**   Lang   ---   getVimsopakaDignityName
***
***************************************************************/
const wxString Lang::getVimsopakaDignityName( const int &index, const int &dignity ) const
{
	assert( index >= 0 && index < 4 && dignity >= 0 );

	const wxChar *shadvarga_dignity[5] = { _( "Kimsuka" ), _( "Vyanjana" ), _( "Chamara" ), _( "Chatra" ), _( "Kundala" ) };
	const wxChar *saptavarga_dignity[6] = { _( "Kimsuka" ), _( "Vyanjana" ), _( "Chamara" ), _( "Chatra" ),
		_( "Kundala" ), _( "Mukuta" ) };
	const wxChar *dasavarga_dignity[9] = { _( "Parijata" ), _( "Uttama" ),	_( "Gopura" ), _( "Simhasana" ),
		_( "Paravata" ), _( "Devaloka" ), _( "Brahmaloka" ), _( "Sakravahana" ), _( "Sridhama"  ) };
	const wxChar *shodasavarga_dignity[15] = { _( "Bhedaka" ), _( "Kusumia"), _( "Nagapushpa" ), _( "Kanduka" ),
		_( "Kerala" ), _( "Kalpa Vriksha" ), _( "Chandana Vana" ), _( "PoornaChandra" ), _( "Uchchaisrava" ),
		_( "Dhanvantari" ), _( "Suryakanta" ), _( "Vidruma" ), _( "Chakra Simhasana" ), _( "Goloki" ), _( "Sri Vallabha" ) };

	if ( dignity < 2 ) return wxEmptyString;

	switch ( index )
	{
	case 0:
		assert( dignity <= 6 );
		return shadvarga_dignity[dignity-2];
		break;
	case 1:
		assert( dignity <= 7 );
		return saptavarga_dignity[dignity-2];
		break;
	case 2:
		assert( dignity <= 10 );
		return dasavarga_dignity[dignity-2];
		break;
	case 3:
		assert( dignity <= 16 );
		return shodasavarga_dignity[dignity-2];
		break;
	default:
		assert( false );
		break;
	}
	return wxEmptyString;
}

/**************************************************************
***
** Lang   ---   getTithiName
***
***************************************************************/
const wxString Lang::getTithiName( const int &index ) const
{
	assert( index >= 0 && index < 30 );
	const wxChar *k_tithi_name[30] = {
		_( "Sukla - Pratipad" ),
		_( "Sukla - Dviteeya" ),
		_( "Sukla - Triteeya" ),
		_( "Sukla - Chaturthi" ),
		_( "Sukla - Panchami" ),
		_( "Sukla - Shasti" ),
		_( "Sukla - Saptami" ),
		_( "Sukla - Asthami" ),
		_( "Sukla - Navami" ),
		_( "Sukla - Dasami" ),
		_( "Sukla - Ekadasi" ),
		_( "Sukla - Dvadasi" ),
		_( "Sukla - Trayodasi" ),
		_( "Sukla - Chaturdasi" ),
		_( "Sukla - Purnima" ),
		_( "Krishna - Pratipad" ),
		_( "Krishna - Dviteeya" ),
		_( "Krishna - Triteeya" ),
		_( "Krishna - Chaturthi" ),
		_( "Krishna - Panchami" ),
		_( "Krishna - Shasti" ),
		_( "Krishna - Saptami" ),
		_( "Krishna - Asthami" ),
		_( "Krishna - Navami" ),
		_( "Krishna - Dasami" ),
		_( "Krishna - Ekadasi" ),
		_( "Krishna - Dvadasi" ),
		_( "Krishna - Trayodasi" ),
		_( "Krishna - Chaturdasi" ),
		_( "Krishna - Amavasya" ),
	};
	return k_tithi_name[index];
}

/**************************************************************
***
** Lang   ---   getWeekdayName
***
***************************************************************/
const wxString Lang::getWeekdayName( const int &day ) const
{
	assert(( day >= 0 ) || ( day < 7 ));
	static const wxChar *wd[7] = { _("Sunday"), _("Monday"), _("Tuesday"), _("Wednesday"),
		_("Thursday"), _("Friday"), _("Saturday") };
	return wd[day];
}

/**************************************************************
***
** Lang   ---   getPlanetSymbolCode
***
***************************************************************/
const wxString Lang::getPlanetSymbolCode( const int &i ) const
{
	assert( i >= 0 );
	if ( i == OARIES ) return wxT( "A" );
	if ( i >= MAX_EPHEM_OBJECTS ) return wxEmptyString;

	static const wxChar *k_symbol[MAX_EPHEM_OBJECTS] =
	{
		wxT( "a" ), wxT( "b" ), wxT( "c" ), wxT( "d" ), wxT( "e" ), wxT( "f" ), wxT( "g" ), // Sun ... Saturn
		wxT( "h" ), wxT( "i" ), wxT( "j" ), // Uranus ... Pluto
		wxT( "k" ), wxT( "k" ), // Dragon head
		wxT( "l" ), wxT( "l" ), // Dragon tail
		wxT( "u" ), wxT( "v" ), // As, MC
		wxT( "m" ), wxT( "n" ), wxT( "o" ), wxT( "p" ), wxT( "q" ), wxT( "r" ), wxT( "s" ), wxT( "t" ), // 8 Uranian
		wxT( "w" ), wxT( "x" ), // Chiron, Pholus
		wxT( "y" ), wxT( "z" ), wxT( "{" ), wxT( "|" ), // Planetoids
		wxT( "}" ) // Lilith
	};

	switch ( i )
	{
	case OURANUS:
		return cfg->uranusSymbol ? wxT( "[" ) :   k_symbol[i];
		break;
	case OPLUTO:
		return cfg->plutoSymbol ? wxT( "\\" ) :   k_symbol[i];
		break;
	}
	return k_symbol[i];
}

/**************************************************************
***
** Lang   ---   getSignSymbolCode
***
***************************************************************/
const wxChar Lang::getSignSymbolCode( const int &i, const int type ) const
{
	switch ( type )
	{
	case 0:
		if ( i == CAPRICORN && cfg->capricornSymbol ) return 'Z';
		else return (wxChar)( i + 'A' );
		break;
	/*
	case 1:
		return (wxChar)( i + '!' );
		break;
	*/
	default:
		return (wxChar)( i + 'A' );
		break;
	}
}

/**************************************************************
***
** Lang   ---   getSignName
***
***************************************************************/
const wxString Lang::getSignName( const int &i, const int format ) const
{
	assert(( i >= 0 ) || ( i < 12 ));
	wxString s;

	const wxChar *k_sign[12]={ _( "Ar" ), _( "Ta" ), _( "Ge" ), _( "Cn" ), _( "Le" ), _( "Vi" ),
		_( "Li" ), _( "Sc" ), _( "Sa" ), _( "Cp" ), _( "Aq" ), _( "Pi" ) };

	const wxChar *k_sign_full[12]={ _( "Aries" ), _( "Taurus" ), _( "Gemini" ), _( "Cancer" ),
		_( "Leo" ), _( "Virgo" ), _( "Libra" ), _( "Scorpio" ), _( "Sagittarius" ), _( "Capricorn" ),
		_( "Aquarius" ), _( "Pisces" ) };

	const wxChar *k_sign_full_vedic[12]={ wxT( "Mesha" ), wxT( "Vrishabha" ), wxT( "Mithuna" ),
		wxT( "Kataka" ), wxT( "Simha" ), wxT( "Kanya" ), wxT( "Thula" ), wxT( "Vrischika" ),
		wxT( "Dhanus" ), wxT( "Makara" ), wxT( "Kumbha" ), wxT( "Meena" ) };

	const wxChar *k_sign_medium_vedic[12]={ wxT( "Mes" ), wxT( "Vrs" ), wxT( "Mit" ),
		wxT( "Kat" ), wxT( "Sim" ), wxT( "Kan" ), wxT( "Thu" ), wxT( "Vrc" ), wxT( "Dha" ),
		wxT( "Mak" ), wxT( "Kum" ), wxT( "Mee" ) };

	const wxChar *k_sign_vedic[12]={ wxT( "Ms" ), wxT( "Vs" ), wxT( "Mi" ), wxT( "Kt" ),
	wxT( "Si" ), wxT( "Kn" ), wxT( "Th" ), wxT( "Vc" ), wxT( "Dh" ), wxT( "Mk" ), wxT( "Ku" ), wxT( "Mn" ) };

	if ( format == TLARGE )
	{
		if ( cfg->useVedicSignNames ) s = k_sign_full_vedic[i];
		else s = k_sign_full[i];
	}
	else if ( format == TMEDIUM )
	{
		if ( cfg->useVedicSignNames ) s = k_sign_medium_vedic[i];
		else s = k_sign_full[i] ;
		s = s.Left( 3 );
	}
	else {
		if ( cfg->useVedicSignNames ) s = k_sign_vedic[i];
		else s = k_sign[i];
	}
	return s;
}

/**************************************************************
***
** Lang   ---   getKarakaName
***
***************************************************************/
const wxString Lang::getKarakaName( const int &i, const int format ) const
{
	const wxChar *chara_karaka_name_long[8] = { _( "Atma" ), _( "Amatya" ), _( "Bhratru" ),
	        _( "Matru" ), _( "Pitru" ), _( "Putra" ), _( "Gnati" ), _( "Dhanya"  )
	                                          };
	assert( i >= 0 && i < 8 );
	return chara_karaka_name_long[i];
}

/**************************************************************
***
**    Lang   ---   getShastiamsaName
***
***************************************************************/
const wxString Lang::getShastiamsaName( const int &i ) const
{
	const wxChar *k_shastiamsa_name[60] = {
		_( "Ghora" ), _( "Rakshasa" ), _( "Deva" ), _( "Kubera" ), _( "Yaksha" ),
		_( "Kinnara" ), _( "Bhrashta" ), _( "Kulaghna" ), _( "Garala" ), _( "Vahni" ),
		_( "Maya" ), _( "Purishaka" ), _( "Apampathi" ), _( "Marut" ), _( "Kaala" ),
		_( "Sarpa" ), _( "Amrita" ), _( "Indu" ), _( "Mridu" ), _( "Komala" ),
		_( "Heramba" ), _( "Brahma" ), _( "Vishnu" ), _( "Maheswara" ), _( "Deva" ),
		_( "Ardra" ), _( "Kalinasa" ), _( "Kshiteesa" ), _( "Kamalakara" ), _( "Gulika" ),
		_( "Mrithyu" ), _( "Kaala" ), _( "Davagani" ), _( "Ghora" ), _( "Yama" ),
		_( "Kantaka" ), _( "Sudha" ), _( "Amrita" ), _( "Poornachandra" ), _( "Vishadagdha" ),
		_( "Kulanasa" ), _( "Vamsakshaya" ), _( "Utpata" ), _( "Kaala" ), _( "Saumya" ),
		_( "Komala" ), _( "Seetala" ), _( "Karaladamshtra" ), _( "Chandramukhi" ), _( "Praveena" ),
		_( "Kaala Pavaka" ), _( "Dandayudha" ), _( "Nirmala" ), _( "Saumya" ), _( "Kroora" ),
		_( "Atiseetala" ), _( "Amrita" ), _( "Payodhi" ), _( "Bhramana" ), _( "Chandra Rekha" )
	};
	assert( i >= 0 && i < 60 );
	return k_shastiamsa_name[i];
}

/**************************************************************
***
**    Lang   ---   getBhavaName
***
***************************************************************/
const wxString Lang::getBhavaName( const int &i ) const
{
	const wxChar *k_bhava_name[12] = { _( "Lagna" ), _( "Dhana" ), _( "Sahaja" ), _( "Bandhu" ), _( "Putra" ),
		_( "Satru" ), _( "Yuvati" ), _( "Randhra" ), _( "Dharma" ), _( "Karma" ), _( "Labha" ), _( "Vyaya" ) }; 
	assert( i >= 0 && i < 12 );
	return k_bhava_name[i];
}

/**************************************************************
***
**    Lang   ---   getMonthName
***
***************************************************************/
const wxString Lang::getMonthName( const int &i ) const
{
	assert( i >= 0 && i < 12 );
	const wxChar *month_name[12] = { _( "January" ), _( "February" ), _( "March" ), _( "April" ), _( "May" ),
		_( "June" ), _( "July" ), _( "August" ), _( "September" ), _( "October" ), _( "November" ), _( "December" ) };
	return month_name[i];
}

/*****************************************************
**
**   Lang   ---   getAyanamsaName
**
******************************************************/
const wxString Lang::getAyanamsaName( const int &ayatype )
{
	wxString s;
	switch ( ayatype )
	{
	case 0:
		s= _( "None" );
		break;
	case 1:
		s = _( "Lahiri" );
		break;
	case 2:
		s = _( "Raman" );
		break;
	case 3:
		s = _( "Krishnamurti" );
		break;
	case 4:
		s.Printf( wxT( "Custom (t0 %8.1f ayan_t0 %2.8f)" ), cfg->custom_t0, cfg->custom_ayan_t0 );
		break;
	default:
		s= wxT( "Unknown" );
		break;
	}
	return s;
}

/*****************************************************
**
**   Formatter   ---   getDegMinSecInts2
**
******************************************************/
void Formatter::getDegMinSecInts2( const double &len, int &deg, int &min, double &sec )
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
**   Formatter   ---   getDegMinSecInts
**
******************************************************/
void Formatter::getDegMinSecInts( const double &len, int &deg, int &min, int &sec )
{
	double mylen = fabs( len );
	deg = (int)mylen;
	double mind = (double)((mylen - (double)deg ) * 60);
	min = (int)mind;
	sec = (int)(( mind - (double)min ) * 60 );
	if ( len < 0 ) deg *= -1;
}

/*****************************************************
**
**   Formatter   ---   getDegMinSecFormatted
**
******************************************************/
const wxString Formatter::getDegMinSecFormatted( double t, const int &format, const double &minvalue, const double &maxvalue )
{
	double sec = 0;
	int deg = 0;
	int min = 0;

	if ( t < minvalue || t > maxvalue )
	{
		printf( "Formatter::getDegMinSecFormatted WARN invalid value %f, minvalue was %f maxvalue %f\n", t, minvalue, maxvalue );
		t = a_red( t, maxvalue );
	}

	getDegMinSecInts2( t, deg, min, sec );

	if ( format == DEG_PRECISION_MORE )
	{
		return wxString::Format( wxT( "%02d:%02d:%02d.%02d" ), deg, min, (int)sec, (int)(.5 + 100 * ( sec - (int)sec )));
	}
	else
	{
		sec = a_rund( sec );
		if ( sec == 60 )
		{
			sec = 0;
			min++;
			if ( min == 60 )
			{
				min = 0;
				deg++;
			}
		}
	}
	if ( format == DEG_PRECISION_SECOND || ( DEG_PRECISION_FLEXIBLE && sec != 0 ))
	{
		return wxString::Format( wxT( "%02d:%02d:%02d" ), deg, min, (int)sec );
	}
	else
	{
		if ( sec >= 30 ) min++;
		if ( min == 60 )
		{
			min = 0;
			deg++;
		}
		return wxString::Format( wxT( "%02d:%02d" ), deg, min );
	}
}

/*****************************************************
**
**   Formatter   ---   getLenFormatted
**
******************************************************/
const wxString Formatter::getLenFormatted( const double &len, const int format )
{
	wxString s;
	if ( len < 0 ) s = wxT( "-" );
	s <<  getDegMinSecFormatted( fabs( len ), format, 0.0, 360.0 );
	return s;
}

/*****************************************************
**
**   Formatter   ---   getLatitudeFormatted
**
******************************************************/
const wxString Formatter::getLatitudeFormatted( const double &l, const int format )
{
	wxString s;
	s <<  getDegMinSecFormatted( fabs( l ), format, 0.0, 90.0 );

	if ( l > 0 ) s << wxT( "N" );
	else if ( l < 0 ) s << wxT( "S" );
	return s;
}

/*****************************************************
**
**   Formatter   ---   getLongitudeFormatted
**
******************************************************/
const wxString Formatter::getLongitudeFormatted( const double &l, const int format )
{
	wxString s;
	s <<  getDegMinSecFormatted( fabs( l ), format, 0.0, 180.0 );
	if ( l > 0 ) s << wxT( "E" );
	else if ( l < 0 ) s << wxT( "W" );
	return s;
}

#define PARSE_FORMAT_STRICT

/*****************************************************
**
**   Formatter   ---   parseDegMinSecAndFormat
**
******************************************************/
wxString Formatter::parseDegMinSecAndFormat( wxString s, const int format )
{
	int deg, min, sec;

	bool b = getDegreeIntsFromString( (const wxChar*)s, deg, min, sec );
	if ( ! b )
	{
		printf( "Formatter::parseDegreesAndFormat WARN: invalid string %s\n", str2char( s ));
		return s;
	}
	double value = sec + min * 60 + deg * 3600;
	value /= 3600.0;
	wxString ret = getDegreesFormatted( value, format );

#ifdef PARSE_FORMAT_STRICT
	int deg2, min2, sec2;

	bool b2 = getDegreeIntsFromString( (const wxChar*)ret, deg2, min2, sec2 );
	if ( ! b2 )
	{
		printf( "ERROR Formatter::parseDegMinSecAndFormat: output parsing failed for string %s\n", str2char( ret )); 
	}
	if ( deg != deg2 || min != min2 || sec != sec2 )
	{
		printf( "ERROR Formatter::parseDegMinSecAndFormat: wrong values (rounding errors?): old %d %d %d, new %d %d %d,\n", deg, min, sec, deg2, min2, sec2 );
	}
#endif

	return ret;
}

/*****************************************************
**
**   Formatter   ---   getLenString
**
******************************************************/
// unused
const wxString Formatter::getLenString( const double &len, const int format, const int dir )
{
	wxString s;
	Lang lang;

	s << ( dir == 1 ? 'R' : 'D' );

	double mylen = red_deg( len );
	const int sign = (int)( mylen / 30 );
	mylen -= 30 * sign;

	s << getDegMinSecFormatted( mylen, format, 0.0, 30.0 ) << lang.getSignName(sign);
	return s;
}

/*****************************************************
**
**   Formatter   ---   getDateIntsFromJD
**
******************************************************/
void Formatter::getDateIntsFromJD( const double &jd, int &day, int &month, int &year, const int format )
{
	double ut = 0;
	CalculatorFactory().getCalculator()->getDateIntsFromJD( jd, year, month, day, ut );
}

/*****************************************************
**
**   Formatter   ---   getFullDateStringFromJD
**
******************************************************/
const wxString Formatter::getFullDateStringFromJD( const double &jd, const int format )
{
	wxString s;
	int year, month, day;

	getDateIntsFromJD( jd, day, month, year );
	int l = LanguageConfig::get()->getLocale()->GetLanguage();
	if ( l == wxLANGUAGE_GERMAN ) // German
		s.Printf( wxT( "%02d.%02d.%04d %s" ), day, month, year, (const wxChar*)getTimeFormatted( getTimeFromJD(jd)) );
	else
		s.Printf( wxT( "%04d-%02d-%02d %s" ), year, month, day, (const wxChar*)getTimeFormatted( getTimeFromJD(jd)) );
	return s;
}

/*****************************************************
**
**   Formatter   ---   getDateStringFromJD
**
******************************************************/
const wxString Formatter::getDateStringFromJD( const double &jd, const int format )
{
	wxString s;
	int year, month, day;
	getDateIntsFromJD( jd, day, month, year );
	int l = LanguageConfig::get()->getLocale()->GetLanguage();
	if ( l == wxLANGUAGE_GERMAN ) // German
		s.Printf( wxT( "%02d.%02d.%04d" ), day, month, year );
	else
		s.Printf( wxT( "%04d-%02d-%02d" ), year, month, day );
	return s;
}


/*****************************************************
**
**   Formatter   ---   getDateIntsFromString
**
******************************************************/
bool Formatter::getDateIntsFromString( const wxChar *value, int &day, int &month, int &year, int format )
{
	const int l = LanguageConfig::get()->getLocale()->GetLanguage();
	if ( ! value )
	{
		wxLogError( wxT( "Error: date value is NULL" ));
		return false;
	}
	wxChar *c = (wxChar*)value;
	int sepTokenCount = 0;
	const wxChar septoken = ( l == wxLANGUAGE_GERMAN ? '.' : '-' );

	while ( *c )
	{
		if ( *c == septoken ) sepTokenCount++;
		if ( ! ( isdigit( *c ) || *c == '.' || *c == '-' )) return false;
		c++;
	}
	if ( sepTokenCount > 2 ) return false;

	int ret;
	if ( l == wxLANGUAGE_GERMAN ) // German
		ret = wxSscanf( value, wxT( "%d.%d.%d" ), &day, &month, &year );
	else
		ret = wxSscanf( value, wxT( "%d-%d-%d" ), &year, &month, &day );
	if ( ret != 3 ) return false;

	if (( day < 1 ) || ( day > 31 )) return false;
	if (( month < 1 ) || ( month > 12 )) return false;
	if (( year < -3000 ) || ( year > 6000 )) return false;

	return true;
}

/*****************************************************
**
**   Formatter   ---   getDegreeIntsFromString
**
******************************************************/
bool Formatter::getDegreeIntsFromString( const wxChar *value, int &deg, int &minute, int &second, int format )
{
	if ( ! value )
	{
		wxLogError( wxT( "Error: degree value is NULL" ));
		return false;
	}
	wxChar *c = (wxChar*)value;
	int sepTokenCount = 0;
	// TODO
	while ( *c )
	{
		if ( *c == ':' ) sepTokenCount++;
		if ( ! ( isdigit( *c ) || *c == ':' )) return false;
		c++;
	}
	if ( sepTokenCount > 2 ) return false;

	deg = 0;
	minute = 0;
	second = 0;

	int ret = wxSscanf( value, wxT( "%d:%d:%d" ), &deg, &minute, &second );
	if ( ret < 1 ) return false;

	if (( deg < -180 ) || ( deg > 180 )) return false;
	if (( minute < 0 ) || ( minute > 59 )) return false;
	if (( second < 0 ) || ( second > 59 )) return false;

	return true;
}

/*****************************************************
**
**   Formatter   ---   getIntOrEmtpy
**
******************************************************/
wxString Formatter::getIntOrEmtpy( const int &a )
{
	if ( a ) return  wxString::Format( wxT( "%d" ), a );
	else return wxEmptyString;
}





