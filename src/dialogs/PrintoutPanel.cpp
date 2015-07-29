/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/PrintoutPanel.cpp
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
#pragma implementation "PrintoutPanel.h"
#endif

#include "PrintoutPanel.h"

#include "Conf.h"
#include "FontConfig.h"
#include "guibase.h"
#include "guivalidators.h"
#include "PrintoutConfig.h"
#include "VedicChartConfig.h"
#include "WesternChartConfig.h"

#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/colordlg.h>
#include <wx/filedlg.h>
#include <wx/font.h>
#include <wx/fontdlg.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/statbox.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

extern Config *config;
enum { CD_USE_FOOTER = wxID_HIGHEST + 1, CD_CHOICE_PRINTOUT, CD_COMMAND, CD_SWITCH_VIEWERMODE, CD_LAUNCHCOMMAND };

IMPLEMENT_CLASS( PrintoutPanel, ConfigPanel )

/*****************************************************
**
**   PrintoutPanel   ---   Constructor
**
******************************************************/
PrintoutPanel::PrintoutPanel( wxWindow* parent )
		: ConfigPanel( parent )
{
    // begin wxGlade: PrintoutPanel::PrintoutPanel
    sizer_printout_staticbox = new wxStaticBox(this, -1, _("Printout"));
    sizer_options_staticbox = new wxStaticBox(this, -1, _("Options"));
    sizer_footer_staticbox = new wxStaticBox(this, -1, _("Footer in PDF Output"));
    sizer_viewer_staticbox = new wxStaticBox(this, -1, _("Pdf Viewer"));
    panel_description = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL);
    const wxString choice_printout_type_choices[] = {
        _("dummy"),
        _("dummy"),
        _("dummy")
    };
    choice_printout_type = new wxChoice(this, CD_CHOICE_PRINTOUT, wxDefaultPosition, wxDefaultSize, 3, choice_printout_type_choices, 0);
    text_printout_description = new wxStaticText(panel_description, wxID_ANY, _("dummy"));
    label_vstyle = new wxStaticText(this, wxID_ANY, _("Vedic Chart Style"));
    const wxString choice_vstyle_choices[] = {
        _("dummy"),
        _("dummy"),
        _("dummy"),
        _("dummy"),
        _("dummy")
    };
    choice_vstyle = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 5, choice_vstyle_choices, 0);
    label_wstyle = new wxStaticText(this, wxID_ANY, _("Western Chart Style"));
    const wxString choice_wstyle_choices[] = {
        _("dummy"),
        _("dummy"),
        _("dummy"),
        _("dummy"),
        _("dummy")
    };
    choice_wstyle = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 5, choice_wstyle_choices, 0);
    label_paper = new wxStaticText(this, wxID_ANY, _("Paper Format"));
    const wxString choice_paperformat_choices[] = {
        _("A4 (210x297mm)"),
        _("Letter (8.5x11 in)"),
        _("Legal (8.5x14 in)")
    };
    choice_paperformat = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 3, choice_paperformat_choices, 0);
    check_overwrite = new wxCheckBox(this, wxID_ANY, _("Ask on Overwrite"));
    check_printusertext = new wxCheckBox(this, CD_USE_FOOTER, _("Print Custom Text in Footer"));
    text_footer = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
    check_launchviewer = new wxCheckBox(this, wxID_ANY, _("Launch Pdf Viewer"));
    button_launchcommand = new wxButton(this, CD_COMMAND, _("Command ..."));
    text_launchcommand = new wxTextCtrl(this, CD_LAUNCHCOMMAND, wxEmptyString);

    set_properties();
    do_layout();
    // end wxGlade
	choice_vstyle->Clear();
	vector<BaseXmlConfig*> gc = VedicChartConfigLoader::get()->getConfigs();
	for ( unsigned int i = 0; i < gc.size(); i++ )
	{
		choice_vstyle->Append( gc[i]->name );
	}
	choice_vstyle->SetSelection( config->printVGraphicSkin );

	choice_wstyle->Clear();
	vector<BaseXmlConfig*> wc = WesternChartConfigLoader::get()->getConfigs();
	for ( unsigned int i = 0; i < wc.size(); i++ )
	{
		choice_wstyle->Append( wc[i]->name );
	}
	choice_wstyle->SetSelection( config->printWGraphicSkin );

	// NO: maybe a command requiring PATH resolution or a complex one, e.g. MacOS open ...
  //text_launchcommand->SetValidator( FilenameValidator());

	Connect( CD_SWITCH_VIEWERMODE, wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( PrintoutPanel::OnSwitchViewerMode ));
	Connect( CD_USE_FOOTER, wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( PrintoutPanel::OnSwitchFooterMode ));
	Connect( CD_CHOICE_PRINTOUT, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( PrintoutPanel::OnPrintoutChoice ));
	Connect( CD_COMMAND, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PrintoutPanel::OnChooseCommand ));

	Connect( CD_LAUNCHCOMMAND, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( PrintoutPanel::OnTextChanged ));
}

