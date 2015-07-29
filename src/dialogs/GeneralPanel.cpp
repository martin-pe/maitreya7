/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/GeneralPanel.cpp
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
#pragma implementation "GeneralPanel.h"
#endif

#include "GeneralPanel.h"

#include "Conf.h"
#include "guibase.h"
#include "Lang.h"
#include "LanguageConfig.h"

#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/statbox.h>
#include <wx/statline.h>
#include <wx/stattext.h>

using namespace std;

extern Config *config;

IMPLEMENT_CLASS( GeneralPanel, ConfigPanel )

/*****************************************************
**
**   GeneralPanel   ---   Constructor
**
******************************************************/
GeneralPanel::GeneralPanel( wxWindow* parent )
		: ConfigPanel( parent )
{
    // begin wxGlade: GeneralPanel::GeneralPanel
    sizer_misc_staticbox = new wxStaticBox(this, -1, _("Miscellaneous"));
    sizer_export_staticbox = new wxStaticBox(this, -1, _("Image and Text Export"));
    sizer_general_staticbox = new wxStaticBox(this, -1, _("General"));
    label_defstyle = new wxStaticText(this, wxID_ANY, _("Default Style"));
    const wxString choice_defstyle_choices[] = {
        _("Vedic"),
        _("Western")
    };
    choice_defstyle = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_defstyle_choices, 0);
    label_lang = new wxStaticText(this, wxID_ANY, _("Language"));
    const wxString choice_lang_choices[] = {
        _("dummy"),
        _("dummy"),
        _("dummy"),
        _("dummy")
    };
    choice_lang = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 4, choice_lang_choices, 0);
    check_opennewdoc = new wxCheckBox(this, wxID_ANY, _("Open New Document on Startup"));
    check_askonquit = new wxCheckBox(this, wxID_ANY, _("Ask on Quit"));
    check_showstatusinfo = new wxCheckBox(this, wxID_ANY, _("Running Objects in Status Bar"));
    spin_recent_files = new wxSpinCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100);
    const wxString choice_file_backup_choices[] = {
        _("Never"),
        _("One"),
        _("Infinite")
    };
    choice_file_backup = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 3, choice_file_backup_choices, 0);
    const wxString choice_text_export_choices[] = {
        _("HTML (*.html)"),
        _("Text (*.txt)"),
        _("CSV (*.csv)")
    };
    choice_text_export = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 3, choice_text_export_choices, 0);
    const wxString choice_graphic_export_choices[] = {
        _("PNG (*.png)"),
        _("JPEG (*.jpg)"),
        _("Bitmap (*.bmp)"),
        _("PCX (*.pcx)"),
        _("PNM (*.pnm)"),
        _("TIFF (*.tif)")
    };
    choice_graphic_export = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 6, choice_graphic_export_choices, 0);
    const wxString choice_graphic_size_dialog_choices[] = {
        _("Show size dialog"),
        _("Use view size")
    };
    choice_graphic_size_dialog = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_graphic_size_dialog_choices, 0);
    check_overwrite = new wxCheckBox(this, wxID_ANY, _("Ask on overwrite"));

    set_properties();
    do_layout();
    // end wxGlade
}

/*****************************************************
**
**   GeneralPanel   ---   set_properties
**
******************************************************/
void GeneralPanel::set_properties()
{
    // begin wxGlade: GeneralPanel::set_properties
    choice_defstyle->SetSelection(0);
    choice_lang->SetSelection(0);
    check_askonquit->SetMinSize(wxSize(150, 24));
    spin_recent_files->SetMinSize(wxSize(30, -1));
    choice_file_backup->SetSelection(0);
    choice_text_export->SetSelection(0);
    choice_graphic_export->SetSelection(0);
    choice_graphic_size_dialog->SetSelection(0);
    // end wxGlade
}

