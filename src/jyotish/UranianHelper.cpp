/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/UranianHelper.cpp
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
#pragma implementation "UranianHelper.h"
#endif

#include "UranianHelper.h"

#include "astrobase.h"
#include "ChartProperties.h"
#include "FileConfig.h"
#include "Horoscope.h"
#include "mathbase.h"
#include "Lang.h"
#include "Uranian.h"
#include "Writer.h"

IMPLEMENT_SINGLETON( UranianTextLoader )

/*****************************************************
**
**   UranianTextLoader   ---   Constructor
**
******************************************************/
UranianTextLoader::UranianTextLoader()
{
	wobjects[ wxT( "mc" ) ] = OMERIDIAN;
	wobjects[ wxT( "ar" ) ] = OARIES;
	wobjects[ wxT( "so" ) ] = OSUN;
	wobjects[ wxT( "as" ) ] = OASCENDANT;
	wobjects[ wxT( "mo" ) ] = OMOON;
	wobjects[ wxT( "kn" ) ] = OTRUENODE;
	wobjects[ wxT( "me" ) ] = OMERCURY;
	wobjects[ wxT( "ve" ) ] = OVENUS;
	wobjects[ wxT( "ma" ) ] = OMARS;
	wobjects[ wxT( "ju" ) ] = OJUPITER;
	wobjects[ wxT( "sa" ) ] = OSATURN;
	wobjects[ wxT( "ur" ) ] = OURANUS;
	wobjects[ wxT( "ne" ) ] = ONEPTUNE;
	wobjects[ wxT( "pl" ) ] = OPLUTO;
	wobjects[ wxT( "cu" ) ] = OCUPIDO;
	wobjects[ wxT( "ha" ) ] = OHADES;
	wobjects[ wxT( "ze" ) ] = OZEUS;
	wobjects[ wxT( "kr" ) ] = OKRONOS;
	wobjects[ wxT( "ap" ) ] = OAPOLLON;
	wobjects[ wxT( "ad" ) ] = OADMETOS;
	wobjects[ wxT( "vu" ) ] = OVULKANUS;
	wobjects[ wxT( "po" ) ] = OPOSEIDON;
}

/*****************************************************
**
**   UranianTextLoader   ---   Destructor
**
******************************************************/
UranianTextLoader::~UranianTextLoader()
{
	cleanup();
	ego = 0;
}

/*****************************************************
**
**   UranianTextLoader   ---   cleanup
**
******************************************************/
void UranianTextLoader::cleanup()
{
	unsigned int i;

	for ( i = 0; i < midpoint_e.size(); i++ ) delete midpoint_e[i];
	midpoint_e.clear();
	for ( i = 0; i < midpoint_w.size(); i++ ) delete midpoint_w[i];
	midpoint_w.clear();
	for ( i = 0; i < conjunction_w.size(); i++ ) delete conjunction_w[i];
	conjunction_w.clear();
}

/*****************************************************
**
**   UranianTextLoader   ---   getMidpointTextItem
**
******************************************************/
MidpointTextItem *UranianTextLoader::getMidpointTextItem( const int &pp, const int &pp1, const int &pp2, const int &mode )
{
	int p = pp == OMEANNODE ? OTRUENODE : pp;
	int p1 = pp1 == OMEANNODE ? OTRUENODE : pp1;
	int p2 = pp2 == OMEANNODE ? OTRUENODE : pp2;
	if ( mode )
	{
		// Witte
		if ( midpoint_w.empty()) loadItems( mode );
		for ( unsigned int i = 0; i < midpoint_w.size(); i++ )
		{
			if ( midpoint_w[i]->p == p &&
			        (( midpoint_w[i]->p1 == p1 && midpoint_w[i]->p2 == p2 ) || ( midpoint_w[i]->p1 == p2 && midpoint_w[i]->p2 == p1 ))
			   ) return midpoint_w[i];
		}
	}
	else
	{
		// Ebertin
		if ( midpoint_e.empty()) loadItems( mode );
		for ( unsigned int i = 0; i < midpoint_e.size(); i++ )
		{
			if ( midpoint_e[i]->p == p &&
			        (( midpoint_e[i]->p1 == p1 && midpoint_e[i]->p2 == p2 ) || ( midpoint_e[i]->p1 == p2 && midpoint_e[i]->p2 == p1 ))
			   ) return midpoint_e[i];
		}
	}
	return 0;
}

