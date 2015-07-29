/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/Ephemeris.cpp
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
#pragma implementation "Ephemeris.h"
#endif

#include "Ephemeris.h"

#include "astrobase.h"
#include "Calculator.h"
#include "ChartProperties.h"
#include "Conf.h"
#include "DataSet.h"
#include "Lang.h"
#include "mathbase.h"
#include "PlanetList.h"
#include "Writer.h"

extern Config *config;

/*************************************************//**
*
*  \brief sort ingress events according to their date
*
******************************************************/
class IngressEventSorter
{
public:
	bool operator()( const IngressEvent &e1, const IngressEvent &e2 )
	{
		return( e1.jd < e2.jd );
	}
};

/*************************************************//**
*
*  \brief sort Lunar events according to their date
*
******************************************************/
class LunarEventSort
{
public:
	bool operator()( const LunarEvent &e1, const LunarEvent &e2 )
	{
		return( e1.jd < e2.jd );
	}
};

/*****************************************************
**
**   EphemExpert   ---   Constructor
**
******************************************************/
EphemExpert::EphemExpert( const ChartProperties *props )
		: chartprops( props )
{
	d = new DataSet();
	d->setLocation( config->defaultLocation );
	show_header = config->showTextViewHeaders;
	init();
}

/*****************************************************
**
**   EphemExpert   ---   Destructor
**
******************************************************/
EphemExpert::~EphemExpert()
{
	delete d;
}

/*****************************************************
**
**   EphemExpert   ---   init
**
******************************************************/
void EphemExpert::init()
{
	int i;
	clen = cingress = ckp = cdetails = clunar = false;
	mydasa = -1;
	dstchange = false;

	// reset variables
	for ( i = 0; i <= 31; i++ )
	{
		weekday[i] = tithi[i] = 0;
		jd[i] = st[i] =  sunrise[i] = sunset[i] = 0;
	}
	planetdata.clear();
}

/*****************************************************
**
**   EphemExpert   ---   prepareMonth
**
******************************************************/
void EphemExpert::prepareMonth( const int &m, const int &y, const bool &isLocal )
{
	int dummy;
	double lastdst = 0;
	Formatter *formatter = Formatter::get();
	TzUtil tzu;
	TzInfo tzinfo;

	year = y;
	month = m;
	this->isLocaltime = isLocal;

	assert( month >= 0 && month <= 12 );
	assert( year >= -2000 && year <= 4000 );

	init();

	switch ( month )
	{
	case 0:
		formatter->getDateIntsFromJD( d->getJD(), dummy, month, year );
		break;
		break;
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		nb_days = 31;
		break;
	case 2: // Feb
		nb_days = 28;
		if ( year % 4 == 0 )
		{
			nb_days = 29;
			if ( year % 100 == 0 ) nb_days = 28; // 1900, 1800 etc. have 28 days
			if ( year % 400 == 0 ) nb_days = 29; // BUT Feb 2000 has 29 days
		}
		break;
	default:
		nb_days = 30;
		break;
	}

	Formatter::get()->getDateIntsFromJD( JDate::getCurrentJD(), current_day, current_month, current_year );

	// set jds and week days
	for ( int i = 0; i <= nb_days; i++ ) // calc JDs for this month and year
	{
		d->setDate( i+1, month, year, 0 );
		jd[i] = d->getJD();
		weekday[i] = ((int)(jd[i] + 2 )) % 7;

		if ( isLocaltime )
		{
			tzinfo = tzu.calculateTzInfoForJD( jd[i] );
			double tzdelta = tzinfo.tzhours + tzinfo.dsthours;
			jd[i] -= tzdelta / 24.0;

			// test for dst change during month
			if ( i > 0 )
			{
				if ( lastdst != tzinfo.dsthours )
				{
					//printf( "DST change day %d last is %f current %f\n", i+1, lastdst, tzinfo.dsthours );
					dstchange = true;
				}
			}
			lastdst = tzinfo.dsthours;
			//printf( "Ephem: tzdelta %f day %d\n", tzdelta, i+1 );
		}
	}
}

