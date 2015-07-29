/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/ConfigDialog.h
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

#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#ifdef __GNUG__
#pragma interface "ConfigDialog.h"
#endif

#include <wx/dialog.h>

#include "ConfigPanel.h"

class ConfigPanel;
class wxBoxSizer;
class wxPanel;

class wxTreebook;

BEGIN_DECLARE_EVENT_TYPES()
DECLARE_EVENT_TYPE( CONFIG_CHANGED, wxID_HIGHEST + 1300 )
DECLARE_EVENT_TYPE( CONFIG_TOOLBAR_CHANGED, wxID_HIGHEST + 1301 )
END_DECLARE_EVENT_TYPES()

/*************************************************//**
*
* \brief base class for config dialog
*
******************************************************/
class ConfigBaseDialog: public wxDialog
{
	DECLARE_CLASS( ConfigBaseDialog )
public:
	ConfigBaseDialog( wxWindow* parent );

protected:

	virtual void saveData() = 0;

	void OnOK( wxCommandEvent& );
	void OnCancel( wxCommandEvent& );

	wxBoxSizer* buttonsizer;
};

/*************************************************//**
*
* \brief main class for notebook based config dialog
*
******************************************************/
class SimpleConfigDialog: public ConfigBaseDialog
{
	DECLARE_CLASS( SimlpeConfigDialog )

public:

	SimpleConfigDialog( const int&, wxWindow* parent );
	~SimpleConfigDialog();

protected:

	void OnApply( wxCommandEvent& );
	ConfigPanel *configpanel;
	//void OnSize( wxSizeEvent& );
	virtual void saveData();
};


/*************************************************//**
*
* \brief main class for notebook based config dialog
*
******************************************************/
class ConfigDialog: public ConfigBaseDialog
{
	DECLARE_CLASS( ConfigDialog )

public:

	ConfigDialog( wxWindow* parent );
	~ConfigDialog();

protected:

	void OnApply( wxCommandEvent& );
	void OnSize( wxSizeEvent& );
	virtual void saveData();

	void showPanel( const int &selected );

	ConfigPanel *configpanel[NB_PANELS];
	wxPanel *panel[NB_PANELS];

	wxTreebook* notebook;

#ifdef _WX29
	void OnTreebook( class wxBookCtrlEvent& );
#else
	void OnTreebook( class wxTreebookEvent& );
#endif
};

#endif // CONFIGDIALOG_H

