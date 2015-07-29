/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/HtmlWriter.cpp
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
#pragma implementation "HtmlWriter.h"
#endif

#include "HtmlWriter.h"

#include "ArabicParts.h"
#include "Aspect.h"
#include "astrobase.h"
#include "ColorConfig.h"
#include "Conf.h"
#include "FontConfig.h"
#include "Lang.h"

#include <wx/font.h>


/*****************************************************
**
**   HtmlWriter   ---   Constructor
**
******************************************************/
HtmlWriter::HtmlWriter( Config *c )
		: Writer( c )
{
	FontConfig *f = FontConfig::get();
	headerFont = f->getHeaderFont();
	//textFont = f->getTextFont();
	type = WRITER_HTML;
}

/*****************************************************
**
**   HtmlWriter   ---   beginWriting
**
******************************************************/
void HtmlWriter::beginWriting()
{
	FontConfig *f = FontConfig::get();
	headerFont = f->getHeaderFont();

	s = wxT( "" );
	s << wxT( "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//en\">" ) << Endl;
	s << wxT( "<html>" ) << Endl;
	s << wxT( "<head>" ) << Endl;
	s << wxT( "</head>" ) << Endl;
	s << wxT( "<body " );
	s << wxT( " bgcolor=" ) << cfg->colors->bgColor.GetAsString( wxC2S_HTML_SYNTAX );
	s << wxT( " text=" ) << cfg->colors->fgColor.GetAsString( wxC2S_HTML_SYNTAX );
	s << wxT( ">" ) << Endl;
}

/*****************************************************
**
**   HtmlWriter   ---   endWriting
**
******************************************************/
void HtmlWriter::endWriting()
{
	s << wxT( "</body>" ) << Endl;
	s << wxT( "</html>" ) << Endl;
}

/*****************************************************
**
**   HtmlWriter   ---   writeHeader1
**
******************************************************/
void HtmlWriter::writeHeader1( const wxString &s1 )
{
	s << wxT( "<h1><font face=\"" ) << headerFont->GetFaceName() << wxT( "\" size=+2>" ) << s1
	<< wxT( "</font></h1>" ) << Endl;
}

/*****************************************************
**
**   HtmlWriter   ---   writeHeader2
**
******************************************************/
void HtmlWriter::writeHeader2( const wxString &s1 )
{
	s << wxT( "<h2><font face=\"" ) << headerFont->GetFaceName() << wxT( "\" size=+1>" ) << s1
	<< wxT( "</font></h2>" ) << Endl;
}

/*****************************************************
**
**   HtmlWriter   ---   writeHeader3
**
******************************************************/
void HtmlWriter::writeHeader3( const wxString &s1 )
{
	s << wxT( "<h3><font face=\"" ) << headerFont->GetFaceName() << wxT( "\">" ) << s1
	<< wxT( "</font></h3>" ) << Endl;
}

/*****************************************************
**
**   HtmlWriter   ---   writeParagraph
**
******************************************************/
void HtmlWriter::writeParagraph( const wxString &s1 )
{
	s << s1 << wxT( "<p>" ) << Endl;
}

/*****************************************************
**
**   HtmlWriter   ---   writeLine
**
******************************************************/
void HtmlWriter::writeLine( const wxString &s1 )
{
	s << s1 << wxT( "<br>" ) << Endl;
}

/*****************************************************
**
**   HtmlWriter   ---   writeTableHeaderEntry
**
******************************************************/
void HtmlWriter::writeTableHeaderEntry( const TableEntry &entry )
{
	s << wxT( "     <th nowrap bgcolor=" )
	<< cfg->colors->tableHeaderBgColor.GetAsString( wxC2S_HTML_SYNTAX )
	<< wxT( "><font face=\"" )
	<< headerFont->GetFaceName() << wxT( "\">" )
	<< entry.value
	<< wxT ( "</font></th>" ) << Endl;
}

/*****************************************************
**
**   HtmlWriter   ---   writeTable
**
******************************************************/
void HtmlWriter::writeTable( Table &t, const bool repeatHeader )
{
	writeTableIntern( t, true );
}

