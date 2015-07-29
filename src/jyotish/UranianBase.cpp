/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/UranianBase.cpp
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
#pragma implementation "UranianBase.h"
#endif

#include "UranianBase.h"

#include <math.h>

#include "astrobase.h"
#include "Conf.h"
#include "constants.h"
#include "Lang.h"
#include "Writer.h"

extern Config *config;

/*****************************************************
**
**   UranianMidpointEvent   ---   Constructor
**
******************************************************/
UranianMidpointEvent::UranianMidpointEvent( const int& p, const int &p1, const int &p2,
        const double &midpoint, const double &orbis, const int &type, const double jd )
{
	this->p = p;
	this->p1 = p1;
	this->p2 = p2;
	this->midpoint = midpoint;
	this->orbis = orbis;
	this->type = type;
	this->jd = jd;
}

/*****************************************************
**
**   UranianMidpointEvent   ---   write
**
******************************************************/
wxString UranianMidpointEvent::write( Writer *writer, const bool show_date ) const
{
	Formatter *formatter = Formatter::get();
	wxString s;

	s << writer->getObjectName( p )
	<< writer->getUranianAspectSymbol( type )
	<< writer->getObjectName( p1 )
	<< wxT( "/" ) << writer->getObjectName( p2 ) << wxT( " " )
	<< ( ( ! show_date && orbis < 0) ? wxT( "-" ) : wxT( " " ));
	if ( show_date ) s << formatter->getDateStringFromJD( jd );
	else s << formatter->getDegreesFormatted( fabs( orbis ));
	return s;
}

/*****************************************************
**
**   UranianConjunctionEvent   ---   Constructor
**
******************************************************/
UranianConjunctionEvent::UranianConjunctionEvent( const int &p1, const int &p2, const double &orbis, const int &type, const double jd )
{
	this->p1 = p1;
	this->p2 = p2;
	this->orbis = orbis;
	this->type = type;
	this->jd = jd;
}

/*****************************************************
**
**   UranianConjunctionEvent   ---   write
**
******************************************************/
wxString UranianConjunctionEvent::write( Writer *writer, const bool show_date ) const
{
	Formatter *formatter = Formatter::get();
	wxString s;

	s << writer->getObjectName( p1 )
	<< writer->getUranianAspectSymbol( type )
	<< writer->getObjectName( p2 )
	<< wxT( " " ) << ( ! show_date &&orbis > 0 ? wxT( "-" ) : wxT( " " ));
	if ( show_date ) s << formatter->getDateStringFromJD( jd );
	else s << formatter->getDegreesFormatted( fabs( orbis ));
	return s;
}

/*****************************************************
**
**   UranianMidpointEventSorter   ---   operator()
**
******************************************************/
bool UranianMidpointEventSorter::operator()( const UranianMidpointEvent &e1, const UranianMidpointEvent &e2 ) const
{
	switch ( order )
	{
	case ASPECT_SORT_PLANET2:
		// erst nach der MidpointValue (Planet2+3), dann nach dem ersten Planeten
		if ( e1.p1 != e2.p1 ) return ( e1.p1 < e2.p1 );
		if ( e1.p2 != e2.p2 ) return ( e1.p2 < e2.p2 );
		return ( e1.p < e2.p );
		break;
	case ASPECT_SORT_ORBIS:
		return( e1.orbis < e2.orbis );
		break;
	case ASPECT_SORT_ORBIS_ABSOLUTE:
		return( fabs( e1.orbis ) < fabs( e2.orbis ));
		break;
	case ASPECT_SORT_PLANET1:
		// erst nach dem 1. Planeten, dann nach den Planeten in der MidpointValue
		if ( e1.p != e2.p ) return ( e1.p < e2.p );
		if ( e1.p1 != e2.p1 ) return ( e1.p1 < e2.p1 );
		return ( e1.p2 < e2.p2 );
		break;
	case ASPECT_SORT_TYPE:
		if ( e1.type != e2.type ) return( e1.type < e2.type );
		return( fabs( e1.orbis ) < fabs( e2.orbis ));
		break;
	case ASPECT_SORT_JD:
	case ASPECT_SORT_DATE:
		return( e1.jd < e2.jd );
		break;
	case ASPECT_SORT_ORBIS_REVERSE:
		return( e1.orbis > e2.orbis );
		break;
	default:
		assert( false );
		break;
	}
	return false;
}

/*****************************************************
**
**   UranianConjunctionEventSorter   ---   operator()
**
******************************************************/
bool UranianConjunctionEventSorter::operator()( const UranianConjunctionEvent &e1, const UranianConjunctionEvent &e2 ) const
{
	switch ( order )
	{
	case ASPECT_SORT_PLANET2:
		return ( e1.p2 < e2.p2 );
		break;
	case ASPECT_SORT_ORBIS:
		return( e1.orbis < e2.orbis );
		break;
	case ASPECT_SORT_ORBIS_ABSOLUTE:
		return( fabs( e1.orbis ) < fabs( e2.orbis ));
		break;
	case ASPECT_SORT_PLANET1:
		return ( e1.p1 < e2.p1 );
		break;
	case ASPECT_SORT_TYPE:
		if ( e1.type != e2.type ) return( e1.type < e2.type );
		return( fabs( e1.orbis ) < fabs( e2.orbis ));
		break;
	case ASPECT_SORT_JD:
	case ASPECT_SORT_DATE:
		return( e1.jd < e2.jd );
		break;
	case ASPECT_SORT_ORBIS_REVERSE:
		return( e1.orbis > e2.orbis );
		break;
	default:
		assert( false );
		break;
	}
	return false;
}

/*****************************************************
**
**   UranianConfig   ---   Constructor
**
******************************************************/
UranianConfig::UranianConfig()
{
	order = config->planetSortOrder;
	circle = config->circleType;
	orbis = (double)config->orbisRadix / 60.0;
	filter = -1;
	yl_int = config->wYearLength;
	year_length = calcYearLength( yl_int, false );
}

