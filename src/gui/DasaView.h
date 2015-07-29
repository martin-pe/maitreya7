/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/DasaView.h
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

#ifndef _DASA_VIEW_H_
#define _DASA_VIEW_H_

#ifdef __GNUG__
#pragma interface "DasaView.h"
#endif

#include "BasicView.h"

#include <wx/treebase.h>  // for wxTreeItemId
#include "TreeWidget.h"

class Dasa;
class DasaExpert;
class DasaPanel;
class DasaListViewItem;
class Horoscope;
class TreeWidget;

/*************************************************//**
*
* \brief tree view for Dasas
*
******************************************************/
class DasaView : public BasicView
{
	DECLARE_CLASS( DasaView )
public:
	DasaView( wxWindow *parent, ChildWindow *frame, Document *doc );

	virtual wxString getWindowLabel( const bool shortname = false );

protected:

	virtual void showToolPanel( const bool b );
	void updateDetailPanel( DasaListViewItem* );
	void expand( MyTreeEvent& event );
	void collapse( MyTreeEvent& event );
	void OnSelChanging( MyTreeEvent& event );
	void OnDataChanged();
	void updateDasa( wxTreeItemId masterid );
	void OnKeyDown( wxKeyEvent& );
	void OnEntryChart( wxCommandEvent& );
	virtual void handleMouseWheelEvent( wxMouseEvent& );

	TreeWidget *treectrl;
	wxTreeItemId rootid, baseid, specialid, conditionalid;
	double currentjd;
};

/*************************************************//**
*
* \brief abstract item in Dasa tree view
*
******************************************************/
class DasaListViewItem : public MyTreeItemData
{
public:

	DasaListViewItem( DasaExpert*, Horoscope* );
	~DasaListViewItem();

	DasaExpert *getExpert() { return expert; }
	Horoscope *getHoroscope() { return horoscope; }
	virtual bool isRootItem() = 0;

protected:

	DasaExpert *expert;
	Horoscope *horoscope;
};

/*************************************************//**
*
* \brief root item in Dasa tree view
*
******************************************************/
class DasaMainListViewItem : public DasaListViewItem
{
public:

	DasaMainListViewItem( DasaExpert*, Horoscope*, int varga = 0 );

	int getVarga() { return varga; }
	virtual bool isRootItem() { return true; }

protected:

	int varga;
};

/*************************************************//**
*
* \brief non root item in Dasa tree view
*
******************************************************/
class DasaDependendViewItem : public DasaListViewItem
{
public:

	DasaDependendViewItem( DasaExpert*, Dasa*, Horoscope* );

	Dasa *getDasa() { return dasa; }
	void setDasa( Dasa *d ) { dasa = d; }
	virtual bool isRootItem() { return false; }
	wxString writeContents();

protected:
	Dasa *dasa;
};

#endif

