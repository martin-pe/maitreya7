/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/base/ViewNode.h
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
#ifndef _VIEWNODE_H_
#define _VIEWNODE_H_

#ifdef __GNUG__
#pragma interface "ViewNode.h"
#endif

#include <wx/gdicmn.h>
#include <wx/object.h>
#include <wx/string.h>
#include <vector>

class wxWindow;

using namespace std;

/*************************************************//**
*
* \brief abstract class representing a node for MultipleChildWindow
*
******************************************************/
struct ViewNode : public wxObject
{

	ViewNode( ViewNode *p ) {
		parent = p;
		size = wxDefaultSize;
		widget = 0;
	}
	virtual ~ViewNode();

	virtual void dump( wxString& ) = 0;

	wxWindow *widget;
	ViewNode *parent;
	wxSize size;

	DECLARE_CLASS( ViewNode )
};

/*************************************************//**
*
* \brief ViewNode as widget
*
******************************************************/
struct WidgetNode : public ViewNode
{
	WidgetNode( ViewNode *p );
	WidgetNode( const int &command );

	virtual ~WidgetNode();

	virtual void dump( wxString& );

	int viewtype;
	int varga;
	int textmode;
	int rows;
	int cols;
	bool vedic;
	bool toolpanel;

	vector<int> vargas;

	DECLARE_CLASS( ViewNode )
};

/*************************************************//**
*
* \brief node as splitter, containing 2 other nodes
*
******************************************************/
struct SplitterNode : public ViewNode
{
	SplitterNode( ViewNode *p ) : ViewNode( p ) {
		window1 = window2 = 0;
		dir = 0;
		sashpos = 200;
		sashpos_rel = 0;
	}
	virtual ~SplitterNode();

	virtual void dump( wxString& );

	ViewNode *window1;
	ViewNode *window2;
	int dir;
	int sashpos;
	int sashpos_rel;

	DECLARE_CLASS( SplitterNode )
};

/*************************************************//**
*
* \brief node as page of a NotebookNode
*
******************************************************/
struct PageNode : public ViewNode
{
	PageNode( ViewNode *p ) : ViewNode( p ) {
		window = 0;
		name = wxT( "dummy" );
	}
	virtual ~PageNode();

	virtual void dump( wxString& );

	ViewNode *window;
	wxString name;

	DECLARE_CLASS( PageNode )
};

/*************************************************//**
*
* \brief node as notebook containing several pages
*
******************************************************/
struct NotebookNode : public ViewNode
{
	NotebookNode( ViewNode *p ) : ViewNode( p ) {}
	virtual ~NotebookNode();

	virtual void dump( wxString& );

	vector<PageNode*> pages;
	wxString name;

	DECLARE_CLASS( NotebookNode )
};

#endif

