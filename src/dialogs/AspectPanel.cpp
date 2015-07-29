/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/AspectPanel.cpp
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
#pragma implementation "AspectPanel.h"
#endif

#include "AspectPanel.h"

#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/statbox.h>
#include <wx/stattext.h>

#include "ColorConfig.h"
#include "Conf.h"
#include "mathbase.h"
#include "Painter.h"

extern Config *config;

enum { CD_ASPECTLIST = wxID_HIGHEST + 1, CD_DEFAULT,
       CD_CHECK_ASPECT,  // * MAX_ASPECT_TYPES
       CD_SPIN_ORBIS = CD_CHECK_ASPECT + MAX_ASPECT_TYPES + 1
     };

IMPLEMENT_CLASS( AspectPanel, ConfigPanel )
IMPLEMENT_CLASS( AspectOrbisPanel, wxPanel )

/*****************************************************
**
**   AspectOrbisPanel   ---   Constructor
**
******************************************************/
AspectOrbisPanel::AspectOrbisPanel(wxWindow* parent, int id, const wxPoint& pos, const wxSize& size, long style)
		: wxPanel(parent, id, pos, size, wxTAB_TRAVERSAL )
{
	const int header_size = 10;
	wxStaticText *header_1a = new wxStaticText(this, wxID_ANY, _("Aspect"));
	header_1a->SetFont(wxFont(header_size, wxDEFAULT, wxNORMAL, wxBOLD, 0, wxT("")));
	wxStaticText *header_1b = new wxStaticText(this, wxID_ANY, _("Orbis"));
	header_1b->SetFont(wxFont(header_size, wxDEFAULT, wxNORMAL, wxBOLD, 0, wxT("")));

	wxStaticText *header_2a = new wxStaticText(this, wxID_ANY, _("Aspect"));
	header_2a->SetFont(wxFont(header_size, wxDEFAULT, wxNORMAL, wxBOLD, 0, wxT("")));
	wxStaticText *header_2b = new wxStaticText(this, wxID_ANY, _("Orbis"));
	header_2b->SetFont(wxFont(header_size, wxDEFAULT, wxNORMAL, wxBOLD, 0, wxT("")));

	for ( int i = 0; i < MAX_ASPECT_TYPES; i++ )
	{
		check_aspect[i] = new wxCheckBox( this, CD_CHECK_ASPECT + i, AspectExpert::getAspectDescription( i ));
		spin_orbis[i] = new wxSpinCtrl( this, CD_SPIN_ORBIS + i, _( "Orbis" ),
				wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 20);
		spin_orbis[i]->SetMinSize(wxSize(60, -1));
	}
	wxBoxSizer* sizer_lr = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* sizer_tb = new wxBoxSizer(wxVERTICAL);
	wxFlexGridSizer* grid_l = new wxFlexGridSizer( 11, 2, 0, 0 );
	wxFlexGridSizer* grid_r = new wxFlexGridSizer( 10, 2, 0, 0 );

	grid_l->Add(header_1a, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	grid_l->Add(header_1b, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	grid_r->Add(header_2a, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	grid_r->Add(header_2b, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
	for ( int i = 0; i < MAX_ASPECT_TYPES; i++ )
	{
		if ( i < 10 )
		{
			grid_l->Add( check_aspect[i], 0, wxALL|wxALIGN_CENTER_VERTICAL, 3 );
			grid_l->Add( spin_orbis[i], 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
		}
		else
		{
			grid_r->Add( check_aspect[i], 0, wxALL|wxALIGN_CENTER_VERTICAL, 3 );
			grid_r->Add( spin_orbis[i], 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
		}
	}
	sizer_lr->Add(grid_l, 1, wxALL, 3);
	sizer_lr->Add(grid_r, 1, wxALL, 3);
	sizer_tb->Add(sizer_lr, 1, wxALL, 3);
	SetSizer( sizer_tb);
	sizer_tb->Fit(this);
}

/*****************************************************
**
**   AspectOrbisPanel   ---   getOrbis
**
******************************************************/
int AspectOrbisPanel::getOrbis( const int &item )
{
	ASSERT_VALID_ASPECT( item );
	return spin_orbis[item]->GetValue();
}

/*****************************************************
**
**   AspectOrbisPanel   ---   setOrbis
**
******************************************************/
void AspectOrbisPanel::setOrbis( const int &item, const int &value )
{
	ASSERT_VALID_ASPECT( item );
	spin_orbis[item]->SetValue( value );
}

/*****************************************************
**
**   AspectOrbisPanel   ---   isActive
**
******************************************************/
bool AspectOrbisPanel::isActive( const int &item )
{
	return check_aspect[item]->IsChecked();
}

/*****************************************************
**
**   AspectOrbisPanel   ---   setActive
**
******************************************************/
void AspectOrbisPanel::setActive( const int &item, const bool value )
{
	check_aspect[item]->SetValue( value );
	spin_orbis[item]->Enable( value );
}

/*****************************************************
**
**   AspectPanel   ---   Constructor
**
******************************************************/
AspectPanel::AspectPanel( wxWindow* parent )
		:  ConfigPanel( parent )
{
    // begin wxGlade: AspectPanel::AspectPanel
    sizer_aspects_staticbox = new wxStaticBox(this, -1, _("Aspects and Orbes"));
    aspect_list = new AspectOrbisPanel(this, CD_ASPECTLIST);
    button_default_copy = new wxButton(this, CD_DEFAULT, _("Restore Defaults"));

    set_properties();
    do_layout();
    // end wxGlade

	Connect( CD_DEFAULT, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AspectPanel::OnDefaultButton ));
	Connect( CD_CHECK_ASPECT, CD_CHECK_ASPECT + MAX_ASPECT_TYPES, wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AspectPanel::OnCheckAspect ));
	Connect( CD_SPIN_ORBIS, CD_SPIN_ORBIS + MAX_ASPECT_TYPES, wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( AspectPanel::OnSpinOrbis ));
}

/*****************************************************
**
**   AspectPanel   ---   setData
**
******************************************************/
void AspectPanel::setData()
{
	AspectExpert aexpert;
	AspectConfigLoader *loader = AspectConfigLoader::get();

	for ( int i = 0; i < MAX_ASPECT_TYPES; i++ )
	{
		configs[i] = *loader->getAspectConfig( i );
	}
	updateControls();
}

/*****************************************************
**
**   AspectPanel   ---   updateControls
**
******************************************************/
void AspectPanel::updateControls()
{
	for ( int i = 0; i < MAX_ASPECT_TYPES; i++ )
	{
		aspect_list->setOrbis( i, (int)configs[i].orbis );
		aspect_list->setActive( i, configs[i].active );
	}
}

/*****************************************************
**
**   AspectPanel   ---   saveData
**
******************************************************/
bool AspectPanel::saveData()
{
	AspectConfigLoader *loader = AspectConfigLoader::get();
	for ( int i = 0; i < MAX_ASPECT_TYPES; i++ )
	{
		loader->setAspectConfig( configs[i], i );
	}
	return true;
}

/*****************************************************
**
**   AspectPanel   ---   OnSpinOrbis
**
******************************************************/
void AspectPanel::OnSpinOrbis( wxSpinEvent &event )
{
	int id = event.GetId() - CD_SPIN_ORBIS;
	ASSERT_VALID_ASPECT( id );
	configs[id].orbis = event.GetPosition();
}

/*****************************************************
**
**   AspectPanel   ---   OnCheckAspect
**
******************************************************/
void AspectPanel::OnCheckAspect( wxCommandEvent &event )
{
	int id = event.GetId() - CD_CHECK_ASPECT;
	ASSERT_VALID_ASPECT( id );
	configs[id].active = event.IsChecked();
	aspect_list->setActive(id, event.IsChecked());
}

/*****************************************************
**
**   AspectPanel   ---   OnDefaultButton
**
******************************************************/
void AspectPanel::OnDefaultButton( wxCommandEvent& )
{
	AspectConfigLoader *loader = AspectConfigLoader::get();
	for ( int i = 0; i < MAX_ASPECT_TYPES; i++ )
	{
		configs[i] = *loader->getDefaultAspectConfig( i );
	}
	updateControls();
}

/*****************************************************
**
**   AspectPanel   ---   set_properties
**
******************************************************/
void AspectPanel::set_properties()
{
    // begin wxGlade: AspectPanel::set_properties
    // end wxGlade
}

/*****************************************************
**
**   AspectPanel   ---   do_layout
**
******************************************************/
void AspectPanel::do_layout()
{
    // begin wxGlade: AspectPanel::do_layout
    wxBoxSizer* sizer_main = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_left = new wxBoxSizer(wxVERTICAL);
    wxStaticBoxSizer* sizer_aspects = new wxStaticBoxSizer(sizer_aspects_staticbox, wxHORIZONTAL);
    sizer_aspects->Add(aspect_list, 0, wxALL, 3);
    sizer_left->Add(sizer_aspects, 0, wxALL, 3);
    sizer_left->Add(button_default_copy, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 3);
    sizer_main->Add(sizer_left, 1, 0, 3);
    SetSizer(sizer_main);
    sizer_main->Fit(this);
    // end wxGlade
}

/*****************************************************
**
**   ConfigPanelFactory   ---   createAspectPanel
**
******************************************************/
ConfigPanel *ConfigPanelFactory::createAspectPanel( wxWindow *parent )
{
	return new AspectPanel( parent );
}

