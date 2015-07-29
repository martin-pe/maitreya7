/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/VedicChart.h
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

#ifndef VEDICCHART_H_
#define VEDICCHART_H_

#ifdef __GNUG__
#pragma interface "VedicChart.h"
#endif

#include "VedicRasiChart.h"

class ChartProperties;
class VargaHoroscope;

/*************************************************//**
*
* \brief Vedic chart graphic based upon a Varga chart
*
******************************************************/
class VedicVargaChart : public VedicRasiChart
{
public:

	/**
	 *  \brief
	 */
	VedicVargaChart( const VargaHoroscope*, const ChartProperties* );

	/**
	 *  \brief
	 */
	VedicVargaChart( const Horoscope*, const int &varga, const ChartProperties* );

	/**
	 *  \brief
	 */
	VedicVargaChart( const VargaHoroscope*, const VargaHoroscope*, const ChartProperties* );

	/**
	 *  \brief
	 */
	VedicVargaChart( const Horoscope*, const int &varga1, const Horoscope*, const int &varga2, const ChartProperties* );
	~VedicVargaChart();

	/**
	 *  \brief Returns the underlying Varga horoscope for chart id (0 or 1)
	 */
	const VargaHoroscope *getVargaHoroscope( const int &chart_id ) { return chart_id == 0 ? vh1 : vh2; }

protected:
	virtual wxString getCenterString();

	virtual int getPlanetField( const int &planet, const int &chart_id );
	virtual bool getPlanetRetro( const int &planet, const int &chart_id );

	virtual void writeChartContents( const int &chart_id );

	const VargaHoroscope *vh1, *vh2;
};

/*************************************************//**
*
* \brief Ashtakavarga chart based on abstract Vedic Rasi chart
*
******************************************************/
class AshtakaVargaChart : public VedicRasiChart
{
public:
	AshtakaVargaChart( const ChartProperties* );
	void setAscendant( const int &a ) { ascendant = a; }
	void setRasiValue( const int &r, const int &value );
	void setRasiValues( const int r[] );
	void setCenterString( wxString cs ) { centerString = cs; }
	void setGraphicSupport( const bool &b ) { graphicSupport = b; }

protected:
	virtual wxString getCenterString() { return centerString; }
	virtual void writeChartContents( const int &chart_id );

	wxString centerString;
	int rasi_values[12];
	bool graphicSupport;

private:

};

#endif

