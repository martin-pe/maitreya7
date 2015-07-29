/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/CommonInputFields.cpp
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
#pragma implementation "CommonInputFields.h"
#endif

#include "CommonInputFields.h"

#include "DataSet.h"
#include "guivalidators.h"
#include "Lang.h"
#include "mathbase.h"

IMPLEMENT_CLASS( BaseInputField, wxTextCtrl )
IMPLEMENT_CLASS( DegMinSecInputField, BaseInputField )
IMPLEMENT_CLASS( DegreeInputField, DegMinSecInputField )
IMPLEMENT_CLASS( LongitudeInputField, DegMinSecInputField )
IMPLEMENT_CLASS( LatitudeInputField, DegMinSecInputField )
IMPLEMENT_CLASS( TimeInputField, DegMinSecInputField )
IMPLEMENT_CLASS( DateInputField, BaseInputField )
IMPLEMENT_CLASS( DoubleInputField, BaseInputField )
IMPLEMENT_CLASS( IntegerInputField, BaseInputField )

/*****************************************************
**
**   BaseInputField   ---   Constructor
**
******************************************************/
BaseInputField::BaseInputField( wxWindow *parent,
	int id, const wxPoint& pos, const wxSize& size )
 : wxTextCtrl( parent, id, wxEmptyString, pos, size, wxTE_PROCESS_ENTER )
{
	value = 0;
	Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( BaseInputField::OnKillFocus ));
}

/*****************************************************
**
**   BaseInputField   ---   OnKillFocus
**
******************************************************/
void BaseInputField::OnKillFocus( wxFocusEvent &event )
{
	if( Validate()) formatValue();
	else resetStoredValue();
}

/*****************************************************
**
**   BaseInputField   ---   resetStoredValue
**
******************************************************/
void BaseInputField::resetStoredValue()
{
	setDoubleValue( value );
}

/*****************************************************
**
**   DegMinSecInputField   ---   Constructor
**
******************************************************/
DegMinSecInputField::DegMinSecInputField( wxWindow *parent, 
	int id, const wxPoint& pos, const wxSize& size )
 : BaseInputField( parent, id, pos, size ),
	 format( DEG_PRECISION_FLEXIBLE )
{
}

/*****************************************************
**
**   DegMinSecInputField   ---   setDoubleValue
**
******************************************************/
void DegMinSecInputField::setDoubleValue( const double &v )
{
	Formatter *formatter = Formatter::get();
	value = v;
	SetValue( formatter->getDegreesFormatted( v, format ));
}

/*****************************************************
**
**   DegMinSecInputField   ---   formatValue
**
******************************************************/
void DegMinSecInputField::formatValue()
{
	Formatter *formatter = Formatter::get();
	if ( Validate())
	{
		value = getDoubleValue();
		SetValue( formatter->getDegreesFormatted( value, format ));
	}
	else
	{
		SetValue( formatter->getDegreesFormatted( value, format ));
	}
}

/*****************************************************
**
**   DegMinSecInputField   ---   getDoubleValue
**
******************************************************/
double DegMinSecInputField::getDoubleValue()
{
	Formatter *formatter = Formatter::get();
	int deg, min, sec;

	if ( GetValue().IsEmpty()) return 0.0;

	const bool b = formatter->getDegreeIntsFromString( (const wxChar*)GetValue(), deg, min, sec );
	if ( ! b )
	{
		printf( "DegMinSecInputField::getDoubleValue WARN: invalid string %s\n", str2char( GetValue()));
		return value;
	}
	double v = sec + min * 60 + deg * 3600;
	return v / 3600.0;
}

/*****************************************************
**
**   DegreeInputField   ---   Constructor
**
******************************************************/
DegreeInputField::DegreeInputField( wxWindow *parent, int id, const wxPoint& pos, const wxSize& size )
 : DegMinSecInputField( parent, id, pos, size )
{
	SetValidator( DegreeValidator( 0, 360 ));
}

/*****************************************************
**
**   LongitudeInputField   ---   Constructor
**
******************************************************/
LongitudeInputField::LongitudeInputField( wxWindow *parent, int id, const wxPoint& pos, const wxSize& size )
 : DegMinSecInputField( parent, id, pos, size )
{
	SetValidator( DegreeValidator( 0, 180 ));
}

