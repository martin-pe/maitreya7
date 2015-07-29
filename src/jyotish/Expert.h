/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/Expert.h
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

#ifndef _EXPERT_H_
#define _EXPERT_H_

#ifdef __GNUG__
#pragma interface "Expert.h"
#endif

class Horoscope;
class VargaHoroscope;
class Writer;
class wxString;

/*************************************************//**
*
* \brief base class for experts (encapsulation of various astrological calculations)
*
******************************************************/
class Expert
{
public:

	Expert();
	Expert( const Horoscope* );
	Expert( const VargaHoroscope* );
	Expert( const Horoscope*, const VargaHoroscope* );
	virtual ~Expert() {}

	virtual void update() {}
	virtual void dump( wxString& ) {}
	virtual void write( Writer* ) {}

protected:
	const Horoscope *horoscope;
	const VargaHoroscope *chart;
};

#endif


