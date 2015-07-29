/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/TextWidget.h
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


#ifndef _TEXTWIDGET_H_
#define _TEXTWIDGET_H_

#ifdef __GNUG__
#pragma interface "TextWidget.h"
#endif

#include <wx/html/htmlwin.h>

class Config;
class Horoscope;
class BasicView;
class wxHtmlWindow;
class wxTextCtrl;
class wxWindow;
class Writer;

/*************************************************//**
*
*  basic widget holding a HTML text view item
*
******************************************************/
class TextWidget : public wxHtmlWindow
{
	DECLARE_CLASS( TextWidget )
public:
	TextWidget( wxWindow *parent, BasicView *v, Config* = 0 );

	~TextWidget();

	virtual int writeContents();

	void doExport();
	void setContents( const wxString& );
	wxString getContents();
	Writer *getWriter() {
		return writer;
	}
	BasicView *getBasicView() {
		return mview;
	}
	void scroll( const int &delta );

protected:

	virtual int write() = 0;

	void setupFonts();

	// Event Handlers
	void HandleMouseWheelEvent( wxMouseEvent& );
	void OnMouseRightDown( wxMouseEvent& );
	void OnKeyDown( wxKeyEvent& );
	void OnMouseWheelEvent( wxMouseEvent& );

	Writer *writer;

	wxString contents;
	BasicView *mview;
};

#endif

