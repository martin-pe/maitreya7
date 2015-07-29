/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/TextView.cpp
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
#pragma implementation "TextView.h"
#endif

#include "TextView.h"

#include <wx/choice.h>
#include <wx/spinctrl.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/toolbar.h>

#include "Conf.h"
#include "constants.h"
#include "Document.h"
#include "TextHelper.h"
#include "TextWidget.h"
#include "Writer.h"

extern Config *config;

IMPLEMENT_CLASS( TextViewBase, BasicView )
IMPLEMENT_CLASS( PlanetListView, TextViewBase )
IMPLEMENT_CLASS( DefaultTextView, TextViewBase )

/*****************************************************
**
**   TextViewBase   ---   Constructor
**
******************************************************/
TextViewBase::TextViewBase( wxWindow *parent, ChildWindow *frame, Document *doc, const int &viewId, const bool &toolpanel, const bool &vedic )
		: BasicView( parent, frame, doc, viewId, toolpanel, vedic )
{
	supportTextExport = true;
}

/*****************************************************
**
**   TextViewBase   ---   OnDataChanged
**
******************************************************/
void TextViewBase::OnDataChanged()
{
	BasicView::OnDataChanged();
	write();
}

/*****************************************************
**
**   TextViewBase   ---   dispatchCommand
**
******************************************************/
bool TextViewBase::dispatchCommand( const int &command )
{
	switch ( command )
	{
	case CMD_CHILD_SAVETEXT:
		theText->doExport();
		break;
	default:
		return BasicView::dispatchCommand( command );
		break;
	}
	return true;
}
/*************************************************//**
*
*
*
******************************************************/
class DefaultViewTextWidget : public TextWidget
{
	DECLARE_CLASS( DefaultViewTextWidget )

public:
	DefaultViewTextWidget( wxWindow *parent, BasicView *v, 	Horoscope *horoscope )
			: TextWidget( parent, v )
	{
		this->horoscope = horoscope;
		mode = varga = dasa = 0;
	}

	virtual int write()
	{
		return TextHelper( horoscope, mview, writer ).writeTextAnalysis( mode, varga, dasa );
	}

	int mode, varga, dasa;

private:
	Horoscope *horoscope;
};

IMPLEMENT_CLASS( DefaultViewTextWidget, TextWidget )

/*****************************************************
**
**   DefaultTextView   ---   Constructor
**
******************************************************/
DefaultTextView::DefaultTextView( wxWindow *parent, ChildWindow *frame, Document *doc, const bool &toolpanel, const bool &vedic, const int &mode )
		: TextViewBase( parent, frame, doc, VIEW_TEXT, toolpanel, vedic )
{
	this->mode = mode;
	varga = dasa = 0;

	theText = new DefaultViewTextWidget( this, this, doc );
	widget = theText;

	//mainView = ( mode == TEXT_MODE_MAIN_VIEW );
	initToolItems();
	OnToolbarCommand();
}

/**************************************************************
***
**   DefaultTextView   ---   initToolItems
***
***************************************************************/
void DefaultTextView::initToolItems()
{
	if ( toolbar )
	{
		wxChoice *choice_viewmode = (wxChoice*)toolbar->FindControl( TBS_TEXTMODE );
		if ( choice_viewmode ) choice_viewmode->SetSelection( mode );

		wxChoice *choice_varga = (wxChoice*)toolbar->FindControl( TBS_VARGA );
		if ( choice_varga )
		{
			choice_varga->SetSelection( varga );
			ADJUST_TOOLBAR_ITEM( choice_varga, mode == TEXT_MODE_ASHTAKAVARGA || mode == TEXT_MODE_JAIMINI );
			choice_varga->Show( mode == TEXT_MODE_ASHTAKAVARGA || mode == TEXT_MODE_JAIMINI );
		}

		wxChoice *choice_dasa = (wxChoice*)toolbar->FindControl( TBS_DASA );
		if ( choice_dasa )
		{
			choice_dasa->SetSelection( dasa );
			ADJUST_TOOLBAR_ITEM( choice_dasa, mode == TEXT_MODE_DASA || mode == TEXT_MODE_DASALONG );
		}
	}
}

/**************************************************************
***
**   DefaultTextView   ---   write
***
***************************************************************/
void DefaultTextView::write()
{
	DefaultViewTextWidget *w = (DefaultViewTextWidget *)theText;
	w->mode = mode;
	w->dasa = dasa;
	w->varga = varga;
	int ret = theText->writeContents();
	if ( ret )
	{
		wxCommandEvent event( SHOW_EPHEM_FILE_WARNING );
		event.SetEventObject( this );
		wxPostEvent( GetParent(), event );
	}
}

