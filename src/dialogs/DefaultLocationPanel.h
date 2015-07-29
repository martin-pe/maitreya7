/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/DefaultLocationPanel.h
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

#ifndef DEFAULTLOCATIONPANEL_H
#define DEFAULTLOCATIONPANEL_H

#ifdef __GNUG__
#pragma interface "DefaultLocationPanel.h"
#endif

#include "ConfigPanel.h"

// begin wxGlade: dependencies
// end wxGlade

class Location;
class wxButton;
class wxCheckBox;
class wxChoice;
class wxSpinCtrl;
class wxStaticBox;
class wxStaticLine;
class wxStaticText;
class wxTextCtrl;
class wxTimer;
class wxTimerEvent;
class LongitudeInputField;
class LatitudeInputField;

/*************************************************//**
*
* \brief ConfigPanel for default location parameters
*
******************************************************/
class DefaultLocationPanel: public ConfigPanel
{
	DECLARE_CLASS( DefaultLocationPanel )

public:
    // begin wxGlade: DefaultLocationPanel::ids
    // end wxGlade

	DefaultLocationPanel( wxWindow* parent );
	~DefaultLocationPanel();

	virtual void setData();
	virtual bool saveData();

private:
    // begin wxGlade: DefaultLocationPanel::methods
    void set_properties();
    void do_layout();
    // end wxGlade
	void OnSearchLocation( wxCommandEvent& );
	void setDefaultLocation( Location* );
	void OnTimer( wxTimerEvent& );
	void updateTzItems();

	virtual void onActivate();
	virtual void onPassivate();

	wxTimer *timer;

protected:
    // begin wxGlade: DefaultLocationPanel::attributes
    wxStaticBox* sizer_tzsettings_staticbox;
    wxStaticBox* sizer_deflocname_staticbox;
    wxTextCtrl* text_name;
    wxStaticText* label_longitude;
    LongitudeInputField* text_longitude;
    wxChoice* choice_ew;
    wxStaticText* label_latitude;
    LatitudeInputField* text_latitude;
    wxChoice* choice_ns;
    wxButton* button_search;
    wxStaticText* label_localtime;
    wxStaticText* text_localtime;
    wxStaticText* label_uttime;
    wxStaticText* text_uttime;
    wxStaticText* label_tzoffset;
    wxStaticText* text_tzoffset;
    wxStaticText* label_systemdst;
    wxStaticText* text_systemdst;
    // end wxGlade
}; // wxGlade: end class


#endif // DEFAULTLOCATIONPANEL_H
