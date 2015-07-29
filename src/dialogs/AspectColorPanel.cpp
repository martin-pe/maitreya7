/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/AspectColorPanel.cpp
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
#pragma implementation "AspectColorPanel.h"
#endif

#include "AspectColorPanel.h"

#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/clrpicker.h>
#include <wx/dc.h>
#include <wx/dcclient.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

#include "ColorConfig.h"
#include "Conf.h"
#include "mathbase.h"
#include "Painter.h"

extern Config *config;

BEGIN_DECLARE_EVENT_TYPES()
DECLARE_EVENT_TYPE(COMMAND_SELECTED_ASPECT_CHANGED, wxID_HIGHEST + 9002 )
END_DECLARE_EVENT_TYPES()

DEFINE_EVENT_TYPE( COMMAND_SELECTED_ASPECT_CHANGED )

enum { CD_STYLE_CHOICE = wxID_HIGHEST + 1, CD_COLORPICKER, CD_SPIN_WIDTH, CD_LINESTYLE, CD_SPIN_ORBIS, CD_DEFAULT, CD_PREVIEW_PANEL };

IMPLEMENT_CLASS( AspectPreviewPanel, wxPanel )

IMPLEMENT_CLASS( AspectColorPanel, ConfigPanel )

/*****************************************************
**
**   AspectPreviewPanel   ---   Constructor
**
******************************************************/
AspectPreviewPanel::AspectPreviewPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
		: wxPanel( parent, id, pos, size, wxTAB_TRAVERSAL | wxBORDER_SUNKEN )
{
	showSymbols = true;
	selected = 0;

	Connect( wxEVT_PAINT, wxPaintEventHandler( AspectPreviewPanel::OnPaint ));
	Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( AspectPreviewPanel::OnMouseLeftDown ));
}

/*****************************************************
**
**   AspectPreviewPanel   ---   drawSingleAspect
**
******************************************************/
void AspectPreviewPanel::drawSingleAspect( Painter *painter, const int &aspectindex, const int &xleft, const int &xright, const int &y )
{
	wxString s;
	const int xh = 15;
	const int w = xright - xleft;

	painter->setPen( wxPen( configs[aspectindex]->color, configs[aspectindex]->width, convertInt2WxPen( configs[aspectindex]->style )));
	painter->setTextColor( configs[aspectindex]->color );

	if ( showSymbols )
	{
		painter->drawLine( xleft, y, xleft + w/2 - xh, y );
		painter->drawLine( xleft + w/2 + xh, y, xright, y );
		s = AspectExpert::getAspectSymbol( aspectindex );

		if ( s.IsEmpty())
		{
			painter->setGraphicFont();
			painter->drawTextFormatted( xleft + w/2 - xh, y - xh, 2 * xh, 2 * xh, AspectExpert::getAspectShortDescription( aspectindex ), Align::Center );
		}
		else
		{
			painter->setSymbolFont();
			painter->drawTextFormatted( xleft + w/2 - xh, y - xh, 2 * xh, 2 * xh, s, Align::Center );
		}
	}
	else painter->drawLine( xleft, y, xright, y );
}


/*****************************************************
**
**   AspectPreviewPanel   ---   OnPaint
**
******************************************************/
void AspectPreviewPanel::OnPaint( wxPaintEvent &event )
{
	wxCoord w, h;
	const int xborder = 5;
	//const int xh = 15;
	wxString s;
	const int selborder = 2;
	const int borderwidth = 1;

	wxPaintDC dc( this );
	PrepareDC( dc );
	DcPainter painter( &dc );

	GetSize( &w, &h  );

	painter.setDefaultBrush();
	painter.setPen( wxPen( config->colors->fgColor ));
	painter.drawRectangle( 0, 0, w, h );

	const int imed = ( MAX_ASPECT_TYPES + 1 ) / 2;
	const int steplen = h / imed;
	painter.drawLine( w / 2 , 0, w / 2, h );

	const int col2 = ( selected >= imed );
	const int sely = selected - col2 * imed;
	painter.setTransparentBrush();

	painter.setPen( wxPen( *wxBLUE, borderwidth, wxDOT ));

	painter.drawRectangle( selborder + col2 * w/2, selborder + sely * steplen, w/2 - 2 * selborder, steplen - 2 * selborder);

	for ( int i = 0; i < imed; i++ )
	{
		drawSingleAspect( &painter, i, xborder, w/2 - xborder, steplen / 2 + i * steplen );
	}
	for ( int i = imed; i < MAX_ASPECT_TYPES; i++ )
	{
		drawSingleAspect( &painter, i, w/2 + xborder, w - xborder, steplen / 2 + ( i - imed ) * steplen );
	}
}

