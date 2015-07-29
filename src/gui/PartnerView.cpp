/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/PartnerView.cpp
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
#pragma implementation "PartnerView.h"
#endif

#include "PartnerView.h"

#include <wx/choice.h>
#include <wx/spinctrl.h>
#include <wx/toolbar.h>

#include "Aspect.h"
#include "constants.h"
#include "Conf.h"
#include "DataSet.h"
#include "DialogElements.h"
#include "Document.h"
#include "GraphicGrid.h"
#include "guibase.h"
#include "Lang.h"
#include "NotebookWidget.h"
#include "Partner.h"
#include "SbcView.h"
#include "SplitterWidget.h"
#include "TextWidget.h"
#include "Uranian.h"
#include "Varga.h"
#include "VargaHoroscope.h"
#include "VedicChart.h"
#include "Writer.h"

extern Config *config;

// PNB = Partner Notebook
enum { PNB_DOUBLE = 0, PNB_VEDIC, PNB_COMPOSITE, PNB_SBC, PNB_SHADVARGA, PNB_VARGA2, PNB_VARGA3, PNB_URANIAN };

IMPLEMENT_CLASS( PartnerView, BasicView )
IMPLEMENT_CLASS( PartnerViewDocListener, DocumentListener )

/*************************************************//**
*
*
*
******************************************************/
class PartnerViewBaseTextWidget : public TextWidget
{
	DECLARE_CLASS( PartnerViewBaseTextWidget )

public:
	PartnerViewBaseTextWidget( wxWindow *parent, BasicView *v )
			: TextWidget( parent, v )
	{
		h1 = h2 = 0;
	}

	virtual int writeContents()
	{
		int ret = 0;
		writer->beginWriting();
		if ( ! h1 || ! h2 ) // only one chart not set
		{
			writer->writeLine( _( "No chart loaded" ));
		}
		else if ( h1 == h2 )
		{
			writer->writeLine( _( "Charts identical" ));
		}
		else
		{
			ret = write();
		}
		writer->endWriting();
		setContents( writer->getContents());
		return ret;
	}

	Horoscope *h1, *h2;

protected:

	virtual int write() = 0;
};

IMPLEMENT_CLASS( PartnerViewBaseTextWidget, TextWidget )

/*************************************************//**
*
*
*
******************************************************/
class PartnerViewUranianTextWidget : public PartnerViewBaseTextWidget
{
	DECLARE_CLASS( PartnerViewUranianTextWidget )

public:
	PartnerViewUranianTextWidget( wxWindow *parent, BasicView *v, UranianConfig *cfg )
			: PartnerViewBaseTextWidget( parent, v )
	{
		this->cfg = cfg;
		expert = new UranianExpert( v, writer, *cfg );
	}

	~PartnerViewUranianTextWidget()
	{
		delete expert;
	}

	virtual int write()
	{
		if ( h1 && h2 ) expert->writePartnerAnalysis( h1, h2 );
		return 0;
	}

private:
	UranianConfig * cfg;
	UranianExpert *expert;
};

IMPLEMENT_CLASS( PartnerViewUranianTextWidget, PartnerViewBaseTextWidget )

/*************************************************//**
*
*
*
******************************************************/
class PartnerViewDoubleTextWidget : public PartnerViewBaseTextWidget
{
	DECLARE_CLASS( PartnerViewDoubleTextWidget )

public:
	PartnerViewDoubleTextWidget( wxWindow *parent, BasicView *v )
			: PartnerViewBaseTextWidget( parent, v )
	{
	}

	virtual int write()
	{
		if ( h1 && h2 )
		{
			AspectExpert e( h1 );
			if ( mview->isVedic())
			{
				e.writeVedicAspectarium( h2, writer, mview, config->showTextViewHeaders );
			}
			else
			{
				e.update( h2, mview );
				e.writeWesternAspectarium( writer, mview );
			}
		}
		return 0;
	}
};

IMPLEMENT_CLASS( PartnerViewDoubleTextWidget, PartnerViewBaseTextWidget )

/*************************************************//**
*
*
*
******************************************************/
class PartnerViewCompositeTextWidget : public PartnerViewBaseTextWidget
{
	DECLARE_CLASS( PartnerViewCompositeTextWidget )

public:
	PartnerViewCompositeTextWidget( wxWindow *parent, BasicView *v, CompositHoroscope *ch )
			: PartnerViewBaseTextWidget( parent, v )
	{
		this->ch = ch;
	}

