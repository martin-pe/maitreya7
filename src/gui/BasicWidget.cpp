/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/BasicWidget.cpp
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
#pragma implementation "BasicWidget.h"
#endif

#include "BasicWidget.h"

#include <wx/dc.h>
#include <wx/dcclient.h>
#include <wx/dcmemory.h>
#include <wx/filedlg.h>
#include <wx/filename.h>

#include "BasicView.h"
#include "Conf.h"
#include "ExportDialog.h"
#include "guibase.h"
#include "Painter.h"

extern Config *config;

IMPLEMENT_CLASS( BasicWidget, wxScrolledWindow )

/*****************************************************
**
**   BasicWidget   ---   Constructor
**
******************************************************/
BasicWidget::BasicWidget( wxWindow *parent, ChartProperties *chartprops, wxWindowID id, const wxPoint& pos, const wxSize& size )
		: wxScrolledWindow( parent, id, pos, size ),
		chartprops(  chartprops )
{
	hborder = 10;
	vborder = 10;
	exportMode = false;
	dragMode = false;
	painter = 0;
	scrollx = scrolly = 1;
	SetScrollRate( scrollx, scrolly );

	Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( BasicWidget::OnMouseWheelEvent ));
	Connect( wxEVT_MOTION, wxMouseEventHandler( BasicWidget::OnMouseMotion ));
	Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( BasicWidget::OnMouseRightDown ));
	Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( BasicWidget::OnMouseLeftDown ));
	Connect( wxEVT_LEFT_UP, wxMouseEventHandler( BasicWidget::OnMouseLeftUp ));
	Connect( wxEVT_LEAVE_WINDOW, wxMouseEventHandler( BasicWidget::OnMouseLeave ));
	Connect( wxEVT_PAINT, wxPaintEventHandler( BasicWidget::OnPaint ));
	Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( BasicWidget::OnKeyDown ));
}

/*****************************************************
**
**   BasicWidget   ---   Destructor
**
******************************************************/
BasicWidget::~BasicWidget()
{
}

/*****************************************************
**
**   BasicWidget   ---   OnDataChanged
**
******************************************************/
void BasicWidget::OnDataChanged()
{
	Refresh();
}

/*****************************************************
**
**   BasicWidget   ---   OnPaint
**
******************************************************/
void BasicWidget::OnPaint( wxPaintEvent &event )
{
	int vx, vy;

	wxPaintDC context( this );
	PrepareDC( context );

	wxRegionIterator upd( GetUpdateRegion());
	if ( upd ) refreshRect = upd.GetRect();
	while ( ++upd )
	{
		refreshRect.Union( upd.GetRect() );
	}
	GetViewStart( &vx, &vy );
	if ( vx || vy )
	{
		refreshRect.x += vx;
		refreshRect.y += vy;
	}

	assert( painter == 0 );
	painter = new DcPainter( &context );
	doPaint();
	delete painter;
	painter = 0;
}

/*****************************************************
**
**   BasicWidget   ---   OnSize
**
******************************************************/
void BasicWidget::OnSize( wxSizeEvent &event )
{
	//printf( "BasicWidget::OnSize x %d y %d\n", event.GetSize().x, event.GetSize().y );
	SetSize( event.GetSize().x, event.GetSize().y );
	//Refresh();
}

/*****************************************************
**
**   BasicWidget   ---   drawTextFormatted
**
******************************************************/
void BasicWidget::drawTextFormatted( const wxRect &rect, const wxString t, const int& align )
{
	assert( painter );
	painter->drawTextFormatted( rect.x, rect.y, rect.width, rect.height, t, align );
}

/*****************************************************
**
**   BasicWidget   ---   drawSimpleText
**
******************************************************/
void BasicWidget::drawSimpleText( const wxRect &r, const wxString &s )
{
	drawTextFormatted( r, s, Align::Center );
}

