/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/AspectColorPanel.h
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

#ifndef ASPECTCOLORPANEL_H
#define ASPECTCOLORPANEL_H

#ifdef __GNUG__
#pragma interface "AspectColorPanel.h"
#endif

#include "ConfigPanel.h"

#include <wx/colour.h>
#include <wx/pen.h>
#include <wx/scrolwin.h>

#include "Aspect.h"

// begin wxGlade: dependencies
// end wxGlade

class wxButton;
class wxCheckBox;
class wxChoice;
class wxColourPickerCtrl;
class wxColourPickerEvent;
class wxSpinCtrl;
class wxSpinEvent;
class wxStaticBox;
class wxStaticText;

/*************************************************//**
*
* \brief Panel for aspect preview
*
******************************************************/
class AspectPreviewPanel: public wxPanel
{
	DECLARE_CLASS( AspectPreviewPanel )

public:

	AspectPreviewPanel( wxWindow* parent, wxWindowID id = -1, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
	                    long style = wxTAB_TRAVERSAL );

	void setConfig( AspectConfig*, const int& );
	void setSelected( const int &sel ) {
		selected = sel;
	}

private:

	AspectConfig *configs[MAX_ASPECT_TYPES];
	bool showSymbols;
	int selected;

	void OnPaint( wxPaintEvent& );
	void drawSingleAspect( class Painter *painter, const int &aspectindex, const int &xleft, const int &xright, const int &y );
	void OnMouseLeftDown( wxMouseEvent& );

};

/*************************************************//**
*
* \brief Panel for aspect color configuration
*
******************************************************/
class AspectColorPanel: public ConfigPanel
{
	DECLARE_CLASS( AspectColorPanel )

public:
    // begin wxGlade: AspectColorPanel::ids
    // end wxGlade

	AspectColorPanel( wxWindow* parent );
	virtual void setData();
	virtual bool saveData();

private:
    // begin wxGlade: AspectColorPanel::methods
    void set_properties();
    void do_layout();
    // end wxGlade
	void OnAspectChoice( wxCommandEvent& );
	void OnLineStyleChoice( wxCommandEvent& );
	void OnSpinWidth( wxSpinEvent& );
	void OnColorButton( wxColourPickerEvent& );
	void OnDefaultButton( wxCommandEvent& );
	void updateControls();
	void OnSelectedAspectChanged( wxCommandEvent& );

	AspectConfig configs[MAX_ASPECT_TYPES];
	int selconfig;

protected:
    // begin wxGlade: AspectColorPanel::attributes
    wxStaticBox* sizer_preview_staticbox;
    wxStaticBox* sizer_styles_staticbox;
    wxChoice* choice_aspect_type;
    wxStaticText* label_color;
    wxColourPickerCtrl* button_color;
    wxStaticText* label_width;
    wxSpinCtrl* spin_width;
    wxStaticText* label_linestyle;
    wxChoice* choice_linestyle;
    wxButton* button_default;
    AspectPreviewPanel* preview_panel;
    // end wxGlade
}; // wxGlade: end class


#endif // ASPECTCOLORPANEL_H
