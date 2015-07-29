/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/AshtottariDasa.h
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
#ifndef _ASHTOTTARIDASA_H_
#define _ASHTOTTARIDASA_H_

#ifdef __GNUG__
#pragma interface "AshtottariDasa.h"
#endif

#include "constants.h"
#include "Dasa.h"

class Horoscope;
class Dasa;

/*************************************************//**
*
* \brief calculation of Ashtottari Dasa
*
******************************************************/
class AshtottariDasaExpert : public GrahaDasaExpert
{
public:

	virtual vector<Dasa*> getFirstLevel( const Horoscope*, const int varga = 0 );
	virtual vector<Dasa*> getNextLevel( Dasa* );
	virtual const wxChar *getName() const {
		return _( "Ashtottari" );
	}

private:
	const static int dasa_lord[8];
	const static int dasa_dur[8];
};

#endif

