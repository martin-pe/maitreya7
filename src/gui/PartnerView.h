/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/PartnerView.h
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

#ifndef _PARTNERVIEW_H_
#define _PARTNERVIEW_H_

#ifdef __GNUG__
#pragma interface "PartnerView.h"
#endif

#include <vector>
#include "Document.h"
#include "NotebookWidget.h"

using namespace std;

class BasicWidget;
class ChartGridWidget;
class ChartProperties;
class CompositHoroscope;
class NotebookWidget;
class PartnerViewDocListener;
class PartnerViewBaseTextWidget;
class SbcWidget;
class ShadVargaChartGridWidget;
class SplitterWidget;
class TextWidget;
class UranianConfig;
class UranianExpert;
class VargaChart;
class VargaTab2ChartGridWidget;
class VargaTab3ChartGridWidget;
class wxNotebookEvent;

#define MAX_PARTNER_PAGES 8

/*************************************************//**
*
* \brief view for widgets related to partner charts
*
******************************************************/
class PartnerView : public BasicNotebookView
{
	DECLARE_CLASS( PartnerView )
	friend class PartnerViewDocListener;

public:
	PartnerView( wxWindow *parent, ChildWindow *frame, DocumentManager*, const bool &vedic );
	~PartnerView();

	virtual wxString getWindowLabel( const bool shortname = false );
	virtual void postCreate();

	virtual bool isVedic() const;
	virtual bool supportsEWToggle() const;
	virtual bool supportsGraphicExport() const;
	virtual bool supportsFullGraphicStyleToggle() const;
	virtual bool supportsTextExport() const;
	virtual bool supportsSbcStyleToggle() const;
	virtual bool supportsObjectToggle() const;

protected:

	void setupCharts();
	void documentListChanged();
	virtual void OnDataChanged();
	virtual bool dispatchCommand( const int &command );

	void setAllDirty( const bool = true );
	void updatePage( const int &i );
	void OnToolbarCommand();
	void updateToolItems();
	void updatePartnerChoices();
	virtual void initToolItems();

	void OnPartnerChoice( wxCommandEvent& );
	void OnNotebook( wxNotebookEvent& );
	virtual void handleMouseWheelEvent( wxMouseEvent& );

	// double chart
	SplitterWidget *doublesplitter;
	PartnerViewBaseTextWidget *doubletext;
	ChartGridWidget *doublewidget;

	// Vedic
	SplitterWidget *vedicsplitter;
	PartnerViewBaseTextWidget *vedictext;
	ChartGridWidget *vedicwidget;

	// Uranian, no splitter
	PartnerViewBaseTextWidget *uraniantext;

	// Composite
	SplitterWidget *compositesplitter;
	PartnerViewBaseTextWidget *compositetext;
	ChartGridWidget *compositewidget;

	// Sbc
	SbcWidget *sbcwidget;

	// Vargas
	ShadVargaChartGridWidget *shadvargawidget;
	VargaTab2ChartGridWidget *varga2widget;
	VargaTab3ChartGridWidget *varga3widget;

	PartnerViewBaseTextWidget *activetext;
	BasicWidget *activegraphic;
	bool dirty[MAX_PARTNER_PAGES];

	Horoscope *h1, *h2;
	CompositHoroscope *ch;
	UranianConfig *uranian_cfg;

	DocumentManager *docmanager;
	PartnerViewDocListener *listener;
};

/*************************************************//**
*
* \brief manages documents for the PartnerView
*
******************************************************/
class PartnerViewDocListener : public DocumentListener
{
	DECLARE_CLASS( PartnerViewDocListener )

public:

	PartnerViewDocListener( DocumentManager *manager, PartnerView *view );

	~PartnerViewDocListener();

protected:

	void updateAllDocs();
	void documentListChanged();

private:

	DocumentManager *manager;
	PartnerView *view;

};


#endif