/*****************************************************
**
**   UranianTextLoader   ---   getConjunctionTextItem
**
******************************************************/
ConjunctionTextItem *UranianTextLoader::getConjunctionTextItem( const int &pp1, const int &pp2, const int &mode )
{
	int p1 = pp1 == OMEANNODE ? OTRUENODE : pp1;
	int p2 = pp2 == OMEANNODE ? OTRUENODE : pp2;
	if ( mode )
	{
		if ( conjunction_w.empty()) loadItems( mode );
		for ( unsigned int i = 0; i < conjunction_w.size(); i++ )
		{
			if (( conjunction_w[i]->p1 == p1 && conjunction_w[i]->p2 == p2 )
			        || ( conjunction_w[i]->p1 == p2 && conjunction_w[i]->p2 == p1 )) return conjunction_w[i];
		}
	}
	return 0;
}

/*****************************************************
**
**   UranianTextLoader   ---   loadItems
**
******************************************************/
bool UranianTextLoader::loadItems( const int &mode )
{
	int tabpos, p, p1, p2;
	wxString s, cp, cp1, cp2, theText;
	wxString filename = FileConfig::get()->getUranianTextFile( mode );
	if ( ! wxFile::Access( filename, wxFile::read )) return false;

	wxTextFile file( filename );
	file.Open();
	for ( s = file.GetFirstLine(); !file.Eof(); s = file.GetNextLine() )
	{
		// get tab position
		tabpos = s.Find( '\t' );
		if ( tabpos == 4 )
		{
			if ( mode == 1 )
			{
				// Conjunction event
				cp1 = s.Mid( 0, 2 );
				p1 = getTextItem( cp1 );
				cp2 = s.Mid( 2, 2 );
				p2 = getTextItem( cp2 );
				theText = s.Mid( 5 );
				if ( p1 != -1 && p2 != -1 && ! theText.IsEmpty() ) conjunction_w.push_back( new ConjunctionTextItem( p1, p2, theText ));
			}
		}
		else if ( tabpos == 6 )
		{
			// midpoint event
			cp = s.Mid( 0, 2 );
			p = getTextItem( cp );
			cp1 = s.Mid( 2, 2 );
			p1 = getTextItem( cp1 );
			cp2 = s.Mid( 4, 2 );
			p2 = getTextItem( cp2 );
			theText = s.Mid( 7 );
			if ( p != -1 && p1 != -1 && p2 != -1 && ! theText.IsEmpty() )
			{
				if ( mode ) midpoint_w.push_back( new MidpointTextItem( p, p1, p2, theText ));
				else midpoint_e.push_back( new MidpointTextItem( p, p1, p2, theText ));
			}
		}
		else
		{
			printf( "Wrong tabpos %d in Uranian filei: %s\n", tabpos, str2char( s ));
		}
	}
	return true;
}

/*****************************************************
**
**   UranianTextLoader   ---   getTextItem
**
******************************************************/
int UranianTextLoader::getTextItem( wxString s )
{
	int ret = wobjects[s];
	if ( ret == 0 && s != wxT ( "so" ) ) return -1;
	else return ret;
}

/*****************************************************
**
**   UranianHelper   ---   Constructor
**
******************************************************/
UranianHelper::UranianHelper( UranianExpert *expert, const ChartProperties *chartprops, Writer *writer, const UranianConfig *cfg )
		: expert( expert ),
		chartprops( chartprops ),
		writer( writer ),
		cfg( cfg )
{
}