	virtual int write()
	{
		if ( ch && h1 && h2 )
		{
			ch->dump( writer, mview, config->showTextViewHeaders );
		}
		return 0;
	}

private:

	CompositHoroscope *ch;

};

IMPLEMENT_CLASS( PartnerViewCompositeTextWidget, PartnerViewBaseTextWidget )

/*************************************************//**
*
*
*
******************************************************/
class PartnerViewVedicTextWidget : public PartnerViewBaseTextWidget
{
	DECLARE_CLASS( PartnerViewVedicTextWidget )

public:
	PartnerViewVedicTextWidget( wxWindow *parent, BasicView *v )
			: PartnerViewBaseTextWidget( parent, v )
	{
	}

	virtual int write()
	{
		if ( h1 && h2 )
		{
			VedicPartnerChart partnerchart( h1, h2 );
			partnerchart.update();
			partnerchart.write( writer );
		}
		return 0;
	}
};

IMPLEMENT_CLASS( PartnerViewVedicTextWidget, PartnerViewBaseTextWidget )

/*****************************************************
**
**   PartnerView   ---   Constructor
**
******************************************************/
PartnerView::PartnerView( wxWindow *parent, ChildWindow *frame, DocumentManager *manager, const bool &vedic )
		: BasicNotebookView( parent, frame, 0, VIEW_PARTNER, true, vedic )
{
	docmanager = manager;
	h1 = h2 = 0;

	setBlank( true );

	listener = new PartnerViewDocListener( manager, this );
	notebook = new NotebookWidget( this, BASIC_NOTEBOOK, wxDefaultPosition, wxDefaultSize, wxNB_DEFAULT );

	for ( int i = 0; i < MAX_PARTNER_PAGES; i++ )
	{
		switch ( i )
		{
		case PNB_DOUBLE:
			doublesplitter = new SplitterWidget( notebook );
			doubletext = new PartnerViewDoubleTextWidget( doublesplitter, this );
			doublewidget = new ChartGridWidget( doublesplitter, this, 1, 1 );
			doublesplitter->SplitVertically( doubletext, doublewidget );
			notebook->AddPage( doublesplitter, _( "Double Chart" ));
			break;

		case PNB_VEDIC:
			vedicsplitter = new SplitterWidget( notebook );
			vedictext = new PartnerViewVedicTextWidget( vedicsplitter, this );
			vedicwidget = new ChartGridWidget( vedicsplitter, this, 2, 2 );
			vedicsplitter->SplitVertically( vedictext, vedicwidget );
			notebook->AddPage( vedicsplitter, _( "Vedic PartnerChart" ));
			break;

		case PNB_COMPOSITE:
			ch = new CompositHoroscope();
			compositesplitter = new SplitterWidget( notebook );
			compositetext = new PartnerViewCompositeTextWidget( compositesplitter, this, ch );
			compositewidget = new ChartGridWidget( compositesplitter, this, 2, 2 );
			compositesplitter->SplitVertically( compositetext, compositewidget );
			notebook->AddPage( compositesplitter, _( "Composite Chart" ));
			break;

		case PNB_SBC:
			sbcwidget = new SbcWidget( notebook, this, 0, 0 );
			notebook->AddPage( sbcwidget, _( "Sarvatobhadra" ));
			break;

		case PNB_SHADVARGA:
			shadvargawidget = new ShadVargaChartGridWidget( notebook, this, h1, h2 );
			notebook->AddPage( shadvargawidget, _( "Shadvarga" ));
			break;

		case PNB_VARGA2:
			varga2widget = new VargaTab2ChartGridWidget( notebook, this, h1, h2 );
			notebook->AddPage( varga2widget, _( "Varga (2)" ));
			break;

		case PNB_VARGA3:
			varga3widget = new VargaTab3ChartGridWidget( notebook, this, h1, h2 );
			notebook->AddPage( varga3widget, _( "Varga (3)" ));
			break;

		case PNB_URANIAN:
			uranian_cfg = new UranianConfig;
			uranian_cfg->orbis = (double)(config->orbisPartner) / 60.0;
			uraniantext = new PartnerViewUranianTextWidget( notebook, this, uranian_cfg );
			notebook->AddPage( uraniantext, _( "Uranian" ));
			break;

		default:
			assert( false );
			break;
		}
	}

	widget = notebook;
	updatePartnerChoices();
	initToolItems();
	OnDataChanged();
	SetSize( config->xPartnerWindow, config->yPartnerWindow );

	Connect( TBS_PARTNER1, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( PartnerView::OnPartnerChoice ));
	Connect( TBS_PARTNER2, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( PartnerView::OnPartnerChoice ));
	Connect( TBS_ORBIS, wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( PartnerView::OnSpinCommand ));
	Connect( TBS_SORT, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( PartnerView::OnChoiceCommand ));
	Connect( TBS_FILTER, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( PartnerView::OnChoiceCommand ));
	Connect( TBS_CIRCLETYPE, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( PartnerView::OnChoiceCommand ));
	Connect( BASIC_NOTEBOOK, wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( PartnerView::OnNotebook ));
}

