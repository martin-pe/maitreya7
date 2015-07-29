/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/TransitView.cpp
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
#pragma implementation "TransitView.h"
#endif

#include "TransitView.h"

#include <wx/choice.h>
#include <wx/sizer.h>
#include <wx/slider.h>
#include <wx/spinctrl.h>
#include <wx/statbox.h>
#include <wx/timer.h>
#include <wx/textctrl.h>
#include <wx/toolbar.h>

#include "Aspect.h"
#include "astrobase.h"
#include "Calculator.h"
#include "ColorConfig.h"
#include "Conf.h"
#include "constants.h"
#include "DataSet.h"
#include "DateTimeCtrl.h"
#include "DialogElements.h"
#include "Document.h"
#include "guibase.h"
#include "IconProvider.h"
#include "GraphicGrid.h"
#include "NotebookWidget.h"
#include "mathbase.h"
#include "Painter.h"
#include "SbcView.h"
#include "SplitterWidget.h"
#include "Transit.h"
#include "TextHelper.h"
#include "TextWidget.h"
#include "Uranian.h"
#include "VedicChart.h"
#include "WesternChart.h"
#include "Writer.h"

extern Config *config;

enum { TRANSIT_NOTEBOOK = wxID_HIGHEST + 3000, TRANSIT_PROGRRESS_TIMER };

#define MAX_TRANSIT_PAGES 6
#define TIMER_PERIOD  100

enum { TNB_DOUBLE = 0, TNB_SBC, TNB_SHADVARGA, TNB_VARGA2, TNB_VARGA3, TNB_URANIAN };

IMPLEMENT_CLASS( TransitView, BasicView )
IMPLEMENT_CLASS( TransitChartWidget, BasicWidget )

/*****************************************************
**
**   TransitChartWidget   ---   Constructor
**
******************************************************/
TransitChartWidget::TransitChartWidget( wxWindow *parent, ChartProperties *chartprops, const Horoscope *h1, const Horoscope *h2, const bool &vedic,
                                        const wxPoint& pos, const wxSize& size )
		: BasicWidget( parent, chartprops, -1, pos, size ),
		doc( h1 ),
		htransit( h2 )
{
	assert( h1 != 0 );
	assert( h2 != 0 );
	preferVedic = vedic;
	wchart = new WesternChart( doc, htransit, chartprops );
	wchart->OnDataChanged();
}

/*****************************************************
**
**   TransitChartWidget   ---   Destructor
**
******************************************************/
TransitChartWidget::~TransitChartWidget()
{
	delete wchart;
}

/*****************************************************
**
**   TransitChartWidget   ---   OnDataChanged
**
******************************************************/
void TransitChartWidget::OnDataChanged()
{
	wchart->OnDataChanged();
}


/*****************************************************
**
**   TransitChartWidget   ---   doPaint
**
******************************************************/
void TransitChartWidget::doPaint( bool eraseBackground, const wxRect* rect )
{
	int w, h;
	assert( painter );
	getWidgetSize( &w, &h  );
	painter->setBrushColor( config->colors->bgColor );
	painter->drawRectangle( 0, 0, w, h );

	wchart->paint( painter, MyRect( 0, 0, w, h ));
}

/*************************************************//**
*
*
*
******************************************************/
class TransitTextWidget : public TextWidget
{
	DECLARE_CLASS( TransitTextWidget )

public:
	TransitTextWidget( wxWindow *parent, BasicView *v, TransitExpert *expert, const int textmode )
			: TextWidget( parent, v ),
			textmode( textmode )
	{
		this->expert = expert;
	}

	virtual int write()
	{
		expert->writeTransitData( writer, textmode );
		return 0;
	}

	TransitExpert *expert;

private:
	const int textmode;

};

IMPLEMENT_CLASS( TransitTextWidget, TextWidget )

