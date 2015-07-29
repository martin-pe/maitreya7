/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/Aspect.cpp
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
#pragma implementation "Aspect.h"
#endif

#include "Aspect.h"

#include "astrobase.h"
#include "constants.h"
#include "ChartProperties.h"
#include "Conf.h"
#include "Horoscope.h"
#include "Lang.h"
#include "mathbase.h"
#include "Writer.h"

#include <math.h>
#include <wx/tokenzr.h>

extern Config *config;

// Aspect groups
enum { ASPECT_GROUP_MAJOR = 0, ASPECT_GROUP_MINOR, ASPECT_GROUP_TERNARY, ASPECT_GROUP_DECLINATION, ASPECT_GROUP_ANTI_DECLINATION };

/*************************************************//**
*
* \brief constants for aspect parameters and their defaults
*
******************************************************/
static const struct AspectTypeDefinition
{
	wxString name;
	int aspect_group;
	int numerator;
	int divisor; // (denominator) >= 1
	double angle;
	wxString shortname;
	wxString symbol;
}
AspectTypeDefinitions[MAX_ASPECT_TYPES] =
{
	// Major
	{ _( "Conjunction" ), ASPECT_GROUP_MAJOR, 1, 1, 0.0, wxT( "Cnj" ), wxT( "M" ) },
	{ _( "Opposition" ), ASPECT_GROUP_MAJOR, 1, 2, 180.0, wxT( "Opp" ), wxT( "N" ) },
	{ _( "Trine" ), ASPECT_GROUP_MAJOR, 1, 3, 120.0, wxT( "Tri" ), wxT( "P" ) },
	{ _( "Square" ), ASPECT_GROUP_MAJOR, 1, 4, 90.0, wxT( "Sqr" ), wxT( "O" ) },
	{ _( "Sextile" ), ASPECT_GROUP_MAJOR, 1, 6, 60.0, wxT( "Sxt" ), wxT( "Q" ) },

	// Minor
	{ _( "Quincunx" ), ASPECT_GROUP_MINOR, 5, 12, 150.0, wxT( "Qncx" ), wxT( "W" ) },
	{ _( "Semisquare" ), ASPECT_GROUP_MINOR, 1, 8, 45.0, wxT( "1/8" ), wxT( "R" ) },
	{ _( "Sesquisquare" ), ASPECT_GROUP_MINOR, 3, 8, 135.0, wxT( "3/8" ), wxT( "R" ) },
	{ _( "Semisextile" ), ASPECT_GROUP_MINOR, 1, 12, 30.0, wxT( "1/12" ), wxT( "V" ) },
	{ _( "Quintile" ), ASPECT_GROUP_MINOR, 1, 5, 72.0, wxT( "1/5" ), wxEmptyString },
	{ _( "Biquintile" ), ASPECT_GROUP_MINOR, 2, 5, 144.0, wxT( "2/5" ), wxEmptyString },

	// Declination
	{ _( "Parallele" ), ASPECT_GROUP_DECLINATION, 1, 1, 0.0, wxT( "Par" ), wxEmptyString },
	{ _( "Contraparallele" ), ASPECT_GROUP_ANTI_DECLINATION, 1, 2, -1.0, wxT( "CPar" ), wxEmptyString },

	// Ternary
	{ _( "Septile" ), ASPECT_GROUP_TERNARY, 1, 7, 51.42857, wxT( "1/7" ), wxEmptyString },
	{ _( "Biseptile" ), ASPECT_GROUP_TERNARY, 2, 7, 102.85714, wxT( "2/7" ), wxEmptyString },
	{ _( "Triseptile" ), ASPECT_GROUP_TERNARY, 3, 7, 154.28571, wxT( "3/7" ), wxEmptyString },
	{ _( "Novile" ), ASPECT_GROUP_TERNARY, 1, 9, 40.0, wxT( "1/9" ), wxEmptyString },
	{ _( "Binovile" ), ASPECT_GROUP_TERNARY, 2, 9, 80.0, wxT( "2/9" ), wxEmptyString },
	{ _( "Quadnovile" ), ASPECT_GROUP_TERNARY, 4, 9, 160.0, wxT( "4/9" ), wxEmptyString }
};
// Total: MAX_ASPECT_TYPES = 19

