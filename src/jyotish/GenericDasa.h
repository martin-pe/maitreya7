/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/GenericDasa.h
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

#ifndef _GENERICDASA_H_
#define _GENERICDASA_H_

#ifdef __GNUG__
#pragma interface "GenericDasa.h"
#endif

#include "constants.h"
#include "Dasa.h"

class Horoscope;

/*************************************************//**
*
* \brief expert class for generic Dasas (following the pattern of Vimsottari, Yogini etc.)
*
******************************************************/
class GenericNakshatraDasaExpert : public GrahaDasaExpert
{
public:

	virtual vector<Dasa*> getFirstLevel( const Horoscope*, const int varga = 0 );
	virtual vector<Dasa*> getNextLevel( Dasa* );
	virtual const wxChar *getName() const = 0;

	virtual bool hasKpFeatures() const {
		return true;
	}

	virtual KpData getKPLords( const double &len );

	virtual vector<KPEvent> getKPEventList( const double &startlen, const double &endlen, const double &startjd );

protected:

	void getKPLordRecursive( int &lord_index, double &elapsed, double &totallen );

	virtual double getPortion( const Horoscope *h, int &nak );
	virtual int getNbLords() = 0;
	virtual int getTotalLength() = 0;
	virtual int getDasaLordByIndex( const int& ) = 0;
	virtual int getDasaIndex4Nakshatra( const int& ) = 0;
	virtual int getDasaDuration( const int& ) = 0;
	virtual const wxChar *getExtraText( const int& ) {
		return 0;
	}
};

/*************************************************//**
*
* \brief expert class for Yogini Dasa
*
******************************************************/
class YoginiDasaExpert : public GenericNakshatraDasaExpert
{
public:

	virtual const wxChar *getName() const {
		return _( "Yogini" );
	}

protected:
	virtual int getNbLords() {
		return 8;
	}
	virtual int getTotalLength() {
		return 36;
	}
	virtual int getDasaLordByIndex( const int& );
	virtual int getDasaIndex4Nakshatra( const int& );
	virtual int getDasaDuration( const int& );
	virtual const wxChar *getExtraText( const int& );

	virtual wxString getDasaLordName( Writer*, const int &lord, const int format = TMEDIUM );
};

/*************************************************//**
*
* \brief expert class for Shodshottari Dasa
*
******************************************************/
class ShodshottariDasaExpert : public GenericNakshatraDasaExpert
{
public:

	virtual const wxChar *getName() const {
		return _( "Shodshottari" );
	}

protected:
	virtual int getNbLords() {
		return 8;
	}
	virtual int getTotalLength() {
		return 116;
	}
	virtual int getDasaLordByIndex( const int& );
	virtual int getDasaIndex4Nakshatra( const int& );
	virtual int getDasaDuration( const int& );
};

/*************************************************//**
*
* \brief expert class for Dvadashottari Dasa
*
******************************************************/
class DvadashottariDasaExpert : public GenericNakshatraDasaExpert
{
public:

	virtual const wxChar *getName() const {
		return _( "Dvadashottari" );
	}

protected:
	virtual int getNbLords() {
		return 8;
	}
	virtual int getTotalLength() {
		return 112;
	}
	virtual int getDasaLordByIndex( const int& );
	virtual int getDasaIndex4Nakshatra( const int& );
	virtual int getDasaDuration( const int& );
};

/*************************************************//**
*
* \brief expert class for Panchottari Dasa
*
******************************************************/
class PanchottariDasaExpert : public GenericNakshatraDasaExpert
{
public:

	virtual const wxChar *getName() const {
		return _( "Panchottari" );
	}

protected:
	virtual int getNbLords() {
		return 7;
	}
	virtual int getTotalLength() {
		return 105;
	}
	virtual int getDasaLordByIndex( const int& );
	virtual int getDasaIndex4Nakshatra( const int& );
	virtual int getDasaDuration( const int& );
};

/*************************************************//**
*
* \brief expert class for Shatabdika Dasa
*
******************************************************/
class ShatabdikaDasaExpert : public GenericNakshatraDasaExpert
{
public:

	virtual const wxChar *getName() const {
		return _( "Shatabdika" );
	}

protected:
	virtual int getNbLords() {
		return 7;
	}
	virtual int getTotalLength() {
		return 100;
	}
	virtual int getDasaLordByIndex( const int& );
	virtual int getDasaIndex4Nakshatra( const int& );
	virtual int getDasaDuration( const int& );
};

/*************************************************//**
*
* \brief expert class for Chaturashiti Sama Dasa
*
******************************************************/
class ChaturashitiSamaDasaExpert : public GenericNakshatraDasaExpert
{
public:

	virtual const wxChar *getName() const {
		return _( "Chaturashiti Sama" );
	}

protected:
	virtual int getNbLords() {
		return 7;
	}
	virtual int getTotalLength() {
		return 84;
	}
	virtual int getDasaLordByIndex( const int& );
	virtual int getDasaIndex4Nakshatra( const int& );
	virtual int getDasaDuration( const int& );
};

/*************************************************//**
*
* \brief expert class for Dvisaptati Sama Dasa
*
******************************************************/
class DvisaptatiSamaDasaExpert : public GenericNakshatraDasaExpert
{
public:

	virtual const wxChar *getName() const {
		return _( "Dvisaptati Sama" );
	}

protected:
	virtual int getNbLords() {
		return 8;
	}
	virtual int getTotalLength() {
		return 72;
	}
	virtual int getDasaLordByIndex( const int& );
	virtual int getDasaIndex4Nakshatra( const int& );
	virtual int getDasaDuration( const int& );
};

/*************************************************//**
*
* \brief expert class for ShatTrimshat Sama Dasa
*
******************************************************/
class ShatTrimshatSamaDasaExpert : public GenericNakshatraDasaExpert
{
public:

	virtual const wxChar *getName() const {
		return _( "Shat Trimsat Sama" );
	}

protected:
	virtual int getNbLords() {
		return 8;
	}
	virtual int getTotalLength() {
		return 36;
	}
	virtual int getDasaLordByIndex( const int& );
	virtual int getDasaIndex4Nakshatra( const int& );
	virtual int getDasaDuration( const int& );
};

#endif