/*****************************************************
**
**   PrintoutPanel   ---   setData
**
******************************************************/
void PrintoutPanel::setData()
{
	choice_printout_type->Clear();
	vector<BaseXmlConfig*> pv = PrintoutConfigLoader::get()->getConfigs();
	for ( unsigned int i = 0; i < pv.size(); i++ )
	{
		choice_printout_type->Append( pv[i]->name );
	}
	choice_printout_type->SetSelection( config->printDefaultPrintout );
	switch ( config->printPaperFormat )
	{
	case wxPAPER_LETTER:
		choice_paperformat->SetSelection( 1 );
		break;
	case wxPAPER_LEGAL:
		choice_paperformat->SetSelection( 2 );
		break;
	default:
		choice_paperformat->SetSelection( 0 );
		break;
	}
	check_overwrite->SetValue( config->printAskOverwrite );
	setPrintoutFields();
	check_launchviewer->SetValue( config->printLaunchPdfViewer );

	// Custom footer in printout
	check_printusertext->SetValue( config->printCustomFooter );
	text_footer->SetValue( config->printCustomFooterText );
	text_footer->Enable( config->printCustomFooter );


#ifdef __WXMSW__
	button_launchcommand->Enable( false );
	text_launchcommand->Enable( false );
#else
	text_launchcommand->SetValue( config->printPdfViewerCommand );
	button_launchcommand->Enable( config->printLaunchPdfViewer );
	text_launchcommand->Enable( config->printLaunchPdfViewer );
#endif
}

/*****************************************************
**
**   PrintoutPanel   ---   saveData
**
******************************************************/
bool PrintoutPanel::saveData()
{
	config->printDefaultPrintout = choice_printout_type->GetSelection();
	config->printLaunchPdfViewer = check_launchviewer->GetValue();
	config->printPdfViewerCommand = text_launchcommand->GetValue();
	config->printVGraphicSkin = choice_vstyle->GetSelection();
	config->printWGraphicSkin = choice_wstyle->GetSelection();
	switch ( choice_paperformat->GetSelection() )
	{
	case 1:
		config->printPaperFormat = wxPAPER_LETTER;
		break;
	case 2:
		config->printPaperFormat = wxPAPER_LEGAL;
		break;
	default:
		config->printPaperFormat = wxPAPER_A4;
		break;
	}
	config->printCustomFooterText =	text_footer->GetValue();
	config->printCustomFooter =	check_printusertext->GetValue();

	config->printAskOverwrite = check_overwrite->GetValue();
	return true;
}

/*****************************************************
**
**   PrintoutPanel   ---   OnTextChanged
**
******************************************************/
void PrintoutPanel::OnTextChanged( wxCommandEvent &event )
{
	Validate();
}

/*****************************************************
**
**   PrintoutPanel   ---   set_properties
**
******************************************************/
void PrintoutPanel::set_properties()
{
    // begin wxGlade: PrintoutPanel::set_properties
    choice_printout_type->SetSelection(0);
    text_printout_description->SetMinSize(wxSize(250, 100));
    choice_vstyle->SetSelection(0);
    choice_wstyle->SetSelection(0);
    choice_paperformat->SetSelection(0);
    text_footer->SetMinSize(wxSize(150,80));
    text_launchcommand->SetMinSize(wxSize(120, 25));
    // end wxGlade
	text_printout_description->SetBackgroundColour( GetBackgroundColour() );
	text_printout_description->SetForegroundColour( GetForegroundColour() );
}