/*****************************************************
**
**   AspectPreviewPanel   ---   setConfigs
**
******************************************************/
void AspectPreviewPanel::setConfig( AspectConfig *cfg, const int &i )
{
	assert( cfg );
	assert( i >= 0 && i < MAX_ASPECT_TYPES );
	configs[i] = cfg;
}

/**************************************************************
***
**   AspectPreviewPanel   ---   OnMouseLeftDown
***
***************************************************************/
void AspectPreviewPanel::OnMouseLeftDown( wxMouseEvent &event )
{
	wxCoord w, h;
	GetSize( &w, &h );
	bool rightside = ( event.m_x > w / 2 );

	const int imed = ( MAX_ASPECT_TYPES + 1 ) / 2;
	const int steplen = h / imed;

	selected = event.m_y / steplen;
	if ( rightside ) selected += imed;

	if ( selected >= MAX_ASPECT_TYPES ) selected = MAX_ASPECT_TYPES - 1;
	if ( selected < 0 ) selected = 0;

	wxCommandEvent e( COMMAND_SELECTED_ASPECT_CHANGED, GetId() );
	e.SetInt( selected );
	wxPostEvent( GetParent(), e );

	Refresh();
}

/*****************************************************
**
**   AspectColorPanel   ---   Constructor
**
******************************************************/
AspectColorPanel::AspectColorPanel(wxWindow* parent)
		:  ConfigPanel( parent )
{
    // begin wxGlade: AspectColorPanel::AspectColorPanel
    sizer_preview_staticbox = new wxStaticBox(this, -1, _("Preview"));
    sizer_styles_staticbox = new wxStaticBox(this, -1, _("Styles"));
    const wxString choice_aspect_type_choices[] = {
        _("dummy"),
        _("dummy"),
        _("dummy"),
        _("dummy"),
        _("dummy")
    };
    choice_aspect_type = new wxChoice(this, CD_STYLE_CHOICE, wxDefaultPosition, wxDefaultSize, 5, choice_aspect_type_choices, 0);
    label_color = new wxStaticText(this, wxID_ANY, _("Color"));
    button_color = new wxColourPickerCtrl(this, CD_COLORPICKER);
    label_width = new wxStaticText(this, wxID_ANY, _("Width"));
    spin_width = new wxSpinCtrl(this, CD_SPIN_WIDTH, wxT(""), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 10);
    label_linestyle = new wxStaticText(this, wxID_ANY, _("Line Style"));
    const wxString choice_linestyle_choices[] = {
        _("Solid"),
        _("Dot"),
        _("Long Dash"),
        _("Short Dash"),
        _("Dot Dash")
    };
    choice_linestyle = new wxChoice(this, CD_LINESTYLE, wxDefaultPosition, wxDefaultSize, 5, choice_linestyle_choices, 0);
    button_default = new wxButton(this, CD_DEFAULT, _("Restore Defaults"));
    preview_panel = new AspectPreviewPanel(this, CD_PREVIEW_PANEL);

    set_properties();
    do_layout();
    // end wxGlade

	Connect( CD_STYLE_CHOICE, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( AspectColorPanel::OnAspectChoice ));
	Connect( CD_LINESTYLE, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( AspectColorPanel::OnLineStyleChoice ));
	Connect( CD_DEFAULT, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AspectColorPanel::OnDefaultButton ));

	Connect( CD_COLORPICKER, wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( AspectColorPanel::OnColorButton ));

	Connect( CD_SPIN_WIDTH, wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( AspectColorPanel::OnSpinWidth ));
	Connect( CD_PREVIEW_PANEL, COMMAND_SELECTED_ASPECT_CHANGED, wxCommandEventHandler( AspectColorPanel::OnSelectedAspectChanged ));
}

