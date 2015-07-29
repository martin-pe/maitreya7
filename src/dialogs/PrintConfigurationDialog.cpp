/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/PrintConfigurationDialog.cpp
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
#pragma implementation "PrintConfigurationDialog.h"
#endif

#include "PrintConfigurationDialog.h"

#include "Conf.h"
#include "constants.h"
#include "guibase.h"
#include "Horoscope.h"
#include "PrintoutConfig.h"

#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/filedlg.h>
#include <wx/filename.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

enum { PC_CHOICE = wxID_HIGHEST + 100, PC_CHOOSEFILE, PC_FILETEXT };

IMPLEMENT_CLASS( PrintConfigurationDialog, wxDialog )

extern Config *config;

/*****************************************************
**
**   PrintConfigurationDialog   ---   Constructor
**
******************************************************/
PrintConfigurationDialog::PrintConfigurationDialog(wxWindow* parent, Horoscope *horoscope )
		: wxDialog(parent, -1, wxT( "" ), wxDefaultPosition, wxDefaultSize, DEFAULT_DIALOG_STYLE )
{
	h = horoscope;

    // begin wxGlade: PrintConfigurationDialog::PrintConfigurationDialog
    sizer_file2_staticbox = new wxStaticBox(this, -1, _("Output File"));
    sizer_type_staticbox = new wxStaticBox(this, -1, _("Type"));
    const wxString choice_type_choices[] = {
        _("dummy"),
        _("dummy"),
        _("dummy")
    };
    choice_type = new wxChoice(this, PC_CHOICE, wxDefaultPosition, wxDefaultSize, 3, choice_type_choices, 0);
    text_description = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY);
    button_file = new wxButton(this, PC_CHOOSEFILE, _("Choose ..."));
    text_file = new wxTextCtrl(this, PC_FILETEXT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    button_ok = new wxButton(this, wxID_OK, _("OK"));
    button_cancel = new wxButton(this, wxID_CANCEL, _("Cancel"));

    set_properties();
    do_layout();
    // end wxGlade
	text_file->SetFocus();

	Connect( PC_CHOICE, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( PrintConfigurationDialog::OnChoice ));
	Connect( PC_CHOOSEFILE, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PrintConfigurationDialog::OnChooseFile ));
	Connect( PC_FILETEXT, wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( PrintConfigurationDialog::OnTextEnter ));
}

/*****************************************************
**
**   PrintConfigurationDialog   ---   set_properties
**
******************************************************/
int PrintConfigurationDialog::getSelectedPrintout()
{
	return choice_type->GetSelection();
}

/*****************************************************
**
**   PrintConfigurationDialog   ---   setFields
**
******************************************************/
void PrintConfigurationDialog::setFields()
{
	wxString s;
	if ( choice_type->GetSelection() == -1 ) return;
	PrintoutConfig *pp = PrintoutConfigLoader::get()->getConfig( choice_type->GetSelection() );
	text_description->SetValue( pp->description );
}

/*****************************************************
**
**   PrintConfigurationDialog   ---   OnChoice
**
******************************************************/
void PrintConfigurationDialog::OnChoice( wxCommandEvent &event )
{
	setFields();
}

/*****************************************************
**
**   PrintConfigurationDialog   ---   OnChooseFile
**
******************************************************/
void PrintConfigurationDialog::OnChooseFile( wxCommandEvent &event )
{
	wxString filename, pp, nn, ee, vv, name;
	wxString filetypes = _( "Pdf Files (*.pdf)|*.pdf|All files (*)| *.*" );

	wxFileName::SplitPath( text_file->GetValue(), &vv, &pp, &nn, &ee );
	name = nn;
	if ( ! ee.IsEmpty()) name << wxT( "." ) << ee;
	wxFileDialog saveFileDialog( this, _("Choose Output File" ), vv + pp, name, filetypes, wxFD_SAVE );

	if ( saveFileDialog.ShowModal() == wxID_OK )
	{
		filename = saveFileDialog.GetDirectory() + wxFileName::GetPathSeparator() + saveFileDialog.GetFilename();
		config->defPdfSavePath = saveFileDialog.GetDirectory();
		wxFileName::SplitPath( filename, &vv, &pp, &nn, &ee );
		if ( ee != wxT( "pdf" )) filename += wxT( ".pdf" );
		text_file->SetValue( filename );
	}
}

