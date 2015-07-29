/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/textclient.cpp
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

#include <wx/app.h>
#include <wx/cmdline.h>
#include <wx/dir.h>
#include <wx/gdicmn.h>
#include <wx/string.h>
#include <wx/tokenzr.h>

#include "Aspect.h"
#include "Calculator.h"
#include "ChartProperties.h"
#include "Conf.h"
#include "Eclipse.h"
#include "Ephemeris.h"
#include "Hora.h"
#include "Horoscope.h"
#include "Lang.h"
#include "mathbase.h"
#include "Partner.h"
#include "PrintoutHelper.h"
#include "FileConfig.h"
#include "TextHelper.h"
#include "Transit.h"
#include "Uranian.h"
#include "Varga.h"
#include "VargaHoroscope.h"
#include "Writer.h"
#include "Yoga.h"

Config *config;

// wx2.9 workaround
#ifdef  _WX29
#define wxT1( x ) x
#define wxEmptyString1 ""
#else
#define wxT1( x ) wxT( x )
#define wxEmptyString1 wxEmptyString
#endif

static const wxCmdLineEntryDesc cmdLineDesc[] =
{
	{ wxCMD_LINE_SWITCH, wxEmptyString1, wxT1("help"), wxT1("show this help message\n"), wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP },

	{ wxCMD_LINE_OPTION, wxEmptyString1, wxT1("date"), wxT1("Set UT date YYYY-MM-DD HH:MM:SS"), wxCMD_LINE_VAL_STRING },
	{ wxCMD_LINE_OPTION, wxEmptyString1, wxT1("location"), wxT1("Set location \"name LL:LL BB:BB ZZ\"\n"), wxCMD_LINE_VAL_STRING },

	{ wxCMD_LINE_SWITCH, wxEmptyString1, wxT1("vedic"),   wxT1("Force Vedic mode (instead of default)") },
	{ wxCMD_LINE_SWITCH, wxEmptyString1, wxT1("western"),   wxT1("Force western mode (instead of default)") },
	{ wxCMD_LINE_SWITCH, wxEmptyString1, wxT1("html"), wxT1("print output in HTML format (instead of text)\n") },

	{ wxCMD_LINE_OPTION, wxEmptyString1, wxT1("year"), wxT1("set additional year (transit, Solar)"), wxCMD_LINE_VAL_NUMBER },
	{ wxCMD_LINE_OPTION, wxEmptyString1, wxT1("month"), wxT1("set additional month (transit)"), wxCMD_LINE_VAL_NUMBER },
	{ wxCMD_LINE_OPTION, wxEmptyString1, wxT1("day"), wxT1("set additional day (transit)\n"), wxCMD_LINE_VAL_NUMBER },

	// Methods without Horoscope class
	{ wxCMD_LINE_SWITCH, wxEmptyString1, wxT1("eclipses"), wxT1("Show eclipses") },
	{ wxCMD_LINE_SWITCH, wxEmptyString1, wxT1("hora"), wxT1("Show Hora") },
	{ wxCMD_LINE_SWITCH, wxEmptyString1, wxT1("ephemeris"), wxT1("Show ephemeris") },
	{ wxCMD_LINE_SWITCH, wxEmptyString1, wxT1("ephemeris-sunrise"), wxT1("Show ephemeris sunrise/Tithi") },
	{ wxCMD_LINE_SWITCH, wxEmptyString1, wxT1("ephemeris-ingress"), wxT1("Show ephemeris ingress events") },
	{ wxCMD_LINE_SWITCH, wxEmptyString1, wxT1("ephemeris-lunar"), wxT1("Show ephemeris Lunar events") },
	{ wxCMD_LINE_OPTION, wxEmptyString1, wxT1("ephemeris-kpevents"), wxT1("Show ephemeris kp events for Dasa #n\n") },

	// methods based upon Horoscope class
	{ wxCMD_LINE_OPTION, wxEmptyString1, wxT1("file"), wxT1("Open given file and use data for calculation"), wxCMD_LINE_VAL_STRING },

	{ wxCMD_LINE_SWITCH, wxEmptyString1, wxT1("base-data"),   wxT1("Show base data of chart") },
	{ wxCMD_LINE_SWITCH, wxEmptyString1, wxT1("planet-list"),   wxT1("List planets (like in main window, using switch for Vedic/western)") },
	{ wxCMD_LINE_SWITCH, wxEmptyString1, wxT1("vedicplanets"),   wxT1("Show vedic planets (like in TextView)") },
	{ wxCMD_LINE_SWITCH, wxEmptyString1, wxT1("westernplanets"), wxT1("Show western planets (like in TextView)") },
	{ wxCMD_LINE_SWITCH, wxEmptyString1, wxT1("kp"), wxT1("Show Krishnamurti Paddhati") },
	{ wxCMD_LINE_SWITCH, wxEmptyString1, wxT1("bhavas"),   wxT1("Show Bhavas") },
	{ wxCMD_LINE_SWITCH, wxEmptyString1, wxT1("nakshatras"),   wxT1("Show Nakshatras") },
	{ wxCMD_LINE_SWITCH, wxEmptyString1, wxT1("vargas"),   wxT1("Show Vargas") },
	{ wxCMD_LINE_SWITCH, wxEmptyString1, wxT1("ashtakavarga"),   wxT1("Show Ashtakavarga") },
	{ wxCMD_LINE_SWITCH, wxEmptyString1, wxT1("yogas"),   wxT1("Show Yogas") },
	{ wxCMD_LINE_SWITCH, wxEmptyString1, wxT1("jaimini"),   wxT1("Show Jaimini calculation") },
	{ wxCMD_LINE_SWITCH, wxEmptyString1, wxT1("astronomical"), wxT1("Show astronomical data") },
	{ wxCMD_LINE_SWITCH, wxEmptyString1, wxT1("arabic"), wxT1("Show Arabic parts") },
	{ wxCMD_LINE_OPTION, wxEmptyString1, wxT1("printsummary"), wxT1("Show printout summary nb #n (header of PDF output)"), wxCMD_LINE_VAL_NUMBER },


	{ wxCMD_LINE_OPTION, wxEmptyString1, wxT1("dasas"), wxT1("Show Dasa nb #n (short form)"), wxCMD_LINE_VAL_NUMBER },
	{ wxCMD_LINE_OPTION, wxEmptyString1, wxT1("dasas-compact"), wxT1("Show Dasa nb #n (compact form)"), wxCMD_LINE_VAL_NUMBER },
	{ wxCMD_LINE_OPTION, wxEmptyString1, wxT1("dasas-long"), wxT1("Show Dasa nb #n (short form)"), wxCMD_LINE_VAL_NUMBER },
	{ wxCMD_LINE_SWITCH, wxEmptyString1, wxT1("aspects"),   wxT1("Show aspects\n") },

	{ wxCMD_LINE_SWITCH, wxEmptyString1, wxT1("transits"),   wxT1("Show transits") },
	{ wxCMD_LINE_SWITCH, wxEmptyString1, wxT1("solar-arc"),   wxT1("Show Solar arc") },
	{ wxCMD_LINE_SWITCH, wxEmptyString1, wxT1("directions"),   wxT1("Show directions") },
	{ wxCMD_LINE_SWITCH, wxEmptyString1, wxT1("lunar-arc"),   wxT1("Show Lunar arc") },
	{ wxCMD_LINE_SWITCH, wxEmptyString1, wxT1("constant-arc"),   wxT1("Show constant arc") },
	{ wxCMD_LINE_SWITCH, wxEmptyString1, wxT1("solar"),   wxT1("Show Solar (Tajaka) positions\n") },

	{ wxCMD_LINE_SWITCH, wxEmptyString1, wxT1("uranian"), wxT1("Show Uranian Analysis") },
	{ wxCMD_LINE_SWITCH, wxEmptyString1, wxT1("uranian-yearlypreview"), wxT1("Show yearly preview (Uranian astrology)\n") },

	{ wxCMD_LINE_OPTION, wxEmptyString1, wxT1("file2"), wxT1("Open second file for partner chart"), wxCMD_LINE_VAL_STRING },
	{ wxCMD_LINE_SWITCH, wxEmptyString1, wxT1("partner-vedic"), wxT1("Show Vedic partner analysis") },
	{ wxCMD_LINE_SWITCH, wxEmptyString1, wxT1("partner-composite"), wxT1("Show partner composite chart") },
	{ wxCMD_LINE_NONE }
};
#undef wxT1
#undef wxEmptyString1

