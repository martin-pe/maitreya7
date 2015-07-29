/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/KalachakraDasa.h
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
#ifndef _KALACHAKRADASA_H_
#define _KALACHAKRADASA_H_

#ifdef __GNUG__
#pragma interface "KalachakraDasa.h"
#endif

#include "Dasa.h"
#include "constants.h"

class Horoscope;

/*************************************************//**
*
* \brief implementation of Dasa class for Kalachakra
*
******************************************************/
class KalachakraDasaImpl : public DasaImpl
{
public:

	KalachakraDasaImpl( DasaExpert*, const int &lord, const double &startjd, const double &endjd, const int &pada, const int &group,
	                    const wxChar *extraText, Dasa* = 0 );

	int getGroup() {
		return group;
	}
	int getPada() {
		return pada;
	}

protected:
	int pada;
	int group;
};

/*************************************************//**
*
* \brief expert class for Kalachakra encapsulates calculation
*
******************************************************/
class KalachakraDasaExpert : public RasiDasaExpert
{
public:

	KalachakraDasaExpert();
	virtual vector<Dasa*> getFirstLevel( const Horoscope*, const int varga = 0 );
	virtual vector<Dasa*> getNextLevel( Dasa* );
	virtual const wxChar *getName() const {
		return _( "Kalachakra" );
	}
	virtual bool hasKpFeatures() const {
		return true;
	}
	virtual vector<KPEvent> getKPEventList( const double &startlen, const double &endlen, const double &startjd );

private:
	int deha, jeeva;
	bool savya;

	static const wxChar K_KALA[4][4][9];
	static const int K_RASIYEARS[12];
	static const int K_TOTALYEARS[4][4];

	static const int K_NAKSHATRA_GROUP[27];
	static const int K_APSAVYA_AMSA[12];

};

#endif