/*****************************************************
**
**   PartnerView   ---   Destructor
**
******************************************************/
PartnerView::~PartnerView()
{
	// Convencience stuff
	config->sashPartnerDouble = doublesplitter->GetSashPosition();
	config->sashPartnerVedic = vedicsplitter->GetSashPosition();
	config->sashPartnerComposite = compositesplitter->GetSashPosition();
	config->avtivePagePartner = activepage;
	config->orbisPartner = (int)(60 * uranian_cfg->orbis);
	config->circleType = uranian_cfg->circle;
	config->planetSortOrder = uranian_cfg->order;

	delete uranian_cfg;
	delete ch;
	docmanager->deleteClient( listener );
}

/*****************************************************
**
**   PartnerView   ---   postCreate
**
******************************************************/
void PartnerView::postCreate()
{
	assert( notebook );
	assert( compositesplitter );
	assert( doublesplitter );
	assert( vedicsplitter );

	doublesplitter->SetSashPosition( config->sashPartnerDouble );
	compositesplitter->SetSashPosition( config->sashPartnerComposite );
	vedicsplitter->SetSashPosition( config->sashPartnerVedic );
	notebook->SetSelection( config->avtivePagePartner );
}

/*****************************************************
**
**   PartnerView   ---   initToolItems
**
******************************************************/
void PartnerView::initToolItems()
{
	if ( toolbar )
	{
		wxSpinCtrl *spin_orbis = (wxSpinCtrl*)toolbar->FindControl( TBS_ORBIS );
		if ( spin_orbis ) spin_orbis->SetValue( 60 * uranian_cfg->orbis );

		wxChoice *choice_sort = (wxChoice*)toolbar->FindControl( TBS_SORT );
		if ( choice_sort ) choice_sort->SetSelection( uranian_cfg->order );

		FilterChoice *choice_filter = (FilterChoice*)toolbar->FindControl( TBS_FILTER );
		if ( choice_filter ) choice_filter->SetSelection( uranian_cfg->filter );

		CircleTypeChoice *choice_circle = (CircleTypeChoice*)toolbar->FindControl( TBS_CIRCLETYPE );
		if ( choice_circle ) choice_circle->SetSelection( uranian_cfg->circle );
	}
}

/*****************************************************
**
**   PartnerView   ---   OnToolbarCommand
**
******************************************************/
void PartnerView::OnToolbarCommand()
{
	if ( toolbar )
	{
		setupCharts();
		wxSpinCtrl *spin_orbis = (wxSpinCtrl*)toolbar->FindControl( TBS_ORBIS );
		if ( spin_orbis ) uranian_cfg->orbis = (double)(spin_orbis->GetValue()) / 60.0;

		wxChoice *choice_sort = (wxChoice*)toolbar->FindControl( TBS_SORT );
		if ( choice_sort ) uranian_cfg->order = choice_sort->GetSelection();

		FilterChoice *choice_filter = (FilterChoice*)toolbar->FindControl( TBS_FILTER );
		if ( choice_filter ) uranian_cfg->filter = choice_filter->GetSelection();

		CircleTypeChoice *choice_circle = (CircleTypeChoice*)toolbar->FindControl( TBS_CIRCLETYPE );
		if ( choice_circle ) uranian_cfg->circle = choice_circle->GetSelection();
	}
	OnDataChanged();
}

