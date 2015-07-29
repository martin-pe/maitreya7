/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/GraphicGrid.cpp
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
#pragma implementation "GraphicGridWidget.h"
#endif

#include "GraphicGrid.h"

#include "Ashtakavarga.h"
#include "ChartProperties.h"
#include "ColorConfig.h"
#include "Conf.h"
#include "Painter.h"
#include "Lang.h"
#include "PlanetList.h"
#include "Varga.h"
#include "VargaHoroscope.h"
#include "VedicChart.h"
#include "VedicChartConfig.h"
#include "WesternChart.h"
#include "WesternChartConfig.h"
#include "Writer.h"

extern Config *config;

IMPLEMENT_CLASS( ChartGridWidget, BasicWidget )
IMPLEMENT_CLASS( DefaultEwChartGridWidget, ChartGridWidget )
IMPLEMENT_CLASS( ShadVargaChartGridWidget, ChartGridWidget )
IMPLEMENT_CLASS( VargaTab2ChartGridWidget, ChartGridWidget )
IMPLEMENT_CLASS( VargaTab3ChartGridWidget, ChartGridWidget )
IMPLEMENT_CLASS( AshtakaVargaChartGridWidget, BasicWidget )

/*****************************************************
**
**   AshtakaVargaChartGridWidget   ---   Constructor
**
******************************************************/
AshtakaVargaChartGridWidget::AshtakaVargaChartGridWidget( wxWindow *parent, ChartProperties *chartprops, AshtakavargaExpert *e, const int &atype,
        const int &rows, const int &cols,	const wxPoint& pos, const wxSize &size )
		: BasicWidget( parent, chartprops, -1, pos, size ),
		ashtakatype( atype ),
		rows( rows ),
		cols( cols )
{
	expert = e;
	chart = new AshtakaVargaChart( chartprops );
}

/*****************************************************
**
**   AshtakaVargaChartGridWidget   ---   Destructor
**
******************************************************/
AshtakaVargaChartGridWidget::~AshtakaVargaChartGridWidget()
{
	delete chart;
}

/*****************************************************
**
**   AshtakaVargaChartGridWidget   ---   doPaint
**
******************************************************/
void AshtakaVargaChartGridWidget::doPaint( bool eraseBackground, const wxRect* rect )
{
	int w, h;
	int nb = 0;
	Writer *writer = WriterFactory().getWriter( WRITER_TEXT );
	Lang lang;

	assert( painter );
	getWidgetSize( &w, &h  );
	const int xstep = w / cols;
	const int ystep = h / rows;

	vector<int> obs = PlanetList().getVedicPlanetOnlyList();
	obs.push_back( OASCENDANT );

	for ( int row = 0; row < rows; row++ )
	{
		for ( int col = 0; col < cols; col++ )
		{
			assert( nb < rows * cols );
			chart->setAscendant( expert->getChart()->getRasi( OASCENDANT ));
			for ( int rasi = ARIES; rasi <= PISCES; rasi++ )
			{
				if ( nb == 0 )
				{
					chart->setRasiValue( rasi, expert->getSarva( ashtakatype, rasi ));
					chart->setCenterString( _( "Sarva" ));
					chart->setGraphicSupport( false );
					chart->setCenterGraphicFormat( false );
				}
				else if ( nb > 0 && nb <= 8 )
				{
					chart->setRasiValue( rasi, expert->getItem( ashtakatype, obs[nb-1], rasi ));
					chart->setCenterGraphicFormat( config->usePlanetSymbols );
					chart->setCenterString( config->usePlanetSymbols ?  lang.getPlanetSymbolCode( obs[nb-1]) : writer->getObjectName( obs[nb-1], TLARGE ));
					chart->setGraphicSupport( true );
				}
				else assert( false );
			}
			chart->OnDataChanged();
			chart->paint( painter, MyRect( col * xstep, row * ystep, xstep, ystep ));
			nb++;
		}
	}
	delete writer;
}

