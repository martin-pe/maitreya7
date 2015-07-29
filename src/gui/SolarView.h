/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/SolarView.h
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

#ifndef SOLARVIEV_H_
#define SOLARVIEW_H_

#ifdef __GNUG__
#pragma interface "SolarView.h"
#endif

#include "BasicView.h"

class DefaultEwChartGridWidget;
class SolarTextWidget;
class SplitterWidget;
class TajakaHoroscope;
class wxSpinEvent;

/*************************************************//**
*
*  view for Solar (Tajaka) chart
*
******************************************************/
class SolarView : public BasicView
{
	DECLARE_CLASS( SolarView )
public:

	SolarView( wxWindow *parent, ChildWindow *frame, Document *doc, const bool &vedic );
	~SolarView();
	virtual wxString getWindowLabel( const bool shortname = false );
	virtual void postCreate();
	virtual bool dispatchCommand( const int &command );

protected:

	void OnDataChanged();
	void setActualYear();
	void OnNow( wxCommandEvent& );
	void OnCreateChart( wxCommandEvent& );
	void OnYearSpin( wxSpinEvent& );
	virtual void initToolItems();

	DefaultEwChartGridWidget *gwidget;
	TajakaHoroscope *th;
	SplitterWidget *splitter;
	SolarTextWidget *theText;
	int year;

};

#endif

