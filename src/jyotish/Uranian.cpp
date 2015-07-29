/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/Uranian.cpp
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
#pragma implementation "Uranian.h"
#endif

#include "Uranian.h"

#include "astrobase.h"
#include "Calculator.h"
#include "ChartProperties.h"
#include "Conf.h"
#include "constants.h"
#include "Horoscope.h"
#include "Lang.h"
#include "mathbase.h"
#include "Transit.h"
#include "UranianHelper.h"
#include "Writer.h"

#include <math.h>

extern Config *config;

/*****************************************************
**
**   UranianExpert   ---   Constructor
**
******************************************************/
UranianExpert::UranianExpert( const ChartProperties *chartprops, Writer *writer, const UranianConfig &cfg )
		: chartprops( chartprops ),
		writer( writer ),
		cfg( cfg )
{
	helper = new UranianHelper( this, chartprops, writer, &cfg );
	calculator = CalculatorFactory().getCalculator();
	show_header = config->showTextViewHeaders;
}

/*****************************************************
**
**   UranianExpert   ---   Destructor
**
******************************************************/
UranianExpert::~UranianExpert()
{
	delete helper;
}

/*****************************************************
**
**   UranianExpert   ---   writeTransitAnalysis
**
******************************************************/
void UranianExpert::writeTransitAnalysis( const Horoscope *h1, const Horoscope *h2 )
{
	calculateEvents( h1, h2 );
	helper->writeTransitAnalysis( h1, h2 );
}

/*****************************************************
**
**   UranianExpert   ---   writeRadixAnalysis
**
******************************************************/
void UranianExpert::writeRadixAnalysis( const Horoscope *h )
{
	Lang lang;
	wxString s;
	if ( show_header )
	{
		writer->writeHeader1( _( "Midpoint Summary" ));
		s << _( "Circle Type" ) << wxT( ": " ) << lang.getCircleName( cfg.circle ) << writer->getDegreeSymbol();
		writer->writeLine( s );
		s.Clear();
		s	<< _( "Orbis" ) << wxT( ": " ) << 60.0 * cfg.orbis << wxT( " " ) << _( "Minutes" );
		writer->writeParagraph( s );
	}

	calculateEvents( h, h );
	helper->writeRadixAnalysis( h, h );
}

/*****************************************************
**
**   UranianExpert   ---   writePartnerAnalysis
**
******************************************************/
void UranianExpert::writePartnerAnalysis( const Horoscope *h1, const Horoscope *h2 )
{
	wxString s;

	if ( show_header ) writer->writeHeader1( _( "Uranian Partner Analysis" ));

	s.Clear();
	s << _( "Partner1" ) << wxT( ": " ) << h1->getHName();
	writer->writeHeader1( s );
	calculateEvents( h1, h2 );
	helper->writePartnerAnalysis( h1, h2 );

	s.Clear();
	s << _( "Partner2" ) << wxT( ": " ) << h2->getHName();
	writer->writeHeader1( s );
	calculateEvents( h2, h1 );
	helper->writePartnerAnalysis( h2, h1 );
}

/*****************************************************
**
**   UranianExpert   ---   calculateMidpointMatch
**
******************************************************/
int UranianExpert::calculateMidpointMatch( const double &len1, const double &len2, double *dist )
{
	double circledeg = 360.0;

	for ( int i = CIRCLE_TYPE_360; i <= cfg.circle; i++ )
	{
		*dist = a_red( len1 - len2, circledeg );
		//printf( "UranianExpert::calculateMidpointMatch len1 %f len2 %f dist %f\n", len1, len2, *dist );
		if ( *dist > .5 * circledeg ) *dist = *dist - circledeg;

		if ( i <= CIRCLE_TYPE_180 && *dist == 0 ) return CIRCLE_TYPE_NONE;

		if ( fabs ( *dist ) <= cfg.orbis )
		{
			return i;
		}
		circledeg /= 2.0;
	}
	return CIRCLE_TYPE_NONE;
}

/*****************************************************
**
**   UranianExpert   ---   calculateEvents
**
******************************************************/
void UranianExpert::calculateEvents( const Horoscope *h1, const Horoscope *h2 )
{
	int type = CIRCLE_TYPE_NONE;
	double dist, midpoint;

	midpoint_events.clear();
	conjunction_events.clear();
	midpoint_values.clear();
	vector<int> l = chartprops->getPlanetList( OBJECTS_INCLUDE_ARIES );

	for ( unsigned int i = 0; i < l.size(); i++ )
	{
		int p1 = l[i];
		for ( unsigned int j = i + 1; j < l.size(); j++ )
		{
			int p2 = l[j];
			midpoint = red_deg( ( h2->getLength( p1, chartprops->isVedic() ) + h2->getLength( p2, chartprops->isVedic() ) ) * .5 );
			if ( planetDistance( midpoint, h2->getLength( p1, chartprops->isVedic() )) > 90 )
			{
				midpoint = red_deg( midpoint + 180 );
			}

			midpoint_values.push_back( UranianMidpointValue( p1, p2, midpoint ));
			for ( unsigned int k = 0; k < l.size(); k++ )
			{
				if ( k == i || k == j ) continue;
				int p3 = l[k];

				// loop if filter condition is not satisfied
				if ( cfg.filter >= 0 && ( p1 != cfg.filter && p2 != cfg.filter && p3 != cfg.filter )) continue;

				type = calculateMidpointMatch( midpoint, h1->getLength( p3, chartprops->isVedic() ), &dist );
				if ( type != CIRCLE_TYPE_NONE ) midpoint_events.push_back( UranianMidpointEvent( p3, p1, p2, midpoint, dist, type ));

			} // for k
		} // for j
	}  // for i
	midpoint_events.sort( UranianMidpointEventSorter( cfg.order ) );

	for ( unsigned int i = 0; i < chartprops->getPlanetList().size(); i++ )
	{
		int p1 = chartprops->getPlanetList()[i];
		for ( unsigned int j = 0; j < l.size(); j++ )
		{
			//if ( h1 == h2 && j <= i ) continue;
			int p2 = l[j];

			if ( cfg.filter >= 0 && ( p1 != cfg.filter && p2 != cfg.filter )) continue;

			type = calculateMidpointMatch( h1->getLength( p1, chartprops->isVedic() ), h2->getLength( p2, chartprops->isVedic() ), &dist );
			if ( type != CIRCLE_TYPE_NONE ) conjunction_events.push_back( UranianConjunctionEvent( p1, p2, dist, type ));

		}
	}
	conjunction_events.sort( UranianConjunctionEventSorter( cfg.order ));
}

