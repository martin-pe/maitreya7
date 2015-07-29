/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/VedicCalculationPanel.cpp
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
#pragma implementation "VedicCalculationPanel.h"
#endif

#include "VedicCalculationPanel.h"
#include "DialogElements.h"

#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/choicdlg.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

#include "Conf.h"
#include "guibase.h"
#include "Lang.h"
#include "PlanetList.h"
#include "Writer.h"

extern Config *config;
enum { CD_YL_CHOICE = wxID_HIGHEST + 1, CD_KALAVELA_CHOICE, CD_KALAVELA_LORD_CHOICE, CD_KALAVELA_PORTION_CHOICE, CD_KALAVELA_DEFAULT, CD_BENEFICS };

IMPLEMENT_CLASS( VedicCalculationPanel, ConfigPanel )

/*****************************************************
**
**   VedicCalculationPanel   ---   Constructor
**
******************************************************/
VedicCalculationPanel::VedicCalculationPanel( wxWindow* parent )
		: ConfigPanel( parent )
{
    // begin wxGlade: VedicCalculationPanel::VedicCalculationPanel
    sizer_av_staticbox = new wxStaticBox(this, -1, _("Ashtaka Varga"));
    sizer_benefics_staticbox = new wxStaticBox(this, -1, _("Benefics"));
    sizer_kalavelas_staticbox = new wxStaticBox(this, -1, _("Kala Velas"));
    sizer_misc_staticbox = new wxStaticBox(this, -1, _("Miscellaneous"));
    sizer_basic_staticbox = new wxStaticBox(this, -1, _("Basic Calculation"));
    label_vcalc_aya = new wxStaticText(this, wxID_ANY, _("Ayanamsa"));
    choice_vcalc_aya = new AyanamsaChoice(this, wxID_ANY, config->vAyanamsa);
    label_vcalc_node = new wxStaticText(this, wxID_ANY, _("Lunar Node"));
    const wxString choice_node_choices[] = {
        _("True"),
        _("Mean")
    };
    choice_node = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_node_choices, 0);
    label_vcalc_house = new wxStaticText(this, wxID_ANY, _("House System"));
    const wxString choice_house_choices[] = {
        _("Placidus"),
        _("Koch"),
        _("Regiomontanus"),
        _("Campanus"),
        _("Porphyry"),
        _("Equal"),
        _("Vehlow Equal"),
        _("Axial Rotation"),
        _("Azimutal/Horizontal"),
        _("Polich/Page"),
        _("Alcabitus"),
        _("Morinus"),
        _("Krusinski")
    };
    choice_house = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 13, choice_house_choices, 0);
    check_westernhouse = new wxCheckBox(this, wxID_ANY, _("K.P. Style Houses (Start at Cusp)"));
    label_av_calcrules = new wxStaticText(this, wxID_ANY, _("Calculation Rules"));
    const wxString choice_av_calcrules_choices[] = {
        _("Parasara"),
        _("Vahara Mihira")
    };
    choice_av_calcrules = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_av_calcrules_choices, 0);
    label_av_benefics = new wxStaticText(this, wxID_ANY, _("Benefic points"));
    const wxString choice_av_benefics_choices[] = {
        _("Number"),
        _("Line"),
        _("Dot")
    };
    choice_av_benefics = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 3, choice_av_benefics_choices, 0);
    label_av_sarva = new wxStaticText(this, wxID_ANY, _("Sarvashtakavarga"));
    const wxString choice_av_sarva_choices[] = {
        _("Planets only"),
        _("With Lagna")
    };
    choice_av_sarva = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_av_sarva_choices, 0);
    label_av_sodhya = new wxStaticText(this, wxID_ANY, _("Sodhya Pinda"));
    const wxString choice_av_sodhya_choices[] = {
        _("Parasara"),
        _("Mantreswar")
    };
    choice_av_sodhya = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_av_sodhya_choices, 0);
    label_moon = new wxStaticText(this, wxID_ANY, _("Moon"));
    const wxString choice_moon_choices[] = {
        _("Never"),
        _("Angle < 180"),
        _("120 < Angle < 240"),
        _("Always")
    };
    choice_moon = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 4, choice_moon_choices, 0);
    label_mercury = new wxStaticText(this, wxID_ANY, _("Mercury"));
    const wxString choice_mercury_choices[] = {
        _("Never"),
        _("No Malefic Conjunction"),
        _("Always")
    };
    choice_mercury = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 3, choice_mercury_choices, 0);
    label_kalavela = new wxStaticText(this, wxID_ANY, _("Kala Vela"));
    const wxString choice_kalavela_choices[] = {
        _("Kala"),
        _("Mrityu"),
        _("Ardhaprahara"),
        _("Yama Ghantaka"),
        _("Gulika"),
        _("Mandi")
    };
    choice_kalavela = new wxChoice(this, CD_KALAVELA_CHOICE, wxDefaultPosition, wxDefaultSize, 6, choice_kalavela_choices, 0);
    label_kalalord = new wxStaticText(this, wxID_ANY, _("Lord"));
    const wxString choice_kalalord_choices[] = {
        _("None"),
        _("Sun"),
        _("Moon"),
        _("Mars"),
        _("Mercury"),
        _("Jupiter"),
        _("Venus"),
        _("Saturn"),
        _("Eight")
    };
    choice_kalalord = new wxChoice(this, CD_KALAVELA_LORD_CHOICE, wxDefaultPosition, wxDefaultSize, 9, choice_kalalord_choices, 0);
    label_kalaportion = new wxStaticText(this, -1, _("Part of Portion"));
    const wxString choice_kalaportion_choices[] = {
        _("Beginning"),
        _("Middle"),
        _("End")
    };
    choice_kalaportion = new wxChoice(this, CD_KALAVELA_PORTION_CHOICE, wxDefaultPosition, wxDefaultSize, 3, choice_kalaportion_choices, 0);
    button_kaladefault = new wxButton(this, CD_KALAVELA_DEFAULT, _("Restore Defaults"));
    label_arudha = new wxStaticText(this, wxID_ANY, _("Arudhas"));
    const wxString choice_arudha_choices[] = {
        _("Parasara"),
        _("Raman")
    };
    choice_arudha = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_arudha_choices, 0);

    set_properties();
    do_layout();
    // end wxGlade

	Connect( CD_KALAVELA_CHOICE, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( VedicCalculationPanel::OnKalaVelaChoice ));
	Connect( CD_KALAVELA_LORD_CHOICE, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( VedicCalculationPanel::OnKalaVelaLordChoice ));
	Connect( CD_KALAVELA_PORTION_CHOICE, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( VedicCalculationPanel::OnKalaVelaPortionChoice ));
	Connect( CD_KALAVELA_DEFAULT, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( VedicCalculationPanel::OnKalaVelaDefault ));
}

