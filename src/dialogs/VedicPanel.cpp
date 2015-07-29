/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/VedicPanel.cpp
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
#pragma implementation "VedicPanel.h"
#endif

#include "VedicPanel.h"

#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/choicdlg.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/statline.h>
#include <wx/stattext.h>

#include "ArabicParts.h"
#include "Conf.h"
#include "guibase.h"
#include "Lang.h"
#include "PlanetList.h"
#include "Writer.h"

extern Config *config;

enum { CD_ARABIC = wxID_HIGHEST + 1 };

IMPLEMENT_CLASS( VedicPanel, ConfigPanel )

/*****************************************************
**
**   VedicPanel   ---   Constructor
**
******************************************************/
VedicPanel::VedicPanel( wxWindow* parent )
		: ConfigPanel( parent )
{
    // begin wxGlade: VedicPanel::VedicPanel
    panel_arabic_selection = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL);
    sizer_sobjects_staticbox = new wxStaticBox(this, -1, _("Special Points"));
    sizer_partly_staticbox = new wxStaticBox(this, -1, _("Partly Displayed Points"));
    sizer_arabic_staticbox = new wxStaticBox(this, -1, _("Arabic Parts"));
    sizer_order_lagna_staticbox = new wxStaticBox(this, -1, _("Order of Lagna"));
    sizer_maincols_staticbox = new wxStaticBox(this, -1, _("Columns in Main Window"));
    sizer_eobjects_staticbox = new wxStaticBox(this, -1, _("Ephemeris Objects"));
    check_objects_outer = new wxCheckBox(this, wxID_ANY, _("3 Outer Planets"));
    check_objects_dragonhead = new wxCheckBox(this, wxID_ANY, _("Rahu"));
    check_objects_dragontail = new wxCheckBox(this, wxID_ANY, _("Ketu"));
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
    const wxString choice_order_lagna_choices[] = {
        _("After Planets"),
        _("Top")
    };
    choice_order_lagna = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_order_lagna_choices, 0);
    check_main_declinations = new wxCheckBox(this, wxID_ANY, _("Declinations"));
    check_main_lords = new wxCheckBox(this, wxID_ANY, _("Lord"));
    check_main_dignities = new wxCheckBox(this, wxID_ANY, _("Dignities"));
    check_main_d9 = new wxCheckBox(this, wxID_ANY, _("Navamsa D9"));
    check_main_nak = new wxCheckBox(this, wxID_ANY, _("Nakshatras"));
    check_main_pada = new wxCheckBox(this, wxID_ANY, _("Nakshatra Padas"));
    check_main_kp = new wxCheckBox(this, wxID_ANY, _("Krishnamurti Lords"));
    check_main_d60 = new wxCheckBox(this, wxID_ANY, _("Shastiamsa D60"));
    check_main_d10 = new wxCheckBox(this, wxID_ANY, _("Dasa Varga"));
    check_main_bhava = new wxCheckBox(this, wxID_ANY, _("Bhava Positions"));
    check_main_kara = new wxCheckBox(this, wxID_ANY, _("Jamini Karakas"));
    check_main_ashtaka = new wxCheckBox(this, wxID_ANY, _("Ashtakavarga Points"));

    set_properties();
    do_layout();
    // end wxGlade

	Connect( CD_ARABIC, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( VedicPanel::OnArabicParts ));
}

