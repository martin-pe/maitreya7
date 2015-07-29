/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/NotebookNodes.cpp
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
#pragma implementation "NotebookNodes.h"
#endif

#include "NotebookNodes.h"

#include "Conf.h"
#include "IconProvider.h"
#include "MultipleViewConfig.h"
#include "MultipleChildWindow.h"

#include <wx/imaglist.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/listbook.h>
#include <wx/choicebk.h>

#ifdef wxUSE_LISTBOOK
#include <wx/listbook.h>
#endif

#ifdef wxUSE_TOOLBOOK
#include <wx/toolbook.h>
#endif

#ifdef wxUSE_TREEBOOK
#include <wx/treebook.h>
#endif

#include <vector>

using namespace std;
extern Config *config;

/*************************************************//**
*
* \brief creates Notebooks for MultipleChildWindow
*
******************************************************/
class NotebookNodeHelper
{
public:
	NotebookNodeHelper( wxBookCtrlBase*, MultipleChildWindow*, NotebookNode* );
	~NotebookNodeHelper() {
		delete imagelist;
	}

	void initPage( wxWindow*, const int& );
	int getPageClassId( const int& );

	NotebookNode *mainnode;
	MultipleChildWindow *multiplechild;
	wxImageList *imagelist;
	vector<int> initialized;
};

/*****************************************************
**
**   NotebookNodeHelper   ---   Constructor
**
******************************************************/
NotebookNodeHelper::NotebookNodeHelper( wxBookCtrlBase *notebook, MultipleChildWindow *mchild, NotebookNode *mnode )
{
	mainnode = mnode;
	mainnode->widget = notebook;
	multiplechild = mchild;

	imagelist = new wxImageList( 24, 24, true );
	imagelist->Add( IconProvider::getBitmap( BITMAP_SVIEW ));
	imagelist->Add( IconProvider::getBitmap( BITMAP_CHART ));
	imagelist->Add( IconProvider::getBitmap( BITMAP_YOGA ));
	imagelist->Add( IconProvider::getBitmap( BITMAP_DASA ));
	imagelist->Add( IconProvider::getBitmap( BITMAP_TEXT ));
	imagelist->Add( IconProvider::getBitmap( BITMAP_SOLAR ));
	imagelist->Add( IconProvider::getBitmap( BITMAP_SBC ));
	imagelist->Add( IconProvider::getBitmap( BITMAP_GRAPHICALDASA ));
	imagelist->Add( IconProvider::getBitmap( BITMAP_URANIAN ));
	imagelist->Add( IconProvider::getBitmap( BITMAP_TRANSIT ));
	imagelist->Add( IconProvider::getBitmap( BITMAP_CHART ));
	imagelist->Add( IconProvider::getBitmap( BITMAP_ASHTAKAVARGA ));
	imagelist->Add( IconProvider::getBitmap( BITMAP_VARGA ));
	imagelist->Add( IconProvider::getBitmap( BITMAP_SHADBALA ));
	notebook->SetImageList( imagelist );
	for ( unsigned int i = 0; i < mainnode->pages.size(); i++ ) initialized.push_back( false );
}

/*****************************************************
**
**   NotebookNodeHelper   ---   initPage
**
******************************************************/
void NotebookNodeHelper::initPage( wxWindow *panel, const int &p )
{
	wxSize size = panel->GetSize();
	panel->Show( false );

	ViewNode *node = mainnode->pages[p]->window;
	wxWindow *window = multiplechild->insertNode( panel, node );

	// Sizer stuff
	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add( window, 1, wxEXPAND );
	panel->SetAutoLayout(true);
	panel->SetSizer(sizer);
	sizer->Fit( panel );
	panel->SetSize( size );
	panel->Show( true );
	panel->SetFocus();
	initialized[p] = true;
	multiplechild->postCreateNode( node );
}

/*****************************************************
**
**   NotebookNodeHelper   ---   getPageClassId
**
******************************************************/
int NotebookNodeHelper::getPageClassId( const int &id )
{
	int count = 0, viewtype;
	const static int bmp_map[14] = {
		VIEW_PLANETLIST, VIEW_GRAPHIC, VIEW_YOGA, VIEW_DASA, VIEW_TEXT, VIEW_SOLAR, VIEW_SBC, VIEW_GRAPHICALDASA,
		VIEW_URANIAN, VIEW_TRANSIT, VIEW_GRAPHICGRID, VIEW_ASHTAKAVARGA, VIEW_VIMSOPAKABALA, VIEW_SHADBALA
	};

	ViewNode *swindow = mainnode->pages[id]->window;

	// splitter nodes have no icon. search for the first widget subnode
	while ( ! swindow->GetClassInfo()->IsKindOf( CLASSINFO( WidgetNode )))
	{
		if ( swindow->GetClassInfo()->IsKindOf( CLASSINFO( SplitterNode ))) swindow = ((SplitterNode*)swindow)->window1;
		assert( count++ < 10 );
	}

	viewtype = ((WidgetNode*)swindow)->viewtype;
	for ( int i = 0; i < 14; i++ )
	{
		if ( bmp_map[i] == viewtype ) return i;
	}
	printf( "Error: wrong viewtype %d for node #%d in method NotebookNodeHelper::getPageClassId\n", viewtype, id );
	return 0;
}

