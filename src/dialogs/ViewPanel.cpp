/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/ViewPanel.cpp
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
#pragma implementation "ViewPanel.h"
#endif

#include "ViewPanel.h"

#include "Conf.h"
#include "guibase.h"
#include "Horoscope.h"
#include "Lang.h"
#include "PlanetList.h"
#include "TextWidget.h"
#include "Writer.h"

#include <wx/choice.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/stattext.h>

extern Config *config;

IMPLEMENT_CLASS( ViewPanel, ConfigPanel )

/*************************************************//**
*
*
*
******************************************************/
class ViewPanelTextWidget : public TextWidget
{
	DECLARE_CLASS( ViewPanelTextWidget )

public:
	ViewPanelTextWidget( wxWindow *parent, BasicView *v, Config *cfg )
			: TextWidget( parent, v, cfg )
	{
		this->cfg = cfg;
		horoscope = new Horoscope();
	}

	~ViewPanelTextWidget()
	{
		delete horoscope;
	}

	virtual int write()
	{
		wxString s;
		Lang lang( cfg );

		horoscope->setActualDate();
		horoscope->update();
		vector<int> obs = PlanetList().getVedicObjectList( OBJECTS_INCLUDE_OUTER );
		Table table( 2, 10 );

		int line = 0;
		for ( unsigned int i = 0; i < obs.size(); i++ )
		{
			if ( obs[i] == OJUPITER ) continue;
			if ( obs[i] == OSATURN ) continue;
			if ( obs[i] == ONEPTUNE ) continue;
			table.setEntry( 0, line, writer->getObjectName( obs[i], TLARGE, true ));
			table.setEntry( 1, line++, writer->getPosFormatted( horoscope->getVedicLength( obs[i] ), horoscope->isRetrograde( obs[i] ), DEG_PRECISION_SECOND ));
		}
		for ( unsigned int i = 8; i < 11; i++ )
		{
			if ( cfg->useVedicPlanetNames ) s = lang.getBhavaName( i );
			else s.Printf( wxT( "%02d" ), i+1 );
			table.setEntry( 0, line, s );
			table.setEntry( 1, line++, writer->getPosFormatted( horoscope->getHouse( i, true ), false, DEG_PRECISION_SECOND ));
		}
		writer->writeTable( table );
		return 0;
	}

private:
	Horoscope *horoscope;
	Config *cfg;
};

IMPLEMENT_CLASS( ViewPanelTextWidget, TextWidget )

/*****************************************************
**
**   ViewPanel   ---   Constructor
**
******************************************************/
ViewPanel::ViewPanel( wxWindow* parent )
		: ConfigPanel( parent )
{
	cfg = new Config();

    // begin wxGlade: ViewPanel::ViewPanel
    panel_preview = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL);
    sizer_special_symbols_staticbox = new wxStaticBox(this, -1, _("Special Symbols"));
    sizer_preview_staticbox = new wxStaticBox(this, -1, _("Preview"));
    sizer_signs_staticbox = new wxStaticBox(this, -1, _("Presentation of Signs and Planets"));
    label_planet_display = new wxStaticText(this, wxID_ANY, _("Display of Planets"));
    const wxString choice_planet_display_choices[] = {
        _("Names"),
        _("Symbols")
    };
    choice_planet_display = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_planet_display_choices, 0);
    label_planet_names = new wxStaticText(this, wxID_ANY, _("Planetary Names"));
    const wxString choice_planet_names_choices[] = {
        _("Western"),
        _("Vedic")
    };
    choice_planet_names = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_planet_names_choices, 0);
    label_positions = new wxStaticText(this, wxID_ANY, _("Format of Positions"));
    const wxString choice_planet_positions_choices[] = {
        _("Western"),
        _("Vedic")
    };
    choice_planet_positions = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_planet_positions_choices, 0);
    label_sign_display = new wxStaticText(this, wxID_ANY, _("Display of Signs"));
    const wxString choice_sign_display_choices[] = {
        _("Names"),
        _("Symbols")
    };
    choice_sign_display = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_sign_display_choices, 0);
    label_sign_names = new wxStaticText(this, wxID_ANY, _("Sign Names"));
    const wxString choice_sign_names_choices[] = {
        _("Western"),
        _("Vedic")
    };
    choice_sign_names = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_sign_names_choices, 0);
    label_uranus = new wxStaticText(this, wxID_ANY, _("Uranus"));
    const wxString choice_uranus_choices[] = {
        _("Symbol 1"),
        _("Symbol 2")
    };
    choice_uranus = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_uranus_choices, 0);
    label_pluto = new wxStaticText(this, wxID_ANY, _("Pluto"));
    const wxString choice_pluto_choices[] = {
        _("Symbol 1"),
        _("Symbol 2")
    };
    choice_pluto = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_pluto_choices, 0);
    label_capricorn = new wxStaticText(this, wxID_ANY, _("Capricorn"));
    const wxString choice_capricorn_choices[] = {
        _("Symbol 1"),
        _("Symbol 2")
    };
    choice_capricorn = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_capricorn_choices, 0);
    theText = new ViewPanelTextWidget(panel_preview, 0, cfg);

    set_properties();
    do_layout();
    // end wxGlade

	Connect( wxID_ANY, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ViewPanel::OnChoice ));
}

