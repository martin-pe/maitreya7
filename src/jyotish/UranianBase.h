/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/UranianBase.h
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
#ifndef _URANIANBASE_H_
#define _URANIANBASE_H_

#ifdef __GNUG__
#pragma interface "UranianBase.h"
#endif

#include "constants.h"

#include <vector>
#include <wx/string.h>

using namespace std;

class Writer;

/*************************************************//**
*
* \brief Midpoint of 2 planets in Uranian astrology
*
******************************************************/
class UranianMidpointValue
{
public:
	UranianMidpointValue( const int &p1, const int &p2, const double &value )
			: p1( p1 ),
			p2( p2 ),
			value( value ) {}

	const int p1, p2;
	const double value;
};

/*************************************************//**
*
* \brief represents a midpoint event in Uranian astrology (planet in midpoint of 2 other planets)
*
******************************************************/
class UranianMidpointEvent
{
public:
	UranianMidpointEvent( const int &p, const int &p1, const int &p2, const double &midpoint,
	                      const double &orbis, const int &type, const double jd = 0 );
	wxString write( Writer *writer, const bool show_date = false ) const;
	int p, p1, p2, type;
	double midpoint, orbis, jd;
};

/*************************************************//**
*
* \brief sorts midpoint events in Uranian astrology
*
******************************************************/
class UranianMidpointEventSorter
{
public:
	UranianMidpointEventSorter( const int &order ) {
		this->order = order;
	}
	bool operator()( const UranianMidpointEvent &e1, const UranianMidpointEvent &e2 ) const;
private:
	int order;
};

/*************************************************//**
*
* \brief holds information about Uranian events
*
******************************************************/
class MidpointMatchEvent
{
public:
	MidpointMatchEvent() {
		this->valid = false;
		this->type = -1;
		this->dist = 0;
	}
	MidpointMatchEvent( const int &type, const double &dist ) {
		this->valid = true;
		this->type = type;
		this->dist = dist;
	}
	bool valid;
	int type;
	double dist;
};

/*************************************************//**
*
* \brief represents a conjuntion in Uranian astrology
*
******************************************************/
class UranianConjunctionEvent
{
public:
	UranianConjunctionEvent( const int &p1, const int &p2, const double &orb, const int &type, const double jd = 0 );
	wxString write( Writer *writer, const bool show_date = false ) const;
	int p1, p2, type;
	double orbis, jd;
};

/*************************************************//**
*
* \brief sorts conjunction events in Uranian astrology
*
******************************************************/
class UranianConjunctionEventSorter
{
public:
	UranianConjunctionEventSorter( const int &order ) {
		this->order = order;
	}
	bool operator()( const UranianConjunctionEvent &e1, const UranianConjunctionEvent &e2 ) const;
private:
	int order;
};

/*************************************************//**
*
* \brief config for Uranian calculation (order, filter, circle type, orbis)
*
******************************************************/
class UranianConfig
{
public:
	UranianConfig();

	int order;
	int circle;
	double orbis;
	int filter;
	int yl_int;
	double year_length;
};

#endif

