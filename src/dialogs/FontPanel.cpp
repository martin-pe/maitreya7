/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/FontPanel.cpp
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
#pragma implementation "FontPanel.h"
#endif

#include "FontPanel.h"

#include "Conf.h"
#include "FontConfig.h"
#include "guibase.h"

#include <wx/button.h>
#include <wx/choice.h>
#include <wx/font.h>
#include <wx/fontdlg.h>
#include <wx/fontenum.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

extern Config *config;
enum { CD_FONT_DEFAULT = wxID_HIGHEST + 1, CD_FONT_TABLE_HEADER, CD_FONT_GRAPHIC, CD_FONT_TEXT, CD_FONT_SYMBOL, 
	CD_PDFFONT_DEFAULT, CD_PDFFONT_TABLE_HEADER, CD_PDFFONT_GRAPHIC, CD_PDFFONT_SYMBOL, CD_SET_DEFAULTS };

IMPLEMENT_CLASS( FontPanel, ConfigPanel )

/*****************************************************
**
**   FontPanel   ---   Constructor
**
******************************************************/
FontPanel::FontPanel( wxWindow* parent )
		: ConfigPanel( parent )
{
    // begin wxGlade: FontPanel::FontPanel
    sizer_pdffonts_staticbox = new wxStaticBox(this, -1, _("Pdf Output"));
    sizer_fonts_staticbox = new wxStaticBox(this, -1, _("Views and Text Widgets"));
    label_default = new wxStaticText(this, wxID_ANY, _("Default"));
    t_font_default = new wxTextCtrl(this, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    button_default = new wxButton(this, CD_FONT_DEFAULT, _("Choose..."));
    label_header = new wxStaticText(this, wxID_ANY, _("Table Header"));
    t_font_table_header = new wxTextCtrl(this, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    button_table_header = new wxButton(this, CD_FONT_TABLE_HEADER, _("Choose..."));
    label_graphic = new wxStaticText(this, wxID_ANY, _("Graphic"));
    t_font_graphic = new wxTextCtrl(this, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    button_graphic = new wxButton(this, CD_FONT_GRAPHIC, _("Choose..."));
    label_text = new wxStaticText(this, wxID_ANY, _("Text"));
    t_font_text = new wxTextCtrl(this, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    button_text = new wxButton(this, CD_FONT_TEXT, _("Choose..."));
    label_symbol = new wxStaticText(this, wxID_ANY, _("Symbol"));
    text_symbol = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    const wxString choice_symbol_font_choices[] = {
        _("Small"),
        _("Medium"),
        _("Big"),
        _("Bigger"),
        _("Huge")
    };
    choice_symbol_font = new wxChoice(this, CD_FONT_SYMBOL, wxDefaultPosition, wxDefaultSize, 5, choice_symbol_font_choices, 0);
    label_pdfdefault = new wxStaticText(this, wxID_ANY, _("Default"));
    t_font_pdfdefault = new wxTextCtrl(this, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    button_pdfdefault = new wxButton(this, CD_PDFFONT_DEFAULT, _("Choose..."));
    label_pdfheader = new wxStaticText(this, wxID_ANY, _("Table Header"));
    t_font_pdftable_header = new wxTextCtrl(this, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    button_pdftable_header = new wxButton(this, CD_PDFFONT_TABLE_HEADER, _("Choose..."));
    label_pdfgraphic = new wxStaticText(this, wxID_ANY, _("Graphic"));
    t_font_pdfgraphic = new wxTextCtrl(this, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    button_pdfgraphic = new wxButton(this, CD_PDFFONT_GRAPHIC, _("Choose..."));
    label_pdfsymbol = new wxStaticText(this, wxID_ANY, _("Symbol"));
    text_pdfsymbol = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    const wxString choice_pdfsymbol_font_choices[] = {
        _("Small"),
        _("Medium"),
        _("Big"),
        _("Bigger"),
        _("Huge")
    };
    choice_pdfsymbol_font = new wxChoice(this, CD_PDFFONT_SYMBOL, wxDefaultPosition, wxDefaultSize, 5, choice_pdfsymbol_font_choices, 0);
    button_defaults = new wxButton(this, CD_SET_DEFAULTS, _("Restore Defaults"));

    set_properties();
    do_layout();
    // end wxGlade

	Connect( CD_FONT_DEFAULT, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FontPanel::OnChooseFont ));
	Connect( CD_FONT_TABLE_HEADER, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FontPanel::OnChooseFont ));
	Connect( CD_FONT_GRAPHIC, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FontPanel::OnChooseFont ));
	Connect( CD_FONT_TEXT, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FontPanel::OnChooseFont ));
	Connect( CD_FONT_SYMBOL, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( FontPanel::OnSymbolFontSize ));

	Connect( CD_PDFFONT_DEFAULT, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FontPanel::OnChooseFont ));
	Connect( CD_PDFFONT_TABLE_HEADER, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FontPanel::OnChooseFont ));
	Connect( CD_PDFFONT_GRAPHIC, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FontPanel::OnChooseFont ));
	Connect( CD_PDFFONT_SYMBOL, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( FontPanel::OnSymbolFontSize ));

	Connect( CD_SET_DEFAULTS, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FontPanel::OnSetDefaults ));
}

