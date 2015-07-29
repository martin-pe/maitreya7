/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/ColorPanel.cpp
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
#pragma implementation "ColorPanel.h"
#endif

#include "ColorPanel.h"

#include "ColorConfig.h"
#include "Conf.h"
#include "guibase.h"

#include <wx/clrpicker.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/stattext.h>

extern Config *config;

enum { CD_SET_DEFAULTS = wxID_HIGHEST + 1 };

IMPLEMENT_CLASS( ColorPanel, ConfigPanel )

/*****************************************************
**
**   ColorPanel   ---   Constructor
**
******************************************************/
ColorPanel::ColorPanel( wxWindow* parent )
		: ConfigPanel( parent )
{
    // begin wxGlade: ColorPanel::ColorPanel
    sizer_bgcolors_staticbox = new wxStaticBox(this, -1, _("Background Colors"));
    sizer_left_staticbox = new wxStaticBox(this, -1, _("Foreground Colors"));
    label_fg = new wxStaticText(this, wxID_ANY, _("Default"));
    button_color_fg = new wxColourPickerCtrl(this, wxID_ANY);
    label_planet = new wxStaticText(this, wxID_ANY, _("Sbc Planets"));
    button_color_planets = new wxColourPickerCtrl(this, wxID_ANY);
    label_transit = new wxStaticText(this, wxID_ANY, _("Sbc Transits"));
    button_color_transits = new wxColourPickerCtrl(this, wxID_ANY);
    label_benefic = new wxStaticText(this, wxID_ANY, _("Benefics"));
    button_color_benefic = new wxColourPickerCtrl(this, wxID_ANY);
    label_malefic = new wxStaticText(this, wxID_ANY, _("Malefics"));
    button_color_malefic = new wxColourPickerCtrl(this, wxID_ANY);
    label_bg = new wxStaticText(this, wxID_ANY, _("Default"));
    button_color_bg = new wxColourPickerCtrl(this, wxID_ANY);
    label_header_copy = new wxStaticText(this, wxID_ANY, _("Table Header"));
    button_color_header = new wxColourPickerCtrl(this, wxID_ANY);
    label_marked = new wxStaticText(this, wxID_ANY, _("Marked"));
    button_color_marked = new wxColourPickerCtrl(this, wxID_ANY);
    label_error = new wxStaticText(this, wxID_ANY, _("Errors"));
    button_color_error = new wxColourPickerCtrl(this, wxID_ANY);
    button_default = new wxButton(this, CD_SET_DEFAULTS, _("Restore Defaults"));

    set_properties();
    do_layout();
    // end wxGlade

	Connect( CD_SET_DEFAULTS, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ColorPanel::OnSetDefaults ));
}

/*****************************************************
**
**   ColorPanel   ---   setData
**
******************************************************/
void ColorPanel::setData()
{
	button_color_fg->SetColour( config->colors->fgColor );
	button_color_planets->SetColour( config->colors->sbcPlanetFgColor );
	button_color_transits->SetColour( config->colors->sbcTransitFgColor );
	button_color_bg->SetColour( config->colors->bgColor );
	button_color_header->SetColour( config->colors->tableHeaderBgColor );
	button_color_marked->SetColour( config->colors->sbcMarkedFieldsBgColor );
	button_color_benefic->SetColour( config->colors->sbcBeneficBgColor );
	button_color_malefic->SetColour( config->colors->sbcMaleficBgColor );
	button_color_error->SetColour( config->colors->errorBgColor );
}

/*****************************************************
**
**   ColorPanel   ---   saveData
**
******************************************************/
bool ColorPanel::saveData()
{
	config->colors->fgColor = button_color_fg->GetColour().GetAsString( wxC2S_HTML_SYNTAX );
	config->colors->sbcPlanetFgColor = button_color_planets->GetColour().GetAsString( wxC2S_HTML_SYNTAX );
	config->colors->sbcTransitFgColor = button_color_transits->GetColour().GetAsString( wxC2S_HTML_SYNTAX );
	config->colors->bgColor = button_color_bg->GetColour().GetAsString( wxC2S_HTML_SYNTAX );
	config->colors->tableHeaderBgColor = button_color_header->GetColour().GetAsString( wxC2S_HTML_SYNTAX );
	config->colors->sbcMarkedFieldsBgColor = button_color_marked->GetColour().GetAsString( wxC2S_HTML_SYNTAX );
	config->colors->sbcBeneficBgColor = button_color_benefic->GetColour().GetAsString( wxC2S_HTML_SYNTAX );
	config->colors->sbcMaleficBgColor = button_color_malefic->GetColour().GetAsString( wxC2S_HTML_SYNTAX );
	config->colors->errorBgColor = button_color_error->GetColour().GetAsString( wxC2S_HTML_SYNTAX );
	return true;
}