/*****************************************************
**
**   LatitudeInputField   ---   Constructor
**
******************************************************/
LatitudeInputField::LatitudeInputField( wxWindow *parent, int id, const wxPoint& pos, const wxSize& size )
 : DegMinSecInputField( parent, id, pos, size )
{
	SetValidator( DegreeValidator( 0, 90 ));
}

/*****************************************************
**
**   TimeInputField   ---   Constructor
**
******************************************************/
TimeInputField::TimeInputField( wxWindow *parent, int id, const wxPoint& pos, const wxSize& size )
 : DegMinSecInputField( parent, id, pos, size )
{
	SetValidator( DegreeValidator( 0, 24 ));
}

/*****************************************************
**
**   DateInputField   ---   Constructor
**
******************************************************/
DateInputField::DateInputField( wxWindow *parent, int id, const wxPoint& pos, const wxSize& size )
 : BaseInputField( parent, id, pos, size )
{
	SetValidator( DateValidator());
}

/*****************************************************
**
**   DateInputField   ---   setDoubleValue
**
******************************************************/
void DateInputField::setDoubleValue( const double &v )
{
	Formatter *formatter = Formatter::get();
	value = v;
	SetValue( formatter->getDateStringFromJD( v ));
}

/*****************************************************
**
**   DateInputField   ---   formatValue
**
******************************************************/
void DateInputField::formatValue()
{
	Formatter *formatter = Formatter::get();
	if ( Validate())
	{
		value = getDoubleValue();
		SetValue( formatter->getDateStringFromJD( value ));
	}
	else
	{
		SetValue( formatter->getDateStringFromJD( value ));
	}
}

/*****************************************************
**
**   DateInputField   ---   getDoubleValue
**
******************************************************/
double DateInputField::getDoubleValue()
{
	Formatter *formatter = Formatter::get();
	int day, month, year;

	if ( Validate())
	{
		if ( formatter->getDateIntsFromString( (const wxChar*)GetValue(), day, month, year ))
		{
			JDate date;
			date.setDate( day, month, year, 0 );
			return date.getJD();
		}
	}
	return value;
}

/*****************************************************
**
**   DoubleInputField   ---   Constructor
**
******************************************************/
DoubleInputField::DoubleInputField( wxWindow *parent, int id, const wxPoint& pos, const wxSize& size )
 : BaseInputField( parent, id, pos, size )
{
	SetValidator( DoubleValidator());
}

/*****************************************************
**
**   DoubleInputField   ---   setDoubleValue
**
******************************************************/
void DoubleInputField::setDoubleValue( const double &v )
{
	value = v;
	SetValue( printfDouble( value ));
}

/*****************************************************
**
**   DoubleInputField   ---   formatValue
**
******************************************************/
void DoubleInputField::formatValue()
{
	value = getDoubleValue();
	setDoubleValue( value );
}

/*****************************************************
**
**   DoubleInputField   ---   getDoubleValue
**
******************************************************/
double DoubleInputField::getDoubleValue()
{
	value = myatof( GetValue() );
	return value;
}

/*****************************************************
**
**   IntegerInputField   ---   Constructor
**
******************************************************/
IntegerInputField::IntegerInputField( wxWindow *parent, int id, const wxPoint& pos, const wxSize& size )
 : BaseInputField( parent, id, pos, size )
{
	SetValidator( DoubleValidator());
}

/*****************************************************
**
**   IntegerInputField   ---   setDoubleValue
**
******************************************************/
void IntegerInputField::setDoubleValue( const double &v )
{
	value = v;
	SetValue( wxString::Format( wxT( "%d" ), (int)value ));
}

/*****************************************************
**
**   IntegerInputField   ---   formatValue
**
******************************************************/
void IntegerInputField::formatValue()
{
	value = getDoubleValue();
	setDoubleValue( value );
}

/*****************************************************
**
**   IntegerInputField   ---   getDoubleValue
**
******************************************************/
double IntegerInputField::getDoubleValue()
{
	value = (int)myatof( GetValue() );
	return value;
}