/*****************************************************
**
**   ChartGridWidget   ---   Constructor
**
******************************************************/
ChartGridWidget::ChartGridWidget( wxWindow *parent, ChartProperties *chartprops, const int &rows, const int &cols, const wxPoint& pos, const wxSize& size )
		: BasicWidget( parent, chartprops, -1, pos, size ),
		rows( rows ),
		cols( cols )
{
	this->wchart = 0;
}

/*****************************************************
**
**   ChartGridWidget   ---   Destructor
**
******************************************************/
ChartGridWidget::~ChartGridWidget()
{
	deleteVargaCharts();
}

/*****************************************************
**
**   ChartGridWidget   ---   addVedicVargaChart
**
******************************************************/
void ChartGridWidget::addVedicVargaChart( VedicVargaChart *chart )
{
	assert( chart );
	if ( vediccharts.size() == 0 && wchart == 0 )
	{
		const VargaHoroscope *vh1 = chart->getVargaHoroscope( 0 );
		const Horoscope *h1 = vh1 ? vh1->getHoroscope() : 0;
		const VargaHoroscope *vh2 = chart->getVargaHoroscope( 1 );
		const Horoscope *h2 = vh2 ? vh2->getHoroscope() : 0;
		wchart = new WesternChart( h1, h2 ? h2 : h1, chartprops );
		wchart->OnDataChanged();
	}
	vediccharts.push_back( chart );
}

/*****************************************************
**
**   ChartGridWidget   ---   addVedicVargaChart
**
******************************************************/
void ChartGridWidget::addVedicVargaChart( const Horoscope *h, const int &varga )
{
	addVedicVargaChart( new VedicVargaChart( new VargaHoroscope( h, varga ), chartprops ));
}

/*****************************************************
**
**   ChartGridWidget   ---   addVedicVargaChart
**
******************************************************/
void ChartGridWidget::addVedicVargaChart( const Horoscope *h1, const Horoscope *h2, const int &varga )
{
	if ( h2 ) addVedicVargaChart( new VedicVargaChart( new VargaHoroscope( h1, varga ), new VargaHoroscope( h2, varga ) , chartprops ));
	else addVedicVargaChart( h1, varga );
}

/*****************************************************
**
**   ChartGridWidget   ---   deleteVargaCharts
**
******************************************************/
void ChartGridWidget::deleteVargaCharts()
{
	for ( unsigned int i = 0; i < vediccharts.size(); i++ ) delete vediccharts[i];
	vediccharts.clear();
	if ( wchart != 0 ) delete wchart;
	wchart = 0;
}

/*****************************************************
**
**   ChartGridWidget   ---   OnDataChanged
**
******************************************************/
void ChartGridWidget::OnDataChanged()
{
	if ( wchart ) wchart->OnDataChanged();
	Refresh();
}

/*****************************************************
**
**   ChartGridWidget   ---   doPaint
**
******************************************************/
void ChartGridWidget::doPaint( bool eraseBackground, const wxRect* rect )
{
	assert( painter );
	int w, h;
	getWidgetSize( &w, &h  );

	if ( chartprops->isVedic() )
	{
		unsigned int nb = 0;
		int xstep = w / cols;
		int ystep = h / rows;
		for ( int row = 0; row < rows && nb < vediccharts.size(); row++ )
		{
			for ( int col = 0; col < cols; col++ )
			{
				assert( nb < vediccharts.size());
				assert( vediccharts[nb] );
				vediccharts[nb]->OnDataChanged();
				vediccharts[nb]->paint( painter, MyRect( col * xstep, row * ystep, xstep, ystep ));
				nb++;
			}
		}
	}
	else
	{
		assert( wchart );
		wchart->paint( painter, MyRect( 0, 0, w, h ));
	}
}

/*****************************************************
**
**   ShadVargaChartGridWidget   ---   Constructor
**
******************************************************/
ShadVargaChartGridWidget::ShadVargaChartGridWidget( wxWindow *parent, ChartProperties *chartprops, const Horoscope *h1, const Horoscope *h2 )
		: ChartGridWidget( parent, chartprops, 2, 3 )
{
	if ( h1 ) setCharts( h1, h2 );
}

