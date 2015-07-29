/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/base/constants.h
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
#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

#ifdef __GNUG__
#pragma interface "constants.h"
#endif

#include <stdio.h>

#include <wx/hashmap.h>
#include <wx/intl.h>

/**************************************************************
***
**   character stuff
***
***************************************************************/

// Line feed
#ifdef __WXMSW__
#define Endl wxT( "\r\n" )
#else
#define Endl wxT( "\n" )
#endif

#define FILE_CONF_MODE wxConvUTF8

//#define str2char( s ) (const char*) wxConvCurrent->cWX2MB(s.c_str())
#define str2char( s ) (const char*) wxConvCurrent->cWX2MB(s)

// console output for wxString
#define Cout( s ) printf( "%s", str2char( s ));
#define PrintLn( s ) printf( "%s\n", str2char( s ));

#if ( wxCHECK_VERSION( 2, 9, 0 ))
#define _WX29
#endif

#ifdef __WXMAC__
#define MAC_PDF_START_COMMAND wxT( "open -a Preview" )
#define MAC_RESOURCE_DIRECTORY wxT( "maitreya7.app/Contents/Resources" )
#endif

#define AllTrim( s ) s.Trim( true ).Trim( false )

WX_DECLARE_STRING_HASH_MAP( int, StringIntHash );
WX_DECLARE_STRING_HASH_MAP( wxString, StringHashMap );
WX_DECLARE_HASH_MAP( int, wxString, wxIntegerHash, wxIntegerEqual, IntStringHash );

/**************************************************************
***
**  VARIOUS CONSTANTS
***
***************************************************************/
#define CALC_NO_UT 0
#define CALC_UT 1

#define YEAR_LEN 365.25;
#define ICUSTOM_YEAR_LEN 365.25f;
#define WCUSTOM_YEAR_LEN 362.83f;

#define PI 3.14159265358979323846
#define SQRT2 1.414213562

// Constants for transformations deg <=> rad
#define DEG2RAD 0.0174532925199433
#define RAD2DEG 57.2957795130823

#define NB_AYANAMSAS 3
#define DEFAULT_T0 2415020.0
#define DEFAULT_AYAN_T0 22.46047
#define DEFAULT_AYA_PERIOD 25771.4021

// Min and Max macros
#ifndef Min
#define Min( a, b ) ( a < b ? a : b )
#endif
#ifndef Max
#define Max( a, b ) ( a > b ? a : b )
#endif

#define APP_NAME wxT( "maitreya6" )
#define CONFIG_DIR_NAME wxT( "maitreya-7.0" )
#define VENDOR_NAME wxT( "Saravali" )
#define GUI_APP_NAME wxT( "Maitreya" )

// print custom footer in PDF
#define DEFAULT_CUSTOM_FOOTER wxT( "Printed by Maitreya $version on $date" )
//#define SYMBOL_FONT_NAME wxT( "MaitreyaSymbols6" )
#define LOGO_FILE wxT( "logo.jpg" )

// Various file and directory names
#define LISENSE_FILE_NAME wxT( "COPYING" )
#define WIN_LISENSE_FILE_NAME wxT( "Copying.txt" )
#define YOGA_DIR_NAME wxT( "yogas" )
//#define YOGA_TARGET_DIR_NAME wxT( "yogas_v0.1" )
#define YOGA_TARGET_DIR_NAME wxT( "yogas" )
#define PIC_DIR_NAME wxT( "pics" )
#define XML_DIR_NAME wxT( "xml" )
#define FONT_DIR_NAME wxT( "fonts" )
#define ATLAS_DIR_NAME wxT( "atlas" )

// Atlas
#define ATLAS_MAX_GRID_ELEMENTS 50
#define ATLAS_DEFAULT_DBFILENAME  wxT( "geonames.db" )
#define ATLAS_DEFAULT_SQLFILENAME wxT( "geonames1000.sql" )

// AboutDialog
#define AUTHOR wxT( "Martin Pettau" )
#define SARAVALI_URL wxT( "http://www.saravali.de" )
#define SARAVALI_HELP_URL wxT( "http://www.saravali.de/documentation.html" )

enum { WRITER_TEXT, WRITER_HTML, WRITER_CSV, WRITER_PDF };

#define MAX_ASPECT_TYPES 19
#define ASSERT_VALID_ASPECT( p ) assert( p >= 0 && p < MAX_ASPECT_TYPES );
#define MAX_DASA_COLORS 22 // 12 + 10
#define ASSERT_VALID_DASA_COLOR( p ) assert( p >= 0 && p < MAX_DASA_COLORS );

/**************************************************************
***
**  PLANETARY OBJECTS -- very sensitive constants !
***
***************************************************************/
enum { OSUN = 0, OMOON, OMERCURY, OVENUS, OMARS, OJUPITER, OSATURN, // 0 .. 6
       OURANUS, ONEPTUNE, OPLUTO, // 7 ... 9

