/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/Writer.cpp
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
#pragma implementation "Writer.h"
#endif

#include "Writer.h"

#include <wx/string.h>

#include "ArabicParts.h"
#include "Aspect.h"
#include "astrobase.h"
#include "Conf.h"
#include "Lang.h"
#include "mathbase.h"

/*****************************************************
**
**   UranianDiagramEntry operator =
**
******************************************************/
UranianDiagramEntry UranianDiagramEntry::operator =( const UranianDiagramEntry &e )
{
	this->p1 = e.p1;
	this->p2 = e.p2;
	return *this;
}

/*****************************************************
**
**   UranianDiagram operator =
**
******************************************************/
UranianDiagram UranianDiagram::operator =( const UranianDiagram &e )
{
	this->master = e.master;
	this->entries.clear();
	for ( unsigned int i = 0; i < e.entries.size(); i++ )
	{
		this->entries.push_back( e.entries[i] );
	}
	return *this;
}

/*****************************************************
**
**   Row   ---   isEmpty
**
******************************************************/
bool Row::isEmpty()
{
	for ( unsigned int i = 0; i < value.size(); i++ )
	{
		if ( value[i].value != wxEmptyString || value[i].isHeader ) return false;
	}
	return true;
}

/*****************************************************
**
**   Table   ---   Constructor
**
******************************************************/
Table::Table( const unsigned int &cols, const unsigned int &rows )
{
	unsigned int r, c;

	show_grid = true;
	nb_rows = rows;
	nb_cols = cols;
	col_width.clear();
	left_p = 0;

	for ( c = 0; c < cols; c++ )
	{
		//col_width.push_back( 0 );
		col_alignment.push_back( Align::Left );
		col_line.push_back( false );
		col_empty.push_back( true );
		col_break.push_back( false );
		col_width.push_back( 0 );
	}

	row_height.clear();
	for ( r = 0; r < rows; r++ )
	{
		contents.push_back( Row());
		for ( c = 0; c < cols; c++ )
		{
			contents[r].value.push_back( TableEntry( wxEmptyString ));
		}
		row_height.push_back( 0 );
	}
}

/*****************************************************
**
**   Table   ---   setHeader
**
******************************************************/
void Table::setHeader( const unsigned int &col, const wxString &s1, const bool symbol, const int rightJoinLines )
{
	contents[0].value[col] = TableEntry( s1, true, symbol, rightJoinLines );
}

/*****************************************************
**
**   Table   ---   setHeaderEntry
**
******************************************************/
void Table::setHeaderEntry( const unsigned int&col, const unsigned int &row, const wxString &s1, const bool symbol )
{
	contents[row].value[col] = TableEntry( s1, true, symbol );
	if ( col_empty[col] &&  s1 != wxEmptyString )
	{
		col_empty[col] = false;
	}
}

/*****************************************************
**
**   Table   ---   setEntry
**
******************************************************/
void Table::setEntry( const unsigned int &col, const unsigned int &row, const wxString &s1, const bool symbol )
{
	if ( col_empty[col] &&  s1 != wxEmptyString ) col_empty[col] = false;
	contents[row].value[col] = TableEntry( s1, false, symbol );
}

/*****************************************************
**
**   Writer   ---   Constructor
**
******************************************************/
Writer::Writer( Config *c )
{
	cfg = c;
	lang = new Lang( cfg );
}

/*****************************************************
**
**   Writer   ---   Destructor
**
******************************************************/
Writer::~Writer()
{
	delete lang;
}

/*****************************************************
**
**   Writer   ---   getObjectName
**
******************************************************/
const wxString Writer::getObjectName( const int &num, const int &format, const bool vedic )
{
	if ( num < OFORTUNE ) return lang->getObjectName( num, format, vedic );
	else return ArabicPartsExpert::getObjectName( num, format );
}

/*****************************************************
**
**   Writer   ---   getRetroSymbol
**
******************************************************/
const wxString Writer::getRetroSymbol( const int type )
{
	return type? wxT( "D" ) : wxT( "R" );
}

/*****************************************************
**
**   Writer   ---   getSignName
**
******************************************************/
const wxString Writer::getSignName( const int &i, const int format )
{
	return lang->getSignName( i, format );
}

/*****************************************************
**
**   Writer   ---   getDegreeSymbol
**
******************************************************/
const wxString Writer::getDegreeSymbol()
{
	return wxT( " " );
}

/*****************************************************
**
**   Writer   ---   getMinuteSymbol
**
******************************************************/
const wxString Writer::getMinuteSymbol()
{
	return wxT( "\'" );
}

/*****************************************************
**
**   Writer   ---   getAspectSymbol
**
******************************************************/
const wxString Writer::getAspectSymbol( const int &i )
{
	return AspectExpert::getAspectShortDescription( i );
}

