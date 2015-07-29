/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/EphemView.h
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

#ifndef _EPHEMVIEW_H_
#define _EPHEMVIEW_H_

#ifdef __GNUG__
#pragma interface "EphemView.h"
#endif

#include "BasicWidget.h"
#include "BasicView.h"

class ChartProperties;
class EphemExpert;
class EphemTextWidget;
class EphemWidget;
class TextWidget;
class Writer;

/*************************************************//**
*
* \brief ephemeris view containing various text tabs and graphical ephemeris
*
******************************************************/
class EphemView : public BasicView
{
	DECLARE_CLASS( EphemView )

public:
	EphemView( wxWindow *parent, ChildWindow *frame, const bool &vedic );
	~EphemView();
	virtual wxString getWindowLabel( const bool shortname = false );
	virtual bool dispatchCommand( const int& );

	virtual bool supportsGraphicExport();
	virtual bool supportsTextExport();

protected:
	EphemExpert *expert;
	EphemWidget *ewidget;
	EphemTextWidget *theText;
	int month, lastmonth, year, planets, mode, dasa, circleType;
	bool isLocaltime;
	bool gmode, updateView;

	virtual bool supportsTextExport() const;
	virtual void OnDataChanged();
	virtual void OnToolbarCommand();
	void OnYearSpin( wxSpinEvent& );
	void OnMonthSpin( wxSpinEvent& );
	void OnNow( wxCommandEvent& );
	void OnChoiceTZ( wxCommandEvent& );
	void initClientView();
	void setMaxDeg( const int& );
	void OnIdle( wxIdleEvent& );
	virtual void initToolItems();
};

/*************************************************//**
*
* \brief widget for graphical ephemeris
*
******************************************************/
class EphemWidget :  public BasicWidget
{
	DECLARE_CLASS( EphemWidget )

public:
	EphemWidget( wxWindow *parent, ChartProperties*, EphemExpert* );
	~EphemWidget();

	void setMaxDeg( const double &m ) {
		max_deg = m;
	}
	void setFilter( const int &f ) {
		filter = f;
	}

protected:

	virtual void OnSize( wxSizeEvent& );
	virtual void HandleMouseWheelEvent( wxMouseEvent& ) {};

private:

	double xleft, xright, ytop, ybottom, max_deg;
	int zoom;
	int scrollx, scrolly, filter;
	EphemExpert *expert;
	Writer *writer;

	void doPaint( bool eraseBackground = true, const wxRect* rect = 0 );
	void setZoom( const int z );
	void paintRuler();
	void paintPlanets();
	void setLineStyle( const int &planet );
};

#endif

