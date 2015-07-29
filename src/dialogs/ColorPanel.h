/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/ColorPanel.h
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

#ifndef COLORPANEL_H
#define COLORPANEL_H

#ifdef __GNUG__
#pragma interface "ColorPanel.h"
#endif

#include "ConfigPanel.h"

#include <wx/colour.h>

// begin wxGlade: dependencies
// end wxGlade

class wxColourPickerCtrl;
class wxColourPickerEvent;
class wxButton;
class wxStaticBox;
class wxStaticText;

/*************************************************//**
*
* \brief ConfigPanel for color parameters
*
******************************************************/
class ColorPanel: public ConfigPanel
{
	DECLARE_CLASS( ColorPanel )

public:
    // begin wxGlade: ColorPanel::ids
    // end wxGlade

	ColorPanel( wxWindow* parent );
	virtual void setData();
	virtual bool saveData();

private:
    // begin wxGlade: ColorPanel::methods
    void set_properties();
    void do_layout();
    // end wxGlade
	void OnSetDefaults( wxCommandEvent& );

protected:
    // begin wxGlade: ColorPanel::attributes
    wxStaticBox* sizer_bgcolors_staticbox;
    wxStaticBox* sizer_left_staticbox;
    wxStaticText* label_fg;
    wxColourPickerCtrl* button_color_fg;
    wxStaticText* label_planet;
    wxColourPickerCtrl* button_color_planets;
    wxStaticText* label_transit;
    wxColourPickerCtrl* button_color_transits;
    wxStaticText* label_benefic;
    wxColourPickerCtrl* button_color_benefic;
    wxStaticText* label_malefic;
    wxColourPickerCtrl* button_color_malefic;
    wxStaticText* label_bg;
    wxColourPickerCtrl* button_color_bg;
    wxStaticText* label_header_copy;
    wxColourPickerCtrl* button_color_header;
    wxStaticText* label_marked;
    wxColourPickerCtrl* button_color_marked;
    wxStaticText* label_error;
    wxColourPickerCtrl* button_color_error;
    wxButton* button_default;
    // end wxGlade
}; // wxGlade: end class


#endif // COLORFONTPANEL_H