/*****************************************************
**
**   AspectColorPanel   ---   setData
**
******************************************************/
void AspectColorPanel::setData()
{
	AspectExpert aexpert;
	AspectConfigLoader *loader = AspectConfigLoader::get();

	for ( int i = 0; i < MAX_ASPECT_TYPES; i++ )
	{
		configs[i] = *loader->getAspectConfig( i );
	}

	selconfig = choice_aspect_type->GetSelection();
	if ( selconfig == -1 ) selconfig = 0;
	choice_aspect_type->Clear();
	for ( int i = 0; i < MAX_ASPECT_TYPES; i++ ) choice_aspect_type->Append( AspectExpert::getAspectName( i ));
	choice_aspect_type->SetSelection( selconfig );
	updateControls();
}

/*****************************************************
**
**   AspectColorPanel   ---   updateControls
**
******************************************************/
void AspectColorPanel::updateControls()
{
	ASSERT_VALID_ASPECT( selconfig );
	for ( int i = 0; i < MAX_ASPECT_TYPES; i++ )
	{
		preview_panel->setConfig( &configs[i], i );
	}
	button_color->SetColour( configs[selconfig].color );
	spin_width->SetValue( configs[selconfig].width );
	choice_linestyle->SetSelection( configs[selconfig].style );
	preview_panel->Refresh();
}

/*****************************************************
**
**   AspectColorPanel   ---   saveData
**
******************************************************/
bool AspectColorPanel::saveData()
{
	AspectConfigLoader *loader = AspectConfigLoader::get();
	for ( int i = 0; i < MAX_ASPECT_TYPES; i++ )
	{
		loader->setAspectConfig( configs[i], i );
	}
	return true;
}

/*****************************************************
**
**   AspectColorPanel   ---   OnAspectChoice
**
******************************************************/
void AspectColorPanel::OnAspectChoice( wxCommandEvent&event )
{

	ASSERT_VALID_ASPECT( selconfig );
	configs[selconfig].color = button_color->GetColour();
	configs[selconfig].width = spin_width->GetValue();
	configs[selconfig].style = choice_linestyle->GetSelection();

	selconfig = choice_aspect_type->GetSelection();
	assert( selconfig >= 0 && selconfig < MAX_ASPECT_TYPES );

	button_color->SetColour( configs[selconfig].color );
	spin_width->SetValue( configs[selconfig].width );
	choice_linestyle->SetSelection( configs[selconfig].style );

	preview_panel->setSelected( selconfig);
	preview_panel->Refresh();
}

/**************************************************************
***
**   AspectColorPanel   ---   OnSelectedAspectChanged
***
***************************************************************/
void AspectColorPanel::OnSelectedAspectChanged( wxCommandEvent &event )
{
	ASSERT_VALID_ASPECT( selconfig );
	configs[selconfig].color = button_color->GetColour();
	configs[selconfig].width = spin_width->GetValue();
	configs[selconfig].style = choice_linestyle->GetSelection();

	selconfig = event.GetInt();
	assert( selconfig >= 0 && selconfig < MAX_ASPECT_TYPES );

	button_color->SetColour( configs[selconfig].color );
	spin_width->SetValue( configs[selconfig].width );
	choice_linestyle->SetSelection( configs[selconfig].style );
	preview_panel->Refresh();

	choice_aspect_type->SetSelection( selconfig );
}

/*****************************************************
**
**   AspectColorPanel   ---   OnLineStyleChoice
**
******************************************************/
void AspectColorPanel::OnLineStyleChoice( wxCommandEvent&event )
{
	configs[selconfig].style = choice_linestyle->GetSelection();
	preview_panel->Refresh();
}

