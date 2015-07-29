/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/Varga.h
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

#ifndef _VARGA_H_
#define _VARGA_H_

#ifdef __GNUG__
#pragma interface "Varga.h"
#endif

#include <wx/string.h>

#include "Expert.h"
#include "constants.h"

class Horoscope;
class Planet;
class VargaHoroscope;
class Writer;

/*************************************************//**
*
* \brief Varga properties of a single planet
*
******************************************************/
class VargaData
{
	friend class Planet;

public:
	VargaData( const Planet*, const int &division );

	int getDivision() const {
		return division;
	}

	int getRasi() const {
		return rasi;
	}

	int getVargaViswa() const {
		return vargaViswa;
	}

	int getSaptaVargajaBala() const {
		return saptaVargajaBala;
	}

	int getDignity() const {
		return dignity;
	}

private:

	const Planet *planet;
	const int division;

	int rasi;
	int vargaViswa, saptaVargajaBala, dignity;
};

/*************************************************//**
*
* \brief encapsulates calculation of Vargas
*
******************************************************/
class VargaExpert : public Expert
{
public:

	VargaExpert() : Expert() {}
	VargaExpert( const Horoscope *h ) : Expert( h ) {}

	const wxChar *getVargaName( const int& ) const;
	const int getVargaDivision( const int& ) const;
	int getVargaIndexByDivision( const int& ) const;

	int calcVarga( const double& len, const int &vargaindex );

	virtual void write( Writer*, const int &style, const bool show_header = true );

private:

	// Dvadasamsa calculation plays a special role an is used by D-12, D-108 and D-144
	double getDvadasamsaLength( const double& ) const;

	const static int vargaDivisions[NB_VARGAS];

};

#endif


