/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/TextHelper.cpp
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
#pragma implementation "TextHelper.h"
#endif

#include "TextHelper.h"

#include <wx/log.h>
#include <wx/string.h>
#include <math.h>

#include "ArabicParts.h"
#include "Aspect.h"
#include "Ashtakavarga.h"
#include "astrobase.h"
#include "Calculator.h"
#include "ChartProperties.h"
#include "Conf.h"
#include "Dasa.h"
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
**   TextHelper   ---   writeTextAnalysis
**
******************************************************/
TextHelper::TextHelper( const Horoscope *h, const ChartProperties *props, Writer *writer )
		: horoscope( h ),
		chartprops( props )
{
	this->writer = writer;
	show_header = config->showTextViewHeaders;
}

/*****************************************************
**
**   TextHelper   ---   writeTextAnalysis
**
******************************************************/
int TextHelper::writeTextAnalysis( const int &mode, const int varga, const int dasa )
{
	int ret = 0;
	switch ( mode )
	{
	case TEXT_MODE_BASE:
		writeBaseData();
		break;

	case TEXT_MODE_WESTERN_PLANETS:
		ret = writeWesternPlanetReport();
		break;

	case TEXT_MODE_VEDIC_PLANETS:
		ret = writeVedicPlanetReport();
		break;

	case TEXT_MODE_NAKSHATRA:
	{
		NakshatraExpert( horoscope ).write( writer, chartprops, show_header );
	}
	break;

	case TEXT_MODE_ASHTAKAVARGA:
	{
		VargaHoroscope chart( horoscope, varga );
		AshtakavargaExpert aexpert( &chart );
		aexpert.update();
		aexpert.write( writer, show_header );
	}
	break;
	case TEXT_MODE_KP:
		ret = writeKp( dasa );
		break;

	case TEXT_MODE_BHAVA:
		writeBhavas();
		break;

	case TEXT_MODE_ARABICPARTS:
	{
		ArabicPartsExpert( horoscope ).write( writer, chartprops->isVedic(), show_header );
	}
	break;

	case TEXT_MODE_VEDIC_ASPECTARIUM:
	{
		AspectExpert aspexpert( horoscope );
		aspexpert.writeVedicAspectarium( horoscope, writer, chartprops, show_header );
	}
	break;

	case TEXT_MODE_WESTERN_ASPECTARIUM:
	{
		AspectExpert aspexpert( horoscope );
		aspexpert.update( horoscope, chartprops );
		if ( show_header ) writer->writeHeader1( _( "Aspectarium" ));
		aspexpert.writeWesternAspectarium( writer, chartprops );
		aspexpert.writeWesternAspectList( writer, chartprops );
	}
	break;

	case TEXT_MODE_WESTERN_ASPECTARIUM_PLAIN:
	{
		AspectExpert aspexpert( horoscope );
		aspexpert.update( horoscope, chartprops );
		aspexpert.writeWesternAspectarium( writer, chartprops );
	}
	break;

	case TEXT_MODE_ASTRONOMICAL:
		ret = writeAstronomicalData();
		break;

	case TEXT_MODE_VARGA:
	{
		VargaExpert vexpert( horoscope );
		vexpert.write( writer, chartprops->getVedicObjectStyle(), show_header );
	}
	break;

	case TEXT_MODE_DASA:
		assert( dasa >= 0 && dasa < (int)DasaExpertFactory::MAX_DASAEXPERTS );
		DasaExpertFactory::get()->getDasaExpert( dasa )->write( writer, horoscope, varga, show_header );
		break;

	case TEXT_MODE_DASACOMPACT:
		assert( dasa >= 0 && dasa < (int)DasaExpertFactory::MAX_DASAEXPERTS );
		DasaExpertFactory::get()->getDasaExpert( dasa )->writeCompact( writer, horoscope, varga, show_header );
		break;

	case TEXT_MODE_DASALONG:
		assert( dasa >= 0 && dasa < (int)DasaExpertFactory::MAX_DASAEXPERTS );
		DasaExpertFactory::get()->getDasaExpert( dasa )->writeMore( writer, horoscope, varga, show_header );
		break;

	case TEXT_MODE_JAIMINI:
	{
		VargaHoroscope chart( horoscope, varga );
		JaiminiExpert jexpert( &chart );
		jexpert.update();
		jexpert.write( writer, show_header );
	}
	break;

	case TEXT_MODE_MAIN_VIEW:
		ret = writePlanets();
		break;

	default:
		assert( false );
		break;
	}
	if ( ret ) printf( "Warn: %d objects couldn't be calculated\n", ret );
	return ret;
}

