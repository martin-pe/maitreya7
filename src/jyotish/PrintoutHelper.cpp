/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/PrintoutHelper.cpp
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
#pragma implementation "PrintoutHelper.h"
#endif

#include "PrintoutHelper.h"

#include <wx/log.h>
#include <wx/string.h>
#include <math.h>

#include "Ashtakavarga.h"
#include "Aspect.h"
#include "astrobase.h"
#include "constants.h"
#include "Conf.h"
#include "constants.h"
#include "Horoscope.h"
#include "Jaimini.h"
#include "Lang.h"
#include "mathbase.h"
#include "Nakshatra.h"
#include "Planet.h"
#include "PlanetList.h"
#include "Varga.h"
#include "VargaHoroscope.h"
#include "Writer.h"

extern Config *config;

/*****************************************************
**
**   PrintoutHelper   ---   Constructor
**
******************************************************/
PrintoutHelper::PrintoutHelper( const Horoscope *horoscope, Writer *ww )
		: h ( horoscope )
{
	writer = ww;
}

/*****************************************************
**
**   PrintoutHelper   ---   writeBaseData
**
******************************************************/
void PrintoutHelper::writeBaseData( Table *table )
{
	wxString s;
	int line = 1;
	double thetime;
	Formatter *f = Formatter::get();

	DataSet *ds = h->getDataSet();
	Location *loc = ds->getLocation();
	JDate *date = ds->getDate();
	table->setHeader( 0,  _( "Base Data" ), false, 1 );

	table->setEntry( 0, line,  _( "Name" ));
	table->setEntry( 1, line++, h->getHName() );

	table->setEntry( 0, line,  _( "Date" ));
	s << f->getDateStringFromJD( h->getJD() + ( loc->getTimeZone() + loc->getDST() )/24 );
	table->setEntry( 1, line++, s );

	table->setEntry( 0, line,  _( "Local Time" ));
	thetime = a_red( getTimeFromJD( date->getJD()) + loc->getTimeZone() + loc->getDST(), 24 );
	table->setEntry( 1, line++, f->getTimeFormatted( thetime ));

	table->setEntry( 0, line,  _( "Location" ));
	table->setEntry( 1, line++, loc->getLocName() );

	table->setEntry( 0, line,  _( "Longitude" ));
	table->setEntry( 1, line++, f->getLongitudeFormatted( loc->getLongitude()));

	table->setEntry( 0, line,  _( "Latitude" ));
	table->setEntry( 1, line++, f->getLatitudeFormatted( loc->getLatitude()));

	table->setEntry( 0, line,  _( "Time Zone" ));
	double tz = loc->getTimeZone();
	s.Printf( wxT( "%s %c%.1f" ), _( "UT" ), ( tz >= 0 ? '+' : '-' ), tz );
	table->setEntry( 1, line++, s );

	table->setEntry( 0, line,  _( "DST" ));
	s.Printf( wxT( "%.1f" ), loc->getDST());
	table->setEntry( 1, line++, s );

	table->setEntry( 0, line,  _( "UT" ));
	table->setEntry( 1, line++, f->getTimeFormatted( getTimeFromJD( h->getJD() )));
}

/*****************************************************
**
**   PrintoutHelper   ---   writeWesternUranian
**
******************************************************/
void PrintoutHelper::writeWesternUranian( Table *table )
{
	int i;
	wxString s;
	table->setHeader( 1,  _( "Base Data" ));
	table->setHeader( 4,  _( "Planets" ));
	table->setHeader( 9,  _( "Uranian" ));

	int line = 1;
	vector<int> obs = PlanetList().getWesternObjectList( OBJECTS_INCLUDE_OUTER );
	for ( unsigned int p = 0; p < obs.size(); p++ )
		writePlanetaryObject( table, 2, line++, obs[p], true, false );

	line = 1; // next line
	table->col_line[6] = true;
	writePlanetaryObject( table, 7, line++, OASCENDANT, false, false );
	writePlanetaryObject( table, 7, line++, OMERIDIAN, false, false );
	for (  i = OCUPIDO; i <= OPOSEIDON; i++ ) writePlanetaryObject( table, 7, line++, i, true, false );
}