/*****************************************************
**
**   ColorPanel   ---   OnSetDefaults
**
******************************************************/
void ColorPanel::OnSetDefaults( wxCommandEvent &event )
{
	ColorConfig cfg;
	button_color_fg->SetColour( cfg.fgColor );
	button_color_planets->SetColour( cfg.sbcPlanetFgColor );
	button_color_transits->SetColour( cfg.sbcTransitFgColor );
	button_color_bg->SetColour( cfg.bgColor );
	button_color_header->SetColour( cfg.tableHeaderBgColor );
	button_color_marked->SetColour( cfg.sbcMarkedFieldsBgColor );
	button_color_benefic->SetColour( cfg.sbcBeneficBgColor );
	button_color_malefic->SetColour( cfg.sbcMaleficBgColor );
	button_color_error->SetColour( cfg.errorBgColor );
}

/*****************************************************
**
**   ColorPanel   ---   set_properties
**
******************************************************/
void ColorPanel::set_properties()
{
    // begin wxGlade: ColorPanel::set_properties
    button_color_fg->SetMinSize(wxSize(60,24));
    button_color_planets->SetMinSize(wxSize(60,24));
    button_color_transits->SetMinSize(wxSize(60,24));
    button_color_benefic->SetMinSize(wxSize(60,24));
    button_color_malefic->SetMinSize(wxSize(60,24));
    button_color_bg->SetMinSize(wxSize(60,24));
    button_color_header->SetMinSize(wxSize(60,24));
    button_color_marked->SetMinSize(wxSize(60,24));
    button_color_error->SetMinSize(wxSize(60, 24));
    button_default->SetToolTip(_("Restore default settings"));
    // end wxGlade
}

/*****************************************************
**
**   ColorPanel   ---   do_layout
**
******************************************************/
void ColorPanel::do_layout()
{
    // begin wxGlade: ColorPanel::do_layout
    wxFlexGridSizer* sizer_main = new wxFlexGridSizer(1, 3, 0, 0);
    wxBoxSizer* sizer_vert = new wxBoxSizer(wxVERTICAL);
    wxFlexGridSizer* grid_lr = new wxFlexGridSizer(1, 2, 0, 0);
    wxFlexGridSizer* sizer_right = new wxFlexGridSizer(3, 1, 0, 0);
    wxStaticBoxSizer* sizer_bgcolors = new wxStaticBoxSizer(sizer_bgcolors_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_bgcolors = new wxFlexGridSizer(4, 2, 3, 3);
    wxStaticBoxSizer* sizer_left = new wxStaticBoxSizer(sizer_left_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_fgcolors = new wxFlexGridSizer(8, 2, 3, 3);
    grid_fgcolors->Add(label_fg, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_fgcolors->Add(button_color_fg, 1, wxALL|wxEXPAND, 3);
    grid_fgcolors->Add(label_planet, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_fgcolors->Add(button_color_planets, 1, wxALL|wxEXPAND, 3);
    grid_fgcolors->Add(label_transit, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_fgcolors->Add(button_color_transits, 1, wxALL|wxEXPAND, 3);
    grid_fgcolors->Add(label_benefic, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_fgcolors->Add(button_color_benefic, 1, wxALL|wxEXPAND, 3);
    grid_fgcolors->Add(label_malefic, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_fgcolors->Add(button_color_malefic, 1, wxALL|wxEXPAND, 3);
    grid_fgcolors->AddGrowableCol(1);
    sizer_left->Add(grid_fgcolors, 1, wxALL|wxEXPAND, 3);
    grid_lr->Add(sizer_left, 1, wxALL, 3);
    grid_bgcolors->Add(label_bg, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_bgcolors->Add(button_color_bg, 1, wxALL|wxEXPAND, 3);
    grid_bgcolors->Add(label_header_copy, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_bgcolors->Add(button_color_header, 1, wxALL|wxEXPAND, 3);
    grid_bgcolors->Add(label_marked, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_bgcolors->Add(button_color_marked, 1, wxALL|wxEXPAND, 3);
    grid_bgcolors->Add(label_error, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_bgcolors->Add(button_color_error, 1, wxALL|wxEXPAND, 3);
    grid_bgcolors->AddGrowableCol(1);
    sizer_bgcolors->Add(grid_bgcolors, 1, wxALL|wxEXPAND, 3);
    sizer_right->Add(sizer_bgcolors, 1, wxALL, 3);
    sizer_right->AddGrowableRow(1);
    sizer_right->AddGrowableCol(0);
    grid_lr->Add(sizer_right, 1, wxALL, 3);
    sizer_vert->Add(grid_lr, 1, wxEXPAND, 0);
    sizer_vert->Add(button_default, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 3);
    sizer_main->Add(sizer_vert, 1, wxEXPAND, 0);
    SetSizer(sizer_main);
    sizer_main->Fit(this);
    // end wxGlade
}

/*****************************************************
**
**   ConfigPanelFactory   ---   createColorPanel
**
******************************************************/
ConfigPanel *ConfigPanelFactory::createColorPanel( wxWindow *parent )
{
	return new ColorPanel( parent );
}

