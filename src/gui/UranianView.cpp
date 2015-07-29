/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/UranianView.cpp
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
#pragma implementation "UranianView.h"
#endif

#include "UranianView.h"

#include <wx/choice.h>
#include <wx/spinctrl.h>
#include <wx/toolbar.h>

#include "astrobase.h"
#include "Conf.h"
#include "constants.h"
#include "DialogElements.h"
#include "Document.h"
#include "Lang.h"
#include "TextWidget.h"
#include "Uranian.h"
#include "Writer.h"

extern Config *config;

IMPLEMENT_CLASS( UranianView, TextViewBase )

/*************************************************//**
*
*
*
******************************************************/
class UranianViewTextWidget : public TextWidget
{
	DECLARE_CLASS( UranianViewTextWidget )

public:
	UranianViewTextWidget( wxWindow *parent, BasicView *v, 	Horoscope *horoscope, UranianConfig *cfg )
			: TextWidget( parent, v )
	{
		this->horoscope = horoscope;
		this->cfg = cfg;
	}

	virtual int write()
	{
		UranianExpert expert( mview, writer, *cfg );

		switch ( mode )
		{
		case 0:
			expert.writeRadixAnalysis( horoscope );
			break;
		case 1:
			expert.writeYearlyPreviewAnalysis( horoscope, year );
			break;
		default:
			printf( "Error: wrong view mode %d in midpoint window\n", mode );
			break;
		}
		return 0;
	}

	int mode, year;

private:
	Horoscope *horoscope;
	UranianConfig * cfg;
};

IMPLEMENT_CLASS( UranianViewTextWidget, TextWidget )

/*****************************************************
**
**   UranianView   ---   Constructor
**
******************************************************/
UranianView::UranianView( wxWindow *parent, ChildWindow *frame, Document *doc, const bool &vedic )
		: TextViewBase( parent, frame, doc, VIEW_URANIAN, true, vedic )
{
	int day, month;

	// set current year as default for yearly preview
	Formatter::get()->getDateIntsFromJD( JDate::getCurrentJD(), day, month, year );

	supportEWToggle = supportObjectToggle = true;

	mode = config->uranianWindowPage;
	cfg = new UranianConfig;

	theText = new UranianViewTextWidget( this, this, doc, cfg );
	widget = theText;

	initToolItems();
	OnToolbarCommand(); // does it all, refresh and hide-and-seek for tool items

	Connect( wxEVT_IDLE, wxIdleEventHandler( UranianView::OnIdle ));
}

/*****************************************************
**
**   UranianView   ---   Destructor
**
******************************************************/
UranianView::~UranianView()
{
	config->uranianWindowPage = mode;
	config->wYearLength = cfg->yl_int;
	config->orbisRadix = 60 * cfg->orbis;
	config->circleType = cfg->circle;
	config->planetSortOrder = cfg->order;
	delete cfg;
}

/**************************************************************
***
**   UranianView   ---   initToolItems
***
***************************************************************/
void UranianView::initToolItems()
{
	if ( toolbar )
	{
		wxChoice *choice_viewmode = (wxChoice*)toolbar->FindControl( TBS_URANIANMODE );
		if ( choice_viewmode ) choice_viewmode->SetSelection( mode );

		wxSpinCtrl *spin_orbis = (wxSpinCtrl*)toolbar->FindControl( TBS_ORBIS );
		if ( spin_orbis ) spin_orbis->SetValue( (int)(60 * cfg->orbis ));

		CircleTypeChoice *choice_circle = (CircleTypeChoice*)toolbar->FindControl( TBS_CIRCLETYPE );
		if ( choice_circle ) choice_circle->SetSelection( cfg->circle );

		wxSpinCtrl *spin_year = (wxSpinCtrl*)toolbar->FindControl( TBS_YEAR );
		if ( spin_year ) spin_year->SetValue( year );

		YearLengthChoice *choice_yl = (YearLengthChoice*)toolbar->FindControl( TBS_YEARLENGTH );
		if ( choice_yl ) choice_yl->SetSelection( cfg->yl_int );

		SortChoice *choice_sort = (SortChoice*)toolbar->FindControl( TBS_SORT );
		if ( choice_sort ) choice_sort->SetSelection( cfg->order );

		FilterChoice *choice_filter = (FilterChoice*)toolbar->FindControl( TBS_FILTER );
		choice_filter->recreate( getPlanetList());
		if ( choice_filter ) choice_filter->SetSelection( 0 );
	}
}

