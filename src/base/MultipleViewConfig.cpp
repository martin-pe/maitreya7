/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/base/MultipleViewConfig.cpp
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
#pragma implementation "MultipleViewConfig.h"
#endif

#include "MultipleViewConfig.h"

#include <wx/string.h>
#include <math.h>

#include "constants.h"
#include "Conf.h"

#include <wx/xml/xml.h>

extern Config *config;

#define XML_VIEW_TYPE_GRID          wxT( "GridView" )
#define XML_VIEW_TYPE_GRAPHIC       wxT( "GraphicView" )
#define XML_VIEW_TYPE_YOGA          wxT( "YogaView" )
#define XML_VIEW_TYPE_DASA          wxT( "DasaView" )
#define XML_VIEW_TYPE_TEXT          wxT( "TextView" )
#define XML_VIEW_TYPE_SOLAR         wxT( "SolarView" )
#define XML_VIEW_TYPE_SBC           wxT( "SbcView" )
#define XML_VIEW_TYPE_GDASA         wxT( "GraphicalDasaView" )
#define XML_VIEW_TYPE_URANIAN       wxT( "UranianView" )
#define XML_VIEW_TYPE_TRANSIT       wxT( "TransitView" )
#define XML_VIEW_TYPE_GRAPHIC_GRID  wxT( "GraphicGridView" )
#define XML_VIEW_TYPE_AV            wxT( "AshtakaVargaView" )
#define XML_VIEW_TYPE_SHADBALA      wxT( "ShadBalaView" )
#define XML_VIEW_TYPE_VIMSOPAKABALA wxT( "VimsopakaBalaView" )

IMPLEMENT_CLASS2( MultipleViewConfig, ViewNode, BaseXmlConfig )

//#define DUMP_MULTIPLE_VIEW_CONFIG

/*****************************************************
**
**   MultipleViewConfig   ---   Destructor
**
******************************************************/
MultipleViewConfig::~MultipleViewConfig()
{
#ifdef DEB_VIEWMODE_DESTRUCTORS
	cout << "Destructor MultipleViewConfig" << Endl;
#endif
	if ( window ) delete window;
}


/*****************************************************
**
**   MultipleViewConfig   ---   dumpXml
**
******************************************************/
wxString MultipleViewConfig::dumpXml()
{
	printf( "Not implemented: MultipleViewConfig::dumpXml\n" );
	wxString s;
	return s;
}
/*****************************************************
**
**   MultipleViewConfig   ---   dump
**
******************************************************/
wxString MultipleViewConfig::dump()
{
	wxString o;
	printf( "Not implemented: MultipleViewConfig::dump\n" );
	dump( o );
	return o;
}

/*****************************************************
**
**   MultipleViewConfig   ---   dump
**
******************************************************/
void MultipleViewConfig::dump( wxString &o )
{
	o << wxT( "MultipleViewConfig -------- sizex " ) << sizex << wxT( " sizey " ) << sizey << Endl;
	o << wxT( "Name: " ) << name << Endl;
	o << wxT( "Description: " ) << description << Endl;
	if ( window ) window->dump( o );
	else o << wxT( "MultipleViewConfig empty" ) << Endl;
}

MultipleViewConfigLoader *MultipleViewConfigLoader::ego = 0;

/*****************************************************
**
**   MultipleViewConfigLoader   ---   Constructor
**
******************************************************/
MultipleViewConfigLoader::MultipleViewConfigLoader()
 : XmlConfigLoader( XML_TOPIC_MULTIPLE_VIEWS )
{
}

/*****************************************************
**
**   MultipleViewConfigLoader   ---   getConfig
**
******************************************************/
MultipleViewConfig *MultipleViewConfigLoader::getConfig( const int &i )
{
	return (MultipleViewConfig*)getConfigInternal( i );
}

/*****************************************************
**
**   MultipleViewConfigLoader   ---   createConfig
**
******************************************************/
BaseXmlConfig *MultipleViewConfigLoader::createConfig( BaseXmlConfig* base )
{
	if ( base )
	{
		MultipleViewConfig *cfg = (MultipleViewConfig*) base;
		return new MultipleViewConfig( *cfg );
	}
	return new MultipleViewConfig;
}

