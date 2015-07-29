/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/FontPanel.h
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

#ifndef FONTPANEL_H
#define FONTPANEL_H

#ifdef __GNUG__
#pragma interface "FontPanel.h"
#endif

#include "ConfigPanel.h"

// begin wxGlade: dependencies
// end wxGlade

class wxButton;
class wxChoice;
class wxStaticBox;
class wxStaticLine;
class wxStaticText;
class wxTextCtrl;

/*************************************************//**
*
* \brief ConfigPanel for font parameters
*
******************************************************/
class FontPanel: public ConfigPanel
{
	DECLARE_CLASS( FontPanel )

public:
    // begin wxGlade: FontPanel::ids
    // end wxGlade

	FontPanel( wxWindow* parent );
	virtual void setData();
	virtual bool saveData();

private:
    // begin wxGlade: FontPanel::methods
    void set_properties();
    void do_layout();
    // end wxGlade

	void OnChooseFont( wxCommandEvent& );
	void OnSymbolFontSize( wxCommandEvent& );
	void OnSetDefaults( wxCommandEvent& );

	void setSymbolFontText();
	void setTextControlItem( wxTextCtrl *ctrl, wxFont *font );

protected:
    // begin wxGlade: FontPanel::attributes
    wxStaticBox* sizer_pdffonts_staticbox;
    wxStaticBox* sizer_fonts_staticbox;
    wxStaticText* label_default;
    wxTextCtrl* t_font_default;
    wxButton* button_default;
    wxStaticText* label_header;
    wxTextCtrl* t_font_table_header;
    wxButton* button_table_header;
    wxStaticText* label_graphic;
    wxTextCtrl* t_font_graphic;
    wxButton* button_graphic;
    wxStaticText* label_text;
    wxTextCtrl* t_font_text;
    wxButton* button_text;
    wxStaticText* label_symbol;
    wxTextCtrl* text_symbol;
    wxChoice* choice_symbol_font;
    wxStaticText* label_pdfdefault;
    wxTextCtrl* t_font_pdfdefault;
    wxButton* button_pdfdefault;
    wxStaticText* label_pdfheader;
    wxTextCtrl* t_font_pdftable_header;
    wxButton* button_pdftable_header;
    wxStaticText* label_pdfgraphic;
    wxTextCtrl* t_font_pdfgraphic;
    wxButton* button_pdfgraphic;
    wxStaticText* label_pdfsymbol;
    wxTextCtrl* text_pdfsymbol;
    wxChoice* choice_pdfsymbol_font;
    wxButton* button_defaults;
    // end wxGlade
}; // wxGlade: end class


#endif // COLORFONTPANEL_H
