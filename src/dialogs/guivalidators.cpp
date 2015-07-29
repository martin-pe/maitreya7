/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/guivalidators.cpp
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
#pragma implementation "guivalidators.h"
#endif

#include "guivalidators.h"

#include "ColorConfig.h"
#include "Commands.h"
#include "Conf.h"
#include "Lang.h"
#include "mathbase.h"

#include <wx/filename.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/window.h>

extern Config *config;

IMPLEMENT_CLASS( BaseTextValidator, wxTextValidator )
IMPLEMENT_CLASS( DegreeValidator, BaseTextValidator )
IMPLEMENT_CLASS( DoubleValidator, BaseTextValidator )
IMPLEMENT_CLASS( DateValidator, BaseTextValidator )
IMPLEMENT_CLASS( FilenameValidator, BaseTextValidator )
IMPLEMENT_CLASS( DirectoryValidator, BaseTextValidator )
IMPLEMENT_CLASS( NumericValidator, BaseTextValidator )

DEFINE_EVENT_TYPE( COMMAND_TEXT_RETURN )

/*****************************************************
**
**   BaseTextValidator   ---   Constructor
**
******************************************************/
BaseTextValidator::BaseTextValidator( const long style )
		: wxTextValidator( style, 0 )
{
	Connect( wxEVT_CHAR, wxKeyEventHandler( BaseTextValidator::OnChar ));
}

/*****************************************************
**
**   BaseTextValidator   ---   COPY Constructor
**
******************************************************/
BaseTextValidator::BaseTextValidator( const BaseTextValidator &v )
	: wxTextValidator()
{
	const wxArrayString a = ((DegreeValidator*)&v)->GetIncludes();
	SetIncludes( a );

	SetWindow(  v.GetWindow());
}

/*****************************************************
**
**   BaseTextValidator   ---   isInCharIncludes
**
******************************************************/
bool BaseTextValidator::isInCharIncludes( const wxString& val )
{
	size_t i;
	for ( i = 0; i < val.length(); i++)
	{
		if (m_includes.Index((wxString) val[i]) == wxNOT_FOUND)
			return false;
	}
	return true;
}

/*****************************************************
**
**   BaseTextValidator   ---   isNotInCharExcludes
**
******************************************************/
bool BaseTextValidator::isNotInCharExcludes( const wxString& val )
{
    size_t i;
    for ( i = 0; i < val.length(); i++)
    {
       if (m_excludes.Index((wxString) val[i]) != wxNOT_FOUND)
            return false;
    }
    return true;
}

/*****************************************************
**
**   BaseTextValidator   ---   OnChar
**
******************************************************/
void BaseTextValidator::OnChar(wxKeyEvent &event )
{
	const int keyCode = event.GetKeyCode();

	if ( keyCode == WXK_RETURN )
	{
		wxCommandEvent e( COMMAND_TEXT_RETURN, GetWindow()->GetId() );
		wxPostEvent( GetWindow(), e );
		return;
	}

	bool isvalid = ( event.m_controlDown == 0 && event.m_metaDown == 0 && event.m_altDown == 0 && keyCode != WXK_ESCAPE );

	if ( isvalid && m_validatorWindow )
	{
		if ( 
			! (keyCode < WXK_SPACE || keyCode == WXK_DELETE || keyCode > WXK_START) &&
			(
			((m_validatorStyle & wxFILTER_INCLUDE_CHAR_LIST) && !isInCharIncludes(wxString((wxChar) keyCode, 1))) ||
			((m_validatorStyle & wxFILTER_EXCLUDE_CHAR_LIST) && !isNotInCharExcludes(wxString((wxChar) keyCode, 1))) ||
			((m_validatorStyle & wxFILTER_ASCII) && !isascii(keyCode)) ||
			((m_validatorStyle & wxFILTER_ALPHA) && !wxIsalpha(keyCode)) ||
			((m_validatorStyle & wxFILTER_ALPHANUMERIC) && !wxIsalnum(keyCode)) ||
			((m_validatorStyle & wxFILTER_NUMERIC) && !wxIsdigit(keyCode)
				&& keyCode != wxT('.') && keyCode != wxT(',') && keyCode != wxT('-') && keyCode != wxT('+') && keyCode != wxT('e') && keyCode != wxT('E'))
			 )
		 )
		{
			isvalid = false;
		}
	}
	//printf( "BaseTextValidator::OnChar %d control %d raw %d isvalid %d\n", event.GetKeyCode(), event.m_controlDown, event.GetRawKeyCode(), isvalid );
	
	if ( isvalid ) event.Skip();
	else
	{
		int i = CommandHandler::get()->lookupCommand4CharEvent( event );
		if ( i )
		{
			wxMenuEvent evt( wxEVT_COMMAND_MENU_SELECTED, i );
			evt.ResumePropagation( wxEVENT_PROPAGATE_MAX );
			wxPostEvent( GetWindow(), evt );
		}
	}
}