/*****************************************************
**
**   ViewPanel   ---   Destructor
**
******************************************************/
ViewPanel::~ViewPanel()
{
	delete cfg;
}

/*****************************************************
**
**   ViewPanel   ---   setData
**
******************************************************/
void ViewPanel::setData()
{
	cfg->useSignSymbols = config->useSignSymbols;
	cfg->useVedicSignNames = config->useVedicSignNames;
	cfg->usePlanetSymbols = config->usePlanetSymbols;
	cfg->useVedicPlanetNames = config->useVedicPlanetNames;
	cfg->useVedicPositions = config->useVedicPositions;

	cfg->uranusSymbol = config->uranusSymbol;
	cfg->plutoSymbol = config->plutoSymbol;
	cfg->capricornSymbol = config->capricornSymbol;

	choice_sign_display->SetSelection( cfg->useSignSymbols );
	choice_sign_names->SetSelection( cfg->useVedicSignNames );
	choice_planet_display->SetSelection( cfg->usePlanetSymbols );
	choice_planet_names->SetSelection( cfg->useVedicPlanetNames );
	choice_planet_positions->SetSelection( cfg->useVedicPositions );

	choice_sign_names->Enable( cfg->useSignSymbols == 0 );

	choice_uranus->SetSelection( cfg->uranusSymbol );
	choice_pluto->SetSelection( cfg->plutoSymbol );
	choice_capricorn->SetSelection( cfg->capricornSymbol );

	theText->writeContents();
}

/*****************************************************
**
**   ViewPanel   ---   saveData
**
******************************************************/
bool ViewPanel::saveData()
{
	config->useSignSymbols = cfg->useSignSymbols;
	config->usePlanetSymbols = cfg->usePlanetSymbols;
	config->useVedicPositions = cfg->useVedicPositions;
	config->useVedicPlanetNames = cfg->useVedicPlanetNames;
	config->useVedicSignNames = cfg->useVedicSignNames;
	config->useVedicPlanetNames = cfg->useVedicPlanetNames;

	config->uranusSymbol = cfg->uranusSymbol;
	config->plutoSymbol = cfg->plutoSymbol;
	config->capricornSymbol = cfg->capricornSymbol;

	return true;
}

/*****************************************************
**
**   ViewPanel   ---   set_properties
**
******************************************************/
void ViewPanel::set_properties()
{
    // begin wxGlade: ViewPanel::set_properties
    choice_planet_display->SetSelection(0);
    choice_planet_names->SetSelection(0);
    choice_planet_positions->SetSelection(0);
    choice_sign_display->SetSelection(0);
    choice_sign_names->SetSelection(0);
    label_uranus->SetMinSize(wxSize(80, -1));
    choice_uranus->SetSelection(0);
    choice_pluto->SetSelection(0);
    choice_capricorn->SetSelection(0);
    theText->SetMinSize(wxSize(200, 220));
    // end wxGlade
}

