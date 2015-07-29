/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/ObjectColorPanel.cpp
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
#pragma implementation "ObjectColorPanel.h"
#endif

#include "ObjectColorPanel.h"

// begin wxGlade: ::extracode

// end wxGlade

#include <wx/button.h>
#include <wx/choice.h>
#include <wx/colour.h>
#include <wx/clrpicker.h>
#include <wx/dc.h>
#include <wx/dcclient.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/stattext.h>

#include "ColorConfig.h"
#include "Conf.h"
#include "Lang.h"
#include "mathbase.h"
#include "Painter.h"

extern Config *config;

IMPLEMENT_CLASS( ObjectColorPanel, ConfigPanel )

enum { CD_SET_DEFAULTS = wxID_HIGHEST, CD_SIGNS, CD_HOUSES, 
	CD_PLANET_FGCOLOR, CD_SIGN_FGCOLOR, CD_SIGN_BGCOLOR, CD_HOUSE_FGCOLOR, CD_HOUSE_BGCOLOR
};

/*************************************************//**
*
* 
*
******************************************************/
class ObjectColorPreviewPanel : public wxPanel
{
public:
	wxColour fgcolor, bgcolor;
	int item;

protected:
	ObjectColorPreviewPanel( wxWindow* parent )
		: wxPanel( parent, -1, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE )
		{
			item = 0;
			Connect( wxEVT_PAINT, wxPaintEventHandler( ObjectColorPreviewPanel::OnPaint ));
		}

	virtual void OnPaint( wxPaintEvent &event )
	{
		wxCoord h, w;
		wxPaintDC context( this );
		PrepareDC( context );

		Painter *painter = new DcPainter( &context );
		painter->setBrush( bgcolor );
		painter->setPen( bgcolor );

		GetSize( &w, &h );
		MyRect rect ( 0, 0, w, h );
		painter->drawRectangle( rect );

		painter->setTextColor( fgcolor );
		paintInternal( painter, rect );
		delete painter;
	}

	virtual void paintInternal( Painter *painter, const MyRect &rect ) = 0;

	Lang lang;
};

/*************************************************//**
*
* 
*
******************************************************/
class PlanetColorPreviewPanel : public ObjectColorPreviewPanel
{
public:
	PlanetColorPreviewPanel( wxWindow* parent ) : ObjectColorPreviewPanel( parent ) {}

protected:
	virtual void paintInternal( Painter *painter, const MyRect &rect )
	{
		ASSERT_VALID_EPHEM_OBJECT( item );
		painter->setSymbolFont();
		painter->drawTextFormatted( rect, lang.getPlanetSymbolCode( item ), Align::Center );
	}
};

/*************************************************//**
*
* 
*
******************************************************/
class HouseFgColorPreviewPanel : public ObjectColorPreviewPanel
{
public:
	HouseFgColorPreviewPanel( wxWindow* parent ) : ObjectColorPreviewPanel( parent ) {}

protected:
	virtual void paintInternal( Painter *painter, const MyRect &rect )
	{
		assert( item >= 0 && item < 4 );
		painter->setGraphicFont();
		painter->drawTextFormatted( rect, wxString::Format( wxT( "%d %d %d" ), item + 1, item + 5, item + 9 ), Align::Center );
	}
};

/*************************************************//**
*
* 
*
******************************************************/
class HouseBgColorPreviewPanel : public ObjectColorPreviewPanel
{
public:
	HouseBgColorPreviewPanel( wxWindow* parent ) : ObjectColorPreviewPanel( parent ) {}

protected:
	virtual void paintInternal( Painter *painter, const MyRect &rect )
	{
		assert( item >= 0 && item < 4 );
		painter->setGraphicFont();
		painter->drawTextFormatted( rect, wxString::Format( wxT( "%d %d %d" ), item + 1, item + 5, item + 9 ), Align::Center );
	}
};

/*************************************************//**
*
* 
*
******************************************************/
class SignFgColorPreviewPanel : public ObjectColorPreviewPanel
{
public:
	SignFgColorPreviewPanel( wxWindow* parent ) : ObjectColorPreviewPanel( parent ) {}

protected:
	virtual void paintInternal( Painter *painter, const MyRect &rect )
	{
		assert( item >= 0 && item < 4 );
		painter->setSymbolFont();
		wxString s;
		s << lang.getSignSymbolCode( item ) << wxT( " " )
			<< lang.getSignSymbolCode( item + 4 ) << wxT( " " )
			<< lang.getSignSymbolCode( item + 8 );
		painter->drawTextFormatted( rect, s, Align::Center );
	}
};