const static AspectConfig DefaultAspectConfigs[MAX_ASPECT_TYPES] =
{
	AspectConfig(  true, 5.0, wxColour( 255, 255, 0 ), 2, 0 ), // conj, yellow
	AspectConfig(  true, 3.0, wxColour( 0, 0, 255 ), 2, 0 ), // opp, blue
	AspectConfig(  true, 3.0, wxColour( 0, 255, 0 ), 2, 0 ), // trine, green
	AspectConfig(  true, 3.0, wxColour( 255, 0, 0 ), 2, 0 ), // square, red
	AspectConfig(  true, 3.0, wxColour( 0, 255, 0 ), 2, 0 ), // sextile, green

	// Minor
	AspectConfig(  true, 1.0, wxColour( 255, 0, 225 ), 1, 0 ), // Quincunx, 5/12
	AspectConfig(  false, 1.0, wxColour( 218, 89, 85 ), 1, 0 ),     // 1/8
	AspectConfig(  false, 1.0, wxColour( 218, 89, 85 ), 1, 0 ),     // 3/8
	AspectConfig(  false, 1.0, wxColour( 85, 218, 113 ), 1, 0 ),  // 1/12
	AspectConfig(  false, 1.0, wxColour( 85, 215, 218 ), 1, 0 ), // 1/5
	AspectConfig(  false, 1.0, wxColour( 85, 215, 218 ), 1, 0 ), // 2/5

	// Declination
	AspectConfig(  false, 2.0, wxColour( 133, 85, 218 ), 1, 0 ), // parallele
	AspectConfig(  false, 2.0, wxColour( 153, 218, 85 ), 1, 0 ), // antiparallel

	// Ternary
	AspectConfig(  false, 1.0, wxColour( 85, 189, 218 ), 1, 0 ), // 1/7
	AspectConfig(  false, 1.0, wxColour( 85, 189, 218 ), 1, 0 ), // 2/7
	AspectConfig(  false, 1.0, wxColour( 85, 189, 218 ), 1, 0 ), // 3/7
	AspectConfig(  false, 1.0, wxColour( 218, 85, 182 ), 1, 0 ), // 1/9
	AspectConfig(  false, 1.0, wxColour( 218, 85, 182 ), 1, 0 ), // 2/9
	AspectConfig(  false, 1.0, wxColour( 218, 85, 182 ), 1, 0 )  // 4/9
};

/*************************************************//**
*
* \brief sorts aspect lists
*
******************************************************/
class AspectSorter
{
public:
	AspectSorter( const int &ord ) {
		order = ord;
	}
	bool operator()( const AspectEvent &e1, const AspectEvent &e2 ) const
	{
		switch ( order )
		{
		case ASPECT_SORT_PLANET1:
			return ( e1.planet1 < e2.planet1 );
			break;
		case ASPECT_SORT_ORBIS:
			return( e1.orbis < e2.orbis );
			break;
		case ASPECT_SORT_ORBIS_ABSOLUTE:
			return( fabs( e1.orbis ) < fabs( e2.orbis ));
			break;
		case ASPECT_SORT_PLANET2:
			return ( e1.planet2 < e2.planet2 );
			break;
		case ASPECT_SORT_TYPE:
			if ( e1.aspectType != e2.aspectType ) return( e1.aspectType < e2.aspectType );
			return( fabs( e1.orbis ) < fabs( e2.orbis ));
			break;
		default:
			assert( false );
			break;
		}
		return false;
	}
private:
	int order;
};

