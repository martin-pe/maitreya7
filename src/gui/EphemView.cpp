/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/EphemView.cpp
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
#pragma implementation "EphemView.h"
#endif

#include "EphemView.h"

#include <wx/choice.h>
#include <wx/dc.h>
#include <wx/dcmemory.h>
#include <wx/log.h>
#include <wx/msgdlg.h>
#include <wx/spinctrl.h>
#include <wx/statbox.h>
#include <wx/string.h>
#include <wx/toolbar.h>

#include "ChartProperties.h"
#include "ColorConfig.h"
#include "Conf.h"
#include "Ephemeris.h"
#include "FontConfig.h"
#include "guibase.h"
#include "Lang.h"
#include "mathbase.h"
#include "Painter.h"
#include "PlanetList.h"
#include "TextWidget.h"
#include "Writer.h"

extern Config *config;

IMPLEMENT_CLASS( EphemView, BasicView )
IMPLEMENT_CLASS( EphemWidget, BasicWidget )

/*************************************************//**
*
*
*
******************************************************/
class EphemTextWidget : public TextWidget
{
	DECLARE_CLASS( EphemTextWidget )

public:
	EphemTextWidget( wxWindow *parent, BasicView *v, EphemExpert *expert )
			: TextWidget( parent, v )
	{
		this->expert = expert;
	}

	virtual int write()
	{
		int ret = 0;
		switch ( mode )
		{
		case 1:
			assert( false );
			break;
		case 2:
			expert->writeDetails( writer );
			break;
		case 3:
			ret = expert->writeIngress( writer );
			break;
		case 4:
			expert->writeLunar( writer );
			break;
		case 5:
			expert->writeKp( writer, dasa );
			break;
		default:
			ret = expert->writeDefaultEphemeris( writer );
			break;
		}
		return ret;
	}

	int mode, dasa;

private:
	EphemExpert *expert;
};

IMPLEMENT_CLASS( EphemTextWidget, TextWidget )

/*****************************************************
**
**   EphemView   ---   Constructor
**
******************************************************/
EphemView::EphemView( wxWindow *parent, ChildWindow *frame, const bool &vedic )
		: BasicView( parent, frame, 0, VIEW_EPHEM, true, vedic )
{
	supportEWToggle = supportRestrictedObjectToggle = true;
	int dummy;
	Formatter::get()->getDateIntsFromJD( JDate::getCurrentJD(), dummy, month, year );
	expert = new EphemExpert( this );

	lastmonth = month;
	dasa = 0;
	planets = 1;
	mode = config->ephemMode;
	circleType = config->ephemCircleType;

	gmode = ( mode == 1 );
	theText = 0;
	ewidget = 0;
	isLocaltime = config->ephemTimezone;

	expert->prepareMonth( month, year, isLocaltime );

	initToolItems();
	initClientView();
	updateView = true;

	Connect( TBB_NOW, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EphemView::OnNow ));

	Connect( TBS_YEAR, wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( EphemView::OnYearSpin ));
	Connect( TBS_MONTH, wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( EphemView::OnMonthSpin ));

	Connect( TBS_EPHEMPLANETS, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( EphemView::OnChoiceCommand ));
	Connect( TBS_TZ, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( EphemView::OnChoiceTZ ));
	Connect( TBS_DASA, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( EphemView::OnChoiceCommand ));
	Connect( TBS_EPHEMMODE, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( EphemView::OnChoiceCommand ));
	Connect( TBS_EPHEMDEG, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( EphemView::OnChoiceCommand ));
	Connect( wxEVT_IDLE, wxIdleEventHandler( EphemView::OnIdle ));
}

/*****************************************************
**
**   EphemView   ---   Destructor
**
******************************************************/
EphemView::~EphemView()
{
	delete expert;
}

