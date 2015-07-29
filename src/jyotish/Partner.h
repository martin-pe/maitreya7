/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/Partner.h
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

#ifndef _PARTNER_H_
#define _PARTNER_H_

#ifdef __GNUG__
#pragma interface "Partner.h"
#endif

#include <wx/string.h>
#include "Horoscope.h"

class ChartProperties;
class Writer;

/*************************************************//**
*
* \brief encapsulates calculation of Vedic partner chart
*
******************************************************/
class VedicPartnerChart
{
public:

	VedicPartnerChart();
	VedicPartnerChart( Horoscope*, Horoscope* );

	void setHoroscope1( Horoscope *h ) {
		h1 = h;
	}
	void setHoroscope2( Horoscope *h ) {
		h2 = h;
	}

	void update();
	void write( Writer* );

	double getPoints() {
		return ptotal;
	}

private:
	Horoscope *h1, *h2;
	double pvarna, pvashya, ptara, pyoni, pmaitri, pkoota, pgana, pnadi, ptotal;
};

/*************************************************//**
*
* \brief composite view on 2 charts (midpoints of 2 charts)
*
******************************************************/
class CompositHoroscope : public Horoscope
{
public:
	CompositHoroscope();
	void update( Horoscope*, Horoscope* );
	void dump( Writer*, const ChartProperties*, const bool show_header = false );

private:
	DECLARE_CLASS( CompositHoroscope )
};

#endif