/*************************************************//**
*
* \brief macro based creation of Notebooks for MultipleChildWindow
*
******************************************************/
#define DECLARE_NODE_CONTAINER( NodeContainer, BaseContainer, SelectEvent, SELECT_EVENT_MACRO_DEF ) \
class NodeContainer: public BaseContainer { \
public: \
	NodeContainer( wxWindow *parent, MultipleChildWindow*, NotebookNode*, \
		const wxPoint &pos = wxDefaultPosition,	const wxSize size = wxDefaultSize, long style = 0 ); \
	~NodeContainer() { delete helper; } \
protected: \
	void OnNotebook( SelectEvent& ); \
	NotebookNodeHelper *helper; \
private: \
	DECLARE_CLASS( cl ) \
	DECLARE_EVENT_TABLE() \
}; \
IMPLEMENT_CLASS( NodeContainer, BaseContainer ) \
BEGIN_EVENT_TABLE( NodeContainer, BaseContainer ) \
		SELECT_EVENT_MACRO_DEF( wxID_ANY, NodeContainer::OnNotebook ) \
END_EVENT_TABLE() \
NodeContainer::NodeContainer( wxWindow *parent, MultipleChildWindow *mchild, NotebookNode *mnode, \
	const wxPoint &pos,	const wxSize size, long style ) \
	: BaseContainer( parent, -1, pos, size, style ) \
{ \
	helper = new NotebookNodeHelper( this, mchild, mnode ); \
	for( unsigned int i = 0; i < helper->mainnode->pages.size(); i++ ) \
	{ \
		wxPanel *panel = new wxPanel( this, -1 ); \
		PageNode *page = helper->mainnode->pages[i]; \
		page->widget = panel; \
		this->AddPage( panel, page->name, false, helper->getPageClassId( i )); \
	}  \
} \
void NodeContainer::OnNotebook( SelectEvent &event ) \
{ \
	if ( event.GetSelection() != -1 ) \
	{ \
		if ( ! helper->initialized[event.GetSelection()] ) helper->initPage( GetPage( event.GetSelection()), event.GetSelection() ); \
	} \
	event.Skip(); \
}

/*****************************************************
**
**   Declarations: ALL are available under wx 2.7 and 2.8.
**   Not available under 2.6 are Toolbook and Treebook
**
******************************************************/
DECLARE_NODE_CONTAINER( NotebookNodeContainer, wxNotebook, wxNotebookEvent, EVT_NOTEBOOK_PAGE_CHANGED )

#if wxUSE_LISTBOOK
DECLARE_NODE_CONTAINER( ListbookNodeContainer, wxListbook, wxListbookEvent, EVT_LISTBOOK_PAGE_CHANGED )
#endif

#if wxUSE_CHOICEBOOK
DECLARE_NODE_CONTAINER( ChoicebookNodeContainer, wxChoicebook, wxChoicebookEvent, EVT_CHOICEBOOK_PAGE_CHANGED )
#endif

#if wxUSE_TOOLBOOK
DECLARE_NODE_CONTAINER( ToolbookNodeContainer, wxToolbook, wxToolbookEvent, EVT_TOOLBOOK_PAGE_CHANGED )
#endif

#if wxUSE_TREEBOOK
DECLARE_NODE_CONTAINER( TreebookNodeContainer, wxTreebook, wxTreebookEvent, EVT_TREEBOOK_PAGE_CHANGED )
#endif

#undef DECLARE_NODE_CONTAINER

/*****************************************************
**
**   NotebookNodeContainerFactory   ---   createNotebookNodeContainer
**
******************************************************/
wxBookCtrlBase *NotebookNodeContainerFactory::createNotebookNodeContainer( wxWindow *parent, MultipleChildWindow *mchild,
        NotebookNode *mnode, const wxPoint &pos,	const wxSize size )
{
	wxBookCtrlBase *ctrl = 0;
	int orient;
	switch ( config->mViewNotebookOrientation )
	{
	case 1:
		orient = wxNB_BOTTOM;
		break;
	case 2:
		orient = wxNB_LEFT;
		break;
	case 3:
		orient = wxNB_RIGHT;
		break;
	default:
		orient = wxNB_TOP;
		break;
	}
	switch ( config->mViewNotebookStyle )
	{
	case 0:
		ctrl = new NotebookNodeContainer( parent, mchild, mnode, pos, size, orient );
		break;
#if wxUSE_CHOICEBOOK
	case 2:
		ctrl = new ChoicebookNodeContainer( parent, mchild, mnode, pos, size, orient );
		break;
#endif
#if wxUSE_TOOLBOOK
	case 3:
		ctrl = new ToolbookNodeContainer( parent, mchild, mnode, pos, size, orient );
		break;
#endif
#if wxUSE_TREEBOOK
	case 4:
		ctrl = new TreebookNodeContainer( parent, mchild, mnode, pos, size, orient );
		break;
#endif
	default:
		ctrl = new ListbookNodeContainer( parent, mchild, mnode, pos, size, orient );
		break;
	}
	assert( ctrl );
	return ctrl;
}