/*****************************************************
**
**   EphemView   ---   initToolItems
**
******************************************************/
void EphemView::initToolItems()
{
	if ( toolbar )
	{
		wxSpinCtrl *spin_year = (wxSpinCtrl*)toolbar->FindControl( TBS_YEAR );
		if ( spin_year ) spin_year->SetValue( year );

		wxSpinCtrl *spin_month = (wxSpinCtrl*)toolbar->FindControl( TBS_MONTH );
		if ( spin_month ) spin_month->SetValue( month );

		wxChoice *choice_ephem_mode = (wxChoice*)toolbar->FindControl( TBS_EPHEMMODE );
		if ( choice_ephem_mode ) choice_ephem_mode->SetSelection( mode );

		wxChoice *choice_ephemtz = (wxChoice*)toolbar->FindControl( TBS_TZ );
		if ( choice_ephemtz ) choice_ephemtz->SetSelection( isLocaltime );

		wxChoice *choice_ephemdeg = (wxChoice*)toolbar->FindControl( TBS_EPHEMDEG );
		if ( choice_ephemdeg )
		{
			choice_ephemdeg->SetSelection( circleType );
			ADJUST_TOOLBAR_ITEM( choice_ephemdeg, mode == 1 );
		}

		wxChoice *choice_dasa = (wxChoice*)toolbar->FindControl( TBS_DASA );
		if ( choice_dasa )
		{
			ADJUST_TOOLBAR_ITEM( choice_dasa, mode == 5 );
		}
	}
}

/*****************************************************
**
**   EphemView   ---   supportsGraphicExport
**
******************************************************/
bool EphemView::supportsGraphicExport()
{
	return ( mode == 1 );
}

/*****************************************************
**
**   EphemView   ---   supportsTextExport
**
******************************************************/
bool EphemView::supportsTextExport()
{
	return ( mode != 1 );
}

/*****************************************************
**
**   EphemView   ---   initClientView
**
******************************************************/
void EphemView::initClientView()
{
	if ( ewidget ) delete ewidget;
	ewidget = 0;
	if ( theText ) delete theText;
	theText = 0;
	if ( mode == 1 )
	{
		ewidget = new EphemWidget( this, this, expert );
		setMaxDeg( circleType );
		widget = ewidget;
	}
	else
	{
		theText = new EphemTextWidget( this, this, expert );
		widget = theText;
	}
	doLayout();
}

/*****************************************************
**
**   EphemView   ---   OnDataChanged
**
******************************************************/
void EphemView::OnDataChanged()
{
	BasicView::OnDataChanged();
	OnToolbarCommand();
	expert->prepareMonth( month, year, isLocaltime );
	updateView = true;
}

/*****************************************************
**
**   EphemView   ---   OnChoiceTZ
**
******************************************************/
void EphemView::OnChoiceTZ( wxCommandEvent& )
{
	OnDataChanged();
}

/*****************************************************
**
**   EphemView   ---   OnYearSpin
**
******************************************************/
void EphemView::OnYearSpin( wxSpinEvent &event )
{
	OnDataChanged();
}

/*****************************************************
**
**   EphemView   ---   OnMonthSpin
**
******************************************************/
void EphemView::OnMonthSpin( wxSpinEvent &event )
{
	OnDataChanged();
}

/*****************************************************
**
**   EphemView   ---   OnToolbarCommand
**
******************************************************/
void EphemView::OnToolbarCommand()
{
	if ( toolbar )
	{
		wxChoice *choice_mode = (wxChoice*)toolbar->FindControl( TBS_EPHEMMODE );
		if ( choice_mode )
		{
			if ( mode != choice_mode->GetSelection() )
			{
				mode = choice_mode->GetSelection();
				config->ephemMode = mode;
				bool newgmode = ( mode == 1 );
				if ( gmode != newgmode )  initClientView();
				gmode = newgmode;
			}
		}

		wxSpinCtrl *spin_year = (wxSpinCtrl*)toolbar->FindControl( TBS_YEAR );
		if ( spin_year ) year = spin_year->GetValue();

		wxSpinCtrl *spin_month = (wxSpinCtrl*)toolbar->FindControl( TBS_MONTH );
		if ( spin_year && spin_month )
		{
			if ( lastmonth == 12 && spin_month->GetValue() == 1 )
				spin_year->SetValue( spin_year->GetValue() + 1 );
			if ( lastmonth == 1 && spin_month->GetValue() == 12 )
				spin_year->SetValue( spin_year->GetValue() - 1 );

			lastmonth = spin_month->GetValue();
			year = spin_year->GetValue();
			month = spin_month->GetValue();
		}

		wxChoice *choice_ephemtz = (wxChoice*)toolbar->FindControl( TBS_TZ );
		if ( choice_ephemtz )
		{
			isLocaltime = choice_ephemtz->GetSelection();
			config->ephemTimezone = isLocaltime;
		}

		wxChoice *choice_ephemdeg = (wxChoice*)toolbar->FindControl( TBS_EPHEMDEG );
		if ( choice_ephemdeg )
		{
			circleType = choice_ephemdeg->GetSelection();
			config->ephemCircleType = circleType;
			setMaxDeg( circleType );
			ADJUST_TOOLBAR_ITEM( choice_ephemdeg, mode == 1 );
		}

		wxChoice *choice_dasa = (wxChoice*)toolbar->FindControl( TBS_DASA );
		if ( choice_dasa )
		{
			dasa = choice_dasa->GetSelection();
			ADJUST_TOOLBAR_ITEM( choice_dasa, mode == 5 );
		}
		updateView = true;
	}
}

