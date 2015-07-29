/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/Hora.cpp
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
#pragma implementation "Hora.h"
#endif

#include "Hora.h"

#include "Calculator.h"
#include "Conf.h"
#include "DataSet.h"
#include "Lang.h"
#include "mathbase.h"
#include "Writer.h"

#include <wx/string.h>

extern Config *config;

/*****************************************************
**
**   HoraExpert   ---   Constructor
**
******************************************************/
HoraExpert::HoraExpert()
{
	location = &config->defaultLocation;
}

/*****************************************************
**
**   HoraExpert   ---   setLocation
**
******************************************************/
void HoraExpert::setLocation( Location *loc )
{
	location = loc;
}

//#define HORA_DEB
/*****************************************************
**
**   HoraExpert   ---   update
**
******************************************************/
void HoraExpert::update( const double &jd )
{
	const int lordseq[7] = { OSATURN, OJUPITER, OMARS, OSUN, OVENUS, OMERCURY, OMOON };
	const int rev_lordseq[7] = { 3, 6, 2, 5, 1, 4, 0 };

	Calculator *calculator = CalculatorFactory().getCalculator();
	Formatter *formatter = Formatter::get();

	int i, weekday, day, month, year;

	double sunrise = calculator->calcNextSolarEvent( SOLAR_EVENT_SUNRISE, jd, location->getLatitude(), location->getLongitude());
	double sunset = calculator->calcNextSolarEvent( SOLAR_EVENT_SUNSET, sunrise, location->getLatitude(), location->getLongitude());
	double sunrise_next = calculator->calcNextSolarEvent( SOLAR_EVENT_SUNRISE, sunset, location->getLatitude(), location->getLongitude());

	weekday = getWeekDay( sunrise );
	dinaLord = lordseq[ ( rev_lordseq[weekday] ) % 7 ];



#ifdef HORA_DEB
	wxString s;
	printf( "##################  DEBUG ######################\n" );
	location->dump( s );
	s = formatter->getFullDateStringFromJD( jd );
	printf( "full date: %s\n", str2char( s ));
	printf( "jd: %s\n", str2char( formatter->getFullDateStringFromJD( jd )));
	printf( "sunrise: %s\n", str2char( formatter->getFullDateStringFromJD( sunrise )));
	printf( "sunset: %s\n", str2char( formatter->getFullDateStringFromJD( sunset )));
	printf( "sunrise NEXT: %s\n", str2char( formatter->getFullDateStringFromJD( sunrise_next )));
	printf( "WEEKDAY %d\n", weekday );
	printf( "dinalord %d\n", dinaLord );
	printf( "##################  END DEBUG  ######################\n" );
#endif

	for ( i = 0; i < 24; i++ )
	{
		horaLord[i] = lordseq[ ( rev_lordseq[weekday] + i ) % 7 ];
	}

	double daydur = sunset - sunrise;
	double daystep = daydur / 12;

	double nightdur = sunrise_next - sunset;
	double nightstep = nightdur / 12;

	currentHoraLord = horaLord[0];
	horaStart[24] = sunrise_next;

	for ( i = 0; i < 12; i++ )
	{
		horaStart[i] = sunrise + i * daystep;
		if ( horaStart[i] < jd ) currentHoraLord = horaLord[i];
	}
	for ( i = 0; i < 12; i++ )
	{
		horaStart[i+12] = sunset + i * nightstep;
		if ( horaStart[i+12] < jd ) currentHoraLord = horaLord[i+12];
	}

	// calculate lords of month and year
	formatter->getDateIntsFromJD( sunrise, day, month, year );

	// lord of 1st day of year
	weekday = getWeekDay( calculator->calc_jd( year, 1, 1, 12 ));
	varshaLord = lordseq[ ( rev_lordseq[weekday] ) % 7 ];

	// lord of 1st day ov month
	weekday = getWeekDay( calculator->calc_jd( year, month, 1, 12 ));
	masaLord = lordseq[ ( rev_lordseq[weekday] ) % 7 ];
}

/*****************************************************
**
**   HoraExpert   ---   getHoraLord
**
******************************************************/
int HoraExpert::getHoraLord( const int &i ) const
{
	assert( i >= 0 && i < 24 );
	return horaLord[i];
}

/*****************************************************
**
**   HoraExpert   ---   getHoraStart
**
******************************************************/
double HoraExpert::getHoraStart( const int &i ) const
{
	assert( i >= 0 && i < 25 );
	return horaStart[i];
}

/*****************************************************
**
**   HoraExpert   ---   write
**
******************************************************/
void HoraExpert::write( Writer *writer, const bool isLocaltime )
{
	Formatter *formatter = Formatter::get();
	TzUtil tzu;
	wxString s;
	TzFormattedDate fd;

	double corr = ( location->getTimeZone() + location->getDST()) / 24;

	if ( config->showTextViewHeaders ) writer->writeHeader1( _( "Hora" ));

	Table table( 7, 13 );
	table.setHeader( 0,  _( "Begin" ));
	table.setHeader( 1,  _( "Lord" ));
	table.setHeader( 2,  _( "End" ));
	table.setHeader( 3,  wxEmptyString );
	table.setHeader( 4,  _( "Begin" ));
	table.setHeader( 5,  _( "Lord" ));
	table.setHeader( 6,  _( "End" ));
	int line = 1;
	int colskip = 0;
	for ( int i = 0; i < 24; i++ )
	{
		if ( i == 12 )
		{
			colskip = 4;
			line = 1;
		}

		table.setEntry( 3, line, wxEmptyString );
		fd = tzu.getDateFormatted( horaStart[i] + corr, isLocaltime );
		table.setEntry( colskip, line, fd.timeFormatted );

		table.setEntry( 1+colskip, line, writer->getObjectName( horaLord[i], TLARGE ));

		fd = tzu.getDateFormatted( horaStart[i+1] + corr, isLocaltime );
		table.setEntry( 2+colskip, line, fd.timeFormatted );
		line++;
	}
	writer->writeTable( table );

	writer->writeLine( wxString::Format( wxT( "%s: %s" ), _( "Lord of the day" ),
		(const wxChar*)writer->getObjectName( getDinaLord(), TLARGE )));
	writer->writeLine( wxString::Format( wxT( "%s: %s" ), _( "Lord of the month" ),
		(const wxChar*)writer->getObjectName( getMasaLord(), TLARGE )));
	writer->writeLine( wxString::Format( wxT( "%s: %s" ), _( "Lord of the year" ),
		(const wxChar*)writer->getObjectName( getVarshaLord(), TLARGE )));
}