/*****************************************************
**
**   FontPanel   ---   setData
**
******************************************************/
void FontPanel::setData()
{
	FontConfig *f = FontConfig::get();
	setTextControlItem( t_font_default, f->getDefaultFont() );
	setTextControlItem( t_font_table_header, f->getHeaderFont() );
	setTextControlItem( t_font_graphic, f->getGraphicFont() );
	setTextControlItem( t_font_text, f->getTextFont() );

	choice_symbol_font->SetSelection( config->symbolFontSize );
	text_symbol->SetValue( wxT( "ABC abc" ));

	setTextControlItem( t_font_pdfdefault, f->getPdfDefaultFont() );
	setTextControlItem( t_font_pdftable_header, f->getPdfHeaderFont() );
	setTextControlItem( t_font_pdfgraphic, f->getPdfGraphicFont() );

	choice_pdfsymbol_font->SetSelection( config->pdfSymbolFontSize );
	text_pdfsymbol->SetValue( wxT( "ABC abc" ));

	setSymbolFontText();
}

/*****************************************************
**
**   FontPanel   ---   setTextControlItem
**
******************************************************/
void FontPanel::setTextControlItem( wxTextCtrl *ctrl, wxFont *font )
{
	ctrl->SetFont( *font );
	ctrl->SetValue( font->GetFaceName() );
	ctrl->Refresh();
}

/*****************************************************
**
**   FontPanel   ---   saveData
**
******************************************************/
bool FontPanel::saveData()
{
	FontConfig *f = FontConfig::get();
	f->setFont( FONT_DEFAULT, t_font_default->GetFont() );
	f->setFont( FONT_HEADER, t_font_table_header->GetFont() );
	f->setFont( FONT_GRAPHIC, t_font_graphic->GetFont() );
	f->setFont( FONT_TEXT, t_font_text->GetFont() );
	f->setFont( FONT_PDFDEFAULT, t_font_pdfdefault->GetFont() );
	f->setFont( FONT_PDFHEADER, t_font_pdftable_header->GetFont() );
	f->setFont( FONT_PDFGRAPHIC, t_font_pdfgraphic->GetFont() );
	config->symbolFontSize = choice_symbol_font->GetSelection();
	config->pdfSymbolFontSize = choice_pdfsymbol_font->GetSelection();
	return true;
}

/*****************************************************
**
**   FontPanel   ---   setSymbolFontText
**
******************************************************/
void FontPanel::setSymbolFontText()
{
	FontConfig *f = FontConfig::get();
	int sel = choice_symbol_font->GetSelection();
	text_symbol->SetFont( *f->getSymbolFont( 100, sel ));
	text_symbol->Refresh();

	sel = choice_pdfsymbol_font->GetSelection();
	text_pdfsymbol->SetFont( *f->getPdfSymbolFont( 100, sel ));
	text_pdfsymbol->Refresh();
}

/*****************************************************
**
**   FontPanel   ---   OnSymbolFontSize
**
******************************************************/
void FontPanel::OnSymbolFontSize( wxCommandEvent &event )
{
	setSymbolFontText();
}

