/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/guibase.cpp
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
#pragma implementation "guibase.h"
#endif

#include "guibase.h"
#include "IconProvider.h"
#include "Painter.h"

#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/dc.h>
#include <wx/dcclient.h>
#include <wx/image.h>
#include <wx/msgdlg.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/statbmp.h>
#include <wx/stattext.h>

#include "Conf.h"

extern Config *config;


/**************************************************************
***
**    mapCommand2ViewId
***
***************************************************************/
int mapCommand2ViewId( const int &command, int &vedic, int &subtype )
{
	switch ( command )
	{
	case CMD_CHILD_NEW_TEXT:
		subtype = config->textWindowPage;
		return VIEW_TEXT;
		break;
	case CMD_CHILD_NEW_TRANSIT:
		return VIEW_TRANSIT;
		break;
	case CMD_CHILD_NEW_SBC:
		return VIEW_SBC;
		break;
	case CMD_CHILD_NEW_SOLAR:
		return VIEW_SOLAR;
		break;
	case CMD_CHILD_NEW_DASA:
		return VIEW_DASA;
		break;
	case CMD_CHILD_NEW_GRAPHICALDASA:
		return VIEW_GRAPHICALDASA;
		break;
	case CMD_CHILD_NEW_URANIAN:
		return VIEW_URANIAN;
		break;
	case CMD_CHILD_NEW_MAINVIEW:
		return VIEW_PLANETLIST;
		break;
	case CMD_CHILD_NEW_YOGA:
		return VIEW_YOGA;
		break;
	case CMD_CHILD_NEW_GRAPHICGRID:
		return VIEW_GRAPHICGRID;
		break;
	case CMD_CHILD_NEW_VARGA:
		return VIEW_VARGA;
		break;
	case CMD_CHILD_NEW_ASHTAKAVARGA:
		return VIEW_ASHTAKAVARGA;
		break;
	case CMD_CHILD_NEW_VIMSOPAKABALA:
		return VIEW_VIMSOPAKABALA;
		break;
	case APP_SHOWHORA:
		return VIEW_HORA;
		break;
	case APP_SHOWECLIPSE:
		return VIEW_ECLIPSE;
		break;
	case APP_NEW_PARTNER:
		return VIEW_PARTNER;
		break;
	case APP_SHOWEPHEM:
		return VIEW_EPHEM;
		break;
	case APP_YOGAEDITOR:
		return VIEW_YOGA_EDITOR;
		break;
	case CMD_CHILD_NEW_WCHART:
		vedic = false;
		subtype = 0;
		return VIEW_GRAPHIC;
		break;
	default:
		if ( command >= CMD_CHILD_NEW_RASI && command <= CMD_CHILD_NEW_RASI+30 )
		{
			vedic = true;
			subtype = command - CMD_CHILD_NEW_RASI;
			return VIEW_GRAPHIC;
		}
		break;
	}
	return 0;
}

/**************************************************************
***
**    doMessageBox
***
***************************************************************/
int doMessageBox( wxWindow *parent, const wxString message, const long style )
{
	return wxMessageDialog( parent, message, GUI_APP_NAME, style | wxCENTRE ).ShowModal();
}

