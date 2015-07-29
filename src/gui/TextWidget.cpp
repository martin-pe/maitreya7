/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/TextWidget.cpp
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
#pragma implementation "TextWidget.h"
#endif

#include "TextWidget.h"

#include <wx/clipbrd.h>
#include <wx/dataobj.h>
#include <wx/filedlg.h>
#include <wx/filename.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>

#include "astrobase.h"
#include "Conf.h"
#include "FontConfig.h"
#include "guibase.h"
#include "Horoscope.h"
#include "BasicView.h"
#include "PlanetList.h"
#include "Writer.h"

extern Config *config;

IMPLEMENT_CLASS( TextWidget, wxHtmlWindow )

/*****************************************************
**
**   TextWidget   ---   Constructor
**
******************************************************/
TextWidget::TextWidget( wxWindow *parent, BasicView *v, Config *cfg )
		: wxHtmlWindow( parent, -1, wxPoint( 0, 0 ))
{
	mview = v;

	setupFonts();
	writer = WriterFactory().getWriter( WRITER_HTML, ( cfg ? cfg : config ));

	Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( TextWidget::OnKeyDown ));
	//Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( TextWidget::OnMouseWheelEvent ));
	Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( TextWidget::OnMouseRightDown ));
}

/*****************************************************
**
**   TextWidget   ---   Destructor
**
******************************************************/
TextWidget::~TextWidget()
{
	delete writer;
}

/*****************************************************
**
**   TextWidget   ---   setupFonts
**
******************************************************/
void TextWidget::setupFonts()
{
	FontConfig *f = FontConfig::get();
	wxFont *defaultFont = f->getDefaultFont();
	wxFont *textFont = f->getTextFont();
	const int ss = defaultFont->GetPointSize();
	const int sizes[7] = { ss-4, ss-2, ss, ss+1, ss+2, ss+3, ss+5};
	SetFonts( defaultFont->GetFaceName(), textFont->GetFaceName(), sizes );
}

/*****************************************************
**
**   TextWidget   ---   writeContents
**
******************************************************/
int TextWidget::writeContents()
{
	int ret = 0;

	setupFonts();
	writer->beginWriting();
	ret = write();
	writer->endWriting();
	setContents( writer->getContents());
	return ret;
}

/*****************************************************
**
**   TextWidget   ---   doExport
**
******************************************************/
void TextWidget::doExport()
{
	const static wxChar *filetypes = _( "Text Files (*.txt)|*.txt|Html Files (*.html)|*.html|CSV Files (*.csv)|*.csv|All Files|*.*" );
	wxString filename;

	switch ( config->defTextExportType )
	{
	case 1:
		filename = wxT( "out.txt"  );
		break;
	case 2:
		filename = wxT( "out.csv"  );
		break;
	default:
		filename = wxT( "out.html"  );
		break;
	}

	int style = wxFD_SAVE;
	if ( config->exportAskOnOverwrite ) style |= wxFD_OVERWRITE_PROMPT;

	wxFileDialog dialog( this, _("Save Text" ), config->defExportPath, filename, filetypes, style, wxDefaultPosition );
	dialog.SetFilterIndex( config->defTextExportType );

	if ( dialog.ShowModal() == wxID_OK )
	{
		filename = dialog.GetDirectory() + wxFileName::GetPathSeparator() + dialog.GetFilename();

		Writer *oldwriter = writer;

		int writertype = WRITER_HTML;
		if ( filename.Right( 4 ).MakeLower() == wxT( ".txt" ))
		{
			writertype = WRITER_TEXT;
			config->defTextExportType = 0;
		}
		else if ( filename.Right( 4 ).MakeLower() == wxT( ".csv" ))
		{
			writertype = WRITER_CSV;
			config->defTextExportType = 2;
		}
		else
		{
			writertype = WRITER_HTML;
			config->defTextExportType = 1;
		}
		writer = WriterFactory().getWriter( writertype );

		wxFile file;
		file.Create( filename, true );
		writeContents();
		file.Write( contents );
		file.Close();

		delete writer;
		writer = oldwriter;
		writeContents();

		config->defExportPath = dialog.GetDirectory();

		doMessageBox( this, wxString::Format( _("Text exported to %s" ), (const wxChar*)filename));
	}
}

/*****************************************************
**
**   TextWidget   ---   setContents
**
******************************************************/
void TextWidget::setContents( const wxString &s )
{
	contents = s;
	SetPage( s );
}

/*****************************************************
**
**   TextWidget   ---   getContents
**
******************************************************/
wxString TextWidget::getContents()
{
	return contents;
}

/*****************************************************
**
**   TextWidget   ---   scroll
**
******************************************************/
void TextWidget::scroll( const int &delta )
{
	int x, y, newy;
	const int offset = 20;

	GetViewStart( &x, &y );
	newy = Max( y - delta / offset, 0 );
	Scroll( x, newy );
}

/*****************************************************
**
**   TextWidget   ---   HandleMouseWheelEvent
**
******************************************************/
void TextWidget::HandleMouseWheelEvent( wxMouseEvent &event )
{
	//printf( "HANDLE WHEEL in TextWidget::onMouseWheelEvent %d - %d DELTA %d rot %d\n", event.GetX(), event.GetY(), event.GetWheelDelta(), event.GetWheelRotation() );
	scroll( event.GetWheelRotation());
}

/**************************************************************
***
**   TextWidget   ---   OnMouseRightDown
***
***************************************************************/
void TextWidget::OnMouseRightDown( wxMouseEvent &event )
{
	//printf( "RIGHT in TextWidget::OnMouseRightDown %d - %d\n", event.GetX(), event.GetY() );
	event.ResumePropagation( wxEVENT_PROPAGATE_MAX );
	event.Skip();
}

/*****************************************************
**
**   TextWidget   ---   OnMouseWheelEvent
**
******************************************************/
void TextWidget::OnMouseWheelEvent( wxMouseEvent &event )
{
	//printf( "WHEEL in TextWidget::onMouseWheelEvent %d - %d\n", event.GetX(), event.GetY() );
	HandleMouseWheelEvent( event );
}

/*****************************************************
**
**   TextWidget   ---   OnKeyDown
**
******************************************************/
void TextWidget::OnKeyDown( wxKeyEvent &event )
{
	//printf( "KEY DOWN in TextWidget::OnKeyDown\n" );
	event.ResumePropagation( wxEVENT_PROPAGATE_MAX );
	event.Skip();
}

