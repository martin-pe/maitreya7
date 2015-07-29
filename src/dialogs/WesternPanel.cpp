/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/WesternPanel.cpp
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
#pragma implementation "WesternPanel.h"
#endif

#include "WesternPanel.h"

#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/choicdlg.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/stattext.h>

#include "ArabicParts.h"
#include "Conf.h"
#include "PlanetList.h"
#include "Writer.h"

extern Config *config;

enum { CD_ARABIC = wxID_HIGHEST + 1  };

IMPLEMENT_CLASS( WesternPanel, wxPanel )

/*****************************************************
**
**   WesternPanel   ---   Constructor
**
******************************************************/
WesternPanel::WesternPanel( wxWindow* parent )
		: ConfigPanel( parent )
{
    // begin wxGlade: WesternPanel::WesternPanel
    panel_arabic_selection = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL);
    sizer_sobjects_staticbox = new wxStaticBox(this, -1, _("Special Points"));
    sizer_partly_staticbox = new wxStaticBox(this, -1, _("Partly Displayed Points"));
    sizer_arabic_staticbox = new wxStaticBox(this, -1, _("Arabic Parts"));
    sizer_mainwindow_staticbox = new wxStaticBox(this, -1, _("Columns in Main Window"));
    sizer_planetorder_staticbox = new wxStaticBox(this, -1, _("Order of Planets"));
    sizer_eobjects_staticbox = new wxStaticBox(this, -1, _("Ephemeris Objects"));
    check_objects_outer = new wxCheckBox(this, wxID_ANY, _("3 Outer Planets"));
    check_objects_dragonhead = new wxCheckBox(this, wxID_ANY, _("Dragon Head"));
    check_objects_dragontail = new wxCheckBox(this, wxID_ANY, _("Dragon Tail"));
    check_objects_ascendant = new wxCheckBox(this, wxID_ANY, _("Ascendant"));
    check_objects_meridian = new wxCheckBox(this, wxID_ANY, _("Meridian"));
    check_objects_uranian = new wxCheckBox(this, wxID_ANY, _("8 Uranian"));
    check_objects_chiron = new wxCheckBox(this, wxID_ANY, _("Chiron"));
    check_objects_pholus = new wxCheckBox(this, wxID_ANY, _("Pholus"));
    check_objects_planetoids = new wxCheckBox(this, wxID_ANY, _("4 Planetoids"));
    check_objects_lilith = new wxCheckBox(this, wxID_ANY, _("Lilith (Black Moon)"));
    check_objects_house_cusp = new wxCheckBox(this, wxID_ANY, _("House Cusps"));
    check_objects_upagrahas = new wxCheckBox(this, wxID_ANY, _("5 Upagrahas"));
    check_objects_kalavelas = new wxCheckBox(this, wxID_ANY, _("Kalavelas"));
    check_objects_lagnas = new wxCheckBox(this, wxID_ANY, _("3 Special Lagnas"));
    check_objects_d9lagna = new wxCheckBox(this, wxID_ANY, _("Sphuta Navamsa Lagna"));
    check_objects_aries = new wxCheckBox(this, wxID_ANY, _("Aries"));
    button_arabic = new wxButton(this, CD_ARABIC, _("Select Parts ..."));
    label_arabic_selection = new wxStaticText(panel_arabic_selection, wxID_ANY, _("No parts selected"));
    check_main_declinations = new wxCheckBox(this, wxID_ANY, _("Declinations"));
    check_main_housepos = new wxCheckBox(this, wxID_ANY, _("House Positions"));
    label_planetorder = new wxStaticText(this, wxID_ANY, _("Order"));
    const wxString choice_planetorder_choices[] = {
        _("Default"),
        _("Uranian")
    };
    choice_planetorder = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_planetorder_choices, 0);

    set_properties();
    do_layout();
    // end wxGlade

	Connect( CD_ARABIC, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( WesternPanel::OnArabicParts ));
}

