/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/DasaView.cpp
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
#pragma implementation "DasaView.h"
#endif

#include "DasaView.h"

#include <wx/bmpbuttn.h>
#include <wx/imaglist.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/stattext.h>

#include "astrobase.h"
#include "Conf.h"
#include "constants.h"
#include "Dasa.h"
#include "Document.h"
#include "FontConfig.h"
#include "guibase.h"
#include "IconProvider.h"
#include "Lang.h"

extern Config *config;

enum { DASAWINDOW_CTRL = wxID_HIGHEST + 1, DASAWINDOW_PANEL, DASA_CREATE_ENTRY_CHART };

IMPLEMENT_CLASS( DasaView, BasicView )

enum { PIC_DASA = 0, PIC_FOLDER, PIC_DASAROOT, PIC_DASATREE1, PIC_DASATREE2, PIC_DASATREE3,
	PIC_DASATREE4, PIC_DASATREE5, PIC_FOLDERE, PIC_DASAROOTE, PIC_DASATREE1E, PIC_DASATREE2E,
	PIC_DASATREE3E, PIC_DASATREE4E, PIC_DASATREE5E
};

/*************************************************//**
* 
*  \brief bottom panel in Dasa tree view 
* 
******************************************************/
class DasaPanel : public wxPanel
{ 
public:
  
