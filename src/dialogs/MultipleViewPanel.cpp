/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/MultipleViewPanel.cpp
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
#pragma implementation "MultipleViewPanel.h"
#endif

#include "MultipleViewPanel.h"

#include "Conf.h"
#include "FileConfig.h"
#include "MultipleViewConfig.h"

#include <wx/choice.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

extern Config *config;
enum { CD_SWITCH_MVIEW = wxID_HIGHEST + 1, CD_CHOICE_DEFAULTVIEW };

IMPLEMENT_CLASS( MultipleViewPanel, ConfigPanel )

/*****************************************************
**
**   MultipleViewPanel   ---   Constructor
**
******************************************************/
MultipleViewPanel::MultipleViewPanel( wxWindow* parent )
		: ConfigPanel( parent )
{
    // begin wxGlade: MultipleViewPanel::MultipleViewPanel
    panel_description = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL);
    sizer_mview_staticbox = new wxStaticBox(this, -1, _("Multiple View Selection"));
    sizer_nbstyle_staticbox = new wxStaticBox(this, -1, _("Notebooks in Multiple View"));
    sizer_usemview_staticbox = new wxStaticBox(this, -1, _("Default View Type"));
    label_usemview = new wxStaticText(this, wxID_ANY, _("Type"));
    const wxString choice_usemview_choices[] = {
        _("Single"),
        _("Multiple")
    };
    choice_usemview = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_usemview_choices, 0);
    label_defview = new wxStaticText(this, wxID_ANY, _("View"));
    const wxString choice_viewlist_choices[] = {
        _("dummy"),
        _("dummy"),
        _("dummy"),
        _("dummy")
    };
    choice_viewlist = new wxChoice(this, CD_CHOICE_DEFAULTVIEW, wxDefaultPosition, wxDefaultSize, 4, choice_viewlist_choices, 0);
    text_view_description = new wxStaticText(panel_description, wxID_ANY, _("dummy"));
    label_nbstyle = new wxStaticText(this, wxID_ANY, _("Style"));
    const wxString choice_nbstyle_choices[] = {
        _("Notebook"),
        _("Listbook"),
        _("Choicebook"),
        _("Toolbook"),
        _("Treebook")
    };
    choice_nbstyle = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 5, choice_nbstyle_choices, 0);
    label_orientation = new wxStaticText(this, wxID_ANY, _("Orientation"));
    const wxString choice_orientation_choices[] = {
        _("Top"),
        _("Bottom"),
        _("Left"),
        _("Right")
    };
    choice_orientation = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 4, choice_orientation_choices, 0);

    set_properties();
    do_layout();
    // end wxGlade
	text_view_description->SetBackgroundColour( GetBackgroundColour() );
	text_view_description->SetForegroundColour( GetForegroundColour() );

	// TODO sel setzen
	choice_viewlist->Clear();
	vector<BaseXmlConfig*> v = MultipleViewConfigLoader::get()->getConfigs();
	unsigned int sel = config->defaultMultipleView;
	if ( config->defaultMultipleView < 0 || (int)v.size() < config->defaultMultipleView ) sel = 0;
	for ( unsigned int i = 0; i < v.size(); i++ )
	{
		choice_viewlist->Append( v[i]->name );
	}
	choice_viewlist->SetSelection( sel );

	choice_nbstyle->Clear();
#if wxUSE_NOTEBOOK
	choice_nbstyle->Append( _( "Notebook" ));
#endif
#if wxUSE_LISTBOOK
	choice_nbstyle->Append( _( "Listbook" ));
#endif
#if wxUSE_CHOICEBOOK
	choice_nbstyle->Append( _( "Choicebook" ));
#endif
#if wxUSE_TOOLBOOK
	choice_nbstyle->Append( _( "Toolbook" ));
#endif
#if wxUSE_TREEBOOK
	choice_nbstyle->Append( _( "Treebook" ));
#endif

	Connect( CD_CHOICE_DEFAULTVIEW, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( MultipleViewPanel::OnMViewChoice ));
}

/*****************************************************
**
**   MultipleViewPanel   ---   setData
**
******************************************************/
void MultipleViewPanel::setData()
{
	choice_usemview->SetSelection( config->useMultipleViews );
	choice_nbstyle->SetSelection( config->mViewNotebookStyle );
	choice_orientation->SetSelection( config->mViewNotebookOrientation );
	updateMViewDescription();
}

/*****************************************************
**
**   MultipleViewPanel   ---   saveData
**
******************************************************/
bool MultipleViewPanel::saveData()
{
	config->useMultipleViews =   choice_usemview->GetSelection();
	config->defaultMultipleView = choice_viewlist->GetSelection();
	config->mViewNotebookStyle =	choice_nbstyle->GetSelection();
	config->mViewNotebookOrientation =	choice_orientation->GetSelection();
	return true;
}