/*****************************************************
**
**   WesternPanel   ---   setData
**
******************************************************/
void WesternPanel::setData()
{
	check_objects_outer->SetValue( config->wObjects & OBJECTS_INCLUDE_OUTER );
	check_objects_dragonhead->SetValue( config->wObjects & OBJECTS_INCLUDE_DRAGONHEAD );
	check_objects_dragontail->SetValue( config->wObjects & OBJECTS_INCLUDE_DRAGONTAIL );
	check_objects_ascendant->SetValue( config->wObjects & OBJECTS_INCLUDE_ASCENDANT );
	check_objects_meridian->SetValue( config->wObjects & OBJECTS_INCLUDE_MERIDIAN );
	check_objects_uranian->SetValue( config->wObjects & OBJECTS_INCLUDE_URANIAN );
	check_objects_chiron->SetValue( config->wObjects & OBJECTS_INCLUDE_CHIRON );
	check_objects_pholus->SetValue( config->wObjects & OBJECTS_INCLUDE_PHOLUS );
	check_objects_planetoids->SetValue( config->wObjects & OBJECTS_INCLUDE_PLANETOIDS );
	check_objects_lilith->SetValue( config->wObjects & OBJECTS_INCLUDE_LILITH );
	check_objects_house_cusp->SetValue( config->wMainChildStyle & MAIN_CHILD_SHOW_HOUSES );

	check_objects_upagrahas->SetValue( config->wObjects & OBJECTS_INCLUDE_UPAGRAHAS );
	check_objects_kalavelas->SetValue( config->wObjects & OBJECTS_INCLUDE_KALAVELAS );
	check_objects_lagnas->SetValue( config->wObjects & OBJECTS_INCLUDE_SPECIALLAGNAS );
	check_objects_aries->SetValue( config->wObjects & OBJECTS_INCLUDE_ARIES );
	check_objects_d9lagna->SetValue( config->wObjects & OBJECTS_INCLUDE_D9_LAGNA );

	check_main_housepos->SetValue( config->wMainChildStyle & MAIN_CHILD_SHOW_HOUSEPOS );
	check_main_declinations->SetValue( config->wMainChildStyle & MAIN_CHILD_SHOW_DECLINATION );

	choice_planetorder->SetSelection( config->wPlanetOrder );

	arabicParts = config->wArabicParts;
	updateArabicText();
}

/*****************************************************
**
**   WesternPanel   ---   saveData
**
******************************************************/
bool WesternPanel::saveData()
{
	config->wObjects = getMainStyleFromGui();

	config->wArabicParts = arabicParts;
	if ( arabicParts.Count() > 0 ) config->wObjects |= OBJECTS_INCLUDE_ARABICPARTS;

	// Main Window Style
	config->wMainChildStyle = 0;
	if ( check_objects_house_cusp->GetValue() ) config->wMainChildStyle |= MAIN_CHILD_SHOW_HOUSES;
	if ( check_main_housepos->GetValue() ) config->wMainChildStyle |= MAIN_CHILD_SHOW_HOUSEPOS;
	if ( check_main_declinations->GetValue() ) config->wMainChildStyle |= MAIN_CHILD_SHOW_DECLINATION;

	config->wPlanetOrder = choice_planetorder->GetSelection();
	return true;
}

/*****************************************************
**
**   WesternPanel   ---   getMainStyleFromGui
**
******************************************************/
int WesternPanel::getMainStyleFromGui()
{
	int style = 0;
#define SET_CONFIG_OBJECT( a, f ) \
  a->GetValue() ? style |= f :  config->wObjects &= ~f;
	SET_CONFIG_OBJECT( check_objects_outer, OBJECTS_INCLUDE_OUTER )
	SET_CONFIG_OBJECT( check_objects_dragonhead, OBJECTS_INCLUDE_DRAGONHEAD )
	SET_CONFIG_OBJECT( check_objects_dragontail, OBJECTS_INCLUDE_DRAGONTAIL )
	SET_CONFIG_OBJECT( check_objects_ascendant, OBJECTS_INCLUDE_ASCENDANT )
	SET_CONFIG_OBJECT( check_objects_meridian, OBJECTS_INCLUDE_MERIDIAN )
	SET_CONFIG_OBJECT( check_objects_uranian, OBJECTS_INCLUDE_URANIAN )
	SET_CONFIG_OBJECT( check_objects_chiron, OBJECTS_INCLUDE_CHIRON )
	SET_CONFIG_OBJECT( check_objects_pholus, OBJECTS_INCLUDE_PHOLUS )
	SET_CONFIG_OBJECT( check_objects_planetoids, OBJECTS_INCLUDE_PLANETOIDS )
	SET_CONFIG_OBJECT( check_objects_lilith, OBJECTS_INCLUDE_LILITH )
	SET_CONFIG_OBJECT( check_objects_upagrahas, OBJECTS_INCLUDE_UPAGRAHAS )
	SET_CONFIG_OBJECT( check_objects_kalavelas, OBJECTS_INCLUDE_KALAVELAS )
	SET_CONFIG_OBJECT( check_objects_lagnas, OBJECTS_INCLUDE_SPECIALLAGNAS )
	SET_CONFIG_OBJECT( check_objects_aries, OBJECTS_INCLUDE_ARIES )
	SET_CONFIG_OBJECT( check_objects_d9lagna, OBJECTS_INCLUDE_D9_LAGNA )
#undef SET_CONFIG_OBJECT
	return style;
}

