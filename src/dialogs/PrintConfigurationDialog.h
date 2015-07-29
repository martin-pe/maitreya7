/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/PrintConfigurationDialog.h
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

#ifndef PRINTCONFIGURATIONDIALOG_H
#define PRINTCONFIGURATIONDIALOG_H

#ifdef __GNUG__
#pragma interface "PrintConfigurationDialog.h"
#endif

// begin wxGlade: dependencies
// end wxGlade

#include <wx/dialog.h>
#include <vector>

using namespace std;

class Horoscope;
class PrintInfo;
class wxButton;
class wxChoice;
class wxCheckBox;
class wxCommandEvent;
class wxStaticBox;
class wxStaticText;
class wxTextCtrl;

/*************************************************//**
*
*  selection of printout type
*
******************************************************/
class PrintConfigurationDialog: public wxDialog
{
	DECLARE_CLASS( PrintConfigurationDialog )

public:
    // begin wxGlade: PrintConfigurationDialog::ids
    // end wxGlade

	PrintConfigurationDialog(wxWindow* parent, Horoscope* );
	int getSelectedPrintout();
	wxString getFilename();
private:
    // begin wxGlade: PrintConfigurationDialog::methods
    void set_properties();
    void do_layout();
    // end wxGlade

	void OnChoice( wxCommandEvent& );
	void OnChooseFile( wxCommandEvent& );
	void OnTextEnter( wxCommandEvent& );

protected:
	Horoscope *h;
	void setFields();
    // begin wxGlade: PrintConfigurationDialog::attributes
    wxStaticBox* sizer_file2_staticbox;
    wxStaticBox* sizer_type_staticbox;
    wxChoice* choice_type;
    wxTextCtrl* text_description;
    wxButton* button_file;
    wxTextCtrl* text_file;
    wxButton* button_ok;
    wxButton* button_cancel;
    // end wxGlade
};



#endif