/*****************************************************
**
**   MultipleViewConfigLoader   ---   readSpecificAttributes
**
******************************************************/
void MultipleViewConfigLoader::readSpecificAttributes( BaseXmlConfig *cfg, wxXmlNode *cur )
{
	// TODO ASSERT und abei allen andern
	MultipleViewConfig *mconf = (MultipleViewConfig*)cfg;

	wxString key;
	while ( cur )
	{
		key = cur->GetName();
		if ( key == XML_NODE_WINDOW_NODE ) mconf->window = parseWindowNode( mconf, cur );
		else reportWrongKey( key );
		cur = cur->GetNext();
	}
}

/*****************************************************
**
**   MultipleViewConfigLoader   ---   assignViewId
**
******************************************************/
int MultipleViewConfigLoader::assignViewId( wxString key )
{
	int ret = -1;
	if ( key ==  XML_VIEW_TYPE_GRID )               ret = VIEW_PLANETLIST;
	else if ( key ==  XML_VIEW_TYPE_GRAPHIC )       ret = VIEW_GRAPHIC;
	else if ( key ==  XML_VIEW_TYPE_YOGA )          ret = VIEW_YOGA;
	else if ( key ==  XML_VIEW_TYPE_DASA )          ret = VIEW_DASA;
	else if ( key ==  XML_VIEW_TYPE_TEXT )          ret = VIEW_TEXT;
	else if ( key ==  XML_VIEW_TYPE_SOLAR )         ret = VIEW_SOLAR;
	else if ( key ==  XML_VIEW_TYPE_SBC )           ret = VIEW_SBC;
	else if ( key ==  XML_VIEW_TYPE_GDASA )         ret = VIEW_GRAPHICALDASA;
	else if ( key ==  XML_VIEW_TYPE_URANIAN )       ret = VIEW_URANIAN;
	else if ( key ==  XML_VIEW_TYPE_TRANSIT )       ret = VIEW_TRANSIT;
	else if ( key ==  XML_VIEW_TYPE_GRAPHIC_GRID )  ret = VIEW_GRAPHICGRID;
	else if ( key ==  XML_VIEW_TYPE_AV )            ret = VIEW_ASHTAKAVARGA;
	else if ( key ==  XML_VIEW_TYPE_VIMSOPAKABALA ) ret = VIEW_VIMSOPAKABALA;
	else
	{
		printf( "Error: wrong view id %d in MultipleViewConfigLoader::assignViewId, returning default.\n", ret );
		ret = VIEW_EMPTY;
	}
	return ret;
}

/*****************************************************
**
**   MultipleViewConfigLoader   ---   parseWidgetVargas
**
******************************************************/
void MultipleViewConfigLoader::parseWidgetVargas( WidgetNode *node, wxXmlNode *cur )
{
	long l;
	wxString key;
	cur = cur->GetChildren();
	while ( cur )
	{
		key = cur->GetName();
		if ( key == XML_NODE_VARGA_LIST_ITEM )
		{
			cur->GetNodeContent().ToLong( &l );
			node->vargas.push_back( (int)l );
		}
		else reportWrongKey( key );
		cur = cur->GetNext();
	}
}

/*****************************************************
**
**   MultipleViewConfigLoader   ---   parseWidgetNode
**
******************************************************/
ViewNode *MultipleViewConfigLoader::parseWidgetNode( ViewNode *parent, wxXmlNode *cur )
{
	WidgetNode *node = new WidgetNode( parent );
	long l;
	wxString key;
	cur = cur->GetChildren();
	while ( cur )
	{
		key = cur->GetName();
		if ( key == XML_NODE_VIEW_TYPE )
		{
			node->viewtype = assignViewId( cur->GetNodeContent() );
		}
		else if ( key == XML_NODE_VARGA )
		{
			cur->GetNodeContent().ToLong( &l );
			node->varga = l;
		}
		else if ( key == XML_NODE_TEXTMODE )
		{
			cur->GetNodeContent().ToLong( &l );
			node->textmode = l;
		}
		else if ( key == XML_NODE_SUB_SHOWTOOLBAR )
		{
			node->toolpanel = ( cur->GetNodeContent().Lower() == wxT( "true" ));
		}
		else if ( key == XML_NODE_VEDIC )
		{
			node->vedic = ( cur->GetNodeContent().Lower() == wxT( "true" ));
		}
		else if ( key == XML_NODE_ROW_COUNT )
		{
			cur->GetNodeContent().ToLong( &l );
			node->rows = l;
		}
		else if ( key == XML_NODE_COL_COUNT )
		{
			cur->GetNodeContent().ToLong( &l );
			node->cols = l;
		}
		else if ( key == XML_NODE_VARGA_LIST )
		{
			parseWidgetVargas( node, cur );
		}
		else reportWrongKey( key );
		cur = cur->GetNext();
	}
	return node;
}