/*****************************************************
**
**   AspectColorPanel   ---   OnSpinWidth
**
******************************************************/
void AspectColorPanel::OnSpinWidth( wxSpinEvent &event )
{
	configs[selconfig].width = spin_width->GetValue();
	preview_panel->Refresh();
}

/*****************************************************
**
**   AspectColorPanel   ---   OnColorButton
**
******************************************************/
void AspectColorPanel::OnColorButton( wxColourPickerEvent &event )
{
	configs[selconfig].color = button_color->GetColour();
	preview_panel->Refresh();
}

/*****************************************************
**
**   AspectColorPanel   ---   OnDefaultButton
**
******************************************************/
void AspectColorPanel::OnDefaultButton( wxCommandEvent& )
{
	AspectConfigLoader *loader = AspectConfigLoader::get();
	for ( int i = 0; i < MAX_ASPECT_TYPES; i++ )
	{
		configs[i] = *loader->getDefaultAspectConfig( i );
	}
	updateControls();
}

/*****************************************************
**
**   AspectColorPanel   ---   set_properties
**
******************************************************/
void AspectColorPanel::set_properties()
{
    // begin wxGlade: AspectColorPanel::set_properties
    choice_aspect_type->SetSelection(0);
    button_color->SetMinSize(wxSize(60, 24));
    spin_width->SetMinSize(wxSize(60, 21));
    choice_linestyle->SetSelection(0);
    button_default->SetToolTip(_("Restore default settings"));
    preview_panel->SetMinSize(wxSize(150, 100));
    // end wxGlade
}


/*****************************************************
**
**   AspectColorPanel   ---   do_layout
**
******************************************************/
void AspectColorPanel::do_layout()
{
    // begin wxGlade: AspectColorPanel::do_layout
    wxFlexGridSizer* sizer_main = new wxFlexGridSizer(1, 2, 0, 0);
    wxFlexGridSizer* sizer_right = new wxFlexGridSizer(1, 1, 0, 0);
    wxStaticBoxSizer* sizer_preview = new wxStaticBoxSizer(sizer_preview_staticbox, wxHORIZONTAL);
    wxFlexGridSizer* sizer_left = new wxFlexGridSizer(2, 1, 0, 0);
    wxStaticBoxSizer* sizer_styles = new wxStaticBoxSizer(sizer_styles_staticbox, wxVERTICAL);
    wxGridSizer* grid_styles = new wxGridSizer(3, 2, 3, 3);
    sizer_styles->Add(choice_aspect_type, 0, wxALL, 3);
    grid_styles->Add(label_color, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_styles->Add(button_color, 1, wxALL|wxEXPAND, 3);
    grid_styles->Add(label_width, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_styles->Add(spin_width, 0, wxALL|wxEXPAND, 3);
    grid_styles->Add(label_linestyle, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_styles->Add(choice_linestyle, 0, wxALL|wxEXPAND, 3);
    sizer_styles->Add(grid_styles, 1, 0, 0);
    sizer_left->Add(sizer_styles, 1, wxEXPAND, 0);
    sizer_left->Add(button_default, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
    sizer_main->Add(sizer_left, 1, wxALL|wxEXPAND, 3);
    sizer_preview->Add(preview_panel, 1, wxALL|wxEXPAND, 3);
    sizer_right->Add(sizer_preview, 1, wxALL|wxEXPAND, 3);
    sizer_right->AddGrowableRow(0);
    sizer_right->AddGrowableCol(0);
    sizer_main->Add(sizer_right, 1, wxALL|wxEXPAND, 3);
    SetSizer(sizer_main);
    sizer_main->Fit(this);
    sizer_main->AddGrowableRow(0);
    sizer_main->AddGrowableCol(1);
    // end wxGlade
}

/*****************************************************
**
**   ConfigPanelFactory   ---   createAspectColorPanel
**
******************************************************/
ConfigPanel *ConfigPanelFactory:: createAspectColorPanel( wxWindow *parent )
{
	return new AspectColorPanel( parent );
}


