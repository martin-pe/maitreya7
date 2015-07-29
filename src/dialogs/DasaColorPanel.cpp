/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/DasaColorPanel.cpp
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
#pragma implementation "DasaColorPanel.h"
#endif

#include "DasaColorPanel.h"

#include "ColorConfig.h"
#include "Conf.h"
#include "constants.h"
#include "FileConfig.h"
#include "guibase.h"
#include "IconProvider.h"
#include "Lang.h"
#include "mathbase.h"
#include "Painter.h"

#include <wx/choice.h>
#include <wx/clrpicker.h>
#include <wx/dc.h>
#include <wx/dir.h>
#include <wx/dcclient.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/statbox.h>
#include <wx/stattext.h>

extern Config *config;

enum { CD_CHOICE_OBJECT = wxID_HIGHEST + 1, CD_PICKER_FGCOLOR, CD_SPIN_FGWIDTH, CD_CHOICE_FGSTYLE,
       CD_PICKER_BGCOLOR, CD_CHOICE_BGSTYLE, CD_CHOICE_BGIMAGE, CD_SPIN_CORNER_RADIUS, CD_SET_DEFAULTS
     };

IMPLEMENT_CLASS( DasaPreviewPanel, wxPanel )

IMPLEMENT_CLASS( DasaColorPanel, ConfigPanel )

const wxString NO_IMAGE = wxT( "--" );

/*****************************************************
**
**   DasaPreviewPanel   ---   Constructor
**
******************************************************/
DasaPreviewPanel::DasaPreviewPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
		: wxPanel( parent, id, pos, size, wxFULL_REPAINT_ON_RESIZE )
{
	pen = new wxPen();
	brush = new wxBrush();
	cornerRadius = 0;

	Connect( wxEVT_PAINT, wxPaintEventHandler( DasaPreviewPanel::OnPaint ));
}

/*****************************************************
**
**   DasaPreviewPanel   ---   OnPaint
**
******************************************************/
void DasaPreviewPanel::OnPaint( wxPaintEvent &event )
{
	wxCoord h, w;
	wxPaintDC context( this );
	PrepareDC( context );

	GetSize( &w, &h );

	DcPainter painter( &context );

	bool isGrahaLord = ( objectid >= 12 );
	int lordid = objectid;
	if ( isGrahaLord )
	{
		if ( objectid <= 18 ) lordid = objectid - 12;
		else if ( objectid == 19 ) lordid = OMEANNODE;
		else if ( objectid == 20 ) lordid = OMEANDESCNODE;
		else if ( objectid == 21 ) lordid = OASCENDANT;
		else assert( false );
	}
	assert( lordid >= 0 );
	GraphicalDasaPaintHelper( &painter ).drawDasaBar( lordid, ! isGrahaLord, dconfig, 0, 0, w, h, cornerRadius );
}