/*****************************************************
**
**   UranianExpert   ---   calcEventDateForSolarArcEvent
**
******************************************************/
double UranianExpert::calcEventDateForSolarArcEvent( DataSet *basedate, double aprox_jd, const double &sun_radixlen,
        const double &radixpos, const double &eventpos, const int &eventtype )
{
	double sunlen, dummy, bogen, transitlen;
	double dist = 1;
	int type;
	int count = 0;
	DataSet d( *basedate );

	while ( fabs( dist ) > .000001 )
	{
		d.setDate( basedate->getJD() + ( aprox_jd - basedate->getJD() ) / cfg.year_length );
		calculator->calcPosition( &d, OSUN, sunlen, dummy, true, chartprops->isVedic() );
		bogen = red_deg( sunlen - sun_radixlen );
		transitlen = red_deg( radixpos + bogen );

		type = calculateMidpointMatch( eventpos, transitlen, &dist );
		if ( type != eventtype )
		{
			printf( "Error in UranianExpert::calcEventDateForSolarArcEvent, wrong event %d\n", type );
			return 0;
		}
		aprox_jd += dist * cfg.year_length;

		if ( ++count > 10 )
		{
			printf( "Error in UranianExpert::calcEventDateForSolarArcEvent, too many loops\n" );
			return 0;
		}
	}
	return aprox_jd;
}

/*****************************************************
**
**   UranianExpert   ---   writeYearlyPreviewAnalysis
**
******************************************************/
void UranianExpert::writeYearlyPreviewAnalysis( const Horoscope *h, const int &year )
{
	Formatter *formatter = Formatter::get();
	int theyear, dummy1, dummy2;
	list<UranianConjunctionEvent>::iterator c_iter, cd_iter;
	list<UranianMidpointEvent>::iterator m_iter, md_iter;

	writer->writeHeader1( _( "Yearly Preview for Solar Chart" ));
	TransitHoroscope htransit;
	htransit.setLocation( *h->getDataSet()->getLocation() );

	// calculate events for 1st Jul
	double mid_jd = calculator->calc_jd( year, 7, 1, 0 );
	htransit.calcTransitPositions( h, mid_jd, chartprops->isVedic(), cfg.year_length, TRANSIT_MODE_SOLAR_ARC );

	// set orbis
	cfg.orbis = .7;
	// calculate events for 1st jul.
	calculateEvents( &htransit, h );

	for ( c_iter = conjunction_events.begin(); c_iter != conjunction_events.end(); c_iter++ )
	{
		(*c_iter).jd = calcEventDateForSolarArcEvent(
		                   h->getDataSet(),
		                   mid_jd,
		                   h->getLength( OSUN, chartprops->isVedic()),
		                   h->getLength( (*c_iter).p1, chartprops->isVedic()),
		                   h->getLength( (*c_iter).p2, chartprops->isVedic()),
		                   (*c_iter).type
		               );
	}

	// delete events with date outside year span
	c_iter = conjunction_events.begin();
	while ( c_iter != conjunction_events.end()) // delete entries with wrong year
	{
		formatter->getDateIntsFromJD( (*c_iter).jd, dummy1, dummy2, theyear );
		if ( theyear != year )
		{
			cd_iter = c_iter; // -> don't delete the one you want to increment.
			c_iter++;
			conjunction_events.erase( cd_iter );
		}
		else c_iter++;
	}

	conjunction_events.sort( UranianConjunctionEventSorter( ASPECT_SORT_JD ));
	helper->writeConjunctions( h, &htransit, true );

	for ( m_iter = midpoint_events.begin(); m_iter != midpoint_events.end(); m_iter++ )
	{
		(*m_iter).jd = calcEventDateForSolarArcEvent( h->getDataSet(), mid_jd,
		               h->getLength( OSUN, chartprops->isVedic()),
		               h->getLength( (*m_iter).p, chartprops->isVedic()),
		               (*m_iter).midpoint, (*m_iter).type );
	}

	m_iter = midpoint_events.begin();
	while ( m_iter != midpoint_events.end())
	{
		formatter->getDateIntsFromJD( (*m_iter).jd, dummy1, dummy2, theyear );
		if ( theyear != year )
		{
			md_iter = m_iter; // -> don't delete the one you want to increment.
			m_iter++;
			midpoint_events.erase( md_iter );
		}
		else m_iter++;
	}

	midpoint_events.sort( UranianMidpointEventSorter( ASPECT_SORT_JD ));
	helper->writeMidpointEvents( h, &htransit, true );
	helper->writeMidpointTextAnalysis( h, &htransit, true );
}

