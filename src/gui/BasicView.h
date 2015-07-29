/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/BasicView.h
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

#ifndef _BASICVIEW_H_
#define _BASICVIEW_H_

#ifdef __GNUG__
#pragma interface "BasicView.h"
#endif

#include <wx/window.h>
#include <vector>

#include "ChartProperties.h"

using namespace std;

class ChildWindow;

class Document;
class DocumentManager;
class MyStatusbar;
class WidgetNode;

class wxBoxSizer;
class wxCommandEvent;
class wxMenu;
class wxPanel;
class wxSizeEvent;
class wxSpinEvent;
class wxToolBar;

/*************************************************//**
*
* \brief base class for all views
*
******************************************************/
class BasicView : public wxWindow, public ChartProperties
{
public:
	BasicView( wxWindow *parent, ChildWindow *frame, Document *doc, const int &viewId, const bool &showToolbar,
	           const bool &vedic, const long style = 0 );

	const Document *getDoc() const {
		return doc;
	}
	const ChildWindow *getFrame() const {
		return frame;
	}
	const int getViewId() const {
		return viewId;
	}

	virtual bool isMainView() {
		return mainView;
	}

	// Toggle vedic/western
	virtual bool supportsEWToggle() const {
		return supportEWToggle;
	}

	// support export of text or graphic
	virtual bool supportsGraphicExport() const {
		return supportGraphicExport;
	}
	virtual bool supportsTextExport() const {
		return supportTextExport;
	}

	// Support for skin/object menus
	virtual bool supportsRestrictedGraphicStyleToggle() const {
		return supportRestrictedGraphicStyleToggle;
	}
	virtual bool supportsFullGraphicStyleToggle() const {
		return supportFullGraphicStyleToggle;
	}
	virtual bool supportsObjectToggle() const {
		return supportObjectToggle;
	}
	virtual bool supportsRestrictedObjectToggle() const {
		return supportRestrictedObjectToggle;
	}
	virtual bool supportsSkinToggle() const {
		return supportSkinToggle;
	}
	virtual bool supportsSbcStyleToggle() const {
		return supportSbcStyleToggle;
	}

	virtual void OnDataChanged();
	virtual wxString getWindowLabel( const bool shortname = false ) {
		return wxT( "---" );
	}
	virtual wxMenu *getContextMenu();
	virtual wxString getTitle();
	void doLayout();
	virtual wxSize DoGetBestSize() const;
	virtual void postCreate() {}
	virtual void handleMouseWheelEvent( wxMouseEvent& );
	void OnMouseWheelEvent( wxMouseEvent& );

	void echo( wxString );
	void setStatusText( wxString message, const int &field );

	wxWindow *getWidget() {
		return widget;
	}
	bool getIsMainWindow() {
		return isMainWindow;
	}
	virtual bool dispatchCommand( const int &command );

	virtual bool queryClose() {
		return true;
	}

protected:
	mutable ChildWindow *frame;
	const Document *doc;
	const int viewId;
	const bool showtoolbar;

	bool supportEWToggle, isMainWindow;
	bool supportGraphicExport;
	bool supportTextExport;
	bool supportFullGraphicStyleToggle;
	bool supportRestrictedGraphicStyleToggle;
	bool supportObjectToggle;
	bool supportSbcStyleToggle;
	bool supportSkinToggle;
	bool supportRestrictedObjectToggle;
	bool mainView;

	wxWindow *widget;
	void OnContextMenuEvent( wxMouseEvent& );
	void OnKeyDown( wxKeyEvent& );
	void OnChar( wxKeyEvent& );
	void OnSetFocus( wxFocusEvent& );

	virtual void handleSetFocusEvent();

	wxToolBar *toolbar;
	MyStatusbar *statusbar;

	wxWindow *detailpanel;
	virtual void initToolItems() {}

	virtual void OnChoiceCommand( wxCommandEvent& );
	virtual void OnSpinCommand( wxSpinEvent& );
	virtual void OnGeneralCommand( wxCommandEvent& );
	virtual void OnCommand( wxCommandEvent& );
	void OnMenuCommand( wxCommandEvent& );
	virtual void OnToolbarCommand() {}
	virtual void OnSize( wxSizeEvent& );
	virtual void OnEnterPressed( wxKeyEvent& );

private:

	wxBoxSizer* sizer;

	DECLARE_CLASS( BasicView )
};

/*************************************************//**
*
* \brief empty view for fallback
*
******************************************************/
class EmptyView : public BasicView
{
public:
	EmptyView( wxWindow *parent, ChildWindow *frame );

private:
	DECLARE_CLASS( EmptyView )
};

/*************************************************//**
*
* \brief encapsulates the creation of a BasicView. Implementation not here.
*
******************************************************/
class ViewFactory
{
public:

	BasicView *createPartnerView( wxWindow *parent, ChildWindow *frame, DocumentManager*, const bool &vedic );
	BasicView *createView( wxWindow *parent, ChildWindow *frame, Document *doc, WidgetNode* );

private:

	BasicView *createDasaView( wxWindow *parent, ChildWindow *frame, Document *doc );
	BasicView *createGraphicalDasaView( wxWindow *parent, ChildWindow *frame, Document *doc ) ;
	BasicView *createSbcView( wxWindow *parent, ChildWindow *frame, Document *doc );
	BasicView *createSolarView( wxWindow *parent, ChildWindow *frame, Document *doc, const bool &vedic );
	BasicView *createDefaultTextView( wxWindow *parent, ChildWindow *frame, Document *doc, const bool &toolpanel, const bool &vedic, const int &mode );
	BasicView *createYogaView( wxWindow *parent, ChildWindow *frame, Document *doc );
	BasicView *createMainView( wxWindow *parent, ChildWindow *frame, Document *doc, const bool &vedic );
	BasicView *createUranianView( wxWindow *parent, ChildWindow *frame, Document *doc, const bool &vedic );
	BasicView *createTransitView( wxWindow *parent, ChildWindow *frame, Document *doc, const bool &vedic );
	BasicView *createVargaView( wxWindow *parent, ChildWindow *frame, Document *doc );
	BasicView *createGraphicGridView( wxWindow *parent, ChildWindow *frame, Document *doc, const int &rows, const int &cols,
	                                  vector<int> vargas, const bool &vedic );
	BasicView *createVimsopakaBalaView( wxWindow *parent, ChildWindow *frame, Document *doc );
	BasicView *createAshtakaVargaView( wxWindow *parent, ChildWindow *frame, Document *doc );
	BasicView *createGraphicView( wxWindow *parent, ChildWindow *frame, Document *doc, const int &varga, const bool &vedic );

	// non doc views
	BasicView *createEclipseView( wxWindow *parent, ChildWindow *frame, const bool &vedic );
	BasicView *createEphemView( wxWindow *parent, ChildWindow *frame, const bool &vedic );
	BasicView *createHoraView( wxWindow *parent, ChildWindow *frame );
	BasicView *createYogaEditorView( wxWindow *parent, ChildWindow *frame );

	BasicView *createEmptyView( wxWindow *parent, ChildWindow *frame );
};

#endif

