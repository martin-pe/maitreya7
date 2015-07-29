/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/YogaView.cpp
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
#pragma implementation "YogaView.h"
#endif

#include "YogaView.h"

#include <wx/choice.h>
#include <wx/spinctrl.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/toolbar.h>

#include "Conf.h"
#include "Document.h"
#include "Jaimini.h"
#include "TextHelper.h"
#include "TextWidget.h"
#include "Varga.h"
#include "VargaHoroscope.h"
#include "Writer.h"
#include "Yoga.h"

extern Config *config;

IMPLEMENT_CLASS( YogaView, TextViewBase )

/*************************************************//**
*
*
*
******************************************************/
class YogaViewTextWidget : public TextWidget
{
	DECLARE_CLASS( YogaViewTextWidget )

public:
	YogaViewTextWidget( wxWindow *parent, BasicView *v, 	Horoscope *horoscope )
			: TextWidget( parent, v )
	{
		this->horoscope = horoscope;
	}

	virtual int write()
	{
		int lagna;
		YogaExpert *expert = YogaExpert::get();
		VargaHoroscope chart( horoscope, varga );
		JaiminiExpert jexpert( &chart );

		switch ( firsthouse )
		{
		case 0:
			lagna = chart.getRasi( OASCENDANT );
			break;
		case 1:
			lagna = chart.getRasi( OMOON );
			break;
		case 2:
			lagna = jexpert.calcPada( 0 );
			break;
		default:
			lagna = firsthouse - 3;
			assert( lagna >= ARIES && lagna <= PISCES );
			break;
		}

		vector<unsigned int> v = expert->update( &chart, lagna, groupFilterString, sourceFilterString );
		expert->write( writer, v );
		return 0;
	}

	int varga, firsthouse;
	wxString groupFilterString, sourceFilterString;

private:
	Horoscope *horoscope;

};

IMPLEMENT_CLASS( YogaViewTextWidget, TextWidget )

/*****************************************************
**
**   YogaView   ---   Constructor
**
******************************************************/
YogaView::YogaView( wxWindow *parent, ChildWindow *frame, Document *doc )
		: TextViewBase( parent, frame, doc, VIEW_YOGA, true, true )
{
	theText = new YogaViewTextWidget( this, this, doc );
	widget = theText;

	supportEWToggle = false;
	varga = firsthouse = 0;
	groupfilter = config->vYogaGroupFilter;
	sourcefilter = config->vYogaSourceFilter;
	initToolItems();
	OnToolbarCommand();
}

/*****************************************************
**
**   YogaView   ---   Destructor
**
******************************************************/
YogaView::~YogaView()
{
}

/**************************************************************
***
**   YogaView   ---   initToolItems
***
***************************************************************/
void YogaView::initToolItems()
{
	YogaExpert *expert = YogaExpert::get();
	unsigned int i;
	vector<wxString> v;

	if ( toolbar )
	{
		expert->init();
		wxChoice *choice_varga = (wxChoice*)toolbar->FindControl( TBS_VARGA );
		if ( choice_varga ) choice_varga->SetSelection( varga );

		wxChoice *choice_firsthouse = (wxChoice*)toolbar->FindControl( TBS_YOGAFIRSTHOUSE );
		if ( choice_firsthouse ) choice_firsthouse->SetSelection( firsthouse );

		wxChoice *choice_groupfilter = (wxChoice*)toolbar->FindControl( TBS_YOGAGROUPFILTER );
		if ( choice_groupfilter )
		{
			v = expert->getGroups();
			choice_groupfilter->Clear();

			choice_groupfilter->Append( _( "All Groups" ));
			for ( i = 0; i < v.size(); i++ ) choice_groupfilter->Append( v[i] );

			choice_groupfilter->SetSelection( groupfilter );
		}

		wxChoice *choice_sourcefilter = (wxChoice*)toolbar->FindControl( TBS_YOGASOURCEFILTER );
		if ( choice_sourcefilter )
		{
			v = expert->getSources();
			choice_sourcefilter->Clear();

			choice_sourcefilter->Append( _( "All Sources" ));
			for ( i = 0; i < v.size(); i++ ) choice_sourcefilter->Append( v[i] );

			choice_sourcefilter->SetSelection( sourcefilter );
		}
	}
}

/**************************************************************
***
**   YogaView   ---   write
***
***************************************************************/
void YogaView::write()
{
	YogaViewTextWidget *w = (YogaViewTextWidget*)theText;

	w->firsthouse = firsthouse;
	w->varga = varga;

	wxChoice *choice_yogafilter = (wxChoice*)toolbar->FindControl( TBS_YOGAGROUPFILTER );
	if ( choice_yogafilter && choice_yogafilter->GetSelection() > 0 )
		w->groupFilterString = choice_yogafilter->GetStringSelection();
	else w->groupFilterString.Clear();

	wxChoice *choice_sourcefilter = (wxChoice*)toolbar->FindControl( TBS_YOGASOURCEFILTER );
	if ( choice_sourcefilter && choice_sourcefilter->GetSelection() > 0 )
		w->sourceFilterString = choice_sourcefilter->GetStringSelection();
	else w->sourceFilterString.Clear();

	theText->writeContents();
}

/**************************************************************
***
**   YogaView   ---   getWindowLabel
***
***************************************************************/
wxString YogaView::getWindowLabel( const bool shortname )
{
	return _( "Yogas" );
}

/*****************************************************
**
**   YogaView   ---   OnToolbarCommand
**
******************************************************/
void YogaView::OnToolbarCommand()
{
	if ( toolbar )
	{
		wxChoice *choice_varga = (wxChoice*)toolbar->FindControl( TBS_VARGA );
		if ( choice_varga ) varga = choice_varga->GetSelection();

		wxChoice *choice_firsthouse = (wxChoice*)toolbar->FindControl( TBS_YOGAFIRSTHOUSE );
		if ( choice_firsthouse ) firsthouse = choice_firsthouse->GetSelection();

		wxChoice *choice_groupfilter = (wxChoice*)toolbar->FindControl( TBS_YOGAGROUPFILTER );
		if ( choice_groupfilter )
		{
			groupfilter = choice_groupfilter->GetSelection();
			config->vYogaGroupFilter = groupfilter;
		}

		wxChoice *choice_sourcefilter = (wxChoice*)toolbar->FindControl( TBS_YOGASOURCEFILTER );
		if ( choice_sourcefilter )
		{
			sourcefilter = choice_sourcefilter->GetSelection();
			config->vYogaSourceFilter = sourcefilter;
		}
	}
	OnDataChanged();
}

/**************************************************************
***
**   ViewFactory   ---   createYogaView
***
***************************************************************/
BasicView *ViewFactory::createYogaView( wxWindow *parent, ChildWindow *frame, Document *doc )
{
	return new YogaView( parent, frame, doc );
}


