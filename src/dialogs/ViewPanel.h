/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/ViewPanel.h
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

#ifndef VIEWPANEL_H
#define VIEWPANEL_H

#ifdef __GNUG__
#pragma interface "ViewPanel.h"
#endif

#include "ConfigPanel.h"

// begin wxGlade: dependencies
// end wxGlade

class Writer;
class ViewPanelTextWidget;

class wxChoice;
class wxStaticBox;
class wxStaticText;

/*************************************************//**
*
* \brief Config panel for view parameters
*
******************************************************/
class ViewPanel: public ConfigPanel
{
	DECLARE_CLASS( ViewPanel )

public:
    // begin wxGlade: ViewPanel::ids
    // end wxGlade

	ViewPanel( wxWindow* parent );
	~ViewPanel();

	virtual void setData();
	virtual bool saveData();

private:
    // begin wxGlade: ViewPanel::methods
    void set_properties();
    void do_layout();
    // end wxGlade

	void OnChoice( wxCommandEvent& );

	Writer *writer;
	class Config *cfg;

protected:
    // begin wxGlade: ViewPanel::attributes
    wxStaticBox* sizer_preview_staticbox;
    wxStaticBox* sizer_special_symbols_staticbox;
    wxStaticBox* sizer_signs_staticbox;
    wxStaticText* label_planet_display;
    wxChoice* choice_planet_display;
    wxStaticText* label_planet_names;
    wxChoice* choice_planet_names;
    wxStaticText* label_positions;
    wxChoice* choice_planet_positions;
    wxStaticText* label_sign_display;
    wxChoice* choice_sign_display;
    wxStaticText* label_sign_names;
    wxChoice* choice_sign_names;
    wxStaticText* label_uranus;
    wxChoice* choice_uranus;
    wxStaticText* label_pluto;
    wxChoice* choice_pluto;
    wxStaticText* label_capricorn;
    wxChoice* choice_capricorn;
    ViewPanelTextWidget* theText;
    wxPanel* panel_preview;
    // end wxGlade
// wxGlade: end class
};

#endif // VIEWPANEL_H