       // Nodes
       OMEANNODE, OTRUENODE, OMEANDESCNODE, OTRUEDESCNODE, // 10 ... 13

       // AS-MC
       OASCENDANT, OMERIDIAN, // 14, 15

       // Uranian
       OCUPIDO, OHADES, OZEUS, OKRONOS, OAPOLLON, OADMETOS, OVULKANUS, OPOSEIDON, // 16 ... 23

       // Planetoids and Lilith
       OCHIRON, OPHOLUS, OCERES, OPALLAS, OJUNO, OVESTA, OLILITH // 24 ... 30
     };

#define FIRST_EPHEM_OBJECT OSUN
#define MAX_EPHEM_OBJECTS 31
#define LAST_EPHEM_OBJECT FIRST_EPHEM_OBJECT + MAX_EPHEM_OBJECTS - 1

enum {
	OARIES = 200,
	ODHUMA, OVYATIPATA, OPARIVESHA, OCHAPA, OUPAKETU, // 201 .. 205
	OKALA, OMRITYU, OARDHAPRAHARA, OYAMAGHANTAKA, OGULIKA, OMANDI,  // 206 .. 211
	OBHAVA_LAGNA, OHORA_LAGNA, OGHATIKA_LAGNA,  // 212 .. 214
	OD9LAGNA
};

#define FIRST_EXTENDED_OBJECT OARIES
#define MAX_EXTENDED_OBJECTS 16
#define LAST_EXTENDED_OBJECT FIRST_EXTENDED_OBJECT + MAX_EXTENDED_OBJECTS - 1

#define OFORTUNE 300

#define FIRST_ARABIC_OBJECT OFORTUNE
#define MAX_ARABIC_OBJECTS 177
#define LAST_ARABIC_OBJECT FIRST_ARABIC_OBJECT + MAX_ARABIC_OBJECTS - 1

#define ASSERT_VALID_EPHEM_OBJECT( p ) assert( p >= FIRST_EPHEM_OBJECT && p <= LAST_EPHEM_OBJECT );
#define ASSERT_VALID_EXTENDED_OBJECT( p ) assert( p >= FIRST_EXTENDED_OBJECT && p <= LAST_EXTENDED_OBJECT );
#define ASSERT_VALID_ARABIC_OBJECT( p ) assert( p >= FIRST_ARABIC_OBJECT && p <= LAST_ARABIC_OBJECT );

#define ASSERT_VALID_OBJECT( p ) \
  assert(( p >= FIRST_EPHEM_OBJECT && p <= LAST_EPHEM_OBJECT ) \
	|| ( p >= FIRST_EXTENDED_OBJECT && p <= LAST_EXTENDED_OBJECT ) \
	|| ( p >= FIRST_ARABIC_OBJECT && p <= LAST_ARABIC_OBJECT ));

enum { OHOUSE1 = 100, OHOUSE2, OHOUSE3, OHOUSE4, OHOUSE5, OHOUSE6, OHOUSE7, OHOUSE8, OHOUSE9, OHOUSE10, OHOUSE11, OHOUSE12 };

enum { CDHUMA = 0, CVYATIPATA, CPARIVESHA, CCHAPA, CUPAKETU, CKALA, CMRITYU, CARDHAPRAHARA, CYAMAGHANTAKA, CGULIKA, CMANDI };
#define NUM_UPA 11

enum { CBHAVA_LAGNA = 0, CHORA_LAGNA, CGHATIKA_LAGNA };
#define NUM_LAGNA 3

// Ephemeris
enum { EPHEM_COORDINATES_GEOCENTRIC = 0, EPHEM_COORDINATES_TOPOCENTRIC, EPHEM_COORDINATES_EQUATORIAL,
       EPHEM_COORDINATES_BARYCENTRIC, EPHEM_COORDINATES_HELIOCENTRIC
     };

// Solar events
enum {SOLAR_EVENT_SUNRISE, SOLAR_EVENT_SUNSET, SOLAR_EVENT_MIDNIGHT, SOLAR_EVENT_NOON };

// convenience indices for houses, so we don't ge confused when counting them
enum { HOUSE1 = 0, HOUSE2, HOUSE3, HOUSE4, HOUSE5, HOUSE6, HOUSE7, HOUSE8, HOUSE9, HOUSE10, HOUSE11, HOUSE12 };

#define ASSERT_VALID_HOUSE( p ) assert( p >= HOUSE1 && p <= HOUSE12 );

#define HOUSE_SYS_NUM 13
enum { HOUSE_SYSTEM_NONE = 0, HOUSE_SYSTEM_PLACIDUS, HOUSE_SYSTEM_KOCH, HOUSE_SYSTEM_REGIOMONTANUS,
       HOUSE_SYSTEM_CAMPANUS, HOUSE_SYSTEM_PORPHYRY, HOUSE_SYSTEM_EQUAL, HOUSE_SYSTEM_VEHLOW, HOUSE_SYSTEM_AXIAL,
       HOUSE_SYSTEM_AZIMUTAL, HOUSE_SYSTEM_POLICH, HOUSE_SYSTEM_ALCABITUS, HOUSE_SYSTEM_MORINUS, HOUSE_SYSTEM_KRUSINSKI
     };

