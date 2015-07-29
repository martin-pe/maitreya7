/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/UranianView.h
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

#ifndef _URANIANVIEW_H_
#define _URANIANVIEW_H_

#ifdef __GNUG__
#pragma interface "UranianView.h"
#endif

#include "TextView.h"

class UranianConfig;

/*************************************************//**
*
*  view for Uranian astrology (midpoint analysis)
*
******************************************************/
class UranianView : public TextViewBase
{
	DECLARE_CLASS( UranianView )

public:

	UranianView( wxWindow *parent, ChildWindow *frame, Document *doc, const bool &vedic );
	~UranianView();

	virtual wxString getWindowLabel( const bool shortname = false );

protected:

	virtual void OnDataChanged();
	virtual void write();
	virtual void OnToolbarCommand();
	virtual void initToolItems();
	void OnIdle( wxIdleEvent& );

	bool updateView;
	int mode, year;
	UranianConfig *cfg;

};

#endif