/*****************************************************
**
**   BasicWidget   ---   doExport
**
******************************************************/
void BasicWidget::doExport()
{
	int exporttype;
	wxString filename, e;

	GetExportSize( &xsize, &ysize );

	if ( config->showGraphicSizeDialog )
	{
		ExportDialog dialog( this, xsize, ysize );
		if ( dialog.ShowModal() != wxID_OK ) return;
		dialog.getSizes( xsize, ysize );
		config->showGraphicSizeDialog = ! dialog.getCheckShowDialog();
	}

	const static wxString filetypes =
	    wxT( "PNG (*.png)|*.png|JPG (*.jpg)|*.jpg|Bitmap (*.bmp)|*.bmp|PCX (*pcx)|*pcx|PNM (*.pnm)|*.pnm|TIFF (*.tif)|*.tif|All files (*)| *.*" );

	switch ( config->defGraphicExportType )
	{
	case 1:
		filename = wxT( "out.jpg"  );
		break;
	case 2:
		filename = wxT( "out.bmp"  );
		break;
	case 3:
		filename = wxT( "out.pcx"  );
		break;
	case 4:
		filename = wxT( "out.pnm"  );
		break;
	case 5:
		filename = wxT( "out.tif"  );
		break;
	default:
		filename = wxT( "out.png"  );
		break;
	}
	int style = wxFD_SAVE;
	if ( config->exportAskOnOverwrite ) style |= wxFD_OVERWRITE_PROMPT;

	wxFileDialog exportFileDialog( this, _("Export Picture" ), config->defExportPath, filename,
	                               filetypes, style, wxDefaultPosition );
	exportFileDialog.SetFilterIndex( config->defGraphicExportType );

	if ( exportFileDialog.ShowModal() == wxID_OK )
	{
		filename = exportFileDialog.GetDirectory() + wxFileName::GetPathSeparator() + exportFileDialog.GetFilename();
		e = exportFileDialog.GetFilename().Right(3).MakeLower();
		if ( e == wxT( "png" ))
		{
			exporttype = wxBITMAP_TYPE_PNG;
			config->defGraphicExportType = 0;
		}
		else if ( e == wxT( "jpg" ))
		{
			exporttype = wxBITMAP_TYPE_JPEG;
			config->defGraphicExportType = 0;
		}
		else if ( e == wxT( "jpeg" ))
		{
			exporttype = wxBITMAP_TYPE_JPEG;
			config->defGraphicExportType = 0;
		}
		else if ( e == wxT( "bmp" ))
		{
			exporttype = wxBITMAP_TYPE_BMP;
			config->defGraphicExportType = 0;
		}
		else if ( e == wxT( "pnm" ))
		{
			exporttype = wxBITMAP_TYPE_PNM;
			config->defGraphicExportType = 0;
		}
		else if ( e == wxT( "pcx" ))
		{
			exporttype = wxBITMAP_TYPE_PCX;
			config->defGraphicExportType = 0;
		}
		else if ( e == wxT( "tif" ))
		{
			exporttype = wxBITMAP_TYPE_TIF;
			config->defGraphicExportType = 0;
		}
		else
		{
			doMessageBox( this, wxString::Format( _( "Can't determine image handler for extension \"%s\", using default (PNG)" ), (const wxChar*)e));
			filename.Append( wxT ( ".png" ));
			exporttype = wxBITMAP_TYPE_PNG;
			config->defGraphicExportType = 0;
		}
		config->defExportPath = exportFileDialog.GetDirectory();
		wxBitmap bitmap( xsize, ysize );
		wxMemoryDC dc;
		dc.SelectObject( bitmap );
		painter = new DcPainter( &dc );
		exportMode = true;
		doPaint();
		exportMode = false;
		delete painter;
		painter = 0;
		if ( bitmap.SaveFile( filename, (wxBitmapType)exporttype ))
		{
			doMessageBox( this, wxString::Format( _("Picture exported to %s"), (const wxChar*)filename));
		}
	}
}

/**************************************************************
***
**   BasicWidget   ---   getWidgetSize
***
***************************************************************/
void BasicWidget::getWidgetSize( int* width, int* height )
{
	if ( exportMode )
	{
		*width = xsize;
		*height = ysize;
	}
	else wxScrolledWindow::GetSize( width,  height );
}