/*****************************************************
**
**   EphemView   ---   setMaxDeg
**
******************************************************/
void EphemView::setMaxDeg( const int &sel )
{
	double max_deg;
	switch ( sel )
	{
	case 0 :
		max_deg =   360;
		break;
	case 1 :
		max_deg =   180;
		break;
	case 2 :
		max_deg =    90;
		break;
	case 3 :
		max_deg =    45;
		break;
	case 4 :
		max_deg =    30;
		break;
	case 5 :
		max_deg =  22.5;
		break;
	case 6 :
		max_deg = 13.333333333;
		break;
	case 7 :
		max_deg = 11.25;
		break;
	case 8 :
		max_deg = 5.125;
		break;
	default:
		max_deg =   360;
		break;
	}
	if ( ewidget ) ewidget->setMaxDeg( max_deg );
}

/*****************************************************
**
**   EphemView   ---   OnNow
**
******************************************************/
void EphemView::OnNow( wxCommandEvent &event )
{
	int dummy;
	if ( toolbar )
	{
		wxSpinCtrl *spin_year = (wxSpinCtrl*)toolbar->FindControl( TBS_YEAR );
		wxSpinCtrl *spin_month = (wxSpinCtrl*)toolbar->FindControl( TBS_MONTH );
		if ( spin_year && spin_month )
		{
			Formatter::get()->getDateIntsFromJD( JDate::getCurrentJD(), dummy, month, year );
			spin_year->SetValue( year );
			spin_month->SetValue( month );
			lastmonth = month;
		}
		expert->prepareMonth( month, year, isLocaltime );
	}
	updateView = true;
}

/*****************************************************
**
**   EphemView   ---   dispatchCommand
**
******************************************************/
bool EphemView::dispatchCommand( const int &command )
{
	if ( command == CMD_CHILD_SAVETEXT )
	{
		theText->doExport();
		return true;
	}
	else if ( command == CMD_CHILD_EXPORT )
	{
		if ( ewidget ) ewidget->doExport();
		else printf( "No ephem widget defined!\n" );
		return true;
	}
	else return BasicView::dispatchCommand( command );
}

/**************************************************************
***
**   EphemView   ---   OnIdle
***
***************************************************************/
void EphemView::OnIdle( wxIdleEvent &event )
{
	if ( ! updateView ) return;

	int ret = 0;
	if ( gmode )
	{
		ret = expert->calcMonth();
		assert( ewidget );
		ewidget->setFilter( planets );
		ewidget->Refresh();
	}
	else
	{
		assert( theText );
		theText->mode = mode;
		theText->dasa = dasa;
		ret = theText->writeContents();
	}
	if ( ret )
	{
		wxCommandEvent event( SHOW_EPHEM_FILE_WARNING );
		event.SetEventObject( this );
		wxPostEvent( GetParent(), event );
	}
	
	updateView = false;
}

/**************************************************************
***
**   EphemView   ---   getWindowLabel
***
***************************************************************/
wxString EphemView::getWindowLabel( const bool shortname )
{
	return _( "Ephemeris" );
}

/*****************************************************
**
**   EphemView   ---   supportsTextExport
**
******************************************************/
bool EphemView::supportsTextExport() const
{
	return( theText != 0 );
}

/**************************************************************
***
**   EphemWidget   ---   Constructor
***
***************************************************************/
EphemWidget::EphemWidget( wxWindow *parent, ChartProperties *chartprops, EphemExpert *e )
		: BasicWidget( parent, chartprops ),
		expert( e )
{
	max_deg = 360;
	filter = 1;
	writer = WriterFactory().getWriter( WRITER_TEXT );

	Connect( wxEVT_SIZE, wxSizeEventHandler( EphemWidget::OnSize ));
}

/**************************************************************
***
**   EphemWidget   ---   Destructor
***
***************************************************************/
EphemWidget::~EphemWidget()
{
	delete writer;
}

/*****************************************************
**
**   EphemWidget   ---   setZoom
**
******************************************************/
void EphemWidget::setZoom( const int z )
{
	if ( z ) zoom = z;
	int width = 100 * zoom;
	int height = 500;
	SetVirtualSize( width, height );

	xleft = 6 * hborder;
	xright = width - xleft - 4 * hborder;
	ytop = 2 * vborder;
	ybottom = height - 4 * vborder;
}

