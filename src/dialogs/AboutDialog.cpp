/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/AboutDialog.cpp
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
#pragma implementation "AboutDialog.h"
#endif

#include "AboutDialog.h"
#include "TextWidget.h"
#include "config.h"

#include <wx/button.h>
#include <wx/dc.h>
#include <wx/dcclient.h>
#include <wx/filename.h>
#include <wx/image.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/textfile.h>
#include <wx/timer.h>

#include "HtmlWriter.h"
#include "FileConfig.h"

#include "guibase.h"
#include "IconProvider.h"

enum { ABOUT_TIMER = wxID_HIGHEST + 1 };

IMPLEMENT_CLASS( LogoWidget, wxWindow )
IMPLEMENT_CLASS( AboutDialog, wxDialog )

#define PIC_X 546
#define PIC_Y 205

/*****************************************************
**
**   LogoWidget   ---   Constructor
**
******************************************************/
LogoWidget::LogoWidget( wxWindow *parent, int id )
		: wxWindow( parent, id )
{
	wxString s;
	s = FileConfig::get()->getPicDir();
	if ( s.Last() != wxFileName::GetPathSeparator()) s << wxFileName::GetPathSeparator();

	bitmap = new wxBitmap( s + LOGO_FILE, wxBITMAP_TYPE_JPEG );

	xzoom = yzoom = 100;
	ani_mode = 0;
	ani_direction = 0;

	timer = new wxTimer( this, ABOUT_TIMER );
	timer->Start( 6000 );

	Connect( wxEVT_PAINT, wxPaintEventHandler( LogoWidget::OnPaint ));
	Connect( ABOUT_TIMER, wxEVT_TIMER, wxTimerEventHandler( LogoWidget::OnTimer ));
}

/*****************************************************
**
**   LogoWidget   ---   Destructor
**
******************************************************/
LogoWidget::~LogoWidget()
{
	timer->Stop();
	delete timer;
	delete bitmap;
}

/*****************************************************
**
**   LogoWidget   ---   doAnimation
**
******************************************************/
void LogoWidget::doAnimation()
{
	if ( ! timer->IsRunning() ) timer->Start( 100 );
	else timer->Stop();
}

/*****************************************************
**
**   LogoWidget   ---   OnTimer
**
******************************************************/
void LogoWidget::OnTimer( wxTimerEvent &event )
{
	if ( xzoom == 100 && yzoom == 100 ) timer->Start( 100 );
	if ( ani_direction == 0 )
	{
		if ( ani_mode == 0 ) xzoom -= 10;
		else yzoom -= 10;
		if ( xzoom <= -100 || yzoom <= -100 ) ani_direction = 1;
	}
	else
	{
		if ( ani_mode == 0 ) xzoom += 10;
		else yzoom += 10;
		if ( xzoom >= 100 && yzoom >= 100 )
		{
			ani_direction = 0;
			ani_mode = ! ani_mode;
			timer->Start( 12000 );
		}
	}
	Refresh( false );
}

/*****************************************************
**
**   LogoWidget   ---   OnPaint
**
******************************************************/
void LogoWidget::OnPaint(wxPaintEvent& event)
{
	double axzoom = ( xzoom > 0 ? xzoom : -xzoom );
	double ayzoom = ( yzoom > 0 ? yzoom : -yzoom );
	int x = (int)(PIC_X * axzoom / 100);
	int xborder = (int)(( PIC_X - x ) / 2);
	int y = (int)(PIC_Y * ayzoom / 100);
	int yborder = (int)(( PIC_Y - y ) / 2);
	bool mirror = ( xzoom == 100 ? false : true );

	wxPaintDC dc(this);
	wxColour bgcolor = wxSystemSettings::GetColour( wxSYS_COLOUR_BACKGROUND );
	dc.SetBrush( wxBrush( GetBackgroundColour(), wxSOLID ));
	dc.SetPen( wxPen( bgcolor, 1, wxSOLID ));
	dc.DrawRectangle( -1, -1, xborder, PIC_Y+2 ); // left
	dc.DrawRectangle( PIC_X - xborder, -1, PIC_X/2+2, PIC_Y+2 ); // right
	dc.DrawRectangle( -1, -1, PIC_X+2, yborder ); // upper
	dc.DrawRectangle( -1, PIC_Y - yborder, PIC_X+2, PIC_Y/2+2 ); // lower

	static double angle = 0;
	angle += .02;
	if ( xzoom == 100 && yzoom == 100 )
	{
		dc.DrawBitmap( *bitmap, xborder, yborder, true );
	}
	else if ( xzoom >= 0 && yzoom >= 0 )
	{
		if ( x > 0 && y > 0 )
			dc.DrawBitmap( wxBitmap( bitmap->ConvertToImage().Scale( x, y )), xborder, yborder, true );
	}
	else
	{
		dc.DrawBitmap( wxBitmap( bitmap->ConvertToImage().Mirror( mirror ).Scale( x, y )), xborder, yborder, true );
	}
}

