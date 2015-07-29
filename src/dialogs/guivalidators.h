/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/guivalidators.h
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


#ifndef _GUIVALIDATORS_H
#define _GUIVALIDATORS_H

#ifdef __GNUG__
#pragma interface "guivalidators.h"
#endif

#include <wx/valtext.h>

BEGIN_DECLARE_EVENT_TYPES()
DECLARE_EVENT_TYPE(COMMAND_TEXT_RETURN, wxID_HIGHEST + 8001 )
END_DECLARE_EVENT_TYPES()

/*************************************************//**
*
* 
*
******************************************************/
class BaseTextValidator : public wxTextValidator
{
	DECLARE_CLASS( BaseTextValidator )

public:
	BaseTextValidator( const long style = wxFILTER_INCLUDE_CHAR_LIST );
	BaseTextValidator( const BaseTextValidator& );

protected:

	void OnChar(wxKeyEvent& );
	void setWindowBackground( const bool& );
	virtual bool Validate( wxWindow* parent ) = 0;

private:
	bool isInCharIncludes( const wxString& );
	bool isNotInCharExcludes( const wxString& );
};

/*************************************************//**
*
* \brief validator for degree input fields in dialogs
*
******************************************************/
class DegreeValidator : public BaseTextValidator
{
	DECLARE_CLASS( DegreeValidator )

public:
	DegreeValidator( const double &minvalue, const double &maxvalue );
	DegreeValidator( const DegreeValidator& );

protected:

	void setup();
	virtual bool Validate( wxWindow* parent );
	virtual wxObject *Clone() const;

	const double minvalue, maxvalue;
};

/*************************************************//**
*
* \brief validator for text input fields with format double
*
******************************************************/
class DoubleValidator : public BaseTextValidator
{
	DECLARE_CLASS( DoubleValidator )

public:
	DoubleValidator( const double minvalue = 0, const double maxvalue = -1 );
	DoubleValidator( const DoubleValidator& );

protected:

	void setup();
	virtual bool Validate( wxWindow* parent );
	virtual wxObject *Clone() const;

	const double minvalue, maxvalue;
};

/*************************************************//**
*
* 
*
******************************************************/
class DateValidator : public BaseTextValidator
{
	DECLARE_CLASS( DateValidator )

public:
	DateValidator();
	DateValidator( const DateValidator& );

protected:

	void setup();
	virtual bool Validate( wxWindow* parent );
	virtual wxObject *Clone() const;
};

/*************************************************//**
*
* 
*
******************************************************/
class FilenameValidator : public BaseTextValidator
{
	DECLARE_CLASS( FilenameValidator )

public:
	FilenameValidator();
	FilenameValidator( const FilenameValidator& );

protected:

	virtual bool Validate( wxWindow* parent );
	virtual wxObject *Clone() const;
};

/*************************************************//**
*
* 
*
******************************************************/
class DirectoryValidator : public BaseTextValidator
{
	DECLARE_CLASS( DirectoryValidator )

public:
	DirectoryValidator() : BaseTextValidator( wxFILTER_ASCII ) {}
	DirectoryValidator( const DirectoryValidator &v ) : BaseTextValidator( v ) {}

protected:

	virtual bool Validate( wxWindow* parent );
	virtual wxObject *Clone() const { return new DirectoryValidator(); }
};

/*************************************************//**
*
* 
*
******************************************************/
class NumericValidator : public BaseTextValidator
{
	DECLARE_CLASS( NumericValidator )

public:
	NumericValidator( const long minvalue = 0, const long maxvalue = -1 );
	NumericValidator( const NumericValidator &v );

protected:

	virtual bool Validate( wxWindow* parent );
	virtual wxObject *Clone() const { return new NumericValidator( minvalue, maxvalue ); }

	const long minvalue, maxvalue;
};

#endif