/*****************************************************
**
**   DasaColorPanel   ---   Constructor
**
******************************************************/
DasaColorPanel::DasaColorPanel( wxWindow* parent )
		: ConfigPanel( parent )
{
    // begin wxGlade: DasaColorPanel::DasaColorPanel
    panel_preview = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL);
    sizer_preview_staticbox = new wxStaticBox(this, -1, _("Preview"));
    sizer_fg_staticbox = new wxStaticBox(this, -1, _("Foreground"));
    sizer_bg_staticbox = new wxStaticBox(this, -1, _("Background"));
    sizer_dasacolors_staticbox = new wxStaticBox(this, -1, _("Dasa Colors"));
    sizer_cornerradius_staticbox = new wxStaticBox(this, -1, _("Corner Radius"));
    sizer_object_staticbox = new wxStaticBox(this, -1, _("Object"));
    const wxString choice_object_choices[] = {
        _("dummy"),
        _("dummy"),
        _("dummy"),
        _("dummy"),
        _("dummy"),
        _("dummy"),
        _("dummy"),
        _("dummy"),
        _("dummy"),
        _("dummy")
    };
    choice_object = new wxChoice(this, CD_CHOICE_OBJECT, wxDefaultPosition, wxDefaultSize, 10, choice_object_choices, 0);
    window_preview = new DasaPreviewPanel(panel_preview, wxID_ANY);
    label_fgcolor = new wxStaticText(this, wxID_ANY, _("Color"));
    button_fgcolor = new wxColourPickerCtrl(this, CD_PICKER_FGCOLOR);
    label_fgwidth = new wxStaticText(this, wxID_ANY, _("Width"));
    spin_fgwidth = new wxSpinCtrl(this, CD_SPIN_FGWIDTH, wxT("1"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10);
    label_fgstyle = new wxStaticText(this, wxID_ANY, _("Style"));
    const wxString choice_fgstyle_choices[] = {
        _("Solid"),
        _("Dot"),
        _("Long Dash"),
        _("Short Dash"),
        _("Dot Dash")
    };
    choice_fgstyle = new wxChoice(this, CD_CHOICE_FGSTYLE, wxDefaultPosition, wxDefaultSize, 5, choice_fgstyle_choices, 0);
    label_bgcolor = new wxStaticText(this, wxID_ANY, _("Color"));
    button_bgcolor = new wxColourPickerCtrl(this, CD_PICKER_BGCOLOR);
    label_bgstyle = new wxStaticText(this, wxID_ANY, _("Style"));
    const wxString choice_bgstyle_choices[] = {
        _("Solid"),
        _("Transparent"),
        _("Backward diagonal hatch"),
        _("Cross-diagonal hatch"),
        _("Forward diagonal hatch"),
        _("Cross hatch"),
        _("Horizontal hatch"),
        _("Vertical hatch"),
        _("Stipple")
    };
    choice_bgstyle = new wxChoice(this, CD_CHOICE_BGSTYLE, wxDefaultPosition, wxDefaultSize, 9, choice_bgstyle_choices, 0);
    label_bgimage = new wxStaticText(this, wxID_ANY, _("Image"));
    const wxString choice_bgimage_choices[] = {
        _("dummy"),
        _("dummy"),
        _("dummy"),
        _("dummy"),
        _("dummy")
    };
    choice_bgimage = new wxChoice(this, CD_CHOICE_BGIMAGE, wxDefaultPosition, wxDefaultSize, 5, choice_bgimage_choices, 0);
    label_cornerradius = new wxStaticText(this, wxID_ANY, _("Radius in Pixels"));
    spin_cornerradius = new wxSpinCtrl(this, CD_SPIN_CORNER_RADIUS, wxT(""), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 20);
    button_default = new wxButton(this, CD_SET_DEFAULTS, _("Restore Defaults"));

    set_properties();
    do_layout();
    // end wxGlade
	wxString s;

	for ( int i = 0; i < MAX_DASA_COLORS; i++ ) dasaconfig[i] = new DasaColorConfig;

	choice_object->Clear();
	for ( int i = 0; i < MAX_DASA_COLORS; i++ )
	{
		choice_object->Append( config->colors->dasa[i].name );
	}
	choice_object->SetSelection( 0 );
	wxString picname, ext;
	wxDir dir;
	//IconProvider ip;

	choice_bgimage->Clear();
	s.Printf( _( "Background images found in directory %s. Supported formats are: PNG, GIF, JPG, XPM" ),
	          (const wxChar*)FileConfig::get()->getPicDir());
	choice_bgimage->SetToolTip( s );
	choice_bgimage->Append( NO_IMAGE );

	dir.Open( FileConfig::get()->getPicDir() );
	bool b = dir.GetFirst( &picname );
	while ( b )
	{
		ext = picname.Right( 4 ).Lower();
		if ( ext == wxT( ".png" ) || ext == wxT( ".jpg" ) || ext == wxT( ".gif" ) || ext == wxT( ".xpm" ))
		{
			choice_bgimage->Append( picname );
		}
		b = dir.GetNext( &picname );
	}

	Connect( CD_SET_DEFAULTS, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DasaColorPanel::OnSetDefaults ));
	Connect( wxID_ANY, wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( DasaColorPanel::OnSpin ));
	Connect( wxID_ANY, wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( DasaColorPanel::OnColorPicker ));

	Connect( CD_CHOICE_OBJECT, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( DasaColorPanel::OnObjectChoice ));
	Connect( CD_CHOICE_FGSTYLE, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( DasaColorPanel::OnChoice ));
	Connect( CD_CHOICE_BGSTYLE, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( DasaColorPanel::OnChoice ));
	Connect( CD_CHOICE_BGIMAGE, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( DasaColorPanel::OnChoice ));
}

/*****************************************************
**
**   DasaColorPanel   ---   Destructor
**
******************************************************/
DasaColorPanel::~DasaColorPanel()
{
	for ( int i = 0; i < MAX_DASA_COLORS; i++ ) delete dasaconfig[i];
}

