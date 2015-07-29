/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/Writer.h
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

#ifndef _WRITER_H_
#define _WRITER_H_

#ifdef __GNUG__
#pragma interface "Writer.h"
#endif

#include <wx/string.h>
#include <vector>

#include "constants.h"

using namespace std;

class Config;
class Lang;

/*************************************************//**
*
* \brief represents a single entry of a Table
*
******************************************************/
class TableEntry
{
public:
	TableEntry() {
		value = wxT( "" );
		isHeader = isSymbol = false;
		rightJoinLines = 0;
		isSkipped = false;
		width = 0;
	}
	TableEntry( const wxString &v, const bool header = false, const bool symbol = false, const int join = 0 )
	{
		value = v;
		isHeader = header;
		isSymbol = symbol;
		rightJoinLines = join;
		isSkipped = false;
		width = 0;
	}
	wxString value;
	bool isHeader;
	bool isSymbol;
	bool isSkipped;
	double width;
	int rightJoinLines;
};

/*************************************************//**
*
* \brief  represents a row in a Table
*
******************************************************/
class Row
{
public:
	Row() {
		max_height = 0;
	}
	vector<TableEntry> value;
	int max_height;
	bool isEmpty();
};

/*************************************************//**
*
* \brief Table for text, pdf and HTML output
*
******************************************************/
class Table
{
public:
	Table( const unsigned int &cols, const unsigned int &rows );
	void setHeader( const unsigned int &col, const wxString&, const bool symbol = false, const int rightJoinLines = 0 );
	void setHeaderEntry( const unsigned int&col, const unsigned int &row, const wxString&, const bool symbol = false );
	void setEntry( const unsigned int &col, const unsigned int &row, const wxString&, const bool = false );
	unsigned int nb_rows, nb_cols;
	vector<Row> contents;

	// formatting helpers for PDF output
	vector<double> col_width;
	vector<double> row_height;
	double left_p; // left border of table

	vector<int> col_alignment;

	// special frame for printout, instead of grid
	vector<bool> col_line;

	vector<bool> col_empty;

	// support line break for large text (Yogas)
	vector<bool> col_break;

	// Default: show grid
	bool show_grid;

protected:
};

/*************************************************//**
*
* \brief single entry in a Uranian diagram (Midpoint match)
*
******************************************************/
class UranianDiagramEntry
{
public:
	UranianDiagramEntry( wxString pp1, wxString pp2 ) {
		p1 = pp1;
		p2 = pp2;
	}
	wxString p1, p2;
	UranianDiagramEntry operator =( const UranianDiagramEntry& );
};

/*************************************************//**
*
* \brief Uranian midpoint match diagram
*
******************************************************/
class UranianDiagram
{
public:
	wxString master;
	vector<UranianDiagramEntry> entries;
	UranianDiagram operator =( const UranianDiagram& );
};


/*************************************************//**
*
* \brief abstract base class for writing items in text, HTML or PDF output
*
******************************************************/
class Writer
{
public:

	Writer( Config* );

	virtual ~Writer();

	/**
	 * start writing and write somethting like HTML header
	 */
	virtual void beginWriting() = 0;

	/**
	 * end writing and write somethting like final HTML tags
	 */
	virtual void endWriting() = 0;

	/**
	 * Write line in format Header1
	 */
	virtual void writeHeader1( const wxString& ) = 0;

	/**
	 * Write line in format Header2
	 */
	virtual void writeHeader2( const wxString& ) = 0;

	/**
	 * Write line in format Header3
	 */
	virtual void writeHeader3( const wxString& ) = 0;

	/**
	 * Write a paragraph with line feed
	 */
	virtual void writeParagraph( const wxString& ) = 0;

	/**
	 * Write a single line with line feed
	 */
	virtual void writeLine( const wxString& ) = 0;

	virtual const wxString getDegreeSymbol();
	virtual const wxString getMinuteSymbol();
	virtual const wxString getSecondSymbol();

	virtual const wxString getObjectName( const int &num, const int &format = TMEDIUM, const bool vedic = false );

	virtual const wxString getSignName( const int &i, const int format = TMEDIUM );

	virtual wxString getPosFormatted( const double &len, const int &dir = DIR_NONE,
	                                  const int &precision = DEG_PRECISION_SECOND );

	virtual const wxString getRetroSymbol( const int type = 0 );
	virtual const wxString getAspectSymbol( const int& );
	virtual const wxString getUranianAspectSymbol( const int& ) {
		return wxT( "=" );
	}

	virtual void writeListInColumns( const vector<wxString>, const int &colnr ) = 0;
	virtual void writeTable( Table&, const bool repeatHeader = false ) = 0;
	virtual void writeUranianDiagram( vector<UranianDiagram> ) = 0;
	wxString getContents() {
		return s;
	}
	int type;

protected:
	wxString s;
	Config *cfg;
	Lang *lang;
};

/*************************************************//**
*
* \brief This factory can create the different types of Writer instances.
*
* Supported types are:
*
* - WRITER_TEXT: for textclient (UNIX or DOS console), old format of GUI views in Maitreya 3
* - WRITER_HTML: nice HTML output for GUI client
* - WRITER_PDF : PDF output for printout
*
******************************************************/
class WriterFactory
{
public:
	Writer *getWriter( const int& type, Config* = 0 );
};

#endif


