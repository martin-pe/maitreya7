/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/GraphicView.h
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

#ifndef _GRAPHICVIEW_H_
#define _GRAPHICVIEW_H_

#ifdef __GNUG__
#pragma interface "GraphicView.h"
#endif

#include "BasicView.h"

class ChartGridWidget;

/*************************************************//**
*
* \brief Simple graphical chart view with one chart in western or Vedic mode
*
******************************************************/
class GraphicView : public BasicView
{
public:
	GraphicView( wxWindow *parent, ChildWindow *frame, Document *doc, const int &varga, const bool &vedic );
	virtual wxString getWindowLabel( const bool shortname = false );

	virtual bool dispatchCommand( const int &command );
	virtual void OnDataChanged();

protected:
	ChartGridWidget *gwidget;
	int varga;

private:
	DECLARE_CLASS( GraphicView )
};

#endif