/*****************************************************
**
**   BaseTextValidator   ---   setWindowBackground
**
******************************************************/
void BaseTextValidator::setWindowBackground( const bool &b )
{
	wxTextCtrl *ctrl = (wxTextCtrl*)GetWindow();
	assert( ctrl );

	ctrl->SetBackgroundColour( b? wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) : config->colors->errorBgColor );

	// must  be set, otherwise forground will be white (GTK, wx 2.8.12)
	ctrl->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOWTEXT ));
	ctrl->Refresh();
}

/*****************************************************
**
**   DegreeValidator   ---   Constructor
**
******************************************************/
DegreeValidator::DegreeValidator( const double &minvalue, const double &maxvalue )
	: BaseTextValidator(),
	minvalue( minvalue ),
	maxvalue( maxvalue )
{
	setup();
}

/*****************************************************
**
**   DegreeValidator   ---   Copy Constructor
**
******************************************************/
DegreeValidator::DegreeValidator( const DegreeValidator &v )
 : BaseTextValidator( v ),
	minvalue( v.minvalue ),
	maxvalue( v.maxvalue )
{
}

/*****************************************************
**
**   DegreeValidator   ---   Clone
**
******************************************************/
wxObject *DegreeValidator::Clone() const
{
	DegreeValidator *v = new DegreeValidator( minvalue, maxvalue );
	return v;
}

/*****************************************************
**
**   DegreeValidator   ---   setup
**
******************************************************/
void DegreeValidator::setup()
{
	wxArrayString a;
	wxString s1;
	for ( int i = 0; i < 10; i++ )
	{
		s1.Printf( wxT( "%d" ), i );
		a.Add( s1 );
	}
	a.Add( wxT( ":" ));
	SetIncludes( a );
}

/*****************************************************
**
**   DegreeValidator   ---   Validate
**
******************************************************/
bool DegreeValidator::Validate(wxWindow* parent)
{
	int deg, min, sec;
	Formatter *formatter = Formatter::get();

	wxTextCtrl *ctrl = (wxTextCtrl*)GetWindow();
	wxString s = ctrl->GetValue();
	bool b = formatter->getDegreeIntsFromString( (const wxChar*)s, deg, min, sec );

	if ( deg < (int)minvalue || deg + 1 > (int)maxvalue ) b = 0;
	if ( s.IsEmpty()) b =  true;

	setWindowBackground( b );
	parent->Refresh();
	return b;
}

/*****************************************************
**
**   DoubleValidator   ---   Constructor
**
******************************************************/
DoubleValidator::DoubleValidator( const double minvalue, const double maxvalue )
	: BaseTextValidator(),
	minvalue( minvalue ),
	maxvalue( maxvalue )
{
	setup();
}

/*****************************************************
**
**   DoubleValidator   ---   Copy Constructor
**
******************************************************/
DoubleValidator::DoubleValidator( const DoubleValidator &v )
 : BaseTextValidator( v ),
	minvalue( v.minvalue ),
	maxvalue( v.maxvalue )
{
}

/*****************************************************
**
**   DoubleValidator   ---   Clone
**
******************************************************/
wxObject *DoubleValidator::Clone() const
{
	DoubleValidator *v = new DoubleValidator( minvalue, maxvalue );
	return v;
}

/*****************************************************
**
**   DoubleValidator   ---   setup
**
******************************************************/
void DoubleValidator::setup()
{
	wxArrayString a;
	for ( int i = 0; i < 10; i++ )
	{
		a.Add( wxString::Format( wxT( "%d" ), i ));
	}
	a.Add( wxT( "." ));
	a.Add( wxT( "," ));
	a.Add( wxT( "-" ));
	SetIncludes( a );
}

/*****************************************************
**
**   DoubleValidator   ---   Validate
**
******************************************************/
bool DoubleValidator::Validate(wxWindow* parent)
{
	double a;
	bool b = true;

	wxTextCtrl *ctrl = (wxTextCtrl*)GetWindow();
	wxString s = ctrl->GetValue();

	int nbdots = 0;
	int nbcommas = 0;
	for( unsigned int i = 0; i < s.Length(); i++ )
	{
		if ( s[i] == '.' ) nbdots++;
		if ( s[i] == ',' ) nbcommas++;
	}

	if ( nbdots > 1 || nbcommas > 1 )
	{
		printf( "DoubleValidator::Validate WARN contents %s has more than 2 tokens\n", str2char( s ));
		b = false;
	}
	if ( b && nbdots > 0 && nbcommas > 0 )
	{
		printf( "DoubleValidator::Validate WARN contents %s has mixed dots and commas\n", str2char( s ) );
		b = false;
	}

	if ( b )
	{
		a = myatof( s );
		if ( minvalue != -1 && a < minvalue ) b = false;
		if ( maxvalue != -1 && a > maxvalue ) b = false;
	}

	setWindowBackground( b );
	return b;
}