/*****************************************************
**
**   EphemWidget   ---   OnSize
**
******************************************************/
void EphemWidget::OnSize( wxSizeEvent &event )
{
	//printf( "EphemWidget::OnSize x %d y %d\n", event.GetSize().x, event.GetSize().y );
	Refresh();
}

/*****************************************************
**
**   EphemWidget   ---   paintRuler
**
******************************************************/
void EphemWidget::paintRuler()
{
	wxString s;
	int i;
	int xtext = 50;
	int ytext = 10;
	int ysizey = 20;
	int y = (int)ybottom-30;
	int x = (int)xleft;
	double ystep;

	painter->setTransparentBrush();

	painter->setPenColor( config->colors->fgColor );
	painter->drawRectangle( (int)xleft, (int)ybottom, (int)(xright - xleft), (int)(ytop - ybottom) );
	int mlen = expert->getNumberOfDays();
	double dstep = ( xright - xleft ) / ( mlen - 1 );
	for ( i = 0; i < mlen; i++ )
	{
		painter->setPenColor( config->colors->fgColor );
		x =  (int)(xleft+i*dstep);
		painter->drawLine( x, (int)ybottom, x, (int)ybottom+5 );
		s.Printf( wxT( "%d" ), i+1 );
		drawTextFormatted( wxRect( x-xtext, y + ysizey + ytext, 2*xtext, 2*ytext ), s, Align::Center );
		if ( ! ( ( i + 1 ) % 5 ) && i > 0 )
		{
			painter->setPenColor( *wxLIGHT_GREY ); // TODO DASH
			painter->drawLine( (int)x, (int)ybottom, (int)x, (int)ytop );
		}
	}

	ystep = (ybottom - ytop) / max_deg;
	for ( i = 1; i < max_deg; i++ )
	{
		painter->setPenColor( config->colors->fgColor );

		if ( max_deg >= 180 && ( i % 30 ) ) continue;
		if ( max_deg >= 90 && ( i % 10 ) ) continue;
		if ( max_deg >= 22 && ( i % 5 ) ) continue;
		y = (int)(ybottom - i * ystep);
		painter->drawLine( (int)xleft-5, (int)y, (int)xleft, (int)y );
		s.Printf( wxT( "%d" ), i );
		drawTextFormatted( wxRect( (int)xleft-xtext-5, y - ysizey, xtext, 2 * ysizey ), s, Align::Right+Align::VCenter );

		painter->setPenColor( *wxLIGHT_GREY ); // TODO DASH
		painter->drawLine( (int)xleft, (int)y, (int)xright, (int)y );
	}

	// Finally mark current date if possible
	if ( expert->isCurrentMonth())
	{
		JDate d;
		double actjd = d.getJD();
		d.setDate( 1, expert->getMonth(), expert->getYear(), 0 );
		double portion = ( actjd - d.getJD() ) / expert->getNumberOfDays();
		x = xleft + portion * ( xright - xleft );
		painter->setPenColor( *wxBLUE );
		painter->drawLine( (int)x, (int)ybottom, (int)x, (int)ytop );
	}
}

/*****************************************************
**
**   EphemWidget   ---   doPaint
**
******************************************************/
void EphemWidget::doPaint( bool eraseBackground, const wxRect* rect )
{
	wxCoord w, h;
	expert->calcMonth();

	assert( painter );
	GetVirtualSize( &w, &h );

	painter->setBrushColor( config->colors->bgColor );
	painter->setPenColor( config->colors->bgColor );
	painter->drawRectangle( hborder, vborder, (int)(w - 2 * hborder), (int)(h - 2 * vborder) );

	painter->setGraphicFont();
	painter->setTextColor( config->colors->fgColor );

	xleft = 6 * hborder;
	xright = w - xleft - 2 * hborder;
	ytop = 2 * vborder;
	ybottom = h - 4 * vborder;

	paintRuler();
	paintPlanets();
}