#define ASSERT_VALID_DEGREE( l ) assert( l >= 0 && l < 360 );


// Zodiacal signs,
enum { RASI_NONE = -1, ARIES, TAURUS, GEMINI, CANCER, LEO, VIRGO, LIBRA, SCORPIO, SAGITTARIUS, CAPRICORN, AQUARIUS, PISCES };

#define ASSERT_VALID_RASI( r ) assert ( r >= ARIES && r <= PISCES );
#define ASSERT_VALID_EXTENDED_RASI( r ) assert ( r >= RASI_NONE && r <= PISCES );

// Sort orders for aspects
enum { ASPECT_SORT_PLANET1 = 0, ASPECT_SORT_ORBIS, ASPECT_SORT_ORBIS_ABSOLUTE, ASPECT_SORT_PLANET2, ASPECT_SORT_TYPE, ASPECT_SORT_DATE,
       ASPECT_SORT_ORBIS_REVERSE, ASPECT_SORT_JD
     };

enum { TRANSIT_MODE_TRANSIT, TRANSIT_MODE_SOLAR_ARC, TRANSIT_MODE_DIRECTION, TRANSIT_MODE_LUNAR_ARC, TRANSIT_MODE_CONSTANT_ARC };

/**************************************************************
***
**  Constants for objects and styles
***
***************************************************************/
#define OBJECTS_INCLUDE_OUTER           0x00010000
#define OBJECTS_INCLUDE_DRAGONTAIL      0x00020000
#define OBJECTS_INCLUDE_DRAGONHEAD      0x00040000
#define OBJECTS_INCLUDE_ASCENDANT       0x00080000
#define OBJECTS_INCLUDE_MERIDIAN        0x00100000
#define OBJECTS_INCLUDE_URANIAN         0x00200000
#define OBJECTS_INCLUDE_CHIRON          0x00400000
#define OBJECTS_INCLUDE_PHOLUS          0x00800000
#define OBJECTS_INCLUDE_PLANETOIDS      0x01000000
#define OBJECTS_INCLUDE_LILITH          0x02000000

#define OBJECTS_INCLUDE_ARIES           0x04000000
#define OBJECTS_INCLUDE_KALAVELAS       0x08000000
#define OBJECTS_INCLUDE_UPAGRAHAS       0x10000000
#define OBJECTS_INCLUDE_SPECIALLAGNAS   0x20000000
#define OBJECTS_INCLUDE_D9_LAGNA        0x40000000
#define OBJECTS_INCLUDE_ARABICPARTS     0x80000000

// Graphical chart frame types
enum { CHART_FRAME_TYPE_INHERITED = 0, CHART_FRAME_TYPE_CIRCLE, CHART_FRAME_TYPE_ELLIPSE, CHART_FRAME_TYPE_SQUARE, CHART_FRAME_TYPE_RECTANGLE };

enum { VGRAPHIC_SOUTH_INDIAN = 0, VGRAPHIC_NORTH_INDIAN, VGRAPHIC_EAST_INDIAN };
enum { VGRAPHIC_NORTH_INDIAN_ASC = 0, VGRAPHIC_NORTH_INDIAN_NUMBER, VGRAPHIC_NORTH_INDIAN_SHORT, VGRAPHIC_NORTH_INDIAN_SYMBOL };
enum { VGRAPHIC_CHART_CENTER_NOTHING = 0, VGRAPHIC_CHART_CENTER_VARGA, VGRAPHIC_CHART_CENTER_DIVISION, VGRAPHIC_CHART_CENTER_BOTH };


class VedicGraphicStyle
{
public:
	VedicGraphicStyle()
	{
		showArudhas = false;
		showRetro = true;
		showPlanetColors = true;

		indianChartType = VGRAPHIC_SOUTH_INDIAN;
		centerInfoType = VGRAPHIC_CHART_CENTER_VARGA;
		northIndianSignDisplayType = VGRAPHIC_NORTH_INDIAN_NUMBER;

		showSbcSanskritSymbols = false;
		showSbcAfflictions = true;
		showSbcNakshatraQuality = true;
	}

	bool showArudhas;
	bool showRetro;
	bool showPlanetColors;

	int indianChartType;
	int northIndianSignDisplayType;
	int centerInfoType;

	bool showSbcSanskritSymbols;
	bool showSbcAfflictions;
	bool showSbcNakshatraQuality;

};

