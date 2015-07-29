/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/Eclipse.h
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

#ifndef _ECLIPSE_H_
#define _ECLIPSE_H_

#ifdef __GNUG__
#pragma interface "Eclipse.h"
#endif

#include <list>

class Writer;
class EclipseData;

using namespace std;

/*************************************************//**
*
* \brief Encapsulates calculation of Eclipses
*
******************************************************/
class EclipseExpert
{
public:
	EclipseExpert();
	~EclipseExpert();

	void update( const double &startjd, const double &endjd, const int &planet, const int &type, const bool &vedic );
	void write( Writer*, const bool &useLocaltime );

private:
	list<EclipseData*> data;
};

#endif


