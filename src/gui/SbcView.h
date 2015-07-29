/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/SbcView.h
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

#ifndef SBCVIEW_H_
#define SBCVIEW_H_

#ifdef __GNUG__
#pragma interface "SbcView.h"
#endif

#include "BasicView.h"
#include "BasicWidget.h"
#include "SarvatobhadraChart.h"

#include <wx/string.h>

class ChildWindow;
class Horoscope;
class SbcWidget;
class Painter;
class Writer;

/*************************************************//**
*
* \brief View class for Sarvatobhadra chart
*
******************************************************/
class SbcView : public BasicView
{
	DECLARE_CLASS( SbcView )

public:

	SbcView( wxWindow *parent, ChildWindow *frame, Document *doc );

	virtual wxString getWindowLabel( const bool shortname = false );

protected:
	virtual void OnDataChanged();
	SbcWidget *gwidget;
};

/*************************************************//**
*
* \brief Widget class for Sarvatobhadra chart
*
******************************************************/
class SbcWidget : public BasicWidget
{
	DECLARE_CLASS( SbcWidget )

public:

	SbcWidget( wxWindow *parent, ChartProperties*, Horoscope*, Horoscope*, wxWindowID id = -1, const wxPoint& pos = wxDefaultPosition,
	           const wxSize& size = wxDefaultSize );
	~SbcWidget();
	virtual void OnDataChanged();
	void setCharts( const Horoscope*, const Horoscope* );

private:

	SarvatobhadraChart *sbcchart;
	void OnMouseLeft( wxMouseEvent& );
	void doPaint( bool eraseBackground = true, const wxRect* rect = 0 );
};

#endif