/*****************************************************
**
**   AspectExpert   ---   update
**
******************************************************/
list<AspectEvent> &AspectExpert::update( const Horoscope *h2, const ChartProperties *chartprops, const int sortorder )
{
	assert( horoscope );
	assert( chartprops );

	unsigned int p1, p2;
	int i, ob1, ob2;
	double orbis;
	AspectConfigLoader *loader = AspectConfigLoader::get();
	vector<int> plist = chartprops->getWesternPlanetList() ;

	bool doubleSided = ( h2 != horoscope );
	if ( h2 == 0 ) h2 = horoscope;
	al.clear();

	for ( p1 = 0; p1 < plist.size(); p1++ )
	{
		ob1 = plist[p1];
		for ( p2 = ( doubleSided ? 0 : p1 + 1 ); p2 < plist.size(); p2++ )
		{
			if ( p1 == p2 ) continue;
			ob2 = plist[p2];

			// no aspects between northern/southern node
			if (( ob1 == OMEANNODE && ob2 == OMEANDESCNODE ) | ( ob2 == OMEANNODE && ob1 == OMEANDESCNODE )
			        | ( ob1 == OTRUENODE && ob2 == OTRUEDESCNODE ) | ( ob2 == OTRUENODE && ob1 == OTRUEDESCNODE )) continue;

			//printf( "P1 %d P2 %d dist %f Ddist %f anti Ddist %f\n", p1, p2, dist, dec_dist, anti_dec_dist );
			for ( i = 0; i < MAX_ASPECT_TYPES; i++ )
			{
				if ( ! loader->getAspectConfig( i )->active ) continue;
				switch ( AspectTypeDefinitions[i].aspect_group )
				{
				case ASPECT_GROUP_DECLINATION:
					orbis = fabs( fabs( h2->getLatitude( ob1 ) - horoscope->getLatitude( ob2 )) - AspectTypeDefinitions[i].angle );
					break;
				case ASPECT_GROUP_ANTI_DECLINATION:
					orbis = fabs( fabs( h2->getLatitude( ob1 ) + horoscope->getLatitude( ob2 )) - AspectTypeDefinitions[i].angle );
					break;
				default:
					orbis = fabs( planetDistance( h2->getWesternLength( ob1 ), horoscope->getWesternLength( ob2 )) - AspectTypeDefinitions[i].angle );
					break;
				}
				if ( orbis < loader->getAspectConfig( i )->orbis )
				{
					al.push_back( AspectEvent( ob1, ob2, i, orbis ));
				}
			}
		}
	}
	if ( sortorder != ASPECT_SORT_PLANET1 ) sort( sortorder );
	return al;
}

/*****************************************************
**
**   AspectExpert   ---   sort
**
******************************************************/
void AspectExpert::sort( const int sortorder )
{
	al.sort( AspectSorter( sortorder ));
}

AspectConfigLoader *AspectConfigLoader::ego = 0;
/*****************************************************
**
**   AspectConfigLoader   ---   Constructor
**
******************************************************/
AspectConfigLoader::AspectConfigLoader()
{
	wxString s;
	long a;
	for ( int i = 0; i < MAX_ASPECT_TYPES; i++ )
	{
		configs[i] = DefaultAspectConfigs[i];
		if ( ! config->wAspectConfig[i].IsEmpty())
		{
			wxStringTokenizer t( config->wAspectConfig[i], wxT( ":" ), wxTOKEN_RET_EMPTY );
			if ( t.HasMoreTokens() )
			{
				configs[i].active = ( t.GetNextToken() == wxT( "true" ) ? true : false );
			}
			if ( t.HasMoreTokens() )
			{
				t.GetNextToken().ToLong( &a );
				configs[i].orbis = (int)a;
			}
			if ( t.HasMoreTokens() )
			{
				configs[i].color = t.GetNextToken();
			}
			if ( t.HasMoreTokens() )
			{
				t.GetNextToken().ToLong( &a );
				configs[i].width = (int)a;
			}
			if ( t.HasMoreTokens() )
			{
				t.GetNextToken().ToLong( &a );
				configs[i].style = (int)a;
			}
		}
	}
}

/*****************************************************
**
**   AspectConfigLoader   ---   get
**
******************************************************/
AspectConfigLoader *AspectConfigLoader::get()
{
	if ( ego == 0 ) ego = new AspectConfigLoader();
	return ego;
}

