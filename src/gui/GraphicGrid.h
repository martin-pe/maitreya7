/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/GraphicGrid.h
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

#ifndef _GRAPHICGRID_H_
#define _GRAPHICGRID_H_

#ifdef __GNUG__
#pragma interface "GraphicGridWidget.h"
#endif

#include<vector>
#include "BasicWidget.h"

using namespace std;

class AshtakavargaExpert;
class AshtakaVargaChart;
class ChartProperties;
class Horoscope;
class Painter;
class VargaHoroscope;
class VedicGraphic;
class VedicVargaChart;
class WesternChart;

/*************************************************//**
*
* \brief Complex graphical chart grid with n:m rows and columns of Vedic Varga charts/alternativly one Western chart
*
******************************************************/
class ChartGridWidget : public BasicWidget
{
	DECLARE_CLASS( ChartGridWidget )
public:
	ChartGridWidget( wxWindow *parent, ChartProperties*, const int &rows, const int &cols,
	                 const wxPoint& pos = wxDefaultPosition, const wxSize& = wxDefaultSize );
	~ChartGridWidget();

	void addVedicVargaChart( VedicVargaChart* );
	void addVedicVargaChart( const Horoscope*, const int &varga );
	void addVedicVargaChart( const Horoscope *h1, const Horoscope *h2, const int &varga );
	void deleteVargaCharts();

	virtual void OnDataChanged();

protected:
	const int rows, cols;

	WesternChart *wchart;
	vector <VedicVargaChart*> vediccharts;

	virtual void doPaint( bool eraseBackground = true, const wxRect* rect = 0 );
};

/*************************************************//**
*
* \brief Convenience class for ChartGridWidget with 6 Shadvarga charts (D-1, D-2, D-3, D-9, D-12, D-30)
*
******************************************************/
class ShadVargaChartGridWidget : public ChartGridWidget
{
	DECLARE_CLASS( ShadVargaChartGridWidget )

public:
	ShadVargaChartGridWidget( wxWindow *parent, ChartProperties*, const Horoscope* = 0, const Horoscope* = 0 );

	void setCharts( const Horoscope *h1 = 0, const Horoscope *h2 = 0 );
};

/*************************************************//**
*
* \brief Convenience class for ChartGridWidget with 6 Varga charts for 2nd tab of VargaView (D-4, D-7, D-10, D-16, D-20, D-24)
*
******************************************************/
class VargaTab2ChartGridWidget : public ChartGridWidget
{
	DECLARE_CLASS( VargaTab2ChartGridWidget )

public:
	VargaTab2ChartGridWidget( wxWindow *parent, ChartProperties*, const Horoscope*, const Horoscope* = 0 );

	void setCharts( const Horoscope *h1 = 0, const Horoscope *h2 = 0 );
};

/*************************************************//**
*
* \brief Convenience class for ChartGridWidget with 6 Varga charts for 3rd tab of VargaView (D-6, D-8, D-27, D-40, D-45, D-60)
*
******************************************************/
class VargaTab3ChartGridWidget : public ChartGridWidget
{
	DECLARE_CLASS( VargaTab3ChartGridWidget )

public:
	VargaTab3ChartGridWidget( wxWindow *parent, ChartProperties*, const Horoscope*, const Horoscope* = 0 );

	void setCharts( const Horoscope *h1 = 0, const Horoscope *h2 = 0 );
};

/*************************************************//**
*
* \brief Convenience class for ChartGridWidget with 4 Vedic charts (D-1, Bhava, D-9, D-30)
*
******************************************************/
class DefaultEwChartGridWidget : public ChartGridWidget
{
	DECLARE_CLASS( DefaultEwChartGridWidget )
public:
	DefaultEwChartGridWidget( wxWindow *parent, ChartProperties*, Horoscope*, const wxPoint& pos = wxDefaultPosition, const wxSize& = wxDefaultSize );
};

/*************************************************//**
*
* \brief Chart grid for Ashtakavarga charts
*
******************************************************/
class AshtakaVargaChartGridWidget : public BasicWidget
{
	DECLARE_CLASS( AshtakaVargaChartGridWidget )
public:
	AshtakaVargaChartGridWidget( wxWindow *parent, ChartProperties*, AshtakavargaExpert*, const int &type, const int &rows, const int &cols,
	                             const wxPoint& pos = wxDefaultPosition, const wxSize& = wxDefaultSize );
	~AshtakaVargaChartGridWidget();

	virtual void doPaint( bool eraseBackground = true, const wxRect* rect = 0 );

protected:

	const int ashtakatype;
	const int rows, cols;

	AshtakavargaExpert *expert;
	AshtakaVargaChart *chart;
};

#endif

