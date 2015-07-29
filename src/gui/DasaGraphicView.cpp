/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/DasaGraphicView.cpp
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
#pragma implementation "DasaGraphicView.h"
#endif

#include "DasaGraphicView.h"

#include <wx/choicdlg.h>
#include <wx/choice.h>
#include <wx/dc.h>
#include <wx/event.h>
#include <wx/textdlg.h>
#include <wx/toolbar.h>

#include "ColorConfig.h"
#include "Conf.h"
#include "Dasa.h"
#include "DialogElements.h"
#include "Document.h"
#include "FontConfig.h"
#include "Lang.h"
#include "Painter.h"

extern Config *config;

IMPLEMENT_CLASS( DasaGraphicView, BasicView )
IMPLEMENT_CLASS( DasaGraphicWidget, BasicWidget )

/*****************************************************
**
**   DasaGraphicView   ---   Constructor
**
******************************************************/
DasaGraphicView::DasaGraphicView( wxWindow *parent, ChildWindow *frame, Document *doc )
		: BasicView( parent, frame, doc, VIEW_GRAPHICALDASA, true, true )
{
	supportEWToggle = false;
	setVedic( true );
	supportGraphicExport = true;

	swidget = new DasaGraphicWidget( this, this, doc, -1 );
	initToolItems();
	widget = swidget;

	Connect( TBB_DASA, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( DasaGraphicView::OnDasas ));
	Connect( TBB_NOW, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( DasaGraphicView::OnNow ));
	Connect( TBB_GOTO, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( DasaGraphicView::OnYear ));

	Connect( TBS_ZOOM, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( DasaGraphicView::OnChoiceCommand ));
	Connect( TBS_ANTARDASALEVEL, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( DasaGraphicView::OnChoiceAntardasa ));
}

/*****************************************************
**
**   DasaGraphicView   ---   initToolItems
**
******************************************************/
void DasaGraphicView::initToolItems()
{
	if ( toolbar )
	{
		wxChoice *choice_zoom = (wxChoice*)toolbar->FindControl( TBS_ZOOM );
		if ( choice_zoom )
		{
			choice_zoom->SetSelection( config->dasaZoomChoice );

			// set zoom for the first time
			wxString s = choice_zoom->GetStringSelection();
			if ( s.Len() > 0 )
			{
				if ( s.Last() == wxT( '%' )) s = s.Left( s.Len() - 1 );
				long z2 = 100;
				s.ToLong( &z2 );
				swidget->setZoom( (int)z2 );
			}
		}
		wxChoice *choice_antardasa = (wxChoice*)toolbar->FindControl( TBS_ANTARDASALEVEL );
		if ( choice_antardasa ) choice_antardasa->SetSelection( config->antarDasaLevelChoice );

		ToolbarLabel *tl = (ToolbarLabel*)toolbar->FindControl( TB_TEXT );
		if ( tl ) swidget->setTextField( tl );
	}
}

/*****************************************************
**
**   DasaGraphicView   ---   postCreate
**
******************************************************/
void DasaGraphicView::postCreate()
{
	swidget->setYear( 0 );
}

/*****************************************************
**
**   DasaGraphicView   ---   OnNow
**
******************************************************/
void DasaGraphicView::OnNow( wxCommandEvent &event )
{
	Formatter *formatter = Formatter::get();
	int month, day, year;

	formatter->getDateIntsFromJD( JDate::getCurrentJD(), day, month, year );
	swidget->setYear( year );
}

/*****************************************************
**
**   DasaGraphicView   ---   dispatchCommand
**
******************************************************/
bool DasaGraphicView::dispatchCommand( const int &command )
{
	if ( command == CMD_CHILD_EXPORT )
	{
		swidget->doExport();
		return true;
	}
	else return BasicView::dispatchCommand( command );
}

/*****************************************************
**
**   DasaGraphicView   ---   OnDataChanged
**
******************************************************/
void DasaGraphicView::OnDataChanged()
{
	swidget->OnDataChanged();
	swidget->Refresh();
}

