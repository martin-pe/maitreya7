/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/base/ChartProperties.cpp
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
#pragma implementation "ChartProperties.h"
#endif

#include "ChartProperties.h"

#include "Conf.h"
#include "PlanetList.h"
#include "VedicChartConfig.h"
#include "WesternChartConfig.h"

extern Config *config;

/*****************************************************
**
**   ChartProperties   ---   Constructor
**
******************************************************/
ChartProperties::ChartProperties( const bool &vedic )
{
	isvedic = vedic;
	init();
}

/*****************************************************
**
**   ChartProperties   ---   Constructor
**
******************************************************/
ChartProperties::ChartProperties()
{
	isvedic = config->preferVedic;
	init();
}

/*****************************************************
**
**   ChartProperties   ---   init
**
******************************************************/
void ChartProperties::init()
{
	vobjectstyle = config->vObjects;
	wobjectstyle = config->wObjects;

	vgraphicstyle = config->vGraphicStyle;
	wgraphicstyle = config->wGraphicStyle;
	wchartorientation = config->wChartOrientation;

	vmainstyle = config->vMainChildStyle;
	wmainstyle = config->wMainChildStyle;

	vskin = config->vGraphicSkin;
	wskin = config->wGraphicSkin;

	isblank = false;

	vobjects = PlanetList().getVedicObjectList( vobjectstyle );
	wobjects = PlanetList().getWesternObjectList( wobjectstyle );
}

/*****************************************************
**
**   ChartProperties   ---   getPlanetList
**
******************************************************/
const vector<int> ChartProperties::getPlanetList( const int &extraObjects ) const
{
	return isVedic() ? getVedicPlanetList( extraObjects ) : getWesternPlanetList( extraObjects );
}

/*****************************************************
**
**   ChartProperties   ---   getVedicPlanetList
**
******************************************************/
const vector<int> ChartProperties::getVedicPlanetList( const int &extraObjects ) const
{
	if ( extraObjects != 0 )
	{
		return PlanetList().getVedicObjectList( vobjectstyle, extraObjects );
	}
	else return vobjects;
}

/*****************************************************
**
**   ChartProperties   ---   getWesternPlanetList
**
******************************************************/
const vector<int> ChartProperties::getWesternPlanetList( const int &extraObjects ) const
{
	if ( extraObjects != 0 )
	{
		return PlanetList().getWesternObjectList( wobjectstyle, extraObjects );
	}
	else return wobjects;
}

/*****************************************************
**
**   ChartProperties   ---   setObjectStyle
**
******************************************************/
void ChartProperties::setObjectStyle( const int &style, const bool &vedic )
{
	isVedic() ? vobjectstyle = style : wobjectstyle = style;
	updatePlanetList( vedic );
}

/*****************************************************
**
**   ChartProperties   ---   updatePlanetList
**
******************************************************/
void ChartProperties::updatePlanetList( const bool &v )
{
	v	?
	vobjects = PlanetList().getVedicObjectList( vobjectstyle )
	           : wobjects = PlanetList().getWesternObjectList( wobjectstyle );
}

/*****************************************************
**
**   ChartProperties   ---   changeSkin
**
******************************************************/
void ChartProperties::changeSkin( const bool &increment, const bool vedic )
{
	int skin = vedic ? vskin : wskin;
	if ( increment ) skin--;
	else skin++;
	int size = vedic ? (int)VedicChartConfigLoader::get()->getConfigs().size()
	           : (int)WesternChartConfigLoader::get()->getConfigs().size();
	if ( skin < 0 ) skin = size - 1;
	if ( skin >= size ) skin = 0;
	vedic? vskin = skin : wskin = skin;
}

