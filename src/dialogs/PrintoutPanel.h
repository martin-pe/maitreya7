/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/PrintoutPanel.h
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

#ifndef PRINTOUTPANEL_H
#define PRINTOUTPANEL_H

#ifdef __GNUG__
#pragma interface "PrintoutPanel.h"
#endif

#include "ConfigPanel.h"

// begin wxGlade: dependencies
// end wxGlade

class wxButton;
class wxCheckBox;
class wxChoice;
class wxSpinCtrl;
class wxStaticBox;
class wxStaticLine;
class wxStaticText;
class wxTextCtrl;

/*************************************************//**
*
* \brief ConfigPanel for printout parameters
*
******************************************************/
class PrintoutPanel: public ConfigPanel
{
	DECLARE_CLASS( PrintoutPanel )

public:
    // begin wxGlade: PrintoutPanel::ids
    // end wxGlade

	PrintoutPanel( wxWindow* parent );

	virtual void setData();
	virtual bool saveData();

private:
    // begin wxGlade: PrintoutPanel::methods
    void set_properties();
    void do_layout();
    // end wxGlade

	void OnSwitchViewerMode( wxCommandEvent& );
	void OnSwitchFooterMode( wxCommandEvent&  );

	void OnTextChanged( wxCommandEvent& );
	void OnChooseCommand( wxCommandEvent& );
	void OnPrintoutChoice( wxCommandEvent& );
	void setPrintoutFields();

protected:
    // begin wxGlade: PrintoutPanel::attributes
    wxStaticBox* sizer_viewer_staticbox;
    wxStaticBox* sizer_footer_staticbox;
    wxStaticBox* sizer_options_staticbox;
    wxStaticBox* sizer_printout_staticbox;
    wxChoice* choice_printout_type;
    wxStaticText* text_printout_description;
    wxPanel* panel_description;
    wxStaticText* label_vstyle;
    wxChoice* choice_vstyle;
    wxStaticText* label_wstyle;
    wxChoice* choice_wstyle;
    wxStaticText* label_paper;
    wxChoice* choice_paperformat;
    wxCheckBox* check_overwrite;
    wxCheckBox* check_printusertext;
    wxTextCtrl* text_footer;
    wxCheckBox* check_launchviewer;
    wxButton* button_launchcommand;
    wxTextCtrl* text_launchcommand;
    // end wxGlade
}; // wxGlade: end class


#endif // PRINTOUTPANEL_H
