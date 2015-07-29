/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/WesternChartPanel.cpp
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
#pragma implementation "WesternChartPanel.h"
#endif

#include "WesternChartPanel.h"

#include "ChartProperties.h"
#include "Conf.h"
#include "Horoscope.h"
#include "Painter.h"
#include "WesternChart.h"
#include "WesternChartConfig.h"

#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/dc.h>
#include <wx/dcclient.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/statbox.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

extern Config *config;

enum { CD_SKIN_SELECT = wxID_HIGHEST + 1, CD_ASCENDANT, CD_SHOW_ASPECTS, CD_SHOW_RETRO, CD_SHOW_HOUSES, CD_USE_ASPECT_COLORS,
	CD_USE_SIGN_COLORS, CD_USE_PLANET_COLORS, CD_USE_HOUSE_COLORS, CD_SHOW_ASPECT_SYMBOLS, CD_SHOW_TRANSIT_PREVIEW, CD_TRANSIT_STYLE };

IMPLEMENT_CLASS( WesternChartPanel, ConfigPanel )

/*************************************************//**
*
* \brief preview panel for western chart
*
******************************************************/
class WesternChartPreviewPanel: public wxPanel
{
	DECLARE_CLASS( WesternChartPreviewPanel )

public:
	/*****************************************************
	**
	**   WesternChartPreviewPanel   ---   Constructor
	**
	******************************************************/
	WesternChartPreviewPanel( wxWindow* parent, ChartProperties *props, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxFULL_REPAINT_ON_RESIZE )
			: wxPanel( parent, -1, pos, size, style )
	{
		chartprops = props;
		horoscope = new Horoscope();
		horoscope->update();

		htransit = new Horoscope();
		htransit->setDate( JDate::getCurrentJD() + 36000 );
		htransit->update();

		chart = new WesternChart( horoscope, horoscope, chartprops );
		chart->OnDataChanged();

		Connect( wxEVT_PAINT, wxPaintEventHandler( WesternChartPreviewPanel::OnPaint ));
	}

	/*****************************************************
	**
	**   WesternChartPreviewPanel   ---   Destructor
	**
	******************************************************/
	~WesternChartPreviewPanel()
	{
		delete chart;
		delete horoscope;
		delete htransit;
	}

	void setTransitMode( const bool &b ) { chart->setTransitChart( b ? htransit : horoscope ); }

private:

	/*****************************************************
	**
	**   WesternChartPreviewPanel   ---   OnPaint
	**
	******************************************************/
	void OnPaint( wxPaintEvent &event )
	{
		wxCoord h, w;
		wxPaintDC context( this );
		PrepareDC( context );

		GetSize( &w, &h );
		horoscope->update();
		DcPainter painter( &context );

		chart->paint( &painter, MyRect( 0, 0, w, h ));
	}

	Horoscope *horoscope;
	Horoscope *htransit;
	WesternChart *chart;
	ChartProperties *chartprops;

};

IMPLEMENT_CLASS( WesternChartPreviewPanel, wxPanel )

/*****************************************************
**
**   WesternChartPanel   ---   Constructor
**
******************************************************/
WesternChartPanel::WesternChartPanel( wxWindow* parent )
		: ConfigPanel( parent )
{
	chartprops = new ChartProperties;
	chartprops->setVedic( false );

    // begin wxGlade: WesternChartPanel::WesternChartPanel
    panel_preview = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL);
    sizer_chartoptions_staticbox = new wxStaticBox(this, -1, _("Chart Options"));
    sizer_show_staticbox = new wxStaticBox(this, -1, _("Show in Chart"));
    sizer_colors_staticbox = new wxStaticBox(this, -1, _("Foreground Colors"));
    sizer_preview_staticbox = new wxStaticBox(this, -1, _("Preview"));
    sizer_defaultview_staticbox = new wxStaticBox(this, -1, _("Default View"));
    const wxString choice_defaultskin_choices[] = {
        _("dummy"),
        _("dummy"),
        _("dummy"),
        _("dummy"),
        _("dummy")
    };
    choice_defaultskin = new wxChoice(this, CD_SKIN_SELECT, wxDefaultPosition, wxDefaultSize, 5, choice_defaultskin_choices, 0);
    label_leftside = new wxStaticText(this, wxID_ANY, _("Left Side"));
    const wxString choice_leftside_choices[] = {
        _("Ascendant"),
        _("Aries 0"),
        _("Aries 15"),
        _("Cancer 0"),
        _("Cancer15"),
        _("Libra 0"),
        _("Libra 15"),
        _("Capricorn 0"),
        _("Capricorn 15")
    };
    choice_leftside = new wxChoice(this, CD_ASCENDANT, wxDefaultPosition, wxDefaultSize, 9, choice_leftside_choices, 0);
    label_transitstyle = new wxStaticText(this, wxID_ANY, _("Transits"));
    const wxString choice_transitstyle_choices[] = {
        _("Outsite"),
        _("Inside")
    };
    choice_transitstyle = new wxChoice(this, CD_TRANSIT_STYLE, wxDefaultPosition, wxDefaultSize, 2, choice_transitstyle_choices, 0);
    check_retro = new wxCheckBox(this, CD_SHOW_RETRO, _("Retrogression (R)"));
    check_aspects = new wxCheckBox(this, CD_SHOW_ASPECTS, _("Aspects"));
    check_aspectsymbols = new wxCheckBox(this, CD_SHOW_ASPECT_SYMBOLS, _("Aspect Symbols"));
    check_houses = new wxCheckBox(this, CD_SHOW_HOUSES, _("Houses"));
    check_use_planet_colors = new wxCheckBox(this, CD_USE_PLANET_COLORS, _("Planets"));
    check_use_sign_colors = new wxCheckBox(this, CD_USE_SIGN_COLORS, _("Signs"));
    check_use_house_colors = new wxCheckBox(this, CD_USE_HOUSE_COLORS, _("Houses"));
    check_use_aspect_colors = new wxCheckBox(this, CD_USE_ASPECT_COLORS, _("Aspects"));
    window_preview = new WesternChartPreviewPanel(panel_preview, chartprops);
    check_transit = new wxCheckBox(this, CD_SHOW_TRANSIT_PREVIEW, _("Transit Preview"));

    set_properties();
    do_layout();
    // end wxGlade

	Connect( wxID_ANY, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( WesternChartPanel::OnCommand ));
	Connect( wxID_ANY, wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( WesternChartPanel::OnCommand ));
}