/*****************************************************
**
**   HtmlWriter   ---   writeTableIntern
**
******************************************************/
void HtmlWriter::writeTableIntern( Table &t, const bool &frame )
{
	unsigned int c, r;
	TableEntry entry;
	Row row;
	wxString wrap;

	if ( frame ) s << wxT( " <table border cellspacing=0 frame=void>" ) << Endl;
	else s << wxT( " <table>" ) << Endl;
	s << wxT( "		<tr>" ) << Endl;
	for ( r = 0; r < t.nb_rows; r++ )
	{
		row = t.contents[r];

		// look if it's empty
		if ( row.isEmpty() ) continue;

		s << wxT( "   <tr>" ) << Endl;
		for ( c = 0; c < t.nb_cols; c++ )
		{
			if ( t.col_break[c] ) wrap = wxEmptyString;
			else wrap = wxT( "nowrap" );
			entry = row.value[c];
			if ( entry.isHeader ) {
				writeTableHeaderEntry( entry );
			}
			else if ( t.col_empty[c] )
			{
				s << wxT( "<td width=10></td>" ) << Endl;
			}
			else
			{
				if ( t.col_alignment[c] & Align::Right )
					s << wxT( "     <td " ) << wrap << wxT( " align=right>" ) << entry.value  << wxT ( "</td>" ) << Endl;
				else if ( t.col_alignment[c] & Align::HCenter )
					s << wxT( "     <td " ) << wrap << wxT( " align=center>" ) << entry.value  << wxT ( "</td>" ) << Endl;
				else
					s << wxT( "     <td " ) << wrap << wxT( " align=left>" ) << entry.value  << wxT ( "</td>" ) << Endl;
			}
		}
		s << wxT( "   </tr>" ) << Endl;
	}
	s << wxT( "	</table><p>" ) << Endl;
}

/*****************************************************
**
**   HtmlWriter   ---  writeListInColumns
**
******************************************************/
void HtmlWriter::writeListInColumns( const vector<wxString> v, const int &colnr )
{
	unsigned int i, j, index;
	unsigned int ePerCol = (int)v.size() / colnr;
	if ( (int)v.size() % colnr ) ePerCol++;

	Table table( colnr, ePerCol );
	for ( i = 0; i < (unsigned int)colnr; i++ )
	{
		table.col_alignment[i] = Align::Left;
		for ( j = 0; j < ePerCol; j++ )
		{
			index = i*ePerCol + j;
			if ( index < v.size() ) table.setEntry( i, j, v[index] );
		}
		s << Endl;
	}
	//writeTableIntern( table, false );
	writeTableIntern( table, true );
}

/*****************************************************
**
**   HtmlWriter   ---  writeUranianDiagram
**
******************************************************/
void HtmlWriter::writeUranianDiagram( vector<UranianDiagram> mp )
{
	writeMidpointDiagramPart( mp, 0, ( mp.size() <= 13 ? mp.size() : 14 ));
	writeLine( wxEmptyString );
	if ( mp.size() > 13 ) writeMidpointDiagramPart( mp, 14, mp.size() );

}

/*****************************************************
**
**   HtmlWriter   ---  writeMidpointDiagramPart
**
******************************************************/
void HtmlWriter::writeMidpointDiagramPart( vector<UranianDiagram> mp, const unsigned int &p1, const unsigned int &p2 )
{
	unsigned int i, j, max_row = 0;
	wxString s1;

	for ( i = p1; i < p2; i++ ) max_row = Max( max_row, mp[i].entries.size());

	Table table( p2 - p1, max_row + 1 );
	for ( i = p1; i < p2; i++ )
	{
		table.setHeader( i-p1, mp[i].master );
		for ( j = 0; j < max_row; j++ )
		{
			if ( mp[i].entries.size() > j )
			{
				s1.Printf( wxT( "%s-%s" ), (const wxChar*)mp[i].entries[j].p1, (const wxChar*)mp[i].entries[j].p2);
				table.setEntry( i-p1, j + 1, s1 );
			}
		}
	}
	writeTableIntern( table, true );
}