/*****************************************************
**
**   TextHelper   ---   writePlanets
**
******************************************************/
int TextHelper::writePlanets()
{
	return chartprops->isVedic() ? writeVedicPlanets() : writeWesternPlanets();
}

/*****************************************************
**
**   TextHelper   ---   writeVedicPlanets
**
******************************************************/
int TextHelper::writeVedicPlanets()
{
#define HOUSE_QUALIFIER 1000
	wxString s;
	int pindex, rowindex = 1, shast, col = 1, p, ret = 0;
	int numcols;
	unsigned int i;
	ObjectPosition pos;
	Formatter *f = Formatter::get();
	Lang lang;

	int nakshatramode =  ( config->vNakshatraMode28 ? N28 : N27 );
	VargaHoroscope chart( horoscope, VARGA_RASI );
	VargaHoroscope bchart( horoscope, VARGA_BHAVA );
	JaiminiExpert expert( &chart );
	AshtakavargaExpert aexpert( &chart );
	Planet *planet;
	int istyle = chartprops->getVedicMainStyle();

	int extraObjects = 0;
	if ( chartprops->getVedicObjectStyle() & OBJECTS_INCLUDE_D9_LAGNA ) extraObjects |= OBJECTS_INCLUDE_D9_LAGNA;
	vector<int> obs = chartprops->getVedicPlanetList( extraObjects );

	int opt_lord, opt_dig, opt_nav, opt_kara, opt_shast, opt_nak, opt_dasa, opt_bha, opt_kp, opt_ashtaka, opt_pada,
	opt_houses, opt_declinations;

	opt_lord = opt_dig = opt_nav = opt_kara = opt_shast = opt_nak = opt_dasa = opt_bha = opt_kp = opt_ashtaka = opt_pada
	                               = opt_houses = opt_declinations = 0;

	numcols = 2;
	if ( istyle & MAIN_CHILD_SHOW_LORD        ) {
		opt_lord = true;
		numcols++;
	}
	if ( istyle & MAIN_CHILD_SHOW_DIGNITY     ) {
		opt_dig = true;
		numcols++;
	}
	if ( istyle & MAIN_CHILD_SHOW_NAVAMSA     ) {
		opt_nav = true;
		numcols++;
	}
	if ( istyle & MAIN_CHILD_SHOW_KARAKA      ) {
		opt_kara = true;
		expert.calcCharaKarakas();
		numcols++;
	}
	if ( istyle & MAIN_CHILD_SHOW_SHASTIAMSA  ) {
		opt_shast = true;
		numcols++;
	}
	if ( istyle & MAIN_CHILD_SHOW_NAKSHATRA   ) {
		opt_nak = true;
		numcols++;
	}
	if ( istyle & MAIN_CHILD_SHOW_DASAVARGA   ) {
		opt_dasa = true;
		numcols++;
	}
	if ( istyle & MAIN_CHILD_SHOW_HOUSEPOS    ) {
		opt_bha = true;
		numcols++;
	}
	if ( istyle & MAIN_CHILD_SHOW_ASHTAKA     ) {
		opt_ashtaka = true;
		aexpert.update();
		numcols += 1;
	}
	if ( istyle & MAIN_CHILD_SHOW_PADA        ) {
		opt_pada = true;
		numcols += 1;
	}
	if ( istyle & MAIN_CHILD_SHOW_KP          ) {
		opt_kp = true;
		horoscope->updateKP( 0 );
		numcols += 3;
	}
	if ( istyle & MAIN_CHILD_SHOW_DECLINATION ) {
		opt_declinations = true;
		numcols ++;
	}

	if ( istyle & MAIN_CHILD_SHOW_HOUSES )
	{
		opt_houses = true;
		for ( i = HOUSE1; i <= HOUSE12; i++ ) obs.push_back( i + HOUSE_QUALIFIER );
	}

	Table table( numcols, obs.size() + 1 );
	table.setHeader( 0,  _( "Planet" ));
	table.setHeader( 1,  _( "Length" ));
	i = 2;
	if ( opt_declinations ) table.setHeader( i++, _( "Declination" ));
	if ( opt_lord ) table.setHeader( i++, _( "Lord" ));
	if ( opt_dig ) table.setHeader( i++, _( "Dignity" ));
	if ( opt_nav ) table.setHeader( i++, _( "D-9" ));
	if ( opt_nak ) table.setHeader( i++, _( "Nakshatra" ));
	if ( opt_pada ) table.setHeader( i++, _( "Pada" ));
	if ( opt_kp )
	{
		table.setHeader( i++, _( "KP Lord" ));
		table.setHeader( i++, _( "Sublord" ));
		table.setHeader( i++, _( "Subsublord" ));
	}
	if ( opt_shast ) table.setHeader( i++, _( "Shastiamsa" ));
	if ( opt_dasa ) table.setHeader( i++, _( "Dasa Varga" ));
	if ( opt_bha ) table.setHeader( i++, _( "Bhava" ));
	if ( opt_kara ) table.setHeader( i++, _( "Karaka" ));
	if ( opt_ashtaka ) table.setHeader( i++, _( "AV" ));

	for ( i = 0; i < obs.size(); i++ )
	{
		pindex = obs[i];

		if ( pindex >= HOUSE_QUALIFIER )
		{
			pos.length = horoscope->getHouse( pindex - HOUSE_QUALIFIER, true, false );
			pos.latitude = 0;
			pos.retro = false;
		}
		else
		{
			pos = horoscope->getObjectPosition( pindex, true );

			// pos.length is 0 for unclaculated planets because Ayanamsa isn't subtracted in that case
			if ( pos.length == 0 && pindex != OARIES ) ret++;
		}

		planet = 0;
		col = 2;
		assert( (int)table.nb_rows >= rowindex );

		table.setEntry( 0, rowindex,
		                ( pindex <= LAST_ARABIC_OBJECT ? writer->getObjectName( pindex, TLARGE, true ) : lang.getBhavaName( pindex - HOUSE_QUALIFIER )));
		if ( pindex <= OSATURN ) planet = horoscope->getPlanet( pindex );

		table.setEntry( 1, rowindex, writer->getPosFormatted( pos.length, pos.retro ));

		// Declination
		if ( opt_declinations )
			table.setEntry( col++, rowindex, f->getLatitudeFormatted( pos.latitude, DEG_PRECISION_SECOND ));

		// Lord
		if ( opt_lord )
			table.setEntry( col++, rowindex, writer->getObjectName( getLord( getRasi( pos.length )), TLARGE, true ));

		// Dignity
		if ( opt_dig )
		{
			if ( pindex >= OSUN && pindex <= OSATURN && obs[i] < MAX_EPHEM_OBJECTS )
				table.setEntry( col, rowindex, lang.getDignityName( horoscope->getVargaData( pindex, VARGA_RASI )->getDignity(), TLARGE ));
			col++;
		}

		// Navamsa
		if ( opt_nav )
			table.setEntry( col++, rowindex, writer->getSignName( getRasi( pos.length * 9 ), config->signPrecision ));

		// Nakshatra
		if ( opt_nak )
			table.setEntry( col++, rowindex,
			                lang.getNakshatraName( getNakshatra( pos.length, nakshatramode ), nakshatramode, TMEDIUM ));

		// Pada
		if ( opt_pada )
		{
			s.Printf( wxT( "%d" ), (int)(getNakshatraLength( pos.length, nakshatramode ) / 3.3333333333 ) + 1 );
			table.setEntry( col++, rowindex, s );
		}

		// Krishnamurti
		if ( opt_kp )
		{
			KpData kp;
			if ( pindex <= LAST_ARABIC_OBJECT )
			{
				kp = horoscope->getKPLords( obs[i] );
			}
			else
			{
				kp = horoscope->getHouseKPLords( pindex - HOUSE_QUALIFIER );
			}
			table.setEntry( col++, rowindex, writer->getObjectName( kp.lord, TLARGE, true ));
			table.setEntry( col++, rowindex, writer->getObjectName( kp.sublord, TLARGE, true ));
			table.setEntry( col++, rowindex, writer->getObjectName( kp.subsublord, TLARGE, true ));
		}

		// Shastiamsa
		if ( opt_shast )
		{
			shast = (int)( a_red( pos.length, 30 ) * 2 );
			if ( isEvenRasi( pos.length )) shast = 59 - shast;
			assert( shast >= 0 && shast < 60 );
			s.Printf( wxT( "%s (%c)" ), (const wxChar*)lang.getShastiamsaName( shast),
			          ( k_shastiamsa_benefic[(int)shast] ? 'B' : 'M' ));
			table.setEntry( col, rowindex, s );
			col++;
		}

		// Dasavarga Dignity
		if ( opt_dasa )
		{
			if ( pindex <= OSATURN  && obs[i] < MAX_EPHEM_OBJECTS )
			{
				VargaExpert expert;
				table.setEntry( col, rowindex, lang.getVimsopakaDignityName( 2, planet->getVimsopakaBalaGoodVargas( 2 )) );
			}
			col++;
		}

		// Bhava
		if ( opt_bha  && obs[i] <= MAX_EPHEM_OBJECTS )
		{
			s.Printf( wxT( "%d" ), red12( bchart.getRasi( pindex ) - bchart.getRasi( OASCENDANT )) + 1 );
			table.setEntry( col++, rowindex, s );
		}

		// Karaka
		if ( opt_kara )
		{
			if ( pindex <= OSATURN ) p = pindex;
			else if ( pindex == OMEANNODE || pindex == OTRUENODE ) p = 7;
			else p = -1;
			if ( p >= 0 ) table.setEntry( col, rowindex, lang.getKarakaName( expert.getCharaKarakaProperty( p )) );
			col++;
		}


		// Ashtaka Varga
		if ( opt_ashtaka )
		{
			if ((( pindex >= OSUN && pindex <= OSATURN ) || pindex == OASCENDANT )  && obs[i] <= MAX_EPHEM_OBJECTS )
			{
				s.Printf( wxT( "%d" ), aexpert.getItem( REKHA, pindex, getRasi( pos.length )));
				table.setEntry( col++, rowindex, s );
			}
			col++;
		}
		rowindex++;
	}
	writer->writeTable( table );
	return ret;
}

