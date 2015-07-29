/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/base/ViewNode.cpp
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
#pragma implementation "ViewNode.h"
#endif

#include "ViewNode.h"

#include <wx/string.h>
#include <math.h>

#include "constants.h"
#include "Conf.h"

extern Config *config;

IMPLEMENT_CLASS( ViewNode, wxObject )
IMPLEMENT_CLASS( WidgetNode, ViewNode )
IMPLEMENT_CLASS( SplitterNode, ViewNode )
IMPLEMENT_CLASS( PageNode, ViewNode )
IMPLEMENT_CLASS( NotebookNode, ViewNode )

//#define DEB_VIEWMODE_DESTRUCTORS
/*****************************************************
**
**   ViewNode   ---   Destructor
**
******************************************************/
ViewNode::~ViewNode()
{
#ifdef DEB_VIEWMODE_DESTRUCTORS
	cout << "Destructor ViewNode" << Endl;
#endif
}

/*****************************************************
**
**   WidgetNode   ---   Constructor
**
******************************************************/
WidgetNode::WidgetNode( ViewNode *p )
		: ViewNode( p )
{
	varga = rows = cols = 0;
	viewtype = VIEW_EMPTY;
	textmode = config->textWindowPage;
	toolpanel = true;
	vedic = config->preferVedic;
}

/*****************************************************
**
**   WidgetNode   ---   Constructor
**
******************************************************/
WidgetNode::WidgetNode( const int &command )
		: ViewNode( 0 )
{
	varga = rows = cols = 0;
	textmode = config->textWindowPage;
	toolpanel = true;
	vedic = config->preferVedic;

	switch ( command )
	{
	case CMD_CHILD_NEW_TEXT:
		//textmode = config->textWindowPage;
		viewtype = VIEW_TEXT;
		break;
	case CMD_CHILD_NEW_TRANSIT:
		viewtype = VIEW_TRANSIT;
		break;
	case CMD_CHILD_NEW_SBC:
		viewtype = VIEW_SBC;
		break;
	case CMD_CHILD_NEW_SOLAR:
		viewtype = VIEW_SOLAR;
		break;
	case CMD_CHILD_NEW_DASA:
		viewtype = VIEW_DASA;
		break;
	case CMD_CHILD_NEW_GRAPHICALDASA:
		viewtype = VIEW_GRAPHICALDASA;
		break;
	case CMD_CHILD_NEW_URANIAN:
		viewtype = VIEW_URANIAN;
		break;
	case CMD_CHILD_NEW_MAINVIEW:
		viewtype = VIEW_PLANETLIST;
		break;
	case CMD_CHILD_NEW_YOGA:
		viewtype = VIEW_YOGA;
		break;
	case CMD_CHILD_NEW_GRAPHICGRID:
		viewtype = VIEW_GRAPHICGRID;
		break;
	case CMD_CHILD_NEW_VARGA:
		viewtype = VIEW_VARGA;
		break;
	case CMD_CHILD_NEW_ASHTAKAVARGA:
		viewtype = VIEW_ASHTAKAVARGA;
		break;
	case CMD_CHILD_NEW_VIMSOPAKABALA:
		viewtype = VIEW_VIMSOPAKABALA;
		break;
	case APP_SHOWHORA:
		viewtype = VIEW_HORA;
		break;
	case APP_SHOWECLIPSE:
		viewtype = VIEW_ECLIPSE;
		break;
	case APP_NEW_PARTNER:
		viewtype = VIEW_PARTNER;
		break;
	case APP_SHOWEPHEM:
		viewtype = VIEW_EPHEM;
		break;
	case APP_YOGAEDITOR:
		viewtype = VIEW_YOGA_EDITOR;
		break;
	case CMD_CHILD_NEW_WCHART:
		vedic = false;
		varga = 0;
		viewtype = VIEW_GRAPHIC;
		break;
	default:
		if ( command >= CMD_CHILD_NEW_RASI && command <= CMD_CHILD_NEW_RASI+30 )
		{
			vedic = true;
			varga = command-CMD_CHILD_NEW_RASI;
			viewtype = VIEW_GRAPHIC;
		}
		else viewtype = VIEW_EMPTY;
		break;
	}
}