class WesternGraphicStyle
{
public:
	WesternGraphicStyle()
	{
		showHouses = true;
		showAspects = true;
		showRetro = true;
		showPlanetColors = true;
		showSignColors = true;
		showHouseColors = true;
		showAspectColors = true;
		showAspectSymbols = false;
		transitStyle = 0;
	}
	
	bool showHouses;
	bool showAspects;
	bool showRetro;
	bool showPlanetColors;
	bool showSignColors;
	bool showHouseColors;
	bool showAspectColors;
	bool showAspectSymbols;
	int transitStyle;

};

// Main window columns
#define MAIN_CHILD_SHOW_DECLINATION  0x001
#define MAIN_CHILD_SHOW_LORD         0x002
#define MAIN_CHILD_SHOW_DIGNITY      0x004
#define MAIN_CHILD_SHOW_NAVAMSA      0x010
#define MAIN_CHILD_SHOW_NAKSHATRA    0x020
#define MAIN_CHILD_SHOW_PADA         0x040
#define MAIN_CHILD_SHOW_KP           0x080
#define MAIN_CHILD_SHOW_SHASTIAMSA   0x100
#define MAIN_CHILD_SHOW_DASAVARGA    0x200
#define MAIN_CHILD_SHOW_HOUSEPOS     0x400
#define MAIN_CHILD_SHOW_KARAKA       0x800
#define MAIN_CHILD_SHOW_ASHTAKA      0x1000
#define MAIN_CHILD_SHOW_HOUSES       0x2000

// Text styles for planets, signs etc
enum { TSHORT = 0, TMEDIUM, TLARGE };

// Text Window
enum {
	TEXT_MODE_BASE = 0,
	TEXT_MODE_ARABICPARTS,
	TEXT_MODE_ASHTAKAVARGA,
	TEXT_MODE_ASTRONOMICAL,
	TEXT_MODE_BHAVA,
	TEXT_MODE_DASA,
	TEXT_MODE_DASACOMPACT,
	TEXT_MODE_DASALONG,
	TEXT_MODE_JAIMINI,
	TEXT_MODE_KP,
	TEXT_MODE_NAKSHATRA,
	TEXT_MODE_VARGA,
	TEXT_MODE_VEDIC_ASPECTARIUM,
	TEXT_MODE_VEDIC_PLANETS,
	TEXT_MODE_WESTERN_ASPECTARIUM,
	TEXT_MODE_WESTERN_PLANETS,
	TEXT_MODE_WESTERN_ASPECTARIUM_PLAIN,
	TEXT_MODE_MAIN_VIEW
};

//#define DEFAULT_SYMBOL_FONT_NAME wxT( "MaitreyaSymbols6" )

#define ADJUST_TOOLBAR_ITEM( t, b ) t->Show( b )
//#define ADJUST_TOOLBAR_ITEM( t, b ) t->Show( b )

// Notebook styles
enum { NB_STYLE_NOTEBOOK = 0, NB_STYLE_LISTBOOK, NB_STYLE_CHOICEBOOK, NB_STYLE_TOOLBOOK, NB_STYLE_TREEBOOK };

// text formatting constants
#define FORMAT_DEGREE         1
#define FORMAT_HOUR           2
#define FORMAT_SHOW_SECONDS  16
#define FORMAT_RED_RASI      32
#define FORMAT_SHOW_DIR      64

enum { DEG_PRECISION_MINUTE, DEG_PRECISION_SECOND, DEG_PRECISION_MORE, DEG_PRECISION_FLEXIBLE };
enum { DIR_NONE = -1, DIR_DIRECT, DIR_RETROGRADE, DIR_STATIONARY, DIR_DIRECT_SHOW };

/**************************************************************
***
**  VEDIC ASTROLOGY
***
***************************************************************/
// Karakas of Jaimini Astrology
enum { ATMA_KARAKA, AMATYA_KARAKA, BHRATRU_KARAKA, MATRU_KARAKA, PITRU_KARAKA, PUTRA_KARAKA, GNATI_KARAKA, DARA_KARAKA };

enum { DASA_TYPE_NONE = -1, DASA_TYPE_NAKSHATRA, DASA_TYPE_PLANET, DASA_TYPE_SIGN };
enum { LUNAR_NODE_TRUE = 0, LUNAR_NODE_MEAN };

enum { VARNA_NONE = -1, VARNA_BRAHMIN, VARNA_KSHATRIYA, VARNA_VAISHYA, VARNA_SHUDRA };
enum { GANA_NONE = -1, GANA_DEVA, GANA_MANUJ, GANA_RAKSHAS };
enum { NADI_NONE = -1, NADI_AADI, NADI_MADHYA, NADI_ANTYA };
enum { VASHYA_NONE = -1, VASHYA_QUADRUPED, VASHYA_HUMAN, VASHYA_JALACHARA, VASHYA_LEO, VASHYA_SCORPIO };

