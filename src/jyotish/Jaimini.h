/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/Jaimini.h
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
#ifndef _JAIMINI_H_
#define _JAIMINI_H_

#ifdef __GNUG__
#pragma interface "Jaimini.h"
#endif

#include "Expert.h"

class Horoscope;
class Writer;

/*************************************************//**
*
*  encapsulation of calculations in Jaimini astrology
*
******************************************************/
class JaiminiExpert : public Expert
{
public:

	// the expert is varga dependent
	JaiminiExpert( const VargaHoroscope *v ) : Expert( v ) {}

	// update all
	void update();

	// update only padas
	int calcPada( const int &rasi );

	// stronger of two rasis
	int getStrongerRasi( const int &rasi1, const int &rasi2 );

	// lord of a rasi (including rahu/ketu for aquarius/scorpio)
	int getJaiminiLord( const int &rasi );

	// whether the nodes are stronger than saturn/mars for aq/sc
	bool isKetuStrongerThanMars();
	bool isRahuStrongerThanSaturn();

	// number of years for each rasi (0..12)
	int getRasiYears( const int &rasi );

	void write( Writer*, const bool show_header = true );

	// calculation of wxChara karakas can be run standalone without update()
	void calcCharaKarakas();

	// index is one of ATMA_KARAKA, AMATYA_KARAKA, BHRATRU_KARAKA, MATRU_KARAKA, PITRU_KARAKA, PUTRA_KARAKA, GNATI_KARAKA, DARA_KARAKA
	// return value is the correspoinding planet ISUN..IRAHU
	int getCharaKaraka( const int &index );

	// returns the karaka fpr the planet ISUN..IRAHU. Return value is ATMA_KARAKA, etc
	int getCharaKarakaProperty( const int &planet );

private:

	int calcPlanetAspects( const int &planet );
	void calcYears();
	void calcNodeStrength();
	int calcSignAspects( const int &rasi );

	int nbplanets[12], rasiYears[12], nbAspectsOnRasi[12], chara_karaka[8], p_chara_karaka[8];
	int aspSat, aspKet, aspMar, aspRah;
	bool ketuStrongerThanMars, rahuStrongerThanSaturn;
};

#endif

