/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/DialogElements.cpp
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
#pragma implementation "DialogElements.h"
#endif

#include "DialogElements.h"

#include "astrobase.h"
#include "constants.h"
#include "Conf.h"
#include "DialogElements.h"
#include "guibase.h"
#include "Writer.h"

#include <math.h>

#include <wx/sizer.h>
#include <wx/settings.h>
#include <wx/statbox.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

extern Config *config;

IMPLEMENT_CLASS( MyGrid, wxGrid )
IMPLEMENT_CLASS( ToolbarLabel, wxControl )
IMPLEMENT_CLASS( MultiLineStaticText, wxPanel )

/*****************************************************
**
**   MyGrid   ---   Constructor
**
******************************************************/
MyGrid::MyGrid( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size,
                long style, const wxString& name )
		: wxGrid( parent, id, pos, size, style, name )
{
	Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( MyGrid::OnMouseEvent ));
	Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( MyGrid::OnKeyDown ));
}

/*****************************************************
**
**   MyGrid   ---   OnMouseEvent
**
******************************************************/
void MyGrid::OnMouseEvent( wxMouseEvent &event )
{
	int x, y;
	const int offset = 3;
	GetViewStart( &x, &y );
	if ( event.GetWheelRotation() < 0 ) y += offset;
	else y -= offset;
	Scroll( x, y );
}

/*****************************************************
**
**   MyGrid   ---   OnKeyDown
**
******************************************************/
void MyGrid::OnKeyDown( wxKeyEvent &event )
{
	long keycode = event.GetKeyCode();
	if ( keycode == WXK_ESCAPE ) {
		GetParent()->Close();
	}
	event.Skip();
}

/*****************************************************
**
**   FilterChoice   ---   Constructor
**
******************************************************/
FilterChoice::FilterChoice( wxWindow *parent, int id )
		: wxChoice( parent, id, wxDefaultPosition, wxSize( 120, -1 ) )
{
	SetToolTip( _( "Set Filter" ));
}

/*****************************************************
**
**   FilterChoice   ---   recreate
**
******************************************************/
void FilterChoice::recreate( const vector<int> &pl )
{
	if ( plist == pl ) return;
	Writer *writer = WriterFactory().getWriter( WRITER_TEXT );

	unsigned int sel = GetSelection();
	int psel = -1;
	if ( sel > 0 && sel < plist.size() + 1 )
	{
		psel = plist[sel - 1]; // Index of selected object
	}

	Clear();
	Append( _( "No Filter" ));

	plist.clear();
	for ( unsigned int i = 0; i < pl.size(); i++ )
	{
		plist.push_back( pl[i] );
		Append( writer->getObjectName( pl[i], TLARGE ));
	}
	int newsel = 0;
	if ( psel != -1 )
	{
		// search for object in new list
		for ( unsigned int i = 0; i < plist.size(); i++ )
		{
			if ( plist[i] == psel )
			{
				newsel = i + 1;
				break;
			}
		}
	}
	SetSelection( newsel );
}

/*****************************************************
**
**   FilterChoice   ---   getFilter
**
******************************************************/
int FilterChoice::getFilter()
{
	unsigned int sel = GetSelection();
	if ( sel > 0 && sel <= plist.size() ) return plist[sel - 1];
	else return -1;
}

/*****************************************************
**
**   YearLengthChoice   ---   Constructor
**
******************************************************/
YearLengthChoice::YearLengthChoice( wxWindow *parent, int id, const bool vedic, int selected )
		: wxChoice( parent, id )
{
	SetToolTip( _( "Year Length" ));
	Append( wxT( "365.25" ));
	Append( _( "Tropical (365.2422)" ));
	Append( wxT( "365" ));
	Append( wxT( "360" ));
	Append( _( "Custom" ));
	SetSize( GetBestSize());
	SetSelection( selected );
}

/*****************************************************
**
**   YearLengthChoice   ---   getYearLength
**
******************************************************/
double YearLengthChoice::getYearLength()
{
	return calcYearLength( GetSelection(), vedic );
}

/*****************************************************
**
**   CircleTypeChoice   ---   Constructor
**
******************************************************/
CircleTypeChoice::CircleTypeChoice( wxWindow *parent, int id, int selected )
		: wxChoice( parent, id )
{
	SetToolTip( _( "Circle Type" ));
	Append( wxT( "360" ));
	Append( wxT( "180" ));
	Append( wxT( "90" ));
	Append( wxT( "45" ));
	Append( wxT( "22.5" ));
	Append( wxT( "11.25" ));
	Append( wxT( "5.625" ));
	SetSize( GetBestSize());
	SetSelection( selected );
}

/*****************************************************
**
**   CircleTypeChoice   ---   DoGetBestSize
**
******************************************************/
wxSize CircleTypeChoice::DoGetBestSize() const
{
	//printf( "DoGetBestSize :::\n" );
	return wxChoice::DoGetBestSize();
}

