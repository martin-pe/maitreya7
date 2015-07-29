/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/GeneralPanel.h
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

#ifndef GENERALPANEL_H
#define GENERALPANEL_H

#ifdef __GNUG__
#pragma interface "GeneralPanel.h"
#endif

#include "ConfigPanel.h"

// begin wxGlade: dependencies
// end wxGlade

class wxCheckBox;
class wxChoice;
class wxSpinCtrl;
class wxStaticBox;
class wxStaticLine;
class wxStaticText;

/*************************************************//**
*
* \brief ConfigPanel for general parameters
*
******************************************************/
class GeneralPanel: public ConfigPanel
{
	DECLARE_CLASS( GeneralPanel )

public:
    // begin wxGlade: GeneralPanel::ids
    // end wxGlade

	GeneralPanel( wxWindow* parent );
	virtual void setData();
	virtual bool saveData();

private:
    // begin wxGlade: GeneralPanel::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: GeneralPanel::attributes
    wxStaticBox* sizer_export_staticbox;
    wxStaticBox* sizer_misc_staticbox;
    wxStaticBox* sizer_general_staticbox;
    wxStaticText* label_defstyle;
    wxChoice* choice_defstyle;
    wxStaticText* label_lang;
    wxChoice* choice_lang;
    wxCheckBox* check_opennewdoc;
    wxCheckBox* check_askonquit;
    wxCheckBox* check_showstatusinfo;
    wxSpinCtrl* spin_recent_files;
    wxChoice* choice_file_backup;
    wxChoice* choice_text_export;
    wxChoice* choice_graphic_export;
    wxChoice* choice_graphic_size_dialog;
    wxCheckBox* check_overwrite;
    // end wxGlade
}; // wxGlade: end class


#endif // GENERALPANEL_H
