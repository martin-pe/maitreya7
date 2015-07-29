/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/base/PlanetList.cpp
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
#pragma implementation "PlanetList.h"
#endif

#include "PlanetList.h"

#include "Conf.h"

extern Config *config;

/*****************************************************
**
**   PlanetList   ---   addOuterObjectList
**
******************************************************/
void PlanetList::addOuterObjectList( const int &style, const bool &vedic, vector<int> &r, const int &extraObjects )
{
	int i;
	unsigned int j;
	for ( i = OCUPIDO; i <= OLILITH; i++ )
	{
		if ( ! ( style & OBJECTS_INCLUDE_URANIAN ) && i >= OCUPIDO && i <= OPOSEIDON ) continue;
		if ( ! ( style & OBJECTS_INCLUDE_CHIRON ) && i == OCHIRON ) continue;
		if ( ! ( style & OBJECTS_INCLUDE_PHOLUS ) && i == OPHOLUS ) continue;
		if ( ! ( style & OBJECTS_INCLUDE_PLANETOIDS ) && i >= OCERES && i <= OVESTA ) continue;
		if ( ! ( style & OBJECTS_INCLUDE_LILITH ) && i == OLILITH ) continue;
		r.push_back( i );
	}
	for ( i = FIRST_EXTENDED_OBJECT; i <= LAST_EXTENDED_OBJECT; i++ )
	{
		if ( i == OARIES
		        && ( ! ( extraObjects & OBJECTS_INCLUDE_ARIES ) || ! ( style & OBJECTS_INCLUDE_ARIES ) || config->wPlanetOrder == 1 )) continue;
		if ( ! ( style & OBJECTS_INCLUDE_UPAGRAHAS ) && i >= ODHUMA && i <= OUPAKETU ) continue;
		if ( i >= OKALA && i <= OMANDI )
		{
			if ( ! ( style & OBJECTS_INCLUDE_KALAVELAS )) continue;
			else if ( config->kalaVelaLord[i-OKALA] == KALAVELA_LORD_NONE ) continue;
		}
		if ( i >= OBHAVA_LAGNA && i <= OGHATIKA_LAGNA  && ! ( style & OBJECTS_INCLUDE_SPECIALLAGNAS )) continue;
		if ( i == OD9LAGNA && ( ! ( extraObjects & OBJECTS_INCLUDE_D9_LAGNA ) || ! ( style & OBJECTS_INCLUDE_D9_LAGNA ))) continue;
		r.push_back( i );
	}

	if ( style & OBJECTS_INCLUDE_ARABICPARTS )
	{
		for ( j = 0; j < ( vedic ? config->vArabicParts.size() :  config->wArabicParts.size()); j++ )
		{
			r.push_back( OFORTUNE + ( vedic ? config->vArabicParts[j] : config->wArabicParts[j] ));
		}
	}
}

/*****************************************************
**
**   PlanetList   ---   getWesternObjectList
**
******************************************************/
vector<int> PlanetList::getWesternObjectList( const int &style, const int &extraObjects )
{
	vector<int> r;
	int i;
	const bool istruenode = config->wLunarNodeMode == LUNAR_NODE_TRUE;

	if ( config->wPlanetOrder == 1 )
	{
		if ( style & OBJECTS_INCLUDE_MERIDIAN ) r.push_back( OMERIDIAN );
		if ( style & OBJECTS_INCLUDE_ARIES && extraObjects & OBJECTS_INCLUDE_ARIES ) r.push_back( OARIES );
		r.push_back( OSUN );
		if ( style & OBJECTS_INCLUDE_ASCENDANT ) r.push_back( OASCENDANT );
		r.push_back( OMOON );
		if ( style & OBJECTS_INCLUDE_DRAGONHEAD ) r.push_back( istruenode ? OTRUENODE : OMEANNODE );
		if ( style & OBJECTS_INCLUDE_DRAGONTAIL ) r.push_back( istruenode ? OTRUEDESCNODE: OMEANDESCNODE );

		for ( i = OMERCURY; i <= OSATURN; i++ ) r.push_back( i );
		if ( style & OBJECTS_INCLUDE_OUTER )
		{
			r.push_back( OURANUS );
			r.push_back( ONEPTUNE );
			r.push_back( OPLUTO );
		}
		addOuterObjectList( style, false, r, extraObjects );
	}
	else
	{
		for ( i = OSUN; i <= OSATURN; i++ ) r.push_back( i );
		if ( style & OBJECTS_INCLUDE_OUTER )
		{
			r.push_back( OURANUS );
			r.push_back( ONEPTUNE );
			r.push_back( OPLUTO );
		}
		if ( style & OBJECTS_INCLUDE_DRAGONHEAD ) r.push_back( istruenode ? OTRUENODE : OMEANNODE );
		if ( style & OBJECTS_INCLUDE_DRAGONTAIL ) r.push_back( istruenode ? OTRUEDESCNODE: OMEANDESCNODE );
		addOuterObjectList( style, false, r, extraObjects );
		if ( style & OBJECTS_INCLUDE_ASCENDANT ) r.push_back( OASCENDANT );
		if ( style & OBJECTS_INCLUDE_MERIDIAN ) r.push_back( OMERIDIAN );
	}
	return r;
}