/*****************************************************
**
**   UranianHelper   ---   writeMidpointTextAnalysis
**
******************************************************/
void UranianHelper::writeMidpointTextAnalysis( const Horoscope *h1, const Horoscope *h2, const bool show_date )
{
	bool witteMode = chartprops->getObjectStyle() & OBJECTS_INCLUDE_URANIAN;
	wxString filename = FileConfig::get()->getUranianTextFile( witteMode );

	// Silent return
	if ( ! wxFile::Access( filename, wxFile::read )) return;

	UranianTextLoader *loader = UranianTextLoader::get();
	list<UranianConjunctionEvent>::const_iterator citer;
	list<UranianMidpointEvent>::const_iterator miter;
	MidpointTextItem *mpitem;
	ConjunctionTextItem *ctitem;

	int count = 0;
	if ( witteMode && expert->conjunction_events.size() > 0 )
	{
		writer->writeHeader2( wxT( "Text: Conjunctions" ));
		for ( citer = expert->conjunction_events.begin(); citer != expert->conjunction_events.end(); citer++ )
		{
			ctitem = loader->getConjunctionTextItem( (*citer).p1, (*citer).p2, witteMode );
			//writer->writeHeader3( wxString::Format( wxT( "%d. %s" ), ++count, (const wxChar*)(*citer).write( writer, show_date )));
			if ( ctitem )
			{
				writer->writeHeader3( wxString::Format( wxT( "%d. %s" ), ++count, (const wxChar*)(*citer).write( writer, show_date )));
				writer->writeParagraph( ctitem->text );
			}
			// else writer->writeParagraph( wxT( "Error" ));
		}
	}

	count = 0;
	if ( expert->midpoint_events.size() > 0 )
	{
		writer->writeHeader2( wxT( "Text: Midpoints" ));
		for ( miter = expert->midpoint_events.begin(); miter != expert->midpoint_events.end(); miter++ )
		{
			mpitem = loader->getMidpointTextItem( (*miter).p, (*miter).p1, (*miter).p2, witteMode );

			writer->writeHeader3( wxString::Format( wxT( "%d. %s" ), ++count, (const wxChar*)(*miter).write( writer, show_date )));
			if ( mpitem )
			{
				//writer->writeHeader3( wxString::Format( wxT( "%d. %s" ), ++count, (const wxChar*)(*miter).write( writer, show_date )));
				writer->writeParagraph( mpitem->text );
			}
			else
				writer->writeParagraph( wxT( "Error" ));
		}
	}
}

/*****************************************************
**
**   UranianHelper   ---   writeRadixAnalysis
**
******************************************************/
void UranianHelper::writeRadixAnalysis( const Horoscope *h1, const Horoscope *h2 )
{
	writePlanetList( h1 );
	writer->writeHeader2( _( "Midpoints" ));
	writeMidpointList( h1 );

	writer->writeHeader2( _( "Conjunctions" ));
	writeConjunctions( h1, h2, false );

	writer->writeHeader2( _( "Midpoint Events" ));
	writeMidpointEvents( h1, h2, false );

	writeMidpointDiagram( h1, h2 );
	writeMidpointTextAnalysis( h1, h2 );
}

/*****************************************************
**
**   UranianHelper   ---   writePartnerAnalysis
**
******************************************************/
void UranianHelper::writePartnerAnalysis( const Horoscope *h1, const Horoscope *h2 )
{
	wxString s;

	writePlanetList( h1 );
	writer->writeHeader2( wxString::Format( _( "Midpoints (%s)" ), (const wxChar*)h1->getHName()));
	writeMidpointList( h1 );

	writer->writeHeader2( _( "Conjunctions" ));
	writeConjunctions( h1, h2, false );

	writer->writeHeader2( wxString::Format( _( "Planets of %s in Midpoints of %s" ),
	                                        (const wxChar*)h1->getHName(), (const wxChar*)h2->getHName()));
	writeMidpointEvents( h1, h2, false );

	writeMidpointDiagram( h1, h2 );
	writeMidpointTextAnalysis( h1, h2 );
}