/**************************************************************
***
**    getSizeForViewId
***
***************************************************************/
wxSize getSizeForViewId( const int &viewId, const bool vedic )
{
	wxSize size;
	switch ( viewId )
	{
	case VIEW_PLANETLIST:
		size = wxSize( config->xMainWindow, config->yMainWindow );
		break;
	case VIEW_GRAPHIC:
		if ( vedic )
			size = wxSize( config->xVedicGraphicWindow, config->yVedicGraphicWindow );
		else
			size = wxSize( config->xWesternGraphicWindow, config->yWesternGraphicWindow );
		break;
	case VIEW_TEXT:
		size = wxSize( config->xTextWindow, config->yTextWindow );
		break;
	case VIEW_TRANSIT:
		size = wxSize( config->xTransitWindow, config->yTransitWindow );
		break;
	case VIEW_SBC:
		size = wxSize( config->xSbcWindow, config->ySbcWindow );
		break;
	case VIEW_SOLAR:
		size = wxSize( config->xSolarWindow, config->ySolarWindow );
		break;
	case VIEW_GRAPHICALDASA:
		size = wxSize( config->xGraphicalDasaWindow, config->yGraphicalDasaWindow );
		break;
	case VIEW_ASHTAKAVARGA:
		size = wxSize( config->xAshtakaVargaWindow, config->yAshtakaVargaWindow );
		break;
	case VIEW_DASA:
		size = wxSize( config->xDasaWindow, config->yDasaWindow );
		break;
	case VIEW_URANIAN:
		size = wxSize( config->xUranianWindow, config->yUranianWindow );
		break;
	case VIEW_HORA:
		size = wxSize( config->xHoraWindow, config->yHoraWindow );
		break;
	case VIEW_ECLIPSE:
		size = wxSize( config->xEclipseWindow, config->yEclipseWindow );
		break;
	case VIEW_EPHEM:
		size = wxSize( config->xEphemWindow, config->yEphemWindow );
		break;
	case VIEW_PARTNER:
		size = wxSize( config->xPartnerWindow, config->yPartnerWindow );
		break;
	case VIEW_YOGA:
		size = wxSize( config->xYogaWindow, config->yYogaWindow );
		break;
	case VIEW_VARGA:
		size = wxSize( config->xVargaSummaryWindow, config->yVargaSummaryWindow );
		break;
	case VIEW_YOGA_EDITOR:
		size = wxSize( config->xYogaEditor, config->yYogaEditor );
		break;
	case VIEW_MULTIPLE:
		size = wxSize( config->xMultipleView, config->yMultipleView );
		break;
	case VIEW_EMPTY:
		size = wxSize( 100, 100 );
		break;
	default:
		printf( "Error: wrong view id %d in getSizeForViewId\n", viewId );
		assert( false );
		break;
	}
	return size;
}

/**************************************************************
***
**    getSizeForViewId
***
***************************************************************/
void setConfigSizeForViewId( const int &viewId, const wxSize &size, const bool vedic )
{
	switch ( viewId )
	{
	case VIEW_PLANETLIST:
		config->xMainWindow = size.x;
		config->yMainWindow = size.y;
		break;
	case VIEW_GRAPHIC:
		if ( vedic )
		{
			config->xVedicGraphicWindow = size.x;
			config->yVedicGraphicWindow = size.y;
		}
		else
		{
			config->xWesternGraphicWindow = size.x;
			config->yWesternGraphicWindow = size.y;
		}
		break;
	case VIEW_TEXT:
		config->xTextWindow = size.x;
		config->yTextWindow = size.y;
		break;
	case VIEW_DASA:
		config->xDasaWindow = size.x;
		config->yDasaWindow = size.y;
		break;
	case VIEW_GRAPHICALDASA:
		config->xGraphicalDasaWindow = size.x;
		config->yGraphicalDasaWindow = size.y;
		break;
	case VIEW_TRANSIT:
		config->xTransitWindow = size.x;
		config->yTransitWindow = size.y;
		break;
	case VIEW_SBC:
		config->xSbcWindow = size.x;
		config->ySbcWindow = size.y;
		break;
	case VIEW_SOLAR:
		config->xSolarWindow = size.x;
		config->ySolarWindow = size.y;
		break;
	case VIEW_URANIAN:
		config->xUranianWindow = size.x;
		config->yUranianWindow = size.y;
		break;
	case VIEW_HORA:
		config->xHoraWindow = size.x;
		config->yHoraWindow = size.y;
		break;
	case VIEW_ECLIPSE:
		config->xEclipseWindow = size.x;
		config->yEclipseWindow = size.y;
		break;
	case VIEW_PARTNER:
		config->xPartnerWindow = size.x;
		config->yPartnerWindow = size.y;
		break;
	case VIEW_EPHEM:
		config->xEphemWindow = size.x;
		config->yEphemWindow = size.y;
		break;
	case VIEW_YOGA:
		config->xYogaWindow = size.x;
		config->yYogaWindow = size.y;
		break;
	case VIEW_VARGA:
		config->xVargaSummaryWindow = size.x;
		config->yVargaSummaryWindow = size.y;
		break;
	case VIEW_ASHTAKAVARGA:
		config->xAshtakaVargaWindow = size.x;
		config->yAshtakaVargaWindow = size.y;
		break;
	case VIEW_YOGA_EDITOR:
		config->xYogaEditor = size.x;
		config->yYogaEditor = size.y;
		break;
	case VIEW_MULTIPLE:
		config->xMultipleView = size.x;
		config->yMultipleView = size.y;
		break;
	case VIEW_EMPTY:
		break;
	default:
		printf( "Error: wrong view id %d in setConfigSizeForViewId\n", viewId );
		assert( false );
		break;
	}
}

