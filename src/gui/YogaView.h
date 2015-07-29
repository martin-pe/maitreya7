/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/YogaView.h
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

#ifndef _YOGAVIEW_H_
#define _YOGAVIEW_H_

#ifdef __GNUG__
#pragma interface "YogaView.h"
#endif

#include <wx/string.h>

#include "TextView.h"

class ChildWindow;
class DataSet;
class TextWidget;
class Writer;

/*************************************************//**
*
* \brief view for Yogas including text view and toolbar controls
*
******************************************************/
class YogaView : public TextViewBase
{
	DECLARE_CLASS( YogaView )
public:
	YogaView( wxWindow *parent, ChildWindow *frame, Document *doc );
	~YogaView();

	virtual wxString getWindowLabel( const bool shortname = false );

protected:
	int varga, groupfilter, firsthouse, sourcefilter;

	virtual void OnToolbarCommand();
	virtual void write();
	virtual void initToolItems();

};

#endif