/*****************************************************
**
**   DateValidator   ---   Constructor
**
******************************************************/
DateValidator::DateValidator()
	: BaseTextValidator()
{
	setup();
}

/*****************************************************
**
**   DateValidator   ---   Copy Constructor
**
******************************************************/
DateValidator::DateValidator( const DateValidator &v )
 : BaseTextValidator( v )
{
}

/*****************************************************
**
**   DateValidator   ---   Clone
**
******************************************************/
wxObject *DateValidator::Clone() const
{
	DateValidator *v = new DateValidator();
	return v;
}

/*****************************************************
**
**   DateValidator   ---   setup
**
******************************************************/
void DateValidator::setup()
{
	wxArrayString a;
	for ( int i = 0; i < 10; i++ )
	{
		a.Add( wxString::Format( wxT( "%d" ), i ));
	}
	a.Add( wxT( "." ));
	a.Add( wxT( "-" ));
	SetIncludes( a );
}

/*****************************************************
**
**   DateValidator   ---   Validate
**
******************************************************/
bool DateValidator::Validate(wxWindow* parent)
{
	int day, month, year;
	Formatter *formatter = Formatter::get();

	wxTextCtrl *ctrl = (wxTextCtrl*)GetWindow();
	bool b =  formatter->getDateIntsFromString( (const wxChar*)ctrl->GetValue(), day, month, year );
	setWindowBackground( b );
	return b;
}

/*****************************************************
**
**   FilenameValidator   ---   Constructor
**
******************************************************/
FilenameValidator::FilenameValidator()
	: BaseTextValidator( wxFILTER_ASCII )
{
}

/*****************************************************
**
**   FilenameValidator   ---   Copy Constructor
**
******************************************************/
FilenameValidator::FilenameValidator( const FilenameValidator &v )
 : BaseTextValidator( v )
{
}

/*****************************************************
**
**   FilenameValidator   ---   Clone
**
******************************************************/
wxObject *FilenameValidator::Clone() const
{
	FilenameValidator *v = new FilenameValidator();
	return v;
}

/*****************************************************
**
**   FilenameValidator   ---   Validate
**
******************************************************/
bool FilenameValidator::Validate(wxWindow* parent)
{
	bool b = true;
	wxTextCtrl *ctrl = (wxTextCtrl*)GetWindow();
	wxString filename = ctrl->GetValue();

	if ( ! wxFileName::FileExists( filename ))
	{
		b = false;
	}
	else if ( ! wxFileName::IsFileReadable( filename ))
	{
		b = false;
	}
	setWindowBackground( b );
	return b;
}

/*****************************************************
**
**   DirectoryValidator   ---   Validate
**
******************************************************/
bool DirectoryValidator::Validate(wxWindow* parent)
{
	bool b = true;
	wxTextCtrl *ctrl = (wxTextCtrl*)GetWindow();
	wxString dirname = ctrl->GetValue();

	if ( ! wxFileName::DirExists( dirname ))
	{
		b = false;
	}
	setWindowBackground( b );
	return b;
}

/*****************************************************
**
**   NumericValidator   ---   Constructor
**
******************************************************/
NumericValidator::NumericValidator( const long minvalue, const long maxvalue )
	: BaseTextValidator( wxFILTER_NUMERIC ),
	minvalue( minvalue ),
	maxvalue( maxvalue )
{
}

/*****************************************************
**
**   NumericValidator   ---   Copy Constructor
**
******************************************************/
NumericValidator::NumericValidator( const NumericValidator &v )
 : BaseTextValidator( v ),
	minvalue( v.minvalue ),
	maxvalue( v.maxvalue )
{
}

/*****************************************************
**
**   NumericValidator   ---   Validate
**
******************************************************/
bool NumericValidator::Validate(wxWindow* parent)
{
	long l;
	bool b = true;

	wxTextCtrl *ctrl = (wxTextCtrl*)GetWindow();
	wxString s = ctrl->GetValue();
	s.ToLong( &l );

	if ( minvalue != -1 && l < minvalue ) b = false;
	if ( maxvalue != -1 && l > maxvalue ) b = false;

	setWindowBackground( b );
	return b;
}

