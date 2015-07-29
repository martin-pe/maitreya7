/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/Statusbar.cpp
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
#pragma implementation "Statusbar.h"
#endif

#include "Statusbar.h"

#include <wx/menu.h>
#include <wx/timer.h>

#include "astrobase.h"
#include "Calculator.h"
#include "Conf.h"
#include "FontConfig.h"
#include "Lang.h"
#include "MenuProvider.h"
#include "Writer.h"

extern Config *config;

#define STATUS_MESSAGE_TIMEOUT 3000

enum { STATUSBAR_PTIMER = wxID_HIGHEST + 2500, STATUSBAR_MSGTIMER };

IMPLEMENT_CLASS( MyStatusbar, wxStatusBar )
IMPLEMENT_CLASS( MainWindowStatusbar, MyStatusbar )

/*****************************************************
**
**   MyStatusbar   ---   Constructor
**
******************************************************/
MyStatusbar::MyStatusbar( wxWindow *parent )
		: wxStatusBar( parent, -1, wxST_SIZEGRIP )
{
	msgtimer = new wxTimer( this, STATUSBAR_MSGTIMER );

	Connect( STATUSBAR_MSGTIMER, wxEVT_TIMER, wxTimerEventHandler( MyStatusbar::OnMessageTimer ));
}

/*****************************************************
**
**   MyStatusbar   ---   Destructor
**
******************************************************/
MyStatusbar::~MyStatusbar()
{
	if ( msgtimer->IsRunning() ) msgtimer->Stop();
	delete msgtimer;
}

/*****************************************************
**
**   MyStatusbar   ---   echo
**
******************************************************/
void MyStatusbar::echo( wxString message )
{
	if ( msgtimer->IsRunning()) msgtimer->Stop();

	SetStatusText( message, 0 );

	if ( ! message.IsEmpty())
	{
		msgtimer->Start( STATUS_MESSAGE_TIMEOUT, true );
	}
}

/*****************************************************
**
**   MyStatusbar   ---   OnMessageTimer
**
******************************************************/
void MyStatusbar::OnMessageTimer( wxTimerEvent &event )
{
	if ( msgtimer->IsRunning()) msgtimer->Stop();
	SetStatusText( wxEmptyString );
}

/*****************************************************
**
**   MainWindowStatusbar   ---   Constructor
**
******************************************************/
MainWindowStatusbar::MainWindowStatusbar( wxWindow *parent )
		: MyStatusbar( parent )
{
	FontConfig *f = FontConfig::get();
	writer = WriterFactory().getWriter( WRITER_TEXT );
	SetFieldsCount( 2 );
	SetFont( *f->getDefaultFont());
	planettimer = new wxTimer( this, STATUSBAR_PTIMER );

	Connect( STATUSBAR_PTIMER, wxEVT_TIMER, wxTimerEventHandler( MainWindowStatusbar::OnPlanetTimer ));
	Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( MainWindowStatusbar::OnRightDown ));
}

/*****************************************************
**
**   MainWindowStatusbar   ---   Destructor
**
******************************************************/
MainWindowStatusbar::~MainWindowStatusbar()
{
	if ( planettimer->IsRunning() ) planettimer->Stop();
	delete planettimer;
	delete writer;
}

/*****************************************************
**
**   MainWindowStatusbar   ---   StartPlanetTimer
**
******************************************************/
void MainWindowStatusbar::StartPlanetTimer( const bool &b )
{
	if ( b )
	{
		if ( ! planettimer->IsRunning() ) planettimer->Start( 1000 );
	}
	else
	{
		if ( planettimer->IsRunning() ) planettimer->Stop();
		SetStatusText( wxEmptyString, 0 );
		SetStatusText( wxEmptyString, 1 );
	}
}

/*****************************************************
**
**   MainWindowStatusbar   ---   OnPlanetTimer
**
******************************************************/
void MainWindowStatusbar::OnPlanetTimer( wxTimerEvent &event )
{
	wxString s;
	double mlen, asclen, dummy, nakportion;
	Lang lang;
	Calculator *calculator = CalculatorFactory().getCalculator();
	int naktype = ( config->vNakshatraMode28 ? N28 : N27 );
	DataSet d;

	d.setActualDate();
	d.setLocation( config->defaultLocation );
	asclen = calculator->calcAscendantAya( d.getJD(), d.getLocation()->getLatitude(),
	                                       d.getLocation()->getLongitude(), config->preferVedic );

	calculator->calcPosition( &d, OMOON, mlen, dummy, true, config->preferVedic );

	s = writer->getObjectName( OASCENDANT, TMEDIUM );
	s << wxT( " " ) << writer->getPosFormatted( asclen );
	SetStatusText( s, 0 );

	if ( config->preferVedic )
	{
		nakportion = (int)(getNakshatraLength( mlen, naktype ) * 10000 /NAKSHATRA_LEN );
		nakportion /= 100;
		s = writer->getObjectName( OMOON, TLARGE );
		s << wxT( " " ) << writer->getPosFormatted( mlen ) << wxT( " - " ) <<  nakportion
		<< wxT( "% " ) << lang.getNakshatraName( getNakshatra( mlen, naktype ), naktype, TMEDIUM );
	}
	else
	{
		s = writer->getObjectName( OMOON, TMEDIUM );
		s << wxT( " " ) << writer->getPosFormatted( mlen );
	}
	SetStatusText( s, 1 );
}

/*****************************************************
**
**   MainWindowStatusbar   ---   OnRightDown
**
******************************************************/
void MainWindowStatusbar::OnRightDown( wxMouseEvent &event )
{
	wxMenu *menu = ContextMenuProvider().getAppWindowContextMenu();
	PopupMenu( menu, event.GetX(), event.GetY() );
	delete menu;
}

