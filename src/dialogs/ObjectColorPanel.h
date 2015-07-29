/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/ObjectColorPanel.h
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

#ifndef OBJECTCOLORPANEL_H
#define OBJECTCOLORPANEL_H

#ifdef __GNUG__
#pragma interface "ObjectColorPanel.h"
#endif

#include "ConfigPanel.h"
#include "ColorConfig.h"

#include "constants.h"

// begin wxGlade: ::dependencies
// end wxGlade

// begin wxGlade: ::extracode

// end wxGlade

class PlanetColorPreviewPanel;
class HouseFgColorPreviewPanel;
class HouseBgColorPreviewPanel;
class SignFgColorPreviewPanel;
class SignBgColorPreviewPanel;

class wxButton;
class wxChoice;
class wxColourPickerCtrl;
class wxColourPickerEvent;
class wxPanel;
class wxStaticBox;
class wxStaticText;

/*************************************************//**
*
* \brief ConfigPanel for object color parameters
*
******************************************************/
class ObjectColorPanel: public ConfigPanel
{
	DECLARE_CLASS( ObjectColorPanel )

public:
    // begin wxGlade: ObjectColorPanel::ids
    // end wxGlade

	ObjectColorPanel( wxWindow* parent );
	virtual void setData();
	virtual bool saveData();

private:
    // begin wxGlade: ObjectColorPanel::methods
    void set_properties();
    void do_layout();
    // end wxGlade

	void OnColorChanged( wxColourPickerEvent& );
	void OnRestoreDefaults( wxCommandEvent& );
	void OnChoice( wxCommandEvent& );

	void repaintPreviewPanels();
	void updateColorPickers();
	int planetChoiceToIndex( const int &selection );

	ChartColorConfig colors;

protected:
    // begin wxGlade: ObjectColorPanel::attributes
    wxStaticBox* sizer_houses_staticbox;
    wxStaticBox* sizer_signs_staticbox;
    wxStaticBox* sizer_planets_staticbox;
    wxChoice* choice_planets;
    wxStaticText* label_fgplanets;
    wxColourPickerCtrl* color_fgplanets;
    PlanetColorPreviewPanel* preview_planetcolor;
    wxChoice* choice_signs;
    wxStaticText* label_fg_signs;
    wxColourPickerCtrl* color_fgsigns;
    SignFgColorPreviewPanel* preview_signfgcolor;
    wxStaticText* label_bg_signs;
    wxColourPickerCtrl* color_bgsigns;
    SignBgColorPreviewPanel* preview_signbgcolor;
    wxChoice* choice_houses;
    wxStaticText* label_fg_houses;
    wxColourPickerCtrl* color_fghouses;
    HouseFgColorPreviewPanel* preview_housefgcolor;
    wxStaticText* label_bg_houses;
    wxColourPickerCtrl* color_bghouses;
    HouseBgColorPreviewPanel* preview_housebgcolor;
    wxButton* button_default;
    // end wxGlade
}; // wxGlade: end class


#endif // OBJECTCOLORPANEL_H