/*****************************************************
**
**   TextHelper   ---   writeWesternPlanets
**
******************************************************/
int TextHelper::writeWesternPlanets()
{
	wxString s;
	Formatter *f = Formatter::get();
	int pindex, line, j, ret = 0;
	int numcols, numrows;
	bool opt_housepos = false;
	bool opt_declination = false;
	ObjectPosition pos;
	int actcol = 0;

	numcols = 2;
	int wstyle = chartprops->getWesternMainStyle();
	if ( wstyle & MAIN_CHILD_SHOW_HOUSEPOS ) {
		opt_housepos = true;
		numcols++;
	}
	if ( wstyle & MAIN_CHILD_SHOW_DECLINATION ) {
		opt_declination = true;
		numcols++;
	}

	vector<int> obs = chartprops->getWesternPlanetList();

	numrows = obs.size() + 1;
	if ( wstyle & MAIN_CHILD_SHOW_HOUSES ) numrows += 4;
	Table table( numcols, numrows );
	table.setHeader( actcol++,  _( "Planet" ));
	table.setHeader( actcol++,  _( "Length" ));
	if ( opt_declination ) table.setHeader( actcol++, _( "Declination" ));
	if ( opt_housepos ) table.setHeader( actcol++, _( "House" ));

	line = 1;
	for ( unsigned int i = 0; i < obs.size(); i++ )
	{
		actcol = 0;
		pindex = obs[i];
		assert( (int)table.nb_rows >= line );

		pos = horoscope->getObjectPosition( pindex, false );
		// pos.length is 0 for unclaculated planets because Ayanamsa isn't subtracted in that case
		if ( pos.length == 0 && pindex != OARIES ) ret++;

		table.setEntry( actcol++, line, writer->getObjectName( pindex, TLARGE ));
		table.setEntry( actcol++, line, writer->getPosFormatted( pos.length, pos.retro ));

		// Declination
		if ( opt_declination ) table.setEntry( actcol++, line, f->getLatitudeFormatted( pos.latitude, DEG_PRECISION_SECOND ));

		// Houses 2, 3, 11, 12. AS and MC are separate params
		if ( opt_housepos )
		{
			assert( table.nb_cols > 0 );
			s.Printf( wxT( "%d" ), horoscope->getHousePos( pindex, false ) + 1 );
			table.setEntry( actcol++, line, s );
		}
		line++;
	}
	if ( wstyle & MAIN_CHILD_SHOW_HOUSES )
	{
		for ( j = 0; j < 12; j++ )
		{
			if (( j == HOUSE1 ) || ( j == HOUSE4 ) || ( j == HOUSE5 ) || ( j == HOUSE6 )
			        || ( j == HOUSE7 ) || ( j == HOUSE8 ) || ( j == HOUSE9 ) || ( j == HOUSE10 )
			   ) continue;
			assert( (int)table.nb_rows > line );
			s.Printf( wxT( "%02d" ), j+1 );
			table.setEntry( 0, line, s );
			table.setEntry( 1, line, writer->getPosFormatted( horoscope->getHouse(j, false), false ));
			line++;
		}
	}
	writer->writeTable( table );
	return ret;
}