// Nakshatra stuff
// 28 Nakshatras (plus Abhijit)
enum {
	ASWINI = 0, BHARANI, KRITTIKA, ROHINI, MRIGASIRA, ARUDRA, PUNARVASU, PUSHYAMI, ASLESHA,
	MAKHA, PPHALGUNI, UPHALGUNI, HASTHA, CHITTA, SWATI, VISAKHA, ANURADHA, JYESHTA,
	MULA, PSHADHA, USHADHA, ABHIJIT, SRAVANA, DHANISHTA, SATHABISHAK, PBHADRA, UBHADRA, REVATI
};

// 27 Nakshatras (without Abhijit)
enum {
	NASWINI = 0, NBHARANI, NKRITTIKA, NROHINI, NMRIGASIRA, NARUDRA, NPUNARVASU, NPUSHYAMI, NASLESHA,
	NMAKHA, NPPHALGUNI, NUPHALGUNI, NHASTHA, NCHITTA, NSWATI, NVISAKHA, NANURADHA, NJYESHTA,
	NMULA, NPSHADHA, NUSHADHA, NSRAVANA, NDHANISHTA, NSATHABISHAK, NPBHADRA, NUBHADRA, NREVATI
};

#define ABHIJIT_INDEX 21

#define NAKSHATRA_LEN 13.33333333333
#define NAKSHATRA_LEN_R 13.33333333333 // 360 / 27
#define NAKSHATRA_LEN_N 12.85714286    // 360/28
#define N27_N28_RATIO .964285714       // 27/28
#define N28_N27_RATIO 1.037037037      // 28/27
#define PADA_LEN 3.333333333333

#define USHADHA_BEGIN_R 266.6666666667
#define ABHIJIT_BEGIN_R 276.6666666667
#define SRAVANA_BEGIN_R 280.8888888889
#define DHANISHTA_BEGIN_R 293.6666666667

#define USHADHA_LEN_R 10
#define ABHIJIT_LEN_R 4.2222222222 // = 4:13:20;
#define SRAVANA_LEN_R 12.444444444 // = 12:26:40
enum { N27 = 0, N28 };  // Normal /old fashioned / learned

// Exaltation points
//su=10 mo=33 ma=298 me=165 ju=95 ve=357 sa=200 ra=285 ke=225
const int k_exaltation[9] = { 10, 33, 165, 357, 298, 95, 200, 285, 225 };

#define NB_VARGAS 21
#define ASSERT_VALID_VARGA( v ) assert ( v >= 0 && v < NB_VARGAS );

// Friends and Enemies
enum { ENEMY = 0, NEUTRAL, FRIEND };

// Shastiamsa - 10 in each line
const bool k_shastiamsa_benefic[60] = {
	false, false, true, true, true, true, false, false, false, false,
	false, false, true, true, false, false, true, true, true, true,
	true, true, true, true, true, true, true, true, true, false,
	false, false, false, false, false, false, true, true, true, false,
	false, false, false, false, true, true, true, false, true, true,
	false, false, true, true, false, true, true, true, false, true,
};

// Vargas
enum { VARGA_RASI = 0, VARGA_NAVAMSA, VARGA_HORA, VARGA_DREKKANA, VARGA_CHATURTAMSA,
       VARGA_SASTAMSA,	VARGA_SAPTAMAMSA, VARGA_ASHTAMSA, VARGA_DASAMSA, VARGA_DVADASAMSA,
       VARGA_SHODASAMSA, VARGA_VIMSAMSA,	VARGA_SIDDHAMSA, VARGA_BHAMSA, VARGA_TRIMSAMSA,
       VARGA_CHATVARIMSAMSA, VARGA_AKSHAVEDAMSA,	VARGA_SHASTIAMSA,
       VARGA_ASHTOTTARAMSA, VARGA_DVADASDVADASAMSA, VARGA_BHAVA
     };

// Dignities
enum { DIGNITY_NONE = -1, DIGNITY_EXALTED, DIGNITY_DEBILATED, DIGNITY_MOOLATRIKONA, DIGNITY_SVAKSHETRA, DIGNITY_ADHIMITRA, DIGNITY_MITRA,
       DIGNITY_SAMA, DIGNITY_SATRU, DIGNITY_ADHISATRU
     };

// Vimsopaka Bala
const static double shadvarga_weight[6] = { 6, 2, 4, 5, 2, 1 };
const static int shadvarga_vargas[6] = { VARGA_RASI, VARGA_HORA, VARGA_DREKKANA, VARGA_NAVAMSA, VARGA_DVADASAMSA, VARGA_TRIMSAMSA };

const static double saptavarga_weight[7] = { 5, 2, 3, 2.5, 4.5, 2, 1 };
const static int saptavarga_vargas[7] = { VARGA_RASI, VARGA_HORA, VARGA_DREKKANA, VARGA_NAVAMSA, VARGA_DVADASAMSA, VARGA_TRIMSAMSA,
                                        VARGA_SAPTAMAMSA
                                        };

