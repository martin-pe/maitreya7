/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/MultipleChildWindow.h
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

#ifndef _MULTIPLECHILDWINDOW_H_
#define _MULTIPLECHILDWINDOW_H_

#ifdef __GNUG__
#pragma interface "MultipleChildWindow.h"
#endif

#include <wx/frame.h>
#include <vector>

#include "ChildWindow.h"

class Document;
class DocumentManager;
class BasicView;
class MultipleViewConfig;
class wxMenu;

using namespace std;

/*************************************************//**
*
* \brief complex child Window containing notebooks, splitters and views
*
******************************************************/
class MultipleChildWindow : public ChildWindow
{
	DECLARE_CLASS( MultipleChildWindow )
	friend class NotebookNodeHelper;

public:
	MultipleChildWindow( wxFrame *parent, Document *doc, const wxSize& size = wxDefaultSize, const bool ismain = false );
	~MultipleChildWindow();
	void initViews( MultipleViewConfig* );
	virtual void OnDataChanged();
	virtual bool getPreferVedic();
	virtual bool supportsEWToggle() const;
	virtual void setTitle();
	virtual wxString getWindowLabel( const bool shortname = false );
	virtual wxMenu *getContextMenu();
	virtual int getViewId() {
		return VIEW_MULTIPLE;
	}
	virtual void postCreate();
	MultipleViewConfig* getMultipleViewConfig() {
		return viewconfig;
	}

protected:
	virtual void handleMouseWheelEvent( wxMouseEvent& );

	wxWindow *insertNode( wxWindow *parent, ViewNode* );
	vector<BasicView*> viewlist;
	void OnActivate( wxActivateEvent& );
	void OnSize( wxSizeEvent& );
	void postCreateNode( ViewNode* );
	MultipleViewConfig *viewconfig;

};

#endif