/*****************************************************
**
**   WesternPanel   ---   OnArabicParts
**
******************************************************/
void WesternPanel::OnArabicParts( wxCommandEvent &event )
{
	wxString parts[MAX_ARABIC_OBJECTS];
	for ( unsigned int i = 0; i < MAX_ARABIC_OBJECTS; i++ ) parts[i] = ArabicPartsExpert::getObjectName( FIRST_ARABIC_OBJECT + i, TLARGE );

	wxMultiChoiceDialog dialog( this,	_( "Choose Arabic Parts" ), GUI_APP_NAME, MAX_ARABIC_OBJECTS, parts );
	dialog.SetSelections( arabicParts );
	if ( dialog.ShowModal() == wxID_OK ) arabicParts = dialog.GetSelections();
	updateArabicText();
}

/*****************************************************
**
**   WesternPanel   ---   updateArabicText
**
******************************************************/
void WesternPanel::updateArabicText()
{
	wxString s;
	if ( arabicParts.size() == 0 )
	{
		label_arabic_selection->SetLabel( _( "No part selected" ));
	}
	else if ( arabicParts.size() == 1 )
	{
		label_arabic_selection->SetLabel( ArabicPartsExpert::getObjectName( arabicParts[0] + FIRST_ARABIC_OBJECT, TLARGE ));
	}
	else
	{
		s.Printf( _( "%d parts selected" ), arabicParts.size());
		label_arabic_selection->SetLabel( s );
	}
}

/*****************************************************
**
**   WesternPanel   ---   set_properties
**
******************************************************/
void WesternPanel::set_properties()
{
    // begin wxGlade: WesternPanel::set_properties
    check_objects_outer->SetToolTip(_("Uranus, Neptune and Pluto"));
    check_objects_dragonhead->SetToolTip(_("Uranus, Neptune and Pluto"));
    check_objects_dragontail->SetToolTip(_("Uranus, Neptune and Pluto"));
    check_objects_ascendant->SetToolTip(_("Uranus, Neptune and Pluto"));
    check_objects_meridian->SetToolTip(_("Uranus, Neptune and Pluto"));
    check_objects_uranian->SetToolTip(_("Fictitious planets of Uranian astrology: Cupido, Hades, Zeus, Kronos, Apollon, Admetos, Vulkanus and Poseidon "));
    check_objects_chiron->SetToolTip(_("Asteroid between Saturn and Uranus. Period 50 years"));
    check_objects_pholus->SetToolTip(_("Asteroid crossing the orbits of outer planets. Period 92 years"));
    check_objects_planetoids->SetToolTip(_("Ceres, Pallas, Juno and Vesta"));
    check_objects_house_cusp->SetToolTip(_("House positions of the objects"));
    check_objects_upagrahas->SetToolTip(_("Dhuma, Vyatipata, Parivesha, Chapa and Upaketu"));
    check_objects_kalavelas->SetToolTip(_("Kala, Mrityu, Ardhaprahara, Yamaghantaka, Gulika and Mandi"));
    check_objects_lagnas->SetToolTip(_("Bhava Lagna, Hora Lagna and Ghatika Lagna"));
    check_objects_d9lagna->SetToolTip(_("Length of Navamsa Lagna"));
    check_objects_aries->SetToolTip(_("Used in Uranian Astrology"));
    check_main_declinations->SetMinSize(wxSize(180, -1));
    choice_planetorder->SetSelection(0);
    // end wxGlade
}