/*****************************************************
**
**   VedicPanel   ---   setData
**
******************************************************/
void VedicPanel::setData()
{
	check_objects_outer->SetValue( config->vObjects & OBJECTS_INCLUDE_OUTER );
	check_objects_dragonhead->SetValue( config->vObjects & OBJECTS_INCLUDE_DRAGONHEAD );
	check_objects_dragontail->SetValue( config->vObjects & OBJECTS_INCLUDE_DRAGONTAIL );
	check_objects_ascendant->SetValue( config->vObjects & OBJECTS_INCLUDE_ASCENDANT );
	check_objects_meridian->SetValue( config->vObjects & OBJECTS_INCLUDE_MERIDIAN );
	check_objects_uranian->SetValue( config->vObjects & OBJECTS_INCLUDE_URANIAN );
	check_objects_chiron->SetValue( config->vObjects & OBJECTS_INCLUDE_CHIRON );
	check_objects_pholus->SetValue( config->vObjects & OBJECTS_INCLUDE_PHOLUS );
	check_objects_planetoids->SetValue( config->vObjects & OBJECTS_INCLUDE_PLANETOIDS );
	check_objects_lilith->SetValue( config->vObjects & OBJECTS_INCLUDE_LILITH );
	check_objects_upagrahas->SetValue( config->vObjects & OBJECTS_INCLUDE_UPAGRAHAS );
	check_objects_kalavelas->SetValue( config->vObjects & OBJECTS_INCLUDE_KALAVELAS );
	check_objects_lagnas->SetValue( config->vObjects & OBJECTS_INCLUDE_SPECIALLAGNAS );
	check_objects_d9lagna->SetValue( config->vObjects & OBJECTS_INCLUDE_D9_LAGNA );
	check_objects_aries->SetValue( config->vObjects & OBJECTS_INCLUDE_ARIES );

	check_objects_house_cusp->SetValue( config->vMainChildStyle & MAIN_CHILD_SHOW_HOUSES );

	choice_order_lagna->SetSelection( config->vOrderLagna );

	check_main_declinations->SetValue( config->vMainChildStyle & MAIN_CHILD_SHOW_DECLINATION );
	check_main_lords->SetValue( config->vMainChildStyle & MAIN_CHILD_SHOW_LORD );
	check_main_dignities->SetValue( config->vMainChildStyle & MAIN_CHILD_SHOW_DIGNITY );
	check_main_d9->SetValue( config->vMainChildStyle & MAIN_CHILD_SHOW_NAVAMSA );
	check_main_nak->SetValue( config->vMainChildStyle & MAIN_CHILD_SHOW_NAKSHATRA );
	check_main_pada->SetValue( config->vMainChildStyle & MAIN_CHILD_SHOW_PADA );
	check_main_kp->SetValue( config->vMainChildStyle & MAIN_CHILD_SHOW_KP );
	check_main_d60->SetValue( config->vMainChildStyle & MAIN_CHILD_SHOW_SHASTIAMSA );
	check_main_d10->SetValue( config->vMainChildStyle & MAIN_CHILD_SHOW_DASAVARGA );
	check_main_bhava->SetValue( config->vMainChildStyle & MAIN_CHILD_SHOW_HOUSEPOS );
	check_main_kara->SetValue( config->vMainChildStyle & MAIN_CHILD_SHOW_KARAKA );
	check_main_ashtaka->SetValue( config->vMainChildStyle & MAIN_CHILD_SHOW_ASHTAKA );

	arabicParts = config->vArabicParts;
	updateArabicText();
}

