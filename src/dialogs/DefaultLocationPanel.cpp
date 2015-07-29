/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/DefaultLocationPanel.cpp
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
#pragma implementation "DefaultLocationPanel.h"
#endif

#include "DefaultLocationPanel.h"

#include "AtlasDialog.h"
#include "astrobase.h"
#include "CommonInputFields.h"
#include "Conf.h"
#include "guibase.h"
#include "Lang.h"
#include "mathbase.h"

#include <math.h>
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/datetime.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/statbox.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/timer.h>

using namespace std;

extern Config *config;
enum { CD_SEARCHLOCATION = wxID_HIGHEST + 1, CD_TIMER, CD_LONGITUDE, CD_LATITUDE };

IMPLEMENT_CLASS( DefaultLocationPanel, ConfigPanel )

/*****************************************************
**
**   DefaultLocationPanel   ---   Constructor
**
******************************************************/
DefaultLocationPanel::DefaultLocationPanel( wxWindow* parent )
		: ConfigPanel( parent )
{
    // begin wxGlade: DefaultLocationPanel::DefaultLocationPanel
    sizer_tzsettings_staticbox = new wxStaticBox(this, -1, _("Timezone System Settings"));
    sizer_deflocname_staticbox = new wxStaticBox(this, -1, _("Default Location"));
    text_name = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    label_longitude = new wxStaticText(this, wxID_ANY, _("Longitude"));
    text_longitude = new LongitudeInputField(this, CD_LONGITUDE);
    const wxString choice_ew_choices[] = {
        _("East"),
        _("West")
    };
    choice_ew = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_ew_choices, 0);
    label_latitude = new wxStaticText(this, wxID_ANY, _("Latitude"));
    text_latitude = new LatitudeInputField(this, CD_LATITUDE);
    const wxString choice_ns_choices[] = {
        _("North"),
        _("South")
    };
    choice_ns = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, choice_ns_choices, 0);
    button_search = new wxButton(this, CD_SEARCHLOCATION, _("Search Location ..."));
    label_localtime = new wxStaticText(this, wxID_ANY, _("Local Time"));
    text_localtime = new wxStaticText(this, wxID_ANY, _("00:00:00"));
    label_uttime = new wxStaticText(this, wxID_ANY, _("Universal Time"));
    text_uttime = new wxStaticText(this, wxID_ANY, _("00:00:00"));
    label_tzoffset = new wxStaticText(this, wxID_ANY, _("Time Zone Offset"));
    text_tzoffset = new wxStaticText(this, wxID_ANY, _("+00:00"));
    label_systemdst = new wxStaticText(this, wxID_ANY, _("Daylight Saving"));
    text_systemdst = new wxStaticText(this, wxID_ANY, _("+00:00"));

    set_properties();
    do_layout();
    // end wxGlade
	timer = new wxTimer( this, CD_TIMER );

	Connect( CD_SEARCHLOCATION, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DefaultLocationPanel::OnSearchLocation ));
	Connect( CD_TIMER, wxEVT_TIMER, wxTimerEventHandler( DefaultLocationPanel::OnTimer ));
	updateTzItems();

	text_name->SetFocus();
}

/*****************************************************
**
**   DefaultLocationPanel   ---   Destructor
**
******************************************************/
DefaultLocationPanel::~DefaultLocationPanel()
{
	if ( timer->IsRunning()) timer->Stop();
	delete timer;
}

/*****************************************************
**
**   DefaultLocationPanel   ---   onActivate
**
******************************************************/
void DefaultLocationPanel::onActivate()
{
	if ( ! timer->IsRunning() ) timer->Start( 1000 );
}

/*****************************************************
**
**   DefaultLocationPanel   ---   onPassivate
**
******************************************************/
void DefaultLocationPanel::onPassivate()
{
	if ( timer->IsRunning()) timer->Stop();
}

/*****************************************************
**
**   DefaultLocationPanel   ---   OnTimer
**
******************************************************/
void DefaultLocationPanel::OnTimer( wxTimerEvent &event )
{
	updateTzItems();
}

/*****************************************************
**
**   DefaultLocationPanel   ---   updateTzItems
**
******************************************************/
void DefaultLocationPanel::updateTzItems()
{
	TzUtil util;
	TzInfo tzinfo = util.getCurrentTzInfo();

	text_localtime->SetLabel( tzinfo.localTimeFormatted );
	text_uttime->SetLabel( tzinfo.utcTimeFormatted );
	text_tzoffset->SetLabel( tzinfo.tzOffsetFormatted );
	text_systemdst->SetLabel( tzinfo.dstOffsetFormatted );
}

/*****************************************************
**
**   DefaultLocationPanel   ---   set_properties
**
******************************************************/
void DefaultLocationPanel::set_properties()
{
    // begin wxGlade: DefaultLocationPanel::set_properties
    choice_ew->SetSelection(0);
    choice_ns->SetSelection(0);
    label_localtime->SetMinSize(wxSize(150, 16));
    text_localtime->SetMinSize(wxSize(100, 16));
    // end wxGlade
}