/*****************************************************
**
**   EphemExpert   ---   calcLength
**
******************************************************/
int EphemExpert::calcLength()
{
	int i, p, ret = 0;
	unsigned int i1;
	double dummy;
	Calculator *calculator = CalculatorFactory().getCalculator();

	planetdata.clear();
	for ( i1 = 0; i1 < chartprops->getPlanetList().size(); i1++ )
	{
		p = chartprops->getPlanetList()[i1];
		if ( p == OASCENDANT || p == OMERIDIAN || p > MAX_EPHEM_OBJECTS ) continue;
		EphemPlanetData pdata( p );
		for ( i = 0; i <= nb_days; i++ ) // calc length and retrogression
		{
			//d->setDate( i+1, month, year, -tz );
			d->setDate( jd[i] );
			calculator->calcPositionSpeed( d, p, pdata.len[i], dummy, pdata.speed[i], true, chartprops->isVedic() );
			pdata.retro[i] = ( pdata.speed[i] < 0 );
			pdata.rasi[i] = getRasi( pdata.len[i] );
			pdata.nakshatra[i] = getNakshatra( pdata.len[i], N27 );
		}

		if ( pdata.len[0] == 0 && p != OARIES ) ret++;

		planetdata.push_back( pdata );
	}
	clen = true;
	return ret;
}

/*****************************************************
**
**   EphemExpert   ---   calcKP
**
******************************************************/
void EphemExpert::calcKP( const int &dasa )
{
	KPEvent *e;
	vector<KPEvent>::iterator iter;

	Calculator *calculator = CalculatorFactory().getCalculator();
	DasaExpert *expert = DasaExpertFactory::get()->getDasaExpert( dasa );
	if ( ! expert->hasKpFeatures() || dasa == 2 ) return;

	if ( !clen  ) calcLength();
	kpevents = expert->getKPEventList( planetdata[OMOON].len[0], planetdata[OMOON].len[nb_days], jd[0] ); // calculates all lord/sublord events

	for ( iter = kpevents.begin(); iter != kpevents.end(); iter++ )
	{
		e = (KPEvent*)(&(*iter));
		d->setDate( e->jd );
		e->jd = calculator->calcPlanetaryEvent( d, e->len, 1, chartprops->isVedic() ); // get the dates for the events
	}
	ckp = true;
	mydasa = dasa;
}

