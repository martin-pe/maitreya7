/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/base/mathbase.cpp
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
#pragma implementation "mathbase.h"
#endif

#include "mathbase.h"

#include <locale.h>
#include <wx/tokenzr.h>
#include <wx/utils.h>
#include <wx/filename.h>

/*****************************************************
**
**   locateFile
**
******************************************************/
wxString locateFile( const wxString &file )
{
	wxString path, fn;

	wxGetEnv( wxT( "PATH" ), &path );
	wxStringTokenizer tk( path, wxT( ":" ));
	while ( tk.HasMoreTokens() )
	{
		fn = tk.GetNextToken();
		if ( fn.Last() != wxFileName::GetPathSeparator()) fn << wxFileName::GetPathSeparator();
		fn << file;
		if ( wxFileName::FileExists( fn )) return fn;
	}
	return wxEmptyString;
}

/**************************************************************
***
**  prints a double into a string and cuts trailing zeros (leaving at least one)
***
***************************************************************/
const wxString printfDouble( const double &d )
{
	wxString s;
	s.Printf( wxT( "%.8f" ), d );
	int i = s.Len();
	wxChar dp = (localeconv()->decimal_point)[0];
	int c = s.Find( dp );
	if ( c > -1 )
	{
		while ( i > c+2 )
		{
			if ( s.GetChar( i-1 ) == '0' ) {
				s = s.Left( i-1 );
			}
			else break;
			i--;
		}
	}
	return s;
}
	
/**************************************************************
***
**  Reduces a given double value x modulo the double a (should be positive).
**  The return value is between 0 and a.
***
***************************************************************/
double a_red( const double &x, const double &a )
{
	assert( a > 0 );
	return( x - floor( x / a ) * a );
}

/**************************************************************
***
**   Reduces a given double value modulo 360.
**   The return value is between 0 and 360.
***
***************************************************************/
double red_deg( const double &input )
{
	return a_red( input, 360 );
}

/**************************************************************
***
**  Reduces a given integer modulo 12.
**  The return value is between 0 and 11.
***
***************************************************************/
int red12( const int &input )
{
	int ret = input %12;
	return ret >= 0 ? ret : ret + 12;
}

/**************************************************************
***
**  Reduces a given integer modulo 27.
**  The return value is between 0 and 26.
***
***************************************************************/
int red27( const int &input )
{
	int ret = input % 27;
	return ret >= 0 ? ret : ret + 27;
}

/**************************************************************
***
**     round a double
***
***************************************************************/
int a_rund( const double &v )
{
	return v >= 0 ? (int)(v+.5) : (int)(v-.5);
}

/**************************************************************
***
**    Conversion of strings to double.
**    Supports locale dependend decimal points
***
***************************************************************/
double myatof( const wxString &s )
{
	if ( ! s ) return 0;
	//assert( s );
	wxString err;
	wxChar tmp[256];
	wxStrcpy( tmp, s );
	wxChar *c = tmp;
	wxChar dp = (localeconv()->decimal_point)[0];

	while ( *c )
	{
		if ( *c == '.' && *c != dp ) *c = ',';
		if ( *c == ',' && *c != dp ) *c = '.';
		c++;
	}
	wxString s1 = tmp;
	double a = 0;
	if ( ! s1.ToDouble( &a ) && s != wxEmptyString )
	{
		err.Printf(
		    wxT( "Can't convert string to double, string was \"%s\", local decimal point \"%c\", converted string \"%s\", double is %f\n" ),
		    (const wxChar*)s, dp, c, a );
		Cout( err );
	}
	return a;
}

/**************************************************************
***
 * Returns the Greenwich mean time for a given julian date.
 * the return value is between 0 and 24. There's no formating like [hhmmss], etc.
***
***************************************************************/
double getTimeFromJD( const double &jd )
{
	double jjd = jd - .5;
	return a_red( ( jjd - (double)(int)jjd ) * 24, 24 );
}

/*****************************************************
**
**   weekday index for a given jd
**
******************************************************/
int getWeekDay( const double &jd )
{
	return (int)(jd + 1.5 ) % 7;
}

/*****************************************************
**
**   convertInt2WxPen
**
******************************************************/
int convertInt2WxPen( const int &istyle )
{
	int style;
	switch ( istyle )
	{
	case 1  :
		style = wxDOT;
		break;
	case 2  :
		style = wxLONG_DASH;
		break;
	case 3  :
		style = wxSHORT_DASH;
		break;
	case 4  :
		style = wxDOT_DASH;
		break;
	case 5  :
		style = wxTRANSPARENT;
		break;
	default :
		style = wxSOLID;
		break;
	}
	return style;
}

/*****************************************************
**
**   convertWxPen2Int
**
******************************************************/
int convertWxPen2Int( const int &style )
{
	int istyle;
	switch ( style )
	{
	case wxDOT         :
		istyle = 1;
		break;
	case wxLONG_DASH   :
		istyle = 2;
		break;
	case wxSHORT_DASH  :
		istyle = 3;
		break;
	case wxDOT_DASH    :
		istyle = 4;
		break;
	case wxTRANSPARENT :
		istyle = 5;
		break;
	default            :
		istyle = 0;
		break;
	}
	return istyle;
}

/*****************************************************
**
**   convertInt2WxBrush
**
******************************************************/
int convertInt2WxBrush( const int &istyle )
{
	int style;
	switch ( istyle )
	{
	case 1 :
		style = wxTRANSPARENT;
		break;
	case 2 :
		style = wxBDIAGONAL_HATCH;
		break;
	case 3 :
		style = wxCROSSDIAG_HATCH;
		break;
	case 4 :
		style = wxFDIAGONAL_HATCH;
		break;
	case 5 :
		style = wxCROSS_HATCH;
		break;
	case 6 :
		style = wxHORIZONTAL_HATCH;
		break;
	case 7 :
		style = wxVERTICAL_HATCH;
		break;
	case 8 :
		style = wxSTIPPLE;
		break;
	default:
		style = wxSOLID;
		break;
	}
	return style;
}

/*****************************************************
**
**   convertWxBrush2Int
**
******************************************************/
int convertWxBrush2Int( const int &style )
{
	int istyle;
	switch ( style )
	{
	case wxTRANSPARENT      :
		istyle = 1;
		break;
	case wxBDIAGONAL_HATCH  :
		istyle = 2;
		break;
	case wxCROSSDIAG_HATCH  :
		istyle = 3;
		break;
	case wxFDIAGONAL_HATCH  :
		istyle = 4;
		break;
	case wxCROSS_HATCH      :
		istyle = 5;
		break;
	case wxHORIZONTAL_HATCH :
		istyle = 6;
		break;
	case wxVERTICAL_HATCH   :
		istyle = 7;
		break;
	case wxSTIPPLE          :
		istyle = 8;
		break;
	default                 :
		istyle = 0;
		break;
	}
	return istyle;
}
