/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/base/constants.cpp
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
#pragma implementation "constants.h"
#endif

#include "constants.h"

/**************************************************************
***
**  Static ids for text alignement
***
***************************************************************/
int Align::VCenter =  0x01;
int Align::Bottom  =  0x02;
int Align::Top     =  0x04;
int Align::HCenter =  0x08;
int Align::Left    = 	0x10;
int Align::Right   = 	0x20;
int Align::Center  =  Align::VCenter | Align::HCenter;

/*****************************************************
**
**   Align   --   toString
**
******************************************************/
wxString Align::toString( const int &a )
{
	wxString s;

	if ( a & Align::Left ) s = wxT( "Left+" );
	else if ( a & Align::Right ) s = wxT( "Right+" );
	else s = wxT( "HCenter+" );

	if ( a & Align::Top ) s += wxT( "Top" );
	else if ( a & Align::Bottom ) s += wxT( "Bottom" );
	else s += wxT( "VCenter" );

	return s;
};