/*****************************************************
**
**   DasaGraphicView   ---   OnYear
**
******************************************************/
void DasaGraphicView::OnYear( wxCommandEvent &event )
{
	wxString defvalue;
	JDate d;
	Formatter *formatter = Formatter::get();
	int month, day, year;
	long y;

	int birthyear = swidget->getBirthYear();
	int totalyears = swidget->getTotalYears();
	formatter->getDateIntsFromJD( d.getJD(), day, month, year );

	defvalue.Printf( wxT( "%d" ), year );

	wxTextEntryDialog dialog( this,
	                          wxString::Format( _("Please enter a year between %d and %d"), birthyear, birthyear + totalyears ),
	                          GUI_APP_NAME, defvalue );
	if ( dialog.ShowModal() == wxID_OK )
	{
		wxString s = dialog.GetValue();
		s.ToLong( &y );
		if ( y < birthyear || y > birthyear + totalyears )
		{
			doMessageBox( this,
			              wxString::Format( _("The value \"%s\" is not valid."), (const wxChar*)s ),
			              wxOK | wxICON_ERROR | wxCENTRE );
			return;
		}
		swidget->setYear( y );
	}
}

/*****************************************************
**
**   DasaGraphicView   ---   OnDasas
**
******************************************************/
void DasaGraphicView::OnDasas( wxCommandEvent &event )
{
	wxString l[DasaExpertFactory::MAX_DASAEXPERTS];

	for ( unsigned int i = 0; i < DasaExpertFactory::MAX_DASAEXPERTS; i++ )
		l[i] = DasaExpertFactory::get()->getDasaExpert( i )->getName();

	wxMultiChoiceDialog dialog( this, _( "Choose Dasas" ), GUI_APP_NAME, DasaExpertFactory::MAX_DASAEXPERTS, l );

	dialog.SetSelections( swidget->dasas );
	if ( dialog.ShowModal() == wxID_OK )
	{
		swidget->dasas = dialog.GetSelections();
		swidget->OnDataChanged();
		swidget->setZoom();
		swidget->Refresh();
	}
}

/*****************************************************
**
**   DasaGraphicView   ---   OnToolbarCommand
**
******************************************************/
void DasaGraphicView::OnToolbarCommand()
{
	if ( toolbar )
	{
		wxChoice *choice_zoom = (wxChoice*)toolbar->FindControl( TBS_ZOOM );
		assert( choice_zoom );

		config->dasaZoomChoice = choice_zoom->GetSelection();
		wxString combo = choice_zoom->GetStringSelection();
		if ( combo.Len() > 0 )
		{
			if ( combo.Last() == wxT( '%' )) combo = combo.Left( combo.Len() - 1 );
			long z2 = 100;
			combo.ToLong( &z2 );
			swidget->setZoom( (int)z2 );
		}
		swidget->Refresh();
	}
}

/*****************************************************
**
**   DasaGraphicView   ---   OnChoiceAntardasa
**
******************************************************/
void DasaGraphicView::OnChoiceAntardasa( wxCommandEvent &event )
{
	if ( toolbar )
	{
		wxChoice *choice_antardasa = (wxChoice*)toolbar->FindControl( TBS_ANTARDASALEVEL );
		assert( choice_antardasa );
		config->antarDasaLevelChoice = choice_antardasa->GetSelection();
		swidget->setAntardasaLevel( config->antarDasaLevelChoice );
		swidget->Refresh();
	}
}

/**************************************************************
***
**   DasaGraphicView   ---   getWindowLabel
***
***************************************************************/
wxString DasaGraphicView::getWindowLabel( const bool shortname )
{
	return shortname ? _( "Dasa" ) : _( "Dasa (Graphical)" );
}

