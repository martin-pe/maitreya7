/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/DasaGraphicView.h
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

#ifndef _DASAGRAPHICVIEW_H_
#define _DASAGRAPHICVIEW_H_

#ifdef __GNUG__
#pragma interface "DasaGraphicView.h"
#endif

#include "BasicWidget.h"
#include "BasicView.h"

#include <vector>

using namespace std;

class wxMouseEvent;
class Horoscope;
class DasaGraphicWidget;
class Dasa;
class DasaExpert;
class Painter;
class ToolbarLabel;

/*************************************************//**
*
* \brief view for grahical Dasa
*
******************************************************/
class DasaGraphicView : public BasicView
{
	DECLARE_CLASS( DasaGraphicView )

public:
	DasaGraphicView( wxWindow *parent, ChildWindow *frame, Document *doc );
	virtual wxString getWindowLabel( const bool shortname = false );
	void OnNow( wxCommandEvent& );
	virtual bool dispatchCommand( const int &command );

protected:
	DasaGraphicWidget *swidget;
	virtual void OnDataChanged();
	virtual void postCreate();
	virtual void OnToolbarCommand();
	void OnDasas( wxCommandEvent& );
	void OnYear( wxCommandEvent& );
	virtual void initToolItems();
	void OnChoiceAntardasa( wxCommandEvent& );
};

/*************************************************//**
*
* \brief represents a single item in graphical Dasa
*
******************************************************/
class GraphicalDasaItem
{
public:
	GraphicalDasaItem( DasaExpert *e, Dasa *d ) {
		expert = e;
		dasa = d;
	}
	~GraphicalDasaItem();
	Dasa* dasa;
	vector<GraphicalDasaItem*> depitems;
	DasaExpert *expert;
	wxString name;

protected:

private:

};

/*************************************************//**
*
* \brief widget for graphical Dasa
*
******************************************************/
class DasaGraphicWidget : public BasicWidget
{
	DECLARE_CLASS( DasaGraphicWidget )

public:
	DasaGraphicWidget( wxWindow *parent, ChartProperties*, Horoscope *doc, wxWindowID id = -1, const wxPoint& pos = wxDefaultPosition,
	                   const wxSize& size = wxDefaultSize, long style = wxHSCROLL | wxVSCROLL | wxSUNKEN_BORDER );
	~DasaGraphicWidget();

	void setZoom( const int = 0 );
	void setAntardasaLevel( const int& );
	void setYear( int );
	void OnDataChanged();
	int getBirthYear() {
		return birthyear;
	}
	int getTotalYears() {
		return totalyears;
	}
	wxArrayInt dasas;
	void setTextField( ToolbarLabel* sb ) {
		theText = sb;
	}
	wxString getDasaText( const int &dasa, const int &level, const double &theJd );

protected:

	int adlevel;
	int xviewport, yviewport;
	Horoscope *doc;
	vector <GraphicalDasaItem*> dasaitems;

	virtual void doPaint( bool eraseBackground = true, const wxRect* rect = 0 );
	void clearDasaItems();
	virtual void GetExportSize( int* width, int *height) const;
	void OnIdle( wxIdleEvent& );
	void OnMouseLeave( wxMouseEvent& );
	void OnMouseEnter( wxMouseEvent& );
	void OnMouseMotion( wxMouseEvent& );
	virtual void HandleMouseWheelEvent( wxMouseEvent& );
	bool updateOfTextRequired;
	long int mouse_x, mouse_y;
	ToolbarLabel *theText;
	void writeTextField( const bool& );
	wxString getTextFieldContents();

private:
	int getX4Jd( const double &jd );
	void paintDasa( const int& );
	void showDasaBar( Dasa *dasa, DasaExpert*, double y );
	void paintRuler();
	int zoom;
	double xleft, xright, ytop, ybottom;
	double startjd, endjd;
	int birthyear, totalyears, ycursor, ybar;

};

#endif