/*****************************************************
**
**   EphemWidget   ---   paintPlanets
**
******************************************************/
void EphemWidget::paintPlanets()
{
	int p;
	int x1, x2, y1, y2;  // daily positions in y and y dimension
	int xp;              // x position for jumps
	//int i;
	double l1, l2;       // length of planets
	double yp, yp2, ydiff;      // length values for jumps
	const int sshift = 60;
	int lsymbolshift[sshift];
	int rsymbolshift[sshift];
	int ylshift, yrshift;
	const int xshiftunit = 15;

	const int ytotal =  (int)(ybottom - ytop);   // total height of interior
	const int mlen = expert->getNumberOfDays();  // length of month
	const double dstep = ( xright - xleft ) / mlen;  // daily step in x dimension
	//const int lstep = 20;                 // step height for legend entries

	wxString s;
	Lang lang;

	for ( int i = 0; i < sshift; i++ )
	{
		lsymbolshift[i] = 0;
		rsymbolshift[i] = 0;
	}

	for ( unsigned i1 = 0; i1 < expert->getPlanetdataSize(); i1++ )
	{
		p = expert->getPlanetId( i1 );

		// loop if daily motion of moon is smaller than max_deg
		if ( max_deg < 14 &&  p == OMOON ) continue;

		setLineStyle( p );
		for ( int day = 0; day < mlen; day++ )
		{
			x1 =  (int)(xleft + day * dstep);
			x2 =  (int)(x1 + dstep);
			l1 = a_red( expert->getPlanetLength( i1, day ), max_deg );
			y1 = (int)(ybottom - l1 * ytotal / max_deg);
			l2 = a_red( expert->getPlanetLength( i1, day+1 ), max_deg );
			y2 = (int)(ybottom - l2 * ytotal / max_deg);

			if ( expert->getPlanetRetro( i1, day ))
			{
				if (( l1 < l2 ) && expert->getPlanetRetro( i1, day+1 )) // handle jumps
				{
					yp = l1;
					yp2 = max_deg - l2;
					ydiff = yp / ( yp + yp2 );
					xp = (int)( x1 + ydiff * dstep );
					painter->drawLine( x1, y1, xp, (int)ybottom );
					painter->drawLine( xp, (int)ytop, x2, y2 );
				}
				else // that's normal
				{
					painter->drawLine( x1, y1, x2, y2 );
				}
			}
			else // non retrograde
			{
				if (( l1 > l2 ) && ! expert->getPlanetRetro( i1, day+1 )) // handle jumps
				{
					yp = max_deg - l1;
					yp2 = l2;
					ydiff = yp / ( yp + yp2 );
					xp = (int)( x1 + ydiff * dstep );
					//printf( "2 planet %d yp %f yp2 %f x1 %d x2 %d xp %d l1 %f l2 %f ydiff %f\n", p, yp, yp2, x1, x2, xp, l1, l2, ydiff );
					painter->drawLine( x1, y1, xp, (int)ytop );
					painter->drawLine( xp, (int)ybottom, x2, y2 );
				}
				else // that happens normally
				{
					painter->drawLine( x1, y1, x2, y2 );
				}
			}
		}
		if ( config->usePlanetSymbols )
		{
			painter->setSymbolFont();
			s = lang.getPlanetSymbolCode( p );
		}
		else
		{
			painter->setGraphicFont();
			s = writer->getObjectName( p, TMEDIUM, chartprops->isVedic() );
		}
		// Planet name on left side
		y1 = (int)(ybottom - a_red( expert->getPlanetLength( i1, 0 ), max_deg ) * ytotal / max_deg);
		ylshift = sshift * y1 / ytotal;
		drawTextFormatted( wxRect( Max( (int)xleft - xshiftunit * lsymbolshift[ylshift] - 10, 0 ), y1 - 5, 10, 10 ), s, Align::Right+Align::VCenter );
		lsymbolshift[ylshift]++;


		// Planet name on right side
		y1 = (int)(ybottom - a_red( expert->getPlanetLength( i1, mlen ), max_deg ) * ytotal / max_deg);
		yrshift = sshift * y1 / ytotal;
		drawTextFormatted( wxRect( (int)xright + xshiftunit * rsymbolshift[yrshift], y1 - 5, 20, 10 ), s, Align::Left+Align::VCenter );
		rsymbolshift[yrshift]++;
	}
}

/*****************************************************
**
**   EphemWidget   ---   setLineStyle
**
******************************************************/
void EphemWidget::setLineStyle( const int &planet )
{
	const int lw = 2;
	painter->setPen( wxPen( config->colors->getPlanetColor( planet ), lw, wxSOLID ));
}

/**************************************************************
***
**   ViewFactory   ---   createEphemView
***
***************************************************************/
BasicView *ViewFactory::createEphemView( wxWindow *parent, ChildWindow *frame, const bool &vedic )
{
	return new EphemView( parent, frame, vedic );
}