/*****************************************************
**
**   EphemExpert   ---   writeKp
**
******************************************************/
void EphemExpert::writeKp( Writer *writer, const int &dasa )
{
	Lang lang;
	KPEvent *e;
	int d, m, y, lord, wd;
	vector<KPEvent>::iterator iter;
	TzUtil tzu;
	wxString s, thetime;
	vector<wxString> v;
	Table *table = 0;
	DasaExpert *expert = DasaExpertFactory::get()->getDasaExpert( dasa );
	if ( ! expert->hasKpFeatures() || dasa == 2 )
	{
		s.Printf( _( "K.P. events not supported for %s Dasa." ), expert->getName());
		writer->writeLine( s );
		return;
	}

	Formatter *formatter = Formatter::get();
	if ( ! chartprops->isVedic() )
	{
		writer->writeLine( _( "Not supported in western mode." ));
		return;
	}
	if ( ! ckp || mydasa != dasa ) calcKP( dasa );
	writeHeaderInfo( writer );

	//s.Printf( _( "%s Dasa" ), expert->getName());
	//writer->writeParagraph( s );

	lord = -1;
	int line = 1;
	int index = -1;
	for ( iter = kpevents.begin(); iter != kpevents.end(); iter++ )
	{
		e = (KPEvent*)(&(*iter));
		if ( e->dasaindex != index )
		{
			index = e->dasaindex;
			if ( table )
			{
				if ( expert->isRasiDasaExpert()) writer->writeHeader2( writer->getSignName( lord, TLARGE ));
				else writer->writeHeader2( writer->getObjectName( lord, TLARGE ));
				writer->writeTable( *table );
				delete table;
				table = 0;
			}
			table = new Table ( 5, 10 );
			table->setHeader( 0, _( "Day" ));
			table->setHeader( 1, _( "Time" ));
			table->setHeader( 2, _( "Lord" ));
			table->setHeader( 3, _( "Sublord" ));
			table->setHeader( 4, _( "Lunar Position" ));
			line = 1;
		}
		assert( table );
		formatter->getDateIntsFromJD( e->jd, d, m, y );
		wd = ((int)(e->jd + 1.5 )) % 7;
		assert( wd >= 0 && wd < 7 );
		s.Printf( wxT( "%02d %s" ), d, (const wxChar*)wxString( (const wxChar*)lang.getWeekdayName( wd )).Left(2) );
		table->setEntry( 0, line, s );

		TzFormattedDate fd = tzu.getDateFormatted( e->jd, isLocaltime );
		thetime = fd.timeFormatted;
		//if ( dstchange ) thetime << wxT( " " ) << fd.timezoneFormatted;
		table->setEntry( 1, line, thetime );
		//table->setEntry( 1, line, formatter->getTimeFormatted( getTimeFromJD( e->jd  + tz/24 )) );

		if ( expert->isRasiDasaExpert() )
		{
			table->setEntry( 2, line, writer->getSignName( e->lord, TLARGE ));
			table->setEntry( 3, line, writer->getSignName( e->sublord, TLARGE ));
		}
		else
		{
			table->setEntry( 2, line, writer->getObjectName( e->lord, TLARGE ));
			table->setEntry( 3, line, writer->getObjectName( e->sublord, TLARGE ));
		}
		table->setEntry( 4, line, writer->getPosFormatted( e->len, false ) );

		lord = e->lord;
		line++;
	}
	if ( table )
	{
		if ( expert->isRasiDasaExpert() )	writer->writeHeader2( writer->getSignName( e->lord, TLARGE ));
		else writer->writeHeader2( writer->getObjectName( e->lord, TLARGE ));
		writer->writeTable( *table );
		delete table;
		table = 0;
	}
}

/*****************************************************
**
**   EphemExpert   ---   calcIngress
**
******************************************************/
int EphemExpert::calcIngress()
{
	int p;
	int ret = 0;
	if ( !clen  ) ret = calcLength();
	ingressEvents.clear();

	// Achtung: das geht nicht immer für den Mond, weil der mehr als ein Event am Tag haben kann
	for ( int i = 0; i < nb_days; i++ )
	{
		d->setDate( i+1, month, year, 12 );
		for ( unsigned int j = 0; j < planetdata.size(); j++ )
		{
			p = planetdata[j].pindex;
			if ( p == OARIES || p == OASCENDANT || p == OMERIDIAN ) continue;

			if( planetdata[j].rasi[i] != planetdata[j].rasi[i+1] )
			{
				testIngressEvent( planetdata[j].rasi[i], planetdata[j].rasi[i+1], p, 0, planetdata[j].retro[i] );
			}

			if ( chartprops->isVedic() )
			{
				// Nakshatra changes
				if ( planetdata[j].nakshatra[i] !=  planetdata[j].nakshatra[i+1] )
				{
					const int base_nak = planetdata[j].nakshatra[i];
					const int diff_nak = red27( planetdata[j].nakshatra[i+1] -  base_nak );
					switch( diff_nak )
					{
						case 1:
							testIngressEvent( base_nak, red27( base_nak + 1 ), p, 1, planetdata[j].retro[i] );
						break;
						case 2:
							testIngressEvent( base_nak, red27( base_nak + 1 ), p, 1, planetdata[j].retro[i] );
							testIngressEvent( red27( base_nak + 1 ), red27( base_nak + 2 ), p, 1, planetdata[j].retro[i+1] );
						break;
						default:
						break;
					}
				}
			}
		}
	}
	ingressEvents.sort( IngressEventSorter() );
	cingress = true;
	return ret;
}