/*****************************************************
**
**   VedicCalculationPanel   ---   setData
**
******************************************************/
void VedicCalculationPanel::setData()
{
	choice_house->SetSelection( config->vHouseSystem ? config->vHouseSystem -1 : 0 );
	check_westernhouse->SetValue( config->vHouseWestern );
	choice_node->SetSelection( config->vLunarNodeMode );

	// Ashataka Varga
	choice_av_calcrules->SetSelection( config->vAshtakavargaMode );
	choice_av_benefics->SetSelection( config->vAshtakavargaNumberMode );
	choice_av_sarva->SetSelection( config->vAshtakavargaSarva );
	choice_av_sodhya->SetSelection( config->vAshtakavargaMantreswarMode );

	// Kala Vela
	for ( int i = 0; i < 6; i++ )
	{
		kalaVelaCfgLord[i] = config->kalaVelaLord[i];
		kalaVelaCfgPortion[i] = config->kalaVelaPortion[i];
	}
	choice_kalavela->SetSelection( 0 );
	choice_kalalord->SetSelection( kalaVelaCfgLord[0] + 1 );
	choice_kalaportion->SetSelection( kalaVelaCfgPortion[0] );
	choice_kalaportion->Enable( kalaVelaCfgPortion[0] != -1 );

	// Benefics
	choice_moon->SetSelection( config->vMoonBenefic );
	choice_mercury->SetSelection( config->vMercuryBenefic );

	// Misc
	choice_arudha->SetSelection( config->vArudhaMode );
}

/*****************************************************
**
**   VedicCalculationPanel   ---   saveData
**
******************************************************/
bool VedicCalculationPanel::saveData()
{
	config->vHouseSystem = choice_house->GetSelection() + 1;
	config->vHouseWestern = check_westernhouse->GetValue();
	config->vLunarNodeMode = choice_node->GetSelection();
	config->vAyanamsa = choice_vcalc_aya->getConfigIndex();

	// Ashataka Varga
	config->vAshtakavargaMode = choice_av_calcrules->GetSelection();
	config->vAshtakavargaNumberMode = choice_av_benefics->GetSelection();
	config->vAshtakavargaSarva = choice_av_sarva->GetSelection();
	config->vAshtakavargaMantreswarMode =	choice_av_sodhya->GetSelection();

	// Kala Vela
	for ( int i = 0; i < 6; i++ )
	{
		config->kalaVelaLord[i] = kalaVelaCfgLord[i];
		config->kalaVelaPortion[i] = kalaVelaCfgPortion[i];
	}

	// Benefics
	config->vMoonBenefic = choice_moon->GetSelection();
	config->vMercuryBenefic = choice_mercury->GetSelection();

	// Misc
	config->vArudhaMode = choice_arudha->GetSelection();

	return true;
}