/*****************************************************
**
**   UranianHelper   ---   writeTransitAnalysis
**
******************************************************/
void UranianHelper::writeTransitAnalysis( const Horoscope *h1, const Horoscope *h2 )
{
	writePlanetList( h1 );

	//writer->writeHeader2( _( "Running Planets in Conjunctions with Radix Planets" ));
	writer->writeHeader2( _( "Conjunctions" ));
	writeConjunctions( h1, h2, false );
	writer->writeHeader2( _( "Running Planets in Radix Midpoints" ));
	writeMidpointEvents( h1, h2, false );
	writeMidpointDiagram( h1, h2 );
	writeMidpointTextAnalysis( h1, h2 );
}

/*****************************************************
**
**   UranianHelper   ---   writePlanetList
**
******************************************************/
void UranianHelper::writePlanetList( const Horoscope *h )
{
	Formatter *formatter = Formatter::get();
	int colnr = 4;

	if ( cfg->circle >= CIRCLE_TYPE_225 )  colnr++;
	if ( cfg->circle >= CIRCLE_TYPE_1125 ) colnr++;
	if ( cfg->circle >= CIRCLE_TYPE_5625 ) colnr++;

	Table table( colnr, chartprops->getPlanetList().size() + 1 );
	table.setHeader( 0,  _( "Planet" ));
	table.setHeader( 1,  _( "Radix" ));
	table.setHeader( 2,  wxT(  "90" ) + writer->getDegreeSymbol());
	table.setHeader( 3,  wxT(  "45" ) + writer->getDegreeSymbol());
	if ( cfg->circle >= CIRCLE_TYPE_225 ) table.setHeader( 4,  wxT(  "22.5" ) + writer->getDegreeSymbol());
	if ( cfg->circle >= CIRCLE_TYPE_1125 ) table.setHeader( 5,  wxT(  "11.25" ) + writer->getDegreeSymbol());
	if ( cfg->circle >= CIRCLE_TYPE_5625 ) table.setHeader( 6,  wxT(  "5.625" ) + writer->getDegreeSymbol());

	for ( unsigned int i = 0; i < chartprops->getPlanetList().size(); i++ )
	{
		int p1 = chartprops->getPlanetList()[i];
		double len = h->getLength( p1, chartprops->isVedic() );
		table.setEntry( 0, i+1, writer->getObjectName( p1, TLARGE ) );
		table.setEntry( 1, i+1, writer->getPosFormatted( len ) );

		table.setEntry( 2, i+1, formatter->getDegreesFormatted( a_red( len, 90 )));
		table.setEntry( 3, i+1, formatter->getDegreesFormatted( a_red( len, 45 )));

		if ( cfg->circle >= CIRCLE_TYPE_225 )  table.setEntry( 4, i+1, formatter->getDegreesFormatted( a_red( len, 22.5 )));
		if ( cfg->circle >= CIRCLE_TYPE_1125 ) table.setEntry( 5, i+1, formatter->getDegreesFormatted( a_red( len, 11.25 )));
		if ( cfg->circle >= CIRCLE_TYPE_5625 ) table.setEntry( 6, i+1, formatter->getDegreesFormatted( a_red( len, 5.625 )));
	}
	writer->writeTable( table );
}