/*****************************************************
**
**   DasaGraphicWidget   ---   Constructor
**
******************************************************/
DasaGraphicWidget::DasaGraphicWidget( wxWindow *parent, ChartProperties *chartprops, Horoscope *doc, wxWindowID id, const wxPoint& pos,
                                      const wxSize& size, long style )
		: BasicWidget( parent, chartprops )
{
	this->doc = doc;
	updateOfTextRequired = false;
	totalyears = 100;

	for ( unsigned int i = 0; i < DasaExpertFactory::MAX_DASAEXPERTS; i++ )
	{
		if ( ( config->vDefaultDasaList >> i ) % 2 ) dasas.Add( i );
	}

	theText = 0;
	mouse_x = mouse_y = 0;
	writeTextField( false );
	ybar = 30;  // constant, height of Dasa bar
	adlevel = config->antarDasaLevelChoice; // Dasa and Antardasa
	OnDataChanged();
	setZoom( 100 );

	Formatter *formatter = Formatter::get();
	int month, day, year;

	formatter->getDateIntsFromJD( JDate::getCurrentJD(), day, month, year );

	Connect( wxEVT_IDLE, wxIdleEventHandler( DasaGraphicWidget::OnIdle ));
	Connect( wxEVT_LEAVE_WINDOW, wxMouseEventHandler( DasaGraphicWidget::OnMouseLeave ));
	Connect( wxEVT_ENTER_WINDOW, wxMouseEventHandler( DasaGraphicWidget::OnMouseEnter ));
	Connect( wxEVT_MOTION, wxMouseEventHandler( DasaGraphicWidget::OnMouseMotion ));
}

/*****************************************************
**
**   DasaGraphicWidget   ---   Destructor
**
******************************************************/
DasaGraphicWidget::~DasaGraphicWidget()
{
	config->vDefaultDasaList = 0;
	for ( unsigned int i = 0; i < dasas.size(); i++ ) config->vDefaultDasaList += 1 << dasas[i];
	clearDasaItems();
}

/*****************************************************
**
**   DasaGraphicWidget   ---   clearDasaItems
**
******************************************************/
void DasaGraphicWidget::clearDasaItems()
{
	for ( unsigned int i = 0; i < dasaitems.size(); i++ ) delete dasaitems[i];
	dasaitems.clear();
}

/*****************************************************
**
**   DasaGraphicWidget   ---   OnMouseLeave
**
******************************************************/
void DasaGraphicWidget::OnMouseLeave( wxMouseEvent &event )
{
	mouse_x = mouse_y = 0;
	updateOfTextRequired = false;
	writeTextField( false );
}

/*****************************************************
**
**   DasaGraphicWidget   ---   OnMouseEnter
**
******************************************************/
void DasaGraphicWidget::OnMouseEnter( wxMouseEvent &event )
{
	mouse_x = event.m_x;
	mouse_y = event.m_y;
	updateOfTextRequired = true;
}

/*****************************************************
**
**   DasaGraphicWidget   ---   OnMouseMotion
**
******************************************************/
void DasaGraphicWidget::OnMouseMotion( wxMouseEvent &event )
{
	mouse_x = event.m_x;
	mouse_y = event.m_y;
	updateOfTextRequired = true;
	event.Skip();
}

/*****************************************************
**
**   DasaGraphicWidget   ---   HandleMouseWheelEvent
**
******************************************************/
void DasaGraphicWidget::HandleMouseWheelEvent( wxMouseEvent &event )
{
	int x, y, x0, y0, xs, ys;
	const int offset = 60;

	GetViewStart( &x, &y );      // scroll position
	GetSize( &xs, &ys );         // size of visible widget only
	GetVirtualSize( &x0, &y0 );  // complete size of widget

	// Wheel scolls in vertical direction, but horizontal if nothing left to scroll
	if ( event.GetWheelRotation() < 0 )
	{
		if ( y + ys >= y0 ) x = Min( x0, x + offset );
		else y += offset;
	}
	else
	{
		if ( y > 0 ) y = Max ( 0, y - offset );
		else x -= offset;
	}
	Scroll( x, y );
}

