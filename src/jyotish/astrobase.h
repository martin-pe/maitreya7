/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/astrobase.h
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
#ifndef _ASTROBASE_H_
#define _ASTROBASE_H_

#ifdef __GNUG__
#pragma interface "astrobase.h"
#endif

#include "constants.h"
#include <vector>

using namespace std;


/**************************************************************
***
**  Variuos mathematical funtions
***
***************************************************************/
double calcYearLength( const int sel, const bool vedic );

// whether len is between to deg values
int between( const double &len, const double &min, const double &max );

// Calculate the distance between 2 points
double planetDistance( const double &len1, const double &len2 );

// return the number of days for a given month and an given year, including leap years
int calcMonthLength( int monat, int jahr );

// get number of years, etc for dasa length
void getDasaDuration( wxString&, double length );
void getDasaDurationInts( int &year, int &month, int &day, double length );

double getYearLength( const bool vedic );
void getDegMinSecInts2( const double &len, int &deg, int &min, double &sec );
void getDegMinSecInts( const double &len, int &deg, int &min, int &sec );

/**************************************************************
***
**  Basic astrological funtions
***
***************************************************************/
// get the rasi for a given length (0..11)
int getRasi( const double &len );

// get the length in a specific rasi - reverse will be counted from the end (0..30)
double getRasiLen( const double &len, const bool &reverse = false );

// get the opposite point
double getOpposition( const double& );

// whether len is in odd, even or odd footed rasi
int isOddRasi( const double &len );
int isEvenRasi( const double &len );
int isOddFootedRasi( const int &rasi );

// get the varna of a length
int getVarna( const double &len );

// whether len is in ascendant, kona, kendra resp. to ascendant length
int inAscendant( const double &len, const double &asc );
int inTrikona( const double &len, const double &asc );
int inKendra( const double &len, const double &asc );

// whether len is in movable, fixed or dual sign
int inMovableSign( const double &len );
int inFixedSign( const double &len );
int inDualSign( const double &len );

// get the lord of a rasi (without nodes, otherwise use JaiminiExpert)
int getLord( const int &planet, const bool vedic = true );

// Get the rasi where planet is exalted
int getExaltationRasi( const int &planet );

// whether planet is exalted or debilated in rasi
bool hasExaltationRasi( const int &planet, const int &rasi );
bool hasDebilationRasi( const int &planet, const int &rasi );

// return the mulatrikona rasi for planet
int getMulatrikonaRasi( const int &planet );

// whether planet with len is in mulatrikona (only for valid deg length)
bool isInMulatrikona( const int &planet, const double &len );

// The same if only the rasi is known
bool hasMulatrikonaRasi( const int &planet, const int &rasi );

// if 2 planets are naisargika or tatkalika mitra
int isFriend( const int &planet1, const int &planet2 );
int isRasiPosTempFriend( const int &rasi1, const int &rasi2 );

// Calculate sputa drishti of 2 planets with a given length
//double getAspectValue( const int &planet1, const double &len1, const int &planet2, const double &len2 );
double getAspectValue( const int &aspectingPlanet, const int &aspectedPlanet, const double &len  );

// Name of the avastha ( Sayana, Upavesana, etc.)
const wxChar *getAvasthaName( const int &i , const int &avastha = 0 );

bool getGrahaDrishti( const int &aspectingPlanet, const int &rasidiff );
double getGrahaDrishtiValue( const int &aspectingPlanet, const int &rasidiff );

/**************************************************************
***
**  Jaimini and Nakshatras
***
***************************************************************/
// get the length of a planet in his nakshatra (nsys = N27|N28)
double getNakshatraLength( const double &len, const int nsys );

// get the nakshatra of a planet with given length (nsys = N27|N28)
int getNakshatra( const double &len, const int nsys );

// number of pada for a given length
int getPada( const double &len );

// whether 2 rasi positions have a rasi drishti
bool hasJaiminiAspect( const int &pos1, const int &pos2 );

// whether 2 rasis are vishama of sama pada
bool isVishamapadaRasi( const int &rasi );
bool isSampadaRasi( const int &rasi );

double getNavamsaPortion( const double &len, const int &pindex, const bool &retro = false );

int getVashya( const double &len );

/**************************************************************
***
**  Western astrology
***
***************************************************************/
double calcGeometricalMidpoint( const double &len1, const double &len2 );

// Number of degrees for a given circle type constant
double getDegrees4CircleType( const int &circle );


#endif