/*****************************************************
**
**   ShadVargaChartGridWidget   ---   setCharts
**
******************************************************/
void ShadVargaChartGridWidget::setCharts( const Horoscope *h1, const Horoscope *h2 )
{
	deleteVargaCharts();
	if ( h1 )
	{
		addVedicVargaChart( h1, h2, VARGA_RASI );
		addVedicVargaChart( h1, h2, VARGA_HORA );
		addVedicVargaChart( h1, h2, VARGA_DREKKANA );
		addVedicVargaChart( h1, h2, VARGA_NAVAMSA );
		addVedicVargaChart( h1, h2, VARGA_DVADASAMSA );
		addVedicVargaChart( h1, h2, VARGA_TRIMSAMSA );
	}
}

/*****************************************************
**
**   VargaTab2ChartGridWidget   ---   Constructor
**
******************************************************/
VargaTab2ChartGridWidget::VargaTab2ChartGridWidget( wxWindow *parent, ChartProperties *chartprops, const Horoscope *h1, const Horoscope *h2 )
		: ChartGridWidget( parent, chartprops, 2, 3 )
{
	if ( h1 ) setCharts( h1, h2 );
}

/*****************************************************
**
**   VargaTab2ChartGridWidget   ---   setCharts
**
******************************************************/
void VargaTab2ChartGridWidget::setCharts( const Horoscope *h1, const Horoscope *h2 )
{
	deleteVargaCharts();
	if ( h1 )
	{
		addVedicVargaChart( h1, h2, VARGA_CHATURTAMSA );
		addVedicVargaChart( h1, h2, VARGA_SAPTAMAMSA );
		addVedicVargaChart( h1, h2, VARGA_DASAMSA );
		addVedicVargaChart( h1, h2, VARGA_SHODASAMSA );
		addVedicVargaChart( h1, h2, VARGA_VIMSAMSA );
		addVedicVargaChart( h1, h2, VARGA_SIDDHAMSA );
	}
}

/*****************************************************
**
**   VargaTab3ChartGridWidget   ---   Constructor
**
******************************************************/
VargaTab3ChartGridWidget::VargaTab3ChartGridWidget( wxWindow *parent, ChartProperties *chartprops, const Horoscope *h1, const Horoscope *h2 )
		: ChartGridWidget( parent, chartprops, 2, 3 )
{
	if ( h1 ) setCharts( h1, h2 );
}

/*****************************************************
**
**   VargaTab3ChartGridWidget   ---   setCharts
**
******************************************************/
void VargaTab3ChartGridWidget::setCharts( const Horoscope *h1, const Horoscope *h2 )
{
	deleteVargaCharts();
	if ( h1 )
	{
		addVedicVargaChart( h1, h2, VARGA_BHAMSA );
		addVedicVargaChart( h1, h2, VARGA_CHATVARIMSAMSA );
		addVedicVargaChart( h1, h2, VARGA_AKSHAVEDAMSA );
		addVedicVargaChart( h1, h2, VARGA_SHASTIAMSA );
		addVedicVargaChart( h1, h2, VARGA_ASHTOTTARAMSA );
		addVedicVargaChart( h1, h2, VARGA_DVADASDVADASAMSA );
	}
}

/*****************************************************
**
**   DefaultEwChartGridWidget   ---   Constructor
**
******************************************************/
DefaultEwChartGridWidget::DefaultEwChartGridWidget( wxWindow *parent, ChartProperties *chartprops, Horoscope *h, const wxPoint& pos, const wxSize& size )
		: ChartGridWidget( parent, chartprops, 2, 2, pos, size )
{
	addVedicVargaChart( h, VARGA_RASI );
	addVedicVargaChart( h, VARGA_BHAVA );
	addVedicVargaChart( h, VARGA_NAVAMSA );
	addVedicVargaChart( h, VARGA_TRIMSAMSA );
}



