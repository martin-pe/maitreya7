/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/ChildWindow.h
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

#ifndef _CHILDWINDOW_H_
#define _CHILDWINDOW_H_

#ifdef __GNUG__
#pragma interface "ChildWindow.h"
#endif

#include <wx/frame.h>
#include <vector>

#include "constants.h"

class Document;
class DocumentManager;
class BasicView;
class ViewContainer;
class ViewNode;
class wxMenu;
class wxImageList;

using namespace std;

BEGIN_DECLARE_EVENT_TYPES()
DECLARE_EVENT_TYPE( CHILD_ADDTORECENT, wxID_HIGHEST + 1100 )
DECLARE_EVENT_TYPE( CHILD_SETACTIVE, wxID_HIGHEST + 1101 )
DECLARE_EVENT_TYPE( CHILD_CLOSED, wxID_HIGHEST + 1102 )
DECLARE_EVENT_TYPE( CHILD_ADDED, wxID_HIGHEST + 1103 )
END_DECLARE_EVENT_TYPES()

/*************************************************//**
*
* \brief base class for child window based upon wxFrame
*
******************************************************/
class ChildWindow : public wxFrame
{
	DECLARE_CLASS( ChildWindow )
public:
	ChildWindow( wxFrame *parent, Document *doc, const wxSize& size = wxDefaultSize, const bool ismain = false );
	~ChildWindow();

	virtual bool getPreferVedic() = 0;
	virtual bool supportsEWToggle() const = 0;
	virtual void setTitle() = 0;
	virtual int getViewId() = 0;

	virtual void OnDataChanged();
	Document *getDoc() { return doc; }
	virtual wxString getWindowLabel( const bool shortname = false ) { return wxT("DUMMY"; ) }
	void setMainWindow( const bool& );
	bool isMainWindow() { return ismainwindow; }
	void showContextMenu( wxMenu *menu, const int &x, const int &y );
	virtual wxMenu *getContextMenu() = 0;
	void OnActivate( wxActivateEvent& );
	virtual void postCreate() = 0;
	virtual void OnCommand( wxCommandEvent &event );

	void setShowEphemFileWarning( const bool b = true ) { showEphemFileWarning = b; }

protected:
	virtual void OnClose( wxCloseEvent &event );
	Document *doc;
	virtual bool dispatchCommand( const int& command, class BasicView* );
	void OnMouseWheelEvent( wxMouseEvent& );
	virtual void handleMouseWheelEvent( wxMouseEvent& ) = 0;

private:
	void OnKeyDown( wxKeyEvent& );
	void OnChar( wxKeyEvent& );
	void OnEphemFileWarning( wxCommandEvent& );

	bool ismainwindow, isvalid;
	bool showEphemFileWarning;
};

/*************************************************//**
*
* \brief child window containing a single BasicView
*
******************************************************/
class SimpleChildWindow : public ChildWindow
{
	DECLARE_CLASS( SimpleChildWindow )
public:
	SimpleChildWindow( wxFrame *parent, Document *doc, const wxSize& size = wxDefaultSize, const bool ismain = false );
	~SimpleChildWindow();
	void insertView( BasicView* );
	BasicView *getView( const int = 0 ) {
		return view;
	}
	virtual void OnDataChanged();
	virtual bool getPreferVedic();
	virtual bool supportsEWToggle() const;
	virtual void setTitle();
	virtual wxString getWindowLabel( const bool shortname = false );
	virtual wxMenu *getContextMenu();
	virtual int getViewId();
	virtual void postCreate();

protected:
	virtual void OnClose( wxCloseEvent &event );
	void OnActivate( wxActivateEvent& );
	void OnSize( wxSizeEvent& );
	virtual bool dispatchCommand( const int& command, class BasicView* );

	virtual void handleMouseWheelEvent( wxMouseEvent& );

private:
	BasicView *view;
};

/*************************************************//**
*
* \brief encapsulates creation of a ChildWindow.
*
******************************************************/
class ChildWindowFactory
{
public:
	ChildWindow *createPartnerWindow( wxFrame*, DocumentManager* );
	ChildWindow *createSingleMainWindow( wxFrame *parent, Document *doc );
	ChildWindow *createMultipleMainWindow( wxFrame *parent, Document *doc, const int = 0 );
	ChildWindow *createMainWindow( wxFrame*, Document*, const bool&, const int = 0 );
	ChildWindow *createChild( wxFrame *parent, Document *doc, const int id, const bool &vedic );
};

#endif