/*****************************************************
**
**   WidgetNode   ---   Destructor
**
******************************************************/
WidgetNode::~WidgetNode()
{
#ifdef DEB_VIEWMODE_DESTRUCTORS
	cout << "Destructor WidgetNode" << Endl;
#endif
}

/*****************************************************
**
**   WidgetNode   ---   dump
**
******************************************************/
void WidgetNode::dump( wxString &o )
{
	o << wxT( "Start Widget" ) << Endl;
	o << wxT( "viewtype " )  << viewtype << Endl;
	o << wxT( "Text mode " ) << textmode << Endl;
	o << wxT( "Varga " ) << varga << Endl;
	o << wxT( "rows " ) << rows << Endl;
	o << wxT( "cols " ) << cols << Endl;
	o << wxT( "--- Start VARGAS " ) << Endl;
	for ( unsigned int i = 0; i < vargas.size(); i++ )
	{
		o << wxT( " - Varga: " ) << vargas[i] << Endl;
	}
	o << wxT( "--- End VARGAS " ) << Endl;
	o << wxT( "toolpanel " ) << toolpanel << Endl;
	o << wxT( "End Widget" ) << Endl;
}

/*****************************************************
**
**   SplitterNode   ---   Destructor
**
******************************************************/
SplitterNode::~SplitterNode()
{
#ifdef DEB_VIEWMODE_DESTRUCTORS
	cout << "Destructor SplitterNode" << Endl;
#endif
	if ( window1 ) delete window1;
	if ( window2 ) delete window2;
}

/*****************************************************
**
**   SplitterNode   ---   dump
**
******************************************************/
void SplitterNode::dump( wxString &o )
{
	o << wxT( "Start Splitter Direction: " ) << dir << Endl;
	o << wxT( "saspos " ) << sashpos << Endl;
	o << wxT( "sasposrel " ) << sashpos_rel << Endl;
	if ( window1 ) window1->dump( o );
	else o << wxT( "Window1 empty" ) << Endl;
	if ( window2 ) window2->dump( o );
	else o << wxT( "Window2 empty" ) << Endl;
	o << wxT( "End Splitter" ) << Endl;
}

/*****************************************************
**
**   NotebookNode   ---   Destructor
**
******************************************************/
NotebookNode::~NotebookNode()
{
#ifdef DEB_VIEWMODE_DESTRUCTORS
	cout << "Destructor NotebookNode window is " << window->getName() << Endl;
#endif
	unsigned int i;
	for ( i = 0; i < pages.size(); i++ )
	{
		delete pages[i];
	}
	pages.clear();
}

/*****************************************************
**
**   NotebookNode   ---   dump
**
******************************************************/
void NotebookNode::dump( wxString &o )
{
	unsigned int i;
	o << wxT( "NOTEBOOK -------- "  ) << Endl;
	for ( i = 0; i < pages.size(); i++ )
	{
		o << Endl << wxT( "-------------------" ) << wxT( "Page No " ) << i+1 << Endl;
		pages[i]->dump( o );
	}
}

/*****************************************************
**
**   PageNode   ---   Destructor
**
******************************************************/
PageNode::~PageNode()
{
#ifdef DEB_VIEWMODE_DESTRUCTORS
	cout << "Destructor PageNode window is " << window->getName() << Endl;
#endif
	if ( window ) delete window;
}

/*****************************************************
**
**   PageNode   ---   dump
**
******************************************************/
void PageNode::dump( wxString &o )
{
	o << wxT( "Start Page " )
	<< name
	<< wxT( " window " )
	<< (long)window << Endl;
	if ( window ) window->dump( o );
	else o << wxT( "Page Empty" ) << Endl;
	o << wxT( "End Page" ) << Endl;
}