/*****************************************************
**
**   WesternChartPanel   ---   Destructor
**
******************************************************/
WesternChartPanel::~WesternChartPanel()
{
	config->configDialogWChartPreviewTransits = check_transit->GetValue();
	delete chartprops;
}

/*****************************************************
**
**   WesternChartPanel   ---   setData
**
******************************************************/
void WesternChartPanel::setData()
{
	choice_defaultskin->Clear();
	vector<BaseXmlConfig*> v = WesternChartConfigLoader::get()->getConfigs();
	for ( unsigned int i = 0; i < v.size(); i++ ) choice_defaultskin->Append( v[i]->name );
	choice_defaultskin->SetSelection( chartprops->getWesternSkin() );

	check_aspects->SetValue( config->wGraphicStyle.showAspects );
	check_aspectsymbols->SetValue( chartprops->getWesternGraphicStyle().showAspectSymbols );
	check_retro->SetValue( config->wGraphicStyle.showRetro );
	check_houses->SetValue( config->wGraphicStyle.showHouses );

	check_use_sign_colors->SetValue( chartprops->getWesternGraphicStyle().showSignColors );
	check_use_planet_colors->SetValue( chartprops->getWesternGraphicStyle().showPlanetColors );
	check_use_house_colors->SetValue( chartprops->getWesternGraphicStyle().showHouseColors );
	check_use_aspect_colors->SetValue( chartprops->getWesternGraphicStyle().showAspectColors );

	choice_leftside->SetSelection( config->wChartOrientation );
	choice_transitstyle->SetSelection( chartprops->getWesternGraphicStyle().transitStyle );
	check_transit->SetValue( config->configDialogWChartPreviewTransits );

	updateGuiElements();
}

/*****************************************************
**
**   WesternChartPanel   ---   calculateStyle
**
******************************************************/
WesternGraphicStyle WesternChartPanel::calculateStyle()
{
	WesternGraphicStyle style;
	style.showAspects = check_aspects->GetValue();
	style.showAspectSymbols = check_aspectsymbols->GetValue();
	style.showRetro = check_retro->GetValue();
	style.showHouses = check_houses->GetValue();

	style.showSignColors = check_use_sign_colors->GetValue();
	style.showPlanetColors = check_use_planet_colors->GetValue();
	style.showHouseColors = check_use_house_colors->GetValue();
	style.showAspectColors = check_use_aspect_colors->GetValue();

	style.transitStyle = choice_transitstyle->GetSelection();

	return style;
}

/*****************************************************
**
**   WesternChartPanel   ---   saveData
**
******************************************************/
bool WesternChartPanel::saveData()
{
	config->wGraphicSkin = choice_defaultskin->GetSelection();
	config->wGraphicStyle = calculateStyle();
	config->wChartOrientation = choice_leftside->GetSelection();
	return true;
}

/*****************************************************
**
**   WesternChartPanel   ---   OnCommand
**
******************************************************/
void WesternChartPanel::OnCommand( wxCommandEvent& )
{
	chartprops->setWesternSkin( choice_defaultskin->GetSelection());
	chartprops->setWesternGraphicStyle( calculateStyle());
	chartprops->setWesternChartOrientation( choice_leftside->GetSelection());

	updateGuiElements();
}

