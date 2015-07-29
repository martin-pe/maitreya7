/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/SolarView.cpp
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
#pragma implementation "SolarView.h"
#endif

#include "SolarView.h"

#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/splitter.h>
#include <wx/stattext.h>
#include <wx/toolbar.h>

#include "Conf.h"
#include "DialogElements.h"
#include "Document.h"
#include "GraphicGrid.h"
#include "IconProvider.h"
#include "Lang.h"
#include "SplitterWidget.h"
#include "TextHelper.h"
#include "TextWidget.h"
#include "Writer.h"

extern Config *config;

IMPLEMENT_CLASS( SolarView, BasicView )

/*************************************************//**
*
*
*
******************************************************/
class SolarTextWidget : public TextWidget
{
	DECLARE_CLASS( SolarTextWidget )

public:
	SolarTextWidget( wxWindow *parent, BasicView *v, Horoscope *horoscope, Horoscope *th, ChartProperties *properties )
			: TextWidget( parent, v )
	{
		this->horoscope = horoscope;
		this->th = th;
		this->properties = properties;
	}

	virtual int write()
	{
		if ( config->showTextViewHeaders ) writer->writeHeader1( _( "Solar Chart" ));
		TextHelper helper( th, properties, writer );
		helper.writePlanets();
		if ( ! properties->isVedic()) helper.writeTextAnalysis( TEXT_MODE_WESTERN_ASPECTARIUM_PLAIN );
		return 0;
	}

	ChartProperties *properties;
	Horoscope *horoscope, *th;
};

IMPLEMENT_CLASS( SolarTextWidget, TextWidget )

/*****************************************************
**
**   SolarView   ---   Constructor
**
******************************************************/
SolarView::SolarView( wxWindow *parent, ChildWindow *frame, Document *doc, const bool &vedic )
		: BasicView( parent, frame, doc, VIEW_SOLAR, true, vedic )
{
	supportEWToggle = supportGraphicExport = supportTextExport = supportFullGraphicStyleToggle = supportObjectToggle = supportSkinToggle = true;
	th = new TajakaHoroscope( doc->getDataSet() );
	th->update();
	setActualYear();

	splitter = new SplitterWidget( this );
	theText = new SolarTextWidget( splitter, this, doc, th, this );
	gwidget = new DefaultEwChartGridWidget( splitter, this, th ); //, wxDefaultPosition, sizeg );
	splitter->SplitVertically( theText, gwidget );
	widget = splitter;

	initToolItems();
	OnDataChanged();

	Connect( TBS_YEAR, wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( SolarView::OnYearSpin ));
	Connect( TBB_NOW, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( SolarView::OnNow ));
	Connect( TBB_NEW, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( SolarView::OnCreateChart ));
}

/*****************************************************
**
**   SolarView   ---   Destructor
**
******************************************************/
SolarView::~SolarView()
{
	config->sashSolar = splitter->GetSashPosition();
	delete th;
}

/*****************************************************
**
**   SolarView   ---   postCreate
**
******************************************************/
void SolarView::postCreate()
{
	wxSplitterWindow *splitter = (wxSplitterWindow*)widget;
	splitter->SetSashPosition( config->sashSolar );
}

/*****************************************************
**
**   SolarView   ---   initToolItems
**
******************************************************/
void SolarView::initToolItems()
{
	if ( toolbar )
	{
		wxSpinCtrl *spin_year = (wxSpinCtrl*)toolbar->FindControl( TBS_YEAR );
		if ( spin_year ) spin_year->SetValue( year );
	}
}

/*****************************************************
**
**   SolarView   ---   dispatchCommand
**
******************************************************/
bool SolarView::dispatchCommand( const int &command )
{
	switch ( command )
	{
	case CMD_CHILD_EXPORT:
		gwidget->doExport();
		break;
	case CMD_CHILD_SAVETEXT:
		theText->doExport();
		break;
	default:
		return BasicView::dispatchCommand( command );// && pptext->dispatchCommand( command );
		break;
	}
	return true;
}

/*****************************************************
**
**   SolarView   ---   setActualYear
**
******************************************************/
void SolarView::setActualYear()
{
	int bday, aday, bmonth, amonth, byear;
	Formatter::get()->getDateIntsFromJD( JDate::getCurrentJD(), aday, amonth, year );
	Formatter::get()->getDateIntsFromJD( doc->getJD(), bday, bmonth, byear );
	if ( amonth < bmonth || ( amonth == bmonth && aday < bday )) year--;
}

/*****************************************************
**
**   SolarView   ---   OnNow
**
******************************************************/
void SolarView::OnNow( wxCommandEvent &event )
{
	setActualYear();
	OnDataChanged();
	if ( toolbar )
	{
		wxSpinCtrl *spin_year = (wxSpinCtrl*)toolbar->FindControl( TBS_YEAR );
		if ( spin_year ) spin_year->SetValue( year );
	}
}

/*****************************************************
**
**   SolarView   ---   OnCreateChart
**
******************************************************/
void SolarView::OnCreateChart( wxCommandEvent &e )
{
	Document *d = new Document;
	d->setLocation( *doc->getLocation() );
	d->setDate( th->getJD() );
	d->setHName( wxString::Format( wxT( "%s: %s (%d)" ), (const wxChar*)doc->getHName(),
	                               ( isVedic() ? _( "Tajaka" ) : _( "Solar" )),
	                               year ));

	wxCommandEvent event( CREATE_ENTRY_CHART, GetParent()->GetId() );
	event.SetEventObject( (wxObject*)d );
	wxPostEvent( GetParent(), event );
}

/*****************************************************
**
**   SolarView   ---   OnYearSpin
**
******************************************************/
void SolarView::OnYearSpin( wxSpinEvent &event )
{
	if ( toolbar )
	{
		wxSpinCtrl *spin_year = (wxSpinCtrl*)toolbar->FindControl( TBS_YEAR );
		if ( spin_year ) year = spin_year->GetValue();
	}
	OnDataChanged();
}

/*****************************************************
**
**   SolarView   ---   OnDataChanged
**
******************************************************/
void SolarView::OnDataChanged()
{
	wxString s;
	Formatter *f = Formatter::get();
	Location *location = th->getLocation();

	BasicView::OnDataChanged();

	th->calcTajaka( year, isVedic() );
	th->update();
	if ( toolbar )
	{
		ToolbarLabel *text_return = (ToolbarLabel*)toolbar->FindControl( TB_TEXT );
		s.Printf( wxT( "%s: %s" ), _( "Return" ),
		          (const wxChar*)f->getFullDateStringFromJD( th->getJD() + ( location->getTimeZone() + location->getDST())/24 ) );
		if ( text_return ) text_return->SetLabel( s );
	}

	theText->writeContents();

	gwidget->OnDataChanged();
	gwidget->Refresh();
}

/**************************************************************
***
**   SolarView   ---   getWindowLabel
***
***************************************************************/
wxString SolarView::getWindowLabel( const bool shortname )
{
	return _( "Solar" );
}

/**************************************************************
***
**   ViewFactory   ---   createSolarView
***
***************************************************************/
BasicView *ViewFactory::createSolarView( wxWindow *parent, ChildWindow *frame, Document *doc, const bool &vedic )
{
	return new SolarView( parent, frame, doc, vedic );
}


