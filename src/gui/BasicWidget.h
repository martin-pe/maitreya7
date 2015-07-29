/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/BasicWidget.h
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


#ifndef _BASICWIDGET_H_
#define _BASICWIDGET_H_

#ifdef __GNUG__
#pragma interface "BasicWidget.h"
#endif

#include <wx/scrolwin.h>

class ChartProperties;
class Painter;
class BasicView;
class Writer;
class wxDC;
class wxWindow;

/*************************************************//**
*
* \brief Basic abstract class for graphical display based upon wxScrolledWindow
*
******************************************************/
class BasicWidget :  public wxScrolledWindow
{
	DECLARE_CLASS( BasicWidget )

public:
	BasicWidget( wxWindow *parent, ChartProperties*, wxWindowID id = -1, const wxPoint& pos = wxDefaultPosition,
	             const wxSize& size = wxDefaultSize );
	~BasicWidget();

	virtual void OnDataChanged();

	void doExport();
	virtual void getWidgetSize( int* width, int* height );

	virtual void HandleMouseWheelEvent( wxMouseEvent &event );

protected:

	ChartProperties *chartprops;
	Painter *painter;
	int hborder, vborder;

	void OnPaint( wxPaintEvent& );
	virtual void doPaint( bool eraseBackground = true, const wxRect* rect = 0 ) = 0;

	// Mouse
	void OnMouseWheelEvent( wxMouseEvent &event );
	void OnMouseRightDown( wxMouseEvent& );
	void OnMouseMotion( wxMouseEvent& );
	void OnMouseLeftDown( wxMouseEvent& );
	void OnMouseLeftUp( wxMouseEvent& );
	void OnMouseLeave( wxMouseEvent& );

	void OnKeyDown( wxKeyEvent& );
	void OnChar( wxKeyEvent &event );
	void OnSize( wxSizeEvent& );

	void drawTextFormatted( const wxRect &rect, const wxString t, const int& align );
	void drawSimpleText( const wxRect &r, const wxString &s );
	bool dragMode, exportMode;
	int scrollx, scrolly;
	int mousex, mousey;
	int xsize, ysize;
	wxRect refreshRect;
	virtual void GetExportSize( int* width, int *height) const  {
		wxScrolledWindow::GetSize( width,  height);
	}

	void echo( wxString );
};

/*************************************************//**
*
* \brief Empty widget (used on error)
*
******************************************************/
class EmptyWidget : public BasicWidget
{
public:
	EmptyWidget( wxWindow *parent, ChartProperties* );

protected:

	virtual void doPaint( bool eraseBackground = true, const wxRect* rect = 0 );
};

#endif

