/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/GraphicView.cpp
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
#pragma implementation "GraphicView.h"
#endif

#include "GraphicView.h"

#include <wx/dc.h>
#include <wx/event.h>
#include <wx/sizer.h>

#include "constants.h"
#include "ChildWindow.h"
#include "Conf.h"
#include "Document.h"
#include "FontConfig.h"
#include "GraphicGrid.h"
#include "Painter.h"
#include "Varga.h"
#include "VedicChart.h"

extern Config *config;

IMPLEMENT_CLASS( GraphicView, BasicView )

/*****************************************************
**
**   GraphicView   ---   Constructor
**
******************************************************/
GraphicView::GraphicView( wxWindow *parent, ChildWindow *frame, Document *doc, const int &varga, const bool &vedic )
		: BasicView( parent, frame, doc, VIEW_GRAPHIC, false, vedic, wxFULL_REPAINT_ON_RESIZE ),
		varga( varga )
{
	supportEWToggle = true;
	supportGraphicExport = supportFullGraphicStyleToggle = supportObjectToggle = supportSkinToggle	= true;
	gwidget = new ChartGridWidget( this, this, 1, 1 );
	gwidget->addVedicVargaChart( doc, varga );
	widget = gwidget;
}

/**************************************************************
***
**   GraphicView   ---   getWindowLabel
***
***************************************************************/
wxString GraphicView::getWindowLabel( const bool shortname )
{
	wxString s;
	if ( isVedic() )
	{
		if ( shortname )
		{
			if ( varga < NB_VARGAS-1 )
				s.Printf( wxT( "D%d" ), VargaExpert().getVargaDivision( varga ) );
			else
				s.Printf( wxT( "%s" ), VargaExpert().getVargaName( varga ));
		}
		else
		{
			if ( varga < NB_VARGAS-1 )
			{
				s.Printf( wxT( "%s (D%d)" ), VargaExpert().getVargaName( varga ),
				          VargaExpert().getVargaDivision( varga ) );
			}
			else
			{
				s.Printf( wxT( "%s" ), VargaExpert().getVargaName( varga ));
			}
		}
	}
	else
	{
		return shortname ? _( "Chart" ) : _( "Western Chart" );
	}
	return s;
}

/*****************************************************
**
**   GraphicView   ---   OnDataChanged
**
******************************************************/
void GraphicView::OnDataChanged()
{
	//printf( "GraphicView::OnDataChanged\n" );
	BasicView::OnDataChanged();
	gwidget->OnDataChanged();
}

/*****************************************************
**
**   GraphicView   ---   dispatchCommand
**
******************************************************/
bool GraphicView::dispatchCommand( const int &command )
{
	int ret;
	if ( command == CMD_CHILD_EXPORT )
	{
		gwidget->doExport();
		return true;
	}
	else if ( command == CMD_CHILD_WESTERNMODE || command == CMD_CHILD_VEDICMODE )
	{
		ret = BasicView::dispatchCommand( command );
		frame->setTitle();
		return ret;
	}
	else return BasicView::dispatchCommand( command );
}

/*****************************************************
**
**   ViewFactory   ---   createGraphicView
**
******************************************************/
BasicView *ViewFactory::createGraphicView( wxWindow *parent, ChildWindow *frame, Document *doc, const int &varga, const bool &vedic )
{
	return new GraphicView( parent, frame, doc, varga, vedic );
}

