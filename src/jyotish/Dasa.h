/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/Dasa.h
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
#ifndef _DASA_H_
#define _DASA_H_

#ifdef __GNUG__
#pragma interface "Dasa.h"
#endif

#include "constants.h"

#include <vector>
#include <wx/string.h>

class Dasa;
class DasaExpert;
class Horoscope;
class VargaHoroscope;
class Writer;

using namespace std;

/*************************************************//**
*
* \brief abstract class encapsulating a Dasa with its Lord, start, end and parent Dasa
*
******************************************************/
class Dasa
{
public:
	virtual ~Dasa() {}

	// the jd's for start and end of the dasa
	virtual double getStartJD() const = 0;
	virtual double getEndJD() const = 0;

	// the jd of birth (because dasa dates before birth have to be faded out)
	virtual double getBirthJD() const = 0;

	// the lord: either a planet or a sign
	virtual int getDasaLord() const = 0;

	// the parent: o for mahadasa, > 0 for Antardasas, etc.
	virtual Dasa *getParent() const = 0;
	virtual void setParent( Dasa* ) = 0;

	// some extra information about the dasa like the dehas and jeevas for kalachakra dasa
	virtual const wxString getExtraText() const = 0;
};

/*************************************************//**
*
* \brief base implementation of abstract Dasa class
*
******************************************************/
class DasaImpl : public Dasa
{
public:

	DasaImpl( DasaExpert*, const int lord, const double startjd, const double endjd, const wxChar *extraText, Dasa* );
	~DasaImpl() {}

	virtual double getStartJD() const {
		return start_jd;
	}
	virtual double getEndJD() const {
		return end_jd;
	}
	virtual double getBirthJD() const {
		return birth_jd;
	}
	virtual int getDasaLord() const {
		return lord;
	}
	virtual Dasa *getParent() const {
		return parent;
	}
	virtual const wxString getExtraText() const {
		return extraText;
	}
	virtual void setParent( Dasa *d ) {
		parent = d;
	}
	virtual void dump( wxString& );

protected:
	DasaImpl() {}
	DasaExpert *expert;
	double start_jd, end_jd, birth_jd;
	int lord;
	Dasa *parent;
	wxString extraText;
};

/*************************************************//**
*
* \brief implementation of abstract Dasa class for Rasi Dasas
*
******************************************************/
class RasiDasaImpl : public DasaImpl
{
public:
	RasiDasaImpl( DasaExpert*, VargaHoroscope*, const int &lord, const double &startjd, const double &endjd, const wxChar *eText, Dasa* );
	~RasiDasaImpl();

	virtual VargaHoroscope *getVargaHoroscope() const {
		return chart;
	}

protected:
	RasiDasaImpl() {}
	VargaHoroscope *chart;
};

/*************************************************//**
*
* \brief event in KP calculation (Ephemeris view)
*
******************************************************/
class KPEvent
{
public:
	KPEvent( int l, int sl, double ll, double jjd, const int &index )
	{
		lord = l;
		sublord = sl;
		len = ll;
		jd = jjd;
		dasaindex = index;
	}
	int lord, sublord, dasaindex;
	double len, jd;
};

/*************************************************//**
*
* \brief abstract class encapsulating the calculation of Dasas
*
******************************************************/
class DasaExpert
{
public:
	virtual ~DasaExpert() {}
	virtual vector<Dasa*> getFirstLevel( const Horoscope*, const int varga = 0 ) = 0;
	virtual vector<Dasa*> getNextLevel( Dasa* ) = 0;

	virtual const wxChar *getName() const = 0;
	virtual bool hasKpFeatures() const {
		return false;
	}

	void write( Writer*, const Horoscope*, const int varga = 0, const bool show_header = true );
	void writeCompact( Writer*, const Horoscope*, const int varga = 0, const bool show_header = true );
	void writeMore( Writer*, const Horoscope*, const int varga = 0, const bool show_header = true );

	virtual vector<KPEvent> getKPEventList( const double &startlen, const double &Endlen, const double &startjd );

	virtual KpData getKPLords( const double &len ) { return KpData(); }

	virtual const bool isGrahaDasaExpert() const = 0;
	const bool isRasiDasaExpert() const  {
		return ! isGrahaDasaExpert();
	}

	virtual wxString getDasaLordName( Writer*, const int &lord, const int format = TMEDIUM );

protected:
};

/*************************************************//**
*
* \brief abstract class encapsulating the calculation of Graha Dasas
*
******************************************************/
class GrahaDasaExpert : public DasaExpert
{
public:
	const bool isGrahaDasaExpert() const {
		return true;
	}
};

/*************************************************//**
*
* \brief abstract class encapsulating the calculation of Rasi Dasas
*
******************************************************/
class RasiDasaExpert : public DasaExpert
{
public:
	const bool isGrahaDasaExpert() const {
		return false;
	}
};

/*************************************************//**
*
* \brief encapsulates the creation of Dasa Experts
*
******************************************************/
class DasaExpertFactory
{
public:
	static DasaExpertFactory *get();
	const static unsigned int MAX_DASAEXPERTS = 12;
	DasaExpert *getDasaExpert( const unsigned int& );

private:
	DasaExpertFactory();
	static DasaExpertFactory *ego;
	void init();
	vector<DasaExpert*>  experts;

	DasaExpert *createVimsottariDasaExpert();
	DasaExpert *createYoginiDasaExpert();
	DasaExpert *createKalachakraDasaExpert();
	DasaExpert *createAshtottariDasaExpert();
	DasaExpert *createShodshottariDasaExpert();
	DasaExpert *createDvadashottariDasaExpert();
	DasaExpert *createPanchottariDasaExpert();
	DasaExpert *createShatabdikaDasaExpert();
	DasaExpert *createChaturashitiSamaDasaExpert();
	DasaExpert *createDvisaptatiSamaDasaExpert();
	DasaExpert *createShatTrimshatSamaDasaExpert();
	DasaExpert *createLagnaVimsottariDasaExpert();

	DasaExpert *createJaiminiDasaExpert( const int& );
};

#endif