/*****************************************************
**
**   GeneralPanel   ---   do_layout
**
******************************************************/
void GeneralPanel::do_layout()
{
    // begin wxGlade: GeneralPanel::do_layout
    wxFlexGridSizer* sizer_main = new wxFlexGridSizer(1, 2, 0, 0);
    wxFlexGridSizer* sizer_right = new wxFlexGridSizer(3, 1, 0, 0);
    wxStaticBoxSizer* sizer_export = new wxStaticBoxSizer(sizer_export_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_misc_export = new wxFlexGridSizer(3, 2, 3, 3);
    wxStaticBoxSizer* sizer_misc = new wxStaticBoxSizer(sizer_misc_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_misc = new wxFlexGridSizer(4, 2, 3, 3);
    wxFlexGridSizer* sizer_left = new wxFlexGridSizer(3, 1, 0, 0);
    wxStaticBoxSizer* sizer_general = new wxStaticBoxSizer(sizer_general_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_general = new wxFlexGridSizer(2, 2, 3, 3);
    grid_general->Add(label_defstyle, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_general->Add(choice_defstyle, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_general->Add(label_lang, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_general->Add(choice_lang, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    grid_general->AddGrowableCol(1);
    sizer_general->Add(grid_general, 1, wxALL, 3);
    sizer_general->Add(check_opennewdoc, 0, wxALL|wxEXPAND, 1);
    sizer_general->Add(check_askonquit, 0, wxALL|wxEXPAND, 1);
    sizer_left->Add(sizer_general, 1, wxALL|wxEXPAND, 3);
    sizer_left->AddGrowableCol(0);
    sizer_main->Add(sizer_left, 1, wxALL, 3);
    sizer_misc->Add(check_showstatusinfo, 0, wxALL|wxEXPAND, 3);
    wxStaticText* label_recent = new wxStaticText(this, wxID_ANY, _("Maximum of Recent Files"));
    grid_misc->Add(label_recent, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_misc->Add(spin_recent_files, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    wxStaticText* label_file_backup = new wxStaticText(this, wxID_ANY, _("Create Backup Files"));
    grid_misc->Add(label_file_backup, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_misc->Add(choice_file_backup, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    sizer_misc->Add(grid_misc, 1, wxALL, 3);
    sizer_right->Add(sizer_misc, 1, wxALL|wxEXPAND, 3);
    wxStaticText* label_text_export = new wxStaticText(this, wxID_ANY, _("Text Format"));
    grid_misc_export->Add(label_text_export, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_misc_export->Add(choice_text_export, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    wxStaticText* label_graphic_export = new wxStaticText(this, wxID_ANY, _("Image Format"));
    grid_misc_export->Add(label_graphic_export, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_misc_export->Add(choice_graphic_export, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    wxStaticText* label_graphic_size_dialog = new wxStaticText(this, wxID_ANY, _("Image Size"));
    grid_misc_export->Add(label_graphic_size_dialog, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_misc_export->Add(choice_graphic_size_dialog, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    sizer_export->Add(grid_misc_export, 1, wxALL, 3);
    sizer_export->Add(check_overwrite, 0, wxALL|wxEXPAND, 3);
    sizer_right->Add(sizer_export, 1, wxALL|wxEXPAND, 3);
    sizer_right->AddGrowableCol(0);
    sizer_main->Add(sizer_right, 1, wxALL, 3);
    SetSizer(sizer_main);
    sizer_main->Fit(this);
    // end wxGlade
}

/*****************************************************
**
**   GeneralPanel   ---   setData
**
******************************************************/
void GeneralPanel::setData()
{
	choice_lang->Clear();
#ifdef __WXMSW__
	list<LanguageEntry>::iterator iter;
	int thelang = 0, i = 0;
	list<LanguageEntry> thelist = LanguageConfig::get()->getLanguages();
	for ( iter = thelist.begin(); iter != thelist.end(); iter++ )
	{
		choice_lang->Append( (*iter).name );
		if ( (*iter).code == config->lang ) thelang = i;
		i++;
	}
	choice_lang->SetSelection( thelang );
#else
	label_lang->Enable( false );
	choice_lang->Enable( false );
#endif

	config->preferVedic ? choice_defstyle->SetSelection( 0 ) : choice_defstyle->SetSelection( 1 );
	check_opennewdoc->SetValue( config->openNewDocOnStart );
	check_askonquit->SetValue( config->askOnQuit );

	check_showstatusinfo->SetValue( config->showStatusInfo );
	spin_recent_files->SetValue( config->maxRecentFiles );
	choice_file_backup->SetSelection( config->backupFileMode );

	choice_text_export->SetSelection( config->defTextExportType );
	choice_graphic_export->SetSelection( config->defGraphicExportType );
	choice_graphic_size_dialog->SetSelection( ! config->showGraphicSizeDialog );
	check_overwrite->SetValue( config->exportAskOnOverwrite );
}

/*****************************************************
**
**   GeneralPanel   ---   saveData
**
******************************************************/
bool GeneralPanel::saveData()
{
	wxColour color;

	list<LanguageEntry>::iterator iter;
	wxString sellang = choice_lang->GetStringSelection();
	wxString oldlang = config->lang;
	list<LanguageEntry> thelist = LanguageConfig::get()->getLanguages();
	for ( iter = thelist.begin(); iter != thelist.end(); iter++ )
	{
		if ( sellang == (*iter).name )
		{
			config->lang = (*iter).code;
			break;
		}
	}

	if ( config->lang != oldlang )
	{
		doNotShowAgainMessage( this, DO_NOT_SHOW_AGAIN_WARNING_AFTER_LANGUAGE_CHANGE );
		//doMessageBox( this, _("The language will be changed when you restart the program."), wxOK | wxCENTRE );
	}

	config->preferVedic = ! choice_defstyle->GetSelection();
	config->openNewDocOnStart = check_opennewdoc->GetValue();
	config->askOnQuit = check_askonquit->GetValue();

	config->showStatusInfo = check_showstatusinfo->GetValue();
	config->maxRecentFiles = spin_recent_files->GetValue();
	config->backupFileMode = choice_file_backup->GetSelection();

	config->defTextExportType = choice_text_export->GetSelection();
	config->defGraphicExportType = choice_graphic_export->GetSelection();
	config->showGraphicSizeDialog = ! choice_graphic_size_dialog->GetSelection();
	config->exportAskOnOverwrite = check_overwrite->GetValue();
	return true;
}

/*****************************************************
**
**   ConfigPanelFactory   ---   createGeneralPanel
**
******************************************************/
ConfigPanel *ConfigPanelFactory:: createGeneralPanel( wxWindow *parent )
{
	return new GeneralPanel( parent );
}


