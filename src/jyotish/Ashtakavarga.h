/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/Ashtakavarga.h
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
#ifndef _ASHTAKAVARGA_H_
#define _ASHTAKAVARGA_H_

#ifdef __GNUG__
#pragma interface "Ashtakavarga.h"
#endif

#include<wx/string.h>

#include "Expert.h"

class Writer;

enum { REKHA, TRIKONA, EKADHI, RASISAMA, NAKSHATRASAMA };
enum { GRAHAPINDA, RASIPINDA, YOGAPINDA };

/*************************************************//**
*
* \brief encapsulates calculation of Ashtaka Varga
*
******************************************************/
class AshtakavargaExpert : public Expert
{
public:

	AshtakavargaExpert( const VargaHoroscope *v ) : Expert( v ) {}
	void update();

	void setChart( const VargaHoroscope *c ) {
		chart = c;
	}
	const VargaHoroscope *getChart() const {
		return chart;
	}

	// get Ashtakavarga value for rasi
	// type is one of REKHA, TRIKONA, EKADHI, RASISAMA, NAKSHATRASAMA
	int getItem( const int &type, const int &planet, const int &rasi );

	// returns the pinda value, pinda is one of GRAHAPINDA, RASIPINDA, YOGAPINDA
	int getPinda( const int &type, const int &rasi ) const;

	// returns the sums for each rasi, type is one of REKHA, TRIKONA, EKADHI, RASISAMA, NAKSHATRASAMA
	int getSarva( const int &type, const int &rasi ) const;

	int getPlanetSarva( const int &type, const int &planet ) const;

	void write( Writer *writer, const bool show_header = false );
	void writeSingleAv( Writer *writer, const int& );

private:


	void calcRekha();
	void calcSarva();
	void calcPinda();
	void calcTrikonaShodana();
	void calcEkadhipatyaPair( const int &b1, const int &b2 );
	void calcEkadhipatyaShodana();
	int getSingleRekha( const int &i, const int &j, const int &k );

	wxChar rekha[8][12], trikona[8][12], ekadhi[8][12];
	int sarvaRekha[12], sarvaTrikona[12], sarvaEkadhi[12];
	int psarvaRekha[8], psarvaTrikona[8], psarvaEkadhi[8];
	int grahaPinda[12], rasiPinda[12], yogaPinda[12];

	int planetNumber[12];
};

#endif