/*****************************************************
**
**   AspectConfigLoader   ---   getAspectConfig
**
******************************************************/
AspectConfig *AspectConfigLoader::getAspectConfig( const int &i )
{
	assert( i >= 0 && i < MAX_ASPECT_TYPES );
	return &configs[i];
}

/*****************************************************
**
**   AspectConfigLoader   ---   getDefaultAspectConfig
**
******************************************************/
const AspectConfig *AspectConfigLoader::getDefaultAspectConfig( const int &i ) const
{
	assert( i >= 0 && i < MAX_ASPECT_TYPES );
	return &DefaultAspectConfigs[i];
}

/*****************************************************
**
**   AspectConfigLoader   ---   setAspectConfig
**
******************************************************/
void AspectConfigLoader::setAspectConfig( const AspectConfig &cfg, const int &i )
{
	assert( i >= 0 && i < MAX_ASPECT_TYPES );
	configs[i] = cfg;
	config->wAspectConfig[i] = configs[i].toString();
}

/*****************************************************
**
**   AspectConfig   ---   toString
**
******************************************************/
wxString AspectConfig::toString()
{
	wxString s;
	s << ( active ? wxT( "true" ) : wxT( "false" ))
	<< wxT( ":" ) << orbis
	<< wxT( ":" ) << color.GetAsString( wxC2S_HTML_SYNTAX )
	<< wxT( ":" ) << width
	<< wxT( ":" ) << style;
	return s;
}

/*****************************************************
**
**   AspectExpert   ---   getAspectSymbol
**
******************************************************/
wxString AspectExpert::getAspectSymbol( const int&i )
{
	assert( i >= 0 && i < MAX_ASPECT_TYPES );
	return AspectTypeDefinitions[i].symbol;
}

/*****************************************************
**
**   AspectExpert   ---   getAspectName
**
******************************************************/
wxString AspectExpert::getAspectName( const int&i )
{
	assert( i >= 0 && i < MAX_ASPECT_TYPES );
	return AspectTypeDefinitions[i].name;
}

/*****************************************************
**
**   AspectExpert   ---   getAspectShortDescription
**
******************************************************/
wxString AspectExpert::getAspectShortDescription( const int&i )
{
	assert( i >= 0 && i < MAX_ASPECT_TYPES );
	wxString s;
	if ( AspectTypeDefinitions[i].aspect_group == ASPECT_GROUP_DECLINATION ) s << wxT( "p" );
	else if ( AspectTypeDefinitions[i].aspect_group == ASPECT_GROUP_ANTI_DECLINATION ) s << wxT( "cp" );
	else s << AspectTypeDefinitions[i].numerator << wxT( "/" ) << AspectTypeDefinitions[i].divisor;
	return s;
}

/*****************************************************
**
**   AspectExpert   ---   getAspectDescription
**
******************************************************/
wxString AspectExpert::getAspectDescription( const int&i )
{
	assert( i >= 0 && i < MAX_ASPECT_TYPES );

	wxString s = wxGetTranslation( (const wxChar*)AspectTypeDefinitions[i].name );

	switch ( AspectTypeDefinitions[i].aspect_group )
	{
	case ASPECT_GROUP_DECLINATION:
	case ASPECT_GROUP_ANTI_DECLINATION:
		s << wxT( " (decl)" );
		break;
	case ASPECT_GROUP_MAJOR:
	case ASPECT_GROUP_MINOR:
	case ASPECT_GROUP_TERNARY:
	default:
		s << wxT( " (" ) << AspectTypeDefinitions[i].numerator << wxT( "/" ) << AspectTypeDefinitions[i].divisor << wxT( ")" );
		break;
	}
	return s;
}

