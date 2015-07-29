/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/Aspect.h
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
#ifndef _ASPECT_H_
#define _ASPECT_H_

#ifdef __GNUG__
#pragma interface "Aspect.h"
#endif

#include <list>
#include <wx/colour.h>
#include <wx/string.h>

#include "constants.h"
#include "Expert.h"

class ChartProperties;
class Horoscope;
class Writer;

using namespace std;

/*************************************************//**
*
* \brief holds data an event in aspect calculation
*
******************************************************/
class AspectEvent
{
public:
	AspectEvent( const int &p1, const int &p2, const int &type, const double &orb )
	{
		planet1 = p1;
		planet2 = p2;
		aspectType = type;
		orbis = orb;
	}
	int planet1, planet2;
	int aspectType;
	double orbis;
};

/*************************************************//**
*
* \brief astrological and grphical configuration for a single aspect
*
******************************************************/
class AspectConfig
{
public:
	AspectConfig() {
		active = true;
		orbis = 1.0;
		width = 1;
		style = wxSOLID;
	}
	AspectConfig( const wxString& );
	AspectConfig( const bool &active, const double &orbis, const wxColour &color, const int& width, const int &style )
	{
		this->active = active;
		this->orbis = orbis;
		this->color = color;
		this->width = width;
		this->style =  style;
	}

	wxString toString();

	bool active;
	double orbis;
	wxColour color;
	int width, style;
};

/*************************************************//**
*
* \brief loads aspect configuration from config stores information about default config
*
******************************************************/
class AspectConfigLoader
{
public:
	static AspectConfigLoader *get();
	AspectConfig *getAspectConfig( const int& );
	const AspectConfig *getDefaultAspectConfig( const int& ) const;
	void setAspectConfig( const AspectConfig&, const int& );

private:
	AspectConfigLoader();
	static AspectConfigLoader *ego;
	AspectConfig configs[MAX_ASPECT_TYPES];
};

/*************************************************//**
*
* \brief encapsulates calcualtion of aspects for a given chart
*
******************************************************/
class AspectExpert : public Expert
{
public:
	AspectExpert() : Expert() {}
	AspectExpert( const Horoscope *h ) : Expert( h ) {}

	list<AspectEvent> &update( const Horoscope *h2, const ChartProperties *props, const int sortorder = ASPECT_SORT_PLANET1 );
	void sort( const int sortorder = ASPECT_SORT_PLANET1 );

	static wxString getAspectName( const int&i );
	static wxString getAspectSymbol( const int&i );
	static wxString getAspectDescription( const int&i );
	static wxString getAspectShortDescription( const int&i );

	list<AspectEvent> &getAspectList() {
		return al;
	}
	void writeWesternAspectList( Writer*, const ChartProperties* );
	void writeWesternAspectarium( Writer*, const ChartProperties* );

	void writeVedicAspectarium( const Horoscope*, Writer*, const ChartProperties*, const bool show_header = true );

private:

	list<AspectEvent> al;
};

#endif