/*****************************************************
**
**   EphemExpert   ---   testIngressEvent
**
******************************************************/
void EphemExpert::testIngressEvent( const int& t1, const int &t2, const int &planet, const int &type, const bool &r )
{
	double tjd, targetlen;
	if ( t1 != t2 )
	{
		Calculator *calculator = CalculatorFactory().getCalculator();
		targetlen = ( r ? t1 : t2 ) * ( type ? 13.3333333333 : 30 );
		tjd = calculator->calcPlanetaryEvent( d, targetlen, planet, chartprops->isVedic() );
		ingressEvents.push_back( IngressEvent( tjd, planet, t2, type ));
	}
}

/*****************************************************
**
**   EphemExpert   ---   writeHeaderInfo
**
******************************************************/
void EphemExpert::writeHeaderInfo( Writer *writer )
{
	if ( ! show_header ) return;

	Lang lang;
	wxString s;
	s.Printf( wxT ( "%s %d" ), (const wxChar*)lang.getMonthName( month-1 ), year );
	writer->writeHeader1( s );
}

/*****************************************************
**
**   EphemExpert   ---   writeIngress
**
******************************************************/
int EphemExpert::writeIngress( Writer *writer )
{
	int ret = 0;
	Lang lang;
	IngressEvent *e;
	list<IngressEvent>::iterator iter;
	wxString s, buf, pname, thetime;
	TzUtil tzu;

	if ( ! cingress ) ret = calcIngress();
	writeHeaderInfo( writer );

	Table table( 4, ingressEvents.size()+1 );
	table.setHeader( 0, _( "Day" ));
	table.setHeader( 1, _( "Time" ));
	table.setHeader( 2, _( "Planet" ));
	table.setHeader( 3, chartprops->isVedic() ? _( "Sign/Nakshatra" ) : _( "Sign" ));

	int line = 1;
	for ( iter = ingressEvents.begin(); iter != ingressEvents.end(); iter++ )
	{
		e = (IngressEvent*)(&(*iter));
		if ( e->type == 0 )
			buf = writer->getSignName( e->which, TLARGE );
		else
			buf = lang.getNakshatraName( e->which, N27, TLARGE );

		pname = writer->getObjectName( e->planet, TLARGE, chartprops->isVedic() );

		TzFormattedDate fd = tzu.getDateFormatted( e->jd, isLocaltime );
		thetime = fd.timeFormatted;
		//if ( dstchange ) thetime << wxT( " " ) << fd.timezoneFormatted;

		s.Printf( wxT( "%02d %s" ), fd.dayOfMonth, fd.weekDay.c_str());
		if ( testDayIndexForCurrent( fd.dayOfMonth )) table.setHeaderEntry( 0, line, s );
		else	table.setEntry( 0, line, s );

		table.setEntry( 1, line, thetime );
		table.setEntry( 2, line, pname );
		table.setEntry( 3, line, buf );
		line++;
	}
	writer->writeTable( table );
	return ret;
}

/*****************************************************
**
**   EphemExpert   ---   addLunarEvent
**
******************************************************/
void EphemExpert::addLunarEvent( const int &stdate, const double& target, const double& sdiff, const double &ediff )
{
	double slen, mlen, cjd;
	Calculator *calculator = CalculatorFactory().getCalculator();

	d->setDate( jd[stdate] + ( target - sdiff ) / ( ediff - sdiff ));

	cjd = calculator->calcSunMoonEvent( d, target, slen, mlen );
	double aya = calculator->calcAyanamsa( cjd, config->vAyanamsa );
	lunarEvents.push_back( LunarEvent( target, cjd,
	                                   chartprops->isVedic() ? red_deg( slen - aya ) : slen,
	                                   chartprops->isVedic() ? red_deg( mlen - aya ) : mlen ));
}


/*****************************************************
**
**   EphemExpert   ---   addLunarSpecialEvent
**
******************************************************/
void EphemExpert::addLunarSpecialEvent( const int &stdate, const double& target, const double& sdiff, const double &ediff )
{
	if ( sdiff < target && ediff >= target ) addLunarEvent( stdate, target, sdiff, ediff );
}

