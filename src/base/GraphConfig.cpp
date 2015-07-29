/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/base/GraphConfig.cpp
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
#pragma implementation "GraphConfig.h"
#endif

#include "GraphConfig.h"
#include "ColorConfig.h"
#include "Conf.h"
#include "IconProvider.h"

extern Config *config;

/*****************************************************
**
**   GcImage   ---   Constructor
**
******************************************************/
GcImage::GcImage()
{
	align = Align::Center;
	mirrorX = false;
	mirrorY = false;
}

/*****************************************************
**
**   GcRegion   ---   Constructor
**
******************************************************/
GcRegion::GcRegion()
{
	show = true;
}

/*****************************************************
**
**   GcCircle   ---   Constructor
**
******************************************************/
GcCircle::GcCircle( const double r )
 : GcRegion()
{
	radius = r;
	//isRectangle = false;
	isRectangle = true;
}

/*****************************************************
**
**   GcChartFrame   ---   Constructor
**
******************************************************/
GcChartFrame::GcChartFrame( const double radius )
 : GcRegion()
{
	this->radius = radius;
	frameType = CHART_FRAME_TYPE_INHERITED;
}

/*****************************************************
**
**   GcRectangle   ---   Constructor
**
******************************************************/
GcRectangle::GcRectangle()
 : GcRegion()
{
	doubleOuterLine = false;
	radius = 100.0;
}

/*****************************************************
**
**   GcArrow   ---   Constructor
**
******************************************************/
GcArrow::GcArrow()
{
	innerFrame.radius = 50;
	innerFrame.show = false;

	outerFrame.radius = 97;
	outerFrame.show = false;
}

/*****************************************************
**
**   GcRegionList   ---   Constructor
**
******************************************************/
GcRegionList::GcRegionList()
{
	useStockPenList = false;
	useStockBrushList = false;
	useStockTextColorList = false;
}

/*****************************************************
**
**   GcRegionList   ---   needsBgPaint
**
******************************************************/
bool GcRegionList::needsBgPaint()
{
	return globalBrush.IsOk() || brushList.size() > 0 || useStockBrushList;
}

/*****************************************************
**
**   GcZodiacalSigns   ---   Constructor
**
******************************************************/
GcZodiacalSigns::GcZodiacalSigns()
 : GcRegionList()
{
	symbolZoomFactor = 100;
  rotateSymbols = false;
	symbolType = 0;

	useStockTextColorList = true;
}

/*****************************************************
**
**   GcZodiacalSigns   ---   getBrush
**
******************************************************/
wxBrush GcZodiacalSigns::getBrush( const int &i )
{
	//ASSERT_VALID_RASI( i );
	unsigned int size = brushList.size();
	if ( globalBrush.IsOk() ) return globalBrush;
	else if ( size > 0 ) return brushList[i % size];
	else if ( useStockBrushList )
	{
		return wxBrush( config->colors->getSignBgColor( i ));
	}
	else return wxBrush( wxNullColour, wxTRANSPARENT );
}

/*****************************************************
**
**   GcZodiacalSigns   ---   getTextColor
**
******************************************************/
wxColour GcZodiacalSigns::getTextColor( const int &i )
{
	unsigned int size = textColorList.size();

	if ( globalTextColor.IsOk() ) return globalTextColor;
	else if ( size > 0 ) return textColorList[i % size];
	else if ( useStockTextColorList )
	{
		return config->colors->getSignFgColor( i );
	}
	else return config->colors->fgColor;
}

/*****************************************************
**
**   GcHouses   ---   Constructor
**
******************************************************/
GcHouses::GcHouses()
{
	useStockTextColorList = true;
	paintHouseNumbers = true;
}

/*****************************************************
**
**   GcHouses   ---   getBrush
**
******************************************************/
wxBrush GcHouses::getBrush( const int &i )
{
	//ASSERT_VALID_RASI( i );
	unsigned int size = brushList.size();
	if ( globalBrush.IsOk() ) return globalBrush;
	else if ( size > 0 ) return brushList[i % size];
	else if ( useStockBrushList )
	{
		return  wxBrush( config->colors->getHouseBgColor( i ));
	}
	else return wxBrush( wxNullColour, wxTRANSPARENT );
}

/*****************************************************
**
**   GcHouses   ---   getTextColor
**
******************************************************/
wxColour GcHouses::getTextColor( const int &i )
{
	unsigned int size = textColorList.size();

	if ( globalTextColor.IsOk() ) return globalTextColor;
	else if ( size > 0 ) return textColorList[i % size];
	else if ( useStockTextColorList )
	{
		return config->colors->getHouseFgColor( i );
	}
	else return config->colors->fgColor;
}