/*************************************************//**
**
*   CLASS MaitreyaTextclient
**
******************************************************/
class MaitreyaTextclient
{
public:

	/**************************************************************
	***
	**    MaitreyaTextclient   ---   Constructor
	***
	***************************************************************/
	MaitreyaTextclient()
	{
		h = h2 = 0;
		texthelper = 0;
		chartprops = 0;
		writer = 0;
		config = 0;
		writecount = 0;
	}

	/**************************************************************
	***
	**    MaitreyaTextclient   ---   Destructor
	***
	***************************************************************/
	~MaitreyaTextclient()
	{
		if ( config ) delete config;
		if ( texthelper ) delete texthelper;
		if ( chartprops ) delete chartprops;
		if ( writer ) delete writer;
		if ( h ) delete h;
		if ( h2 ) delete h2;
	}

	void run( int argc, wxChar** );

protected:
	Horoscope *h, *h2;
	ChartProperties *chartprops;
	DataSet dataset;
	Writer *writer;
	TextHelper *texthelper;
	int writecount;

	/**************************************************************
	***
	**    MaitreyaTextclient   ---   showEclipses
	***
	***************************************************************/
	void showEclipses()
	{
		double jd = dataset.getJD();
		EclipseExpert ec;
		ec.update( jd, jd + 365 * 3, 0, 0, chartprops->isVedic() );
		ec.write( writer, config->ephemTimezone );
		writecount++;
	}