/*****************************************************
**
**   UranianHelper   ---   writeMidpointList
**
******************************************************/
void UranianHelper::writeMidpointList( const Horoscope *h )
{
	int i, j;
	Formatter *formatter = Formatter::get();
	double midpoint;
	wxString s;

	vector<int> l = chartprops->getPlanetList( OBJECTS_INCLUDE_ARIES );
	int pmax = l.size();

	double mod_degrees = getDegrees4CircleType( cfg->circle );

	Table table( pmax + 2, pmax + 2 );

	table.setHeader( 0,  wxEmptyString );
	table.setHeader( pmax + 1, wxEmptyString );
	table.setHeaderEntry( 0, pmax + 1,  wxEmptyString );
	table.setHeaderEntry( pmax + 1, pmax + 1, wxEmptyString );
	for ( i = 0; i < pmax; i++ )
	{
		table.setHeader( i+1, writer->getObjectName( l[i] ));
		table.setHeaderEntry( i + 1, pmax + 1, writer->getObjectName( l[i] ));
	}

	for ( i = 0; i < pmax; i++ )
	{
		table.setHeaderEntry( 0, i + 1, writer->getObjectName( l[i] ));
		for ( j = 0; j < pmax; j++ )
		{
			if ( i >= j )
			{
				// TODO
				midpoint = a_red(( h->getLength( l[i], chartprops->isVedic() )
				                   +  h->getLength( l[j], chartprops->isVedic() ) ) * .5, mod_degrees );
				if ( cfg->circle == 0 )
				{
					if ( planetDistance( midpoint, h->getLength( l[i], chartprops->isVedic() )) > 90 )
					{
						midpoint = red_deg( midpoint + 180 );
					}
					s = writer->getPosFormatted( midpoint );
				}
				else
				{
					s = formatter->getDegreesFormatted( midpoint );
				}
				table.setEntry( j + 1, i + 1, s );
			}
		}
		table.setHeaderEntry( pmax + 1, i + 1, writer->getObjectName( l[i] ));
	}
	writer->writeTable( table );
}

