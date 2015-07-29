/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/VedicPanel.h
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

#ifndef VEDICPANEL_H
#define VEDICPANEL_H

#ifdef __GNUG__
#pragma interface "VedicPanel.h"
#endif

#include "ConfigPanel.h"

// begin wxGlade: dependencies
// end wxGlade

class wxButton;
class wxCheckBox;
class wxChoice;
class wxStaticBox;
class wxStaticLine;
class wxStaticText;

/*************************************************//**
*
* \brief ConfigPanel for Vedic objects
*
******************************************************/
class VedicPanel: public ConfigPanel
{
	DECLARE_CLASS( VedicPanel )

public:
    // begin wxGlade: VedicPanel::ids
    // end wxGlade

	VedicPanel( wxWindow* parent );
	virtual void setData();
	virtual bool saveData();

private:
    // begin wxGlade: VedicPanel::methods
    void set_properties();
    void do_layout();
    // end wxGlade

	void OnArabicParts( wxCommandEvent& );
	void updateArabicText();
	int getMainStyleFromGui();

	wxArrayInt arabicParts;

protected:
    // begin wxGlade: VedicPanel::attributes
    wxStaticBox* sizer_maincols_staticbox;
    wxStaticBox* sizer_order_lagna_staticbox;
    wxStaticBox* sizer_arabic_staticbox;
    wxStaticBox* sizer_partly_staticbox;
    wxStaticBox* sizer_sobjects_staticbox;
    wxStaticBox* sizer_eobjects_staticbox;
    wxCheckBox* check_objects_outer;
    wxCheckBox* check_objects_dragonhead;
    wxCheckBox* check_objects_dragontail;
    wxCheckBox* check_objects_ascendant;
    wxCheckBox* check_objects_meridian;
    wxCheckBox* check_objects_uranian;
    wxCheckBox* check_objects_chiron;
    wxCheckBox* check_objects_pholus;
    wxCheckBox* check_objects_planetoids;
    wxCheckBox* check_objects_lilith;
    wxCheckBox* check_objects_house_cusp;
    wxCheckBox* check_objects_upagrahas;
    wxCheckBox* check_objects_kalavelas;
    wxCheckBox* check_objects_lagnas;
    wxCheckBox* check_objects_d9lagna;
    wxCheckBox* check_objects_aries;
    wxButton* button_arabic;
    wxStaticText* label_arabic_selection;
    wxPanel* panel_arabic_selection;
    wxChoice* choice_order_lagna;
    wxCheckBox* check_main_declinations;
    wxCheckBox* check_main_lords;
    wxCheckBox* check_main_dignities;
    wxCheckBox* check_main_d9;
    wxCheckBox* check_main_nak;
    wxCheckBox* check_main_pada;
    wxCheckBox* check_main_kp;
    wxCheckBox* check_main_d60;
    wxCheckBox* check_main_d10;
    wxCheckBox* check_main_bhava;
    wxCheckBox* check_main_kara;
    wxCheckBox* check_main_ashtaka;
    // end wxGlade
}; // wxGlade: end class


#endif // VEDICPANEL_H