/*************************************************//**
*
* 
*
******************************************************/
class SignBgColorPreviewPanel : public ObjectColorPreviewPanel
{
public:
	SignBgColorPreviewPanel( wxWindow* parent ) : ObjectColorPreviewPanel( parent ) {}

protected:
	virtual void paintInternal( Painter *painter, const MyRect &rect )
	{
		assert( item >= 0 && item < 4 );
		painter->setSymbolFont();
		wxString s;
		s << lang.getSignSymbolCode( item ) << wxT( " " )
			<< lang.getSignSymbolCode( item + 4 ) << wxT( " " )
			<< lang.getSignSymbolCode( item + 8 );
		painter->drawTextFormatted( rect, s, Align::Center );
	}
};

/*****************************************************
**
**   ObjectColorPanel   ---   Constructor
**
******************************************************/
ObjectColorPanel::ObjectColorPanel( wxWindow* parent )
		: ConfigPanel( parent )
{
    // begin wxGlade: ObjectColorPanel::ObjectColorPanel
    sizer_signs_staticbox = new wxStaticBox(this, -1, _("Signs"));
    sizer_houses_staticbox = new wxStaticBox(this, -1, _("Houses"));
    sizer_planets_staticbox = new wxStaticBox(this, -1, _("Planets"));
    const wxString choice_planets_choices[] = {
        _("dummy"),
        _("dummy"),
        _("dummy"),
        _("dummy"),
        _("dummy")
    };
    choice_planets = new wxChoice(this, CD_PLANET_FGCOLOR, wxDefaultPosition, wxDefaultSize, 5, choice_planets_choices, 0);
    label_fgplanets = new wxStaticText(this, wxID_ANY, _("Foreground"));
    color_fgplanets = new wxColourPickerCtrl(this, -1);
    preview_planetcolor = new PlanetColorPreviewPanel(this);
    const wxString choice_signs_choices[] = {
        _("Fire"),
        _("Earth"),
        _("Air"),
        _("Water")
    };
    choice_signs = new wxChoice(this, CD_SIGNS, wxDefaultPosition, wxDefaultSize, 4, choice_signs_choices, 0);
    label_fg_signs = new wxStaticText(this, wxID_ANY, _("Foreground"));
    color_fgsigns = new wxColourPickerCtrl(this, CD_SIGN_FGCOLOR);
    preview_signfgcolor = new SignFgColorPreviewPanel(this);
    label_bg_signs = new wxStaticText(this, wxID_ANY, _("Background"));
    color_bgsigns = new wxColourPickerCtrl(this, CD_SIGN_BGCOLOR);
    preview_signbgcolor = new SignBgColorPreviewPanel(this);
    const wxString choice_houses_choices[] = {
        _("Fire"),
        _("Earth"),
        _("Air"),
        _("Water")
    };
    choice_houses = new wxChoice(this, CD_HOUSES, wxDefaultPosition, wxDefaultSize, 4, choice_houses_choices, 0);
    label_fg_houses = new wxStaticText(this, wxID_ANY, _("Foreground"));
    color_fghouses = new wxColourPickerCtrl(this, CD_HOUSE_FGCOLOR);
    preview_housefgcolor = new HouseFgColorPreviewPanel(this);
    label_bg_houses = new wxStaticText(this, wxID_ANY, _("Background"));
    color_bghouses = new wxColourPickerCtrl(this, CD_HOUSE_BGCOLOR);
    preview_housebgcolor = new HouseBgColorPreviewPanel(this);
    button_default = new wxButton(this, CD_SET_DEFAULTS, _("Restore Defaults"));

    set_properties();
    do_layout();
    // end wxGlade

	Connect( wxID_ANY, wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( ObjectColorPanel::OnColorChanged ));
	Connect( CD_SET_DEFAULTS, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ObjectColorPanel::OnRestoreDefaults ));
	Connect( wxID_ANY, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ObjectColorPanel::OnChoice ));
}

/*****************************************************
**
**   ObjectColorPanel   ---   setData
**
******************************************************/
void ObjectColorPanel::setData()
{
	printf( "ObjectColorPanel::setData\n" );
	colors = config->colors->chart;
	updateColorPickers();
	repaintPreviewPanels();
}

/*****************************************************
**
**   ObjectColorPanel   ---   saveData
***
******************************************************/
bool ObjectColorPanel::saveData()
{
	config->colors->chart = colors;
	return true;
}

