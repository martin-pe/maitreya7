/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/VedicChart.cpp
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
#pragma implementation "VedicChart.h"
#endif

#include "VedicChart.h"

#include "ChartProperties.h"
#include "Conf.h"
#include "Jaimini.h"
#include "Varga.h"
#include "VargaHoroscope.h"
#include "Writer.h"

extern Config *config;

/*****************************************************
**
**   AshtakaVargaChart   ---   Constructor
**
******************************************************/
AshtakaVargaChart::AshtakaVargaChart( const ChartProperties *chartprops )
		: VedicRasiChart( chartprops )
{
	extrazoom *= 1.5;
}

/*****************************************************
**
**   AshtakaVargaChart   ---   setRasiValue
**
******************************************************/
void AshtakaVargaChart::setRasiValue( const int &r, const int &value )
{
	assert( r >= ARIES && r <= PISCES );
	rasi_values[r] = value;
}

/*****************************************************
**
**   AshtakaVargaChart   ---   setRasiValues
**
******************************************************/
void AshtakaVargaChart::setRasiValues( const int r[] )
{
	for ( int i = 0; i < 12; i++ ) rasi_values[i] = r[i];
}

/*****************************************************
**
**   AshtakaVargaChart   ---   writeChartContents
**
******************************************************/
void AshtakaVargaChart::writeChartContents( const int &chart_id )
{
	wxString s;
	wxChar avitem = config->vAshtakavargaNumberMode == 1 ? 'I' : 'o';

	for ( int i = ARIES; i <= PISCES; i++ )
	{
		contents[i].clear();
		s.Clear();
		if ( config->vAshtakavargaNumberMode != 0 )
		{
			if ( graphicSupport )
				for ( int j = 0; j < rasi_values[i]; j++ )
				{
					if ( j == 5 )
					{
						contents[i].textitems.push_back( ChartTextItem( s, s ));
						s.Clear();
					}
					s << avitem;
				}
			else s.Printf( wxT( "%d" ), rasi_values[i] );
		}
		else
		{
			s.Printf( wxT( "%d" ), rasi_values[i] );
		}
		contents[i].textitems.push_back( ChartTextItem( s, s ));
	}
}

/*****************************************************
**
**   VedicVargaChart   ---   Constructor
**
******************************************************/
VedicVargaChart::VedicVargaChart( const VargaHoroscope *vh1, const ChartProperties *chartprops )
		: VedicRasiChart( chartprops, 1 ),
		vh1( vh1 ),
		vh2( 0 )
{
	assert( vh1 );
}

/*****************************************************
**
**   VedicVargaChart   ---   Constructor
**
******************************************************/
VedicVargaChart::VedicVargaChart( const Horoscope *h, const int &varga, const ChartProperties *chartprops )
		: VedicRasiChart( chartprops, 1 ),
		vh1( new VargaHoroscope( h, varga )),
		vh2( 0 )
{
	assert( vh1 );
}

/*****************************************************
**
**   VedicVargaChart   ---   Constructor
**
******************************************************/
VedicVargaChart::VedicVargaChart( const VargaHoroscope *vh1, const VargaHoroscope *vh2, const ChartProperties *chartprops )
		: VedicRasiChart( chartprops, 2 ),
		vh1( vh1 ),
		vh2( vh2 )
{
	assert( vh1 );
	assert( vh2 );
}

/*****************************************************
**
**   VedicVargaChart   ---   Constructor
**
******************************************************/
VedicVargaChart::VedicVargaChart( const Horoscope *h1, const int &varga1, const Horoscope *h2, const int &varga2, const ChartProperties *chartprops )
		: VedicRasiChart( chartprops, 2 ),
		vh1( new VargaHoroscope( h1, varga1 )),
		vh2( new VargaHoroscope( h2, varga2 ))
{
	assert( vh1 );
	assert( vh2 );
}