/*****************************************************
**
**   HtmlWriter   ---   getSignName
**
******************************************************/
const wxString HtmlWriter::getSignName( const int &i, const int format )
{
	FontConfig *f = FontConfig::get();
	if ( ! cfg->useSignSymbols ) return Writer::getSignName( i, format );
	wxString s = wxT( "<font face=\"" ) + f->getSymbolFontName() + wxT( "\">" ) +
	             lang->getSignSymbolCode( i ) + wxT( "</font>" );
	return s;
}

/*****************************************************
**
**   HtmlWriter   ---   getObjectName
**
******************************************************/
const wxString HtmlWriter::getObjectName( const int &num, const int &format, const bool vedic )
{
	if ( num >= OFORTUNE ) return ArabicPartsExpert::getObjectName( num, format );
	else
	{
		FontConfig *f = FontConfig::get();
		if ( ! cfg->usePlanetSymbols ) return Writer::getObjectName( num, format, vedic );
		if ( num >= ODHUMA && num <= LAST_EXTENDED_OBJECT ) return Writer::getObjectName( num, format, vedic );

		wxString s = wxT( "<font face=\"" ) + f->getSymbolFontName() + wxT( "\">" )
		             + lang->getPlanetSymbolCode( num ) + wxT( "</font>" );
		return s;
	}
}

/*****************************************************
**
**   HtmlWriter   ---   getDegreeSymbol
**
******************************************************/
const wxString HtmlWriter::getDegreeSymbol()
{
	return wxT( "&deg;" );
}

/*****************************************************
**
**   HtmlWriter   ---   getMinuteSymbol
**
******************************************************/
const wxString HtmlWriter::getMinuteSymbol()
{
	return wxT( "&prime;" );
}

/*****************************************************
**
**   HtmlWriter   ---   getSecondSymbol
**
******************************************************/
const wxString HtmlWriter::getSecondSymbol()
{
	return wxT( "&Prime;" );
}


/*****************************************************
**
**   HtmlWriter   ---   getRetroSymbol
**
******************************************************/
const wxString HtmlWriter::getRetroSymbol( const int type )
{
	FontConfig *f = FontConfig::get();
	if ( ! cfg->useSignSymbols ) return Writer::getRetroSymbol( type );
	wxString s = wxT( "<font face=\"" ) + f->getSymbolFontName() + wxT( "\">" );
	if ( type ) s += wxT( "^" );
	else s += wxT( "_" );
	s += wxT( "</font>" );
	return s;
}

/*****************************************************
**
**   HtmlWriter   ---   getAspectSymbol
**
******************************************************/
const wxString HtmlWriter::getAspectSymbol( const int &a )
{
	FontConfig *f = FontConfig::get();
	ASSERT_VALID_ASPECT( a );

	wxString s = AspectExpert::getAspectSymbol( a );
	if ( s.IsEmpty() ) return Writer::getAspectSymbol( a );
	else return wxT( "<font face=\"" ) + f->getSymbolFontName() + wxT( "\">" ) + s + wxT( "</font>" );
}

/*****************************************************
**
**   HtmlWriter   ---   getUranianAspectSymbol
**
******************************************************/
const wxString HtmlWriter::getUranianAspectSymbol( const int &i )
{
	FontConfig *f = FontConfig::get();
	assert( i >= 0 && i < 7 );
	//printf( "HtmlWriter::getUranianAspectSymbol %d\n", i );

//enum { CIRCLE_TYPE_NONE = -1, CIRCLE_TYPE_360, CIRCLE_TYPE_180, CIRCLE_TYPE_90, CIRCLE_TYPE_45, CIRCLE_TYPE_225, CIRCLE_TYPE_1125, CIRCLE_TYPE_5625  };
	static const wxChar *event_symbol[7] = {
		wxT( "M" ),
		wxT( "N" ),
		wxT( "O" ),
		wxT( "R" ),
		wxT( "S" ),
		wxT( "T" ),
		wxT( "U" )
	};
	wxString s = wxT( "<font face=\"" ) + f->getSymbolFontName() + wxT( "\">" ) + event_symbol[i] + wxT( "</font>" );
	return s;
}

