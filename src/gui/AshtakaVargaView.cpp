/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/AshtakaVargaView.cpp
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
#pragma implementation "AshtakaVargaView.h"
#endif

#include "AshtakaVargaView.h"

#include <wx/choice.h>
#include <wx/toolbar.h>

#include "Ashtakavarga.h"
#include "Conf.h"
#include "Lang.h"
#include "Varga.h"
#include "VargaHoroscope.h"
#include "Writer.h"

#include "Document.h"
#include "GraphicGrid.h"
#include "NotebookWidget.h"
#include "Painter.h"
#include "PlanetList.h"
#include "TextWidget.h"
#include "VedicChart.h"

extern Config *config;

#define MAX_ASHTAKAVARGA_PAGES 6

enum { ANB_REKHA, ANB_TRIKONA, ANB_EKADHIPATYA, ANB_RASIDIAGRAM, ANB_PINDADIAGRAM, ANB_TEXT };

IMPLEMENT_CLASS( AshtakaVargaView, BasicNotebookView )

/*************************************************//**
*
* \brief Widget containing bar diagrams for Ashtakavarga Rasi evaluation
*
******************************************************/
class AshtakaVargaRasiDiagram : public DiagramWidget
{
	DECLARE_CLASS( AshtakaVargaRasiDiagram )
public:

	/*****************************************************
	**
	**   AshtakaVargaRasiDiagram   ---   Constructor
	**
	******************************************************/
	AshtakaVargaRasiDiagram( wxWindow *parent, BasicView *v, const AshtakavargaExpert *e )
			: DiagramWidget( parent, v, 2, 2 ),
			expert( e )
	{
		planetmode = false;
	}

protected:

	/*****************************************************
	**
	**   AshtakaVargaRasiDiagram   ---   paintDiagrams
	**
	******************************************************/
	virtual void paintDiagrams()
	{
		unsigned int i;

		for ( i = ARIES; i <= PISCES; i++ ) info.addItem( DiagramItem( i, expert->getSarva( REKHA, i )));
		paintDiagram( 0, 0, _( "Rekha" ));
		info.clear();

		for ( i = ARIES; i <= PISCES; i++ ) info.addItem( DiagramItem( i, expert->getSarva( TRIKONA, i )));
		paintDiagram( 1, 0, _( "Trikona Shodana" ));
		info.clear();

		for ( i = ARIES; i <= PISCES; i++ ) info.addItem( DiagramItem( i, expert->getSarva( EKADHI, i )));
		paintDiagram( 0, 1, _( "Ekadhipatya Shodana" ));
		info.clear();
	}

	const AshtakavargaExpert *expert;
};

IMPLEMENT_CLASS( AshtakaVargaRasiDiagram, DiagramWidget )

/*************************************************//**
*
* \brief Widget containing bar diagrams for Ashtakavarga Pinda evaluation
*
******************************************************/
class AshtakaVargaPindaDiagram : public DiagramWidget
{
	DECLARE_CLASS( AshtakaVargaPindaDiagram )
public:

	/*****************************************************
	**
	**   AshtakaVargaPindaDiagram   ---   Constructor
	**
	******************************************************/
	AshtakaVargaPindaDiagram( wxWindow *parent, BasicView *v, const AshtakavargaExpert *e )
			: DiagramWidget( parent, v, 2, 2 ),
			expert( e )
	{
		planetmode = true;
	}

protected:

	/*****************************************************
	**
	**   AshtakaVargaPindaDiagram   ---   paintDiagrams
	**
	******************************************************/
	virtual void paintDiagrams()
	{
		unsigned int i;

		vector<int> obs = PlanetList().getVedicPlanetOnlyList();
		obs.push_back( OASCENDANT );

		for ( i = 0; i < obs.size(); i++ ) info.addItem( DiagramItem( obs[i], expert->getPinda( GRAHAPINDA, i )));
		paintDiagram( 0, 0, _( "Graha Pinda" ));
		info.clear();

		for ( i = 0; i < obs.size(); i++ ) info.items.push_back( DiagramItem( obs[i], expert->getPinda( RASIPINDA, i )));
		paintDiagram( 1, 0, _( "Rasi Pinda" ));
		info.clear();

		for ( i = 0; i < obs.size(); i++ ) info.items.push_back( DiagramItem( obs[i], expert->getPinda( YOGAPINDA, i )));
		paintDiagram( 0, 1, _( "Sodhya Pinda" ));
		info.clear();
	}