/*****************************************************
**
**   PartnerView   ---   updateToolItems
**
******************************************************/
void PartnerView::updateToolItems()
{
	if ( ! toolbar ) return;
	const bool showExtraItems = ( activepage == PNB_URANIAN );

	wxSpinCtrl *spin_orbis = (wxSpinCtrl*)toolbar->FindControl( TBS_ORBIS );
	if ( spin_orbis ) spin_orbis->Enable( showExtraItems );

	wxChoice *choice_sort = (wxChoice*)toolbar->FindControl( TBS_SORT );
	if ( choice_sort ) choice_sort->Enable( showExtraItems );

	FilterChoice *choice_filter = (FilterChoice*)toolbar->FindControl( TBS_FILTER );
	if ( choice_filter ) choice_filter->Enable( showExtraItems );

	CircleTypeChoice *choice_circle = (CircleTypeChoice*)toolbar->FindControl( TBS_CIRCLETYPE );
	if ( choice_circle ) choice_circle->Enable( showExtraItems );
}

/**************************************************************
***
**    PartnerView   ---   getWindowLabel
***
***************************************************************/
wxString PartnerView::getWindowLabel( const bool shortname )
{
	return shortname ? _( "Partner" ) : _( "Partner View" );
}

/*****************************************************
**
**   PartnerView   ---   OnNotebook
**
******************************************************/
void PartnerView::OnNotebook( wxNotebookEvent &event )
{
	activepage = event.GetSelection();
	assert( activepage >= 0 && activepage < MAX_PARTNER_PAGES );

	switch ( activepage )
	{
	case PNB_DOUBLE:
		activetext = doubletext;
		activegraphic = doublewidget;
		break;
	case PNB_VEDIC:
		activetext = vedictext;
		activegraphic = vedicwidget;
		break;
	case PNB_COMPOSITE:
		activetext = compositetext;
		activegraphic = compositewidget;
		break;
	case PNB_URANIAN:
		activetext = uraniantext;
		activegraphic = 0;
		break;
	case PNB_SBC:
		activetext = 0;
		activegraphic = sbcwidget;
		break;
	case PNB_SHADVARGA:
		activetext = 0;
		activegraphic = shadvargawidget;
		break;
	case PNB_VARGA2:
		activetext = 0;
		activegraphic = varga2widget;
		break;
	case PNB_VARGA3:
		activetext = 0;
		activegraphic = varga3widget;
		break;
	default:
		assert( false );
		break;
	}
	if ( dirty[activepage] ) updatePage( activepage );
	updateToolItems();
}

/*****************************************************
**
**   PartnerView   ---   setAllDirty
**
******************************************************/
void PartnerView::setAllDirty( const bool b )
{
	for ( int i = 0; i < MAX_PARTNER_PAGES; i++ ) dirty[i] = b;
	doublewidget->deleteVargaCharts();
	compositewidget->deleteVargaCharts();
	vedicwidget->deleteVargaCharts();
	shadvargawidget->setCharts( 0, 0 );
	varga2widget->setCharts( 0, 0 );
	varga3widget->setCharts( 0, 0 );
}

/*****************************************************
**
**   PartnerView   ---   updatePage
**
******************************************************/
void PartnerView::updatePage( const int &page )
{
	assert( page >= 0 && page < MAX_PARTNER_PAGES );

	setBlank( ! h1 || ! h2 || h1 == h2 );

	if ( activetext )
	{
		activetext->h1 = h1;
		activetext->h2 = h2;
	}

	if ( ! isBlank() )
	{
		switch ( page )
		{
		case PNB_DOUBLE: // writeContents below
		{
			VedicVargaChart *vch = new VedicVargaChart( new VargaHoroscope( h1, VARGA_RASI ), new VargaHoroscope( h2, VARGA_RASI ), this );
			doublewidget->addVedicVargaChart( vch );
			doublewidget->Refresh();
		}
		break;

		case PNB_VEDIC: // writeContents below
		{
			vedicwidget->addVedicVargaChart( h1, VARGA_RASI );
			vedicwidget->addVedicVargaChart( h1, VARGA_NAVAMSA );
			vedicwidget->addVedicVargaChart( h2, VARGA_RASI );
			vedicwidget->addVedicVargaChart( h2, VARGA_NAVAMSA );
			VedicPartnerChart partnerchart( h1, h2 );
			partnerchart.update();
			vedicwidget->Refresh();
		}
		break;

		case PNB_COMPOSITE: // writeContents below
			ch->update( h1, h2 );
			compositewidget->addVedicVargaChart( ch, VARGA_RASI );
			compositewidget->addVedicVargaChart( ch, VARGA_BHAVA );
			compositewidget->addVedicVargaChart( ch, VARGA_NAVAMSA );
			compositewidget->addVedicVargaChart( ch, VARGA_TRIMSAMSA );
			compositewidget->Refresh();
			compositesplitter->Refresh();
			// das ist es nicht...
			compositewidget->Refresh();
			break;

		case PNB_URANIAN: // writeContents below
			break;

		case PNB_SBC:
		{
			sbcwidget->setCharts( h1, h2 );
			sbcwidget->OnDataChanged();
		}
		break;

		case PNB_SHADVARGA:
			shadvargawidget->setCharts( h1, h2 );
			shadvargawidget->OnDataChanged();
			break;

		case PNB_VARGA2:
			varga2widget->setCharts( h1, h2 );
			varga2widget->OnDataChanged();
			break;

		case PNB_VARGA3:
			varga3widget->setCharts( h1, h2 );
			varga3widget->OnDataChanged();
			break;

		default:
			assert( false );
			break;
		}
	}
	dirty[page] = false;
	if ( activetext ) activetext->writeContents();
	if ( activegraphic ) activegraphic->Refresh();
}