/*****************************************************
**
**   VedicVargaChart   ---   Destructor
**
******************************************************/
VedicVargaChart::~VedicVargaChart()
{
	assert( vh1 );
	if ( vh1 ) delete vh1;
	if ( vh2 ) delete vh2;
}

/*****************************************************
**
**   VedicVargaChart   ---   getCenterString
**
******************************************************/
wxString VedicVargaChart::getCenterString()
{
	if ( ! vh1 )
	{
		return wxT( "Error in VedicVargaChart::getCenterString: Vargachart not set" );
	}

	VargaExpert expert;
	wxString s;

	switch( chartprops->getVedicGraphicStyle().centerInfoType )
	{
		case VGRAPHIC_CHART_CENTER_VARGA:
			if ( vh1->getDivision() ==  NB_VARGAS-1 ) s = _( "Bhava" );
			else s = expert.getVargaName( vh1->getDivision() );
		break;
		case VGRAPHIC_CHART_CENTER_DIVISION:
			if ( vh1->getDivision() ==  NB_VARGAS-1 ) s = _( "Bhava" );
			else s = wxString::Format( _( "D-%d" ), expert.getVargaDivision( vh1->getDivision() ));
		break;
		case VGRAPHIC_CHART_CENTER_BOTH:
			if ( vh1->getDivision() ==  NB_VARGAS-1 ) s = _( "Bhava" );
			else s = wxString::Format( _( "%s (D-%d)" ), expert.getVargaName( vh1->getDivision() ), expert.getVargaDivision( vh1->getDivision() ));
		break;
		case VGRAPHIC_CHART_CENTER_NOTHING:
		default:
		break;
	}
	return s;
}

/*****************************************************
**
**   VedicVargaChart   ---   getPlanetIndex
**
******************************************************/
int VedicVargaChart::getPlanetField( const int &planet, const int &chart_id )
{
	const VargaHoroscope *v = getVargaHoroscope( chart_id );
	if ( v ) return v->getRasi( planet );
	else
	{
		printf( "Warning: Vargachart is NULL in VedicVargaChart::getPlanetIndex, returning ARIES\n" );
		return ARIES;
	}
}

/*****************************************************
**
**   VedicVargaChart   ---   getPlanetRetro
**
******************************************************/
bool VedicVargaChart::getPlanetRetro( const int &planet, const int &chart_id )
{
	const VargaHoroscope *v = getVargaHoroscope( chart_id );
	if ( v ) return v->isRetrograde( planet );
	else
	{
		printf( "Warning: Vargachart is NULL in VedicVargaChart::getPlanetRetro, returning FALSE\n" );
		return false;
	}
}

/*****************************************************
**
**   VedicVargaChart   ---   writeChartContents
**
******************************************************/
void VedicVargaChart::writeChartContents( const int &chart_id )
{
	// normal astrological objects from superclass
	const VargaHoroscope *v = getVargaHoroscope( chart_id );
	if ( ! v )
	{
		printf( "Warning: Vargachart is NULL in VedicVargaChart::writeChartContents, returning\n" );
		return;
	}
	BasicVedicChart::writeChartContents( chart_id );

	int i, rasi;
	wxString lname, sname;
	vector<ChartContents> &c = getChartContents( chart_id );

	if ( chart_id == 0 ) ascendant = getPlanetField( OASCENDANT, 0 );

	if ( chartprops->getVedicGraphicStyle().showArudhas )
	{
		JaiminiExpert expert( getVargaHoroscope( chart_id ));
		for ( i = 0; i < 12; i++ )
		{
			rasi = expert.calcPada( i );
			c[rasi].planets.Add( i );
			if ( i == 0 )
			{
				sname = _( "AL" );
				lname = _( "Arudha L" );
			}
			else if ( i == 11 )
			{
				sname = _( "UL" );
				lname = _( "Upapada L" );
			}
			else
			{
				sname.Printf( _( "A%d" ), i+1 );
				lname = sname;
			}
			c[rasi].textitems.push_back( ChartTextItem( lname, sname, false ));
		}
	}
}