/*****************************************************
**
**   VedicCalculationPanel   ---   OnKalaVelaChoice
**
******************************************************/
void VedicCalculationPanel::OnKalaVelaChoice( wxCommandEvent& )
{
	int selkala = choice_kalavela->GetSelection();
	assert( selkala >= 0 && selkala < 6 );
	choice_kalalord->SetSelection( kalaVelaCfgLord[selkala] + 1 );
	choice_kalaportion->SetSelection( kalaVelaCfgPortion[selkala] );
}

/*****************************************************
**
**   VedicCalculationPanel   ---   OnKalaVelaLordChoice
**
******************************************************/
void VedicCalculationPanel::OnKalaVelaLordChoice( wxCommandEvent& )
{
	int selkala = choice_kalavela->GetSelection();
	assert( selkala >= 0 && selkala < 6 );
	kalaVelaCfgLord[selkala] = choice_kalalord->GetSelection() - 1;
}

/*****************************************************
**
**   VedicCalculationPanel   ---   OnKalaVelaPortionChoice
**
******************************************************/
void VedicCalculationPanel::OnKalaVelaPortionChoice( wxCommandEvent& )
{
	int selkala = choice_kalavela->GetSelection();
	assert( selkala >= 0 && selkala < 6 );
	kalaVelaCfgPortion[selkala] = choice_kalaportion->GetSelection();
}

/*****************************************************
**
**   VedicCalculationPanel   ---   OnKalaVelaDefault
**
******************************************************/
void VedicCalculationPanel::OnKalaVelaDefault( wxCommandEvent& )
{
	int selkala = choice_kalavela->GetSelection();
	assert( selkala >= 0 && selkala < 6 );

	kalaVelaCfgLord[0] = KALAVELA_LORD1;     // Kala - Sun
	kalaVelaCfgLord[1] = KALAVELA_LORD3;     // Mriyu - Mars
	kalaVelaCfgLord[2] = KALAVELA_LORD4;     // Ardhaprahara - Mercury
	kalaVelaCfgLord[3] = KALAVELA_LORD5;     // Yamagantaka - Jupiter
	kalaVelaCfgLord[4] = KALAVELA_LORD7;     // Gulika - Saturn
	kalaVelaCfgLord[5] = KALAVELA_LORD_NONE; // Mandi - None
	for ( int i = 0; i < 6; ) kalaVelaCfgPortion[i++] = KALAVELA_PORTION_START;

	choice_kalalord->SetSelection( kalaVelaCfgLord[selkala] + 1 );
	choice_kalaportion->SetSelection( kalaVelaCfgPortion[selkala] );
	choice_kalaportion->Enable( kalaVelaCfgPortion[selkala] != -1 );
}

/*****************************************************
**
**   VedicCalculationPanel   ---   set_properties
**
******************************************************/
void VedicCalculationPanel::set_properties()
{
    // begin wxGlade: VedicCalculationPanel::set_properties
    choice_vcalc_aya->SetToolTip(_("Custom Ayanamsa parameters can be configured under General->Ephemeris"));
    choice_node->SetToolTip(_("Mean Node is most frequently used in Vedic Astrology"));
    choice_node->SetSelection(0);
    choice_house->SetSelection(0);
    check_westernhouse->SetToolTip(_("Checked: houses start a cusp; Unchecked: cusp is middle of house"));
    choice_av_calcrules->SetSelection(0);
    choice_av_benefics->SetSelection(0);
    choice_av_sarva->SetSelection(0);
    choice_av_sodhya->SetSelection(0);
    choice_moon->SetToolTip(_("Angle is Moon - Sun"));
    choice_moon->SetSelection(0);
    choice_mercury->SetSelection(0);
    choice_kalavela->SetSelection(0);
    choice_kalalord->SetSelection(0);
    choice_kalaportion->SetSelection(0);
    choice_arudha->SetSelection(0);
    // end wxGlade
}

