/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/CommonInputFields.h
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


#ifndef _COMMONINPUTFIELDS_H_
#define _COMMONINPUTFIELDS_H_

#ifdef __GNUG__
#pragma interface "CommonInputFields.h"
#endif

#include <wx/textctrl.h>

/*************************************************//**
*
* 
*
******************************************************/
class BaseInputField : public wxTextCtrl
{
	DECLARE_CLASS( BaseInputField )

public:
	BaseInputField( wxWindow *parent, 
		int id = -1, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize );

	void resetStoredValue();
	virtual double getDoubleValue() = 0;
	virtual void setDoubleValue( const double& ) = 0;
	virtual void formatValue() = 0;

protected:
	void OnKillFocus( wxFocusEvent& );

	double value;

private:

};

/*************************************************//**
*
* 
*
******************************************************/
class DegMinSecInputField : public BaseInputField
{
	DECLARE_CLASS( DegMinSecInputField )

public:
	DegMinSecInputField( wxWindow *parent, 
		int id = -1, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize );

	virtual double getDoubleValue();
	virtual void setDoubleValue( const double& );

protected:
	virtual void formatValue();
	const int format;
};

/*************************************************//**
*
* 
*
******************************************************/
class DegreeInputField : public DegMinSecInputField
{
	DECLARE_CLASS( DegreeInputField )
public:
	DegreeInputField( wxWindow *parent, int id = -1, const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize );
};

/*************************************************//**
*
* 
*
******************************************************/
class LongitudeInputField : public DegMinSecInputField
{
	DECLARE_CLASS( LongitudeInputField )
public:
	LongitudeInputField( wxWindow *parent, int id = -1, const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize );
};

/*************************************************//**
*
* 
*
******************************************************/
class LatitudeInputField : public DegMinSecInputField
{
	DECLARE_CLASS( LatitudeInputField )
public:
	LatitudeInputField( wxWindow *parent, int id = -1, const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize );
};

/*************************************************//**
*
* 
*
******************************************************/
class TimeInputField : public DegMinSecInputField
{
	DECLARE_CLASS( TimeInputField )
public:
	TimeInputField( wxWindow *parent, int id = -1, const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize );
};

/*************************************************//**
*
* 
*
******************************************************/
class DateInputField : public BaseInputField
{
	DECLARE_CLASS( DateInputField )

public:
	DateInputField( wxWindow *parent, int id = -1, const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize );

	virtual double getDoubleValue();
	virtual void setDoubleValue( const double& );

protected:
	virtual void formatValue();
};

/*************************************************//**
*
* 
*
******************************************************/
class DoubleInputField : public BaseInputField
{
	DECLARE_CLASS( DoubleInputField )

public:
	DoubleInputField( wxWindow *parent, int id = -1, const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize );

	virtual double getDoubleValue();
	virtual void setDoubleValue( const double& );

protected:
	virtual void formatValue();
};


/*************************************************//**
*
* 
*
******************************************************/
class IntegerInputField : public BaseInputField
{
	DECLARE_CLASS( IntegerInputField )

public:
	IntegerInputField( wxWindow *parent, int id = -1, const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize );

	virtual double getDoubleValue();
	virtual void setDoubleValue( const double& );

protected:
	virtual void formatValue();
};

#endif