/*****************************************************
**
**   Writer   ---   getSecondSymbol
**
******************************************************/
const wxString Writer::getSecondSymbol()
{
	return wxT( "\"" );
}

/*****************************************************
**
**   Writer   ---   getPosFormatted
**
******************************************************/
wxString Writer::getPosFormatted( const double &len, const int &dir, const int &format )
{
	int deg, min, sign;
	double sec;
	wxString d = wxT( " " );
	const int pformat = cfg->signPrecision;
	wxString s;

	double mylen = red_deg( len );
	sign = (int)( mylen / 30 );
	mylen -= 30 * sign;

	getDegMinSecInts2( mylen, deg, min, sec );

	// Rounding
	if ( format == DEG_PRECISION_MINUTE && sec > 30 ) min+= 1;
	if ( format == DEG_PRECISION_SECOND && sec - (int)sec > .5 ) sec += .5;

	if ( dir == DIR_DIRECT ) d = wxEmptyString;
	else if ( dir == DIR_RETROGRADE ) d = getRetroSymbol();
	else if ( dir == DIR_STATIONARY ) d = wxT( "S" );
	else if ( dir == DIR_DIRECT_SHOW ) d = _( "D" );
	else if ( dir != DIR_NONE )
	{
		printf( "Error getPosFormatted : value %d not allowed, exiting..", dir );
		exit(1);
	}

	// Don't print direction
	if ( dir == -1 )
	{
		if ( format == DEG_PRECISION_SECOND )
		{
			if ( cfg->useVedicPositions )
				s.Printf( wxT( "%02d-%02d-%02d-%02d" ), sign, deg, min, (int)sec );
			else
			{
				s.Printf( wxT( "%02d%s%02d%s%02d%s %s" ), deg, (const wxChar*)getDegreeSymbol(), min,
				          (const wxChar*)getMinuteSymbol(), (int)sec, (const wxChar*)getSecondSymbol(),
				          (const wxChar*)getSignName(sign, pformat ));
			}
		}
		else if ( format == DEG_PRECISION_MORE )
		{
			if ( cfg->useVedicPositions )
				s.Printf( wxT( "%02d-%02d-%02d-%02d.%05d" ), sign, deg, min, (int)sec, (int)((sec-(int)sec)*100000));
			else
				s.Printf( wxT( "%02d%s%02d%s%02d.%05d\" %s" ), deg, (const wxChar*)getDegreeSymbol(), min,
				          (const wxChar*)getMinuteSymbol(),
				          (int)sec, (int)((sec-(int)sec)*100000),
				          (const wxChar*)getSignName(sign, format ));
		}
		else
		{
			if ( cfg->useVedicPositions )
				s.Printf( wxT( "%02d-%02d-%02d" ) , sign, deg, min );
			else
				s.Printf( wxT( "%02d%s%02d%s %s" ) , deg,
				          (const wxChar*)getDegreeSymbol(), min, (const wxChar*)getMinuteSymbol(),
				          (const wxChar*)getSignName(sign, pformat ));
		}
	}
	else
	{
		if ( format == DEG_PRECISION_SECOND )
		{
			if ( cfg->useVedicPositions )
				s.Printf( wxT( "%s%02d-%02d-%02d-%02d" ), (const wxChar*)d, sign, deg, min, (int)sec );
			else
				s.Printf( wxT( "%s%02d%s%02d%s%02d%s %s" ), (const wxChar*)d, deg,
				          (const wxChar*)getDegreeSymbol(), min, (const wxChar*)getMinuteSymbol(), (int)sec,
				          (const wxChar*)getSecondSymbol(),
				          (const wxChar*)getSignName(sign, pformat ));
		}
		else if ( format == DEG_PRECISION_MORE )
		{
			if ( cfg->useVedicPositions )
				s.Printf( wxT( "%s %02d-%02d-%02d-%02d.%05d\"" ),
				          (const wxChar*)d, sign, deg, min, (int)sec, (int)((sec-(int)sec)*100000));
			else
				s.Printf( wxT( "%s %02d%s%02d%s%02d.%05d%s %s" ),
				          (const wxChar*)d, deg, (const wxChar*)getDegreeSymbol(), min,
				          (const wxChar*)getMinuteSymbol(), (int)sec, (int)((sec-(int)sec)*100000),
				          (const wxChar*)getSecondSymbol(),
				          (const wxChar*)getSignName(sign, pformat ));
		}
		else
		{
			if ( cfg->useVedicPositions )
				s.Printf( wxT( "%s %02d-%02d-%02d" ), (const wxChar*)d, sign, deg, min );
			else
				s.Printf( wxT( "%s %02d%s%02ds%s %s" ), (const wxChar*)d, deg,
				          (const wxChar*)getDegreeSymbol(), min, (const wxChar*)getMinuteSymbol(),
				          (const wxChar*)getSignName(sign, pformat ));
		}
	}
	return s;
}