const static double dasavarga_weight[10] = { 3, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 5 };
const static int dasavarga_vargas[10] = { VARGA_RASI, VARGA_HORA, VARGA_DREKKANA, VARGA_NAVAMSA, VARGA_DVADASAMSA, VARGA_TRIMSAMSA,
                                        VARGA_SAPTAMAMSA, VARGA_DASAMSA, VARGA_SHODASAMSA, VARGA_SHASTIAMSA
                                        };

const static double shodasavarga_weight[16] = { 3.5, 1, 1, 3, .5, 1, .5, .5, 2, 4, .5, .5, .5, .5, .5, .5  };
const static int shodasavarga_vargas[16] = { VARGA_RASI, VARGA_HORA, VARGA_DREKKANA, VARGA_NAVAMSA, VARGA_DVADASAMSA, VARGA_TRIMSAMSA,
        VARGA_SAPTAMAMSA, VARGA_DASAMSA, VARGA_SHODASAMSA, VARGA_SHASTIAMSA, VARGA_VIMSAMSA, VARGA_SIDDHAMSA, VARGA_BHAMSA,
        VARGA_CHATURTAMSA, VARGA_CHATVARIMSAMSA, VARGA_AKSHAVEDAMSA
                                           };

// Dasas
enum { DASA_VIMSOTTARI = 0, DASA_YOGINI, DASA_KALACHAKRA, DASA_ASHTOTTARI, DASA_SHODSHOTTARI,
       DASA_DVADASHOTTARI, DASA_PANCHOTTARI, DASA_SHATABDIKA, DASA_CHATURASHITISAMA,
       DASA_DVISAPTATISAMA, DASA_SHATTRIMSATSAMA, DASA_LAGNAVIMSOTTARI
     };

/*************************************************//**
*
*
*
******************************************************/
struct KpData
{
	KpData() { lord = sublord = subsublord = OSUN; }
	int lord, sublord, subsublord;
};

// Kala Velas
enum { KALAVELA_LORD_NONE = -1, KALAVELA_LORD1, KALAVELA_LORD2, KALAVELA_LORD3, KALAVELA_LORD4,
       KALAVELA_LORD5, KALAVELA_LORD6, KALAVELA_LORD7, KALAVELA_LORD8
     };
enum { KALAVELA_PORTION_START = 0, KALAVELA_PORTION_MIDDLE, KALAVELA_PORTION_END };

const int nak_aspects[28][4] = {
	{ ASWINI, ROHINI, PPHALGUNI, JYESHTA },
	{ BHARANI, KRITTIKA, MAKHA, ANURADHA },
	{ KRITTIKA, VISAKHA, SRAVANA, BHARANI },
	{ ROHINI, SWATI, ABHIJIT, ASWINI },
	{ MRIGASIRA, CHITTA, USHADHA, REVATI },
	{ ARUDRA, HASTHA, PSHADHA, UBHADRA },
	{ PUNARVASU, UPHALGUNI, MULA, PBHADRA },
	{ PUSHYAMI, PPHALGUNI, JYESHTA, SATHABISHAK },
	{ ASLESHA, MAKHA, ANURADHA, DHANISHTA },
	{ MAKHA, SRAVANA, BHARANI, ASLESHA },
	{ PPHALGUNI, ABHIJIT, ASWINI, PUSHYAMI },
	{ UPHALGUNI, USHADHA, REVATI, PUNARVASU },
	{ HASTHA, PSHADHA, UBHADRA, ARUDRA },
	{ CHITTA, MULA, PBHADRA, MRIGASIRA },
	{ SWATI, JYESHTA, SATHABISHAK, ROHINI },
	{ VISAKHA, ANURADHA, DHANISHTA, KRITTIKA },
	{ ANURADHA, BHARANI, ASLESHA, VISAKHA },
	{ JYESHTA, ASWINI, PUSHYAMI, SWATI },
	{ MULA, REVATI, PUNARVASU, CHITTA },
	{ PSHADHA, UBHADRA, ARUDRA, HASTHA },
	{ USHADHA, PBHADRA, MRIGASIRA, UPHALGUNI },
	{ ABHIJIT, SATHABISHAK, ROHINI, PPHALGUNI },
	{ SRAVANA, MAKHA, KRITTIKA, DHANISHTA },
	{ DHANISHTA, ASLESHA, VISAKHA, SRAVANA },
	{ SATHABISHAK, PUSHYAMI, SWATI, ABHIJIT },
	{ PBHADRA, PUNARVASU, CHITTA, USHADHA },
	{ UBHADRA, ARUDRA, HASTHA, PSHADHA },
	{ REVATI, MRIGASIRA, UPHALGUNI, MULA }
};

/**************************************************************
***
**  Western/Uranian astrology
***
***************************************************************/
enum { CIRCLE_TYPE_NONE = -1, CIRCLE_TYPE_360, CIRCLE_TYPE_180, CIRCLE_TYPE_90, CIRCLE_TYPE_45, CIRCLE_TYPE_225, CIRCLE_TYPE_1125, CIRCLE_TYPE_5625  };