/*****************************************************
**
**   UranianView   ---   OnDataChanged
**
******************************************************/
void UranianView::OnDataChanged()
{
	BasicView::OnDataChanged();
	if ( toolbar )
	{
		FilterChoice *choice_filter = (FilterChoice*)toolbar->FindControl( TBS_FILTER );
		assert( choice_filter );
		choice_filter->recreate( getPlanetList());
	}
	updateView = true;
}

/**************************************************************
***
**   UranianView   ---   OnIdle
***
***************************************************************/
void UranianView::OnIdle( wxIdleEvent &event )
{
	if ( ! updateView ) return;
	UranianViewTextWidget * w = (UranianViewTextWidget*)theText;

	w->mode = mode;
	w->year = year;

	write();
	updateView = false;
}

/*****************************************************
**
**   UranianView   ---   write
**
******************************************************/
void UranianView::write()
{
	theText->writeContents();
}

/*****************************************************
**
**   UranianView   ---   OnToolbarCommand
**
******************************************************/
void UranianView::OnToolbarCommand()
{
	if ( toolbar )
	{
		wxChoice *choice_viewmode = (wxChoice*)toolbar->FindControl( TBS_URANIANMODE );
		if ( choice_viewmode ) mode = choice_viewmode->GetSelection();

		wxSpinCtrl *spin_orbis = (wxSpinCtrl*)toolbar->FindControl( TBS_ORBIS );
		if ( spin_orbis )
		{
			spin_orbis->Enable( mode == 0 );
			cfg->orbis = spin_orbis->GetValue();
			cfg->orbis /= 60;
		}

		wxSpinCtrl *spin_year = (wxSpinCtrl*)toolbar->FindControl( TBS_YEAR );
		if ( spin_year )
		{
			spin_year->Enable( mode == 1 );
			year = spin_year->GetValue();
		}

		YearLengthChoice *choice_yl = (YearLengthChoice*)toolbar->FindControl( TBS_YEARLENGTH );
		if ( choice_yl )
		{
			cfg->yl_int = choice_yl->GetSelection();
			choice_yl->Enable( mode == 1 );
			cfg->year_length = choice_yl->getYearLength();
		}

		SortChoice *choice_sort = (SortChoice*)toolbar->FindControl( TBS_SORT );
		if ( choice_sort )
		{
			choice_sort->Enable( mode == 0 );
			cfg->order = choice_sort->GetSelection();
		}

		FilterChoice *choice_filter = (FilterChoice*)toolbar->FindControl( TBS_FILTER );
		if ( choice_filter )
		{
			choice_filter->Enable( mode == 0 );
			cfg->filter = choice_filter->getFilter();
		}

		CircleTypeChoice *choice_circle = (CircleTypeChoice*)toolbar->FindControl( TBS_CIRCLETYPE );
		if ( choice_circle ) cfg->circle = choice_circle->GetSelection();
	}
	updateView = true;
}


/**************************************************************
***
**   UranianView   ---   getWindowLabel
***
***************************************************************/
wxString UranianView::getWindowLabel( const bool shortname )
{
	return _( "Uranian" );
}

/**************************************************************
***
**   ViewFactory   ---   createUranianView
***
***************************************************************/
BasicView *ViewFactory::createUranianView( wxWindow *parent, ChildWindow *frame, Document *doc, const bool &vedic )
{
	return new UranianView( parent, frame, doc, vedic );
}


