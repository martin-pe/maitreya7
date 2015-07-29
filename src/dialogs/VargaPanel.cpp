/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/VargaPanel.cpp
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
#pragma implementation "VargaPanel.h"
#endif

#include "VargaPanel.h"
#include "DialogElements.h"

#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

#include "Conf.h"
#include "guibase.h"
#include "Lang.h"
#include "mathbase.h"

extern Config *config;
enum { CD_YL_CHOICE = wxID_HIGHEST + 1, CD_KALAVELA_CHOICE, CD_KALAVELA_LORD_CHOICE, CD_KALAVELA_PORTION_CHOICE,
       CD_KALAVELA_DEFAULT
     };

IMPLEMENT_CLASS( VargaPanel, ConfigPanel )

/*****************************************************
**
**   VargaPanel   ---   Constructor
**
******************************************************/
VargaPanel::VargaPanel( wxWindow* parent )
		: ConfigPanel( parent )
{
    // begin wxGlade: VargaPanel::VargaPanel
    sizer_nakshatra_staticbox = new wxStaticBox(this, -1, _("Nakshatras and Dasas"));
    sizer_yl_staticbox = new wxStaticBox(this, -1, _("Year Length"));
    sizer_varga_staticbox = new wxStaticBox(this, -1, _("Vargas"));
    label_tempfriend = new wxStaticText(this, wxID_ANY, _("Temporary Friends"));
    const wxString choice_tempfriend_choices[] = {
        _("Always Rasi"),
        _("Varga")
    };
    choice_tempfriend = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_tempfriend_choices, 0);
    label_hora = new wxStaticText(this, wxID_ANY, _("Hora"));
    const wxString choice_hora_choices[] = {
        _("Parasara"),
        _("Continuous")
    };
    choice_hora = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_hora_choices, 0);
    label_drekkana = new wxStaticText(this, wxID_ANY, _("Drekkana"));
    const wxString choice_drekkana_choices[] = {
        _("Parasara"),
        _("Continuous")
    };
    choice_drekkana = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_drekkana_choices, 0);
    label_chaturtamsa = new wxStaticText(this, wxID_ANY, _("Chaturtamsa"));
    const wxString choice_chaturtamsa_choices[] = {
        _("Parasara"),
        _("Continuous")
    };
    choice_chaturtamsa = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_chaturtamsa_choices, 0);
    label_vcalc_naks = new wxStaticText(this, wxID_ANY, _("Nakshatras"));
    const wxString choice_naks_choices[] = {
        _("27"),
        _("28")
    };
    choice_naks = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_naks_choices, 0);
    label_vcalc_dasa = new wxStaticText(this, wxID_ANY, _("Nakshatra Dasas"));
    const wxString choice_nakdasa_choices[] = {
        _("Length"),
        _("Entry Time")
    };
    choice_nakdasa = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_nakdasa_choices, 0);
    label_kalachakra = new wxStaticText(this, wxID_ANY, _("Kalachakra Dasa"));
    const wxString choice_kala_choices[] = {
        _("Progressive"),
        _("Cyclic"),
        _("Progr same Naks"),
        _("Portion Zero")
    };
    choice_kala = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 4, choice_kala_choices, 0);
    choice_yl = new YearLengthChoice(this, CD_YL_CHOICE, true);
    text_custom_yl = new wxTextCtrl(this, wxID_ANY, wxEmptyString);

    set_properties();
    do_layout();
    // end wxGlade

	Connect( CD_YL_CHOICE, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( VargaPanel::OnYlChoice ));
}

/*****************************************************
**
**   VargaPanel   ---   setData
**
******************************************************/
void VargaPanel::setData()
{
	// Left
	choice_tempfriend->SetSelection( config->tempFriendBasedOnVarga ? 1 : 0 );
	choice_hora->SetSelection( config->vVargaHoraMode );
	choice_drekkana->SetSelection( config->vVargaDrekkanaMode );
	choice_chaturtamsa->SetSelection( config->vVargaChaturtamsaMode );

	// Right
	choice_naks->SetSelection( config->vNakshatraMode28 );
	choice_nakdasa->SetSelection( config->vNakshatraPortionMode );
	choice_kala->SetSelection( config->vKalachakraMode );

	choice_yl->SetSelection( config->vYearLength );
	text_custom_yl->SetValue( printfDouble( config->vCustomYearLength ));
	text_custom_yl->Enable( choice_yl->GetSelection() == 4 );
}

/*****************************************************
**
**   VargaPanel   ---   saveData
**
******************************************************/
bool VargaPanel::saveData()
{
	// Left
	config->tempFriendBasedOnVarga = choice_tempfriend->GetSelection() > 0;
	config->vVargaHoraMode = choice_hora->GetSelection();
	config->vVargaDrekkanaMode = choice_drekkana->GetSelection();
	config->vVargaChaturtamsaMode = choice_chaturtamsa->GetSelection();

	// Right
	config->vNakshatraMode28 = choice_naks->GetSelection();
	config->vNakshatraPortionMode = choice_nakdasa->GetSelection();
	config->vKalachakraMode = choice_kala->GetSelection();

	config->vYearLength = choice_yl->GetSelection();
	config->vCustomYearLength = myatof( text_custom_yl->GetValue() );
	if ( config->vCustomYearLength < 0 ) config->vCustomYearLength = ICUSTOM_YEAR_LEN;

	return true;
}