/*****************************************************
**
**   EphemExpert   ---   calcLunar
**
******************************************************/
void EphemExpert::calcLunar()
{
	int i, t, t2;
	double diff, diff2;
	lunarEvents.clear();

	if ( !clen  ) calcLength();

	diff = red_deg( planetdata[OMOON].len[0] - planetdata[OSUN].len[0] );
	t = (int)(diff/12);
	for ( i = 1; i <= nb_days; i++ )
	{
		diff2 = red_deg( planetdata[OMOON].len[i] - planetdata[OSUN].len[i] );
		t2 = (int)(diff2/12);

		if ( t2 != t )
		{
			addLunarEvent( i-1, t2 * 12, diff, diff2 );

			// Happens once a month, i.e. double step
			if ( t2 - t > 1 )
			{
				addLunarEvent( i-1, ( t + 1 ) * 12, diff, diff2 );
			}
		}
		// Squares
		addLunarSpecialEvent( i-1, 90, diff, diff2 );
		addLunarSpecialEvent( i-1, 270, diff, diff2 );

		// Semi Squares
		addLunarSpecialEvent( i-1, 45, diff, diff2 );
		addLunarSpecialEvent( i-1, 135, diff, diff2 );
		addLunarSpecialEvent( i-1, 225, diff, diff2 );
		addLunarSpecialEvent( i-1, 315, diff, diff2 );

		diff = diff2;
		t = t2;
	}

	lunarEvents.sort( LunarEventSort() );
	clunar = true;
}

/*****************************************************
**
**   EphemExpert   ---   writeLunar
**
******************************************************/
void EphemExpert::writeLunar( Writer *writer )
{
	if ( ! clunar ) calcLunar();
	Lang lang;
	wxString s, thetime;
	list<LunarEvent>::iterator iter;
	LunarEvent *e;
	TzUtil tzu;

	writeHeaderInfo( writer );
	Table table( 7, lunarEvents.size()+1 );
	table.setHeader( 0, _( "Day" ));
	table.setHeader( 1, _( "Time" ));
	table.setHeader( 2, _( "Sun" ));
	table.setHeader( 3, _( "Moon" ));
	table.setHeader( 4, _( "Angle" ));
	table.setHeader( 5, _( "Tithi" ));
	table.setHeader( 6, _( "Western" ));

	int line = 1;
	for ( iter = lunarEvents.begin(); iter != lunarEvents.end(); iter++ )
	{
		e = (LunarEvent*)(&(*iter));
		TzFormattedDate fd = tzu.getDateFormatted( e->jd, isLocaltime );
		thetime = fd.timeFormatted;
		//if ( dstchange ) thetime << wxT( " " ) << fd.timezoneFormatted;

		s.Printf( wxT( "%02d %s" ), fd.dayOfMonth, fd.weekDay.c_str());
		if ( testDayIndexForCurrent( fd.dayOfMonth )) table.setHeaderEntry( 0, line, s );
		else	table.setEntry( 0, line, s );

		table.setEntry( 1, line, thetime );
		table.setEntry( 2, line, writer->getPosFormatted( e->slen, false ) );
		table.setEntry( 3, line, writer->getPosFormatted( e->mlen, false ) );

		int angle = (int)( red_deg( e->mlen - e->slen ) + .00001 );
		if ( angle >= 360 ) angle -= 360;
		s.Printf( wxT( "%d" ), angle );
		table.setEntry( 4, line, s );

		if ( ! ( angle % 12 ))
		{
			table.setEntry( 5, line, lang.getTithiName( angle / 12 ) );
		}
		if ( angle == 0 ) table.setEntry( 6, line, _( "New Moon" ));
		else if ( angle == 180 ) table.setEntry( 6, line, _( "Full Moon" ));
		else if ( angle == 60 || angle == 300 ) table.setEntry( 6, line, _( "Sextile" ));
		else if ( angle == 90 ) table.setEntry( 6, line, _( "Half Moon (Waxing)" ));
		else if ( angle == 120 || angle == 240 ) table.setEntry( 6, line, _( "Trine" ));
		else if ( angle == 270 ) table.setEntry( 6, line, _( "Half Moon (Waning)" ));

		// Semi squares
		else if (( angle == 45 || angle == 135 || angle == 225 || angle == 315 ))
			table.setEntry( 6, line, _( "Semi Square" ));
		line++;
	}
	writer->writeTable( table );
}