/*****************************************************
**
**   ObjectColorPanel   ---   planetChoiceToIndex
***
******************************************************/
int ObjectColorPanel::planetChoiceToIndex( const int &selection )
{
	 int index = selection;

	 //OMEANNODE, OTRUENODE, OMEANDESCNODE, OTRUEDESCNODE, // 10 ... 13
	// OMEANNODE and OMEANDESCNODE ) are missing
	if ( selection == OTRUENODE ) index++;
	else if ( selection >= OMEANDESCNODE ) index += 2;
	return index;
}

/*****************************************************
**
**   ObjectColorPanel   ---   repaintPreviewPanels
***
******************************************************/
void ObjectColorPanel::repaintPreviewPanels()
{
	const int selplanet = planetChoiceToIndex( choice_planets->GetSelection());
	preview_planetcolor->item = selplanet;
	preview_planetcolor->fgcolor = colors.planetFgColor[selplanet];
	preview_planetcolor->bgcolor = config->colors->bgColor;
	preview_planetcolor->Refresh();

	const int selsign = choice_signs->GetSelection();
	assert( selsign >= 0 && selsign < 4 );

	preview_signfgcolor->item = selsign;
	preview_signfgcolor->fgcolor = colors.signFgColor[selsign];
	preview_signfgcolor->bgcolor = config->colors->bgColor;
	preview_signfgcolor->Refresh();

	preview_signbgcolor->item = selsign;
	preview_signbgcolor->fgcolor = config->colors->fgColor;
	preview_signbgcolor->bgcolor = colors.signBgColor[selsign];
	preview_signbgcolor->Refresh();

	const int selhouse = choice_houses->GetSelection();
	assert( selhouse >= 0 && selhouse < 4 );
	preview_housefgcolor->item = selhouse;
	preview_housefgcolor->bgcolor = config->colors->bgColor;
	preview_housefgcolor->fgcolor = colors.houseFgColor[selhouse];
	preview_housefgcolor->Refresh();

	preview_housebgcolor->item = selhouse;
	preview_housebgcolor->fgcolor = config->colors->fgColor;
	preview_housebgcolor->bgcolor = colors.houseBgColor[selhouse];
	preview_housebgcolor->Refresh();
}

/*****************************************************
**
**   ObjectColorPanel   ---   OnColorChanged
***
******************************************************/
void ObjectColorPanel::OnColorChanged( wxColourPickerEvent& )
{
	const int selplanet = choice_planets->GetSelection();
	assert( selplanet >= 0 && selplanet < MAX_EPHEM_OBJECTS - 2 );
	const int pindex = planetChoiceToIndex( selplanet );
	colors.planetFgColor[pindex] = color_fgplanets->GetColour();

	// nodes are pair each
	if ( pindex == OTRUENODE )
	{
		colors.planetFgColor[OMEANNODE] = color_fgplanets->GetColour();
	}
	else if ( pindex == OTRUEDESCNODE )
	{
		colors.planetFgColor[OMEANDESCNODE] = color_fgplanets->GetColour();
	}

	const int selsign = choice_signs->GetSelection();
	assert( selsign >= 0 && selsign < 4 );
	colors.signFgColor[selsign] = color_fgsigns->GetColour();
	colors.signBgColor[selsign] = color_bgsigns->GetColour();

	const int selhouse = choice_houses->GetSelection();
	assert( selhouse >= 0 && selhouse < 4 );
	colors.houseFgColor[selhouse] = color_fghouses->GetColour();
	colors.houseBgColor[selhouse] = color_bghouses->GetColour();

	repaintPreviewPanels();
}

/*****************************************************
**
**   ObjectColorPanel   ---   OnChoice
***
******************************************************/
void ObjectColorPanel::OnChoice( wxCommandEvent& )
{
	updateColorPickers();
	repaintPreviewPanels();
}

/*****************************************************
**
**   ObjectColorPanel   ---   updateColorPickers
***
******************************************************/
void ObjectColorPanel::updateColorPickers()
{
	const int selplanet = choice_planets->GetSelection();
	assert( selplanet >= 0 && selplanet < MAX_EPHEM_OBJECTS - 2 );
	color_fgplanets->SetColour( colors.planetFgColor[selplanet] );

	const int selsign = choice_signs->GetSelection();
	assert( selsign >= 0 && selsign < 4 );
	color_fgsigns->SetColour( colors.signFgColor[selsign] );
	color_bgsigns->SetColour( colors.signBgColor[selsign] );

	const int selhouse = choice_houses->GetSelection();
	assert( selhouse >= 0 && selhouse < 4 );
	color_fghouses->SetColour( colors.houseFgColor[selhouse] );
	color_bghouses->SetColour( colors.houseBgColor[selhouse] );

	repaintPreviewPanels();
}