/*****************************************************
**
**   WesternPanel   ---   do_layout
**
******************************************************/
void WesternPanel::do_layout()
{
    // begin wxGlade: WesternPanel::do_layout
    wxFlexGridSizer* sizer_main = new wxFlexGridSizer(1, 3, 0, 0);
    wxFlexGridSizer* sizer_right = new wxFlexGridSizer(3, 1, 3, 3);
    wxStaticBoxSizer* sizer_planetorder = new wxStaticBoxSizer(sizer_planetorder_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_planetorder2 = new wxFlexGridSizer(1, 2, 3, 3);
    wxStaticBoxSizer* sizer_mainwindow = new wxStaticBoxSizer(sizer_mainwindow_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_sizer_2 = new wxFlexGridSizer(2, 1, 3, 3);
    wxFlexGridSizer* sizer_center = new wxFlexGridSizer(3, 1, 0, 0);
    wxStaticBoxSizer* sizer_arabic = new wxStaticBoxSizer(sizer_arabic_staticbox, wxVERTICAL);
    wxBoxSizer* sizer_arabic_selection = new wxBoxSizer(wxHORIZONTAL);
    wxStaticBoxSizer* sizer_partly = new wxStaticBoxSizer(sizer_partly_staticbox, wxVERTICAL);
    wxStaticBoxSizer* sizer_sobjects = new wxStaticBoxSizer(sizer_sobjects_staticbox, wxHORIZONTAL);
    wxFlexGridSizer* grid_sobjects = new wxFlexGridSizer(7, 1, 3, 3);
    wxFlexGridSizer* sizer_left = new wxFlexGridSizer(2, 1, 0, 0);
    wxStaticBoxSizer* sizer_eobjects = new wxStaticBoxSizer(sizer_eobjects_staticbox, wxHORIZONTAL);
    wxFlexGridSizer* grid_eobjects = new wxFlexGridSizer(11, 1, 3, 3);
    grid_eobjects->Add(check_objects_outer, 0, wxALL|wxEXPAND, 3);
    grid_eobjects->Add(check_objects_dragonhead, 0, wxALL|wxEXPAND, 3);
    grid_eobjects->Add(check_objects_dragontail, 0, wxALL|wxEXPAND, 3);
    grid_eobjects->Add(check_objects_ascendant, 0, wxALL|wxEXPAND, 3);
    grid_eobjects->Add(check_objects_meridian, 0, wxALL|wxEXPAND, 3);
    grid_eobjects->Add(check_objects_uranian, 0, wxALL|wxEXPAND, 3);
    grid_eobjects->Add(check_objects_chiron, 0, wxALL|wxEXPAND, 3);
    grid_eobjects->Add(check_objects_pholus, 0, wxALL|wxEXPAND, 3);
    grid_eobjects->Add(check_objects_planetoids, 0, wxALL|wxEXPAND, 3);
    grid_eobjects->Add(check_objects_lilith, 0, wxALL|wxEXPAND, 3);
    grid_eobjects->Add(check_objects_house_cusp, 0, wxALL|wxEXPAND, 3);
    sizer_eobjects->Add(grid_eobjects, 1, wxALL, 3);
    sizer_left->Add(sizer_eobjects, 1, wxALL, 3);
    sizer_left->AddGrowableCol(0);
    sizer_main->Add(sizer_left, 1, wxALL, 3);
    grid_sobjects->Add(check_objects_upagrahas, 0, wxALL|wxEXPAND, 3);
    grid_sobjects->Add(check_objects_kalavelas, 0, wxALL|wxEXPAND, 3);
    grid_sobjects->Add(check_objects_lagnas, 0, wxALL|wxEXPAND, 3);
    sizer_sobjects->Add(grid_sobjects, 1, wxALL|wxEXPAND, 3);
    sizer_center->Add(sizer_sobjects, 1, wxALL|wxEXPAND, 3);
    sizer_partly->Add(check_objects_d9lagna, 0, wxALL|wxEXPAND, 3);
    sizer_partly->Add(check_objects_aries, 0, wxALL|wxEXPAND, 3);
    sizer_center->Add(sizer_partly, 1, wxALL|wxEXPAND, 3);
    sizer_arabic->Add(button_arabic, 0, wxALL, 3);
    sizer_arabic_selection->Add(label_arabic_selection, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    panel_arabic_selection->SetSizer(sizer_arabic_selection);
    sizer_arabic->Add(panel_arabic_selection, 1, wxALL|wxEXPAND, 3);
    sizer_center->Add(sizer_arabic, 1, wxEXPAND, 0);
    sizer_center->AddGrowableCol(0);
    sizer_main->Add(sizer_center, 1, wxALL, 3);
    grid_sizer_2->Add(check_main_declinations, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_2->Add(check_main_housepos, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    sizer_mainwindow->Add(grid_sizer_2, 1, wxALL|wxEXPAND, 3);
    sizer_right->Add(sizer_mainwindow, 1, wxALL|wxEXPAND, 3);
    grid_planetorder2->Add(label_planetorder, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_planetorder2->Add(choice_planetorder, 0, wxALL|wxEXPAND, 3);
    sizer_planetorder->Add(grid_planetorder2, 1, wxALL|wxEXPAND, 3);
    sizer_right->Add(sizer_planetorder, 1, wxALL|wxEXPAND, 3);
    sizer_right->AddGrowableCol(0);
    sizer_main->Add(sizer_right, 1, wxALL|wxEXPAND, 3);
    SetSizer(sizer_main);
    sizer_main->Fit(this);
    // end wxGlade
}

/*****************************************************
**
**   ConfigPanelFactory   ---   createWesternPanel
**
******************************************************/
ConfigPanel *ConfigPanelFactory::createWesternPanel( wxWindow *parent )
{
	return new WesternPanel( parent );
}

