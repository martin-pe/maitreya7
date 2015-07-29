/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/Expert.cpp
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

#ifdef __GNUG__
#pragma implementation "Expert.h"
#endif

#include "Expert.h"
#include "Varga.h"
#include "VargaHoroscope.h"

/*****************************************************
**
**   Expert   ---   Constructor
**
******************************************************/
Expert::Expert()
		:		horoscope( 0 ),
		chart( 0 )
{
}

/*****************************************************
**
**   Expert   ---   Constructor
**
******************************************************/
Expert::Expert( const Horoscope *h )
		:	horoscope( h ),
		chart( 0 )
{
}

/*****************************************************
**
**   Expert   ---   Constructor
**
******************************************************/
Expert::Expert( const VargaHoroscope *v )
		: horoscope( v->getHoroscope() ),
		chart( v )
{
}

/*****************************************************
**
**   Expert   ---   Constructor
**
******************************************************/
Expert::Expert( const Horoscope *h, const VargaHoroscope *chart )
		: horoscope( h ),
		chart ( chart )
{
}