/*****************************************************
**
**   TransitView   ---   Constructor
**
******************************************************/
TransitView::TransitView( wxWindow *parent, ChildWindow *frame, Document *doc, const bool &vedic )
		: BasicNotebookView( parent, frame, doc, VIEW_TRANSIT, true, vedic )
{
	supportObjectToggle = true;

	expert = new TransitExpert( doc, this );

	tzchoice = config->transitTimezone;
	transitdate = JDate::getCurrentJD();
	calcTzOffset();

	notebook = new NotebookWidget( this, BASIC_NOTEBOOK, wxDefaultPosition, wxDefaultSize, wxNB_DEFAULT );
	initToolItems();

	for ( int i = 0; i < MAX_TRANSIT_PAGES; i++ )
	{
		switch ( i )
		{
		case TNB_DOUBLE:
			tsplitter = new SplitterWidget( notebook );
			aspectText = new TransitTextWidget( tsplitter, this, expert, 0 );
			gwidget =  new ChartGridWidget( tsplitter, this, 1, 1 );
			gwidget->addVedicVargaChart( new VedicVargaChart( doc, VARGA_RASI, expert->getTransitChart(), VARGA_RASI, this ));
			tsplitter->SplitVertically( aspectText, gwidget );
			notebook->AddPage( tsplitter, _( "Double Chart" ));
			break;

		case TNB_SBC:
			swidget = new SbcWidget( notebook, this, (Horoscope*)doc, (Horoscope*)expert->getTransitChart() );
			notebook->AddPage( swidget, _( "Sarvatobhadra" ));
			break;

		case TNB_SHADVARGA:
			shadvargawidget = new ShadVargaChartGridWidget( notebook, this, doc, expert->getTransitChart());
			notebook->AddPage( shadvargawidget, _( "Shadvarga" ));
			break;

		case TNB_VARGA2:
			varga2widget = new VargaTab2ChartGridWidget( notebook, this, doc, expert->getTransitChart());
			notebook->AddPage( varga2widget, _( "Varga (2)" ));
			break;

		case TNB_VARGA3:
			varga3widget = new VargaTab3ChartGridWidget( notebook, this, doc, expert->getTransitChart());
			notebook->AddPage( varga3widget, _( "Varga (3)" ));
			break;

		case TNB_URANIAN:
			uranianText = new TransitTextWidget( notebook, this, expert, 1 );
			notebook->AddPage( uranianText, _( "Uranian" ));
			break;

		default:
			printf( "Error: wrong notebook index %d in TransitView::TransitView\n", i );
			break;
		}
	}

	widget = notebook;
	timer = new wxTimer( this, TRANSIT_PROGRRESS_TIMER );

	Connect( TBB_NOW, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( TransitView::OnSetToCurrent ));
	Connect( TBS_DATE, COMMAND_SPIN_CHANGED, wxCommandEventHandler( TransitView::OnDateCtrlChanged ));
	Connect( TBS_DATE, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( TransitView::OnDateCtrlChanged ));
	Connect( TBS_TIME, COMMAND_SPIN_CHANGED, wxCommandEventHandler( TransitView::OnDateCtrlChanged ));
	Connect( TBS_TIME, COMMAND_SPIN_WRAP, wxCommandEventHandler( TransitView::OnTimeCtrlWrap ));

	Connect( TBS_TZ, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( TransitView::OnChoiceTimezone ));
	Connect( TBS_TRANSITMODE, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( TransitView::OnChoiceCommand ));
	Connect( TBS_YEARLENGTH, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( TransitView::OnChoiceCommand ));
	Connect( TBS_ORBIS, wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( TransitView::OnSpinCommand ));
	Connect( TBS_ORBIS, wxEVT_COMMAND_TEXT_ENTER, wxTextEventHandler( TransitView::OnGeneralCommand ));
	Connect( TBS_SORT, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( TransitView::OnChoiceCommand ));
	Connect( TBS_CIRCLETYPE, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( TransitView::OnChoiceCommand ));
	Connect( wxEVT_IDLE, wxIdleEventHandler( TransitView::OnIdle ));

	Connect( TBS_DATE_PROGRESS, wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( TransitView::OnDateProgress ));

	Connect( TBS_DATE_PROGRESS, wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( TransitView::OnDateProgress ));
	Connect( TBS_DATE_PROGRESS, wxEVT_SCROLL_CHANGED, wxScrollEventHandler( TransitView::OnDateProgress ));

	Connect( TRANSIT_PROGRRESS_TIMER, wxEVT_TIMER, wxTimerEventHandler( TransitView::OnTimer ));
}