/*****************************************************
**
**   TextHelper   ---   writeBaseData
**
******************************************************/
void TextHelper::writeBaseData()
{
	wxString s;
	double time;

	Formatter *f = Formatter::get();
	Lang lang;

	DataSet *ds = horoscope->getDataSet();
	Location *loc = ds->getLocation();
	JDate *date = ds->getDate();

	if ( show_header ) writer->writeHeader1( _( "Base Data" ));
	Table table( 2, 17 );

	int line = 0;
	table.setEntry( 0, line,  _( "Name" ));
	table.setEntry( 1, line++, horoscope->getHName() );

	table.setEntry( 0, line,  _( "Date" ));
	s << f->getDateStringFromJD( horoscope->getJD() + ( loc->getTimeZone() + loc->getDST() )/24 )
	<< wxT( " " ) << lang.getWeekdayName( ds->getWeekDay() );
	table.setEntry( 1, line++, s );

	table.setEntry( 0, line,  _( "Local Time" ));
	time = a_red( getTimeFromJD( date->getJD()) + loc->getTimeZone() + loc->getDST(), 24 );
	table.setEntry( 1, line++, f->getTimeFormatted( time ));

	table.setEntry( 0, line,  _( "Location" ));
	table.setEntry( 1, line++, loc->getLocName() );

	table.setEntry( 0, line,  _( "Longitude" ));
	table.setEntry( 1, line++, f->getLongitudeFormatted( loc->getLongitude()));

	table.setEntry( 0, line,  _( "Latitude" ));
	table.setEntry( 1, line++, f->getLatitudeFormatted( loc->getLatitude()));

	table.setEntry( 0, line,  _( "Time Zone" ));
	double tz = loc->getTimeZone();
	s.Printf( wxT( "%s %c%.1f %s" ), _( "UT" ), ( tz >= 0 ? '+' : ' ' ), tz, _( "Hours" ));
	table.setEntry( 1, line++, s );

	table.setEntry( 0, line,  _( "Daylight Saving" ));
	s.Printf( wxT( "%.1f %s" ), loc->getDST(), _( "Hours" ));
	table.setEntry( 1, line++, s );

	table.setEntry( 0, line,  _( "Universal Time" ));
	table.setEntry( 1, line++, f->getTimeFormatted( getTimeFromJD( horoscope->getJD() )));

	table.setEntry( 0, line,  _( "Sidereal Time" ));
	table.setEntry( 1, line++, f->getTimeFormatted( horoscope->getSiderealTime()));

	table.setEntry( 0, line,  _( "Julian Date" ));
	s.Printf( wxT( "%8.5f" ), horoscope->getJD());
	table.setEntry( 1, line++, s );

	table.setEntry( 0, line,  _( "Vedic Ayanamsa" ));
	table.setEntry( 1, line++, lang.getAyanamsaName( horoscope->getAyanamsaType( true ) ));

	table.setEntry( 0, line,  _( "Ayanamsa Value" ));
	table.setEntry( 1, line++, f->getDegreesFormatted( horoscope->getAyanamsa( true )));

	table.setEntry( 0, line,  _( "Western Ayanamsa" ));
	table.setEntry( 1, line++, lang.getAyanamsaName( horoscope->getAyanamsaType( false )));

	table.setEntry( 0, line,  _( "Ayanamsa Value" ));
	table.setEntry( 1, line++, f->getDegreesFormatted( horoscope->getAyanamsa( false )));

	table.setEntry( 0, line,  _( "Sunrise" ));
	if ( horoscope->getSunrise() != 0 )
		s = f->getFullDateStringFromJD( horoscope->getSunrise() + ( loc->getTimeZone() + loc->getDST() ) / 24.0 );
	else s = _( "n.a." );
	table.setEntry( 1, line++, s );

	table.setEntry( 0, line,  _( "Sunset" ));
	if ( horoscope->getSunset() != 0 )
		s = f->getFullDateStringFromJD( horoscope->getSunset() + ( loc->getTimeZone() + loc->getDST() ) / 24.0 );
	else s = _( "n.a." );
	table.setEntry( 1, line++, s );

	writer->writeTable( table );
}

