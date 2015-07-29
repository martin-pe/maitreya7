/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/ConfigDialog.cpp
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
#pragma implementation "ConfigDialog.h"
#endif

#include "ConfigDialog.h"

#include <wx/app.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/statline.h>
#include <wx/treebook.h>

#include "Conf.h"
#include "IconProvider.h"
#include "guibase.h"

enum { CONFIG_NOTEBOOK = wxID_HIGHEST + 3000 };

IMPLEMENT_CLASS( ConfigBaseDialog, wxDialog )
IMPLEMENT_CLASS( ConfigDialog, ConfigBaseDialog )
IMPLEMENT_CLASS( SimpleConfigDialog, ConfigBaseDialog )

DEFINE_EVENT_TYPE( CONFIG_CHANGED )
DEFINE_EVENT_TYPE( CONFIG_TOOLBAR_CHANGED )

extern Config *config;

/*****************************************************
**
**   SimpleConfigDialog   ---   Constructor
**
******************************************************/
SimpleConfigDialog::SimpleConfigDialog( const int &id, wxWindow* parent )
		:  ConfigBaseDialog( parent )
{
	ConfigPanelFactory factory;

	wxBoxSizer* sizer_main = new wxBoxSizer( wxVERTICAL );
	configpanel = factory.createPanel( id, this );
	configpanel->setData();

	sizer_main->Add( configpanel, 1, wxALL|wxEXPAND, 3);
	sizer_main->Add( new wxStaticLine(this, -1), 0, wxEXPAND, 0);
	sizer_main->Add(buttonsizer, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxFIXED_MINSIZE , 3);
	SetAutoLayout( true );
	SetSizer( sizer_main );
	Layout();
	SetIcon( IconProvider::getIcon( BITMAP_CONFIG ));

	wxString title;
	title << _( "Configuration" ) << wxT( ": " ) << factory.getPanelName( id );
	SetTitle( title );

	SetSize( config->xSimpleConfigDialog, config->ySimpleConfigDialog );
	Connect( wxID_APPLY, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SimpleConfigDialog::OnApply ));
}

/*****************************************************
**
**   SimpleConfigDialog   ---   Destructor
**
******************************************************/
SimpleConfigDialog::~SimpleConfigDialog()
{
	config->xSimpleConfigDialog = GetSize().GetWidth();
	config->ySimpleConfigDialog = GetSize().GetHeight();
}

/*****************************************************
**
**   SimpleConfigDialog   ---   saveData
**
******************************************************/
void SimpleConfigDialog::saveData()
{
	configpanel->saveData();
}

/*****************************************************
**
**   SimpleConfigDialog   ---   OnApply
**
******************************************************/
void SimpleConfigDialog::OnApply( wxCommandEvent &event )
{
	saveData();
	wxCommandEvent e( CONFIG_CHANGED, GetId() );
	wxPostEvent( GetParent(), e );
}

/*****************************************************
**
**   ConfigBaseDialog   ---   Constructor
**
******************************************************/
ConfigBaseDialog::ConfigBaseDialog( wxWindow* parent )
		:  wxDialog( parent, -1, _( "Configuration" ), wxDefaultPosition, wxDefaultSize, DEFAULT_DIALOG_STYLE )
{
	buttonsizer = new wxBoxSizer( wxHORIZONTAL );
	buttonsizer->Add( new wxButton(this, wxID_OK, _("OK")), 0, wxALL, 3);
	buttonsizer->Add( new wxButton(this, wxID_APPLY, _("Apply")), 0, wxALL, 3);
	buttonsizer->Add( new wxButton(this, wxID_CANCEL, _("Cancel")), 0, wxALL, 3);

	Connect( wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigBaseDialog::OnOK ));
	Connect( wxID_CANCEL, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigBaseDialog::OnCancel ));
}

/*****************************************************
**
**   ConfigDialog   ---   Constructor
**
******************************************************/
ConfigDialog::ConfigDialog(wxWindow* parent )
		:  ConfigBaseDialog( parent )
{
	ConfigPanelFactory factory;
	const static int page_types[NB_PANELS] = {
		1, 0, 0, 0, 0, 0, 0,  // General
		1, 0, 0, 0, 0, 0, 0,  // View
		1, 0, 0, 0,           // Vedic
		1, 0, 0, 0            // Western
	};
	wxString title;

	wxBoxSizer* sizer_main = new wxBoxSizer( wxVERTICAL );
	notebook = new wxTreebook( this, CONFIG_NOTEBOOK );
	for ( int i = CONFIGPANEL_FIRST; i <= CONFIGPANEL_LAST; i++ )
	{
		title = factory.getPanelName( i );
		configpanel[i] = 0;
		panel[i] = new wxPanel( notebook, -1 );
		if ( page_types[i] )
		{
			notebook->AddPage( panel[i], title );
		}
		else
		{
			notebook->AddSubPage( panel[i], title );
		}
	}

	sizer_main->Add( notebook, 1, wxALL|wxEXPAND, 3);
	sizer_main->Add( new wxStaticLine(this, -1), 0, wxEXPAND, 0);
	sizer_main->Add(buttonsizer, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxFIXED_MINSIZE , 3);
	SetAutoLayout( true );
	SetSizer( sizer_main );
	Layout();
	SetIcon( IconProvider::getIcon( BITMAP_CONFIG ));

	SetSize( config->xConfigDialog, config->yConfigDialog );

	Connect( wxID_APPLY, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigDialog::OnApply ));