/*****************************************************
**
**   DasaGraphicWidget   ---   OnDataChanged
**
******************************************************/
void DasaGraphicWidget::OnDataChanged()
{
	unsigned int dasa, mahadasa, antardasa, pratyantardasa;
	DasaExpert *expert;
	GraphicalDasaItem *item, *depitem, *item2;
	vector<Dasa*> w, w2;
	Formatter *formatter = Formatter::get();
	int day, month;

	formatter->getDateIntsFromJD( doc->getJD(), day, month, birthyear );

	clearDasaItems();
	for ( dasa = 0; dasa < dasas.Count(); dasa++ ) // First loop generates Dasas like Vimsottari, Yogini, Kalachakra
	{
		expert = DasaExpertFactory::get()->getDasaExpert( dasas.Item( dasa ));
		item = new GraphicalDasaItem( expert, 0 );
		vector<Dasa*> v = expert->getFirstLevel( doc, 0 );
		if ( dasas.Item( dasa ) == 2 && ! v[0]->getExtraText().IsEmpty()) // Kalachakra
		{
			item->name.Printf( wxT( "%s (%s)" ), expert->getName(), (const wxChar*)(v[0]->getExtraText()) );
		}
		else {
			item->name = expert->getName();
		}

		for ( mahadasa = 0; mahadasa < v.size(); mahadasa++ )
		{
			depitem = new GraphicalDasaItem( expert, v[mahadasa] );
			if ( adlevel > 0 )
			{
				w = expert->getNextLevel( v[mahadasa] );
				for ( antardasa = 0; antardasa < w.size(); antardasa++ )
				{
					item2 = new GraphicalDasaItem( expert, w[antardasa] );
					if ( adlevel > 1 )
					{
						w2 = expert->getNextLevel( w[antardasa] );
						for ( pratyantardasa = 0; pratyantardasa < w2.size(); pratyantardasa++ )
						{
							item2->depitems.push_back( new GraphicalDasaItem( expert, w2[pratyantardasa] ) );
						}
						w2.clear();
					}
					depitem->depitems.push_back( item2 );
				}
				w.clear();
			}
			item->depitems.push_back( depitem );
		}
		dasaitems.push_back( item );
	}
}

/*****************************************************
**
**   DasaGraphicWidget   ---   paintRuler
**
******************************************************/
void DasaGraphicWidget::paintRuler()
{
	//int y = ycursor + ybar * dasas.Count() * 3;
	int y = yviewport - 3 * ybar;
	int ysizey = 20;
	int ylength;
	int x;
	int xtext = 50;
	int ytext = 10;
	int i, j, xm;
	double steplength = ( xright - xleft ) / totalyears;
	double xmonth = steplength / 12;
	wxString s;
	JDate d;

	painter->setTransparentBrush();

	wxPen fgpen( config->colors->fgColor );
	painter->setPen( fgpen );
	painter->drawLine( (int)xleft, y, (int)xright, y );
	painter->setTextColor( config->colors->fgColor );
	painter->setGraphicFont();

	for ( i = 0; i <= totalyears; i++ )
	{
		x = (int)(xleft + i * steplength);
		painter->drawLine( x, (int)(ytop + 2 * hborder), x, y );
		painter->setPen( fgpen );
		painter->drawLine( x, y - ysizey, x, y + ysizey );
		s.Printf( wxT( "%d" ), birthyear + i );
		if ( i > 0 && i < totalyears )
			drawTextFormatted( wxRect( x-xtext, y + ysizey + ytext, 2*xtext, 2*ytext ), s, Align::Center );
		if ( i < totalyears )
		{
			for ( j = 1; j < 12; j++ )
			{
				xm = (int)(x + j* xmonth);
				ylength = ysizey/( j % 3 ? 3 : 2 );
				if ( xmonth > 3 ) painter->drawLine( xm, y - ylength, xm, y + ylength );
				if (( xmonth > 3 && ! ( j%3 )) || xmonth > 15 )
				{
					s.Printf( wxT( "%d" ), j+1 );
					drawTextFormatted( wxRect( xm-xtext, y + ytext, 2*xtext, 2*ytext ), s, Align::Center );
				}
			}
		}
	}

	// Finally mark current date
	painter->setPen( wxPen( *wxBLUE, 2, wxSOLID ));
	double actjd = d.getJD();
	d.setDate( 1, 1 , birthyear, 0 );
	double yy = ( actjd - d.getJD()  ) / 365.25; // age in years
	x = (int)(xleft + (xright - xleft ) * yy / totalyears );
	painter->drawLine( x, (int)(ytop + hborder), x, y + 2 * ysizey );
}

/*****************************************************
**
**   DasaGraphicWidget   ---   setYear
**
******************************************************/
void DasaGraphicWidget::setYear( int year )
{
	Formatter *formatter = Formatter::get();
	int day, month;
	assert( doc );
	if ( year == 0 ) {
		formatter->getDateIntsFromJD( JDate::getCurrentJD(), day, month, year );
	}
	double portion = ( year - birthyear ) / (double)totalyears;
	double y = portion * ( xright - xleft );
	Scroll( (int)(y/scrolly), 0 );
}