/*****************************************************
**
**   BasicWidget   ---   OnKeyDown
**
******************************************************/
void BasicWidget::OnKeyDown( wxKeyEvent &event )
{
	//printf( "KEY DOWN in BasicWidget::OnKeyDown\n" );
	event.ResumePropagation( wxEVENT_PROPAGATE_MAX );
	event.Skip();
}

/*****************************************************
**
**   BasicWidget   ---   OnChar
**
******************************************************/
void BasicWidget::OnChar( wxKeyEvent &event )
{
	//printf( "CHAR in BasicWidget::OnChar\n" );
	event.ResumePropagation( wxEVENT_PROPAGATE_MAX );
	event.Skip();
}

/*****************************************************
**
**   BasicWidget   ---   OnMouseWheelEvent
**
******************************************************/
void BasicWidget::OnMouseWheelEvent( wxMouseEvent &event )
{
	//printf( "WHEEL in BasicWidget::onMouseWheelEvent %d - %d\n", event.GetX(), event.GetY() );
	HandleMouseWheelEvent( event );
}

/*****************************************************
**
**   BasicWidget   ---   HandleMouseWheelEvent
**
******************************************************/
void BasicWidget::HandleMouseWheelEvent( wxMouseEvent &event )
{
	//printf( "HANDLE WHEEL in BasicWidget::onMouseWheelEvent %d - %d\n", event.GetX(), event.GetY() );

#if defined( __WXMAC__ )
	bool shiftpressed = wxGetKeyState( WXK_SHIFT );
	if ( shiftpressed )
	{
		chartprops->changeSkin( event.GetWheelRotation() > 0 );
	}
#else
	chartprops->changeSkin( event.GetWheelRotation() > 0 );
#endif
	Refresh();
}

/**************************************************************
***
**   BasicWidget   ---   OnMouseMotion
***
***************************************************************/
void BasicWidget::OnMouseMotion( wxMouseEvent &event )
{
	int dx, dy, vx, vy;
	if ( dragMode )
	{
		dx = event.m_x - mousex;
		dy = event.m_y - mousey;
		GetViewStart( &vx, &vy );
		Scroll( vx - dx / scrollx, vy - dy / scrolly );
	}
	mousex = event.m_x;
	mousey = event.m_y;
}

/**************************************************************
***
**   BasicWidget   ---   OnMouseLeftDown
***
***************************************************************/
void BasicWidget::OnMouseLeftDown( wxMouseEvent& )
{
	SetFocus();
	dragMode = true;
}

/**************************************************************
***
**   BasicWidget   ---   OnMouseRightDown
***
***************************************************************/
void BasicWidget::OnMouseRightDown( wxMouseEvent &event )
{
	// Obsolet, wenn Propagator vorhanden
	//printf( "RIGHT in OnMouseRightDown %d - %d\n", event.GetX(), event.GetY() );
	event.ResumePropagation( wxEVENT_PROPAGATE_MAX );
	event.Skip();
}

/**************************************************************
***
**   BasicWidget   ---   OnMouseLeftUp
***
***************************************************************/
void BasicWidget::OnMouseLeftUp( wxMouseEvent& )
{
	dragMode = false;
}

/**************************************************************
***
**   BasicWidget   ---   OnMouseLeave
***
***************************************************************/
void BasicWidget::OnMouseLeave( wxMouseEvent& )
{
	dragMode = false;
}

/**************************************************************
***
**   BasicWidget   ---   echo
***
***************************************************************/
void BasicWidget::echo( wxString  s )
{
	// otto
}

/**************************************************************
***
**   EmptyWidget   ---   Constructor
***
***************************************************************/
EmptyWidget::EmptyWidget( wxWindow *parent, ChartProperties *props )
		: BasicWidget( parent, chartprops )
{
}

/**************************************************************
***
**   EmptyWidget   ---   Constructor
***
***************************************************************/
void EmptyWidget::doPaint( bool eraseBackground, const wxRect* )
{
	int w, h;
	getWidgetSize( &w, &h );
	painter->setPen( *wxRED );
	painter->drawLine( 0, 0, w, h );
	painter->drawLine( 0, h, w, 0 );
}