/*****************************************************
**
**   SortChoice   ---   Constructor
**
******************************************************/
SortChoice::SortChoice( wxWindow *parent, int id, int selected )
		: wxChoice( parent, id, wxDefaultPosition, wxSize( 110, -1 ))
{
	SetToolTip( _( "Sort Order" ));
	Append( _( "Planets" ));
	Append( _( "Orbis" ));
	Append( _( "Orbis Absolute" ));
	Append( _( "Planets (2)" ));
	Append( _( "Aspects" ));
	SetSize( GetBestSize());
	SetSelection( selected );
}

/*****************************************************
**
**   TransitModeChoice   ---   Constructor
**
******************************************************/
TransitModeChoice::TransitModeChoice( wxWindow *parent, int id, int selected )
		: wxChoice( parent, id ) //, wxDefaultPosition, wxSize( 110, -1 ))
{
	SetToolTip( _( "Mode" ));
	Append( _( "Transits" ));
	Append( _( "Solar Arc" ));
	Append( _( "Directions" ));
	Append( _( "Lunar Arc" ));
	Append( _( "Constant Arc" ));
	SetSize( GetBestSize());
	SetSelection( selected );
}

/*****************************************************
**
**   ToolbarLabel   ---   Constructor
**
******************************************************/
ToolbarLabel::ToolbarLabel( wxWindow *parent, wxWindowID id, wxPoint pos, wxSize size, wxString tooltip )
		: wxControl( parent, id, pos, size )
{
	// TODO
	wxSize sizep = wxDefaultSize;
	wxSize sizet = wxDefaultSize;
	if ( size != wxDefaultSize )
	{
		sizep = size;
		sizep.x -= 6;
		sizep.y -= 6;
		sizet = size;
		sizet.x -= 12;
		sizet.y -= 12;
	}
	wxPanel *panel = new wxPanel(this, -1, pos, sizep, wxEXPAND|wxNO_BORDER|wxTAB_TRAVERSAL);
	wxBoxSizer* sizerp = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* sizert = new wxBoxSizer(wxVERTICAL);
	sizerp->Add( panel, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3 );

	theText = new wxStaticText( panel, -1, wxEmptyString, pos, sizet );
	sizert->Add( theText, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );
	panel->SetSizer(sizert);
	sizert->Fit(panel);
	sizerp->Fit(this);
	sizerp->SetSizeHints(this);
	sizert->SetSizeHints(panel);
	SetSizer(sizerp);
	Layout();
}

/*****************************************************
**
**   ToolbarLabel   ---   DoGetBestSize
**
******************************************************/
wxSize ToolbarLabel::DoGetBestSize() const
{
	wxSize ret( wxControl::DoGetBestSize() );

	wxSize best(95, ret.y);
	//printf( "DoGetBestSize :::\n" );
	return best;
}

/*****************************************************
**
**   ToolbarLabel   ---   SetLabel
**
******************************************************/
void ToolbarLabel::SetLabel( wxString s )
{
	theText->SetLabel( s );
}

/*****************************************************
**
**   AyanamsaChoice   ---   Constructor
**
******************************************************/
AyanamsaChoice::AyanamsaChoice( wxWindow *parent, int id, int ayatype )
		: wxChoice( parent, id )
{
	Append( _( "None" ));
	Append( _( "Lahiri" ));
	Append( _( "Raman" ));
	Append( _( "Krishnamurti" ));
	Append( _( "Custom" ));
	SetSelection( ayatype );
	SetSize( GetBestSize());
}

/*****************************************************
**
**   AyanamsaChoice   ---   getConfigIndex
**
******************************************************/
int AyanamsaChoice::getConfigIndex()
{
	return GetSelection();
}

/*****************************************************
**
**   MultiLineStaticText   ---   Constructor
**
******************************************************/
MultiLineStaticText::MultiLineStaticText( wxWindow *parent, wxWindowID id, wxPoint pos, wxSize size )
 : wxPanel( parent, id, pos, size, wxSUNKEN_BORDER|wxTAB_TRAVERSAL )
{
	text = new wxStaticText( this, wxID_ANY, wxEmptyString );
	text->SetMinSize(wxSize( size.x - 6, size.y - 6 ));

	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(text, 0, wxALL|wxEXPAND, 3);

	SetSizer( sizer );
	sizer->Fit(this);
	Layout();

	Connect( wxEVT_SIZE, wxSizeEventHandler( MultiLineStaticText::OnSize ));
}

/*****************************************************
**
**   MultiLineStaticText   ---   SetLabel
**
******************************************************/
void MultiLineStaticText::SetLabel( wxString s )
{
	text->SetLabel( s );
}

/*****************************************************
**
**   MultiLineStaticText   ---   GetLabel
**
******************************************************/
wxString MultiLineStaticText::GetLabel()
{
	return text->GetLabel();
}

/*****************************************************
**
**   MultiLineStaticText   ---   OnSize
**
******************************************************/
void MultiLineStaticText::OnSize( wxSizeEvent &event )
{
	printf( "ON size %d\n", event.GetSize().x );
	wxString s = text->GetLabel();
	s.Replace( wxT( "\n" ), wxEmptyString );	
	text->SetLabel( s );
	PrintLn( text->GetLabel());
	text->SetSize( event.GetSize());
	text->Wrap( event.GetSize().x - 12 );
	text->Layout();
	text->Refresh();
	//event.Skip();
}