/*****************************************************
**
**   DasaGraphicWidget   ---   setAntardasaLevel
**
******************************************************/
void DasaGraphicWidget::setAntardasaLevel( const int &a )
{
	adlevel = a;
	OnDataChanged();
	setZoom( zoom );
}

/*****************************************************
**
**   DasaGraphicWidget   ---   setZoom
**
******************************************************/
void DasaGraphicWidget::setZoom( const int z )
{
	int oldx, oldy, newx, newy;
	GetViewStart( &oldx, &oldy );
	double prop = z;
	prop /= zoom;
	newx = (int)( oldx * prop );
	newy = (int)( oldy * prop );

	if ( z ) zoom = z;
	xviewport = 100 * zoom;
	//int height = 100 + 100 * dasas.Count();
	yviewport = 5 * ybar +  ybar * ( adlevel + 2 ) * dasas.Count();
	SetVirtualSize( xviewport, yviewport );

	xleft = 2 * hborder;
	xright = xviewport - 4 * hborder;
	ytop = 2 * vborder;
	ybottom = yviewport - 4 * vborder;
	Scroll( newx, newy );
}

/*****************************************************
**
**   DasaGraphicWidget   ---   getX4Jd
**
******************************************************/
int DasaGraphicWidget::getX4Jd( const double &jd )
{
	double a = (xright - xleft)/(totalyears * 365.25); // pro tag
	return (int)(a * ( jd - startjd ) + 2 * hborder );
}

/*****************************************************
**
**   DasaGraphicWidget   ---   showDasaBar
**
******************************************************/
void DasaGraphicWidget::showDasaBar( Dasa *d, DasaExpert *expert, double y )
{
	int x1 = getX4Jd( Max( d->getStartJD(), doc->getJD()));
	int x2 = getX4Jd( Min( d->getEndJD(), endjd )) - x1;
	int xstart, dummy, xwidth;

	GetViewStart( &xstart, &dummy );
	GetClientSize( &xwidth, &dummy );

	// Nothing to do because bar is outside refresh area
	if ( ! exportMode && ( x1 > xstart + xwidth || xstart > x2 + x1  )) return;

	if ( ! refreshRect.Intersects( wxRect( x1, y, x2, (int)ybar )))
	{
		return;
	}

	int objectid = config->colors->getDasaColorConfigId( d->getDasaLord(), expert->isRasiDasaExpert() );
	GraphicalDasaPaintHelper( painter ).drawDasaBar( d->getDasaLord(), expert->isRasiDasaExpert(),
		&config->colors->dasa[objectid], x1, y, x2, (int)ybar, config->dasaCornerRadius );
}

/*****************************************************
**
**   DasaGraphicWidget   ---   paintDasa
**
******************************************************/
void DasaGraphicWidget::paintDasa( const int &whichdasa )
{
	unsigned int i, j, k;
	GraphicalDasaItem *item2, *item3, *item4, *dasaitem = dasaitems[whichdasa];
	wxString s;
	double w, h;

	int x1 = getX4Jd( doc->getJD());
	int x2;
	for ( i = 0; i < dasaitem->depitems.size(); i++ )
	{
		painter->setGraphicFont();
		//dc->SetTextBackground( *wxWHITE );
		painter->setTextColor( *wxBLACK );

		s =  dasaitem->name;
		painter->getTextExtent( s, &w, &h );

		x2 = x1;
		while ( x2 < xright - w )
		{
			drawTextFormatted( wxRect( x2, (int)(ycursor-ybar), 100, (int)ybar ), s, Align::Bottom + Align::Left );
			x2 += Max( 800, 2 * w );
		}

		item2 = dasaitem->depitems[i];
		if ( item2->dasa->getStartJD() < endjd )
		{
			showDasaBar( item2->dasa, item2->expert, ycursor );
		}
		for ( j = 0; j < item2->depitems.size(); j++ )
		{
			item3 = item2->depitems[j];
			if (( doc->getJD() < item3->dasa->getEndJD() && endjd > item3->dasa->getStartJD() ))
				showDasaBar( item3->dasa, item3->expert, ycursor + ybar + 2 );
			for ( k = 0; k < item3->depitems.size(); k++ )
			{
				item4 = item3->depitems[k];
				if (( doc->getJD() < item4->dasa->getEndJD() && endjd > item4->dasa->getStartJD() ))
					showDasaBar( item4->dasa, item4->expert, ycursor + 2 * ybar + 3 );
			}
		}
	}
	ycursor += ( adlevel + 2 ) * ybar;
}