/*****************************************************
**
**   PrintoutPanel   ---   do_layout
**
******************************************************/
void PrintoutPanel::do_layout()
{
    // begin wxGlade: PrintoutPanel::do_layout
    wxFlexGridSizer* sizer_main = new wxFlexGridSizer(1, 2, 0, 0);
    wxFlexGridSizer* sizer_right = new wxFlexGridSizer(2, 1, 0, 0);
    wxStaticBoxSizer* sizer_viewer = new wxStaticBoxSizer(sizer_viewer_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_sizer_3 = new wxFlexGridSizer(2, 1, 0, 0);
    wxStaticBoxSizer* sizer_footer = new wxStaticBoxSizer(sizer_footer_staticbox, wxVERTICAL);
    wxFlexGridSizer* sizer_left = new wxFlexGridSizer(1, 1, 0, 0);
    wxStaticBoxSizer* sizer_options = new wxStaticBoxSizer(sizer_options_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_options = new wxFlexGridSizer(3, 1, 3, 3);
    wxFlexGridSizer* grid_options2 = new wxFlexGridSizer(4, 2, 3, 3);
    wxStaticBoxSizer* sizer_printout = new wxStaticBoxSizer(sizer_printout_staticbox, wxVERTICAL);
    wxFlexGridSizer* sizer_printout2 = new wxFlexGridSizer(7, 1, 0, 0);
    wxBoxSizer* sizer_description = new wxBoxSizer(wxHORIZONTAL);
    sizer_printout2->Add(choice_printout_type, 0, wxALL, 3);
    sizer_description->Add(text_printout_description, 0, wxALL|wxEXPAND, 3);
    panel_description->SetSizer(sizer_description);
    sizer_printout2->Add(panel_description, 1, wxALL|wxEXPAND, 3);
    sizer_printout2->AddGrowableRow(2);
    sizer_printout2->AddGrowableCol(0);
    sizer_printout->Add(sizer_printout2, 1, wxALL|wxEXPAND, 3);
    sizer_left->Add(sizer_printout, 1, wxEXPAND, 3);
    grid_options2->Add(label_vstyle, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_options2->Add(choice_vstyle, 0, wxALL|wxEXPAND, 3);
    grid_options2->Add(label_wstyle, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_options2->Add(choice_wstyle, 0, wxALL|wxEXPAND, 3);
    grid_options2->Add(label_paper, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    grid_options2->Add(choice_paperformat, 0, wxALL, 3);
    grid_options->Add(grid_options2, 1, wxALL, 3);
    grid_options->Add(check_overwrite, 0, wxALL|wxEXPAND, 3);
    sizer_options->Add(grid_options, 1, wxEXPAND, 0);
    sizer_left->Add(sizer_options, 1, wxEXPAND, 0);
    sizer_left->AddGrowableCol(0);
    sizer_main->Add(sizer_left, 1, wxALL|wxEXPAND, 3);
    sizer_footer->Add(check_printusertext, 0, wxALL|wxEXPAND, 3);
    sizer_footer->Add(text_footer, 0, wxALL|wxEXPAND, 3);
    sizer_right->Add(sizer_footer, 1, wxEXPAND, 0);
    grid_sizer_3->Add(check_launchviewer, 0, wxALL|wxEXPAND, 3);
    grid_sizer_3->Add(button_launchcommand, 0, wxALL, 3);
    grid_sizer_3->Add(text_launchcommand, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    sizer_viewer->Add(grid_sizer_3, 1, wxALL|wxEXPAND, 3);
    sizer_right->Add(sizer_viewer, 1, wxALL|wxEXPAND, 3);
    sizer_main->Add(sizer_right, 1, wxALL|wxEXPAND, 3);
    SetSizer(sizer_main);
    sizer_main->Fit(this);
    // end wxGlade
	text_footer->Enable( check_printusertext->GetValue());
}

/*****************************************************
**
**   PrintoutPanel   ---   OnSwitchFooterMode
**
******************************************************/
void PrintoutPanel::OnSwitchFooterMode( wxCommandEvent &event )
{
	text_footer->Enable( check_printusertext->GetValue());
}

/*****************************************************
**
**   PrintoutPanel   ---   OnSwitchViewerMode
**
******************************************************/
void PrintoutPanel::OnSwitchViewerMode( wxCommandEvent &event )
{
#ifndef __WXMSW__
	button_launchcommand->Enable( check_launchviewer->GetValue());
	text_launchcommand->Enable( check_launchviewer->GetValue());
#endif
}

/*****************************************************
**
**   PrintoutPanel   ---   OnChooseCommand
**
******************************************************/
void PrintoutPanel::OnChooseCommand( wxCommandEvent &event )
{
	wxFileDialog dialog( this, _("Choose Pdf Viewer" ));

	if ( dialog.ShowModal() == wxID_OK )
	{
		config->printPdfViewerCommand = dialog.GetDirectory();
		if ( config->printPdfViewerCommand != wxEmptyString ) config->printPdfViewerCommand << wxT( "/" );
		config->printPdfViewerCommand << dialog.GetFilename();
		text_launchcommand->SetValue( config->printPdfViewerCommand );
	}
}

/*****************************************************
**
**   PrintoutPanel   ---   setPrintoutFields
**
******************************************************/
void PrintoutPanel::setPrintoutFields()
{
	wxString s;
	if ( choice_printout_type->GetSelection() == -1 ) return;
	PrintoutConfig *pp = PrintoutConfigLoader::get()->getConfig( choice_printout_type->GetSelection() );
	text_printout_description->SetLabel( pp->description );

	wxCoord w, h;
	panel_description->GetSize( &w, &h );
	text_printout_description->Wrap( (int)w - 12 );
}

/*****************************************************
**
**   PrintoutPanel   ---   OnPrintoutChoice
**
******************************************************/
void PrintoutPanel::OnPrintoutChoice( wxCommandEvent &event )
{
	setPrintoutFields();
}

/*****************************************************
**
**   ConfigPanelFactory   ---   createPrintoutPanel
**
******************************************************/
ConfigPanel *ConfigPanelFactory:: createPrintoutPanel( wxWindow *parent )
{
	return new PrintoutPanel( parent );
}



