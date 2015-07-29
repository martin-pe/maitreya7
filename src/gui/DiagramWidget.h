/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/DiagramWidget.h
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

#ifndef _DIAGRAMWIDGET_H_
#define _DIAGRAMWIDGET_H_

#ifdef __GNUG__
#pragma interface "DiagramWidget.h"
#endif

#include "BasicWidget.h"

#include<vector>

using namespace std;

class ChartProperties;

/*************************************************//**
*
* \brief information about a single item in DiagramWidget
*
******************************************************/
class DiagramItem
{
public:
	DiagramItem( const int id, const double v ) {
		objectId = id;
		value = v;
	}

	/**
	 * \brief id of the object (planet or sign)
	 */
	int objectId;

	/**
	 * \brief value of the object (planet or sign) in diagram
	 */
	double value;
};

/*************************************************//**
*
* \brief information about diagram in DiagramWidget
*
******************************************************/
class DiagramInfo
{
public:
	DiagramInfo() {
		xmax = 0;
	}
	DiagramInfo( const int &xm ) {
		xmax = xm;
	}

	void addItem( const DiagramItem& );
	void clear();

	/**
	 * \brief vector of items (planet or signs)
	 */
	vector<DiagramItem> items;

	/**
	 * \brief maximal value for diagram bar (will be calculated otherwise)
	 */
	int xmax;
};

/*************************************************//**
*
* \brief Widget containing a number of bar diagrams
*
******************************************************/
class DiagramWidget : public BasicWidget
{
public:
	DiagramWidget( wxWindow*, ChartProperties*, const int &cols, const int &rows );

protected:

	/**
	 * \brief prepares graphic and delegates to paintDiagrams
	 */
	virtual void doPaint( bool eraseBackground = true, const wxRect* rect = 0 );

	/**
	 * \brief must be overwritten by subclasses
	 */
	virtual void paintDiagrams() = 0;

	/**
	 * \brief called from subclass for each diagram
	 */
	void paintDiagram( const int &col, const int& row, wxString title );

	void drawBar( const double &left, const double &bottom, const double &w, const double &h, const int &object, const double &value );

	virtual void HandleMouseWheelEvent( wxMouseEvent& ) {};

	/**
	 * \brief information about the currently painted diagram
	 */
	DiagramInfo info;

	/**
	 * \brief number of columns in diagram (constant)
	 */
	const int cols;

	/**
	 * \brief number of rows in diagram (constant)
	 */
	const int rows;

	/**
	 * \brief step length in x direction
	 */
	int xstep;

	/**
	 * \brief step length in y direction
	 */
	int ystep;

	int xdiagramstart, ydiagramstart;

	/**
	 * \brief whether planet or signs should be displayed
	 */
	bool planetmode;

private:
	double x3d, y3d, text_height;

	DECLARE_CLASS( DiagramWidget )
};


#endif