#define IMAGE_HASH_SIZE 10

/**************************************************************
***
**    createImageHash
***
***************************************************************/
wxString createImageHash( wxImage *image )
{
	wxString s;
	unsigned char *data = image->GetData();

	// 3 chars rgb for each pixel
	const int imagesize = 3 * ( image->GetWidth() * image->GetHeight());

	for( int i = 0; i < IMAGE_HASH_SIZE && i < imagesize; i++ )
	{
		s << *data;
		data++;
	}

	//printf( "size %d s Hash %s\n", imagesize, str2char( s ) );
	return s;
}

/*************************************************//**
*
* 
*
******************************************************/
class DoNotShowAgainDialog : public wxDialog
{
public:
	DoNotShowAgainDialog( wxWindow *parent, wxString message, bool *item )
	:	wxDialog( parent, -1, _( "Information" ), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE ),
	cfgitem( item )
	{
		wxBoxSizer* sizer_main = new wxBoxSizer( wxVERTICAL );
		wxBoxSizer* sizer_lr = new wxBoxSizer( wxHORIZONTAL );
		wxBoxSizer* sizer_text = new wxBoxSizer( wxVERTICAL );
		wxStaticText *messageLabel = new wxStaticText(this, wxID_ANY, message );
		messageLabel->Wrap( 500 );

		wxCheckBox(this, wxID_ANY, _("Use Refraction"));
		check_not_ask_again = new wxCheckBox(this, wxID_ANY, _("Do no show this message again"));
		check_not_ask_again->SetValue( *cfgitem );
		wxButton *button = new wxButton( this, wxID_OK, _( "OK" ));

		wxStaticBitmap *bmp = new wxStaticBitmap( this, -1, IconProvider::getBitmap( BITMAP_MAITREYA ));
		sizer_lr->Add( bmp, 0, wxALL|wxALIGN_CENTER_VERTICAL, 30 );

		sizer_text->Add( messageLabel, 0, wxALL, 5 );
		sizer_text->Add( check_not_ask_again, 0, wxALL, 5 );
		sizer_lr->Add( sizer_text );

		sizer_main->Add( sizer_lr );
		sizer_main->Add( button, 0, wxALL|wxALIGN_CENTER, 3 );
		SetSizer(sizer_main);
		sizer_main->Fit(this);
		Layout();
		button->SetFocus();
		Connect( wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DoNotShowAgainDialog::OnOK ));
	}

private:

	void OnOK( wxCommandEvent &event )
	{
		*cfgitem = check_not_ask_again->IsChecked();
		EndModal( true );
	}

	wxCheckBox *check_not_ask_again;
	bool *cfgitem;

};

/**************************************************************
***
**    doNotShowAgainMessage
***
***************************************************************/
void doNotShowAgainMessage( wxWindow *parent, const int &type )
{
	assert( type >= 0 && type < 4 );
	wxString message;
	bool *cfgitem;
	switch( type )
	{
		case DO_NOT_SHOW_AGAIN_WARNING_DFONTS:
			message = _( "Printout on Mac OS X may be broken if fonts in Data Fork Suitcase Format (*.dfont) are used. Usage of True Type Fonts is recommended." );
			cfgitem = &config->doNotShowAgainWarningDFonts;
		break;
		case DO_NOT_SHOW_AGAIN_WARNING_EPHEM_FILES:
			message = _( "Some data could not be calculated. Please look if you configured Swiss Ephemeris data files in Configuration/Ephemeris correctly.");
			cfgitem = &config->doNotShowAgainWarningEphemFiles;
		break;
		case DO_NOT_SHOW_AGAIN_WARNING_AFTER_LANGUAGE_CHANGE:
		    message = _("The language will be changed when you restart the program.");
			cfgitem = &config->doNotShowAgainAfterLanguageChange;
		break;
		case DO_NOT_SHOW_AGAIN_WARNING_EPHEM_CONFIG_CHANGE:
			message = _( "Warning: changes in calculation configuration may result in wrong calculation results!");
			cfgitem = &config->doNotShowAgainEphemConfigChange;
		break;
		default:
			printf( "Error in doNotShowAgainMessage, unknown type %d\n", type );
			return;
		break;
	}
	if ( ! cfgitem || *cfgitem ) return;

	DoNotShowAgainDialog dialog( parent, message, cfgitem );
	dialog.ShowModal();
}