/*****************************************************
**
**   MultipleViewPanel   ---   set_properties
**
******************************************************/
void MultipleViewPanel::set_properties()
{
    // begin wxGlade: MultipleViewPanel::set_properties
    choice_usemview->SetSelection(0);
    choice_viewlist->SetSelection(0);
    text_view_description->SetMinSize(wxSize(150, 100));
    choice_nbstyle->SetSelection(0);
    choice_orientation->SetSelection(0);
    // end wxGlade
}

/*****************************************************
**
**   MultipleViewPanel   ---   do_layout
**
******************************************************/
void MultipleViewPanel::do_layout()
{
    // begin wxGlade: MultipleViewPanel::do_layout
    wxFlexGridSizer* grid_main = new wxFlexGridSizer(2, 2, 0, 0);
    wxFlexGridSizer* sizer_right = new wxFlexGridSizer(1, 1, 3, 3);
    wxStaticBoxSizer* sizer_nbstyle = new wxStaticBoxSizer(sizer_nbstyle_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_nbstyle = new wxFlexGridSizer(2, 2, 3, 3);
    wxFlexGridSizer* sizer_left = new wxFlexGridSizer(2, 1, 3, 3);
    wxStaticBoxSizer* sizer_mview = new wxStaticBoxSizer(sizer_mview_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_mview = new wxFlexGridSizer(3, 1, 0, 0);
    wxBoxSizer* sizer_description = new wxBoxSizer(wxHORIZONTAL);
    wxFlexGridSizer* grid_select_view = new wxFlexGridSizer(1, 2, 0, 0);
    wxStaticBoxSizer* sizer_usemview = new wxStaticBoxSizer(sizer_usemview_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_usemview = new wxFlexGridSizer(2, 2, 3, 3);
    grid_usemview->Add(label_usemview, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_usemview->Add(choice_usemview, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    grid_usemview->AddGrowableCol(1);
    sizer_usemview->Add(grid_usemview, 1, wxALL|wxEXPAND, 3);
    sizer_left->Add(sizer_usemview, 1, wxALL|wxEXPAND, 3);
    grid_select_view->Add(label_defview, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_select_view->Add(choice_viewlist, 0, wxALL|wxEXPAND, 3);
    grid_select_view->AddGrowableCol(1);
    grid_mview->Add(grid_select_view, 1, wxALL|wxEXPAND, 3);
    sizer_description->Add(text_view_description, 0, wxALL, 3);
    panel_description->SetSizer(sizer_description);
    grid_mview->Add(panel_description, 1, wxALL|wxEXPAND, 3);
    grid_mview->AddGrowableRow(2);
    grid_mview->AddGrowableCol(0);
    sizer_mview->Add(grid_mview, 1, wxEXPAND, 0);
    sizer_left->Add(sizer_mview, 1, wxALL|wxEXPAND, 3);
    grid_main->Add(sizer_left, 1, wxEXPAND, 0);
    grid_nbstyle->Add(label_nbstyle, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_nbstyle->Add(choice_nbstyle, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    grid_nbstyle->Add(label_orientation, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_nbstyle->Add(choice_orientation, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    sizer_nbstyle->Add(grid_nbstyle, 1, wxALL|wxEXPAND, 3);
    sizer_right->Add(sizer_nbstyle, 1, wxALL|wxEXPAND, 3);
    grid_main->Add(sizer_right, 1, wxEXPAND, 0);
    SetSizer(grid_main);
    grid_main->Fit(this);
    // end wxGlade
}

/*****************************************************
**
**   MultipleViewPanel   ---   updateMViewDescription
**
******************************************************/
void MultipleViewPanel::updateMViewDescription()
{
	int sel = choice_viewlist->GetSelection();
	wxCoord w, h;
	panel_description->GetSize( &w, &h );
	if ( sel == -1 )
		text_view_description->SetLabel( wxT( "Error" ));
	else
		text_view_description->SetLabel( MultipleViewConfigLoader::get()->getConfig( sel )->description );
	text_view_description->Wrap( (int)w - 12 );
}

/*****************************************************
**
**   MultipleViewPanel   ---   OnMViewChoice
**
******************************************************/
void MultipleViewPanel::OnMViewChoice( wxCommandEvent &event )
{
	updateMViewDescription();
}

/*****************************************************
**
**   ConfigPanelFactory   ---   createMultipleViewPanel
**
******************************************************/
ConfigPanel *ConfigPanelFactory::createMultipleViewPanel( wxWindow *parent )
{
	return new MultipleViewPanel( parent );
}