/*****************************************************
**
**   UranianHelper   ---   writeConjunctions
**
******************************************************/
void UranianHelper::writeConjunctions( const Horoscope *h1, const Horoscope *h2, const bool show_date )
{
	Formatter *formatter = Formatter::get();
	list<UranianConjunctionEvent>::iterator iter;
	vector<wxString> v;
	wxString s;

	if ( expert->conjunction_events.size() == 0 )
	{
		writer->writeParagraph( _( "No events found." ));
		return;
	}

	if ( expert->conjunction_events.size() <= 8 )
	{
		Table table( 3, expert->conjunction_events.size()+1 );

		wxString header = ( show_date ? _( "Date" ) : _( "Orbis" ));
		table.setHeader( 0, _( "No." ));
		table.setHeader( 1, _( "Event" ));
		table.setHeader( 2, header );
		table.col_alignment[0] = Align::Right;
		table.col_alignment[1] = Align::Center;
		table.col_alignment[2] = Align::Right;
		int line = 1;
		for ( iter = expert->conjunction_events.begin(); iter != expert->conjunction_events.end(); iter++ )
		{
			s.Printf( wxT( "%d" ), line );
			table.setHeaderEntry( 0, line, s );
			s.Printf( wxT( "%s %s %s" ), (const wxChar*)writer->getObjectName( (*iter).p1 ),
			          (const wxChar*)writer->getUranianAspectSymbol( (*iter).type ),
			          (const wxChar*)writer->getObjectName( (*iter).p2 ) );
			table.setEntry( 1, line, s );
			if ( show_date )
			{
				if ( (*iter).jd == 0 ) s = wxT( "---" );
				else s = formatter->getDateStringFromJD( (*iter).jd );
			}
			else
			{
				s.Printf( wxT( "%s%s" ), ( (*iter).orbis < 0 ? wxT( "-" ) : wxT( " " )),
				          (const wxChar*)formatter->getDegreesFormatted( fabs( (*iter).orbis )));
			}
			table.setEntry( 2, line, s );
			line++;
		}
		writer->writeTable( table );
	}
	else
	{
		int nb_lines = expert->conjunction_events.size() / 4;
		if ( expert->conjunction_events.size() % 4 ) nb_lines++;
		Table table( 15, nb_lines+1 );

		wxString header = ( show_date ? _( "Date" ) : _( "Orbis" ));
		table.setHeader( 0, _( "No." ));
		table.setHeader( 1, _( "Event" ));
		table.setHeader( 2, header );
		table.setHeader( 3, wxEmptyString );
		table.setHeader( 4, _( "No." ));
		table.setHeader( 5, _( "Event" ));
		table.setHeader( 6, header );
		table.setHeader( 7, wxEmptyString );
		table.setHeader( 8, _( "No." ));
		table.setHeader( 9, _( "Event" ));
		table.setHeader( 10, _( "Orbis" ));
		table.setHeader( 11, wxEmptyString );
		table.setHeader( 12, _( "No." ));
		table.setHeader( 13, _( "Event" ));
		table.setHeader( 14, header );
		table.col_alignment[0] = Align::Right;
		table.col_alignment[1] = Align::Center;
		table.col_alignment[2] = Align::Right;
		table.col_alignment[4] = Align::Right;
		table.col_alignment[5] = Align::Center;
		table.col_alignment[6] = Align::Right;
		table.col_alignment[8] = Align::Right;
		table.col_alignment[9] = Align::Center;
		table.col_alignment[10] = Align::Right;
		table.col_alignment[12] = Align::Right;
		table.col_alignment[13] = Align::Center;
		table.col_alignment[14] = Align::Right;
		int line = 1;
		int number = 1;
		int col = 0;
		for ( int j = 0; j < nb_lines; j++ )
		{
			table.setEntry( 3, j+1, wxEmptyString );
			table.setEntry( 7, j+1, wxEmptyString );
			table.setEntry( 11, j+1, wxEmptyString );
		}
		for ( iter = expert->conjunction_events.begin(); iter != expert->conjunction_events.end(); iter++ )
		{
			s.Printf( wxT( "%d" ), number );
			table.setHeaderEntry( 4 * col, line, s );
			s.Printf( wxT( "%s %s %s" ), (const wxChar*)writer->getObjectName( (*iter).p1 ),
			          (const wxChar*)writer->getUranianAspectSymbol( (*iter).type ),
			          (const wxChar*)writer->getObjectName( (*iter).p2 ) );
			table.setEntry( 4 * col+1, line, s );
			if ( show_date )
			{
				if ( (*iter).jd == 0 ) s = wxT( "---" );
				else s = formatter->getDateStringFromJD( (*iter).jd );
			}
			else
			{
				s.Printf( wxT( "%s%s" ), ( (*iter).orbis < 0 ? wxT( "-" ) : wxT( " " )),
				          (const wxChar*)formatter->getDegreesFormatted( fabs( (*iter).orbis )));
			}
			table.setEntry( 4 * col +2, line, s );
			number++;
			line++;
			if ( line > nb_lines )
			{
				line = 1;
				col++;
			}
		}
		writer->writeTable( table );
	}
}

