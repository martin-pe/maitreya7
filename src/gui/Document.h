/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/Document.h
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

#ifndef _DOCUMENT_H_
#define _DOCUMENT_H_

#ifdef __GNUG__
#pragma interface "Document.h"
#endif

#include "Horoscope.h"
#include "guibase.h"

#include <wx/object.h>
#include <wx/timer.h>
#include <vector>

BEGIN_DECLARE_EVENT_TYPES()
DECLARE_EVENT_TYPE( DOC_SAVED, wxID_HIGHEST + 1200 )
DECLARE_EVENT_TYPE( DOC_UPDATED, wxID_HIGHEST + 1201 )
DECLARE_EVENT_TYPE( CREATE_ENTRY_CHART, wxID_HIGHEST + 1203 )
END_DECLARE_EVENT_TYPES()

class ChildWindow;
class DocumentManager;

using namespace std;

/*************************************************//**
*
* \brief Horoscope implementation with child window management and other GUI features
*
******************************************************/
class Document : public Horoscope
{
	DECLARE_CLASS( Document )

public:

	Document();
	~Document();

	void addChildWindow( ChildWindow* );
	void releaseChildWindow( ChildWindow *child );
	void setMainWindow( ChildWindow* );
	ChildWindow *getMainWindow() {
		return mainwindow;
	}
	bool save();
	bool saveAs();
	bool queryClose();
	bool isDirty() {
		return dirty;
	}
	void setDirty( const bool &d ) {
		dirty = d;
	}
	void updateAllChildWindows();
	void closeDependentChildWindows();
	void editData();
	void doPrint();
	void doQuickPrint();
	void StartTimer( bool b );
	bool isAnimated() const {
		return timer->IsRunning();
	}

protected:
	wxTimer *timer;
	void OnTimer( wxTimerEvent &event );
	int myAniInterval;
	bool dirty;
	vector<ChildWindow*> childlist;
	ChildWindow *mainwindow;
};

/*************************************************//**
*
* \brief listener propagating events for document lists (used in PartnerView)
*
******************************************************/
class DocumentListener : public wxObject
{
	DECLARE_CLASS( DocumentListener )
public:
	DocumentListener( DocumentManager *manager ) : wxObject() {
		this->manager = manager;
	}
	virtual ~DocumentListener() {}
	virtual void documentListChanged() = 0;

protected:
	DocumentManager *manager;

};

/*************************************************//**
*
* \brief holds dynamic lists of documents for PartnerView
*
******************************************************/
class DocumentManager
{
public:
	void addDoc( Document* );
	void deleteDoc( Document* );
	void updateAllDocs();

	void addClient( DocumentListener* );
	void deleteClient( DocumentListener* );
	int getNbDocuments();
	Document *getDocument( const int &i );

protected:
	vector<Document*> docs;
	vector<DocumentListener*> clients;

private:

};


#endif