/*****************************************************
**
**   TransitView   ---   Destructor
**
******************************************************/
TransitView::~TransitView()
{
	if ( timer->IsRunning() ) timer->Stop();

	config->sashTransit = tsplitter->GetSashPosition();

	wxSpinCtrl *spin_orbis = (wxSpinCtrl*)toolbar->FindControl( TBS_ORBIS );
	if ( spin_orbis ) config->orbisTransit = spin_orbis->GetValue();

	config->transitTimezone = tzchoice;

	config->avtivePageTransit = activepage;

	delete expert;
}

/*****************************************************
**
**   TransitView   ---   postCreate
**
******************************************************/
void TransitView::postCreate()
{
	assert( tsplitter );
	tsplitter->SetSashPosition( config->sashTransit );
	notebook->SetSelection( config->avtivePageTransit );
	setToCurrent();
	dirty = true;
}

/*****************************************************
**
**   TransitView   ---   calcTzOffset
**
******************************************************/
void TransitView::calcTzOffset()
{
	if ( tzchoice == 1 )
	{
		TzUtil tzu;
		TzInfo tzinfo = tzu.calculateTzInfoForJD( transitdate );
		tzoffset = tzinfo.tzhours + tzinfo.dsthours;
	}
	else tzoffset = 0;
}

/*****************************************************
**
**   TransitView   ---   OnChoiceTimezone
**
******************************************************/
void TransitView::OnChoiceTimezone( wxCommandEvent &event )
{
	if ( event.GetInt())
	{
		// local time switched on: set tzoffset, add tzoffset later
		tzchoice = 1;
		calcTzOffset();
		transitdate += tzoffset / 24;
	}
	else
	{
		// local time switched off: adjust transitdate, set tzoffset later
		transitdate -= tzoffset / 24;
		tzchoice = 0;
		calcTzOffset();
	}
	setDateTimeSpinsFromTransitdate();
}

/*****************************************************
**
**   TransitView   ---   setTransitdateFromDateTimeSpins
**
******************************************************/
void TransitView::setTransitdateFromDateTimeSpins()
{
	if ( toolbar )
	{
		// Date and Time
		DateSpin *spin_date = (DateSpin*)toolbar->FindControl( TBS_DATE );
		TimeSpin *spin_time = (TimeSpin*)toolbar->FindControl( TBS_TIME );
		if ( spin_date && spin_time )
		{
			double dd = spin_date->getDoubleValue();
			double tt = spin_time->getDoubleValue();
			transitdate = dd + tt / 24.0;
		}
	}
	dirty = true;
}

/*****************************************************
**
**   TransitView   ---   setDateTimeSpinsFromTransitdate
**
******************************************************/
void TransitView::setDateTimeSpinsFromTransitdate()
{
	if ( toolbar )
	{
		DateSpin *spin_date = (DateSpin*)toolbar->FindControl( TBS_DATE );
		TimeSpin *spin_time = (TimeSpin*)toolbar->FindControl( TBS_TIME );
		if ( spin_date && spin_time )
		{
			double thedate = (int)(transitdate + .5 );
			spin_date->setDoubleValue( thedate -.5 );

			double thetime = ( transitdate - (long)transitdate ) + .5;
			thetime = a_red( 24.0 * thetime, 24 );
			spin_time->setDoubleValue( thetime );
		}
	}
	dirty = true;
}

/*****************************************************
**
**   TransitView   ---   setToCurrent
**
******************************************************/
void TransitView::setToCurrent()
{
	transitdate = JDate::getCurrentJD() + tzoffset / 24.0;
	setDateTimeSpinsFromTransitdate();
}

