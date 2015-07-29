/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/base/PlanetList.h
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
#ifndef _PLANETLIST_H_
#define _PLANETLIST_H_

#ifdef __GNUG__
#pragma interface "PlanetList.h"
#endif

#include <vector>

using namespace std;

/*************************************************//**
*
* \brief creates lists of planets for Vedic and western astrology
*
******************************************************/
class PlanetList
{
public:
	vector<int> getWesternObjectList( const int &style, const int &extraObjects = 0 );
	//vector<int> getWesternObjectListWithAM( const int &style, const int &extraObjects = 0 );
	vector<int> getVedicObjectList( const int &style, const int &extraObjects = 0 );
	vector<int> getVedicObjectListWithoutLagna( const int &style, const int &extraObjects = 0 );
	vector<int> getDefaultVedicObjectList();
	vector<int> getDefaultVedicObjectListWithLagna();
	vector<int> getVedicPlanetOnlyList();

private:
	void addOuterObjectList( const int &style, const bool &vedic, vector<int> &r, const int &extraObjects = 0 );
	vector<int> getVedicObjectListInternal( const int &style, const bool &withLagna, const int &extraObjects = 0 );
	//vector<int> getWesternObjectListInternal( const int &style, const bool &withAM );

};

#endif