/*****************************************************
**
**   DasaGraphicWidget   ---   doPaint
**
******************************************************/
void DasaGraphicWidget::doPaint( bool eraseBackground, const wxRect* rect )
{
	Formatter *formatter = Formatter::get();
	JDate d;
	int month, day;
	wxCoord w, h;
	//wxLongLong starttime = wxGetLocalTimeMillis();

	painter->setGraphicFont();
	assert( painter );
	GetVirtualSize( &w, &h );

	formatter->getDateIntsFromJD( doc->getJD(), day, month, birthyear );
	d.setDate( 1, 1, birthyear, 0 );
	startjd = d.getJD();
	d.setDate( 1, 1, birthyear + totalyears, 0 );
	endjd = d.getJD();

	ycursor = (int)(ybar * 1.5);

	painter->setBrushColor( *wxLIGHT_GREY );
	painter->drawRectangle( 0, 0, w, h );
	painter->setGraphicFont();

	painter->setDefaultBrush();
	painter->setPen( wxPen( config->colors->fgColor ));
	painter->drawRectangle( hborder, vborder, (int)(xright + hborder), (int)(ybottom + vborder) );

	paintRuler();
	for ( unsigned int i = 0; i < dasaitems.size(); i++ ) paintDasa( i );
	//wxLongLong elapsed = wxGetLocalTimeMillis() - starttime;
	//printf( "DasaGraphicWidget::doPaint time %ld millisec\n", elapsed.ToLong());
}

/**************************************************************
***
**   DasaGraphicWidget   ---   GetExportSize
***
***************************************************************/
void DasaGraphicWidget::GetExportSize( int* width, int *height) const
{
	*width = 100 * zoom;
	*height = 500;
}

/**************************************************************
***
**   DasaGraphicWidget   ---   getDasaText
***
***************************************************************/
wxString DasaGraphicWidget::getDasaText( const int &whichdasa, const int &level, const double &theJd )
{
	wxString dasaname, ret;
	wxString mahalord, antarlord, pratyantarlord;
	GraphicalDasaItem *item2, *item3, *item4, *dasaitem = dasaitems[whichdasa];
	unsigned int i, j, k;
	double startJd, endJd;
	wxString startDate, endDate;
	Formatter *formatter = Formatter::get();
	Lang lang;

	DasaExpert *expert = dasaitem->expert;
	for ( i = 0; i < dasaitem->depitems.size(); i++ )
	{
		//ret =  dasaitem->name;
		item2 = dasaitem->depitems[i];
		if ( item2->dasa->getStartJD() < theJd && item2->dasa->getEndJD() > theJd )
		{
			if ( expert->isGrahaDasaExpert() )
			{
				mahalord = lang.getObjectName(item2->dasa->getDasaLord(), TMEDIUM, true );
			}
			else {
				mahalord = lang.getSignName(item2->dasa->getDasaLord(), TMEDIUM);
			}
			startJd = item2->dasa->getStartJD();
			endJd = item2->dasa->getEndJD();
			if ( level > 0 )
			{
				for ( j = 0; j < item2->depitems.size(); j++ )
				{
					item3 = item2->depitems[j];
					if ( item3->dasa->getStartJD() < theJd && item3->dasa->getEndJD() > theJd )
					{
						if ( expert->isGrahaDasaExpert())
						{
							antarlord = lang.getObjectName(item3->dasa->getDasaLord(), TMEDIUM, true );
						}
						else {
							antarlord = lang.getSignName(item3->dasa->getDasaLord(), TMEDIUM);
						}

						startJd = item3->dasa->getStartJD();
						endJd = item3->dasa->getEndJD();
						if ( level > 1 )
						{
							for ( k = 0; k < item3->depitems.size(); k++ )
							{
								item4 = item3->depitems[k];
								if ( item4->dasa->getStartJD() < theJd && item4->dasa->getEndJD() > theJd )
								{
									if ( expert->isGrahaDasaExpert() )
									{
										pratyantarlord = lang.getObjectName(item4->dasa->getDasaLord(), TMEDIUM, true );
									}
									else {
										pratyantarlord = lang.getSignName(item4->dasa->getDasaLord(), TMEDIUM);
									}
									startJd = item4->dasa->getStartJD();
									endJd = item4->dasa->getEndJD();
									break;
								}
							}
							break;
						}
					}
				}
			}
			break;
		}
	}

	//ret += wxT( ": " ) + mahalord;
	ret += mahalord;
	if ( antarlord.Len() > 1 ) ret += wxT( "/" ) + antarlord;
	if ( pratyantarlord.Len() > 1 ) ret += wxT( "/" ) + pratyantarlord;
	startDate = formatter->getDateStringFromJD( startJd );
	endDate = formatter->getDateStringFromJD( endJd );
	ret += wxT( ": " ) + startDate + wxT( " - " ) + endDate;
	//SetToolTip( ret );
	return ret;
}


