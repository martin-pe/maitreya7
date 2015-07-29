/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/VargaPanel.h
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

#ifndef VARGAPANEL_H
#define VARGAPANEL_H

#ifdef __GNUG__
#pragma interface "VargaPanel.h"
#endif

#include "ConfigPanel.h"

// begin wxGlade: dependencies
// end wxGlade

class YearLengthChoice;
class wxChoice;
class wxStaticBox;
class wxStaticLine;
class wxStaticText;
class wxTextCtrl;

/*************************************************//**
*
* \brief ConfigPanel for Varga parameters
*
******************************************************/
class VargaPanel: public ConfigPanel
{
	DECLARE_CLASS( VargaPanel )

public:
    // begin wxGlade: VargaPanel::ids
    // end wxGlade

	VargaPanel( wxWindow* parent );
	virtual void setData();
	virtual bool saveData();

private:
    // begin wxGlade: VargaPanel::methods
    void set_properties();
    void do_layout();
    // end wxGlade
	void OnYlChoice( wxCommandEvent& );
protected:
    // begin wxGlade: VargaPanel::attributes
    wxStaticBox* sizer_yl_staticbox;
    wxStaticBox* sizer_nakshatra_staticbox;
    wxStaticBox* sizer_varga_staticbox;
    wxStaticText* label_tempfriend;
    wxChoice* choice_tempfriend;
    wxStaticText* label_hora;
    wxChoice* choice_hora;
    wxStaticText* label_drekkana;
    wxChoice* choice_drekkana;
    wxStaticText* label_chaturtamsa;
    wxChoice* choice_chaturtamsa;
    wxStaticText* label_vcalc_naks;
    wxChoice* choice_naks;
    wxStaticText* label_vcalc_dasa;
    wxChoice* choice_nakdasa;
    wxStaticText* label_kalachakra;
    wxChoice* choice_kala;
    YearLengthChoice* choice_yl;
    wxTextCtrl* text_custom_yl;
    // end wxGlade
}; // wxGlade: end class


#endif // VARGAPANEL_H