	const AshtakavargaExpert *expert;
};

IMPLEMENT_CLASS( AshtakaVargaPindaDiagram, DiagramWidget )

/*************************************************//**
*
*
*
******************************************************/
class AshtakaVargaTextWidget : public TextWidget
{
	DECLARE_CLASS( AshtakaVargaTextWidget )

public:
	AshtakaVargaTextWidget( wxWindow *parent, BasicView *v, AshtakavargaExpert *expert )
			: TextWidget( parent, v )
	{
		this->expert = expert;
	}

	virtual int write()
	{
		expert->write( writer, config->showTextViewHeaders );
		return 0;
	}

	int varga;

private:
	AshtakavargaExpert *expert;
};

IMPLEMENT_CLASS( AshtakaVargaTextWidget, TextWidget )

/*****************************************************
**
**   AshtakaVargaView   ---   Constructor
**
******************************************************/
AshtakaVargaView::AshtakaVargaView( wxWindow *parent, ChildWindow *frame, Document *doc )
		: BasicNotebookView( parent, frame, doc, VIEW_ASHTAKAVARGA, true, true, wxFULL_REPAINT_ON_RESIZE )
{
	supportEWToggle = false;
	setVedic( true );
	chart = new VargaHoroscope( doc );
	expert = new AshtakavargaExpert( chart );
	varga = VARGA_RASI;

	notebook = new NotebookWidget( this, BASIC_NOTEBOOK, wxDefaultPosition, wxDefaultSize, wxNB_DEFAULT );
	for ( int i = 0; i < MAX_ASHTAKAVARGA_PAGES; i++ )
	{
		switch ( i )
		{
		case ANB_REKHA:
			rekhagrid = new AshtakaVargaChartGridWidget( notebook, this, expert, REKHA, 3, 3 );
			notebook->AddPage( rekhagrid, _( "Rekha" ));
			break;
		case ANB_TRIKONA:
			trikonagrid = new AshtakaVargaChartGridWidget( notebook, this, expert, TRIKONA, 3, 3 );
			notebook->AddPage( trikonagrid, _( "Trikona Shodana" ));
			break;
		case ANB_EKADHIPATYA:
			ekadhigrid = new AshtakaVargaChartGridWidget( notebook, this, expert, EKADHI, 3, 3 );
			notebook->AddPage( ekadhigrid, _( "Ekadhipatya Shodana" ));
			break;
		case ANB_RASIDIAGRAM:
			rdiagram = new AshtakaVargaRasiDiagram( notebook, this, expert );
			notebook->AddPage( rdiagram, _( "Rasi Diagram" ));
			break;
		case ANB_PINDADIAGRAM:
			pdiagram = new AshtakaVargaPindaDiagram( notebook, this, expert );
			notebook->AddPage( pdiagram, _( "Pinda Diagram" ));
			break;
		case ANB_TEXT:
			theText = new AshtakaVargaTextWidget ( notebook, this, expert );
			notebook->AddPage( theText, _( "Text" ));
			break;
		default:
			printf( "Error: wrong notebook index %d in AshtakaVargaView::AshtakaVargaView\n", i );
			break;
		}
	}
	widget = notebook;
	initToolItems();
	OnDataChanged();

	Connect( TBS_VARGA, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( AshtakaVargaView::OnChoiceCommand ));
}

/*****************************************************
**
**   AshtakaVargaView   ---   Destructor
**
******************************************************/
AshtakaVargaView::~AshtakaVargaView()
{
	config->activePageAshtakaVarga = activepage;
	delete expert;
	delete chart;
}

