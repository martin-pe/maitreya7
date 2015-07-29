/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/HtmlWriter.h
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

#ifndef _HTMLWRITER_H_
#define _HTMLWRITER_H_

#ifdef __GNUG__
#pragma interface "HtmlWriter.h"
#endif

#include "Writer.h"

class wxFont;

/*************************************************//**
*
*  \brief Writer specialisation for HTML output (no style sheets)
*
******************************************************/
class HtmlWriter : public Writer
{
public:

	HtmlWriter( Config* );
	//virtual ~HtmlWriter() {}
	virtual void beginWriting();
	virtual void endWriting();
	virtual void writeHeader1( const wxString& );
	virtual void writeHeader2( const wxString& );
	virtual void writeHeader3( const wxString& );
	virtual void writeParagraph( const wxString& );
	virtual void writeLine( const wxString& );
	virtual void writeTable( Table&, const bool repeatHeader = false );
	virtual void writeListInColumns( const vector<wxString>, const int &colnr );
	virtual void writeUranianDiagram( vector<UranianDiagram> );

	virtual const wxString getObjectName( const int &num, const int &format = TMEDIUM, const bool vedic = false );

	virtual const wxString getSignName( const int &i, const int format = TMEDIUM );
	virtual const wxString getDegreeSymbol();
	virtual const wxString getMinuteSymbol();
	virtual const wxString getSecondSymbol();

	virtual const wxString getRetroSymbol( const int = 0 );
	virtual const wxString getAspectSymbol( const int & );
	virtual const wxString getUranianAspectSymbol( const int& );

private:

	void writeTableIntern( Table &t, const bool &paintFrame );
	void writeTableHeaderEntry( const TableEntry& );
	void writeMidpointDiagramPart( vector<UranianDiagram> mp, const unsigned int &p1, const unsigned int &p2 );
	wxFont *headerFont;
};

#endif