/**************************************************************
***
**   DasaGraphicWidget   ---   writeTextField
***
***************************************************************/
void DasaGraphicWidget::writeTextField( const bool &b )
{
	if ( theText )
	{
		if ( b ) theText->SetLabel( getTextFieldContents() );
		else theText->SetLabel( wxEmptyString );
	}
}

/**************************************************************
***
**   DasaGraphicWidget   ---   getTextFieldContents
***
***************************************************************/
wxString DasaGraphicWidget::getTextFieldContents()
{
	int x, y;
	double absx, absy, portionx, theJd;
	Formatter *formatter = Formatter::get();
	wxString theDate;
	unsigned int dasa;
	double mycursor;

	CalcUnscrolledPosition( mouse_x, mouse_y, &x, &y );
	absx = x - xleft;
	absy = y - vborder;
	portionx = absx / ( xright - xleft );
	if ( absx < 0 || absy < 0 || portionx > 1 || absy > ybottom )
	{
		return wxEmptyString;
	}
	JDate date;
	date.setDate( 1, 1, birthyear, 0 );
	theJd = date.getJD() + portionx * 36525;
	theDate = formatter->getDateStringFromJD( theJd );
	mycursor = 1.5 * ybar;
	for ( dasa = 0; dasa < dasas.Count(); dasa++ )
	{
		if ( y >= (int)mycursor && y < (int)(mycursor + ybar ))
		{
			return getDasaText( dasa, 0, theJd );
		}
		else if ( adlevel > 0 && y >= (int)mycursor + ybar + 1  && y < (int)(mycursor + 2 * ybar + 2 ))
		{
			return getDasaText( dasa, 1, theJd );
		}
		else if ( adlevel > 1 && y >= (int)mycursor + 2 * ybar + 2  && y < (int)(mycursor + 3 * ybar + 3 ))
		{
			return getDasaText( dasa, 2, theJd );
		}
		mycursor += ( adlevel + 2 ) * ybar;
	}
	return theDate;
}

/**************************************************************
***
**   DasaGraphicWidget   ---   OnIdle
***
***************************************************************/
void DasaGraphicWidget::OnIdle( wxIdleEvent &event )
{
	if ( ! updateOfTextRequired ) return;
	writeTextField( true );
	updateOfTextRequired = false;
}

/**************************************************************
***
**   GraphicalDasaItem   ---   Destructor
***
***************************************************************/
GraphicalDasaItem::~GraphicalDasaItem()
{
	unsigned int i;
	delete dasa;
	dasa = 0;
	for ( i = 0; i < depitems.size(); i++ )
	{
		delete depitems[i];
	}
	depitems.clear();
}

/**************************************************************
***
**   ViewFactory   ---   createDasaGraphicView
***
***************************************************************/
BasicView *ViewFactory::createGraphicalDasaView( wxWindow *parent, ChildWindow *frame, Document *doc )
{
	return new DasaGraphicView( parent, frame, doc );
}


