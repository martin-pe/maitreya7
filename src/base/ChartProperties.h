/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/base/ChartProperties.h
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

#ifndef _CHARTPROPERTIES_H_
#define _CHARTPROPERTIES_H_

#ifdef __GNUG__
#pragma interface "ChartProperties.h"
#endif

#include <vector>

#include "constants.h"

using namespace std;

/*************************************************//**
*
* \brief contains all common config properties for a chart
*
******************************************************/
class ChartProperties
{
public:

	ChartProperties();
	ChartProperties( const bool& );
	virtual ~ChartProperties() {}

	// Vedic - western
	void setVedic( const bool b = true ) { isvedic = b; }
	virtual bool isVedic() const { return isvedic; }

	// supported objects
	int getVedicObjectStyle() const { return vobjectstyle; }
	int getWesternObjectStyle() const { return wobjectstyle; }
	int getObjectStyle() const { return isVedic() ? vobjectstyle : wobjectstyle; }

	void setVedicObjectStyle( const int &style )   { setObjectStyle( style, true ); }
	void setWesternObjectStyle( const int &style ) { setObjectStyle( style, false ); }
	void setObjectStyle( const int &style )        { setObjectStyle( style, isvedic ); }

	// graphic styles
	VedicGraphicStyle getVedicGraphicStyle() const { return vgraphicstyle; }
	WesternGraphicStyle getWesternGraphicStyle() const { return wgraphicstyle; }
	int getWesternChartOrientation() const { return wchartorientation; }

	void setVedicGraphicStyle( const VedicGraphicStyle &style )   { vgraphicstyle = style; }
	void setWesternGraphicStyle( const WesternGraphicStyle &style ) { wgraphicstyle = style; }
	void setWesternChartOrientation( const int &orientation ) { wchartorientation = orientation; }

	// main view styles
	int getVedicMainStyle() const { return vmainstyle; }
	int getWesternMainStyle() const { return wmainstyle; }
	int getMainStyle() const { return isVedic() ? vmainstyle : wmainstyle; }

	void setVedicMainStyle( const int &style )   { vmainstyle = style; }
	void setWesternMainStyle( const int &style ) { wmainstyle = style; }

	// skin for graphical charts
	int getVedicSkin() const { return vskin; }
	int getWesternSkin() const { return wskin; }
	int getSkin() const { return isVedic() ? vskin : wskin; }

	void setVedicSkin( const int &skin )   { vskin = skin ; }
	void setWesternSkin( const int &skin ) { wskin = skin; }
	void setSkin( const int &skin )        { isVedic() ? vskin = skin : wskin = skin; }

	// Change Skins, supports range check
	void changeSkin( const bool &increment, const bool vedic );
	void changeSkin( const bool &increment ) { changeSkin( increment, isVedic() ); }

	// planet lists
	const vector<int> getPlanetList( const int &extraObjects = 0 ) const;
	const vector<int> getVedicPlanetList( const int &extraObjects = 0 ) const;
	const vector<int> getWesternPlanetList( const int &extraObjects = 0 ) const;

	void setBlank( const bool b = true ) { isblank = b; }
	bool isBlank() const { return isblank; }

	bool dispatchWidgetPropertyCommand( const int& );

	void dump();

protected:

private:

	void init();
	void setObjectStyle( const int &style, const bool &vedic );
	void updatePlanetList( const bool& );

	bool isvedic, isblank;

	int vobjectstyle, wobjectstyle;
	VedicGraphicStyle vgraphicstyle;
	WesternGraphicStyle wgraphicstyle;
	int wchartorientation;
	int vmainstyle, wmainstyle;
	int vskin, wskin;
	vector<int> vobjects, wobjects;
};


#endif

