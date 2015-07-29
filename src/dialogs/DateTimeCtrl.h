/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/DateTimeCtrl.h
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


#ifndef _DATETIMECTRL_H_
#define _DATETIMECTRL_H_

#ifdef __GNUG__
#pragma interface "DateTimeCtrl.h"
#endif

#include <wx/control.h>
#include <wx/valtext.h>

class BaseInputField;
class wxSpinButton;
class wxSpinEvent;
class wxStaticText;

BEGIN_DECLARE_EVENT_TYPES()
DECLARE_EVENT_TYPE(COMMAND_SPIN_CHANGED, wxID_HIGHEST + 5001 )
DECLARE_EVENT_TYPE(COMMAND_SPIN_WRAP, wxID_HIGHEST + 5002 )
END_DECLARE_EVENT_TYPES()

/*************************************************//**
*
* 
*
******************************************************/
class BaseDateTimeSpin : public wxControl
{
	DECLARE_CLASS( BaseDateTimeSpin )

public:
	BaseDateTimeSpin( wxWindow *parent, int id, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize );

	void setDoubleValue( const double &value );
	double getDoubleValue();

protected:

	BaseInputField *text;
	wxStaticText *label;
	wxSpinButton *spin;

	virtual void updateLabel() {}
	virtual void add( const double &value );
	virtual void spinChanged( const double &delta );

private:

	void OnSpinUp( wxSpinEvent &event );
	void OnSpinDown( wxSpinEvent &event );
	void OnTextChanged( wxCommandEvent& );
	void OnTextUpdated( wxCommandEvent& );
	void OnMouseWheelEvent( wxMouseEvent& );
	void OnNavigationEvent( wxNavigationKeyEvent& );
	void OnReturn( wxCommandEvent& );
};

/*************************************************//**
*
* \brief control for date input
*
******************************************************/
class DateSpin : public BaseDateTimeSpin
{
	DECLARE_CLASS( DateSpin )

public:
	DateSpin( wxWindow *parent, int id = -1, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize );

protected:

	virtual void updateLabel();
};

/*************************************************//**
*
* \brief spin control for time input
*
******************************************************/
class TimeSpin : public BaseDateTimeSpin
{
	DECLARE_CLASS( TimeSpin )

public:

	TimeSpin( wxWindow *parent, int id = -1, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize );

protected:
	virtual void add( const double &delta );
};


#endif