/*****************************************************
**
**   DasaColorPanel   ---   setData
**
******************************************************/
void DasaColorPanel::setData()
{
	for ( int i = 0; i < MAX_DASA_COLORS; i++ ) *dasaconfig[i] = config->colors->dasa[i];
	spin_cornerradius->SetValue( config->dasaCornerRadius );
	updateControls();
}

/*****************************************************
**
**   DasaColorPanel   ---   saveData
**
******************************************************/
bool DasaColorPanel::saveData()
{
	for ( int i = 0; i < MAX_DASA_COLORS; i++ )
	{
		if ( dasaconfig[i]->bgStyle == wxSTIPPLE && dasaconfig[i]->bgImage == NO_IMAGE ) dasaconfig[i]->bgStyle = wxSOLID;
	}
	for ( int i = 0; i < MAX_DASA_COLORS; i++ )
	{
		config->colors->dasa[i] = *dasaconfig[i];
	}
	config->dasaCornerRadius = spin_cornerradius->GetValue();
	return true;
}

/*****************************************************
**
**   DasaColorPanel   ---   OnSetDefaults
**
******************************************************/
void DasaColorPanel::OnSetDefaults( wxCommandEvent &event )
{
	ColorConfig cfg;
	for ( int i = 0; i < MAX_DASA_COLORS; i++ ) *dasaconfig[i] = cfg.dasa[i];
	spin_cornerradius->SetValue( 5 );
	updateControls();
}

/*****************************************************
**
**   DasaColorPanel   ---   updateControls
**
******************************************************/
void DasaColorPanel::updateControls()
{
	int bgsel;
	int sel = choice_object->GetSelection();
	if ( sel < 0 ) return;

	assert( sel >= 0 && sel < MAX_DASA_COLORS );

	button_fgcolor->SetColour( dasaconfig[sel]->fgColor );
	choice_fgstyle->SetSelection( convertWxPen2Int( dasaconfig[sel]->fgStyle ));
	spin_fgwidth->SetValue( dasaconfig[sel]->fgWidth );
	button_bgcolor->SetColour( dasaconfig[sel]->bgColor );
	choice_bgstyle->SetSelection( convertWxBrush2Int( dasaconfig[sel]->bgStyle ));

	bool bgflag =  choice_bgstyle->GetSelection() == (int)(choice_bgstyle->GetCount() - 1 );

	bgsel = choice_bgimage->FindString( dasaconfig[sel]->bgImage );
	choice_bgimage->SetSelection( bgsel );

	choice_bgimage->Enable( bgflag );
	label_bgimage->Enable( bgflag );
	window_preview->setObjectId( sel );
	window_preview->setDasaConfig( dasaconfig[sel] );
	window_preview->setCornerRadius( spin_cornerradius->GetValue() );
	window_preview->Refresh();
}

/*****************************************************
**
**   DasaColorPanel   ---   updateCfg
**
******************************************************/
void DasaColorPanel::updateCfg()
{
	int sel = choice_object->GetSelection();
	assert( sel >= 0 && sel < MAX_DASA_COLORS );

	dasaconfig[sel]->fgColor = button_fgcolor->GetColour();
	dasaconfig[sel]->bgColor = button_bgcolor->GetColour();
	dasaconfig[sel]->fgStyle = convertInt2WxPen( choice_fgstyle->GetSelection());
	dasaconfig[sel]->fgWidth = spin_fgwidth->GetValue();

	int bgstyle = convertInt2WxBrush( choice_bgstyle->GetSelection());
	dasaconfig[sel]->bgStyle = bgstyle;
	dasaconfig[sel]->bgImage = choice_bgimage->GetStringSelection();
	window_preview->Refresh();
}

/*****************************************************
**
**   DasaColorPanel   ---   onStyleControl
**
******************************************************/
void DasaColorPanel::onStyleControl()
{
	updateCfg();
	updateControls();
}

/*****************************************************
**
**   DasaColorPanel   ---   OnObjectChoice
**
******************************************************/
void DasaColorPanel::OnObjectChoice( wxCommandEvent &event )
{
	updateControls();
}

/*****************************************************
**
**   DasaColorPanel   ---   set_properties
**
******************************************************/
void DasaColorPanel::set_properties()
{
    // begin wxGlade: DasaColorPanel::set_properties
    choice_object->SetToolTip(_("Sign or Lord of the Dasa"));
    choice_object->SetSelection(0);
    window_preview->SetMinSize(wxSize(180, 40));
    panel_preview->SetMinSize(wxSize(150, 40));
    choice_fgstyle->SetSelection(0);
    choice_bgstyle->SetToolTip(_("Background images can be used with option 'Stipple'"));
    choice_bgstyle->SetSelection(0);
    choice_bgimage->SetSelection(0);
    button_default->SetToolTip(_("Restore default settings"));
    // end wxGlade
}