/*****************************************************
**
**   TransitView   ---   OnDateCtrlChanged
**
******************************************************/
void TransitView::OnDateCtrlChanged( wxCommandEvent &event )
{
	setTransitdateFromDateTimeSpins();
	event.Skip();
}

/*****************************************************
**
**   TransitView   ---   OnTimeCtrlWrap
**
******************************************************/
void TransitView::OnTimeCtrlWrap( wxCommandEvent &event )
{
	setTransitdateFromDateTimeSpins();
	transitdate += event.GetInt();
	setDateTimeSpinsFromTransitdate();
}

/*****************************************************
**
**   TransitView   ---   dispatchCommand
**
******************************************************/
bool TransitView::dispatchCommand( const int &command )
{
	switch ( command )
	{
	case CMD_CHILD_EXPORT:
		if ( activewidget ) activewidget->doExport();
		else printf( "Error: no graphic export possible on page %d\n", activepage );
		break;
	case CMD_CHILD_SAVETEXT:
		if ( activetext ) activetext->doExport();
		else printf( "Error: no text export possible on page %d\n", activepage );
		break;
	default:
		return BasicView::dispatchCommand( command );
		break;
	}
	return true;
}

/*****************************************************
**
**   TransitView   ---   supportsEWToggle
**
******************************************************/
bool TransitView::supportsEWToggle() const
{
	switch ( activepage )
	{
	case TNB_SBC:
	case TNB_SHADVARGA:
	case TNB_VARGA2:
	case TNB_VARGA3:
		return false;
		break;
	case TNB_DOUBLE:
	case TNB_URANIAN:
	default:
		return true;
		break;
	}
	return true;
}

/*****************************************************
**
**   TransitView   ---   supportsFullGraphicStyleToggle
**
******************************************************/
bool TransitView::supportsFullGraphicStyleToggle() const
{
	return( activepage != TNB_URANIAN && activepage != TNB_SBC );
}

/*****************************************************
**
**   TransitView   ---   supportsSkinToggle
**
******************************************************/
bool TransitView::supportsSkinToggle() const
{
	return( activepage != TNB_URANIAN );
}

/*****************************************************
**
**   TransitView   ---   supportsSbcStyleToggle
**
******************************************************/
bool TransitView::supportsSbcStyleToggle() const
{
	return( activepage == TNB_SBC );
}

/*****************************************************
**
**   TransitView   ---   supportsGraphicExport
**
******************************************************/
bool TransitView::supportsGraphicExport() const
{
	return( activewidget != 0 );
}

/*****************************************************
**
**   TransitView   ---   supportsTextExport
**
******************************************************/
bool TransitView::supportsTextExport() const
{
	return ( activetext != 0 );
}

/*****************************************************
**
**   TransitView   ---   isVedic
**
******************************************************/
bool TransitView::isVedic() const
{
	return( supportsEWToggle() ? ChartProperties::isVedic() : true );
}

/**************************************************************
***
**   TransitView   ---   initToolItems
***
***************************************************************/
void TransitView::initToolItems()
{
	if ( toolbar )
	{
		wxChoice *choice_mode = (wxChoice*)toolbar->FindControl( TBS_TRANSITMODE );
		if ( choice_mode ) choice_mode->SetSelection( config->transitModus );

		wxChoice *choice_tz = (wxChoice*)toolbar->FindControl( TBS_TZ );
		if ( choice_tz ) choice_tz->SetSelection( tzchoice );

		YearLengthChoice *choice_yl = (YearLengthChoice*)toolbar->FindControl( TBS_YEARLENGTH );
		if ( choice_yl ) choice_yl->SetSelection( config->wYearLength );

		wxSpinCtrl *spin_orbis = (wxSpinCtrl*)toolbar->FindControl( TBS_ORBIS );
		if ( spin_orbis ) spin_orbis->SetValue( config->orbisTransit );

		CircleTypeChoice *choice_degree = (CircleTypeChoice*)toolbar->FindControl( TBS_CIRCLETYPE );
		if ( choice_degree ) choice_degree->SetSelection( config->circleType );

		SortChoice *choice_sort = (SortChoice*)toolbar->FindControl( TBS_SORT );
		if ( choice_sort ) choice_sort->SetSelection( config->planetSortOrder );
	}

	setDateTimeSpinsFromTransitdate();
}