/*****************************************************
**
**   DefaultLocationPanel   ---   do_layout
**
******************************************************/
void DefaultLocationPanel::do_layout()
{
    // begin wxGlade: DefaultLocationPanel::do_layout
    wxFlexGridSizer* sizer_main = new wxFlexGridSizer(1, 2, 0, 0);
    wxFlexGridSizer* grid_sizer_4 = new wxFlexGridSizer(1, 1, 0, 0);
    wxStaticBoxSizer* sizer_tzsettings = new wxStaticBoxSizer(sizer_tzsettings_staticbox, wxVERTICAL);
    wxGridSizer* grid_systemsettings = new wxGridSizer(4, 2, 3, 6);
    wxFlexGridSizer* sizer_defloc = new wxFlexGridSizer(4, 1, 0, 0);
    wxStaticBoxSizer* sizer_deflocname = new wxStaticBoxSizer(sizer_deflocname_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_sizer_geographicaldata = new wxFlexGridSizer(2, 3, 3, 3);
    sizer_deflocname->Add(text_name, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_geographicaldata->Add(label_longitude, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_geographicaldata->Add(text_longitude, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_geographicaldata->Add(choice_ew, 0, wxALL|wxEXPAND, 3);
    grid_sizer_geographicaldata->Add(label_latitude, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_geographicaldata->Add(text_latitude, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_geographicaldata->Add(choice_ns, 0, wxALL|wxEXPAND, 3);
    grid_sizer_geographicaldata->AddGrowableCol(1);
    sizer_deflocname->Add(grid_sizer_geographicaldata, 1, wxALL, 3);
    sizer_defloc->Add(sizer_deflocname, 1, wxALL|wxEXPAND, 3);
    sizer_defloc->Add(button_search, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 3);
    sizer_defloc->AddGrowableCol(0);
    sizer_main->Add(sizer_defloc, 1, wxALL, 3);
    grid_systemsettings->Add(label_localtime, 0, wxALIGN_CENTER_VERTICAL, 0);
    grid_systemsettings->Add(text_localtime, 0, wxALIGN_CENTER_VERTICAL, 0);
    grid_systemsettings->Add(label_uttime, 0, wxALIGN_CENTER_VERTICAL, 0);
    grid_systemsettings->Add(text_uttime, 0, wxALIGN_CENTER_VERTICAL, 0);
    grid_systemsettings->Add(label_tzoffset, 0, wxALIGN_CENTER_VERTICAL, 0);
    grid_systemsettings->Add(text_tzoffset, 0, wxALIGN_CENTER_VERTICAL, 0);
    grid_systemsettings->Add(label_systemdst, 0, wxALIGN_CENTER_VERTICAL, 0);
    grid_systemsettings->Add(text_systemdst, 0, wxALIGN_CENTER_VERTICAL, 0);
    sizer_tzsettings->Add(grid_systemsettings, 1, wxALL, 3);
    grid_sizer_4->Add(sizer_tzsettings, 1, wxALL, 3);
    sizer_main->Add(grid_sizer_4, 1, wxALL|wxEXPAND, 3);
    SetSizer(sizer_main);
    sizer_main->Fit(this);
    // end wxGlade
}

/*****************************************************
**
**   DefaultLocationPanel   ---   setData
**
******************************************************/
void DefaultLocationPanel::setData()
{
	setDefaultLocation( &config->defaultLocation );
}

/*****************************************************
**
**   DefaultLocationPanel   ---   saveData
**
******************************************************/
bool DefaultLocationPanel::saveData()
{
	double longitude = text_longitude->getDoubleValue();
	if ( choice_ew->GetSelection() > 0 ) longitude *= -1;
	double latitude = text_latitude->getDoubleValue();
	if ( choice_ns->GetSelection() > 0 ) latitude *= -1;

	config->defaultLocation.setLocation( text_name->GetValue(), longitude, latitude, 0, 0 );

	return true;
}

/*****************************************************
**
**   DefaultLocationPanel   ---   setDefaultLocation
**
******************************************************/
void DefaultLocationPanel::setDefaultLocation( Location *loc )
{
	text_name->SetValue( loc->getLocName() );

	text_longitude->setDoubleValue( fabs( loc->getLongitude()));
	choice_ew->SetSelection( loc->getLongitude() < 0 );
	text_latitude->setDoubleValue( fabs( loc->getLatitude()));
	choice_ns->SetSelection( loc->getLatitude() < 0 );
}

/*****************************************************
**
**   DefaultLocationPanel   ---   OnSearchLocation
**
******************************************************/
void DefaultLocationPanel::OnSearchLocation( wxCommandEvent &event )
{
	AtlasDialogWrapper dialog( this, ATLASDIALOG_CALLER_DEFAULTLOCATIONPANEL );
	if ( dialog.run() == wxID_OK )
	{
		AtlasEntry *e = dialog.getSelectedEntry();
		text_name->SetValue( e->name );

		text_longitude->setDoubleValue( fabs( e->longitude ));
		choice_ew->SetSelection( e->longitude < 0 );
		text_latitude->setDoubleValue( fabs( e->latitude ));
		choice_ns->SetSelection( e->latitude < 0 );
	}
}

/*****************************************************
**
**   ConfigPanelFactory   ---   createDefaultLocationPanel
**
******************************************************/
ConfigPanel *ConfigPanelFactory::createDefaultLocationPanel( wxWindow *parent )
{
	return new DefaultLocationPanel( parent );
}

