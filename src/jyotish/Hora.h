/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/Hora.h
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

#ifndef _HORA_H_
#define _HORA_H_

#ifdef __GNUG__
#pragma interface "Hora.h"
#endif

class Location;
class Writer;

/*************************************************//**
*
* \brief Calculates the Lords of year, month, day and hour
*
******************************************************/
class HoraExpert
{
public:
	HoraExpert();

	void setLocation( Location* );
	void update( const double &jd );

	int getDinaLord() const {
		return dinaLord;
	}
	int getMasaLord() const {
		return masaLord;
	}
	int getVarshaLord() const {
		return varshaLord;
	}
	int getCurrentHoraLord() const {
		return currentHoraLord;
	}

	int getHoraLord( const int& ) const;
	double getHoraStart( const int& ) const;

	void write( Writer*, const bool localtime = true );

private:

	Location *location;
	int horaLord[24];
	double horaStart[25];
	int dinaLord, varshaLord, masaLord, currentHoraLord;
};

#endif