	/**************************************************************
	***
	**    MaitreyaTextclient   ---   showHora
	***
	***************************************************************/
	void showHora()
	{
		HoraExpert expert;
		expert.update( dataset.getJD());
		expert.write( writer );
		writecount++;
	}

	/**************************************************************
	***
	**    MaitreyaTextclient   ---   showEphemeris
	***
	***************************************************************/
	void showEphemeris( const int &type, const int dasa = 0  )
	{
		int day, month, year;
		Formatter *formatter = Formatter::get();

		EphemExpert e( chartprops );
		e.setShowHeader( true );

		formatter->getDateIntsFromJD( dataset.getJD(), day, month, year );
		e.prepareMonth( month, year, 0 );

		switch ( type )
		{
		case 0:
			e.writeDefaultEphemeris( writer );
			break;
		case 1:
			e.writeDetails( writer );
			break;
		case 2:
			e.writeIngress( writer );
			break;
		case 3:
			e.writeLunar( writer );
			break;
		case 4:
			e.writeKp( writer, dasa );
			break;
		}
		writecount++;
	}

	/**************************************************************
	***
	**    MaitreyaTextclient   ---   showPlanets
	***
	***************************************************************/
	void showPlanets()
	{
		texthelper->writePlanets();
		writecount++;
	}

	/**************************************************************
	***
	**    MaitreyaTextclient   ---   showYogas
	***
	***************************************************************/
	void showYogas()
	{
		YogaExpert *expert = YogaExpert::get();
		VargaHoroscope chart( h, 0 );
		vector<unsigned int> v = expert->update( &chart, chart.getRasi( OASCENDANT ));
		expert->write( writer, v );
		writecount++;
	}


	/**************************************************************
	***
	**    MaitreyaTextclient   ---   showTajaka
	***
	***************************************************************/
	void showTajaka( const int &tajakayear )
	{
		TajakaHoroscope th( h->getDataSet() );
		th.calcTajaka( tajakayear, chartprops->isVedic() );
		th.update();
		TextHelper( &th, chartprops, writer ).writePlanets();
		writecount++;
	}

