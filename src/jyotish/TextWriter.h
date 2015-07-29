/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/TextWriter.h
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

#ifndef _TEXTWRITER_H_
#define _TEXTWRITER_H_

#ifdef __GNUG__
#pragma interface "TextWriter.h"
#endif

#include "Writer.h"

/*************************************************//**
*
*  Writer specialisation for plain text output
*
******************************************************/
class TextWriter : public Writer
{
public:

	TextWriter( Config* );
	virtual void beginWriting() {
		s.Clear();
	}
	virtual void endWriting() {}
	virtual void writeHeader1( const wxString& );
	virtual void writeHeader2( const wxString& );
	virtual void writeHeader3( const wxString& );
	virtual void writeParagraph( const wxString& );
	virtual void writeLine( const wxString& );
	virtual void writeTable( Table&, const bool repeatHeader = false );
	virtual void writeListInColumns( const vector<wxString>, const int &colnr );
	virtual void writeUranianDiagram( vector<UranianDiagram> );

private:
	void writeEntry( const wxString &entry, const int &len );
	void writeMidpointDiagramPart( vector<UranianDiagram> mp, const unsigned int &p1, const unsigned int &p2 );
};


/*************************************************//**
*
*  Writer specialisation for csv (spreadsheet) text output
*
******************************************************/
class CsvWriter : public TextWriter
{
public:

	CsvWriter( Config* );
	virtual void writeTable( Table& );
	virtual void writeHeader1( const wxString &s ) {
		writeLine( s );
	}
	virtual void writeHeader2( const wxString &s ) {
		writeLine( s );
	}
	virtual void writeHeader3( const wxString &s ) {
		writeLine( s );
	}

};

#endif


