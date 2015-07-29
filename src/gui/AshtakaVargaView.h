/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/AshtakaVargaView.h
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

#ifndef _ASHTAKAVARGAVIEW_H_
#define _ASHTAKAVARGAVIEW_H_

#ifdef __GNUG__
#pragma interface "AshtakaVargaView.h"
#endif

#include "DiagramWidget.h"
#include "NotebookWidget.h"

class AshtakaVargaChartGridWidget;
class AshtakavargaExpert;
class AshtakaVargaPindaDiagram;
class AshtakaVargaRasiDiagram;
class AshtakaVargaTextWidget;
class NotebookWidget;
class TextWidget;
class VargaHoroscope;

class wxNotebook;
class wxNotebookEvent;

/*************************************************//**
*
* \brief view containing Ashtaka Varga charts, diagrams and text view
*
******************************************************/
class AshtakaVargaView : public BasicNotebookView
{
	DECLARE_CLASS( AshtakaVargaView )

public:
	AshtakaVargaView( wxWindow *parent, ChildWindow *frame, Document *doc );
	~AshtakaVargaView();
	virtual wxString getWindowLabel( const bool shortname = false );
	virtual void postCreate();
	virtual void OnDataChanged();
	bool dispatchCommand( const int &command );

	virtual bool supportsGraphicExport() const;
	virtual bool supportsTextExport() const;
	virtual bool supportsSkinToggle() const;
	virtual bool supportsRestrictedGraphicStyleToggle() const;

protected:
	void initToolItems();
	void OnToolbarCommand();

	AshtakaVargaChartGridWidget *rekhagrid, *trikonagrid, *ekadhigrid;
	AshtakaVargaTextWidget *theText;
	AshtakaVargaRasiDiagram *rdiagram;
	AshtakaVargaPindaDiagram *pdiagram;

	AshtakavargaExpert *expert;
	VargaHoroscope *chart;
	int varga;
};


#endif