/*****************************************************
**
**   ObjectColorPanel   ---   OnRestoreDefaults
***
******************************************************/
void ObjectColorPanel::OnRestoreDefaults( wxCommandEvent& )
{
	colors = ChartColorConfig();
	updateColorPickers();
	repaintPreviewPanels();
}

/*****************************************************
**
**   ObjectColorPanel   ---   set_properties
***
******************************************************/
void ObjectColorPanel::set_properties()
{
	Lang lang;
	choice_planets->Clear();

	for ( int i = 0; i < MAX_EPHEM_OBJECTS; i++ )
	{
		if ( i == OMEANNODE || i == OMEANDESCNODE ) continue;
		choice_planets->Append( lang.getObjectName( i, TLARGE ));
	}

    // begin wxGlade: ObjectColorPanel::set_properties
    choice_planets->SetSelection(0);
    choice_signs->SetSelection(0);
    choice_houses->SetSelection(0);
    button_default->SetToolTip(_("Restore default settings"));
    // end wxGlade
}

/*****************************************************
**
**   ObjectColorPanel   ---   do_layout
***
******************************************************/
void ObjectColorPanel::do_layout()
{
    // begin wxGlade: ObjectColorPanel::do_layout
    wxFlexGridSizer* sizer_main = new wxFlexGridSizer(1, 2, 0, 0);
    wxFlexGridSizer* sizer_left = new wxFlexGridSizer(3, 1, 0, 0);
    wxStaticBoxSizer* sizer_houses = new wxStaticBoxSizer(sizer_houses_staticbox, wxHORIZONTAL);
    wxFlexGridSizer* grid_houses = new wxFlexGridSizer(2, 3, 3, 3);
    wxStaticBoxSizer* sizer_signs = new wxStaticBoxSizer(sizer_signs_staticbox, wxHORIZONTAL);
    wxFlexGridSizer* grid_signs = new wxFlexGridSizer(2, 3, 3, 3);
    wxStaticBoxSizer* sizer_planets = new wxStaticBoxSizer(sizer_planets_staticbox, wxHORIZONTAL);
    sizer_planets->Add(choice_planets, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    sizer_planets->Add(label_fgplanets, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    sizer_planets->Add(color_fgplanets, 1, wxALL|wxEXPAND, 3);
    sizer_planets->Add(preview_planetcolor, 1, wxALL|wxEXPAND, 3);
    sizer_left->Add(sizer_planets, 1, wxALL|wxEXPAND, 3);
    sizer_signs->Add(choice_signs, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_signs->Add(label_fg_signs, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_signs->Add(color_fgsigns, 1, wxALL|wxEXPAND, 3);
    grid_signs->Add(preview_signfgcolor, 1, wxALL|wxEXPAND, 3);
    grid_signs->Add(label_bg_signs, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_signs->Add(color_bgsigns, 1, wxALL|wxEXPAND, 3);
    grid_signs->Add(preview_signbgcolor, 1, wxALL|wxEXPAND, 3);
    grid_signs->AddGrowableCol(2);
    sizer_signs->Add(grid_signs, 1, wxEXPAND, 0);
    sizer_left->Add(sizer_signs, 1, wxEXPAND, 0);
    sizer_houses->Add(choice_houses, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_houses->Add(label_fg_houses, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_houses->Add(color_fghouses, 1, wxALL|wxEXPAND, 3);
    grid_houses->Add(preview_housefgcolor, 1, wxALL|wxEXPAND, 3);
    grid_houses->Add(label_bg_houses, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_houses->Add(color_bghouses, 1, wxALL|wxEXPAND, 3);
    grid_houses->Add(preview_housebgcolor, 1, wxALL|wxEXPAND, 3);
    grid_houses->AddGrowableCol(2);
    sizer_houses->Add(grid_houses, 1, wxEXPAND, 0);
    sizer_left->Add(sizer_houses, 1, wxEXPAND, 0);
    sizer_left->Add(button_default, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 3);
    sizer_left->AddGrowableCol(0);
    sizer_main->Add(sizer_left, 1, wxALL, 3);
    SetSizer(sizer_main);
    sizer_main->Fit(this);
    // end wxGlade
}

/*****************************************************
**
**   ConfigPanelFactory   ---   createObjectColorPanel
**
******************************************************/
ConfigPanel *ConfigPanelFactory:: createObjectColorPanel( wxWindow *parent )
{
	return new ObjectColorPanel( parent );
}