/*****************************************************
**
**   VedicPanel   ---   saveData
**
******************************************************/
bool VedicPanel::saveData()
{
	config->vObjects = getMainStyleFromGui();

	config->vArabicParts = arabicParts;
	if ( arabicParts.Count() > 0 ) config->vObjects |= OBJECTS_INCLUDE_ARABICPARTS;

	config->vOrderLagna = choice_order_lagna->GetSelection();

	config->vMainChildStyle = 0;
	if ( check_objects_house_cusp->GetValue()) config->vMainChildStyle |= MAIN_CHILD_SHOW_HOUSES;
	if ( check_main_declinations->GetValue()) config->vMainChildStyle |= MAIN_CHILD_SHOW_DECLINATION;
	if ( check_main_lords->GetValue()) config->vMainChildStyle |= MAIN_CHILD_SHOW_LORD;
	if ( check_main_dignities->GetValue()) config->vMainChildStyle |= MAIN_CHILD_SHOW_DIGNITY;
	if ( check_main_d9->GetValue()) config->vMainChildStyle |= MAIN_CHILD_SHOW_NAVAMSA;
	if ( check_main_nak->GetValue()) config->vMainChildStyle |= MAIN_CHILD_SHOW_NAKSHATRA;
	if ( check_main_pada->GetValue()) config->vMainChildStyle  |= MAIN_CHILD_SHOW_PADA;
	if ( check_main_kp->GetValue()) config->vMainChildStyle |= MAIN_CHILD_SHOW_KP;
	if ( check_main_d60->GetValue()) config->vMainChildStyle |= MAIN_CHILD_SHOW_SHASTIAMSA;
	if ( check_main_d10->GetValue()) config->vMainChildStyle |= MAIN_CHILD_SHOW_DASAVARGA;
	if ( check_main_bhava->GetValue()) config->vMainChildStyle |= MAIN_CHILD_SHOW_HOUSEPOS ;
	if ( check_main_kara->GetValue()) config->vMainChildStyle |= MAIN_CHILD_SHOW_KARAKA;
	if ( check_main_ashtaka->GetValue()) config->vMainChildStyle  |= MAIN_CHILD_SHOW_ASHTAKA;
	return true;
}

