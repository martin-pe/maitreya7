/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/VargaView.cpp
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
#pragma implementation "VargaView.h"
#endif

#include "VargaView.h"

#include <wx/event.h>
#include <wx/sizer.h>
#include <wx/statbox.h>

#include "constants.h"
#include "Conf.h"
#include "TextWidget.h"
#include "Varga.h"
#include "VargaHoroscope.h"
#include "Writer.h"

#include "Document.h"
#include "FontConfig.h"
#include "GraphicGrid.h"
#include "NotebookWidget.h"
#include "Painter.h"
#include "Planet.h"
#include "PlanetList.h"
#include "VedicChart.h"

extern Config *config;

#define MAX_VARGA_PAGES 5

enum { VNB_SHADVARGA, VNB_VARGA2, VNB_VARGA3, VNB_DIAGRAM, VNB_TEXT };

IMPLEMENT_CLASS( VargaView, BasicNotebookView )
IMPLEMENT_CLASS( GraphicGridView, BasicView )
IMPLEMENT_CLASS( VimsopakaBalaView, BasicView )

/*****************************************************
**
**   VimsopakaBalaDiagram   ---   Constructor
**
******************************************************/
VimsopakaBalaDiagram::VimsopakaBalaDiagram( wxWindow *parent, BasicView *v, const Horoscope *h )
		: DiagramWidget( parent, v, 3, 2 ),
		horoscope( h )
{
	planetmode = true;
	info.xmax = 20;
}

/*****************************************************
**
**   VimsopakaBalaDiagram   ---   paintDiagrams
**
******************************************************/
void VimsopakaBalaDiagram::paintDiagrams()
{
	unsigned int i;
	const vector<int> &obs = PlanetList().getVedicPlanetOnlyList();

	for ( i = 0; i < obs.size(); i++ ) info.addItem( DiagramItem( obs[i], horoscope->getPlanet( obs[i] )->getVimsopakaBala( 0 )));
	paintDiagram( 0, 0, _( "Shad Varga" ));
	info.clear();

	for ( i = 0; i < obs.size(); i++ ) info.addItem( DiagramItem( obs[i], horoscope->getPlanet( obs[i] )->getVimsopakaBala( 1 )));
	paintDiagram( 1, 0, _( "Sapta Varga" ));
	info.clear();

	for ( i = 0; i < obs.size(); i++ ) info.addItem( DiagramItem( obs[i], horoscope->getPlanet( obs[i] )->getVimsopakaBala( 2 )));
	paintDiagram( 2, 0, _( "Dasa Varga" ));
	info.clear();

	for ( i = 0; i < obs.size(); i++ ) info.addItem( DiagramItem( obs[i], horoscope->getPlanet( obs[i] )->getVimsopakaBala( 3 )));
	paintDiagram( 0, 1, _( "Shodasa Varga" ));
	info.clear();

	for ( i = 0; i < obs.size(); i++ ) info.addItem( DiagramItem( obs[i], horoscope->getPlanet( obs[i] )->getAverageVimsopakaBala()));
	paintDiagram( 1, 1, _( "Average" ));
	info.clear();
}

/*************************************************//**
*
*
*
******************************************************/
class VargaViewTextWidget : public TextWidget
{
	DECLARE_CLASS( VargaViewTextWidget )

public:
	VargaViewTextWidget( wxWindow *parent, BasicView *v, 	Horoscope *horoscope )
			: TextWidget( parent, v )
	{
		this->horoscope = horoscope;
	}

	virtual int write()
	{
		VargaExpert expert( horoscope );
		expert.write( getWriter(), mview->getVedicObjectStyle() );
		return 0;
	}

private:
	Horoscope *horoscope;
};

IMPLEMENT_CLASS( VargaViewTextWidget, TextWidget )

/*****************************************************
**
**   VargaView   ---   Constructor
**
******************************************************/
VargaView::VargaView( wxWindow *parent, ChildWindow *frame, Document *doc )
		: BasicNotebookView( parent, frame, doc, VIEW_VARGA, false, true, wxFULL_REPAINT_ON_RESIZE )
{
	supportEWToggle = false;
	setVedic( true );
	notebook = new NotebookWidget( this, BASIC_NOTEBOOK, wxDefaultPosition, wxDefaultSize, wxNB_DEFAULT );

	for ( int i = 0; i < MAX_VARGA_PAGES; i++  )
	{
		switch ( i )
		{
		case VNB_SHADVARGA:
			varga1 = new ShadVargaChartGridWidget( notebook, this, doc );
			notebook->AddPage( varga1, _( "Shad Varga" ));
			break;
		case VNB_VARGA2:
			varga2 = new VargaTab2ChartGridWidget( notebook, this, doc );
			notebook->AddPage( varga2, _( "Vargas (2)" ));
			break;
		case VNB_VARGA3:
			varga3 = new VargaTab3ChartGridWidget( notebook, this, doc );
			notebook->AddPage( varga3, _( "Vargas (3)" ));
			break;
		case VNB_DIAGRAM:
			balawidget = new VimsopakaBalaDiagram( notebook, this, doc );
			notebook->AddPage( balawidget, _( "Diagram" ));
			break;
		case VNB_TEXT:
			theText = new VargaViewTextWidget( notebook, this, doc );
			notebook->AddPage( theText, _( "Text" ));
			break;
		default:
			printf( "Error: wrong notebook index %d in VargaView::VargaView\n", i );
			break;
		}
	}
	widget = notebook;
	OnDataChanged();
}

