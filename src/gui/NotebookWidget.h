/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/NotebookWidget.h
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


#ifndef _NOTEBOOKWIDGET_H_
#define _NOTEBOOKWIDGET_H_

#ifdef __GNUG__
#pragma interface "NotebookWidget.h"
#endif

#include "BasicView.h"

#include <wx/notebook.h>

#define BASIC_NOTEBOOK wxID_HIGHEST + 2600

/*************************************************//**
*
* \brief simple widget based on wxNotebook supporting additional mouse and key events
*
******************************************************/
class NotebookWidget : public wxNotebook
{
	DECLARE_CLASS( NotebookWidget )
public:

	NotebookWidget( wxWindow *parent, const wxWindowID = -1, const wxPoint &pos = wxDefaultPosition,
	                const wxSize size = wxDefaultSize, long style = 0, const wxString& name = wxNotebookNameStr );

	~NotebookWidget();

protected:
	void OnMouseWheelEvent( wxMouseEvent& );
	void OnKeyDown( wxKeyEvent& );
};

/*************************************************//**
*
* \brief base class for notebook views
*
******************************************************/
class BasicNotebookView : public BasicView
{
	DECLARE_CLASS( BasicNotebookView )
public:
	BasicNotebookView( wxWindow *parent, ChildWindow *frame, Document *doc, const int &viewId, const bool &showToolbar,
	                   const bool &vedic, const long style = 0 );

protected:

	virtual void handleSetFocusEvent();
	virtual void OnNotebook( wxNotebookEvent& );

	NotebookWidget *notebook;
	int activepage;
};

#endif