/*****************************************************
**
**   PrintoutHelper   ---   writeWesternDefault
**
******************************************************/
void PrintoutHelper::writeWesternDefault( Table *table )
{
	int line = 1;
	int col = 2;
	table->setHeader( 2,  _( "Planets" ), false, 4);
	table->setHeader( 7,  _( "Planets" ), false, 4);
	table->col_line[6] = true;

	vector<int> obs = PlanetList().getWesternObjectList( OBJECTS_INCLUDE_OUTER );
	if ( config->wLunarNodeMode == LUNAR_NODE_TRUE ) obs.push_back( OTRUEDESCNODE );
	else obs.push_back( OMEANDESCNODE );

	for ( unsigned int p = 0; p < obs.size(); p++ )
	{
		if ( p == 9 )
		{
			col = 7;
			line = 1;
		}
		writePlanetaryObject( table, col, line++, obs[p], true, false );
	}
	writePlanetaryObject( table, col, line++, OASCENDANT, false, false );
	writePlanetaryObject( table, col, line++, OMERIDIAN, false, false );
	writeHouse( table, col, line++, 11, false );
	writeHouse( table, col, line++, 12, false );
	writeHouse( table, col, line++, 2, false );
	writeHouse( table, col, line++, 3, false );
}

/*****************************************************
**
**   PrintoutHelper   ---   writePlanetaryObject
**
******************************************************/
void PrintoutHelper::writePlanetaryObject( Table *table, const int& row, const int &line, const int& i, const bool& withHouse, const bool &vedic, const bool cutname )
{
	wxString s;
	Formatter *formatter = Formatter::get();
	ObjectPosition pos = h->getObjectPosition( i, vedic );

	table->setEntry( row, line, writer->getObjectName( i, TMEDIUM, true ), config->usePlanetSymbols );
	if ( pos.retro ) table->setEntry( row + 1, line, ( config->usePlanetSymbols ? wxT( "_" ) : wxT( "R" )), config->usePlanetSymbols );

	if ( config->useVedicPositions )
		table->setEntry( row + 2, line, writer->getPosFormatted( pos.length, pos.retro, DEG_PRECISION_SECOND ), false );
	else
	{
		table->setEntry( row + 2, line, formatter->getLenFormatted( getRasiLen( pos.length )), false );
		table->setEntry( row + 3, line, writer->getSignName( getRasi( pos.length ), cutname ? TSHORT : TMEDIUM ), config->useSignSymbols );
	}
	if ( withHouse )
	{
		s.Printf( wxT( "%02d" ), h->getHousePos( i, vedic ) + 1 );
		table->setEntry( row + 4, line, s );
	}
}

/*****************************************************
**
**   PrintoutHelper   ---   writeHouse
**
******************************************************/
void PrintoutHelper::writeHouse( Table *table, const int &row, const int &line, const int& house, const bool &vedic )
{
	double len;
	wxString s;
	Lang lang;
	Formatter *formatter = Formatter::get();

	if ( config->useVedicPlanetNames ) s = lang.getBhavaName( house - 1 );
	else s.Printf( wxT( "%02d" ), house );
	table->setEntry( row, line, s );

	len = h->getHouse( house - 1, vedic );
	if ( config->useVedicPositions )
		table->setEntry( row + 2, line, writer->getPosFormatted( len, false, DEG_PRECISION_SECOND ), false );
	else
	{
		table->setEntry( row + 2, line, formatter->getLenFormatted( getRasiLen( len ), DEG_PRECISION_MINUTE ));
		table->setEntry( row + 3, line, writer->getSignName( getRasi( len )), config->useSignSymbols );
	}
	if ( vedic )
	{
		KpData kp = h->getHouseKPLords( house - 1 );
		table->setEntry( row + 5, line, writer->getObjectName( kp.lord, TSHORT, true ), config->usePlanetSymbols );
		table->setEntry( row + 6, line, writer->getObjectName( kp.sublord, TSHORT, true ), config->usePlanetSymbols );
		table->setEntry( row + 7, line, writer->getObjectName( kp.subsublord, TSHORT, true ), config->usePlanetSymbols );
	}
}