/**************************************************************
***
**  Command and view IDs
***
***************************************************************/
enum { CMD_CHILD_FIRST = wxID_HIGHEST + 1000,

       // status preferVedic, handled by all children
       CMD_CHILD_VEDICMODE, CMD_CHILD_WESTERNMODE,

       // Status of western graphic
       CMD_CHILD_WSHOWASPECTS, CMD_CHILD_WSHOWRETRO, CMD_CHILD_WSHOWHOUSES,
       CMD_CHILD_WSHOWPLANETCOLORS, CMD_CHILD_WSHOWSIGNCOLORS, CMD_CHILD_WSHOWHOUSECOLORS, CMD_CHILD_WSHOWASPECTCOLORS,
       CMD_CHILD_WSHOWASPECTSYMBOLS, CMD_CHILD_WSHOWLEFTASC, CMD_CHILD_WTRANSIT_STYLE,

       // Status of vedic charts (graphic mode)
       CMD_CHILD_VSHOWRETRO, CMD_CHILD_VSHOWARUDHA,
       CMD_CHILD_VSHOWPLANETCOLORS, CMD_CHILD_VSHOWSANSKRITSYMBOLS, CMD_CHILD_VSHOWAFFLICTIONS, CMD_CHILD_VSHOW_SBC_NAKSHATRA_QUALITY,
       CMD_CHILD_VSHOWSOUTHINDIAN, CMD_CHILD_VSHOWNORTHINDIAN, CMD_CHILD_VSHOWEASTINDIAN,
       CMD_CHILD_VSHOWCENTERNOTHING, CMD_CHILD_VSHOWCENTERVARGA, CMD_CHILD_VSHOWCENTERDIVISION, CMD_CHILD_VSHOWCENTERBOTH,
       CMD_CHILD_VSHOWNORTHASC, CMD_CHILD_VSHOWNORTHNUMBER, CMD_CHILD_VSHOWNORTHSHORT,
       CMD_CHILD_VSHOWNORTHSYMBOL,

       // 15x planetary objects (western and vedic): handled by western+vedic text+graphic
       CMD_CHILD_SHOWOUTER, CMD_CHILD_SHOWDRAGONHEAD, CMD_CHILD_SHOWDRAGONTAIL, CMD_CHILD_SHOWASCENDANT, CMD_CHILD_SHOWMERIDIAN,
       CMD_CHILD_SHOWURANIAN, CMD_CHILD_SHOWCHIRON, CMD_CHILD_SHOWPHOLUS, CMD_CHILD_SHOWPLANETOIDS, CMD_CHILD_SHOWLILITH,
       CMD_CHILD_SHOWUPAGRAHAS, CMD_CHILD_SHOWKALAVELAS, CMD_CHILD_SHOWSPECIALLAGNAS, CMD_CHILD_SHOWARIES, CMD_CHILD_SHOWD9LAGNA,
       CMD_CHILD_SHOWARABICPARTS,

       // 12 status flags for Vedic main window: handled by MainView -> TextViewBase with preferVedic = true
       CMD_CHILD_MAIN_SHOW_DECLINATION, CMD_CHILD_MAIN_SHOW_LORD,	CMD_CHILD_MAIN_SHOW_DIGNITY, CMD_CHILD_MAIN_SHOW_NAVAMSA,
       CMD_CHILD_MAIN_SHOW_NAKSHATRA, CMD_CHILD_MAIN_SHOW_PADA, CMD_CHILD_MAIN_SHOW_KP, CMD_CHILD_MAIN_SHOW_SHASTIAMSA,
       CMD_CHILD_MAIN_SHOW_DASAVARGA, CMD_CHILD_MAIN_SHOW_HOUSES, CMD_CHILD_MAIN_SHOW_KARAKA, CMD_CHILD_MAIN_SHOW_ASHTAKA,
       CMD_CHILD_MAIN_SHOW_UPA, CMD_CHILD_MAIN_SHOW_HOUSEPOS,

       // handle by text widget
       CMD_CHILD_COPY, CMD_CHILD_SELECT_ALL, CMD_CHILD_SAVETEXT,	CMD_CHILD_EXPORT,

       // delegated to application object
       CMD_CHILD_CLOSE, CMD_CHILD_CLOSE_DOC, CMD_CHILD_EDITDATA, CMD_CHILD_ANIMATE,
       CMD_CHILD_SAVE, CMD_CHILD_SAVEAS, CMD_CHILD_PRINT, CMD_CHILD_QUICKPRINT, CMD_CHILD_TEX,