	/**************************************************************
	***
	**    MaitreyaTextclient   ---   showPrintoutSummary
	***
	***************************************************************/
	void showPrintoutSummary( int type )
	{
		PrintoutHelper ph( h, writer );
		ph.writePrintoutSummary( type );
		writecount++;
	}

	/**************************************************************
	***
	**    MaitreyaTextclient   ---   showTextAnalysis
	***
	***************************************************************/
	void showTextAnalysis( const int &mode, const int varga = 0, const int dasa = 0 )
	{
		texthelper->writeTextAnalysis( mode, varga, dasa );
		writecount++;
	}

	/**************************************************************
	***
	**    MaitreyaTextclient   ---   showTransits
	***
	***************************************************************/
	void showTransits( const int &transitmode, const double &jd )
	{
		TransitExpert expert( h, chartprops );
		expert.setTransitMode( transitmode );
		expert.setTransitDate( jd );
		expert.update();
		expert.writeTransitData( writer, false );

		AspectExpert aexpert( h );
		aexpert.update( expert.getTransitChart(), chartprops );
		aexpert.writeWesternAspectarium( writer, chartprops );
		writecount++;
	}

	/**************************************************************
	***
	**    MaitreyaTextclient   ---   showUranian
	***
	***************************************************************/
	void showUranian( const int &type, const int year = 0 )
	{
		UranianConfig cfg;
		UranianExpert expert( chartprops, writer, cfg );

		switch ( type )
		{
		case 1:
			expert.writeYearlyPreviewAnalysis( h, year );
			break;
		default:
			expert.writeRadixAnalysis( h );
			break;
		}
		writecount++;
	}

	/**************************************************************
	***
	**    MaitreyaTextclient   ---   showPartnerAnalysis
	***
	***************************************************************/
	void showPartnerAnalysis( const int &type )
	{
		if ( type == 0 )
		{
			VedicPartnerChart chart( h, h2 );
			chart.update();
			chart.write( writer );
		}
		else
		{
			CompositHoroscope ch;
			ch.update( h, h2 );
			ch.dump( writer, chartprops );
		}
		writecount++;
	}

	int getIntFromString( wxString s )
	{
		long l;
		s.ToLong( &l );
		return (int)l;
	}

	/**************************************************************
	***
	**    MaitreyaTextclient   ---   setDate
	***
	***************************************************************/
	void setDate( wxString s )
	{
		long  year, month, day, hour, minute, second;
		double time;

		if ( ! s.IsEmpty())
		{
			year = month = day = hour = minute = second = 0;
			wxStringTokenizer t( s, wxT( " -:" ));

			if ( t.HasMoreTokens()) year = getIntFromString( t.GetNextToken());
			if ( t.HasMoreTokens()) month = getIntFromString( t.GetNextToken());
			if ( t.HasMoreTokens()) day = getIntFromString( t.GetNextToken());
			if ( t.HasMoreTokens()) hour = getIntFromString( t.GetNextToken());
			if ( t.HasMoreTokens()) minute = getIntFromString( t.GetNextToken());
			if ( t.HasMoreTokens()) second = getIntFromString( t.GetNextToken());

			time = second + 60 * minute + 3600 * hour;
			time /= 3600;

			dataset.setDate( day, month, year, time );
		}
	}

	/**************************************************************
	***
	**    MaitreyaTextclient   ---   setLocation
	***
	***************************************************************/
	void setLocation( wxString s )
	{
		long ldeg, lmin, bdeg, bmin, dst;
		double lon, lat, tz;
		wxString name;

		ldeg = lmin = bdeg = bmin = dst = 0;
		lon = lat = tz = 0;
		wxStringTokenizer t( s, wxT( " :" ));

		if ( t.HasMoreTokens()) name = t.GetNextToken();
		if ( t.HasMoreTokens()) ldeg = getIntFromString( t.GetNextToken());
		if ( t.HasMoreTokens()) lmin = getIntFromString( t.GetNextToken());
		if ( t.HasMoreTokens()) bdeg = getIntFromString( t.GetNextToken());
		if ( t.HasMoreTokens()) bmin = getIntFromString( t.GetNextToken());
		if ( t.HasMoreTokens()) tz = myatof( t.GetNextToken());

		lon = lmin + 60 * ldeg;
		lon /= 60;
		lat = bmin + 60 * bdeg;
		lat /= 60;
		dataset.setLocation( name, lon, lat, tz, dst );
	}
};

