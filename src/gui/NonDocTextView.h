/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/NonDocTextView.h
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

#ifndef _NONDOCTEXTVIEW_H_
#define _NONDOCTEXTVIEW_H_

#ifdef __GNUG__
#pragma interface "NonDocTextView.h"
#endif

#include <wx/string.h>

#include "TextView.h"
#include "BasicView.h"

class EclipseExpert;
class HoraExpert;
class Writer;

/*************************************************//**
*
* \brief view containing text output for eclipses
*
******************************************************/
class EclipseView : public TextViewBase
{
	DECLARE_CLASS( EclipseView )

public:

	EclipseView( wxWindow *parent, ChildWindow *frame, const bool &vedic );
	~EclipseView();

	virtual wxString getWindowLabel( const bool shortname = false );

protected:

	virtual void OnToolbarCommand();
	virtual void initToolItems();
	virtual void write();

	void OnIdle( wxIdleEvent& );

	int startyear, years, type;
	bool updateView;
	int isLocaltime;
	EclipseExpert *expert;
};

/*************************************************//**
*
* \brief view containing text output for Hora
*
******************************************************/
class HoraView : public TextViewBase
{
	DECLARE_CLASS( HoraView )
public:

	HoraView( wxWindow *parent, ChildWindow *frame );
	~HoraView();

	virtual wxString getWindowLabel( const bool shortname = false );

protected:

	virtual void write();
	virtual void OnToolbarCommand();
	virtual void initToolItems();

	void OnDateCtrlChanged( wxCommandEvent& );
	void OnNow( wxCommandEvent& );

	HoraExpert *expert;
	bool isLocaltime;
	double jd;

};

#endif

