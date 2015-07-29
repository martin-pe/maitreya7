/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/TextWriter.cpp
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
#pragma implementation "TextWriter.h"
#endif

#include "TextWriter.h"

/*****************************************************
**
**   TextWriter   ---   Constructor
**
******************************************************/
TextWriter::TextWriter( Config *c )
		: Writer( c )
{
	type = WRITER_TEXT;
}

/*****************************************************
**
**   TextWriter   ---   writeHeader1
**
******************************************************/
void TextWriter::writeHeader1( const wxString &s1 )
{
	s << s1 << Endl;
	for ( unsigned int i = 0; i < s1.Len(); i++ ) s << wxT( "-" );
	s << Endl << Endl;
}

/*****************************************************
**
**   TextWriter   ---   writeHeader2
**
******************************************************/
void TextWriter::writeHeader2( const wxString &s1 )
{
	s << s1 << Endl;
	for ( unsigned int i = 0; i < s1.Len(); i++ ) s << wxT( "-" );
	s << Endl;
}

/*****************************************************
**
**   TextWriter   ---   writeHeader3
**
******************************************************/
void TextWriter::writeHeader3( const wxString &s1 )
{
	s << s1 << Endl;
	for ( unsigned int i = 0; i < s1.Len(); i++ ) s << wxT( "-" );
	s << Endl;
}

/*****************************************************
**
**   TextWriter   ---   writeParagraph
**
******************************************************/
void TextWriter::writeParagraph( const wxString &s1 )
{
	s << s1 << Endl << Endl;
}

/*****************************************************
**
**   TextWriter   ---   writeLine
**
******************************************************/
void TextWriter::writeLine( const wxString &s1 )
{
	s << s1 << Endl;
}

/*****************************************************
**
**   TextWriter   ---   writeEntry
**
******************************************************/
void TextWriter::writeEntry( const wxString &entry, const int &len )
{
	wxString t = entry;
	for ( int i = 0; i < len; i++ ) t.Append( wxT( " " ));
	s << t.Left( len );
}

/*****************************************************
**
**   TextWriter   ---   writeTable
**
******************************************************/
void TextWriter::writeTable( Table &t, const bool repeatHeader )
{
	unsigned int c, r;
	TableEntry entry;
	vector<int> len;
	int ll, total_len = 0, i;
	Row row;

	for ( c = 0; c < t.nb_cols; c++ )
	{
		ll = 0;
		for ( r = 0; r < t.nb_rows; r++ )
		{
			ll = Max( (int)t.contents[r].value[c].value.Len(), ll );
		}
		len.push_back( ll+2 );
		total_len += ll+2;
	}

	row = t.contents[0];
	for ( c = 0; c < t.nb_cols; c++ )
	{
		entry = row.value[c];
		writeEntry( entry.value, len[c] );
	}
	s << Endl;
	for ( i = 0; i < total_len; i++ ) s << wxT( "-" );
	s << Endl;
	for ( r = 1; r < t.nb_rows; r++ )
	{
		row = t.contents[r];
		for ( c = 0; c < t.nb_cols; c++ )
		{
			entry = row.value[c];
			writeEntry(  entry.value, len[c] );
		}
		s << Endl;
	}
	s << Endl;
}

/*****************************************************
**
**   TextWriter   ---  writeListInColumns
**
******************************************************/
void TextWriter::writeListInColumns( const vector<wxString> v, const int &colnr )
{
	unsigned int i, j, index;
	unsigned int ePerCol = (int)v.size() / colnr;
	if ( (int)v.size() % colnr ) ePerCol++;

	for ( j = 0; j < ePerCol; j++ )
	{
		for ( i = 0; i < (unsigned int)colnr; i++ )
		{
			index = i*ePerCol + j;
			if ( index < v.size() ) s << v[index] << wxT( "  |  " );
		}
		s << Endl;
	}
	s << Endl;
}

/*****************************************************
**
**   TextWriter   ---  writeUranianDiagram
**
******************************************************/
void TextWriter::writeUranianDiagram( vector<UranianDiagram> mp )
{
	writeMidpointDiagramPart( mp, 0, ( mp.size() <= 13 ? mp.size() : 14 ));
	if ( mp.size() > 13 ) writeMidpointDiagramPart( mp, 14, mp.size() );

}

/*****************************************************
**
**   TextWriter   ---  writeMidpointDiagramPart
**
******************************************************/
void TextWriter::writeMidpointDiagramPart( vector<UranianDiagram> mp, const unsigned int &p1, const unsigned int &p2 )
{
	unsigned int i, j;
	unsigned int max_row = 0;
	vector<wxString> contents;

	for ( i = p1; i < p2; i++ ) {
		if ( mp[i].entries.size() > max_row ) max_row = mp[i].entries.size();
	}
	for ( i = 0; i < max_row+2; i++ ) contents.push_back( wxString());

	for ( i = p1; i < p2; i++ )
	{
		contents[0] << wxT( "  " ) << mp[i].master << wxT( "   " );
		contents[1] << wxT( "   |    " );
		for ( j = 2; j < max_row; j++ )
		{
			if ( mp[i].entries.size() >= j-1 )
			{
				contents[j] << mp[i].entries[j-2].p1 << wxT( "-+-" ) << mp[i].entries[j-2].p2 << wxT( " " );
			}
			else
			{
				//contents[j] << wxT( "------- " );
				contents[j] << wxT( "        " );
			}
		}
	}
	for ( i = 0; i < contents.size(); i++ ) s << contents[i] << Endl;
}

/*****************************************************
**
**   CsvWriter   ---   Constructor
**
******************************************************/
CsvWriter::CsvWriter( Config *c )
		: TextWriter( c )
{
	type = WRITER_CSV;
}

/*****************************************************
**
**   CsvWriter   ---   writeTable
**
******************************************************/
void CsvWriter::writeTable( Table &t )
{
	unsigned int c, r;
	TableEntry entry;
	Row row;
	const static wxChar separator = ';';

	row = t.contents[0];
	for ( c = 0; c < t.nb_cols; c++ )
	{
		entry = row.value[c];
		s << entry.value;
		if ( c < t.nb_cols - 1 ) s << separator;
	}
	s << Endl;
	for ( r = 1; r < t.nb_rows; r++ )
	{
		row = t.contents[r];
		for ( c = 0; c < t.nb_cols; c++ )
		{
			entry = row.value[c];
			s << entry.value;
			if ( c < t.nb_cols - 1 ) s << separator;
		}
		s << Endl;
	}
	s << Endl;
}