/*****************************************************
**
**   EphemExpert   ---   calcDetails
**
******************************************************/
void EphemExpert::calcDetails()
{
	int i;
	Calculator *calculator = CalculatorFactory().getCalculator();
	double mlen, slen, dummy, diff;

	for ( i = 0; i < nb_days; i++ )
	{
		d->setDate( i+1, month, year, 0 ); // Must be 0, correct offset will follow in formatting

		// Only the time will be affected: location ist default! (not 0 - 0)
		st[i] = calculator->calcSiderealTime( d->getJD(), d->getLocation()->getLongitude() );
		calculator->calcSunRiseSunSet( d, &sunrise[i], &sunset[i] );

		d->setDate( sunrise[i] );
		calculator->calcPosition( d, OMOON, mlen, dummy, false, false );
		calculator->calcPosition( d, OSUN, slen, dummy, false, false );
		diff = red_deg( mlen - slen );
		tithi[i] = (int)(diff/12);
	}

	cdetails = true;
}

/*****************************************************
**
**   EphemExpert   ---   writeDetails
**
******************************************************/
void EphemExpert::writeDetails( Writer *writer )
{
	int i;
	wxString s, tz_str, thetime;
	Lang lang;
	Formatter *formatter = Formatter::get();
	TzUtil tzu;

	if ( ! cdetails ) calcDetails();

	s.Printf( wxT ( "%s %d" ), (const wxChar*)lang.getMonthName( month-1 ), year );
	writer->writeHeader1( s );

	int line = 1;
	int nb_leaps = 0;
	for ( i = 0; i < nb_days; i++ )
	{
		if ( i > 0 && weekday[i] == 0 ) nb_leaps++;
	}
	Table table( 5, nb_days+1+nb_leaps );
	table.setHeader( 0, _( "Day" ));
	table.setHeader( 1, _( "Sidereal Time" ));
	table.setHeader( 2, _( "Sunrise" ));
	table.setHeader( 3, _( "Sunset" ));
	table.setHeader( 4, _( "Tithi (Sunrise)" ));

	line = 1;
	for ( i = 0; i < nb_days; i++ )
	{
		// blank line on weekend
		if ( i > 0 && weekday[i] == 0 )
		{
			for ( int j = 0; j < 5; j++ ) table.setHeaderEntry( j, line, wxEmptyString );
			line++;
		}

		s.Printf( wxT( "%02d %s" ), i+1, (const wxChar*)lang.getWeekdayName( weekday[i] ).Left(2) );
		if ( testDayIndexForCurrent( i+1 )) table.setHeaderEntry( 0, line, s );
		else	table.setEntry( 0, line, s );

		table.setEntry( 1, line, formatter->getTimeFormatted( st[i] ) );

		TzFormattedDate fd = tzu.getDateFormatted( sunrise[i], isLocaltime );
		thetime = fd.timeFormatted;
		//if ( dstchange ) thetime << wxT( " " ) << fd.timezoneFormatted;
		table.setEntry( 2, line, thetime );

		fd = tzu.getDateFormatted( sunset[i], isLocaltime );
		table.setEntry( 3, line, fd.timeFormatted );

		table.setEntry( 4, line, lang.getTithiName( tithi[i] ));
		line++;
	}
	writer->writeTable( table );
}

/*****************************************************
**
**   EphemExpert   ---   getLength
**
******************************************************/
double EphemExpert::getLength( const int &i, const int &day )
{
	return planetdata[i].len[day];
}

/*****************************************************
**
**   EphemExpert   ---   getSpeed
**
******************************************************/
double EphemExpert::getSpeed( const int &i, const int &day )
{
	assert( i >= 0 && i < (int)planetdata.size());
	return planetdata[i].speed[day];
}