/*****************************************************
**
**   PrintoutHelper   ---   writeVedicDefault
**
******************************************************/
void PrintoutHelper::writeVedicDefault( Table *table )
{
	int i;
	Lang lang;

	VargaHoroscope chart( h, 0 );
	VargaHoroscope nchart( h, 1 );
	JaiminiExpert jexpert( &nchart );
	jexpert.calcCharaKarakas();
	NakshatraExpert nexpert;

	table->setHeader( 2,  _( "Planet" ), false, 1 );
	table->setHeader( 4,  _( "Length" ), false, 1 );
	table->setHeader( 6,  _( "Nakshatra" ));
	table->setHeader( 7,  _( "D-9" ));
	table->setHeader( 8,  _( "Karaka" ));
	table->col_line[5] = true;
	int line = 1;

	vector<int> obs = PlanetList().getVedicObjectList( OBJECTS_INCLUDE_ASCENDANT | OBJECTS_INCLUDE_DRAGONTAIL | OBJECTS_INCLUDE_DRAGONHEAD );
	for ( unsigned int p = 0; p < obs.size(); p++ )
	{
		i = obs[p];
		writePlanetaryObject( table, 2, line, i, false, true );
		table->setEntry( 6, line, lang.getNakshatraName(::getNakshatra(h->getObjectPosition( i, true ).length, N27), N27, TLARGE ) );
		table->setEntry( 7, line, writer->getSignName(nchart.getRasi( i ), config->signPrecision ), config->useSignSymbols );
		if ( p < 8 )  table->setEntry( 8, line, lang.getKarakaName( jexpert.getCharaKarakaProperty(p)) );
		line++;
	}
}

/*****************************************************
**
**   PrintoutHelper   ---   writeVedicKp
**
******************************************************/
void PrintoutHelper::writeVedicKp( Table *table )
{
	int i;
	wxString s;
	Lang lang;

	VargaHoroscope chart( h, 0 );
	VargaHoroscope nchart( h, 1 );
	JaiminiExpert jexpert( &nchart );
	jexpert.calcCharaKarakas();
	NakshatraExpert nexpert;
	h->updateKP( 0 );

	table->setHeader( 2,  _( "Planet" ), false, 1 );
	table->setHeader( 4,  _( "Length" ), false, 1 );
	table->setHeader( 6,  _( "Naks" ));
	table->setHeader( 7,  _( "L" ));
	table->setHeader( 8,  _( "SL" ));
	table->setHeader( 9,  _( "SSL" ));

	table->setHeader( 10,  _( "Cusp" ), false, 4 );
	table->setHeader( 15,  _( "L" ));
	table->setHeader( 16,  _( "SL" ));
	table->setHeader( 17,  _( "SSL" ));
	table->col_line[9] = true;
	int line = 1;

	vector<int> obs = PlanetList().getVedicObjectList( OBJECTS_INCLUDE_ASCENDANT | OBJECTS_INCLUDE_DRAGONTAIL | OBJECTS_INCLUDE_DRAGONHEAD );
	for ( unsigned int p = 0; p < obs.size(); p++ )
	{
		i = obs[p];
		writePlanetaryObject( table, 2, line, i, false, true, true );
		table->setEntry( 6, line, lang.getNakshatraName(::getNakshatra(h->getObjectPosition( i, true ).length, N27), N27, TMEDIUM ) );

		KpData kp = h->getKPLords( i );
		table->setEntry( 7, line, writer->getObjectName( kp.lord, TSHORT, true ), config->usePlanetSymbols );
		table->setEntry( 8, line, writer->getObjectName( kp.sublord, TSHORT, true ), config->usePlanetSymbols );
		table->setEntry( 9, line, writer->getObjectName( kp.subsublord, TSHORT, true ), config->usePlanetSymbols );
		line++;
	}
	line = 1;
	for ( i = 2; i <= 12 ; i++ )
	{
		writeHouse( table, 10, line++, i, true );
	}
}

