/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/AspectPanel.h
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

#ifndef ASPECTPANEL_H
#define ASPECTPANEL_H

#ifdef __GNUG__
#pragma interface "AspectPanel.h"
#endif

#include "ConfigPanel.h"

#include "Aspect.h"

// begin wxGlade: dependencies
// end wxGlade

class wxButton;
class wxCheckBox;
class wxChoice;
class wxSpinCtrl;
class wxSpinEvent;
class wxStaticBox;
class wxStaticText;

/*************************************************//**
*
* \brief scrolled panel containing parameters for single aspects
*
******************************************************/
class AspectOrbisPanel: public wxPanel
{
	DECLARE_CLASS( AspectOrbisPanel )

public:
	AspectOrbisPanel( wxWindow* parent, int id, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=0 );

	int getOrbis( const int &item );
	void setOrbis( const int &item, const int &value );
	bool isActive( const int &item );
	void setActive( const int &item, const bool = true );

protected:
	wxCheckBox* check_aspect[MAX_ASPECT_TYPES];
	wxSpinCtrl* spin_orbis[MAX_ASPECT_TYPES];
};

/*************************************************//**
*
* \brief ConfigPanel for aspect parameters
*
******************************************************/
class AspectPanel: public ConfigPanel
{
	DECLARE_CLASS( AspectPanel )

public:
    // begin wxGlade: AspectPanel::ids
    // end wxGlade

	AspectPanel( wxWindow* parent );
	virtual void setData();
	virtual bool saveData();

private:
    // begin wxGlade: AspectPanel::methods
    void set_properties();
    void do_layout();
    // end wxGlade

	void OnDefaultButton( wxCommandEvent& );
	void OnSpinOrbis( wxSpinEvent& );
	void OnCheckAspect( wxCommandEvent& );
	void updateControls();

	AspectConfig configs[MAX_ASPECT_TYPES];
	int selconfig;

protected:
    // begin wxGlade: AspectPanel::attributes
    wxStaticBox* sizer_aspects_staticbox;
    AspectOrbisPanel* aspect_list;
    wxButton* button_default_copy;
    // end wxGlade
}; // wxGlade: end class


#endif // ASPECTPANEL_H
