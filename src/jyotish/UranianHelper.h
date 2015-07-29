/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/UranianHelper.h
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
#ifndef _URANIANHELPER_H_
#define _URANIANHELPER_H_

#ifdef __GNUG__
#pragma interface "UranianHelper.h"
#endif

#include "UranianBase.h"
#include "constants.h"

class ChartProperties;
class Horoscope;
class UranianExpert;
class Writer;

/*************************************************//**
*
* \brief
*
******************************************************/
class ConjunctionTextItem
{
public:
	ConjunctionTextItem( const int &p1, const int &p2, wxString text )
			: p1 ( p1 ), p2( p2 ), text( text )
	{}

	const int p1, p2;
	wxString text;
};

/*************************************************//**
*
* \brief
*
******************************************************/
class MidpointTextItem
{
public:
	MidpointTextItem( const int &p, const int &p1, const int &p2, wxString text )
			: p( p ), p1 ( p1 ), p2( p2 ), text( text )
	{}

	const int p, p1, p2;
	wxString text;
};

/*************************************************//**
*
* \brief
*
******************************************************/
class UranianTextLoader
{
	DECLARE_SINGLETON_WITHOUT_CONSTRUCTOR( UranianTextLoader )

public:
	~UranianTextLoader();

	void cleanup();

	MidpointTextItem *getMidpointTextItem( const int &p, const int &p1, const int &p2, const int &mode );
	ConjunctionTextItem *getConjunctionTextItem( const int &p1, const int &p2, const int &mode );

private:
	UranianTextLoader();

	int getTextItem( wxString s );
	bool loadItems( const int &mode );

	StringIntHash wobjects;
	vector<MidpointTextItem*> midpoint_e, midpoint_w;
	vector<ConjunctionTextItem*> conjunction_w;
};

/*************************************************//**
*
* \brief encapsulates report writing in Uranian astrology
*
******************************************************/
class UranianHelper
{
public:

	UranianHelper( UranianExpert *expert, const ChartProperties*, Writer*, const UranianConfig *cfg );

	void writeRadixAnalysis( const Horoscope*, const Horoscope* );
	void writePartnerAnalysis( const Horoscope*, const Horoscope* );
	void writeTransitAnalysis( const Horoscope*, const Horoscope* );
	void writeConjunctions( const Horoscope*, const Horoscope*, const bool show_date );
	void writeMidpointEvents( const Horoscope*, const Horoscope*, const bool show_date );
	void writeMidpointTextAnalysis( const Horoscope*, const Horoscope*, const bool show_date = false );

private:
	void writePlanetList( const Horoscope* );
	void writeMidpointList( const Horoscope* );
	void writeMidpointDiagram( const Horoscope*, const Horoscope* );

	UranianExpert *expert;
	const ChartProperties *chartprops;
	Writer *writer;
	const UranianConfig *cfg;

};


#endif