/*****************************************************
**
**   VedicCalculationPanel   ---   do_layout
**
******************************************************/
void VedicCalculationPanel::do_layout()
{
    // begin wxGlade: VedicCalculationPanel::do_layout
    wxFlexGridSizer* sizer_main = new wxFlexGridSizer(1, 2, 3, 3);
    wxFlexGridSizer* sizer_right = new wxFlexGridSizer(2, 1, 3, 3);
    wxStaticBoxSizer* sizer_misc = new wxStaticBoxSizer(sizer_misc_staticbox, wxHORIZONTAL);
    wxFlexGridSizer* grid_misc = new wxFlexGridSizer(3, 2, 3, 3);
    wxStaticBoxSizer* sizer_kalavelas = new wxStaticBoxSizer(sizer_kalavelas_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_kalavelas = new wxFlexGridSizer(3, 2, 3, 3);
    wxStaticBoxSizer* sizer_benefics = new wxStaticBoxSizer(sizer_benefics_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_sizer_other = new wxFlexGridSizer(3, 2, 3, 3);
    wxFlexGridSizer* sizer_left = new wxFlexGridSizer(2, 1, 0, 0);
    wxStaticBoxSizer* sizer_av = new wxStaticBoxSizer(sizer_av_staticbox, wxHORIZONTAL);
    wxFlexGridSizer* grid_av = new wxFlexGridSizer(4, 2, 3, 3);
    wxStaticBoxSizer* sizer_basic = new wxStaticBoxSizer(sizer_basic_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_calc1 = new wxFlexGridSizer(3, 2, 3, 3);
    grid_calc1->Add(label_vcalc_aya, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_calc1->Add(choice_vcalc_aya, 1, wxALL|wxEXPAND, 3);
    grid_calc1->Add(label_vcalc_node, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_calc1->Add(choice_node, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    grid_calc1->Add(label_vcalc_house, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_calc1->Add(choice_house, 0, wxALL|wxEXPAND, 3);
    grid_calc1->AddGrowableCol(1);
    sizer_basic->Add(grid_calc1, 1, wxALL, 3);
    sizer_basic->Add(check_westernhouse, 0, wxALL|wxEXPAND, 3);
    sizer_left->Add(sizer_basic, 1, wxALL|wxEXPAND, 3);
    grid_av->Add(label_av_calcrules, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_av->Add(choice_av_calcrules, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    grid_av->Add(label_av_benefics, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_av->Add(choice_av_benefics, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    grid_av->Add(label_av_sarva, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_av->Add(choice_av_sarva, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    grid_av->Add(label_av_sodhya, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_av->Add(choice_av_sodhya, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    grid_av->AddGrowableCol(1);
    sizer_av->Add(grid_av, 1, wxALL|wxEXPAND, 3);
    sizer_left->Add(sizer_av, 1, wxALL|wxEXPAND, 3);
    sizer_main->Add(sizer_left, 1, wxALL|wxEXPAND, 3);
    grid_sizer_other->Add(label_moon, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_other->Add(choice_moon, 0, wxALL|wxEXPAND, 3);
    grid_sizer_other->Add(label_mercury, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_other->Add(choice_mercury, 0, wxALL|wxEXPAND, 3);
    grid_sizer_other->AddGrowableCol(1);
    sizer_benefics->Add(grid_sizer_other, 0, wxEXPAND, 0);
    sizer_right->Add(sizer_benefics, 1, wxALL|wxEXPAND, 3);
    grid_kalavelas->Add(label_kalavela, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_kalavelas->Add(choice_kalavela, 0, wxALL|wxEXPAND, 3);
    grid_kalavelas->Add(label_kalalord, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_kalavelas->Add(choice_kalalord, 0, wxALL|wxEXPAND, 3);
    grid_kalavelas->Add(label_kalaportion, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_kalavelas->Add(choice_kalaportion, 0, wxALL|wxEXPAND, 3);
    grid_kalavelas->AddGrowableCol(1);
    sizer_kalavelas->Add(grid_kalavelas, 1, wxALL|wxEXPAND, 3);
    sizer_kalavelas->Add(button_kaladefault, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
    sizer_right->Add(sizer_kalavelas, 1, wxALL|wxEXPAND, 3);
    grid_misc->Add(label_arudha, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_misc->Add(choice_arudha, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    grid_misc->AddGrowableCol(1);
    sizer_misc->Add(grid_misc, 1, wxALL|wxEXPAND, 3);
    sizer_right->Add(sizer_misc, 1, wxALL|wxEXPAND, 3);
    sizer_main->Add(sizer_right, 1, wxALL|wxEXPAND, 3);
    SetSizer(sizer_main);
    sizer_main->Fit(this);
    // end wxGlade
}

/*****************************************************
**
**   ConfigPanelFactory   ---   createVedicCalculationPanel
**
******************************************************/
ConfigPanel *ConfigPanelFactory::createVedicCalculationPanel( wxWindow *parent )
{
	return new VedicCalculationPanel( parent );
}