/*****************************************************
**
**   TextHelper   ---   writeAstronomicalData
**
******************************************************/
int TextHelper::writeAstronomicalData()
{
	wxString pname;
	int p, ret = 0;

	Calculator *calculator = CalculatorFactory().getCalculator();
	Formatter *f = Formatter::get();
	Lang lang;

	vector<int> obs1 = chartprops->getPlanetList();
	vector<int> obs;
	for ( unsigned int i = 0; i < obs1.size(); i++ )
	{
		if ( obs1[i] < MAX_EPHEM_OBJECTS ) obs.push_back( obs1[i] );
	}

	if ( show_header ) writer->writeHeader1( _( "Astronomical Positions" ));
	Table table( 4, obs.size() + 1 );
	table.setHeader( 0,  _( "Planet" ));
	table.setHeader( 1,  _( "Longitude" ));
	table.setHeader( 2,  _( "Latitute" ));
	table.setHeader( 3,  _( "Speed (deg/day)" ));
	table.col_line[0] = true;
	table.col_alignment[0] = Align::Right;
	table.col_alignment[1] = Align::Right;

	int line = 1;
	for ( unsigned int i = 0; i < obs.size(); i++ )
	{
		p = obs[i];
		if ( horoscope->getTropicalLength( p ) == 0 ) ret++;
		pname = writer->getObjectName( p, TLARGE);
		table.setEntry( 0, line, pname );
		table.setEntry( 1, line, writer->getPosFormatted( horoscope->getTropicalLength( p ), horoscope->isRetrograde( p ), DEG_PRECISION_MORE ));

		if ( p != OASCENDANT && p != OMERIDIAN )
		{
			table.setEntry( 2, line, f->getLatitudeFormatted( horoscope->getLatitude( p ), DEG_PRECISION_MORE ));
			table.setEntry( 3, line, f->getLenFormatted( horoscope->getSpeed( p ), DEG_PRECISION_MORE ));
		}
		line++;
	}
	writer->writeTable( table );

	Table t2( 2, 5 );
	t2.setHeader( 0,  _( "Name" ));
	t2.setHeader( 1,  _( "Value" ));
	writer->writeHeader2( _( "Ayanamsa" ));
	line = 1;
	for ( int i = 0; i < NB_AYANAMSAS; i++ )
	{
		t2.setEntry( 0, line, lang.getAyanamsaName( i+1 ));
		t2.setEntry( 1, line, f->getDegreesFormatted( calculator->calcAyanamsa( horoscope->getJD(), i+1 ), DEG_PRECISION_MORE ));
		line++;
	}
	t2.setEntry( 0, 4, _( "Custom" ) );
	t2.setEntry( 1, 4, f->getDegreesFormatted( calculator->calcAyanamsa( horoscope->getJD(), 4 ), DEG_PRECISION_MORE ));
	writer->writeTable( t2 );
	return ret;
}