/*****************************************************
**
**   TransitView   ---   getDeltaValue
**
******************************************************/
double TransitView::getDeltaValue( const int &v )
{
	double ret = 0;
	int val = v > 0 ? v : -v;
	switch ( val )
	{
	case 1: // second
		ret = 1/ 86400.0;
		break;
	case 2: // Minute
		ret = 1/ 1440.0;
		break;
	case 3: // hour
		ret = 1/ 24.0;
		break;
	case 4: // day
		ret = 1.0;
		break;
	case 5: // week
		ret = 7.0;
		break;
	case 6: // month
		ret = 30.0;
		break;
	case 7: //year
		ret = 365.25;
		break;
	}
	if ( v < 0 ) ret *= -1;
	return ret;
}

/*****************************************************
**
**   TransitView   ---   OnDateProgress
**
******************************************************/
void TransitView::OnDateProgress( wxScrollEvent &event )
{
	int type = event.GetEventType();
	if ( type == wxEVT_SCROLL_THUMBRELEASE )
	{
		resetSlider();
	}
	else if ( type == wxEVT_SCROLL_THUMBTRACK )
	{
		if ( ! timer->IsRunning() )
		{
			timer->Start( TIMER_PERIOD );
		}
	}
	dirty = true;
}

/**************************************************************
***
**   TransitView   ---   getSliderValue
***
***************************************************************/
int TransitView::getSliderValue()
{
	int delta = 0;
	wxSlider *slider = (wxSlider*)toolbar->FindControl( TBS_DATE_PROGRESS );
	if ( slider ) delta = slider->GetValue();
	return delta;
}

/**************************************************************
***
**   TransitView   ---   resetSlider
***
***************************************************************/
void TransitView::resetSlider()
{
	if ( toolbar )
	{
		wxSlider *slider = (wxSlider*)toolbar->FindControl( TBS_DATE_PROGRESS );
		if ( slider ) slider->SetValue( 0 );
	}
	if ( timer->IsRunning() )
	{
		timer->Stop();
	}
	dirty = true;
}

/**************************************************************
***
**   TransitView   ---   OnTimer
***
***************************************************************/
void TransitView::OnTimer( wxTimerEvent &event )
{
	int delta = getSliderValue();
	if ( delta == 0 )
	{
		timer->Stop();
	}
	else
	{
		transitdate += getDeltaValue( delta );
		setDateTimeSpinsFromTransitdate();
		OnDataChanged();
	}
	dirty = true;
}

/**************************************************************
***
**   TransitView   ---   OnIdle
***
***************************************************************/
void TransitView::OnIdle( wxIdleEvent &event )
{
	if ( dirty ) OnDataChanged();
}

/*****************************************************
**
**   TransitView   ---   OnToolbarCommand
**
******************************************************/
void TransitView::OnToolbarCommand()
{
	dirty = true;
}

