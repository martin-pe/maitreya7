/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/ConfigPanel.h
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

#ifndef CONFIGPANEL_H
#define CONFIGPANEL_H

#ifdef __GNUG__
#pragma interface "ConfigPanel.h"
#endif

#include <wx/panel.h>

enum {
	CONFIGPANEL_GENERAL = 0,
	CONFIGPANEL_DEFAULTLOCATION,
	CONFIGPANEL_PRINTOUT,
	CONFIGPANEL_EPHEMERIS,
	CONFIGPANEL_ANIMATION,
	CONFIGPANEL_ATLAS,

	CONFIGPANEL_VIEW,
	CONFIGPANEL_COLOR,
	CONFIGPANEL_OBJECTCOLOR,
	CONFIGPANEL_DASACOLOR,
	CONFIGPANEL_ASPECTCOLOR,
	CONFIGPANEL_FONT,
	CONFIGPANEL_MULITPLEVIEW,
	CONFIGPANEL_TOOLBAR,

	CONFIGPANEL_VEDIC,
	CONFIGPANEL_VEDICCHART,
	CONFIGPANEL_VEDICCALCULATION,
	CONFIGPANEL_VARGA,

	CONFIGPANEL_WESTERN,
	CONFIGPANEL_WESTERNCHART,
	CONFIGPANEL_WESTERNCALCULATION,
	CONFIGPANEL_ASPECT
};
#define CONFIGPANEL_FIRST CONFIGPANEL_GENERAL
#define CONFIGPANEL_LAST CONFIGPANEL_ASPECT

#define NB_PANELS CONFIGPANEL_LAST+1

/*************************************************//**
*
* \brief Base class for all panels in ConfigurationDialog
*
******************************************************/
class ConfigPanel: public wxPanel
{
	DECLARE_CLASS( ConfigPanel )

public:

	ConfigPanel( wxWindow* parent );

	/**
	 * \brief Abstact method for reading data from config and setup of UI items. Must be overwritten.
	 */
	virtual void setData() = 0;

	/**
	 * \brief Abstact method for reading UI item states and saving data to config. Must be overwritten.
	 *
	 * May return false on error.
	 */
	virtual bool saveData() = 0;

	/**
	 * \brief Called on panel activation. May be overwritten.
	 */
	virtual void onActivate() {}

	/**
	 * \brief Called on panel deactivation. May be overwritten.
	 */
	virtual void onPassivate() {}

	bool isDirty() {
		return dirty;
	}

protected:
	void OnSize( wxSizeEvent& );

	void OnChoice( wxCommandEvent& );
	bool dirty;
};

/*************************************************//**
*
*
*
******************************************************/
class ConfigPanelFactory
{
public:
	ConfigPanel *createPanel( const int&, wxWindow* );
	wxString getPanelName( const int& );

private:
	ConfigPanel *createGeneralPanel( wxWindow* );
	ConfigPanel *createDefaultLocationPanel( wxWindow* );
	ConfigPanel *createPrintoutPanel( wxWindow* );
	ConfigPanel *createEphemerisPanel( wxWindow* );
	ConfigPanel *createAnimationPanel( wxWindow* );
	ConfigPanel *createAtlasPanel( wxWindow* );

	ConfigPanel *createViewPanel( wxWindow* );
	ConfigPanel *createColorPanel( wxWindow* );
	ConfigPanel *createObjectColorPanel( wxWindow* );
	ConfigPanel *createDasaColorPanel( wxWindow* );
	ConfigPanel *createAspectColorPanel( wxWindow* );
	ConfigPanel *createFontPanel( wxWindow* );
	ConfigPanel *createMultipleViewPanel( wxWindow* );
	ConfigPanel *createToolbarPanel( wxWindow* );

	ConfigPanel *createVedicPanel( wxWindow* );
	ConfigPanel *createVedicChartPanel( wxWindow* );
	ConfigPanel *createVedicCalculationPanel( wxWindow* );
	ConfigPanel *createVargaPanel( wxWindow* );

	ConfigPanel *createWesternPanel( wxWindow* );
	ConfigPanel *createWesternChartPanel( wxWindow* );
	ConfigPanel *createWesternCalculationPanel( wxWindow* );
	ConfigPanel *createAspectPanel( wxWindow* );
};

#endif // CONFIGPANEL_H