/*****************************************************
**
**   TextHelper   ---   writeBhavas
**
******************************************************/
void TextHelper::writeBhavas()
{
	int i;
	Lang lang;
	wxString s;
	double len;

	if ( show_header ) writer->writeHeader1( _( "Bhavas" ));
	Table table( 3, 13 );
	table.setHeader( 0,  _( "Bhava" ));
	table.setHeader( 1,  _( "Cusp" ));
	table.setHeader( 2,  _( "Sandhi" ));

	int line = 1;
	for ( i = HOUSE1; i <= HOUSE12; i++ )
	{
		table.setEntry( 0, line, lang.getBhavaName( i ));
		table.setEntry( 1, line, writer->getPosFormatted( horoscope->getHouse(i, true, false), false ));
		table.setEntry( 2, line, writer->getPosFormatted( horoscope->getHouse(i, true, true), false ));
		line++;
	}

	writer->writeTable( table );

	vector<int> obs = chartprops->getPlanetList();
	writer->writeHeader2( _( "Planets in Bhavas" ));
	Table t2( 3, obs.size() + 1 );
	t2.setHeader( 0,  _( "Planet" ));
	t2.setHeader( 1,  _( "Length" ));
	t2.setHeader( 2,  _( "Bhava" ));
	line = 1;
	for ( unsigned int p = 0; p < obs.size(); p++ )
	{
		i = obs[p];
		len = horoscope->getVedicLength( i );
		t2.setEntry( 0, line, writer->getObjectName( i, TLARGE, true ));
		t2.setEntry( 1, line, writer->getPosFormatted( len, horoscope->isRetrograde( i )));
		t2.setEntry( 2, line, lang.getBhavaName( horoscope->getHousePos( i, true )));
		line++;
	}
	writer->writeTable( t2 );
}