/*****************************************************
**
**   ChartProperties   ---   dispatchWidgetPropertyCommand
**
******************************************************/
bool ChartProperties::dispatchWidgetPropertyCommand( const int &command )
{
	if ( command >= CMD_CHILD_GRAPHIC_STYLE && command < CMD_CHILD_GRAPHIC_STYLE + 100 )
	{
		if ( isVedic() ) vskin = command - CMD_CHILD_GRAPHIC_STYLE;
		else wskin = command - CMD_CHILD_GRAPHIC_STYLE;
	}
	else switch ( command )
		{
		case CMD_CHILD_WESTERNMODE:
			isvedic = false;
			break;
		case CMD_CHILD_VEDICMODE:
			isvedic = true;
			break;

		case CMD_CHILD_WSHOWHOUSES:
			wgraphicstyle.showHouses = ! wgraphicstyle.showHouses;
		break;

		case CMD_CHILD_WSHOWASPECTS:
			wgraphicstyle.showAspects = ! wgraphicstyle.showAspects;
		break;

		case CMD_CHILD_WSHOWRETRO:
			wgraphicstyle.showRetro = ! wgraphicstyle.showRetro;
		break;

		case CMD_CHILD_WSHOWPLANETCOLORS:
			wgraphicstyle.showPlanetColors = ! wgraphicstyle.showPlanetColors;
		break;

		case CMD_CHILD_WSHOWSIGNCOLORS:
			wgraphicstyle.showSignColors = ! wgraphicstyle.showSignColors;
		break;

		case CMD_CHILD_WSHOWHOUSECOLORS:
			wgraphicstyle.showHouseColors = !  wgraphicstyle.showHouseColors;
		break;

		case CMD_CHILD_WSHOWASPECTCOLORS:
			wgraphicstyle.showAspectColors = !  wgraphicstyle.showAspectColors;
		break;

		case CMD_CHILD_WSHOWASPECTSYMBOLS:
			wgraphicstyle.showAspectSymbols = ! wgraphicstyle.showAspectSymbols;
		break;

		case CMD_CHILD_WTRANSIT_STYLE:
			wgraphicstyle.transitStyle = ! wgraphicstyle.transitStyle;
		break;

		case CMD_CHILD_VSHOWSOUTHINDIAN:
			vgraphicstyle.indianChartType = VGRAPHIC_SOUTH_INDIAN;
		break;
		case CMD_CHILD_VSHOWNORTHINDIAN:
			vgraphicstyle.indianChartType = VGRAPHIC_NORTH_INDIAN;
		break;
		case CMD_CHILD_VSHOWEASTINDIAN:
			vgraphicstyle.indianChartType = VGRAPHIC_EAST_INDIAN;
		break;

		case CMD_CHILD_VSHOWCENTERNOTHING:
			vgraphicstyle.centerInfoType = VGRAPHIC_CHART_CENTER_NOTHING;
		break;
		case CMD_CHILD_VSHOWCENTERVARGA:
			vgraphicstyle.centerInfoType = VGRAPHIC_CHART_CENTER_VARGA;
		break;
		case CMD_CHILD_VSHOWCENTERDIVISION:
			vgraphicstyle.centerInfoType = VGRAPHIC_CHART_CENTER_DIVISION;
		break;
		case CMD_CHILD_VSHOWCENTERBOTH:
			vgraphicstyle.centerInfoType = VGRAPHIC_CHART_CENTER_BOTH;
		break;

		case CMD_CHILD_VSHOWNORTHASC:
			vgraphicstyle.northIndianSignDisplayType = VGRAPHIC_NORTH_INDIAN_ASC;
		break;
		case CMD_CHILD_VSHOWNORTHNUMBER:
			vgraphicstyle.northIndianSignDisplayType = VGRAPHIC_NORTH_INDIAN_NUMBER;
		break;
		case CMD_CHILD_VSHOWNORTHSHORT:
			vgraphicstyle.northIndianSignDisplayType = VGRAPHIC_NORTH_INDIAN_SHORT;
		break;
		case CMD_CHILD_VSHOWNORTHSYMBOL:
			vgraphicstyle.northIndianSignDisplayType = VGRAPHIC_NORTH_INDIAN_SYMBOL;
		break;

		case CMD_CHILD_VSHOWARUDHA:
			vgraphicstyle.showArudhas = !vgraphicstyle.showArudhas;
		break;
		case CMD_CHILD_VSHOWRETRO:
			vgraphicstyle.showRetro = !vgraphicstyle.showRetro;
		break;

		case CMD_CHILD_VSHOWPLANETCOLORS:
			vgraphicstyle.showPlanetColors = !vgraphicstyle.showPlanetColors;
		break;
		case CMD_CHILD_VSHOWSANSKRITSYMBOLS:
			vgraphicstyle.showSbcSanskritSymbols = !vgraphicstyle.showSbcSanskritSymbols;
		break;
		case CMD_CHILD_VSHOWAFFLICTIONS:
			vgraphicstyle.showSbcAfflictions = !vgraphicstyle.showSbcAfflictions;
		break;
		case CMD_CHILD_VSHOW_SBC_NAKSHATRA_QUALITY:
			vgraphicstyle.showSbcNakshatraQuality = !vgraphicstyle.showSbcNakshatraQuality;
		break;

#define SETOBJECTFLAG( a, c ) \
			case a: \
				if ( isVedic() ) \
				{ \
					vobjectstyle & c ? vobjectstyle &= ~c : vobjectstyle |= c; \
				} \
				else \
				{ \
					wobjectstyle & c ? wobjectstyle &= ~c : wobjectstyle |= c; \
				} \
			break;
			SETOBJECTFLAG( CMD_CHILD_SHOWOUTER, OBJECTS_INCLUDE_OUTER )
			SETOBJECTFLAG( CMD_CHILD_SHOWDRAGONHEAD, OBJECTS_INCLUDE_DRAGONHEAD )
			SETOBJECTFLAG( CMD_CHILD_SHOWDRAGONTAIL, OBJECTS_INCLUDE_DRAGONTAIL )
			SETOBJECTFLAG( CMD_CHILD_SHOWASCENDANT, OBJECTS_INCLUDE_ASCENDANT )
			SETOBJECTFLAG( CMD_CHILD_SHOWMERIDIAN, OBJECTS_INCLUDE_MERIDIAN )
			SETOBJECTFLAG( CMD_CHILD_SHOWURANIAN, OBJECTS_INCLUDE_URANIAN )
			SETOBJECTFLAG( CMD_CHILD_SHOWCHIRON, OBJECTS_INCLUDE_CHIRON )
			SETOBJECTFLAG( CMD_CHILD_SHOWPHOLUS, OBJECTS_INCLUDE_PHOLUS )
			SETOBJECTFLAG( CMD_CHILD_SHOWPLANETOIDS, OBJECTS_INCLUDE_PLANETOIDS )
			SETOBJECTFLAG( CMD_CHILD_SHOWLILITH, OBJECTS_INCLUDE_LILITH )
			SETOBJECTFLAG( CMD_CHILD_SHOWUPAGRAHAS, OBJECTS_INCLUDE_UPAGRAHAS )
			SETOBJECTFLAG( CMD_CHILD_SHOWKALAVELAS, OBJECTS_INCLUDE_KALAVELAS )
			SETOBJECTFLAG( CMD_CHILD_SHOWSPECIALLAGNAS, OBJECTS_INCLUDE_SPECIALLAGNAS )
			SETOBJECTFLAG( CMD_CHILD_SHOWARIES, OBJECTS_INCLUDE_ARIES )
			SETOBJECTFLAG( CMD_CHILD_SHOWD9LAGNA, OBJECTS_INCLUDE_D9_LAGNA )
			SETOBJECTFLAG( CMD_CHILD_SHOWARABICPARTS, OBJECTS_INCLUDE_ARABICPARTS )
#undef SETOBJECTFLAG

#define SETMAINVIEWFLAG( a, c ) \
			case a: \
				if ( isVedic() ) vmainstyle & c ? vmainstyle &= ~c : vmainstyle |= c; \
				else wmainstyle & c ? wmainstyle &= ~c : wmainstyle |= c; \
			break;

			SETMAINVIEWFLAG( CMD_CHILD_MAIN_SHOW_LORD, MAIN_CHILD_SHOW_LORD );
			SETMAINVIEWFLAG( CMD_CHILD_MAIN_SHOW_DIGNITY, MAIN_CHILD_SHOW_DIGNITY );
			SETMAINVIEWFLAG( CMD_CHILD_MAIN_SHOW_NAVAMSA,MAIN_CHILD_SHOW_NAVAMSA );
			SETMAINVIEWFLAG( CMD_CHILD_MAIN_SHOW_KARAKA, MAIN_CHILD_SHOW_KARAKA );
			SETMAINVIEWFLAG( CMD_CHILD_MAIN_SHOW_SHASTIAMSA, MAIN_CHILD_SHOW_SHASTIAMSA );
			SETMAINVIEWFLAG( CMD_CHILD_MAIN_SHOW_NAKSHATRA, MAIN_CHILD_SHOW_NAKSHATRA );
			SETMAINVIEWFLAG( CMD_CHILD_MAIN_SHOW_DASAVARGA, MAIN_CHILD_SHOW_DASAVARGA );
			SETMAINVIEWFLAG( CMD_CHILD_MAIN_SHOW_HOUSEPOS, MAIN_CHILD_SHOW_HOUSEPOS );
			SETMAINVIEWFLAG( CMD_CHILD_MAIN_SHOW_KP, MAIN_CHILD_SHOW_KP );
			SETMAINVIEWFLAG( CMD_CHILD_MAIN_SHOW_ASHTAKA, MAIN_CHILD_SHOW_ASHTAKA );
			SETMAINVIEWFLAG( CMD_CHILD_MAIN_SHOW_PADA, MAIN_CHILD_SHOW_PADA );
			SETMAINVIEWFLAG( CMD_CHILD_MAIN_SHOW_HOUSES, MAIN_CHILD_SHOW_HOUSES );
			SETMAINVIEWFLAG( CMD_CHILD_MAIN_SHOW_DECLINATION, MAIN_CHILD_SHOW_DECLINATION );
#undef SETMAINVIEWFLAG

			// dummy
		case 0:
			break;
		default:
			return false;
			break;
		}
	updatePlanetList( isVedic() );
	return true;
}

/*****************************************************
**
**   ChartProperties   ---   dump
**
******************************************************/
void ChartProperties::dump()
{
	unsigned int i;
	printf( "Chart Properties: vedic %d blank %d\n", isVedic(), isblank );
	printf( " vobjectstyle %d wobjectstyle %d\n",  vobjectstyle, wobjectstyle );
	//printf( " vgraphicstyle %d wgraphicstyle %d\n", vgraphicstyle, wgraphicstyle );
	printf( " vmainstyle %d wmainstyle %d\n",  vmainstyle, wmainstyle );
	printf( " vskin %d wskin %d\n", vskin, wskin );

	printf( " Vedic Objects: " );
	for ( i = 0; i < vobjects.size(); i++ ) printf( "%d ", vobjects[i] );
	printf( "\n" );

	printf( " Western Objects: " );
	for ( i = 0; i < wobjects.size(); i++ ) printf( "%d ", wobjects[i] );
	printf( "\n" );

}