/*****************************************************
**
**   PlanetList   ---   getDefaultVedicObjectList
**
******************************************************/
vector<int> PlanetList::getDefaultVedicObjectList()
{
	vector<int> r = getVedicPlanetOnlyList();
	if ( config->vLunarNodeMode == LUNAR_NODE_TRUE )
	{
		r.push_back( OTRUENODE );
		r.push_back( OTRUEDESCNODE );
	}
	else
	{
		r.push_back( OMEANNODE );
		r.push_back( OMEANDESCNODE );
	}
	return r;
}

/*****************************************************
**
**   PlanetList   ---   getVedicPlanetOnlyList
**
******************************************************/
vector<int> PlanetList::getVedicPlanetOnlyList()
{
	vector<int> r;
	r.push_back( OSUN );
	r.push_back( OMOON );
	r.push_back( OMARS );
	r.push_back( OMERCURY );
	r.push_back( OJUPITER );
	r.push_back( OVENUS );
	r.push_back( OSATURN );
	return r;
}

/*****************************************************
**
**   PlanetList   ---   getVedicObjectListInternal
**
******************************************************/
vector<int> PlanetList::getVedicObjectListInternal( const int &style, const bool &withLagna, const int &extraObjects )
{
	vector<int> r;
	const bool istruenode = config->vLunarNodeMode == LUNAR_NODE_TRUE;

	if (( style & OBJECTS_INCLUDE_ASCENDANT ) && config->vOrderLagna == 1 && withLagna ) r.push_back( OASCENDANT );
	r.push_back( OSUN );
	r.push_back( OMOON );
	r.push_back( OMARS );
	r.push_back( OMERCURY );
	r.push_back( OJUPITER );
	r.push_back( OVENUS );
	r.push_back( OSATURN );
	if ( style & OBJECTS_INCLUDE_OUTER )
	{
		r.push_back( OURANUS );
		r.push_back( ONEPTUNE );
		r.push_back( OPLUTO );
	}
	if ( style & OBJECTS_INCLUDE_DRAGONHEAD )
	{
		r.push_back( istruenode ? OTRUENODE : OMEANNODE );
	}
	if ( style & OBJECTS_INCLUDE_DRAGONTAIL )
	{
		r.push_back( istruenode ? OTRUEDESCNODE: OMEANDESCNODE );
	}

	if (( style & OBJECTS_INCLUDE_ASCENDANT ) && config->vOrderLagna == 0 && withLagna ) r.push_back( OASCENDANT );

	if ( style & OBJECTS_INCLUDE_MERIDIAN ) r.push_back( OMERIDIAN );
	addOuterObjectList( style, true, r, extraObjects );
	return r;
}

/*****************************************************
**
**   PlanetList   ---   getVedicObjectListWithoutLagna
**
******************************************************/
vector<int> PlanetList::getVedicObjectListWithoutLagna( const int &style, const int &extraObjects )
{
	return getVedicObjectListInternal( style, false, extraObjects );
}

/*****************************************************
**
**   PlanetList   ---   getVedicObjectList
**
******************************************************/
vector<int> PlanetList::getVedicObjectList( const int &style, const int &extraObjects )
{
	return getVedicObjectListInternal( style, true, extraObjects );
}