/**************************************************************
***
**    MaitreyaTextclient   ---   run
***
***************************************************************/
void MaitreyaTextclient::run( int argc, wxChar **argv )
{
	long a;
	wxString s;
	writecount = 0;
	int tyear, tmonth, tday;
	double tjd;
	Formatter *formatter = Formatter::get();

	// TODO ugly and not working for installations e.g. in /opt/maitreya6
	wxString basedir = wxT( "." );
	if ( ! wxDir::Exists( wxT( "../xml/yogas" ))) basedir = wxT( "/usr/local/share/maitreya6" );
	if ( ! wxDir::Exists( basedir )) basedir = wxT( "/usr/share/maitreya6" );

	FileConfig::get()->init( basedir );
	chartprops = new ChartProperties;

	Calculator *calculator = CalculatorFactory().getCalculator();

	wxCmdLineParser parser( cmdLineDesc, argc, argv );
	parser.Parse( true );

	// create writer
	if ( parser.Found( wxT( "html" ) )) writer = WriterFactory().getWriter( WRITER_HTML );
	else writer = WriterFactory().getWriter( WRITER_TEXT );
	writer->beginWriting();

	// Vedic and western mode
	if ( parser.Found( wxT( "vedic" ))) chartprops->setVedic();
	if ( parser.Found( wxT( "western" ))) chartprops->setVedic( false );

	// set date
	if ( parser.Found( wxT( "date" ), &s )) setDate( s );
	if ( parser.Found( wxT( "location" ), &s )) setLocation( s );

	// additional year, month, day
	formatter->getDateIntsFromJD( dataset.getJD(), tday, tmonth, tyear );
	if ( parser.Found( wxT( "year" ), &a )) tyear = (int)a;
	if ( parser.Found( wxT( "month" ), &a )) tyear = (int)a;
	if ( parser.Found( wxT( "day" ), &a )) tday = (int)a;
	tjd = calculator->calc_jd( tyear, tmonth, tday, 0 );

	if ( parser.Found( wxT( "eclipses" ) )) showEclipses();
	if ( parser.Found( wxT( "hora" ) )) showHora();

	// ephemeris
	if ( parser.Found( wxT( "ephemeris" ) )) showEphemeris( 0 );
	if ( parser.Found( wxT( "ephemeris-sunrise" ) )) showEphemeris( 1 );
	if ( parser.Found( wxT( "ephemeris-ingress" ) )) showEphemeris( 2 );
	if ( parser.Found( wxT( "ephemeris-lunar" ) )) showEphemeris( 3 );
	if ( parser.Found( wxT( "ephemeris-kpevents" ), &a )) showEphemeris( 4, (int)a );

	// Create Horoscope
	h = new Horoscope();
	h->setDate( dataset.getJD());
	h->setLocation( *dataset.getLocation());

	texthelper = new TextHelper( h, chartprops, writer );
	if ( parser.Found( wxT( "file" ), &s  ))
	{
		if ( ! h->openFile( s ))
		{
			fprintf( stderr, "ERROR: could not open chart" );
			exit( 1 );
		}
	}
	h->update();

	if ( parser.Found( wxT( "planet-list" )))     showPlanets();
	if ( parser.Found( wxT( "base-data" )))       showTextAnalysis( TEXT_MODE_BASE );
	if ( parser.Found( wxT( "westernplanets" )))  showTextAnalysis( TEXT_MODE_WESTERN_PLANETS );
	if ( parser.Found( wxT( "vedicplanets" )))    showTextAnalysis( TEXT_MODE_VEDIC_PLANETS );
	if ( parser.Found( wxT( "bhavas" )))          showTextAnalysis( TEXT_MODE_BHAVA );
	if ( parser.Found( wxT( "kp" )))              showTextAnalysis( TEXT_MODE_KP );
	if ( parser.Found( wxT( "nakshatras" )))      showTextAnalysis( TEXT_MODE_NAKSHATRA );
	if ( parser.Found( wxT( "ashtakavarga" )))     showTextAnalysis( TEXT_MODE_ASHTAKAVARGA );
	if ( parser.Found( wxT( "jaimini" )))         showTextAnalysis( TEXT_MODE_JAIMINI );
	if ( parser.Found( wxT( "vargas" )))          showTextAnalysis( TEXT_MODE_VARGA );
	if ( parser.Found( wxT( "arabic" )))          showTextAnalysis( TEXT_MODE_ARABICPARTS );
	if ( parser.Found( wxT( "astronomical" )))    showTextAnalysis( TEXT_MODE_ASTRONOMICAL );
	if ( parser.Found( wxT( "dasas" ), &a ))      showTextAnalysis( TEXT_MODE_DASA, 0, (int)a );
	if ( parser.Found( wxT( "dasas-compact" ), &a )) showTextAnalysis( TEXT_MODE_DASACOMPACT, 0, (int)a );
	if ( parser.Found( wxT( "dasas-long" ), &a )) showTextAnalysis( TEXT_MODE_DASALONG, 0, (int)a );
	if ( parser.Found( wxT( "aspects" )))         showTextAnalysis(
		    chartprops->isVedic() ? TEXT_MODE_VEDIC_ASPECTARIUM : TEXT_MODE_WESTERN_ASPECTARIUM );

	if ( parser.Found( wxT( "printsummary" ), &a )) showPrintoutSummary( (int)a );

	if ( parser.Found( wxT( "yogas" )))        showYogas();
	if ( parser.Found( wxT( "transits" )))     showTransits( TRANSIT_MODE_TRANSIT, tjd );
	if ( parser.Found( wxT( "solar-arc" )))    showTransits( TRANSIT_MODE_SOLAR_ARC, tjd );
	if ( parser.Found( wxT( "directions" )))   showTransits( TRANSIT_MODE_DIRECTION, tjd );
	if ( parser.Found( wxT( "lunar-arc" )))    showTransits( TRANSIT_MODE_LUNAR_ARC, tjd );
	if ( parser.Found( wxT( "constant-arc" ))) showTransits( TRANSIT_MODE_CONSTANT_ARC, tjd );


	if ( parser.Found( wxT( "solar" ))) showTajaka( tyear );

	if ( parser.Found( wxT( "uranian" ))) showUranian( 0 );
	if ( parser.Found( wxT( "uranian-yearlypreview" ))) showUranian( 1, tyear );

	if ( parser.Found( wxT( "partner-vedic" )) || parser.Found( wxT( "partner-composite" )))
	{
		h2 = new Horoscope();
		if ( parser.Found( wxT( "file2" ), &s ))
		{
			if ( ! h2->openFile( s ))
			{
				fprintf( stderr, "ERROR: could not open chart" );
				exit(1);
			}
		}
		h2->update();
		if ( parser.Found( wxT( "partner-vedic" ))) showPartnerAnalysis( 0 );
		if ( parser.Found( wxT( "partner-composite" ))) showPartnerAnalysis( 1 );
	}

	if ( writecount == 0 ) parser.Usage();
	else
	{
		writer->endWriting();
		Cout( writer->getContents() );
	}
}

/*************************************************//**
**
*   CLASS TextClientApplication
**
******************************************************/
class TextClientApplication : public wxApp
{
public:

protected:
	virtual bool OnInit();
};

IMPLEMENT_APP_CONSOLE( TextClientApplication )
/**************************************************************
***
**    TextClientApplication   ---   OnInit
***
***************************************************************/
bool TextClientApplication::OnInit()
{
	SetAppName( APP_NAME );
	SetVendorName( VENDOR_NAME );
	MaitreyaTextclient textclient;
	textclient.run( argc, argv );
	return 0;
}