/*****************************************************
**
**   AshtakaVargaView   ---   postCreate
**
******************************************************/
void AshtakaVargaView::postCreate()
{
	notebook->SetSelection( config->activePageAshtakaVarga );
}

/**************************************************************
***
**   AshtakaVargaView   ---   initToolItems
***
***************************************************************/
void AshtakaVargaView::initToolItems()
{
	if ( toolbar )
	{
		wxChoice *choice_varga = (wxChoice*)toolbar->FindControl( TBS_VARGA );
		if ( choice_varga ) choice_varga->SetSelection( varga );
	}
}

/*****************************************************
**
**   AshtakaVargaView   ---   OnToolbarCommand
**
******************************************************/
void AshtakaVargaView::OnToolbarCommand()
{
	if ( toolbar )
	{
		wxChoice *choice_varga = (wxChoice*)toolbar->FindControl( TBS_VARGA );
		if ( choice_varga ) varga = choice_varga->GetSelection();
	}
	OnDataChanged();
}

/*****************************************************
**
**   AshtakaVargaView   ---   supportsGraphicExport
**
******************************************************/
bool AshtakaVargaView::supportsGraphicExport() const
{
	return ( activepage != ANB_TEXT );
}

/*****************************************************
**
**   AshtakaVargaView   ---   supportsTextExport
**
******************************************************/
bool AshtakaVargaView::supportsTextExport() const
{
	return ( activepage == ANB_TEXT );
}

/*****************************************************
**
**   AshtakaVargaView   ---   supportsSkinToggle
**
******************************************************/
bool AshtakaVargaView::supportsSkinToggle() const
{
	switch ( activepage )
	{
	case ANB_REKHA:
	case ANB_TRIKONA:
	case ANB_EKADHIPATYA:
		return true;
		break;
	case ANB_RASIDIAGRAM:
	case ANB_PINDADIAGRAM:
	case ANB_TEXT:
	default:
		return false;
		break;
	}
}

/*****************************************************
**
**   AshtakaVargaView   ---   supportsRestrictedGraphicStyleToggle
**
******************************************************/
bool AshtakaVargaView::supportsRestrictedGraphicStyleToggle() const
{
	return supportsSkinToggle();
}

/*****************************************************
**
**   AshtakaVargaView   ---   dispatchCommand
**
******************************************************/
bool AshtakaVargaView::dispatchCommand( const int &command )
{
	switch ( command )
	{
	case CMD_CHILD_EXPORT:
		switch ( activepage )
		{
		case ANB_REKHA:
			rekhagrid->doExport();
			break;
		case ANB_TRIKONA:
			trikonagrid->doExport();
			break;
		case ANB_EKADHIPATYA:
			ekadhigrid->doExport();
			break;
		case ANB_RASIDIAGRAM:
			rdiagram->doExport();
			break;
		case ANB_PINDADIAGRAM:
			pdiagram->doExport();
			break;
		default:
			break;
		}
		break;
	case CMD_CHILD_SAVETEXT:
		theText->doExport();
		break;
	default:
		return BasicView::dispatchCommand( command );
		break;
	}
	return true;
}

/*****************************************************
**
**   AshtakaVargaView   ---   OnDataChanged
**
******************************************************/
void AshtakaVargaView::OnDataChanged()
{
	chart->setDivision( varga );
	expert->update();
	theText->varga = varga;
	theText->writeContents();
	notebook->GetCurrentPage()->Refresh();
}

/**************************************************************
***
**   AshtakaVargaView   ---   getWindowLabel
***
***************************************************************/
wxString AshtakaVargaView::getWindowLabel( const bool shortname )
{
	return shortname ? _( "AV" ) : _( "Ashtaka Varga" );
}

/*****************************************************
**
**   ViewFactory  ---   createAshtakaVargaView
**
******************************************************/
BasicView *ViewFactory::createAshtakaVargaView( wxWindow *parent, ChildWindow *frame, Document *doc )
{
	//info.showtoolbar = true;
	return new AshtakaVargaView( parent, frame, doc );
}