  DasaPanel(wxWindow* parent, const int id )
	 : wxPanel(parent, id )
	{
		button_entry_chart = new wxBitmapButton(this, DASA_CREATE_ENTRY_CHART, wxNullBitmap);
		button_entry_chart->SetBitmapLabel( IconProvider::getBitmap( BITMAP_NEW ));
		button_entry_chart->SetMinSize(wxSize(32, 32));
		button_entry_chart->SetToolTip(_("Create Entry Chart"));

		text_header = new wxStaticText(this, -1, _("dummy"));
		text_header->SetFont( *FontConfig::get()->getHeaderFont());
		text_lord = new wxStaticText(this, -1, _("dummy"));
		text_startdate = new wxStaticText(this, -1, _("dummy"));
		text_enddate = new wxStaticText(this, -1, _("dummy"));
		text_duration = new wxStaticText(this, -1, _("dummy"));

		wxBoxSizer* sizer_main = new wxBoxSizer( wxHORIZONTAL );
		wxBoxSizer* sizer_text = new wxBoxSizer( wxVERTICAL );

		sizer_text->Add(text_header, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);

		wxFlexGridSizer* sizer = new wxFlexGridSizer(4, 2, 3, 3);
		sizer->Add( new  wxStaticText( this, wxID_ANY, _("Lord")), 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
		sizer->Add(text_lord, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);

		sizer->Add( new  wxStaticText( this, wxID_ANY, _("From")), 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
		sizer->Add(text_startdate, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);

		sizer->Add( new  wxStaticText( this, wxID_ANY, _("To")), 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
		sizer->Add(text_enddate, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);

		sizer->Add( new  wxStaticText( this, wxID_ANY, _("Duration")), 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
		sizer->Add(text_duration, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
		sizer->AddGrowableCol(1);

		sizer_text->Add(sizer, 0, wxALL|wxEXPAND, 3);
		sizer_main->Add(sizer_text, 1, wxALL, 3);
		sizer_main->Add(button_entry_chart, 0, wxALL|wxALIGN_RIGHT, 3);

		SetSizer(sizer_main);
		sizer_main->Fit(this);
	}

  void clear()
	{
		text_lord->SetLabel( wxEmptyString );
		text_startdate->SetLabel( wxEmptyString );
		text_enddate->SetLabel( wxEmptyString);
		text_duration->SetLabel( wxEmptyString );;
		enableButton( false );
	}

	void setHeader( wxString s ) { text_header->SetLabel( s ); }
	void setLord( wxString s ) { text_lord->SetLabel( s ); }
	void setStartDate( wxString s ) { text_startdate->SetLabel( s ); }
	void setEndDate( wxString s ) { text_enddate->SetLabel( s ); }
	void setDuration( wxString s ) { text_duration->SetLabel( s ); }
	void enableButton( const bool b ) { button_entry_chart->Enable( b ); }
  
private:

  wxBitmapButton* button_entry_chart;
  wxStaticText* text_header;
  wxStaticText* text_lord;
  wxStaticText* text_startdate;
  wxStaticText* text_enddate;
  wxStaticText* text_duration;

};

/*****************************************************
**
**   DasaView   ---   Constructor
**
******************************************************/
DasaView::DasaView( wxWindow *parent, ChildWindow *frame, Document *doc )
		: BasicView( parent, frame, doc, VIEW_DASA, false, true )
{
	DasaExpert *dexpert;
	wxString dasaname;
	unsigned int i;
	DasaMainListViewItem *item;
	wxTreeItemId id, root, vimsotid;
	FontConfig *f = FontConfig::get();

	setVedic( true );
	currentjd = 0;
	treectrl = new TreeWidget( this, DASAWINDOW_CTRL, wxDefaultPosition, wxDefaultSize,
		wxTR_HAS_BUTTONS|wxSUNKEN_BORDER );
	treectrl->SetFont( *f->getDefaultFont() );
	treectrl->SetIndent( 30 );
	treectrl->AssignImageList( IconProvider::createDasaImageList());

	widget = treectrl;
	showToolPanel( true );

	rootid = treectrl->AddRoot( _( "Dasas" ), PIC_DASA );
	treectrl->SetItemImage( rootid, PIC_DASA, wxTreeItemIcon_Expanded );
	baseid = treectrl->AppendItem( rootid, _( "Standard" ), PIC_FOLDER );
	treectrl->SetItemImage( baseid, PIC_FOLDERE, wxTreeItemIcon_Expanded );
	conditionalid = treectrl->AppendItem( rootid, _( "Conditional" ), PIC_FOLDER );
	treectrl->SetItemImage( conditionalid, PIC_FOLDERE, wxTreeItemIcon_Expanded );
	specialid = treectrl->AppendItem( rootid, _( "Special" ), PIC_FOLDER );
	treectrl->SetItemImage( specialid, PIC_FOLDERE, wxTreeItemIcon_Expanded );
	for ( i = 0; i < DasaExpertFactory::MAX_DASAEXPERTS; i++ )
	{
		if ( i == DASA_VIMSOTTARI || i == DASA_YOGINI || i == DASA_KALACHAKRA ) root = baseid;
		else if ( i >= DASA_ASHTOTTARI && i <= DASA_SHATTRIMSATSAMA ) root = conditionalid;
		else root = specialid;
		dexpert = DasaExpertFactory::get()->getDasaExpert( i );
		dasaname = dexpert->getName();
		item = new DasaMainListViewItem( dexpert, doc, 0 );
		id = treectrl->AppendItem( root, dasaname,  PIC_DASAROOT, PIC_DASAROOT, item );
		if ( i == DASA_VIMSOTTARI ) vimsotid = id;
		treectrl->SetItemImage( id, PIC_DASAROOTE, wxTreeItemIcon_Expanded );
		treectrl->SetItemHasChildren( id );
		treectrl->SetItemBold( id, true );
	}

	Connect( DASAWINDOW_CTRL, wxEVT_COMMAND_TREE_ITEM_EXPANDING, MyTreeEventHandler( DasaView::expand ));
	Connect( DASAWINDOW_CTRL, wxEVT_COMMAND_TREE_ITEM_COLLAPSING, MyTreeEventHandler( DasaView::collapse ));
	Connect( DASAWINDOW_CTRL, wxEVT_COMMAND_TREE_SEL_CHANGING, MyTreeEventHandler( DasaView::OnSelChanging ));
	Connect( DASA_CREATE_ENTRY_CHART, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DasaView::OnEntryChart ));

	treectrl->Expand( rootid );
	treectrl->Expand( baseid );
	treectrl->Expand( vimsotid );
	treectrl->Show( true );
}

/*****************************************************
**
**   DasaView   ---   showToolPanel
**
******************************************************/
void DasaView::showToolPanel( const bool b )
{
	if ( b )
	{
		DasaPanel *dasapanel = new DasaPanel( this, DASAWINDOW_PANEL );
		detailpanel = dasapanel;
	}
	else
	{
		if ( detailpanel ) delete detailpanel;
		detailpanel = 0;
	}
	updateDetailPanel( 0 );
}

/*****************************************************
**
**   DasaView   ---   collapse
**
******************************************************/
void DasaView::collapse( MyTreeEvent& event )
{
	DasaPanel *panel = (DasaPanel*)detailpanel;
	panel->clear();
}

/*****************************************************
**
**   DasaView   ---   OnMouseWheelEvent
**
******************************************************/
void DasaView::handleMouseWheelEvent( wxMouseEvent &event )
{
	int x, y;
	const int offset = 10;
	treectrl->GetViewStart( &x, &y );
	if ( event.GetWheelRotation() < 0 ) y += offset;
	else y -= offset;
	treectrl->Scroll( x, y );
}

/*****************************************************
**
**   DasaView   ---   expand
**
******************************************************/
void DasaView::expand( MyTreeEvent& event )
{
	unsigned int i;
	wxString s;
	vector<Dasa*> divector;
	wxTreeItemId id, id2;
	Dasa *dasa, *dasa2;
	DasaDependendViewItem *ditem;
	int level = 0;
	int pic = PIC_DASATREE1;
	int epic = PIC_DASATREE1E;

	id = event.GetItem();
	MyTreeItemData *data = treectrl->GetItemData( id );

	// Root item
	if ( ! data ) return;

	// Children already there
	if ( treectrl->GetChildrenCount( id ) > 0 ) return;

	DasaListViewItem *dlistitem = (DasaListViewItem*)data;

	DasaExpert *e = dlistitem->getExpert();
	if ( dlistitem->isRootItem())
	{
		divector = e->getFirstLevel( doc, ((DasaMainListViewItem*)dlistitem)->getVarga() );
	}
	else
	{
		dasa = ((DasaDependendViewItem*)dlistitem)->getDasa();
		divector = e->getNextLevel( dasa );
		dasa2 = dasa->getParent();
		while ( dasa2 )
		{
			level++;
			dasa2 = dasa2->getParent();
		}
		switch ( level )
		{
		case 0:
			pic = PIC_DASATREE2;
			epic = PIC_DASATREE2E;
			break;
		case 1:
			pic = PIC_DASATREE3;
			epic = PIC_DASATREE3E;
			break;
		case 2:
			pic = PIC_DASATREE4;
			epic = PIC_DASATREE4E;
			break;
		default:
			pic = PIC_DASATREE5;
			epic = PIC_DASATREE5E;
			break;
		}
	}

	for ( i = 0; i < divector.size(); i++ )
	{
		ditem = new DasaDependendViewItem( e, divector[i], (Horoscope*)doc );
		id2 = treectrl->AppendItem( id, ditem->writeContents(), pic, pic, ditem );
		treectrl->SetItemImage( id2, epic, wxTreeItemIcon_Expanded );
		treectrl->SetItemHasChildren( id2 );
		treectrl->SetItemHidden( id2, (divector[i]->getEndJD() < doc->getJD() ));
	}
}

/*****************************************************
**
**   DasaView   ---   OnKeyDown
**
******************************************************/
void DasaView::OnKeyDown( wxKeyEvent &event )
{
	event.ResumePropagation( wxEVENT_PROPAGATE_MAX );
	event.Skip();
}

/*****************************************************
**
**   DasaView   ---   OnSelChanging
**
******************************************************/
void DasaView::OnSelChanging( MyTreeEvent& event )
{
	wxTreeItemId id = event.GetItem();
	updateDetailPanel( (DasaListViewItem*)treectrl->GetItemData( id ) );
}

/*****************************************************
**
**   DasaView   ---   updateDetailPanel
**
******************************************************/
void DasaView::updateDetailPanel( DasaListViewItem *item )
{
	if ( ! detailpanel ) return;

	const static wxChar *k_dasaname[5] = { _( "Mahadasa" ), _( "Antardasa" ),
		_( "Pratyantardasa" ), _( "Sooksmantardasa" ), _( "Pranadasa" ) };

	Lang lang;
	wxString dasalords, s;

	DasaPanel *panel = (DasaPanel*)detailpanel;

	if ( ! item )
	{
		panel->clear();
		panel->setHeader( _( "Dasa Details" ));
		panel->enableButton( false );
		return;
	}
	DasaExpert *expert = item->getExpert();
	if ( item->isRootItem())
	{
		panel->clear();
		panel->setHeader( wxString::Format( _( "%s Dasa" ), expert->getName()));
		panel->enableButton( false );
	}
	else
	{
		// Dasa/Antardasa type, prepend
		Dasa *dasa = ((DasaDependendViewItem*)item)->getDasa();
		int level = 0;
		while ( dasa )
		{
			if ( level > 0 ) dasalords.Prepend( wxT( "/" ));

			if ( expert->isRasiDasaExpert() )
				dasalords.Prepend( lang.getSignName( dasa->getDasaLord()));
			else
				dasalords.Prepend( lang.getObjectName( dasa->getDasaLord(), TLARGE, true ));
			level++;
			dasa = dasa->getParent();
		}
		panel->setLord( dasalords );

		// Dasa Header, comes after type because level is used
		panel->setHeader( wxString::Format( _( "%s of %s Dasa" ),
			(	level <= 5 ? k_dasaname[level-1] : (const wxChar*)wxString::Format( _( "Antardasa Level %d" ), level )),
			expert->getName()));

		// reset dasa
		dasa = ((DasaDependendViewItem*)item)->getDasa();

		// Start and end of Dasa
		TzUtil tzu;
		const bool isLocaltime = true;
		TzFormattedDate startdate = tzu.getDateFormatted( dasa->getStartJD(), isLocaltime );
		panel->setStartDate( startdate.fullDateTimeTzFormatted );

		TzFormattedDate enddate = tzu.getDateFormatted( dasa->getEndJD(), isLocaltime );
		panel->setEndDate( enddate.fullDateTimeTzFormatted );

		getDasaDuration( s, dasa->getEndJD() - Max( doc->getJD(), dasa->getStartJD()));
		panel->setDuration( s );

		currentjd = dasa->getStartJD();
		panel->enableButton( true );
	}
}

/*****************************************************
**
**   DasaView   ---   OnDataChanged
**
******************************************************/
void DasaView::OnDataChanged()
{
	updateDasa( rootid );
	updateDetailPanel( 0 );
}

/*****************************************************
**
**   DasaView   ---   updateDasa
**
******************************************************/
void DasaView::updateDasa( wxTreeItemId masterid )
{
	wxTreeItemId id;
	MyTreeItemData *data;
	DasaListViewItem *item;
	DasaDependendViewItem *depitem;
	vector<Dasa*> vdasa;
	int i = 0;
	long cook;

	if ( treectrl->GetChildrenCount( masterid ) == 0 ) return;
	data = treectrl->GetItemData( masterid );
	if ( ! data )
	{
		id = treectrl->GetFirstChild( masterid, cook );
		while ( id.IsOk() )
		{
			updateDasa( id );
			id = treectrl->GetNextSibling( id );
		}
		return;
	}
	item = (DasaListViewItem*)data;
	DasaExpert *expert = item->getExpert();

	if ( item->isRootItem() )
		vdasa = expert->getFirstLevel( item->getHoroscope(), ((DasaMainListViewItem*)item)->getVarga() );
	else
		vdasa = expert->getNextLevel( ((DasaDependendViewItem*)item)->getDasa() );

	id = treectrl->GetFirstChild( masterid, cook );
	while ( id.IsOk() )
	{
		depitem = (DasaDependendViewItem*)treectrl->GetItemData( id );
		assert( depitem );
		delete depitem->getDasa();
		depitem->setDasa( vdasa[i] );
		treectrl->SetItemText( id, depitem->writeContents() );
		treectrl->SetItemHidden( id, vdasa[i]->getEndJD() < doc->getJD() );
		updateDasa( id );

		assert( i < 100 && i >= 0 );
		i++;
		id = treectrl->GetNextSibling( id );
	}
}

/*****************************************************
**
**   DasaView   ---   OnEntryChart
**
******************************************************/
void DasaView::OnEntryChart( wxCommandEvent &event )
{
	if ( currentjd == 0 )
	{
		doMessageBox( this, wxT( "Error: can't determine date for entry chart." ));
	}
	else
	{
		Document *d = new Document;
		d->setLocation( *doc->getLocation() );
		d->setDate( currentjd );
		d->setHName( wxString::Format( wxT( "%s: %s" ), (const wxChar*)doc->getHName(), _( "Entry Chart" )));

		wxCommandEvent event( CREATE_ENTRY_CHART, GetParent()->GetId() );
		event.SetEventObject( (wxObject*)d );
		wxPostEvent( GetParent(), event );
	}
}

/**************************************************************
***
**   DasaView   ---   getWindowLabel
***
***************************************************************/
wxString DasaView::getWindowLabel( const bool shortname )
{
	return shortname ? _( "Dasa" ) : _( "Dasa (Tree)" );
	//return _( "Dasa" );
}

/*****************************************************
**
**   DasaListViewItem   ---   Constructor
**
******************************************************/
DasaListViewItem::DasaListViewItem( DasaExpert *e, Horoscope *h )
		: expert( e )
{
	horoscope = h;
}

/*****************************************************
**
**   DasaListViewItem   ---   Destructor
**
******************************************************/
DasaListViewItem::~DasaListViewItem()
{
}

/*****************************************************
**
**   DasaMainListViewItem   ---   Constructor
**
******************************************************/
DasaMainListViewItem::DasaMainListViewItem( DasaExpert *expert, Horoscope *h, int v )
		: DasaListViewItem( expert, h )
{
	varga = v;
}

/*****************************************************
**
**   DasaDependendViewItem   ---   Constructor
**
******************************************************/
DasaDependendViewItem::DasaDependendViewItem( DasaExpert *expert, Dasa *d, Horoscope *h )
		: DasaListViewItem( expert, h ), dasa( d )
{
}

/*****************************************************
**
**   DasaDependendViewItem   ---   writeContents
**
******************************************************/
wxString DasaDependendViewItem::writeContents()
{
	wxString s, t;
	Formatter *formatter = Formatter::get();
	Location *loc = horoscope->getDataSet()->getLocation();
	Lang lang;
	double timediff = loc->getTimeZone() + loc->getDST();
	timediff /= 24;

	double jd = horoscope->getJD() + timediff;

	if ( expert->isRasiDasaExpert() )
	{
		t = lang.getSignName( dasa->getDasaLord());
	}
	else
	{
		t = lang.getObjectName( dasa->getDasaLord(), TLARGE, true );
	}
	if ( dasa->getExtraText().IsEmpty())
		s << formatter->getDateStringFromJD( Max( jd, dasa->getStartJD() )) << wxT( " " ) << t;
	else
		s.Printf( wxT( "%s %s (%s)" ), (const wxChar*)formatter->getDateStringFromJD( Max( jd, dasa->getStartJD() )),
		          (const wxChar*)t, (const wxChar*)dasa->getExtraText() );

	return s;
}

/**************************************************************
***
**   ViewFactory   ---   createDasaView
***
***************************************************************/
BasicView *ViewFactory::createDasaView( wxWindow *parent, ChildWindow *frame, Document *doc )
{
	return new DasaView( parent, frame, doc );
}


