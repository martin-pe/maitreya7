/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/Transit.cpp
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
#pragma implementation "Transit.h"
#endif

#include "Transit.h"

#include "ArabicParts.h"
#include "Aspect.h"
#include "astrobase.h"
#include "constants.h"
#include "Calculator.h"
#include "ChartProperties.h"
#include "Conf.h"
#include "Horoscope.h"
#include "Lang.h"
#include "mathbase.h"
#include "Planet.h"
#include "Varga.h"
#include "Uranian.h"
#include "Writer.h"

extern Config *config;

IMPLEMENT_CLASS( TransitHoroscope, Horoscope )

/*****************************************************
**
**   TransitHoroscope   ---   Constructor
**
******************************************************/
TransitHoroscope::TransitHoroscope()
		: Horoscope()
{
}

/*****************************************************
**
**   TransitHoroscope   ---   calcTransitPositions
**
******************************************************/
double TransitHoroscope::calcTransitPositions( const Horoscope *hbase, const double &transitJD,
        const bool &vedic, const double &yl, const int &mode )
{
	double newjd = 0, posdelta = 0;
	double len, dummy;
	int i;
	Calculator *calculator = CalculatorFactory().getCalculator();

	calcdate = transitJD;
	DataSet ds( *getDataSet());
	newjd = hbase->getJD() + ( transitJD - hbase->getJD() ) / yl;

	switch ( mode )
	{
	case TRANSIT_MODE_TRANSIT:
		setDate( transitJD );
		break;
	case TRANSIT_MODE_SOLAR_ARC:
	case TRANSIT_MODE_CONSTANT_ARC:
	case TRANSIT_MODE_LUNAR_ARC:
		setDate( hbase->getJD() );
		break;
	case TRANSIT_MODE_DIRECTION:
		setDate( newjd );
		break;
	default:
		assert( false );
		break;
	}
	update();

	// TODO: date before birth
	// TODO: big Lunar arc can be > 360
	if ( mode == TRANSIT_MODE_SOLAR_ARC )
	{
		ds.setDate( newjd );
		calculator->calcPosition( &ds, OSUN, len, dummy, true, vedic );
		posdelta = red_deg( len - getLength( OSUN, vedic ));
	}
	else if ( mode == TRANSIT_MODE_CONSTANT_ARC )
	{
		posdelta = ( transitJD - hbase->getJD() ) / yl;
	}
	else if ( mode == TRANSIT_MODE_LUNAR_ARC )
	{
		ds.setDate( newjd );
		calculator->calcPosition( &ds, OMOON, len, dummy, true, vedic );
		posdelta = red_deg( len - getLength( OMOON, vedic ));
	}
	if ( posdelta != 0 )
	{
		for ( i = FIRST_EPHEM_OBJECT; i <= LAST_EPHEM_OBJECT; i++ )
		{
			object_len[i] = red_deg( object_len[i] + posdelta );

			// no retrogression for most modes
			if ( mode != TRANSIT_MODE_TRANSIT ) object_speed[i] = 1;
		}

		for ( i = 0; i <= NUM_LAGNA; i++ ) lagna_len[i] = red_deg( lagna_len[ i ] + posdelta );
		for ( i = 0; i <= NUM_UPA; i++ ) upagraha_len[i] = red_deg( upagraha_len[ i ] + posdelta );

		for ( i = HOUSE1; i <= HOUSE12; i++ )
		{
			whousecusp[i] = red_deg( whousecusp[i] + posdelta );
			ihousecusp[i] = red_deg( ihousecusp[i] + posdelta );
			ihousesandhi[i] = red_deg( ihousesandhi[i] + posdelta );
		}
		arieslen = posdelta;
	}
	for ( i = 0; i < MAX_EPHEM_OBJECTS; i++ )
	{
		ephem_planets[i]->update( getVedicLength( i ));
	}
	arabic_expert->update();

	updatePositionArrays();
	updateObjectPositions();
	return posdelta;
}

/*****************************************************
**
**   TransitExpert   ---   Constructor
**
******************************************************/
TransitExpert::TransitExpert( Horoscope *h, const ChartProperties *chartprops )
		: Expert( h ),
		chartprops( chartprops )
{
	transitmode = sortorder = 0;
	filter = -1;

	// TODO ist das so richtig?
	yearlength = calcYearLength( chartprops->isVedic() ? config->vYearLength : config->wYearLength, chartprops->isVedic() );
	htransit = new TransitHoroscope();
	transitJD = JDate::getCurrentJD();
	tzoffset = 0;
	aexpert = new AspectExpert( h );
}

/*****************************************************
**
**   TransitExpert   ---   Destructor
**
******************************************************/
TransitExpert::~TransitExpert()
{
	delete htransit;
}