/*****************************************************
**
**   MultipleViewConfigLoader   ---   parseSplitterNode
**
******************************************************/
ViewNode *MultipleViewConfigLoader::parseSplitterNode( ViewNode *parent, wxXmlNode *cur )
{
	SplitterNode *node = new SplitterNode( parent );
	wxString key;
	cur = cur->GetChildren();
	while ( cur )
	{
		key = cur->GetName();
		if ( key == XML_NODE_WINDOW1 )
		{
			node->window1 = parseWindowNode( node, cur );
		}
		else if ( key == XML_NODE_WINDOW2 )
		{
			node->window2 = parseWindowNode( node, cur );
		}
		else if ( key == XML_NODE_SPLIT_DIRECTION )
		{
			if ( cur->GetNodeContent() == XML_NODE_SPLIT_VERTICAL ) node->dir = 1;
			else if ( cur->GetNodeContent() == XML_NODE_SPLIT_HORIZONTAL ) node->dir = 0;
			else
			{
				printf( "Error: wrong split direction in splitter, using default.\n" );
				node->dir = 0;
			}
		}
		else if ( key == XML_NODE_SASH_POS )
		{
			XMLNODE2INT( cur->GetNodeContent(), node->sashpos )
		}
		else if ( key == XML_NODE_SASH_POS_PERCENT )
		{
			XMLNODE2INT( cur->GetNodeContent(), node->sashpos_rel )
		}
		else reportWrongKey( key );
		cur = cur->GetNext();
	}
	return node;
}

/*****************************************************
**
**   MultipleViewConfigLoader   ---   parseNotebookNode
**
******************************************************/
ViewNode *MultipleViewConfigLoader::parseNotebookNode( ViewNode *parent, wxXmlNode *cur )
{
	NotebookNode *node = new NotebookNode( parent );
	wxString key;
	cur = cur->GetChildren();
	while ( cur )
	{
		key = cur->GetName();
		if ( key == XML_NODE_PAGE_NODE ) node->pages.push_back( parsePageNode( node, cur ));
		else reportWrongKey( key );
		cur = cur->GetNext();
	}
	return node;
}

/*****************************************************
**
**   MultipleViewConfigLoader   ---   parsePageNode
**
******************************************************/
PageNode *MultipleViewConfigLoader::parsePageNode( ViewNode *parent, wxXmlNode *cur )
{
	PageNode *node = new PageNode( parent );
	wxString key;
	cur = cur->GetChildren();
	while ( cur )
	{
		key = cur->GetName();
		if ( key == XML_NODE_NAME ) node->name = cur->GetNodeContent();
		else if ( key == XML_NODE_WIDGET_NODE ) node->window = parseWidgetNode( parent, cur );
		else if ( key == XML_NODE_SPLITTER_NODE ) node->window = parseSplitterNode( parent, cur );
		else if ( key == XML_NODE_NOTEBOOK_NODE ) node->window = parseNotebookNode( parent, cur );
		else reportWrongKey( key );
		cur = cur->GetNext();
	}
	return node;
}

/*****************************************************
**
**   MultipleViewConfigLoader   ---   parseWindowNode
**
******************************************************/
ViewNode *MultipleViewConfigLoader::parseWindowNode( ViewNode *parent, wxXmlNode *cur )
{
	ViewNode *node = 0;
	wxString key;
	cur = cur->GetChildren();
	while ( cur )
	{
		key = cur->GetName();
		if ( key == XML_NODE_WIDGET_NODE ) node = parseWidgetNode( parent, cur );
		else if ( key == XML_NODE_SPLITTER_NODE ) node = parseSplitterNode( parent, cur );
		else if ( key == XML_NODE_NOTEBOOK_NODE ) node = parseNotebookNode( parent, cur );
		else reportWrongKey( key );
		cur = cur->GetNext();
	}
	return node;
}


