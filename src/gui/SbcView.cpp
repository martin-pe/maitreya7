/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/SbcView.cpp
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
#pragma implementation "SbcView.h"
#endif

#include "SbcView.h"

#include "ChartProperties.h"
#include "Document.h"
#include "guibase.h"
#include "Nakshatra.h"
#include "Painter.h"
#include "VedicChartConfig.h"
#include "Writer.h"

IMPLEMENT_CLASS( SbcView, BasicView )

IMPLEMENT_CLASS( SbcWidget, BasicWidget )

/*****************************************************
**
**   SbcView   ---   Constructor
**
******************************************************/
SbcView::SbcView( wxWindow *parent, ChildWindow *frame, Document *doc )
		: BasicView( parent, frame, doc, VIEW_SBC, false, true, wxFULL_REPAINT_ON_RESIZE )
{
	setVedic( true );
	supportEWToggle = supportGraphicExport = supportObjectToggle = supportSkinToggle = true;
	supportSbcStyleToggle = true;

	gwidget = new SbcWidget( this, this, doc, 0 );
	widget = gwidget;
	OnDataChanged();
}

/*****************************************************
**
**   SbcView   ---   OnDataChanged
**
******************************************************/
void SbcView::OnDataChanged()
{
	BasicView::OnDataChanged();
	gwidget->OnDataChanged();
	gwidget->Refresh();
}

/**************************************************************
***
**   SbcView   ---   getWindowLabel
***
***************************************************************/
wxString SbcView::getWindowLabel( const bool shortname )
{
	return shortname ? _( "Sbc" ) : _( "Sarvatobhadra" );
}

/*****************************************************
**
**   SbcWidget   ---   Constructor
**
******************************************************/
SbcWidget::SbcWidget( wxWindow *parent, ChartProperties *chartprops, Horoscope* h1, Horoscope *h2, wxWindowID id, const wxPoint& pos,
                      const wxSize& size )
		: BasicWidget( parent, chartprops, id, pos, size )
{
	SetMinSize( wxSize( 150, 150 ));
	if ( ! h2 || h1 == h2 )
		sbcchart = new SarvatobhadraChart( new NakshatraHoroscope( h1, N28 ), chartprops );
	else
		sbcchart = new SarvatobhadraChart( new NakshatraHoroscope( h1, N28 ), h2 ? new NakshatraHoroscope( h2, N28 ) : 0, chartprops );

	Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( SbcWidget::OnMouseLeft ));
}

/*****************************************************
**
**   SbcWidget   ---   Destructor
**
******************************************************/
SbcWidget::~SbcWidget()
{
	delete sbcchart;
}

/*****************************************************
**
**   SbcWidget   ---   setCharts
**
******************************************************/
void SbcWidget::setCharts( const Horoscope *h1, const Horoscope *h2 )
{
	sbcchart->setCharts( new NakshatraHoroscope( h1, N28 ), new NakshatraHoroscope( h2, N28 ));
}

/*****************************************************
**
**   SbcWidget   ---   OnMouseLeft
**
******************************************************/
void SbcWidget::OnMouseLeft( wxMouseEvent& event)
{
	wxRect rect;
	int x = event.m_x;
	int y = event.m_y;
	if ( sbcchart->handleMouseLeftEvent( x, y, rect )) RefreshRect( rect );
}

/*****************************************************
**
**   SbcWidget   ---   OnDataChanged
**
******************************************************/
void SbcWidget::OnDataChanged()
{
	sbcchart->OnDataChanged();
	Refresh();
}

/*****************************************************
**
**   SbcWidget   ---   doPaint
**
******************************************************/
void SbcWidget::doPaint( bool eraseBackground, const wxRect* repaintRect )
{
	wxCoord w, h;
	GetSize( &w, &h  );
	sbcchart->paint( painter, MyRect( 0, 0, (int)w, (int)h ), refreshRect.IsEmpty() ? 0 : &refreshRect );
}

/**************************************************************
***
**   ViewFactory   ---   createSbcView
***
***************************************************************/
BasicView *ViewFactory::createSbcView( wxWindow *parent, ChildWindow *frame, Document *doc )
{
	return new SbcView( parent, frame, doc );
}