/*****************************************************
**
**   TextHelper   ---   writeKp
**
******************************************************/
int TextHelper::writeKp( const int &dasaindex )
{
	wxString s;
	Lang lang;
	int p, ret = 0;
	double len, hlen;
	DasaExpert *expert = DasaExpertFactory::get()->getDasaExpert( dasaindex );
	KpData kp;

	if ( ! expert->hasKpFeatures() ) // Not supported by all (e.g. Jaimini)
	{
		writer->writeLine( _( "Not supported" ) );
		return 0;
	}
	horoscope->updateKP( dasaindex );

	if ( show_header )
	{
		writer->writeHeader1( _( "Krishnamurti Paddhati" ));
		s.Printf( wxT( "%s: %s" ), _( "Dasa"), expert->getName());
		writer->writeParagraph( s );
	}

	int kpstyle =  chartprops->getVedicObjectStyle();
	if ( chartprops->getVedicObjectStyle() & OBJECTS_INCLUDE_ASCENDANT ) kpstyle -= OBJECTS_INCLUDE_ASCENDANT;
	if ( chartprops->getVedicObjectStyle() & OBJECTS_INCLUDE_MERIDIAN ) kpstyle -= OBJECTS_INCLUDE_MERIDIAN;
	vector<int> obs = PlanetList().getVedicObjectList( kpstyle );

	Table table( 7, obs.size()+13 );
	table.setHeader( 0,  _( "Planet" ));
	table.setHeader( 1,  _( "Length" ));
	table.setHeader( 2,  _( "Sign Lord" ));
	table.setHeader( 3,  _( "Bhava" ));
	table.setHeader( 4,  _( "KP Lord" ));
	table.setHeader( 5,  _( "Sublord" ));
	table.setHeader( 6,  _( "Subsublord" ));
	for( int i = 0; i < 7; i++ ) table.col_alignment[i] = Align::Center;

	int line = 1;
	for ( unsigned int i1 = 0; i1 < obs.size(); i1++ )
	{
		p = obs[i1];
		len = horoscope->getVedicLength( p );
		if ( len == 0 ) ret++;
		table.setEntry( 0, line, writer->getObjectName( p, TLARGE, true ) );
		table.setEntry( 1, line, writer->getPosFormatted( len, horoscope->isRetrograde( p )));
		table.setEntry( 2, line, writer->getObjectName( getLord( getRasi( len )), TLARGE, true ) );
		s.Printf( wxT( "%02d" ), horoscope->getHousePos( p, true ) + 1 );
		table.setEntry( 3, line, s );

		kp = horoscope->getKPLords( p );
		if ( expert->isRasiDasaExpert() )
		{
			table.setEntry( 4, line, writer->getSignName( kp.lord, TLARGE ) );
			table.setEntry( 5, line, writer->getSignName( kp.sublord, TLARGE ) );
			table.setEntry( 6, line, writer->getSignName( kp.subsublord, TLARGE ) );
		}
		else
		{
			table.setEntry( 4, line, writer->getObjectName( kp.lord, TLARGE, true ) );
			table.setEntry( 5, line, writer->getObjectName( kp.sublord, TLARGE, true ) );
			table.setEntry( 6, line, writer->getObjectName( kp.subsublord, TLARGE, true ) );
		}
		line++;
	}
	for ( p = HOUSE1; p <= HOUSE12; p++ )
	{
		hlen = horoscope->getHouse( p, true, false );
		table.setEntry( 0, line, lang.getBhavaName( p ));
		table.setEntry( 1, line, writer->getPosFormatted( hlen, false ) );
		table.setEntry( 2, line, writer->getObjectName( getLord(getRasi( hlen )), TLARGE, true ) );

		kp = horoscope->getHouseKPLords( p );
		if ( expert->isRasiDasaExpert() )
		{
			table.setEntry( 4, line, writer->getSignName( kp.lord, TLARGE ) );
			table.setEntry( 5, line, writer->getSignName( kp.sublord, TLARGE ) );
			table.setEntry( 6, line, writer->getSignName( kp.subsublord, TLARGE ) );
		}
		else
		{
			table.setEntry( 4, line, writer->getObjectName( kp.lord, TLARGE, true ) );
			table.setEntry( 5, line, writer->getObjectName( kp.sublord, TLARGE, true ) );
			table.setEntry( 6, line, writer->getObjectName( kp.subsublord, TLARGE, true ) );
		}
		line++;
	}
	writer->writeTable( table );
	return ret;
}