/*****************************************************
**
**   DasaColorPanel   ---   do_layout
**
******************************************************/
void DasaColorPanel::do_layout()
{
    // begin wxGlade: DasaColorPanel::do_layout
    wxFlexGridSizer* sizer_main = new wxFlexGridSizer(2, 1, 0, 0);
    wxFlexGridSizer* sizer_right = new wxFlexGridSizer(3, 1, 0, 0);
    wxStaticBoxSizer* sizer_cornerradius = new wxStaticBoxSizer(sizer_cornerradius_staticbox, wxHORIZONTAL);
    wxStaticBoxSizer* sizer_dasacolors = new wxStaticBoxSizer(sizer_dasacolors_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_dasacolors = new wxFlexGridSizer(2, 2, 3, 3);
    wxStaticBoxSizer* sizer_bg = new wxStaticBoxSizer(sizer_bg_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_bg = new wxFlexGridSizer(3, 2, 3, 3);
    wxStaticBoxSizer* sizer_fg = new wxStaticBoxSizer(sizer_fg_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_fg = new wxFlexGridSizer(3, 2, 3, 3);
    wxStaticBoxSizer* sizer_preview = new wxStaticBoxSizer(sizer_preview_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_previewpanel = new wxFlexGridSizer(1, 1, 0, 0);
    wxStaticBoxSizer* sizer_object = new wxStaticBoxSizer(sizer_object_staticbox, wxVERTICAL);
    sizer_object->Add(choice_object, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    grid_dasacolors->Add(sizer_object, 1, wxALL|wxEXPAND, 3);
    grid_previewpanel->Add(window_preview, 1, wxALL|wxEXPAND|wxFIXED_MINSIZE, 3);
    panel_preview->SetSizer(grid_previewpanel);
    grid_previewpanel->AddGrowableRow(0);
    grid_previewpanel->AddGrowableCol(0);
    sizer_preview->Add(panel_preview, 1, wxALL|wxEXPAND, 3);
    grid_dasacolors->Add(sizer_preview, 1, wxALL|wxEXPAND, 3);
    grid_fg->Add(label_fgcolor, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_fg->Add(button_fgcolor, 1, wxALL|wxEXPAND, 3);
    grid_fg->Add(label_fgwidth, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_fg->Add(spin_fgwidth, 0, wxALL|wxEXPAND, 3);
    grid_fg->Add(label_fgstyle, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_fg->Add(choice_fgstyle, 0, wxALL|wxEXPAND, 3);
    grid_fg->AddGrowableCol(1);
    sizer_fg->Add(grid_fg, 1, wxEXPAND, 0);
    grid_dasacolors->Add(sizer_fg, 1, wxALL|wxEXPAND, 3);
    grid_bg->Add(label_bgcolor, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_bg->Add(button_bgcolor, 1, wxALL|wxEXPAND, 3);
    grid_bg->Add(label_bgstyle, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_bg->Add(choice_bgstyle, 0, wxALL|wxEXPAND, 3);
    grid_bg->Add(label_bgimage, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_bg->Add(choice_bgimage, 0, wxALL|wxEXPAND, 3);
    sizer_bg->Add(grid_bg, 1, wxEXPAND, 0);
    grid_dasacolors->Add(sizer_bg, 1, wxALL|wxEXPAND, 3);
    grid_dasacolors->AddGrowableRow(1);
    grid_dasacolors->AddGrowableCol(0);
    sizer_dasacolors->Add(grid_dasacolors, 1, wxEXPAND, 0);
    sizer_right->Add(sizer_dasacolors, 1, wxEXPAND, 0);
    sizer_cornerradius->Add(label_cornerradius, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    sizer_cornerradius->Add(spin_cornerradius, 0, wxALL, 3);
    sizer_right->Add(sizer_cornerradius, 1, wxALL|wxEXPAND, 3);
    sizer_right->Add(button_default, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 3);
    sizer_main->Add(sizer_right, 1, wxALL|wxEXPAND, 3);
    SetSizer(sizer_main);
    sizer_main->Fit(this);
    // end wxGlade
}

/*****************************************************
**
**   ConfigPanelFactory   ---   createDasaColorPanel
**
******************************************************/
ConfigPanel *ConfigPanelFactory:: createDasaColorPanel( wxWindow *parent )
{
	return new DasaColorPanel( parent );
}