/*****************************************************
**
**   WesternChartPanel   ---   updateGuiElements
**
******************************************************/
void WesternChartPanel::updateGuiElements()
{
	check_use_aspect_colors->Enable( chartprops->getWesternGraphicStyle().showAspects );
	check_aspectsymbols->Enable( chartprops->getWesternGraphicStyle().showAspects );
	check_use_house_colors->Enable( chartprops->getWesternGraphicStyle().showHouses );

	window_preview->setTransitMode( check_transit->GetValue());
	window_preview->Refresh();
}

/*****************************************************
**
**   WesternChartPanel   ---   set_properties
**
******************************************************/
void WesternChartPanel::set_properties()
{
    // begin wxGlade: WesternChartPanel::set_properties
    choice_defaultskin->SetSelection(0);
    choice_leftside->SetSelection(0);
    choice_transitstyle->SetSelection(0);
    panel_preview->SetMinSize(wxSize(100, 100));
    // end wxGlade
}

/*****************************************************
**
**   WesternChartPanel   ---   do_layout
**
******************************************************/
void WesternChartPanel::do_layout()
{
    // begin wxGlade: WesternChartPanel::do_layout
    wxFlexGridSizer* sizer_main = new wxFlexGridSizer(1, 2, 0, 0);
    wxFlexGridSizer* sizer_right = new wxFlexGridSizer(1, 1, 0, 0);
    wxStaticBoxSizer* sizer_preview = new wxStaticBoxSizer(sizer_preview_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_previewpanel = new wxFlexGridSizer(1, 1, 3, 3);
    wxFlexGridSizer* sizer_left = new wxFlexGridSizer(3, 1, 0, 0);
    wxStaticBoxSizer* sizer_colors = new wxStaticBoxSizer(sizer_colors_staticbox, wxVERTICAL);
    wxStaticBoxSizer* sizer_show = new wxStaticBoxSizer(sizer_show_staticbox, wxVERTICAL);
    wxStaticBoxSizer* sizer_chartoptions = new wxStaticBoxSizer(sizer_chartoptions_staticbox, wxVERTICAL);
    wxGridSizer* grid_options = new wxGridSizer(2, 2, 0, 0);
    wxStaticBoxSizer* sizer_defaultview = new wxStaticBoxSizer(sizer_defaultview_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_defaultview = new wxFlexGridSizer(4, 1, 3, 3);
    grid_defaultview->Add(choice_defaultskin, 0, wxALL|wxEXPAND, 3);
    grid_defaultview->AddGrowableCol(0);
    sizer_defaultview->Add(grid_defaultview, 1, wxEXPAND, 0);
    sizer_left->Add(sizer_defaultview, 0, wxALL|wxEXPAND, 3);
    grid_options->Add(label_leftside, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_options->Add(choice_leftside, 0, wxALL|wxEXPAND, 3);
    grid_options->Add(label_transitstyle, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_options->Add(choice_transitstyle, 0, wxALL|wxEXPAND, 3);
    sizer_chartoptions->Add(grid_options, 1, 0, 0);
    sizer_left->Add(sizer_chartoptions, 0, wxALL|wxEXPAND, 3);
    sizer_show->Add(check_retro, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    sizer_show->Add(check_aspects, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    sizer_show->Add(check_aspectsymbols, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    sizer_show->Add(check_houses, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    sizer_left->Add(sizer_show, 1, wxEXPAND, 0);
    sizer_colors->Add(check_use_planet_colors, 0, wxALL|wxEXPAND, 3);
    sizer_colors->Add(check_use_sign_colors, 0, wxALL|wxEXPAND, 3);
    sizer_colors->Add(check_use_house_colors, 0, wxALL|wxEXPAND, 3);
    sizer_colors->Add(check_use_aspect_colors, 0, wxALL|wxEXPAND, 3);
    sizer_left->Add(sizer_colors, 1, wxALL|wxEXPAND, 3);
    sizer_left->AddGrowableCol(0);
    sizer_main->Add(sizer_left, 1, wxALL, 3);
    grid_previewpanel->Add(window_preview, 1, wxALL|wxEXPAND, 3);
    panel_preview->SetSizer(grid_previewpanel);
    grid_previewpanel->AddGrowableRow(0);
    grid_previewpanel->AddGrowableCol(0);
    sizer_preview->Add(panel_preview, 1, wxALL|wxEXPAND, 3);
    sizer_preview->Add(check_transit, 0, wxALL, 3);
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
**   ConfigPanelFactory   ---   createWesternChartPanel
**
******************************************************/
ConfigPanel *ConfigPanelFactory::createWesternChartPanel( wxWindow *parent )
{
	return new WesternChartPanel( parent );
}