/*****************************************************
**
**   ViewPanel   ---   do_layout
**
******************************************************/
void ViewPanel::do_layout()
{
    // begin wxGlade: ViewPanel::do_layout
    wxFlexGridSizer* sizer_main = new wxFlexGridSizer(1, 2, 0, 0);
    wxFlexGridSizer* sizer_right = new wxFlexGridSizer(1, 1, 0, 0);
    wxStaticBoxSizer* sizer_preview = new wxStaticBoxSizer(sizer_preview_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_previewpanel = new wxFlexGridSizer(1, 1, 3, 3);
    wxFlexGridSizer* sizer_left = new wxFlexGridSizer(2, 1, 0, 0);
    wxStaticBoxSizer* sizer_special_symbols = new wxStaticBoxSizer(sizer_special_symbols_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_special_symbols = new wxFlexGridSizer(3, 2, 3, 3);
    wxStaticBoxSizer* sizer_signs = new wxStaticBoxSizer(sizer_signs_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_sizer_presentation = new wxFlexGridSizer(5, 2, 3, 3);
    grid_sizer_presentation->Add(label_planet_display, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_presentation->Add(choice_planet_display, 0, wxALL|wxEXPAND, 3);
    grid_sizer_presentation->Add(label_planet_names, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_presentation->Add(choice_planet_names, 0, wxALL|wxEXPAND, 3);
    grid_sizer_presentation->Add(label_positions, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_presentation->Add(choice_planet_positions, 0, wxALL|wxEXPAND, 3);
    grid_sizer_presentation->Add(label_sign_display, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_presentation->Add(choice_sign_display, 0, wxALL|wxEXPAND, 3);
    grid_sizer_presentation->Add(label_sign_names, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_presentation->Add(choice_sign_names, 0, wxALL|wxEXPAND, 3);
    grid_sizer_presentation->AddGrowableCol(1);
    sizer_signs->Add(grid_sizer_presentation, 1, wxALL, 3);
    sizer_left->Add(sizer_signs, 1, wxALL|wxEXPAND, 3);
    grid_special_symbols->Add(label_uranus, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_special_symbols->Add(choice_uranus, 0, wxALL|wxEXPAND, 3);
    grid_special_symbols->Add(label_pluto, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_special_symbols->Add(choice_pluto, 0, wxALL|wxEXPAND, 3);
    grid_special_symbols->Add(label_capricorn, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_special_symbols->Add(choice_capricorn, 0, wxALL|wxEXPAND, 3);
    grid_special_symbols->AddGrowableCol(1);
    sizer_special_symbols->Add(grid_special_symbols, 1, wxALL|wxEXPAND, 3);
    sizer_left->Add(sizer_special_symbols, 1, wxALL|wxEXPAND, 3);
    sizer_left->AddGrowableCol(0);
    sizer_main->Add(sizer_left, 1, wxALL, 3);
    grid_previewpanel->Add(theText, 1, wxALL|wxEXPAND, 3);
    panel_preview->SetSizer(grid_previewpanel);
    grid_previewpanel->AddGrowableRow(0);
    grid_previewpanel->AddGrowableCol(0);
    sizer_preview->Add(panel_preview, 1, wxALL|wxEXPAND, 3);
    sizer_right->Add(sizer_preview, 1, wxALL|wxEXPAND, 3);
    sizer_right->AddGrowableRow(0);
    sizer_right->AddGrowableCol(0);
    sizer_main->Add(sizer_right, 1, wxALL|wxEXPAND, 3);
    SetSizer(sizer_main);
    sizer_main->Fit(this);
    sizer_main->AddGrowableRow(0);
    sizer_main->AddGrowableCol(1);
    // end wxGlade
}

/*****************************************************
**
**   ViewPanel   ---   OnChoice
**
******************************************************/
void ViewPanel::OnChoice( wxCommandEvent& )
{
	cfg->useSignSymbols = choice_sign_display->GetSelection();
	cfg->usePlanetSymbols = choice_planet_display->GetSelection();
	cfg->useVedicPositions = choice_planet_positions->GetSelection();
	cfg->useVedicSignNames = choice_sign_names->GetSelection();
	cfg->useVedicPlanetNames = choice_planet_names->GetSelection();

	cfg->uranusSymbol = choice_uranus->GetSelection();
	cfg->plutoSymbol = choice_pluto->GetSelection();
	cfg->capricornSymbol = choice_capricorn->GetSelection();

	choice_sign_names->Enable( choice_sign_display->GetSelection() == 0 );
	theText->writeContents();
}

/*****************************************************
**
**   ConfigPanelFactory   ---   createViewPanel
**
******************************************************/
ConfigPanel *ConfigPanelFactory:: createViewPanel( wxWindow *parent )
{
	return new ViewPanel( parent );
}