/**************************************************************
***
**   DefaultTextView   ---   supportsEWToggle
***
***************************************************************/
bool DefaultTextView::supportsEWToggle() const
{
	return mode == TEXT_MODE_ARABICPARTS;
}

/**************************************************************
***
**   DefaultTextView   ---   getWindowLabel
***
***************************************************************/
wxString DefaultTextView::getWindowLabel( const bool shortname )
{
	return shortname ? _( "Text" ) : _( "Text View" );
}

/*****************************************************
**
**   DefaultTextView   ---   OnToolbarCommand
**
******************************************************/
void DefaultTextView::OnToolbarCommand()
{
	if ( toolbar )
	{
		wxChoice *choice_viewmode = (wxChoice*)toolbar->FindControl( TBS_TEXTMODE );
		if ( choice_viewmode )
		{
			mode = choice_viewmode->GetSelection();
			config->textWindowPage = mode;
		}

		wxChoice *choice_varga = (wxChoice*)toolbar->FindControl( TBS_VARGA );
		if ( choice_varga )
		{
			varga = choice_varga->GetSelection();
			ADJUST_TOOLBAR_ITEM( choice_varga, mode == TEXT_MODE_ASHTAKAVARGA || mode == TEXT_MODE_JAIMINI );
		}

		wxChoice *choice_dasa = (wxChoice*)toolbar->FindControl( TBS_DASA );
		if ( choice_dasa )
		{
			dasa = choice_dasa->GetSelection();
			ADJUST_TOOLBAR_ITEM( choice_dasa, mode == TEXT_MODE_DASA || mode == TEXT_MODE_DASACOMPACT || mode == TEXT_MODE_DASALONG || mode == TEXT_MODE_KP );
			choice_dasa->Show( mode == TEXT_MODE_DASA || mode == TEXT_MODE_DASACOMPACT || mode == TEXT_MODE_DASALONG || mode == TEXT_MODE_KP );
		}
	}
	OnDataChanged();
}

/**************************************************************
***
**   ViewFactory   ---   createDefaultTextView
***
***************************************************************/
BasicView *ViewFactory::createDefaultTextView( wxWindow *parent, ChildWindow *frame, Document *doc, const bool &toolpanel, const bool &vedic, const int &mode )
{
	return new DefaultTextView( parent, frame, doc, toolpanel, vedic, mode );
}

/*************************************************//**
*
*
*
******************************************************/
class PlanetListViewTextWidget : public TextWidget
{
	DECLARE_CLASS( PlanetListViewTextWidget )

public:
	PlanetListViewTextWidget( wxWindow *parent, BasicView *v, 	Horoscope *horoscope )
			: TextWidget( parent, v )
	{
		this->horoscope = horoscope;
	}

	virtual int write()
	{
		TextHelper th( horoscope, mview, writer );
		return th.writePlanets();
	}

private:
	Horoscope *horoscope;
};

IMPLEMENT_CLASS( PlanetListViewTextWidget, TextWidget )

/**************************************************************
***
**   PlanetListView   ---   Constructor
***
***************************************************************/
PlanetListView::PlanetListView( wxWindow *parent, ChildWindow *frame, Document *doc, const bool &vedic )
		: TextViewBase( parent, frame, doc, VIEW_PLANETLIST, false, vedic )
{
	supportEWToggle = supportObjectToggle = true;
	mainView = true;

	theText = new PlanetListViewTextWidget( this, this, doc );
	widget = theText;

	OnDataChanged();
}

/**************************************************************
***
**   PlanetListView   ---   Destructor
***
***************************************************************/
PlanetListView::~PlanetListView()
{
	//delete doc;
}

/**************************************************************
***
**   PlanetListView   ---   getWindowLabel
***
***************************************************************/
wxString PlanetListView::getWindowLabel( const bool shortname )
{
	return shortname ? wxT( "" ) : _( "Main Window" );
}

/**************************************************************
***
**   PlanetListView   ---   write
***
***************************************************************/
void PlanetListView::write()
{
	int ret = theText->writeContents();
	if ( ret )
	{
		wxCommandEvent event( SHOW_EPHEM_FILE_WARNING );
		event.SetEventObject( this );
		wxPostEvent( GetParent(), event );
	}
}

/**************************************************************
***
**   ViewFactory   ---   createMainView
***
***************************************************************/
BasicView *ViewFactory::createMainView( wxWindow *parent, ChildWindow *frame, Document *doc, const bool &vedic )
{
	return new PlanetListView( parent, frame, doc, vedic );
	//return new DefaultTextView( parent, frame, doc, false, vedic, TEXT_MODE_MAIN_VIEW );
}