/*****************************************************
**
**   VedicPanel   ---   getMainStyleFromGui
**
******************************************************/
int VedicPanel::getMainStyleFromGui()
{
	int style = 0;
#define SET_CONFIG_OBJECT( a, f ) \
  a->GetValue() ? style |= f :  config->vObjects &= ~f;
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
**   VedicPanel   ---   OnArabicParts
**
******************************************************/
void VedicPanel::OnArabicParts( wxCommandEvent &event )
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
**   VedicPanel   ---   updateArabicText
**
******************************************************/
void VedicPanel::updateArabicText()
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
**   VedicPanel   ---   set_properties
**
******************************************************/
void VedicPanel::set_properties()
{
    // begin wxGlade: VedicPanel::set_properties
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
    choice_order_lagna->SetSelection(0);
    check_main_declinations->SetToolTip(_("House positions of the objects"));
    check_main_lords->SetToolTip(_("Owner of the Rasi"));
    check_main_dignities->SetToolTip(_("Exaltation, Moolatrikona, friendship etc."));
    check_main_d9->SetToolTip(_("D-9 chart for marriage"));
    check_main_nak->SetToolTip(_("Lunar mansions or constellations. See next tab for 27 resp. 28 Nakshatra zodiac"));
    check_main_pada->SetToolTip(_("Quarters of Nakshatras. 1 Pada = 1 Navamsa"));
    check_main_kp->SetToolTip(_("Lords and Sublords according to Krishnamurti Paddhati system"));
    check_main_d60->SetToolTip(_("Display of Shastiamsa Lords. Can be used for general indications"));
    check_main_d10->SetToolTip(_("Names of dignities according to Dasa Varga: Parijata, Uttama, Gopura etc."));
    check_main_bhava->SetToolTip(_("Bhava according to house cusps (K.P.) or Bhava Sandhis"));
    check_main_kara->SetToolTip(_("Atma Karaka, Amatya Karaka etc. See next tab for inclusion of Rahu"));
    check_main_ashtaka->SetToolTip(_("Total number of Rekha points"));
    // end wxGlade
}

/*****************************************************
**
**   VedicPanel   ---   do_layout
**
******************************************************/
void VedicPanel::do_layout()
{
    // begin wxGlade: VedicPanel::do_layout
    wxFlexGridSizer* sizer_main = new wxFlexGridSizer(1, 3, 0, 0);
    wxFlexGridSizer* sizer_right = new wxFlexGridSizer(1, 1, 0, 0);
    wxStaticBoxSizer* sizer_maincols = new wxStaticBoxSizer(sizer_maincols_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_maincols = new wxFlexGridSizer(12, 1, 3, 0);
    wxFlexGridSizer* sizer_center = new wxFlexGridSizer(4, 1, 0, 0);
    wxStaticBoxSizer* sizer_order_lagna = new wxStaticBoxSizer(sizer_order_lagna_staticbox, wxVERTICAL);
    wxStaticBoxSizer* sizer_arabic = new wxStaticBoxSizer(sizer_arabic_staticbox, wxVERTICAL);
    wxBoxSizer* sizer_arabic_selection = new wxBoxSizer(wxHORIZONTAL);
    wxStaticBoxSizer* sizer_partly = new wxStaticBoxSizer(sizer_partly_staticbox, wxVERTICAL);
    wxStaticBoxSizer* sizer_sobjects = new wxStaticBoxSizer(sizer_sobjects_staticbox, wxHORIZONTAL);
    wxFlexGridSizer* grid_sobjects = new wxFlexGridSizer(3, 1, 3, 3);
    wxFlexGridSizer* sizer_left = new wxFlexGridSizer(1, 1, 0, 0);
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
    sizer_sobjects->Add(grid_sobjects, 1, wxALL, 3);
    sizer_center->Add(sizer_sobjects, 1, wxALL, 3);
    sizer_partly->Add(check_objects_d9lagna, 0, wxALL|wxEXPAND, 3);
    sizer_partly->Add(check_objects_aries, 0, wxALL|wxEXPAND, 3);
    sizer_center->Add(sizer_partly, 1, wxALL|wxEXPAND, 3);
    sizer_arabic->Add(button_arabic, 0, wxALL, 3);
    sizer_arabic_selection->Add(label_arabic_selection, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    panel_arabic_selection->SetSizer(sizer_arabic_selection);
    sizer_arabic->Add(panel_arabic_selection, 1, wxALL|wxEXPAND, 3);
    sizer_center->Add(sizer_arabic, 1, wxEXPAND, 0);
    sizer_order_lagna->Add(choice_order_lagna, 0, wxALL|wxEXPAND, 3);
    sizer_center->Add(sizer_order_lagna, 1, wxEXPAND, 0);
    sizer_center->AddGrowableCol(0);
    sizer_main->Add(sizer_center, 1, wxALL, 3);
    grid_maincols->Add(check_main_declinations, 0, wxALL|wxEXPAND, 3);
    grid_maincols->Add(check_main_lords, 0, wxALL|wxEXPAND, 3);
    grid_maincols->Add(check_main_dignities, 0, wxALL|wxEXPAND, 3);
    grid_maincols->Add(check_main_d9, 0, wxALL|wxEXPAND, 3);
    grid_maincols->Add(check_main_nak, 0, wxALL|wxEXPAND, 3);
    grid_maincols->Add(check_main_pada, 0, wxALL|wxEXPAND, 3);
    grid_maincols->Add(check_main_kp, 0, wxALL|wxEXPAND, 3);
    grid_maincols->Add(check_main_d60, 0, wxALL|wxEXPAND, 3);
    grid_maincols->Add(check_main_d10, 0, wxALL|wxEXPAND, 3);
    grid_maincols->Add(check_main_bhava, 0, wxALL|wxEXPAND, 3);
    grid_maincols->Add(check_main_kara, 0, wxALL|wxEXPAND, 3);
    grid_maincols->Add(check_main_ashtaka, 0, wxALL|wxEXPAND, 3);
    grid_maincols->AddGrowableCol(0);
    sizer_maincols->Add(grid_maincols, 1, wxALL, 3);
    sizer_right->Add(sizer_maincols, 1, wxALL, 3);
    sizer_right->AddGrowableRow(0);
    sizer_right->AddGrowableCol(0);
    sizer_main->Add(sizer_right, 1, wxALL, 3);
    SetSizer(sizer_main);
    sizer_main->Fit(this);
    // end wxGlade
}

/*****************************************************
**
**   ConfigPanelFactory   ---   createVedicPanel
**
******************************************************/
ConfigPanel *ConfigPanelFactory::createVedicPanel( wxWindow *parent )
{
	return new VedicPanel( parent );
}

