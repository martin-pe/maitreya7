/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/NotebookWidget.cpp
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
#pragma implementation "NotebookWidget.h"
#endif

#include "NotebookWidget.h"

#include "BasicWidget.h"
//#include "EventPropagator.h"
#include "SplitterWidget.h"

IMPLEMENT_CLASS( NotebookWidget, wxNotebook )
IMPLEMENT_CLASS( BasicNotebookView, BasicView )

/*****************************************************
**
**   NotebookWidget   ---   Constructor
**
******************************************************/
NotebookWidget::NotebookWidget( wxWindow *parent, const wxWindowID id,	const wxPoint &pos,	const wxSize size,
                                long style, const wxString& name )
		: wxNotebook( parent, id, pos, size, style, name )
{
	Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( NotebookWidget::OnMouseWheelEvent ));
	Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( NotebookWidget::OnKeyDown ));

	//PushEventHandler( new EventPropagator());
}

/*****************************************************
**
**   NotebookWidget   ---   Destructor
**
******************************************************/
NotebookWidget::~NotebookWidget()
{
}

/*****************************************************
**
**   NotebookWidget   ---   OnKeyDown
**
******************************************************/
void NotebookWidget::OnKeyDown( wxKeyEvent &event )
{
	//printf( "KEY DOWN in NotebookWidget::OnKeyDown\n" );
	event.ResumePropagation( wxEVENT_PROPAGATE_MAX );
	event.Skip();
}

/*****************************************************
**
**   NotebookWidget   ---   OnMouseWheelEvent
**
******************************************************/
void NotebookWidget::OnMouseWheelEvent( wxMouseEvent &event )
{
	if ( event.GetEventObject() != this )
	{
		event.Skip();
		return;
	}

	int sel = GetSelection();
	if ( sel == -1 ) return;

	if ( ! GetCurrentPage()->GetRect().Contains( event.GetPosition()))
	{
		if ( event.GetWheelRotation() < 0 ) sel--;
		else sel++;

		if ( sel < 0 ) sel = GetPageCount() - 1;
		else if ( sel == (int)GetPageCount() ) sel = 0;
		SetSelection( sel );
	}
	else
	{
		wxNotebookPage *page = GetPage( sel );
		if ( page->GetClassInfo()->IsKindOf( CLASSINFO( BasicWidget )))
		{
			((BasicWidget*)page)->HandleMouseWheelEvent( event );
		}
		if ( page->GetClassInfo()->IsKindOf( CLASSINFO( SplitterWidget )))
		{
			((SplitterWidget*)page)->handleMouseWheelEvent( event );
		}
	}
}

/*****************************************************
**
**   BasicNotebookView   ---   Constructor
**
******************************************************/
BasicNotebookView::BasicNotebookView( wxWindow *parent, ChildWindow *frame, Document *doc,
                                      const int &viewId, const bool &showToolbar, const bool &vedic, const long style )
		: BasicView( parent, frame, doc, viewId, showToolbar, vedic, style )
{
	activepage = 0;
	Connect( BASIC_NOTEBOOK, wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( BasicNotebookView::OnNotebook ));
}

/*****************************************************
**
**   BasicNotebookView   ---   handleSetFocusEvent
**
******************************************************/
void BasicNotebookView::handleSetFocusEvent()
{
	// needed on Windows, focus is not automatically set
	int sel = notebook->GetSelection();
	notebook->GetPage( sel )->SetFocus();
}

/*****************************************************
**
**   BasicNotebookView   ---   OnMouseWheelEvent
**
******************************************************/
void BasicNotebookView::OnNotebook( wxNotebookEvent &event )
{
	activepage = event.GetSelection();
}