/*****************************************************
**
**   VargaView   ---   Destructor
**
******************************************************/
VargaView::~VargaView()
{
	config->activePageVargaOverview = activepage;
}

/*****************************************************
**
**   VargaView   ---   postCreate
**
******************************************************/
void VargaView::postCreate()
{
	notebook->SetSelection( config->activePageVargaOverview );
}

/*****************************************************
**
**   VargaView   ---   supportsGraphicExport
**
******************************************************/
bool VargaView::supportsGraphicExport() const
{
	return ( activepage != VNB_TEXT );
}

/*****************************************************
**
**   VargaView   ---   supportsTextExport
**
******************************************************/
bool VargaView::supportsTextExport() const
{
	return ( activepage == VNB_TEXT );
}

/*****************************************************
**
**   VargaView   ---   supportsFullGraphicStyleToggle
**
******************************************************/
bool VargaView::supportsFullGraphicStyleToggle() const
{
	switch ( activepage )
	{
	case VNB_SHADVARGA:
	case VNB_VARGA2:
	case VNB_VARGA3:
		return true;
		break;
	case VNB_DIAGRAM:
	case VNB_TEXT:
	default:
		return false;
		break;
	}
}

/*****************************************************
**
**   VargaView   ---   supportsSkinToggle
**
******************************************************/
bool VargaView::supportsSkinToggle() const
{
	return supportsObjectToggle();
}

/*****************************************************
**
**   VargaView   ---   supportsObjectToggle
**
******************************************************/
bool VargaView::supportsObjectToggle() const
{
	switch ( activepage )
	{
	case VNB_SHADVARGA:
	case VNB_VARGA2:
	case VNB_VARGA3:
		return true;
		break;
	case VNB_DIAGRAM:
	case VNB_TEXT:
	default:
		return false;
		break;
	}
}

/*****************************************************
**
**   VargaView   ---   dispatchCommand
**
******************************************************/
bool VargaView::dispatchCommand( const int &command )
{
	switch ( command )
	{
	case CMD_CHILD_EXPORT:
		switch ( activepage )
		{
		case 0:
			varga1->doExport();
			break;
		case 1:
			varga2->doExport();
			break;
		case 2:
			varga3->doExport();
			break;
		case 3:
			balawidget->doExport();
			break;
		default:
			//assert( false );
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
**   VargaView   ---   OnDataChanged
**
******************************************************/
void VargaView::OnDataChanged()
{
	theText->writeContents();
	notebook->GetCurrentPage()->Refresh();
}

/**************************************************************
***
**   VargaView   ---   getWindowLabel
***
***************************************************************/
wxString VargaView::getWindowLabel( const bool shortname )
{
	return _( "Vargas" );
}

/*****************************************************
**
**   GraphicGridView   ---   Constructor
**
******************************************************/
GraphicGridView::GraphicGridView( wxWindow *parent, ChildWindow *frame, Document *doc, const int &rows, const int &cols,
                                  vector<int> vargas, const bool &vedic )
		: BasicView( parent, frame, doc, VIEW_GRAPHICGRID, false, vedic )
{
	supportEWToggle = false;
	setVedic( true );
	supportGraphicExport = supportFullGraphicStyleToggle = supportObjectToggle = supportSkinToggle	= true;
	gwidget = new ChartGridWidget( this, this, rows, cols );

	for ( unsigned int i = 0; i < vargas.size(); i++ ) gwidget->addVedicVargaChart( doc, vargas[i] );
	widget = gwidget;
}

/*****************************************************
**
**   GraphicGridView   ---   Destructor
**
******************************************************/
GraphicGridView::~GraphicGridView()
{
	for ( unsigned int i = 0; i < charts.size(); i++ ) delete charts[i];
	charts.clear();
}

/**************************************************************
***
**   GraphicGridView   ---   getWindowLabel
***
***************************************************************/
wxString GraphicGridView::getWindowLabel( const bool shortname )
{
	return shortname ? _( "Vargas" ) : _( "Vargas" );
}

/*****************************************************
**
**   VimsopakaBalaView   ---   Constructor
**
******************************************************/
VimsopakaBalaView::VimsopakaBalaView( wxWindow *parent, ChildWindow *frame, Document *doc )
		: BasicView( parent, frame, doc, VIEW_VIMSOPAKABALA, false, true )
{
	supportEWToggle = false;
	diagram = new VimsopakaBalaDiagram( this, this, (Horoscope*)doc );

	widget = diagram;
}

/**************************************************************
***
**   VimsopakaBalaView   ---   getWindowLabel
***
***************************************************************/
wxString VimsopakaBalaView::getWindowLabel( const bool shortname )
{
	return _( "Vimsopaka Bala" );
}

/*****************************************************
**
**   ViewFactory  ---   createVimsopakaBalaView
**
******************************************************/
BasicView *ViewFactory::createVimsopakaBalaView( wxWindow *parent, ChildWindow *frame, Document *doc )
{
	return new VimsopakaBalaView( parent, frame, doc );
}

/*****************************************************
**
**   ViewFactory  ---   createVargaView
**
******************************************************/
BasicView *ViewFactory::createVargaView( wxWindow *parent, ChildWindow *frame, Document *doc )
{
	return new VargaView( parent, frame, doc );
}

/*****************************************************
**
**   ViewFactory  ---   createGraphicGridView
**
******************************************************/
BasicView *ViewFactory::createGraphicGridView( wxWindow *parent, ChildWindow *frame, Document *doc, const int &rows, const int &cols,
        vector<int> vargas, const bool &vedic )
{
	return new GraphicGridView( parent, frame, doc, rows, cols, vargas, vedic );
}