/*****************************************************
**
**   FontPanel   ---   OnChooseFont
**
******************************************************/
void FontPanel::OnChooseFont( wxCommandEvent &event )
{
	wxTextCtrl *t = 0;
	switch ( event.GetId() )
	{
	case CD_FONT_DEFAULT:
		t = t_font_default;
	break;
	case CD_FONT_TABLE_HEADER:
		t = t_font_table_header;
	break;
	case CD_FONT_GRAPHIC:
		t = t_font_graphic;
	break;
	case CD_FONT_TEXT:
		t = t_font_text;
	break;
	case CD_PDFFONT_DEFAULT:
		t = t_font_pdfdefault;
	break;
	case CD_PDFFONT_TABLE_HEADER:
		t = t_font_pdftable_header;
	break;
	case CD_PDFFONT_GRAPHIC:
		t = t_font_pdfgraphic;
	break;
	default:
		assert( false );
		break;
	}
	assert( t );

	wxFontData data;
	data.SetInitialFont( t->GetFont());

/*
#if defined(__WXMAC__)
	if ( event.GetId() == CD_PDFFONT_DEFAULT || event.GetId() == CD_PDFFONT_TABLE_HEADER || event.GetId() == CD_PDFFONT_GRAPHIC )
	{
		doNotShowAgainMessage( this, DO_NOT_SHOW_AGAIN_WARNING_DFONTS );
	}
#endif
*/

	wxFontDialog dialog( this, data );
	if ( dialog.ShowModal() == wxID_OK )
	{
		wxFontData retData = dialog.GetFontData();
		t->SetFont( retData.GetChosenFont() );
		t->SetValue( retData.GetChosenFont().GetFaceName() );
	}
}

/*****************************************************
**
**   FontPanel   ---   OnSetDefaults
**
******************************************************/
void FontPanel::OnSetDefaults( wxCommandEvent &event )
{
	FontConfig *f = FontConfig::get();
	setTextControlItem( t_font_default, f->getFallbackFont( FONT_DEFAULT ) );
	setTextControlItem( t_font_table_header, f->getFallbackFont( FONT_HEADER ));
	setTextControlItem( t_font_graphic, f->getFallbackFont( FONT_GRAPHIC ));
	setTextControlItem( t_font_text, f->getFallbackFont( FONT_TEXT ));

	setTextControlItem( t_font_pdfdefault, f->getFallbackFont( FONT_PDFDEFAULT ) );
	setTextControlItem( t_font_pdftable_header, f->getFallbackFont( FONT_PDFHEADER ));
	setTextControlItem( t_font_pdfgraphic, f->getFallbackFont( FONT_PDFGRAPHIC ));

	choice_symbol_font->SetSelection( 1 );
	choice_pdfsymbol_font->SetSelection( 1 );
	setSymbolFontText();
}

/*****************************************************
**
**   FontPanel   ---   chooseFont
**
******************************************************/
/*
void FontPanel::chooseFont( wxTextCtrl *t )
{
	wxFontData data;
	data.SetInitialFont( t->GetFont());

	// TODO Mac OS und id des Buttons
enum { CD_FONT_DEFAULT = wxID_HIGHEST + 1, CD_FONT_TABLE_HEADER, CD_FONT_GRAPHIC, CD_FONT_TEXT, CD_FONT_SYMBOL, 
	CD_PDFFONT_DEFAULT, CD_PDFFONT_TABLE_HEADER, CD_PDFFONT_GRAPHIC, CD_PDFFONT_SYMBOL, CD_SET_DEFAULTS };
	if ( true )
	{
		doNotShowAgainMessage( this, DO_NOT_SHOW_AGAIN_WARNING_DFONTS );
	}

	wxFontDialog dialog( this, data );
	if ( dialog.ShowModal() == wxID_OK )
	{
		wxFontData retData = dialog.GetFontData();
		t->SetFont( retData.GetChosenFont() );
		t->SetValue( retData.GetChosenFont().GetFaceName() );
	}
}
*/

