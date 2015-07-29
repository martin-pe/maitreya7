/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/DialogElements.h
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


#ifndef _DIALOGELEMENTS_H_
#define _DIALOGELEMENTS_H_

#ifdef __GNUG__
#pragma interface "DialogElements.h"
#endif

#include <wx/choice.h>
#include <wx/event.h>
#include <wx/grid.h>
#include <wx/panel.h>

#include <vector>

using namespace std;
class wxStaticText;

/*************************************************//**
*
* \brief basic grid class adding some mouse and key events to wxGrid
*
******************************************************/
class MyGrid : public wxGrid
{
	DECLARE_CLASS( ChildWindow )

public:
	MyGrid( wxWindow* parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition,
	        const wxSize& size = wxDefaultSize, long style = wxWANTS_CHARS, const wxString& name = wxPanelNameStr);

protected:
	void OnMouseEvent( wxMouseEvent& );
	void OnKeyDown( wxKeyEvent& );

};

/*************************************************//**
*
* \brief filter choice for Uranian astrology
*
******************************************************/
class FilterChoice : public wxChoice
{
public:
	FilterChoice( wxWindow *parent, int id );
	void recreate( const vector<int>& );
	int getFilter();

protected:
	vector<int> plist;
};

/*************************************************//**
*
* \brief choice for circle types
*
******************************************************/
class CircleTypeChoice : public wxChoice
{
public:
	CircleTypeChoice( wxWindow *parent, int id, int selected = 0 );
	virtual wxSize DoGetBestSize() const;
};

/*************************************************//**
*
* \brief choice for year length
*
******************************************************/
class YearLengthChoice : public wxChoice
{
public:
	YearLengthChoice( wxWindow *parent, int id, const bool vedic, int selected = 0 );
	double getYearLength();

private:
	bool vedic;
};

/*************************************************//**
*
* \brief choice for sort orders in Uranian astrology
*
******************************************************/
class SortChoice : public wxChoice
{
public:
	SortChoice( wxWindow *parent, int id, int selected = 0 );
};

/*************************************************//**
*
* \brief choice with transit modes for TransitView
*
******************************************************/
class TransitModeChoice : public wxChoice
{
public:
	TransitModeChoice( wxWindow *parent, int id, int selected = 0 );
};

/*************************************************//**
*
* \brief choice box with Ayanamsa items
*
******************************************************/
class AyanamsaChoice : public wxChoice
{
public:
	AyanamsaChoice( wxWindow *parent, int id, int ayatype );
	int getConfigIndex();
};

/*************************************************//**
*
* \brief simple text label for toolbar
*
******************************************************/
class ToolbarLabel : public wxControl
{
	DECLARE_CLASS( ToolbarLabel )
public:

	ToolbarLabel( wxWindow *parent, wxWindowID id, wxPoint pos, wxSize size, wxString tooltip );
	void SetLabel( wxString );
	virtual wxSize DoGetBestSize() const;

private:
	wxStaticText *theText;
};

/*************************************************//**
*
* 
*
******************************************************/
class MultiLineStaticText : public wxPanel
{
	DECLARE_CLASS( MultiLineStaticText )
public:

	MultiLineStaticText( wxWindow *parent, wxWindowID id, wxPoint pos = wxDefaultPosition, wxSize size = wxDefaultSize );
	void SetLabel( wxString );
	wxString GetLabel();

private:
	void OnSize( wxSizeEvent& );
	wxStaticText *text;
};

#endif


