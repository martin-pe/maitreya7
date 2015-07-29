/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/VedicCalculationPanel.h
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

#ifndef VEDICCALCULATIONPANEL_H
#define VEDICCALCULATIONPANEL_H

#ifdef __GNUG__
#pragma interface "VedicCalculationPanel.h"
#endif

#include "ConfigPanel.h"

// begin wxGlade: dependencies
// end wxGlade

class AyanamsaChoice;
class YearLengthChoice;
class wxButton;
class wxCheckBox;
class wxChoice;
class wxStaticBox;
class wxStaticLine;
class wxStaticText;
class wxTextCtrl;

/*************************************************//**
*
* \brief ConfigPanel for Vedic calculation parameters
*
******************************************************/
class VedicCalculationPanel: public ConfigPanel
{
	DECLARE_CLASS( VedicCalculationPanel )

public:
    // begin wxGlade: VedicCalculationPanel::ids
    // end wxGlade

	VedicCalculationPanel( wxWindow* parent );
	virtual void setData();
	virtual bool saveData();

private:
    // begin wxGlade: VedicCalculationPanel::methods
    void set_properties();
    void do_layout();
    // end wxGlade

	int kalaVelaCfgLord[6];
	int kalaVelaCfgPortion[6];
	void OnKalaVelaChoice( wxCommandEvent& );
	void OnKalaVelaLordChoice( wxCommandEvent& );
	void OnKalaVelaPortionChoice( wxCommandEvent& );
	void OnKalaVelaDefault( wxCommandEvent& );

protected:
    // begin wxGlade: VedicCalculationPanel::attributes
    wxStaticBox* sizer_misc_staticbox;
    wxStaticBox* sizer_kalavelas_staticbox;
    wxStaticBox* sizer_benefics_staticbox;
    wxStaticBox* sizer_av_staticbox;
    wxStaticBox* sizer_basic_staticbox;
    wxStaticText* label_vcalc_aya;
    AyanamsaChoice* choice_vcalc_aya;
    wxStaticText* label_vcalc_node;
    wxChoice* choice_node;
    wxStaticText* label_vcalc_house;
    wxChoice* choice_house;
    wxCheckBox* check_westernhouse;
    wxStaticText* label_av_calcrules;
    wxChoice* choice_av_calcrules;
    wxStaticText* label_av_benefics;
    wxChoice* choice_av_benefics;
    wxStaticText* label_av_sarva;
    wxChoice* choice_av_sarva;
    wxStaticText* label_av_sodhya;
    wxChoice* choice_av_sodhya;
    wxStaticText* label_moon;
    wxChoice* choice_moon;
    wxStaticText* label_mercury;
    wxChoice* choice_mercury;
    wxStaticText* label_kalavela;
    wxChoice* choice_kalavela;
    wxStaticText* label_kalalord;
    wxChoice* choice_kalalord;
    wxStaticText* label_kalaportion;
    wxChoice* choice_kalaportion;
    wxButton* button_kaladefault;
    wxStaticText* label_arudha;
    wxChoice* choice_arudha;
    // end wxGlade
}; // wxGlade: end class


#endif // VEDICCALCULATIONPANEL_H
