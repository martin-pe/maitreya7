/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/TextView.h
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

#ifndef _TEXTVIEW_H_
#define _TEXTVIEW_H_

#ifdef __GNUG__
#pragma interface "TextView.h"
#endif

#include <wx/string.h>

#include "BasicView.h"

class ChildWindow;
class DataSet;
class TextWidget;
class Writer;

/*************************************************//**
*
* \brief base class for text views
*
******************************************************/
class TextViewBase : public BasicView
{
	DECLARE_CLASS( TextViewBase )
public:
	TextViewBase( wxWindow *parent, ChildWindow *frame, Document *doc, const int &viewId, const bool &toolpanel, const bool &vedic );

	virtual void OnDataChanged();
protected:

	virtual bool dispatchCommand( const int &command );

	virtual void write() = 0;
	TextWidget *theText;

};

/*************************************************//**
*
* \brief view for various kinds of text output including toolbar controls
*
******************************************************/
class DefaultTextView : public TextViewBase
{
	DECLARE_CLASS( DefaultTextView )
public:
	DefaultTextView( wxWindow *parent, ChildWindow *frame, Document *doc, const bool &toolpanel, const bool &vedic, const int &mode );
	//~DefaultTextView();

	virtual wxString getWindowLabel( const bool shortname = false );
	virtual bool supportsEWToggle() const;

protected:
	int mode, varga;
	unsigned int dasa;
	virtual void OnToolbarCommand();
	virtual void write();
	virtual void initToolItems();

};

/*************************************************//**
*
* \brief this is the main view for a chart in single view mode
*
******************************************************/
class PlanetListView : public TextViewBase
{
	DECLARE_CLASS( PlanetListView )
public:
	PlanetListView( wxWindow *parent, ChildWindow *frame, Document *doc, const bool &vedic );
	virtual wxString getWindowLabel( const bool shortname = false );

	~PlanetListView();

protected:
	virtual void write();

};

#endif