/*****************************************************
**
**   PartnerView   ---   OnDataChanged
**
******************************************************/
void PartnerView::OnDataChanged()
{
	setAllDirty();
	if ( toolbar )
	{
		FilterChoice *choice_filter = (FilterChoice*)toolbar->FindControl( TBS_FILTER );
		assert( choice_filter );
		choice_filter->recreate( getPlanetList());
	}
	updatePage( activepage );
}

/*****************************************************
**
**   PartnerView   ---   setupCharts
**
******************************************************/
void PartnerView::setupCharts()
{
	int sel1 = -1, sel2 = -1;

	wxChoice *choice_partner1 = (wxChoice*)toolbar->FindControl( TBS_PARTNER1 );
	if ( choice_partner1 ) sel1 = choice_partner1->GetSelection();

	wxChoice *choice_partner2 = (wxChoice*)toolbar->FindControl( TBS_PARTNER2 );
	if ( choice_partner2 ) sel2 = choice_partner2->GetSelection();
	if ( sel1 > -1 ) h1 = docmanager->getDocument( sel1 );
	else h1 = 0;
	if ( sel2 > -1 ) h2 = docmanager->getDocument( sel2 );
	else h2 = 0;
}

/*****************************************************
**
**   PartnerView   ---   OnPartnerChoice
**
******************************************************/
void PartnerView::OnPartnerChoice( wxCommandEvent &event )
{
	setupCharts();
	OnDataChanged();
}

/*****************************************************
**
**   PartnerView   ---   isVedic
**
******************************************************/
bool PartnerView::isVedic() const
{
	if ( ! supportsEWToggle() ) return true;
	else return ChartProperties::isVedic();
}

/*****************************************************
**
**   PartnerView   ---   supportsEWToggle
**
******************************************************/
bool PartnerView::supportsEWToggle() const
{
	if ( isBlank() ) return false;
	return( activepage == PNB_DOUBLE || activepage == PNB_COMPOSITE || activepage == PNB_URANIAN );
}

/*****************************************************
**
**   PartnerView   ---   supportsGraphicExport
**
******************************************************/
bool PartnerView::supportsGraphicExport() const
{
	return( ! isBlank() && activepage != PNB_URANIAN );
}

/*****************************************************
**
**   PartnerView   ---   supportsTextExport
**
******************************************************/
bool PartnerView::supportsTextExport() const
{
	if ( isBlank()) return false;
	return( activepage ==	PNB_DOUBLE || activepage == PNB_VEDIC
	        || activepage == PNB_COMPOSITE || activepage == PNB_URANIAN );
}

/*****************************************************
**
**   PartnerView   ---   supportsFullGraphicStyleToggle
**
******************************************************/
bool PartnerView::supportsFullGraphicStyleToggle() const
{
	return( ! isBlank() && activepage != PNB_URANIAN );
}

/*****************************************************
**
**   PartnerView   ---   supportsFullGraphicStyleToggle
**
******************************************************/
bool PartnerView::supportsSbcStyleToggle() const
{
	return( ! isBlank() &&  activepage == PNB_SBC );
}

/*****************************************************
**
**   PartnerView   ---   supportsFullGraphicStyleToggle
**
******************************************************/
bool PartnerView::supportsObjectToggle() const
{
	return( ! isBlank());
}
//enum { PNB_DOUBLE = 0, PNB_VEDIC, PNB_COMPOSITE, PNB_SBC, PNB_SHADVARGA, PNB_VARGA2, PNB_VARGA3, PNB_URANIAN };