/*****************************************************
**
**   TransitExpert   ---   update
**
******************************************************/
void TransitExpert::update()
{
	posdelta = htransit->calcTransitPositions( horoscope, transitJD, chartprops->isVedic(), yearlength, transitmode );
}

/*****************************************************
**
**   TransitExpert   ---   setTransitDate
**
******************************************************/
void TransitExpert::setTransitDate( const double &jd, const double tzoffset )
{
	transitJD = jd - tzoffset / 24.0;
	this->tzoffset = tzoffset;
}

/*****************************************************
**
**   TransitExpert   ---   writeTransitData
**
******************************************************/
void TransitExpert::writeTransitData( Writer *writer, const bool &uranian )
{
	wxString s;
	Formatter *formatter = Formatter::get();
	wxString gradkreis_name[7] = { wxT( "360" ), wxT( "180" ), wxT( "90" ), wxT( "45" ), wxT( "22.5" ),
		wxT( "11.25" ), wxT( "5.625" ) };

	switch ( transitmode )
	{
	case TRANSIT_MODE_TRANSIT:
		writer->writeHeader1( _( "Transit" ));
		break;
	case TRANSIT_MODE_SOLAR_ARC:
		writer->writeHeader1( _( "Solar Arc" ));
		break;
	case TRANSIT_MODE_DIRECTION:
		writer->writeHeader1( _( "Directions" ));
		break;
	case TRANSIT_MODE_CONSTANT_ARC:
		writer->writeHeader1( _( "Constant Arc" ));
		break;
	case TRANSIT_MODE_LUNAR_ARC:
		writer->writeHeader1( _( "Lunar Arc" ));
		break;
	default:
		assert( false );
		break;
	}

	// Date and time
	if ( transitmode == TRANSIT_MODE_TRANSIT || transitmode == TRANSIT_MODE_DIRECTION )
	{
		s.Printf( wxT( "%s: %s (%c%1.1f)" ),
			_( "Event date" ),
			(const wxChar*)formatter->getFullDateStringFromJD( htransit->getJD() + tzoffset/24.0 ),
			tzoffset < 0 ? '-' : '+', tzoffset );
		writer->writeLine( s );
	}

	// Location
	s.Printf( wxT( "%s: %s" ),  _( "Location" ), (const wxChar*)htransit->getLocation()->getLocName() );
	writer->writeLine( s );

	// Year Length
	if ( transitmode != TRANSIT_MODE_TRANSIT )
	{
		s.Clear();
		s << _( "Year Length" ) << wxT( ": " ) << yearlength;
		writer->writeLine( s );
	}
	s.Clear();
	if ( posdelta != 0 )
	{
		s << _( "Arc" ) << wxT( ": " ) << formatter->getLenFormatted( posdelta );
		writer->writeLine( s );
	}
	s.Clear();

	if ( uranian == true )
	{
		s.Printf( wxT( "%s: %d %s" ),  _( "Orbis" ), (int)(60 * orbis), wxT( "min" ));
		writer->writeLine( s );
		s.Clear();
		s << _( "Circle" ) << wxT( ": " ) << gradkreis_name[degrees] << wxT( " " ) << _(  "degrees" );
		writer->writeLine( s );
		s.Clear();
	}
	writer->writeLine( wxEmptyString );

	if ( uranian == true ) writeUranianPlanetList( writer, chartprops );
	else
	{
		writeStandardPlanetList( writer, chartprops );
	}
}

/*****************************************************
**
**   TransitExpert   ---   writeUranianPlanetList
**
******************************************************/
void TransitExpert::writeUranianPlanetList( Writer *writer, const ChartProperties *chartprops )
{
	UranianConfig cfg;
	cfg.order = sortorder;
	cfg.circle = degrees;
	cfg.orbis = orbis;
	cfg.filter = filter;
	UranianExpert expert( chartprops, writer, cfg );
	expert.writeTransitAnalysis( htransit, horoscope );
}

/*****************************************************
**
**   TransitExpert   ---   writeStandardPlanetList
**
******************************************************/
void TransitExpert::writeStandardPlanetList( Writer *writer, const ChartProperties *chartprops )
{
	int line = 1, i;
	vector<int> obs = chartprops->getPlanetList( 	transitmode == TRANSIT_MODE_TRANSIT ? 0 : OBJECTS_INCLUDE_ARIES );
	ObjectPosition pos;
	int numrows = obs.size() + 1;

	Table table( 2, numrows );
	table.setHeader( 0, _( "Planet" ) );
	table.setHeader( 1, _( "Length" ) );
	for ( unsigned int p = 0; p < obs.size(); p++ )
	{
		i = obs[p];
		pos = htransit->getObjectPosition( i, chartprops->isVedic() );
		table.setEntry( 0, line, writer->getObjectName( i, TMEDIUM ));
		table.setEntry( 1, line, writer->getPosFormatted( pos.length, pos.retro ));
		line++;
	}
	writer->writeTable( table );
}