/*****************************************************
**
**   UranianHelper   ---   writeMidpointEvents
**
******************************************************/
void UranianHelper::writeMidpointEvents( const Horoscope *h1, const Horoscope *h2, const bool show_date )
{
	list<UranianMidpointEvent>::const_iterator iter;
	vector<wxString> v;
	wxString s;
	Formatter *formatter = Formatter::get();

	if ( expert->midpoint_events.size() == 0 )
	{
		writer->writeParagraph( _( "No events found." ));
		return;
	}

	int nb_lines = expert->midpoint_events.size() / 4;
	if ( expert->midpoint_events.size() % 4 ) nb_lines++;
	Table table( 15, nb_lines+1 );

	wxString header = ( show_date ? _( "Date" ) : _( "Orbis" ));
	table.setHeader( 0, _( "No." ));
	table.setHeader( 1, _( "Event" ));
	table.setHeader( 2, header );
	table.setHeader( 3, wxEmptyString );
	table.setHeader( 4, _( "No." ));
	table.setHeader( 5, _( "Event" ));
	table.setHeader( 6, header );
	table.setHeader( 7, wxEmptyString );
	table.setHeader( 8, _( "No." ));
	table.setHeader( 9, _( "Event" ));
	table.setHeader( 10, header );
	table.setHeader( 11, wxEmptyString );
	table.setHeader( 12, _( "No." ));
	table.setHeader( 13, _( "Event" ));
	table.setHeader( 14, _( "Orbis" ));
	table.col_alignment[0] = Align::Right;
	table.col_alignment[1] = Align::Center;
	table.col_alignment[2] = Align::Right;
	table.col_alignment[4] = Align::Right;
	table.col_alignment[5] = Align::Center;
	table.col_alignment[6] = Align::Right;
	table.col_alignment[8] = Align::Right;
	table.col_alignment[9] = Align::Center;
	table.col_alignment[10] = Align::Right;
	table.col_alignment[12] = Align::Right;
	table.col_alignment[13] = Align::Center;
	table.col_alignment[14] = Align::Right;
	int line = 1;
	int number = 1;
	int col = 0;
	for ( int j = 0; j < nb_lines; j++ )
	{
		table.setEntry( 3, j+1, wxEmptyString );
		table.setEntry( 7, j+1, wxEmptyString );
		table.setEntry( 11, j+1, wxEmptyString );
	}
	vector<int> l = chartprops->getPlanetList( OBJECTS_INCLUDE_ARIES );

	for ( iter = expert->midpoint_events.begin(); iter != expert->midpoint_events.end(); iter++ )
	{
		s.Printf( wxT( "%d" ), number );
		table.setHeaderEntry( 4*col, line, s );
		s.Printf( wxT( "%s %s %s/%s" ), (const wxChar*)writer->getObjectName( (*iter).p ),
		          (const wxChar*)writer->getUranianAspectSymbol( (*iter).type ),
		          (const wxChar*)writer->getObjectName( (*iter).p1 ),
		          (const wxChar*)writer->getObjectName( (*iter).p2 ) );
		table.setEntry( 4*col+1, line, s );
		if ( show_date )
		{
			if ( (*iter).jd == 0 ) s = wxT( "---" );
			else s = formatter->getDateStringFromJD( (*iter).jd );
		}
		else
		{
			s.Printf( wxT( "%s%s" ), ( (*iter).orbis < 0 ? wxT( "-" ) : wxT( " " )),
			          (const wxChar*)formatter->getDegreesFormatted( fabs( (*iter).orbis )));
		}
		table.setEntry( 4 * col +2, line, s );
		number++;
		line++;
		if ( line > nb_lines )
		{
			line = 1;
			col++;
		}
	}
	writer->writeTable( table );
}

/*****************************************************
**
**   UranianHelper   ---   writeMidpointDiagram
**
******************************************************/
void UranianHelper::writeMidpointDiagram( const Horoscope *h1, const Horoscope *h2 )
{
	bool found;
	vector<UranianDiagram> mp;
	list<UranianMidpointEvent>::iterator iter;

	writer->writeHeader2( _( "Diagram" ));
	if ( expert->midpoint_events.size() == 0 )
	{
		writer->writeParagraph( _( "No events found." ));
		return;
	}
	vector<int> l = chartprops->getPlanetList( OBJECTS_INCLUDE_ARIES );
	for ( unsigned int i = 0; i < l.size(); i++ )
	{
		found = false;
		for ( iter = expert->midpoint_events.begin(); iter != expert->midpoint_events.end(); iter++ )
		{
			if ( (*iter).p == l[i] )
			{
				found = true;
				break;
			}
		}
		if ( found )
		{
			UranianDiagram diagram;
			diagram.master = writer->getObjectName( l[i], TMEDIUM );
			for ( iter = expert->midpoint_events.begin(); iter != expert->midpoint_events.end(); iter++ )
			{
				if ( (*iter).p != (int)i ) continue;
				diagram.entries.push_back( UranianDiagramEntry( writer->getObjectName( (*iter).p1 ),
				                           writer->getObjectName( (*iter).p2 )));
			}
			mp.push_back( diagram );
		}
	}
	writer->writeUranianDiagram( mp );
}


