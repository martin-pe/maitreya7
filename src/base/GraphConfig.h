/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/base/GraphConfig.h
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

#ifndef _GRAPHCONFIG_H_
#define _GRAPHCONFIG_H_

#ifdef __GNUG__
#pragma interface "GraphConfig.h"
#endif

#include "constants.h"

#include <wx/brush.h>
#include <wx/pen.h>
#include <wx/colour.h>

#include <vector>

using namespace std;

/*************************************************//**
*
*
*
******************************************************/
class GcImage
{
public:
	GcImage();

	wxString filename;
	int align;
	bool mirrorX;
	bool mirrorY;
};

/*************************************************//**
*
*
*
******************************************************/
class GcRegion
{
public:
	GcRegion();

	wxBrush brush;
	wxPen pen;
	wxColour textColor;

	bool show;
};

/*************************************************//**
*
*
*
******************************************************/
class GcCircle : public GcRegion
{
public:
	GcCircle( const double r = 100.0 );

	double radius;
	bool isRectangle;
};

/*************************************************//**
*
*
*
******************************************************/
class GcChartFrame : public GcRegion
{
public:
	GcChartFrame( const double radius = 100.0 );

	double radius;
	int frameType;
};

/*************************************************//**
*
*
*
******************************************************/
class GcRectangle : public GcRegion
{
public:
	GcRectangle();
	bool doubleOuterLine;
	double radius;
};

/*************************************************//**
*
*
*
******************************************************/
class GcArrow
{
public:
	GcArrow();

	GcChartFrame outerFrame;
	GcChartFrame innerFrame;

	wxPen pen;
};

/*************************************************//**
*
*
*
******************************************************/
class GcWidgetBackground : public GcRegion
{
public:
	vector<GcImage> images;
};

/*************************************************//**
*
*
*
******************************************************/
class GcRegionList
{
public:
	GcRegionList();

	vector<wxPen> penList;
	wxPen globalPen;
	bool useStockPenList;

	vector<wxBrush> brushList;
	wxBrush globalBrush;
	bool useStockBrushList;

	vector<wxColour> textColorList;
	wxColour globalTextColor;
	bool useStockTextColorList;

	bool needsBgPaint();
};

/*************************************************//**
*
*
*
******************************************************/
class GcZodiacalSigns : public GcRegionList
{
public:
	GcZodiacalSigns();

	wxBrush getBrush( const int& );
	wxColour getTextColor( const int& );

	int symbolZoomFactor;
  int rotateSymbols;
	int symbolType;
};

/*************************************************//**
*
*
*
******************************************************/
class GcHouses : public GcRegionList
{
public:
	GcHouses();

	wxBrush getBrush( const int& );
	wxColour getTextColor( const int& );

	bool paintHouseNumbers;
};

#endif