#ifdef _WX29
	Connect( CONFIG_NOTEBOOK, wxEVT_COMMAND_TREEBOOK_PAGE_CHANGED, wxBookCtrlEventHandler( ConfigDialog::OnTreebook ));
#else
	Connect( CONFIG_NOTEBOOK, wxEVT_COMMAND_TREEBOOK_PAGE_CHANGED, wxTreebookEventHandler( ConfigDialog::OnTreebook ));
#endif

	// Uncomment to expand all base nodes
	//for ( int i = CONFIGPANEL_FIRST; i <= CONFIGPANEL_LAST; i++ ) notebook->ExpandNode( i, true );
	notebook->SetSelection( config->configDialogActivePage );
}

/*****************************************************
**
**   ConfigDialog   ---   Destructor
**
******************************************************/
ConfigDialog::~ConfigDialog()
{
	config->configDialogActivePage = notebook->GetSelection();
	config->xConfigDialog = GetSize().GetWidth();
	config->yConfigDialog = GetSize().GetHeight();
}

/*****************************************************
**
**   ConfigDialog   ---   saveData
**
******************************************************/
void ConfigDialog::saveData()
{
	for ( int i = 0; i < NB_PANELS; i++ )
	{
		if ( configpanel[i] && configpanel[i]->saveData() &&  i == CONFIGPANEL_TOOLBAR )
		{
			wxCommandEvent e( CONFIG_TOOLBAR_CHANGED, GetId() );
			wxPostEvent( GetParent(), e );
		}
	}
}

/*****************************************************
**
**   ConfigDialog   ---   showPanel
**
******************************************************/
void ConfigDialog::showPanel( const int &sel )
{
	ConfigPanelFactory factory;

	assert( sel >= 0 && sel < NB_PANELS );
	wxString title;
	title << _( "Configuration" ) << wxT( ": " ) << factory.getPanelName( sel );
	SetTitle( title );
	if ( ! configpanel[sel] )
	{
		panel[sel]->Show( false );
		configpanel[sel] = factory.createPanel( sel, panel[sel] );
		configpanel[sel]->setData();

		wxFlexGridSizer* sizer = new wxFlexGridSizer( 1, 1, 0, 0 );
		sizer->AddGrowableCol( 0 );
		sizer->AddGrowableRow( 0 );
		sizer->Add( configpanel[sel], 1, wxEXPAND, 0 );
		panel[sel]->SetSizer(sizer);
		panel[sel]->Layout();
		panel[sel]->Show( true );
	}
	configpanel[sel]->onActivate();
	configpanel[sel]->SetFocus();
}

/*****************************************************
**
**   ConfigDialog   ---   OnTreebook
**
******************************************************/
/*#ifdef _WX29
void ConfigDialog::OnTreebook( wxBookCtrlEvent &event )
#else
void ConfigDialog::OnTreebook( wxTreebookEvent &event )
#endif
*/
void ConfigDialog::OnTreebook( wxTreebookEvent &event )
{
	showPanel( event.GetSelection());
	if ( event.GetOldSelection() != -1 && configpanel[event.GetOldSelection()] != 0 )
		configpanel[event.GetOldSelection()]->onPassivate();
}

/*****************************************************
**
**   ConfigDialog   ---   OnApply
**
******************************************************/
void ConfigDialog::OnApply( wxCommandEvent &event )
{
	saveData();
	for ( int i = CONFIGPANEL_FIRST; i <= CONFIGPANEL_LAST; i++ )
	{
		if ( configpanel[i] )
		{
			configpanel[i]->setData();
		}
	}
	wxCommandEvent e( CONFIG_CHANGED, GetId() );
	wxPostEvent( GetParent(), e );
}

/*****************************************************
**
**   ConfigDialog   ---   OnSize
**
******************************************************/
void ConfigDialog::OnSize( wxSizeEvent &event )
{
	config->xConfigDialog = event.GetSize().GetWidth();
	config->yConfigDialog = event.GetSize().GetHeight();
	event.Skip();
}

/*****************************************************
**
**   ConfigBaseDialog   ---   OnOK
**
******************************************************/
void ConfigBaseDialog::OnOK( wxCommandEvent &event )
{
	saveData();
	wxCommandEvent e( CONFIG_CHANGED, GetId() );
	wxPostEvent( GetParent(), e );
	EndModal( wxID_OK );
}

/*****************************************************
**
**   ConfigBaseDialog   ---   OnCancel
**
******************************************************/
void ConfigBaseDialog::OnCancel( wxCommandEvent &event )
{
	EndModal( wxID_CANCEL );
}


