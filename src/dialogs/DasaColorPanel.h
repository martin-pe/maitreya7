/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/DasaColorPanel.h
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

#ifndef DASACOLORPANEL_H
#define DASACOLORPANEL_H

#ifdef __GNUG__
#pragma interface "DasaColorPanel.h"
#endif

#include "ConfigPanel.h"

#include "constants.h"

#include <wx/colour.h>

// begin wxGlade: dependencies
// end wxGlade

class DasaColorConfig;
class wxBrush;
class wxButton;
class wxChoice;
class wxColourPickerCtrl;
class wxColourPickerEvent;
class wxPen;
class wxSpinCtrl;
class wxSpinEvent;
class wxStaticBox;
class wxStaticLine;
class wxStaticText;

/*************************************************//**
*
* \brief preview panel for Dasa bar
*
******************************************************/
class DasaPreviewPanel: public wxPanel
{
	DECLARE_CLASS( DasaPreviewPanel )

public:
	DasaPreviewPanel( wxWindow* parent, wxWindowID id = -1, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
	                  long style = wxTAB_TRAVERSAL );
	void setDasaConfig( DasaColorConfig *d ) {
		dconfig = d;
	}
	void setObjectId( const int &id ) {
		objectid = id;
	}
	void setCornerRadius( const int &r )  {
		cornerRadius = r;
	}

private:

	wxPen *pen;
	wxBrush *brush;
	wxColour textcolor;
	int objectid, cornerRadius;
	void OnPaint( wxPaintEvent& );
	DasaColorConfig *dconfig;
};

/*************************************************//**
*
* \brief ConfigPanel for Dasa parameters
*
******************************************************/
class DasaColorPanel: public ConfigPanel
{
	DECLARE_CLASS( DasaColorPanel )

public:
    // begin wxGlade: DasaColorPanel::ids
    // end wxGlade

	DasaColorPanel( wxWindow* parent );
	~DasaColorPanel();
	virtual void setData();
	virtual bool saveData();

private:
    // begin wxGlade: DasaColorPanel::methods
    void set_properties();
    void do_layout();
    // end wxGlade

	void updateControls();
	void updateCfg();
	void OnSetDefaults( wxCommandEvent &event );
	void OnObjectChoice( wxCommandEvent& );

	void onStyleControl();
	void OnChoice( wxCommandEvent& ) {
		onStyleControl();
	}
	void OnSpin( wxSpinEvent& ) {
		onStyleControl();
	}
	void OnColorPicker( wxColourPickerEvent& ) {
		onStyleControl();
	}

	DasaColorConfig *dasaconfig[MAX_DASA_COLORS];

protected:
    // begin wxGlade: DasaColorPanel::attributes
    wxStaticBox* sizer_cornerradius_staticbox;
    wxStaticBox* sizer_dasacolors_staticbox;
    wxStaticBox* sizer_bg_staticbox;
    wxStaticBox* sizer_fg_staticbox;
    wxStaticBox* sizer_preview_staticbox;
    wxStaticBox* sizer_object_staticbox;
    wxChoice* choice_object;
    DasaPreviewPanel* window_preview;
    wxPanel* panel_preview;
    wxStaticText* label_fgcolor;
    wxColourPickerCtrl* button_fgcolor;
    wxStaticText* label_fgwidth;
    wxSpinCtrl* spin_fgwidth;
    wxStaticText* label_fgstyle;
    wxChoice* choice_fgstyle;
    wxStaticText* label_bgcolor;
    wxColourPickerCtrl* button_bgcolor;
    wxStaticText* label_bgstyle;
    wxChoice* choice_bgstyle;
    wxStaticText* label_bgimage;
    wxChoice* choice_bgimage;
    wxStaticText* label_cornerradius;
    wxSpinCtrl* spin_cornerradius;
    wxButton* button_default;
    // end wxGlade
}; // wxGlade: end class


#endif // DASACOLORPANEL_H