/*****************************************************
**
**   TextHelper   ---   writeVedicPlanetReport
**
******************************************************/
int TextHelper::writeVedicPlanetReport()
{
	VargaExpert expert;
	int i, j;
	Lang lang;
	Calculator *calculator = CalculatorFactory().getCalculator();
	wxString s1;
	double len;
	int ret = 0;

	VargaHoroscope chart( horoscope, 0 );
	VargaHoroscope nchart( horoscope, 1 );
	JaiminiExpert jexpert( &nchart );
	jexpert.calcCharaKarakas();
	NakshatraExpert nexpert;

	if ( show_header ) writer->writeHeader1( _( "Vedic Planets" ));
	vector<int> obs = chartprops->getVedicPlanetList();
	Table table( 5, obs.size() + 1 );
	table.setHeader( 0,  _( "Planet" ));
	table.setHeader( 1,  _( "Length" ));
	table.setHeader( 2,  _( "Karaka" ));
	table.setHeader( 3,  _( "Navamsa" ));
	table.setHeader( 4,  _( "Nakshatra" ));
	table.col_alignment[0] = Align::Center;
	table.col_alignment[1] = Align::Right;
	table.col_alignment[3] = Align::Center;

	//const int inode = config->iLunarNodeMode == LUNAR_NODE_TRUE ? OTRUENODE : OMEANNODE;
	int line = 1;
	for ( unsigned int p = 0; p < obs.size(); p++ )
	{
		i = obs[p];
		len = horoscope->getVedicLength( i );
		if ( len == 0 ) ret++;
		table.setEntry( 0, line,  writer->getObjectName( i, TLARGE, true ));
		table.setEntry( 1, line,  writer->getPosFormatted( len, horoscope->isRetrograde( i )));

		if ( i <= OSATURN ) j = i;
		else if ( i == OMEANNODE || i == OTRUENODE ) j = 7;
		else j = -1;
		if ( j != -1 )  table.setEntry( 2, line, lang.getKarakaName( jexpert.getCharaKarakaProperty( j )));

		table.setEntry( 3, line, writer->getSignName(nchart.getRasi( i ), config->signPrecision ));
		table.setEntry( 4, line, lang.getNakshatraName( getNakshatra( len, N27), N27, TLARGE ) );
		line++;
	}
	writer->writeTable( table );

	writer->writeHeader2( _( "Qualities" ));
	Table t4( 2, 5 );
	t4.setHeader( 0,  _( "Quality" ));
	t4.setHeader( 1,  _( "Value" ));
	double mlen = horoscope->getVedicLength( OMOON );

	line = 1;
	t4.setEntry( 0, line, _( "Varna" ) );
	t4.setEntry( 1, line++, lang.getVarnaName( getVarna( mlen )));

	t4.setEntry( 0, line, _( "Yoni" ) );
	t4.setEntry( 1, line++, lang.getYoniName( nexpert.getYoni( mlen )));

	t4.setEntry( 0, line, _( "Gana" ) );
	t4.setEntry( 1, line++, lang.getGanaName( nexpert.getGana( mlen )));

	t4.setEntry( 0, line, _( "Nadi" ) );
	t4.setEntry( 1, line++, lang.getNadiName( nexpert.getNadi( mlen )));
	writer->writeTable( t4 );

	writer->writeHeader2( _( "Moon's Nakshatra and Pada Portions" ));
	Table t5( 3, 3 );
	t5.setHeader( 0,  _( "Quality" ));
	t5.setHeader( 1,  _( "Value" ));
	t5.setHeader( 2,  _( "Value (Percent)" ));
	t5.setEntry( 0, 1, _( "Nakshatra" ) );
	double nportion = calculator->calcNakshatraPortion( horoscope->getDataSet(), mlen, false );
	s1.Printf( wxT( "%1.6f" ), nportion );
	t5.setEntry( 1, 1, s1 );

	s1.Printf( wxT( "%01.2f%%" ), 100.0 * nportion );
	t5.setEntry( 2, 1, s1 );

	t5.setEntry( 0, 2, _( "Pada" ) );
	double pportion = calculator->calcNakshatraPortion( horoscope->getDataSet(), mlen, true );
	s1.Printf( wxT( "%1.6f" ), pportion );
	t5.setEntry( 1, 2, s1 );

	s1.Printf( wxT( "%01.2f%%" ), 100.0 * pportion );
	t5.setEntry( 2, 2, s1 );

	writer->writeTable( t5 );

	return ret;
}

/*****************************************************
**
**   TextHelper   ---   writeWesternPlanetReport
**
******************************************************/
int TextHelper::writeWesternPlanetReport()
{
	wxString s;
	double len;
	Lang lang;
	int ret = 0;

	if ( show_header ) writer->writeHeader1( _( "Western Planets" ));
	vector<int> obs = chartprops->getWesternPlanetList();
	Table table( 3, obs.size() + 13 );
	table.setHeader( 0,  _( "Planet" ));
	table.setHeader( 1,  _( "Length" ));
	table.setHeader( 2,  _( "House" ));
	table.col_alignment[0] = Align::Center;

	int line = 1;
	for ( unsigned int i = 0; i < obs.size(); i++ )
	{
		len = horoscope->getWesternLength( obs[i] );
		if ( len == 0 ) ret++;
		table.setEntry( 0, line, writer->getObjectName( obs[i], TLARGE, false ) );
		table.setEntry( 1, line, writer->getPosFormatted( len, horoscope->isRetrograde( obs[i] ), DEG_PRECISION_SECOND ) );
		s.Printf( wxT( "%02d" ), horoscope->getHousePos( i, false ) + 1 );
		table.setEntry( 2, line, s );
		line++;
	}
	for ( int i = 0; i < 12; i++ )
	{
		if ( config->useVedicPlanetNames ) s = lang.getBhavaName( i );
		else s.Printf( wxT( "%02d" ), i+1 );
		table.setEntry( 0, line, s );
		table.setEntry( 1, line, writer->getPosFormatted( horoscope->getHouse( i, false ), false ) );
		line++;
	}
	writer->writeTable( table );
	return ret;
}