       // creation of new child views -> delegate to application object
       CMD_CHILD_NEW_TEXT, CMD_CHILD_NEW_TRANSIT, CMD_CHILD_NEW_SBC, CMD_CHILD_NEW_SOLAR, CMD_CHILD_NEW_YOGA, CMD_CHILD_NEW_YOGAEDITOR,
       CMD_CHILD_NEW_GRAPHICALDASA, CMD_CHILD_NEW_GRAPHICGRID, CMD_CHILD_NEW_VARGA, CMD_CHILD_NEW_ASHTAKAVARGA,
       CMD_CHILD_NEW_VIMSOPAKABALA, CMD_CHILD_NEW_DASA, CMD_CHILD_NEW_URANIAN, CMD_CHILD_NEW_WCHART,
			 

       //TODO = main text
       CMD_CHILD_NEW_MAINVIEW,

       // final commands
       CMD_CHILD_NEW_RASI,
       CMD_CHILD_GRAPHIC_STYLE = CMD_CHILD_NEW_RASI + 30,
       CMD_CHILD_LAST
     };

enum { VIEW_TEXT, VIEW_TRANSIT, VIEW_SBC, VIEW_SOLAR, VIEW_YOGA, VIEW_GRAPHICALDASA, VIEW_DASA, VIEW_URANIAN, VIEW_VARGA,
       VIEW_ASHTAKAVARGA, VIEW_VIMSOPAKABALA, VIEW_GRAPHIC, VIEW_GRAPHICGRID, VIEW_PLANETLIST, VIEW_HORA, VIEW_ECLIPSE, VIEW_PARTNER,
       VIEW_EPHEM, VIEW_MULTIPLE, VIEW_DIAGRAM, VIEW_SHADBALA,
			 VIEW_YOGA_EDITOR, VIEW_EMPTY
     };

enum {
	APP_CONFIGURE = wxID_PREFERENCES,
	APP_ABOUT = wxID_ABOUT,
	APP_FILEQUIT = wxID_EXIT,
	APP_FIRST = wxID_HIGHEST + 2000, APP_FILENEW, APP_FILE_NEW_SIMPLE, APP_FILE_NEW_MULTIPLE_DEFAULT, APP_FILE_NEW_MULTIPLE,
	APP_FILEOPEN = APP_FILE_NEW_MULTIPLE + 100,
	APP_SHOWHORA, APP_SHOWECLIPSE, APP_NEW_PARTNER, APP_YOGAEDITOR, APP_SHOWEPHEM,
	APP_HELP, APP_SHOWSTATUSBAR, APP_SHOWMAINTOOLBAR, APP_SAVECONFIG,
	APP_LOCATION, APP_LIST_BIGICONS, APP_LIST_SMALLICONS, APP_LIST_TABLE, APP_LISTCTRL,
	APP_FILERECENT
};

// Toolbar constants
enum { TBS_TEXTMODE = wxID_HIGHEST + 10000, TBS_VARGA, TBS_DASA,
       TBS_PARTNER1 , TBS_PARTNER2, TBS_DATE, TBS_TIME, TBS_CIRCLETYPE, TBS_YEARLENGTH,
       TBS_YEAR, TBS_NBYEARS, TBS_ECL_TYPE, TBS_TRANSITMODE, TBS_ORBIS, TBS_SORT, TBS_FILTER,
       TBS_URANIANMODE, TBS_ZOOM, TBS_ANTARDASALEVEL, TBS_SBCMODE, TBS_SBCTEXT,
       TBS_MONTH, TBS_EPHEMMODE, TBS_TZ, TBS_EPHEMDEG, TBS_EPHEMPLANETS,
       TBS_YOGAGROUPFILTER, TBS_YOGAFIRSTHOUSE, TBS_YOGASOURCEFILTER, TBS_DATE_PROGRESS,
       TBS_LAST
     };

enum { TB_SEPARATOR = wxID_HIGHEST + 10999 };
enum { TBB_NOW = wxID_HIGHEST + 11000, TBB_DASA, TBB_GOTO, TBB_NEW, TBB_OPEN, TBB_SAVE, TBB_SAVEAS, TBB_CLOSE };
enum { TB_TEXT = wxID_HIGHEST + 12000 };

/*************************************************//**
*
* \brief constants for text alignment
*
******************************************************/
struct Align
{
	static int Center, VCenter, Bottom, Top, HCenter, Left, Right;
	wxString toString( const int &a );
};

/*****************************************************
*
*
*
******************************************************/
#define DECLARE_SINGLETON_WITHOUT_CONSTRUCTOR( c ) \
public: \
	static c *get() \
	{ \
		if ( ego == 0 ) ego = new c; \
		return ego; \
	}\
protected: \
	static c *ego;

#define DECLARE_SINGLETON( c ) \
public: \
	static c *get() \
	{ \
		if ( ego == 0 ) ego = new c; \
		return ego; \
	}\
protected: \
	static c *ego; \
	c() {} \
	c( const c& ) {}

#define IMPLEMENT_SINGLETON( c ) c *c::ego = 0;

#endif