/*****************************************************
**
**   PrintConfigurationDialog   ---   getFilename
**
******************************************************/
wxString PrintConfigurationDialog::getFilename()
{
	return text_file->GetValue();
}

/*****************************************************
**
**   PrintConfigurationDialog   ---   OnTextEnter
**
******************************************************/
void PrintConfigurationDialog::OnTextEnter( wxCommandEvent &event )
{
	EndModal( wxID_OK );
}

/*****************************************************
**
**   PrintConfigurationDialog   ---   set_properties
**
******************************************************/
void PrintConfigurationDialog::set_properties()
{
	choice_type->Clear();
	vector<BaseXmlConfig*> pv = PrintoutConfigLoader::get()->getConfigs();
	for ( unsigned int i = 0; i < pv.size(); i++ )
	{
		choice_type->Append( pv[i]->name );
	}
    // begin wxGlade: PrintConfigurationDialog::set_properties
    SetTitle(_("Print"));
    choice_type->SetSelection(0);
    text_description->SetMinSize(wxSize(400, 100));
    // end wxGlade
	wxString filename = config->defPdfSavePath;
	if ( !filename.IsEmpty() && filename.Last() != wxFileName::GetPathSeparator())
		filename << wxFileName::GetPathSeparator();
	filename <<  h->getHName() << wxT( ".pdf" );
	text_file->SetValue( filename );
}

/*****************************************************
**
**   PrintConfigurationDialog   ---   do_layout
**
******************************************************/
void PrintConfigurationDialog::do_layout()
{
    // begin wxGlade: PrintConfigurationDialog::do_layout
    wxFlexGridSizer* sizer_main = new wxFlexGridSizer(3, 1, 0, 0);
    wxBoxSizer* sizer_buttons = new wxBoxSizer(wxHORIZONTAL);
    wxStaticBoxSizer* sizer_file2 = new wxStaticBoxSizer(sizer_file2_staticbox, wxHORIZONTAL);
    wxFlexGridSizer* sizer_file = new wxFlexGridSizer(1, 2, 0, 0);
    wxStaticBoxSizer* sizer_type = new wxStaticBoxSizer(sizer_type_staticbox, wxVERTICAL);
    wxFlexGridSizer* sizer_type2 = new wxFlexGridSizer(2, 1, 0, 0);
    sizer_type2->Add(choice_type, 0, wxALL|wxEXPAND, 3);
    sizer_type2->Add(text_description, 0, wxALL|wxEXPAND, 3);
    sizer_type2->AddGrowableRow(1);
    sizer_type2->AddGrowableCol(0);
    sizer_type->Add(sizer_type2, 1, wxALL|wxEXPAND, 3);
    sizer_main->Add(sizer_type, 1, wxALL|wxEXPAND, 3);
    sizer_file->Add(button_file, 0, wxALL, 3);
    sizer_file->Add(text_file, 0, wxALL|wxEXPAND, 3);
    sizer_file->AddGrowableCol(1);
    sizer_file2->Add(sizer_file, 1, wxALL|wxEXPAND, 3);
    sizer_main->Add(sizer_file2, 1, wxEXPAND, 0);
    sizer_buttons->Add(button_ok, 0, wxALL, 3);
    sizer_buttons->Add(button_cancel, 0, wxALL, 3);
    sizer_main->Add(sizer_buttons, 1, wxALL|wxALIGN_CENTER_HORIZONTAL, 3);
    SetSizer(sizer_main);
    sizer_main->Fit(this);
    sizer_main->AddGrowableRow(0);
    sizer_main->AddGrowableCol(0);
    Layout();
    // end wxGlade
	choice_type->SetSelection( config->printDefaultPrintout );
	setFields();
}

