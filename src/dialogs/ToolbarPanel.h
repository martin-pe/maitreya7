/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/ToolbarPanel.h
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

#ifndef TOOLBARPANEL_H
#define TOOLBARPANEL_H

#ifdef __GNUG__
#pragma interface "ToolbarPanel.h"
#endif

#include "ConfigPanel.h"

// begin wxGlade: dependencies
// end wxGlade

class ToolBarCommand;
class wxBitmapButton;
class wxButton;
class wxImageList;
class wxListEvent;
class wxListCtrl;
class wxStaticBox;
class wxStaticLine;
class wxStaticText;
class wxTextCtrl;

/*************************************************//**
*
* \brief ConfigPanel for toolbar parameters
*
******************************************************/
class ToolbarPanel : public ConfigPanel
{
	DECLARE_CLASS( ToolbarPanel )

public:
    // begin wxGlade: ToolbarPanel::ids
    // end wxGlade

	ToolbarPanel( wxWindow* parent );
	~ToolbarPanel();

	virtual void setData();
	virtual bool saveData();

private:
    // begin wxGlade: ToolbarPanel::methods
    void set_properties();
    void do_layout();
    // end wxGlade

	void OnLeft( wxCommandEvent& );
	void OnRight( wxCommandEvent& );
	void OnUp( wxCommandEvent& );
	void OnDown( wxCommandEvent& );

	void OnDefault( wxCommandEvent& );

	void OnLeftSelected( wxListEvent& );
	void OnRightSelected( wxListEvent& );
	void initRightControl( const bool useDefault = false );

	wxImageList *imageList;
	bool dirty;
	int left_sel, right_sel;

protected:
    // begin wxGlade: ToolbarPanel::attributes
    wxStaticBox* sizer_selected_staticbox;
    wxStaticBox* sizer_available_staticbox;
    wxListCtrl* list1;
    wxBitmapButton* button_right;
    wxBitmapButton* button_left;
    wxBitmapButton* button_up;
    wxBitmapButton* button_down;
    wxListCtrl* list2;
    wxButton* button_default;
    // end wxGlade
}; // wxGlade: end class


#endif // TOOLBARPANEL_H