/*****************************************************
**
**   PartnerView   ---   handleMouseWheelEvent
**
******************************************************/
void PartnerView::handleMouseWheelEvent( wxMouseEvent &event)
{
	if ( activepage == PNB_COMPOSITE )
	{
		changeSkin( event.GetWheelRotation() > 0 );
		Refresh();
	}
	else if ( activepage == PNB_VEDIC )
	{
		changeSkin( event.GetWheelRotation() > 0, true );
		Refresh();
	}
	else if ( activepage == PNB_URANIAN )
	{
		assert( activetext );
		activetext->scroll( event.GetWheelRotation() );
	}
	else event.Skip();
}

/*****************************************************
**
**   PartnerView   ---   dispatchCommand
**
******************************************************/
bool PartnerView::dispatchCommand( const int &command )
{
	switch ( command )
	{
	case CMD_CHILD_EXPORT:
		if ( activegraphic ) activegraphic->doExport();
		else printf( "Error: no graphic export possible on page %d\n", activepage );
		break;
	case CMD_CHILD_SAVETEXT:
		if ( activetext ) activetext->doExport();
		else printf( "Error: no text export possible on page %d\n", activepage );
		break;
	default:
		return BasicView::dispatchCommand( command );
		break;
	}
	return true;
}

/*****************************************************
**
**   PartnerView   ---   updatePartnerChoices
**
******************************************************/
void PartnerView::updatePartnerChoices()
{
	if ( ! toolbar ) return;
	int sel1 = -1, sel2 = -1;
	wxString docname;

	int docnum = docmanager->getNbDocuments();
	wxChoice *choice_partner1 = (wxChoice*)toolbar->FindControl( TBS_PARTNER1 );
	if ( choice_partner1 ) sel1 = choice_partner1->GetSelection();

	wxChoice *choice_partner2 = (wxChoice*)toolbar->FindControl( TBS_PARTNER2 );
	if ( choice_partner2 ) sel2 = choice_partner2->GetSelection();
	if ( sel2 >= docnum ) sel2 = sel1;

	if ( sel1 == -1 && docnum > 0 ) sel1 = 0;
	if ( ( sel2 == sel1 || sel2 == -1 ) && docnum > 0 ) sel2 = docnum - 1;

	choice_partner1->Clear();
	choice_partner2->Clear();

	int i = 1;
	for ( i = 0; i < docnum; i++ )
	{
		docname = docmanager->getDocument( i )->getHName();
		choice_partner1->Append( docname );
		choice_partner2->Append( docname );
	}
	choice_partner1->SetSelection( sel1 );
	choice_partner2->SetSelection( sel2 );
	if ( sel1 != -1 && docnum > 0 ) h1 = docmanager->getDocument( Min( sel1, docnum ));
	else h1 = 0;
	if ( sel2 != -1 && docnum > 0 ) h2 = docmanager->getDocument( Min( sel2, docnum ));
	else h2 = 0;
}

/*****************************************************
**
**   PartnerView   ---   documentListChanged
**
******************************************************/
void PartnerView::documentListChanged()
{
	updatePartnerChoices();
	OnDataChanged();
}

/*****************************************************
**
**   PartnerViewDocListener   ---   Constructor
**
******************************************************/
PartnerViewDocListener::PartnerViewDocListener( DocumentManager *manager, PartnerView *view )
		:  DocumentListener( manager )
{
	this->manager = manager;
	this->view = view;
	manager->addClient( this );
}

/*****************************************************
**
**   PartnerViewDocListener   ---   Destructor
**
******************************************************/
PartnerViewDocListener::~PartnerViewDocListener()
{
	manager->deleteClient( this );
}

/*****************************************************
**
**   PartnerViewDocListener   ---   updateAllDocs
**
******************************************************/
void PartnerViewDocListener::updateAllDocs()
{
}

/*****************************************************
**
**   PartnerViewDocListener   ---   documentListChanged
**
******************************************************/
void PartnerViewDocListener::documentListChanged()
{
	view->documentListChanged();
}

/**************************************************************
***
**    ViewFactory   ---   createPartnerView
***
***************************************************************/
BasicView *ViewFactory::createPartnerView( wxWindow *parent, ChildWindow *frame, DocumentManager *manager, const bool &vedic )
{
	return new PartnerView( parent, frame, manager, vedic );
}


