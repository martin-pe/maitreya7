/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/NotebookNodes.h
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

#ifndef _NOTEBOOKNODES_H_
#define _NOTEBOOKNODES_H_

#ifdef __GNUG__
#pragma interface "NotebookNodes.h"
#endif

#include <wx/window.h>

class NotebookNode;
class MultipleChildWindow;
class wxBookCtrlBase;

/*************************************************//**
*
* \brief creates notebooks for MultipleChildWindow
*
******************************************************/
class NotebookNodeContainerFactory
{
public:
	wxBookCtrlBase *createNotebookNodeContainer( wxWindow *parent, MultipleChildWindow*, NotebookNode*,
	        const wxPoint &pos = wxDefaultPosition,	const wxSize size = wxDefaultSize );
};

#endif