/*****************************************************
**
**   VargaPanel   ---   OnYlChoice
**
******************************************************/
void VargaPanel::OnYlChoice( wxCommandEvent& )
{
	text_custom_yl->Enable( choice_yl->GetSelection() == 4 );
}
/*****************************************************
**
**   VargaPanel   ---   set_properties
**
******************************************************/
void VargaPanel::set_properties()
{
    // begin wxGlade: VargaPanel::set_properties
    choice_tempfriend->SetSelection(0);
    choice_hora->SetSelection(0);
    choice_drekkana->SetSelection(0);
    choice_chaturtamsa->SetSelection(0);
    choice_naks->SetSelection(0);
    choice_nakdasa->SetSelection(0);
    choice_kala->SetSelection(0);
    // end wxGlade
}

/*****************************************************
**
**   VargaPanel   ---   do_layout
**
******************************************************/
void VargaPanel::do_layout()
{
    // begin wxGlade: VargaPanel::do_layout
    wxFlexGridSizer* sizer_main = new wxFlexGridSizer(1, 2, 0, 0);
    wxFlexGridSizer* sizer_right = new wxFlexGridSizer(2, 1, 0, 0);
    wxStaticBoxSizer* sizer_yl = new wxStaticBoxSizer(sizer_yl_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_yl = new wxFlexGridSizer(2, 2, 3, 3);
    wxStaticBoxSizer* sizer_nakshatra = new wxStaticBoxSizer(sizer_nakshatra_staticbox, wxHORIZONTAL);
    wxFlexGridSizer* grid_nakshatra = new wxFlexGridSizer(3, 2, 3, 3);
    wxFlexGridSizer* sizer_left = new wxFlexGridSizer(1, 1, 3, 3);
    wxStaticBoxSizer* sizer_varga = new wxStaticBoxSizer(sizer_varga_staticbox, wxHORIZONTAL);
    wxFlexGridSizer* grid_varga = new wxFlexGridSizer(4, 2, 3, 3);
    grid_varga->Add(label_tempfriend, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_varga->Add(choice_tempfriend, 0, wxALL|wxEXPAND, 3);
    grid_varga->Add(label_hora, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_varga->Add(choice_hora, 0, wxALL|wxEXPAND, 3);
    grid_varga->Add(label_drekkana, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_varga->Add(choice_drekkana, 0, wxALL|wxEXPAND, 3);
    grid_varga->Add(label_chaturtamsa, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_varga->Add(choice_chaturtamsa, 0, wxALL|wxEXPAND, 3);
    grid_varga->AddGrowableCol(1);
    sizer_varga->Add(grid_varga, 1, wxALL|wxEXPAND, 3);
    sizer_left->Add(sizer_varga, 1, wxALL, 3);
    sizer_main->Add(sizer_left, 1, wxALL|wxEXPAND, 3);
    grid_nakshatra->Add(label_vcalc_naks, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_nakshatra->Add(choice_naks, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    grid_nakshatra->Add(label_vcalc_dasa, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_nakshatra->Add(choice_nakdasa, 0, wxALL|wxEXPAND, 3);
    grid_nakshatra->Add(label_kalachakra, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_nakshatra->Add(choice_kala, 0, wxALL|wxEXPAND, 3);
    sizer_nakshatra->Add(grid_nakshatra, 1, wxALL, 3);
    sizer_right->Add(sizer_nakshatra, 1, wxALL|wxEXPAND, 3);
    wxStaticText* label_yl = new wxStaticText(this, wxID_ANY, _("Length"));
    grid_yl->Add(label_yl, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_yl->Add(choice_yl, 1, wxLEFT|wxEXPAND, 0);
    wxStaticText* label_customyl = new wxStaticText(this, wxID_ANY, _("Custom Length"));
    grid_yl->Add(label_customyl, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_yl->Add(text_custom_yl, 0, wxEXPAND|wxALIGN_CENTER_VERTICAL, 0);
    grid_yl->AddGrowableCol(1);
    sizer_yl->Add(grid_yl, 1, 0, 0);
    sizer_right->Add(sizer_yl, 1, wxALL|wxEXPAND, 3);
    sizer_right->AddGrowableCol(0);
    sizer_main->Add(sizer_right, 1, wxALL, 3);
    SetSizer(sizer_main);
    sizer_main->Fit(this);
    // end wxGlade
}

/*****************************************************
**
**   ConfigPanelFactory   ---   createVargaPanel
**
******************************************************/
ConfigPanel *ConfigPanelFactory::createVargaPanel( wxWindow *parent )
{
	return new VargaPanel( parent );
}

