/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/DateTimeCtrl.cpp
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
#pragma implementation "DateTimeCtrl.h"
#endif

#include "DateTimeCtrl.h"

#include "CommonInputFields.h"
#include "DataSet.h"
#include "guivalidators.h"
#include "Lang.h"
#include "mathbase.h"

#include <wx/app.h>
#include <wx/sizer.h>
#include <wx/spinbutt.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

DEFINE_EVENT_TYPE( COMMAND_SPIN_CHANGED )
DEFINE_EVENT_TYPE( COMMAND_SPIN_WRAP )

enum { BDT_SPIN = wxID_HIGHEST + 650, BDT_TEXT, BDT_LABEL };

IMPLEMENT_CLASS( BaseDateTimeSpin, wxControl )
IMPLEMENT_CLASS( DateSpin, BaseDateTimeSpin )
IMPLEMENT_CLASS( TimeSpin, BaseDateTimeSpin )

/*****************************************************
**
**   BaseDateTimeSpin   ---   Constructor
**
******************************************************/
BaseDateTimeSpin::BaseDateTimeSpin( wxWindow *parent, int id, const wxPoint& pos, const wxSize& size )
		: wxControl( parent, id, pos, size, wxBORDER_NONE | wxTAB_TRAVERSAL )
{
	spin = new wxSpinButton( this, BDT_SPIN, wxDefaultPosition, wxSize( 15, -1 ), wxSP_ARROW_KEYS | wxSP_WRAP );

	Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( BaseDateTimeSpin::OnMouseWheelEvent ));
	Connect( wxEVT_SCROLL_LINEUP, wxSpinEventHandler( BaseDateTimeSpin::OnSpinUp ));
	Connect( wxEVT_SCROLL_LINEDOWN, wxSpinEventHandler( BaseDateTimeSpin::OnSpinDown ));
	Connect( BDT_TEXT, wxEVT_COMMAND_TEXT_UPDATED, wxTextEventHandler( BaseDateTimeSpin::OnTextUpdated ));
	//Connect( wxID_ANY, wxEVT_NAVIGATION_KEY, wxNavigationKeyEventHandler( BaseDateTimeSpin::OnNavigationEvent ));
	Connect( wxID_ANY, COMMAND_TEXT_RETURN, wxCommandEventHandler( BaseDateTimeSpin::OnReturn ));
}

/*****************************************************
**
**   BaseDateTimeSpin   ---   OnNavigationEvent
**
******************************************************/
void BaseDateTimeSpin::OnNavigationEvent( wxNavigationKeyEvent &event )
{
	// TODO how to shift the keyboard focus to the next item in the toolbar
	printf( "BaseDateTimeSpin::OnNavigationEvent %d\n", event.GetDirection() );
	event.ResumePropagation( wxEVENT_PROPAGATE_MAX );
	event.Skip();
}

/*****************************************************
**
**   BaseDateTimeSpin   ---   add
**
******************************************************/
void BaseDateTimeSpin::add( const double &delta )
{
	double value = text->getDoubleValue();
	text->setDoubleValue( value + delta );
	updateLabel();
	wxCommandEvent e( COMMAND_SPIN_CHANGED, GetId());
	wxPostEvent( GetParent(), e );
}

/*****************************************************
**
**   BaseDateTimeSpin   ---   setDoubleValue
**
******************************************************/
void BaseDateTimeSpin::setDoubleValue( const double &d )
{
	text->setDoubleValue( d );
	updateLabel();
	wxCommandEvent e( COMMAND_SPIN_CHANGED, GetId());
	wxPostEvent( GetParent(), e );
}

/*****************************************************
**
**   BaseDateTimeSpin   ---   getDoubleValue
**
******************************************************/
double BaseDateTimeSpin::getDoubleValue()
{
	return text->getDoubleValue();
}

/*****************************************************
**
**   BaseDateTimeSpin   ---   OnSpinDown
**
******************************************************/
void BaseDateTimeSpin::OnSpinDown( wxSpinEvent &event )
{
	add( -1 );
}

/*****************************************************
**
**   BaseDateTimeSpin   ---   OnSpinUp
**
******************************************************/
void BaseDateTimeSpin::OnSpinUp( wxSpinEvent &event )
{
	add( 1 );
}

