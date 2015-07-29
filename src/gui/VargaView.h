/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/VargaView.h
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

#ifndef _VARGAVIEW_H_
#define _VARGAVIEW_H_

#ifdef __GNUG__
#pragma interface "VargaView.h"
#endif

#include "BasicView.h"
#include "NotebookWidget.h"
#include "DiagramWidget.h"

class Horoscope;
class NotebookWidget;
class TextWidget;
class VargaHoroscope;
class ChartGridWidget;
class wxNotebookEvent;

/*************************************************//**
*
* \brief Widget containing bar diagrams for Vimsopaka Bala
*
******************************************************/
class VimsopakaBalaDiagram : public DiagramWidget
{
public:
	VimsopakaBalaDiagram( wxWindow *parent, BasicView*, const Horoscope* );

protected:
	virtual void paintDiagrams();

	const Horoscope *horoscope;
};

/*************************************************//**
*
*  view for Varga topics (charts, bar diagrams and text output)
*
******************************************************/
class VargaView : public BasicNotebookView
{
	DECLARE_CLASS( VargaView )

public:

	VargaView( wxWindow *parent, ChildWindow *frame, Document *doc );
	~VargaView();
	virtual wxString getWindowLabel( const bool shortname = false );
	virtual void postCreate();
	virtual void OnDataChanged();

	virtual bool supportsGraphicExport() const;
	virtual bool supportsTextExport() const;
	virtual bool supportsFullGraphicStyleToggle() const;
	virtual bool supportsObjectToggle() const;
	virtual bool supportsSkinToggle() const;

	virtual bool dispatchCommand( const int &command );

protected:
	ChartGridWidget *varga1, *varga2, *varga3;
	TextWidget *theText;
	VimsopakaBalaDiagram *balawidget;

};

/*************************************************//**
*
*  \brief container for a grid of graphical charts
*
******************************************************/
class GraphicGridView : public BasicView
{
	DECLARE_CLASS( GraphicGridView )
public:
	GraphicGridView( wxWindow *parent, ChildWindow *frame, Document *doc, const int &rows, const int &cols,
	                 vector<int> vargas, const bool &vedic );
	~GraphicGridView();
	virtual wxString getWindowLabel( const bool shortname = false );

protected:
	ChartGridWidget *gwidget;
	vector<VargaHoroscope*> charts;

};

/*************************************************//**
*
* \brief Simple view for Vimsopaka Bala (used in Mulitple View)
*
******************************************************/
class VimsopakaBalaView : public BasicView
{
	DECLARE_CLASS( VimsopakaBalaView )
public:
	VimsopakaBalaView( wxWindow *parent, ChildWindow *frame, Document *doc );
	virtual wxString getWindowLabel( const bool shortname = false );

protected:
	VimsopakaBalaDiagram *diagram;

};


#endif