/*****************************************************
**
**   TransitView   ---   OnDataChanged
**
******************************************************/
void TransitView::OnDataChanged()
{
	int transitmode = 0;
	BasicView::OnDataChanged();

	// must be called each time because dst may have changed
	calcTzOffset();

	if ( toolbar )
	{
		// Date and Time
		setTransitdateFromDateTimeSpins();
		expert->setTransitDate( transitdate, tzoffset );

		// Mode
		wxChoice *choice_mode = (wxChoice*)toolbar->FindControl( TBS_TRANSITMODE );
		if ( choice_mode )
		{
			transitmode = choice_mode->GetSelection();
			expert->setTransitMode( transitmode );
			config->transitModus = transitmode;
		}

		// Year length
		YearLengthChoice *choice_yl = (YearLengthChoice*)toolbar->FindControl( TBS_YEARLENGTH );
		if ( choice_yl )
		{
			ADJUST_TOOLBAR_ITEM ( choice_yl, transitmode != 0 );
			double yearlength = calcYearLength( choice_yl->GetSelection(), false );
			expert->setYearLength( yearlength );
			config->wYearLength = choice_yl->GetSelection();
		}

		// sort order
		SortChoice *choice_sort = (SortChoice*)toolbar->FindControl( TBS_SORT );
		if ( choice_sort )
		{
			expert->setSortOrder( choice_sort->GetSelection() );
			ADJUST_TOOLBAR_ITEM( choice_sort, activepage == TNB_URANIAN );
			config->planetSortOrder = choice_sort->GetSelection();
		}

		// Orbis (uranian only)
		wxSpinCtrl *spin_orbis = (wxSpinCtrl*)toolbar->FindControl( TBS_ORBIS );
		if ( spin_orbis )
		{
			expert->setOrbis( spin_orbis->GetValue() / 60.0 );
			ADJUST_TOOLBAR_ITEM( spin_orbis, activepage == TNB_URANIAN );
		}

		CircleTypeChoice *choice_degree = (CircleTypeChoice*)toolbar->FindControl( TBS_CIRCLETYPE );
		if ( choice_degree )
		{
			expert->setCircleDegrees( choice_degree->GetSelection() );
			ADJUST_TOOLBAR_ITEM( choice_degree, activepage == TNB_URANIAN );
		}

	}
	expert->update();

	if ( activepage == TNB_DOUBLE )
	{
		gwidget->OnDataChanged();
		gwidget->Refresh();
		writeAspectText();
	}
	else if ( activepage == TNB_URANIAN ) writeUranianText();
	else if ( activepage == TNB_SBC )
	{
		swidget->OnDataChanged();
	}
	if ( activewidget ) activewidget->Refresh();
	dirty = false;
}

/*****************************************************
**
**   TransitView   ---   OnSetToCurrent
**
******************************************************/
void TransitView::OnSetToCurrent( wxCommandEvent &event )
{
	resetSlider();
	setToCurrent();
}

/*****************************************************
**
**   TransitView   ---   writeAspectText
**
******************************************************/
void TransitView::writeAspectText()
{
	aspectText->writeContents();
}

/*****************************************************
**
**   TransitView   ---   writeUranianText
**
******************************************************/
void TransitView::writeUranianText()
{
	uranianText->writeContents();
}

/*****************************************************
**
**   TransitView   ---   OnNotebook
**
******************************************************/
void TransitView::OnNotebook( wxNotebookEvent &event )
{
	activepage = event.GetSelection();
	switch ( activepage )
	{
	case TNB_DOUBLE:
		activetext = aspectText;
		activewidget = gwidget;
		break;

	case TNB_SBC:
		activetext = 0;
		activewidget = swidget;
		break;

	case TNB_SHADVARGA:
		activetext = 0;
		activewidget = shadvargawidget;
		break;

	case TNB_VARGA2:
		activetext = 0;
		activewidget = varga2widget;
		break;

	case TNB_VARGA3:
		activetext = 0;
		activewidget = varga3widget;
		break;

	case TNB_URANIAN:
		activetext = uranianText;
		activewidget = 0;
		break;

	default:
		printf( "Error: wrong notebook index %d in TransitView::OnNotebook\n", activepage );
		break;
	}
	OnDataChanged();
}

/**************************************************************
***
**   TransitView   ---   getWindowLabel
***
***************************************************************/
wxString TransitView::getWindowLabel( const bool shortname )
{
	return _( "Transit" );
}

/**************************************************************
***
**   ViewFactory   ---   createTransitView
***
***************************************************************/
BasicView *ViewFactory::createTransitView( wxWindow *parent, ChildWindow *frame, Document *doc, const bool &vedic )
{
	return new TransitView( parent, frame, doc, vedic );
}