/*****************************************************
**
**   FontPanel   ---   set_properties
**
******************************************************/
void FontPanel::set_properties()
{
    // begin wxGlade: FontPanel::set_properties
    t_font_default->SetMinSize(wxSize(300, 30));
    choice_symbol_font->SetSelection(0);
    t_font_pdfdefault->SetMinSize(wxSize(300, 30));
    choice_pdfsymbol_font->SetSelection(0);
    button_defaults->SetToolTip(_("Restore default settings"));
    // end wxGlade
}

/*****************************************************
**
**   FontPanel   ---   do_layout
**
******************************************************/
void FontPanel::do_layout()
{
    // begin wxGlade: FontPanel::do_layout
    wxFlexGridSizer* sizer_main = new wxFlexGridSizer(1, 1, 0, 0);
    wxFlexGridSizer* sizer_left = new wxFlexGridSizer(3, 1, 0, 0);
    wxStaticBoxSizer* sizer_pdffonts = new wxStaticBoxSizer(sizer_pdffonts_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_pdffonts = new wxFlexGridSizer(4, 3, 3, 3);
    wxStaticBoxSizer* sizer_fonts = new wxStaticBoxSizer(sizer_fonts_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_fonts = new wxFlexGridSizer(5, 3, 3, 3);
    grid_fonts->Add(label_default, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_fonts->Add(t_font_default, 0, wxALL|wxEXPAND, 3);
    grid_fonts->Add(button_default, 0, wxALL, 3);
    grid_fonts->Add(label_header, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_fonts->Add(t_font_table_header, 0, wxALL|wxEXPAND, 3);
    grid_fonts->Add(button_table_header, 0, wxALL, 3);
    grid_fonts->Add(label_graphic, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_fonts->Add(t_font_graphic, 0, wxALL|wxEXPAND, 3);
    grid_fonts->Add(button_graphic, 0, wxALL, 3);
    grid_fonts->Add(label_text, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_fonts->Add(t_font_text, 0, wxALL|wxEXPAND, 3);
    grid_fonts->Add(button_text, 0, wxALL, 3);
    grid_fonts->Add(label_symbol, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_fonts->Add(text_symbol, 0, wxALL|wxEXPAND, 3);
    grid_fonts->Add(choice_symbol_font, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_fonts->AddGrowableCol(1);
    sizer_fonts->Add(grid_fonts, 1, wxEXPAND, 0);
    sizer_left->Add(sizer_fonts, 1, wxALL|wxEXPAND, 3);
    grid_pdffonts->Add(label_pdfdefault, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_pdffonts->Add(t_font_pdfdefault, 0, wxALL|wxEXPAND, 3);
    grid_pdffonts->Add(button_pdfdefault, 0, wxALL, 3);
    grid_pdffonts->Add(label_pdfheader, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_pdffonts->Add(t_font_pdftable_header, 0, wxALL|wxEXPAND, 3);
    grid_pdffonts->Add(button_pdftable_header, 0, wxALL, 3);
    grid_pdffonts->Add(label_pdfgraphic, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_pdffonts->Add(t_font_pdfgraphic, 0, wxALL|wxEXPAND, 3);
    grid_pdffonts->Add(button_pdfgraphic, 0, wxALL, 3);
    grid_pdffonts->Add(label_pdfsymbol, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_pdffonts->Add(text_pdfsymbol, 0, wxALL|wxEXPAND, 3);
    grid_pdffonts->Add(choice_pdfsymbol_font, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_pdffonts->AddGrowableCol(1);
    sizer_pdffonts->Add(grid_pdffonts, 1, wxEXPAND, 0);
    sizer_left->Add(sizer_pdffonts, 1, wxALL|wxEXPAND, 3);
    sizer_left->Add(button_defaults, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 3);
    sizer_left->AddGrowableCol(0);
    sizer_main->Add(sizer_left, 1, wxALL, 3);
    SetSizer(sizer_main);
    sizer_main->Fit(this);
    sizer_main->AddGrowableRow(0);
    sizer_main->AddGrowableCol(0);
    // end wxGlade
}

/*****************************************************
**
**   ConfigPanelFactory   ---   createFontPanel
**
******************************************************/
ConfigPanel *ConfigPanelFactory::createFontPanel( wxWindow *parent )
{
	return new FontPanel( parent );
}


