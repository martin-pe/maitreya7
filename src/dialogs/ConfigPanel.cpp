/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/ConfigPanel.cpp
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
#pragma implementation "ConfigPanel.h"
#endif

#include "ConfigPanel.h"

IMPLEMENT_CLASS( ConfigPanel, wxPanel )

/*****************************************************
**
**   ConfigPanel   ---   Constructor
**
******************************************************/
ConfigPanel::ConfigPanel( wxWindow* parent )
		: wxPanel( parent, -1 )
{
	dirty = false;
}

/*****************************************************
**
**   ConfigPanel   ---   OnSize
**
******************************************************/
void ConfigPanel::OnSize( wxSizeEvent &event )
{
	//int a = event.GetSize().GetWidth();
	//int b = event.GetSize().GetHeight();
	event.Skip();
}

/*****************************************************
**
**   ConfigPanel   ---   OnChoice
**
******************************************************/
void ConfigPanel::OnChoice( wxCommandEvent& )
{
	dirty = true;
}

/*****************************************************
**
**   ConfigPanelFactory   ---   getPanelName
**
******************************************************/
wxString ConfigPanelFactory::getPanelName( const int &id )
{
	assert( id >= 0 && id < NB_PANELS );
	const wxChar *notebook_title[NB_PANELS] =
	{
		// General section
		_("General"), _("Default Location"), _( "Multiple Views" ), _( "Atlas" ),
		_( "Ephemeris" ), _( "Print" ), _("Animation"),

		// View section
		_("View"), _( "Fonts"), _( "Colors" ), _( "Object Colors" ),
		_( "Dasa Colors" ), _( "Aspect Display" ), _( "Toolbar" ),

		// Vedic section
		_( "Vedic Astrology" ), _("Chart"), _("Calculation"), _( "Vargas and Dasas" ),

		// Western section
		_( "Western Astrology" ), _("Chart"), _("Calculation"),  _("Aspects")
	};
	return notebook_title[id];
}

/*****************************************************
**
**   ConfigPanelFactory   ---   createPanel
**
******************************************************/
ConfigPanel *ConfigPanelFactory::createPanel( const int &id, wxWindow *parent )
{
	assert( id >= 0 && id < NB_PANELS );
	ConfigPanel *panel;
	switch ( id )
	{
	case 0:
		panel = createGeneralPanel( parent );
		break;
	case 1:
		panel = createDefaultLocationPanel( parent );
		break;
	case 2:
		panel = createMultipleViewPanel( parent );
		break;
	case 3:
		panel = createAtlasPanel( parent );
		break;
	case 4:
		panel = createEphemerisPanel( parent );
		break;
	case 5:
		panel = createPrintoutPanel( parent );
		break;
	case 6:
		panel = createAnimationPanel( parent );
		break;
	case 7:
		panel = createViewPanel( parent );
		break;
	case 8:
		panel = createFontPanel( parent );
		break;
	case 9:
		panel = createColorPanel( parent );
		break;
	case 10:
		panel = createObjectColorPanel( parent );
		break;
	case 11:
		panel = createDasaColorPanel( parent );
		break;
	case 12:
		panel = createAspectColorPanel( parent );
		break;
	case 13:
		panel = createToolbarPanel( parent );
		break;
	case 14:
		panel = createVedicPanel( parent );
		break;
	case 15:
		panel = createVedicChartPanel( parent );
		break;
	case 16:
		panel = createVedicCalculationPanel( parent );
		break;
	case 17:
		panel = createVargaPanel( parent );
		break;
	case 18:
		panel = createWesternPanel( parent );
		break;
	case 19:
		panel = createWesternChartPanel( parent );
		break;
	case 20:
		panel = createWesternCalculationPanel( parent );
		break;
	case 21:
		panel = createAspectPanel( parent );
		break;
	default:
		assert( false );
		break;
	}
	assert( panel );
	return panel;
}