/*****************************************************
**
**   EphemExpert   ---   getRetro
**
******************************************************/
bool EphemExpert::getRetro( const int &i, const int &day )
{
	assert( i >= 0 && i < (int)planetdata.size());
	return planetdata[i].retro[day];
}

/*****************************************************
**
**   EphemExpert   ---   calcMonth
**
******************************************************/
int EphemExpert::calcMonth()
{
	if ( !clen  ) return calcLength();
	else return 0;
}

/*****************************************************
**
**   EphemExpert   ---   testDayIndexForCurrent
**
******************************************************/
bool EphemExpert::testDayIndexForCurrent( const int &i )
{
	return ( month == current_month && year == current_year && i == current_day );
}

/*****************************************************
**
**   EphemExpert   ---   isCurrentMonth
**
******************************************************/
bool EphemExpert::isCurrentMonth()
{
	return ( month == current_month && year == current_year );
}

/*****************************************************
**
**   EphemExpert   ---   writeDefaultEphemeris
**
******************************************************/
int EphemExpert::writeDefaultEphemeris( Writer *writer )
{
	int ret = 0;
	wxString tz_str, s, rasi_str, nak_str;
	int i, j, deg, min, numcols;
	unsigned int i1;
	double rasilen;
	bool showrasi, shownak;
	wxString d;
	Lang lang;

	if ( !clen  ) ret = calcLength();
	writeHeaderInfo( writer );
	numcols = planetdata.size() + 1;

	int line = 1;
	int nb_leaps = 0;
	for ( i = 0; i < nb_days; i++ )
	{
		if ( i > 0 && weekday[i] == 0 ) nb_leaps++;
	}
	Table table( numcols, nb_days+nb_leaps+2 );
	table.setHeader( 0, _( "Day" ));

	// Header
	int col = 1;
	for ( i1 = 0; i1 < planetdata.size(); i1++ )
	{
		table.setHeader( col, writer->getObjectName( planetdata[i1].pindex, TLARGE, chartprops->isVedic() ));
		col++;
	}
	line = 1;
	for ( i = 0; i < nb_days; i++ )
	{
		//if ( i > 0 && weekday[i] == 0 ) o << Endl; // blank line on weekend
		if ( i > 0 && weekday[i] == 0 )
		{
			for ( j = 0; j < numcols; j++ ) table.setHeaderEntry( j, line, wxEmptyString );
			line++;
		}

		s.Printf( wxT( "%02d %s" ), i+1, (const wxChar*)lang.getWeekdayName( weekday[i] ).Left(2) );

		// write TZ if change during month
		/*
		if ( dstchange )
		{
			TzFormattedDate fd = tzutil.getDateFormatted( jd[i], isLocaltime );
			s << wxT( " " ) << fd.timezoneFormatted;
		}
		*/

		if ( testDayIndexForCurrent( i+1 )) table.setHeaderEntry( 0, line, s );
		else	table.setEntry( 0, line, s );

		col = 1;
		for ( i1 = 0; i1 < planetdata.size(); i1++ )
		{
			rasilen = getRasiLen( planetdata[i1].len[i] ) + .008333333;
			deg = (int)rasilen;
			min = (int)( 60 * ( rasilen - (double)deg ));

			// symbol for retrogression
			d = wxT( " " );
			if ( planetdata[i1].retro[i] ) {
				d = writer->getRetroSymbol();
			}
			else if ( i > 1 && planetdata[i1].retro[i] != planetdata[i1].retro[i-1] )
			{
				planetdata[i1].retro[i] ? d = writer->getRetroSymbol(): d = writer->getRetroSymbol( 1 );
			}
			else if ( i == nb_days - 1 && planetdata[i1].retro[i] ) {
				d = writer->getRetroSymbol();
			}

			showrasi = shownak = true;
			rasi_str = showrasi ? writer->getSignName( planetdata[i1].rasi[i] ) : wxT( "  " );
			nak_str = shownak ? lang.getNakshatraName( planetdata[i1].nakshatra[i], N27 ) : wxT( "   " );

			// ready to print now
			if ( config->useVedicPositions )
			{
				if ( chartprops->isVedic() )
				{
					s.Printf( wxT( "%s%02d-%02d-%02d %s" ), (const wxChar*)d, planetdata[i1].rasi[i], deg, min, (const wxChar*)nak_str );
				}
				else
				{
					s.Printf( wxT( "%s%02d-%02d-%02d" ), (const wxChar*)d, planetdata[i1].rasi[i], deg, min );
				}
			}
			else
			{
				if ( chartprops->isVedic() )
				{
					s.Printf( wxT( "%s%02d%s%02d %s" ), (const wxChar*)d, deg, (const wxChar*)rasi_str, min, (const wxChar*)nak_str );
				}
				else
				{
					s.Printf( wxT( "%s%02d%s%02d" ), (const wxChar*)d, deg, (const wxChar*)rasi_str, min );
				}
			}
			table.setEntry( col, line, s );

			col++;
		}

		line++;
	}
	// Header
	table.setHeaderEntry( 0, line, _( "Day" ));
	col = 1;
	for ( i1 = 0; i1 < planetdata.size(); i1++ )
	{
		table.setHeaderEntry( col, line, writer->getObjectName( planetdata[i1].pindex, TLARGE, chartprops->isVedic() ));
		col++;
	}
	writer->writeTable( table );
	return ret;
}