/*****************************************************
**
**   AboutDialog   ---   Constructor
**
******************************************************/
AboutDialog::AboutDialog(wxWindow* parent )
		: wxDialog( parent, -1, wxT( "" ), wxDefaultPosition, wxDefaultSize, DEFAULT_DIALOG_STYLE )
{
    // begin wxGlade: AboutDialog::AboutDialog
    sizer_author_staticbox = new wxStaticBox(this, -1, _("Author"));
    sizer_software_staticbox = new wxStaticBox(this, -1, _("Software"));
    panel_logo = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL);
    widget = new LogoWidget(panel_logo, wxID_ANY);
    label_author = new wxStaticText(this, wxID_ANY, _("Author"));
    label_url = new wxStaticText(this, wxID_ANY, _("Url"));
    label_dedicate = new wxStaticText(this, wxID_ANY, _("Dedicated to Paramapadma Dhirananda"));
    label_version = new wxStaticText(this, wxID_ANY, _("Version"));
    label_builddate = new wxStaticText(this, wxID_ANY, _("Build Date"));
    label_compiler = new wxStaticText(this, wxID_ANY, _("Compiler"));
    label_toolkit = new wxStaticText(this, wxID_ANY, _("Gui Framework"));
    label_unicode = new wxStaticText(this, wxID_ANY, _("Unicode Support"));
    label_os = new wxStaticText(this, wxID_ANY, _("Operating System"));
    button_ok = new wxButton(this, wxID_OK, _("OK"));

    set_properties();
    do_layout();
    // end wxGlade
	SetIcon( IconProvider::getIcon( BITMAP_ABOUT ));
	button_ok->SetFocus();
}

/*****************************************************
**
**   AboutDialog   ---   ProcessEvent
**
******************************************************/
bool AboutDialog::ProcessEvent(wxEvent& event)
{
	if ( event.GetClassInfo()->IsKindOf( CLASSINFO( wxKeyEvent )))
	{
		if ( ((wxKeyEvent*)&event)->GetKeyCode() == WXK_ESCAPE )
		{
			EndModal( wxID_CANCEL );
			return true;
		}
		else return wxDialog::ProcessEvent( event );
	}
	else return wxDialog::ProcessEvent( event );
}

/*****************************************************
**
**   AboutDialog   ---   set_properties
**
******************************************************/
void AboutDialog::set_properties()
{
	wxString s;

	label_author->SetLabel( AUTHOR );
	label_url->SetLabel( SARAVALI_URL );

	// Version of the software
	s << _( "Version" ) << wxT( ": " ) << wxConvertMB2WX( PACKAGE_VERSION );
	label_version->SetLabel( s );

	// Build date
	label_builddate->SetLabel( wxString::Format( wxT( "%s: %s" ), _( "Build Date" ), wxT( __DATE__ ) ) );

	// Compiler
	s.Clear();
	s << _( "Compiled by" ) << wxT ( ": " );
#ifdef __GNUC__
	s << wxT( "GNUC " ) << wxConvertMB2WX( __VERSION__ );
#elif defined(__VISUALC__)
	s << wxT( "Visual C " ) << wxString::Format( wxT( "%d" ), _MSC_VER );
#else
	s << wxT( "Unknown Compiler" );
#endif
	label_compiler->SetLabel( s );

	label_toolkit->SetLabel( wxString::Format( wxT( "%s: %d.%d.%d" ), _( "wxWidgets Version" ), wxMAJOR_VERSION, wxMINOR_VERSION, wxRELEASE_NUMBER ));

#if wxUSE_UNICODE
	const wxChar *s1 = _( "yes" );
#else
	const wxChar *s1 = _( "no" );
#endif

	label_unicode->SetLabel( wxString::Format( wxT( "%s: %s" ), _( "Unicode Support" ), s1 ));

	label_os->SetLabel( wxString::Format( _( "Currently running on %s" ), (const wxChar*)wxGetOsDescription()));

    // begin wxGlade: AboutDialog::set_properties
    SetTitle(_("About"));
    widget->SetMinSize(wxSize(546,205));
    label_author->SetMinSize(wxSize(350, 16));
    label_version->SetMinSize(wxSize(300, 16));
    label_builddate->SetMinSize(wxSize(300, 16));
    label_compiler->SetMinSize(wxSize(300, 16));
    label_toolkit->SetMinSize(wxSize(300, 16));
    label_unicode->SetMinSize(wxSize(300, 16));
    label_os->SetMinSize(wxSize(300, 16));
    button_ok->SetDefault();
    // end wxGlade
}

/*****************************************************
**
**   AboutDialog   ---   do_layout
**
******************************************************/
void AboutDialog::do_layout()
{
    // begin wxGlade: AboutDialog::do_layout
    wxFlexGridSizer* sizer_about = new wxFlexGridSizer(4, 1, 0, 0);
    wxStaticBoxSizer* sizer_software = new wxStaticBoxSizer(sizer_software_staticbox, wxVERTICAL);
    wxStaticBoxSizer* sizer_author = new wxStaticBoxSizer(sizer_author_staticbox, wxVERTICAL);
    wxBoxSizer* sizer_logo = new wxBoxSizer(wxHORIZONTAL);
    sizer_logo->Add(widget, 1, wxALL|wxEXPAND, 3);
    panel_logo->SetSizer(sizer_logo);
    sizer_about->Add(panel_logo, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
    sizer_author->Add(label_author, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    sizer_author->Add(label_url, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    sizer_author->Add(label_dedicate, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    sizer_about->Add(sizer_author, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
    sizer_software->Add(label_version, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    sizer_software->Add(label_builddate, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    sizer_software->Add(label_compiler, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    sizer_software->Add(label_toolkit, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    sizer_software->Add(label_unicode, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    sizer_software->Add(label_os, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    sizer_about->Add(sizer_software, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 3);
    sizer_about->Add(button_ok, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
    SetSizer(sizer_about);
    sizer_about->Fit(this);
    sizer_about->AddGrowableCol(0);
    Layout();
    // end wxGlade
}