/*****************************************************
**
**   BaseDateTimeSpin   ---   spinChanged
**
******************************************************/
void BaseDateTimeSpin::spinChanged( const double &delta )
{
	add( delta );
}

/*****************************************************
**
**   BaseDateTimeSpin   ---   OnMouseWheelEvent
**
******************************************************/
void BaseDateTimeSpin::OnMouseWheelEvent( wxMouseEvent &event )
{
	spinChanged( event.GetWheelRotation() > 0 ? 1 : -1 );
}

/*****************************************************
**
**   BaseDateTimeSpin   ---   OnTextUpdated
**
******************************************************/
void BaseDateTimeSpin::OnTextUpdated( wxCommandEvent &event )
{
	//printf( "BaseDateTimeSpin::OnTextUpdated\n" );
	Validate();
}

/*****************************************************
**
**   BaseDateTimeSpin   ---   OnReturn
**
******************************************************/
void BaseDateTimeSpin::OnReturn( wxCommandEvent& )
{
	//printf ( "BaseDateTimeSpin::OnReturn\n" );
	if ( Validate())
	{
		text->formatValue();
	}
	else
	{
		text->resetStoredValue();
	}
	updateLabel();
	wxCommandEvent e( COMMAND_SPIN_CHANGED, GetId());
	wxPostEvent( GetParent(), e );
}

/*****************************************************
**
**   DateSpin   ---   Constructor
**
******************************************************/
DateSpin::DateSpin( wxWindow *parent, int id, const wxPoint& pos, const wxSize& size )
		: BaseDateTimeSpin( parent, id, pos, size )
{
	//text = new DateInputField( this, BDT_TEXT, wxDefaultPosition, wxDefaultSize );
	text = new DateInputField( this, BDT_TEXT, wxDefaultPosition, wxSize( 120, -1 ) );
	label = new wxStaticText( this, BDT_LABEL, wxT( "--" ), wxDefaultPosition, wxSize( 30, -1 ));

	wxBoxSizer* sizer = new wxBoxSizer( wxHORIZONTAL );
	sizer->Add( text, 1, wxALL|wxEXPAND, 3 );
	sizer->Add( spin, 0, wxALL|wxEXPAND, 3 );
	sizer->Add( label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3 );

	SetAutoLayout( true );
	SetSizer( sizer );
	sizer->Fit( this );
	sizer->SetSizeHints(this);
	Layout();
	SetToolTip( _( "Date" ));

	text->setDoubleValue( JDate::getCurrentJD());
	updateLabel();
}

/*****************************************************
**
**   DateSpin   ---   updateLabel
**
******************************************************/
void DateSpin::updateLabel()
{
	Lang lang;
	double value = text->getDoubleValue();
	label->SetLabel( lang.getWeekdayName( ((int)(value + 1.5 )) % 7).Left( 2 ));
}

/*****************************************************
**
**   TimeSpin   ---   Constructor
**
******************************************************/
TimeSpin::TimeSpin( wxWindow *parent, int id, const wxPoint& pos, const wxSize& size )
	: BaseDateTimeSpin( parent, id, pos, size )
{
	text = new TimeInputField( this, BDT_TEXT, wxDefaultPosition, wxSize( 100, -1 ));

	wxBoxSizer* sizer = new wxBoxSizer( wxHORIZONTAL );
	sizer->Add( text, 1, wxALL|wxEXPAND, 3 );
	sizer->Add( spin, 0, wxALL|wxEXPAND, 3 );

	SetAutoLayout( true );
	SetSizer( sizer );
	sizer->Fit( this );
	sizer->SetSizeHints(this);
	Layout();
	SetToolTip( _( "Time" ));
}

/*****************************************************
**
**   TimeSpin   ---   add
**
******************************************************/
void TimeSpin::add( const double &delta )
{
	int commandid = COMMAND_SPIN_CHANGED;
	int commandint = 0;
	
	double value = text->getDoubleValue();

	if ( value >= 23 && delta == 1 )
	{
		commandid = COMMAND_SPIN_WRAP;
		commandint = 1;
	}
	else if ( value <= 1 && delta == -1 )
	{
		commandid = COMMAND_SPIN_WRAP;
		commandint = -1;
	}
	value += delta;
	value = a_red( value, 24 );

	wxCommandEvent e( commandid, GetId() );
	e.SetInt( commandint );
	wxPostEvent( GetParent(), e );

	text->setDoubleValue( value );
}