/*****************************************************
**
**   EphemExpert   ---   getPlanetRetro
**
******************************************************/
bool EphemExpert::getPlanetRetro( const unsigned int &plistindex, const int &day ) const
{
	assert( day >= 0 && day < MAX_DAY );
	assert( plistindex >= 0 && plistindex < MAX_EPHEM_OBJECTS );
	assert( planetdata.size() > 0 && plistindex <= planetdata.size());
	return planetdata[plistindex].retro[day];
}

/*****************************************************
**
**   EphemExpert   ---   getPlanetLength
**
******************************************************/
double EphemExpert::getPlanetLength( const unsigned int &plistindex, const int &day ) const
{
	assert( day >= 0 && day < MAX_DAY );
	assert( plistindex >= 0 && plistindex < MAX_EPHEM_OBJECTS );
	assert( planetdata.size() > 0 && plistindex <= planetdata.size());
	return planetdata[plistindex].len[day];
}

/*****************************************************
**
**   EphemExpert   ---   getPlanetSpeed
**
******************************************************/
double EphemExpert::getPlanetSpeed( const unsigned int &plistindex, const int &day ) const
{
	assert( day >= 0 && day < MAX_DAY );
	assert( plistindex >= 0 && plistindex < MAX_EPHEM_OBJECTS );
	assert( planetdata.size() > 0 && plistindex <= planetdata.size());
	return planetdata[plistindex].speed[day];
}

/*****************************************************
**
**   EphemExpert   ---   getPlanetRasi
**
******************************************************/
int EphemExpert::getPlanetRasi( const unsigned int &plistindex, const int &day ) const
{
	assert( day >= 0 && day < MAX_DAY );
	assert( plistindex >= 0 && plistindex < MAX_EPHEM_OBJECTS );
	assert( planetdata.size() > 0 && plistindex <= planetdata.size());
	return planetdata[plistindex].rasi[day];
}

/*****************************************************
**
**   EphemExpert   ---   getPlanetNakshatra
**
******************************************************/
int EphemExpert::getPlanetNakshatra( const unsigned int &plistindex, const int &day ) const
{
	assert( day >= 0 && day < MAX_DAY );
	assert( plistindex >= 0 && plistindex < MAX_EPHEM_OBJECTS );
	assert( planetdata.size() > 0 && plistindex <= planetdata.size());
	return planetdata[plistindex].nakshatra[day];
}

/*****************************************************
**
**   EphemExpert   ---   getPlanetId
**
******************************************************/
int EphemExpert::getPlanetId( const int &plistindex ) const
{
	assert( plistindex >= 0 && plistindex < MAX_EPHEM_OBJECTS );
	return planetdata[plistindex].pindex;
}