/*****************************************************
**
**   PrintoutHelper   ---   writePrintoutSummary
**
******************************************************/
void PrintoutHelper::writePrintoutSummary( const int &st )
{
	style = st;
	int num_cols, num_rows = 30;
	switch ( style )
	{
	case PSUM_WDEFAULT:
		num_rows = 10;
		num_cols = 12;
		break;
	case PSUM_WURANIAN:
		num_rows = 12;
		num_cols = 12;
		break;
	case PSUM_VDEFAULT:
		num_rows = 11;
		num_cols = 9;
		break;
	case PSUM_VKP:
		num_rows = 12;
		num_cols = 18;
		break;
	default:
		assert( false );
		break;
	}

	Table *table = new Table( num_cols, num_rows );
	table->show_grid = false;
	table->col_line[1] = true;
	table->col_alignment[0] = Align::Right;
	table->col_alignment[4] = Align::Right;

	writeBaseData(table );
	switch ( style )
	{
	case 0:
		writeWesternDefault( table );
		break;
	case 1:
		writeWesternUranian(table );
		break;
	case 2:
		writeVedicDefault(table );
		break;
	case 3:
		writeVedicKp(table );
		break;
	default:
		assert( 0 );
		break;
	}
	writer->writeTable( *table );
	delete table;
}

/*****************************************************
**
**   PrintoutHelper   ---   writeWesternAspectarium
**
******************************************************/
void PrintoutHelper::writeWesternAspectarium( ChartProperties *props, const int &type )
{
	Formatter *formatter = Formatter::get();
	wxString s;

  AspectExpert expert( h );
	list<AspectEvent> al = expert.update( h, props );

	int numrows = (int)al.size() / 3;
	if ( al.size() % 3 ) numrows++;

	Table table( 12, numrows + 1 );
	table.show_grid = false;

	table.setHeader( 0, _( "Aspect" ), false, 2 );
	table.setHeader( 3, _( "Orbis" ) );
	table.col_line[2] = true;
	table.col_line[3] = true;

	table.setHeader( 4, _( "Aspect" ), false, 2 );
	table.setHeader( 7, _( "Orbis" ) );
	table.col_line[6] = true;
	table.col_line[7] = true;

	table.setHeader( 8, _( "Aspect" ), false, 2 );
	table.setHeader( 11, _( "Orbis" ) );
	table.col_line[10] = true;

	int col = 0;
	int row = 0;
	for ( list<AspectEvent>::iterator iter = al.begin(); iter != al.end(); iter++ )
	{
		table.setEntry( 4 * col, row + 1, writer->getObjectName( (*iter).planet1, TMEDIUM ), config->usePlanetSymbols );

		s = AspectExpert::getAspectSymbol( (*iter).aspectType );
    if ( s.IsEmpty())
    {
      s = AspectExpert::getAspectShortDescription( (*iter).aspectType );
			table.setEntry( 4 * col + 1, row + 1, s, false );
    }
		else
		{
			table.setEntry( 4 * col + 1, row + 1, s, true );
		}
		table.setEntry( 4 * col + 2, row + 1, writer->getObjectName( (*iter).planet2, TMEDIUM ), config->usePlanetSymbols );
		table.setEntry( 4 * col + 3, row + 1,  formatter->getLenFormatted( (*iter).orbis, DEG_PRECISION_MINUTE ), false );

		row++;
		if ( row >= numrows )
		{
			col++;
			row = 0;
		}
	}
	writer->writeTable( table, true );
}