/*****************************************************
**
**   AspectExpert   ---   writeWesternAspectarium
**
******************************************************/
void AspectExpert::writeWesternAspectarium( Writer *writer, const ChartProperties *chartprops )
{
	list<AspectEvent>::iterator iter;
	unsigned int p1, p2;
	wxString s, s1;

	vector<int> plist = chartprops->getWesternPlanetList();
	Table table( plist.size() + 1, plist.size() + 1 );

	table.setHeader( 0, wxEmptyString );
	for ( p1 = 0; p1 < plist.size(); p1++ )
	{
		table.setHeader( p1 + 1, writer->getObjectName( plist[p1], TMEDIUM ));
		table.setHeaderEntry( 0, p1 + 1, writer->getObjectName( plist[p1], TMEDIUM ));
		table.setEntry( p1 + 1, p1 + 1, wxT( "-" ));
	}
	for ( p1 = 0; p1 < plist.size(); p1++ )
	{
		for ( p2 = 0; p2 < plist.size(); p2++ )
		{
			if ( p1 == p2 ) continue;
			s.Clear();
			for ( iter = al.begin(); iter != al.end(); iter++ )
			{
				if ( (*iter).planet1 == plist[p1] && (*iter).planet2 == plist[p2] )
				{
					if ( ! s.IsEmpty() )
					{
						s << wxT( ", " );
					}
					s << writer->getAspectSymbol( (*iter).aspectType );
				}
			}
			table.setEntry( p1 + 1, p2 + 1, s );
		}
	}
	writer->writeTable( table );
}

/*****************************************************
**
**   AspectExpert   ---   writeVedicAspectarium
**
******************************************************/
void AspectExpert::writeVedicAspectarium( const Horoscope* h2, Writer *writer, const ChartProperties *chartprops, const bool show_header )
{
	int rasidiff, drishti;
	unsigned int p1, p2;
	wxString s, s1;

	if ( show_header ) writer->writeHeader1( _( "Vedic Aspectarium" ));

	vector<int> plist = chartprops->getVedicPlanetList();
	Table table( plist.size() + 1, plist.size() + 1 );

	table.setHeader( 0, wxEmptyString );
	for ( p1 = 0; p1 < plist.size(); p1++ )
	{
		table.setHeader( p1 + 1, writer->getObjectName( plist[p1], TMEDIUM ));
		table.setHeaderEntry( 0, p1 + 1, writer->getObjectName( plist[p1], TMEDIUM ));
		table.setEntry( p1 + 1, p1 + 1, wxT( "-" ));
	}
	for ( p1 = 0; p1 < plist.size(); p1++ )
	{
		for ( p2 = 0; p2 < plist.size(); p2++ )
		{
			if ( p1 == p2 ) continue;
			rasidiff = red12( getRasi( horoscope->getVedicLength( plist[p1] ))
			                  - getRasi( horoscope->getVedicLength( plist[p2] )));
			drishti = (int)(60.0 * getGrahaDrishtiValue( plist[p2], rasidiff ));
			table.setEntry( p1 + 1, p2 + 1, wxString::Format( wxT( "%d" ), drishti ));
		}
	}
	writer->writeTable( table );
}

/*****************************************************
**
**   AspectExpert   ---   writeWesternAspectList
**
******************************************************/
void AspectExpert::writeWesternAspectList( Writer *writer, const ChartProperties *chartprops )
{
	Formatter *formatter = Formatter::get();
	wxString s;
	int line = 1;

	Table table( 4, al.size() + 1 );
	table.setHeader( 0, _( "Type" ) );
	table.setHeader( 1, _( "Aspecting" ) );
	table.setHeader( 2, _( "Aspected" ) );
	table.setHeader( 3, _( "Orbis" ) );

	for ( list<AspectEvent>::iterator iter = al.begin(); iter != al.end(); iter++ )
	{
		s = writer->getAspectSymbol( (*iter).aspectType );
		table.setEntry( 0, line, s );
		table.setEntry( 1, line, writer->getObjectName( (*iter).planet1, TLARGE ));
		table.setEntry( 2, line, writer->getObjectName( (*iter).planet2, TLARGE ));
		table.setEntry( 3, line++,  formatter->getLenFormatted( (*iter).orbis ));
	}
	writer->writeTable( table, true );
}


