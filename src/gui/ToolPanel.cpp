/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/ToolPanel.cpp
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
#pragma implementation "ToolPanel.h"
#endif

#include "ToolPanel.h"

#include <wx/app.h>
#include <wx/datectrl.h>
#include <wx/log.h>
#include <wx/panel.h>
#include <wx/slider.h>
#include <wx/spinctrl.h>
#include <wx/stattext.h>
#include <wx/tokenzr.h>

#include "DateTimeCtrl.h"
#include "DialogElements.h"
#include "guibase.h"
#include "IconProvider.h"

#include "Commands.h"
#include "Conf.h"
#include "Dasa.h"
#include "Lang.h"
#include "Varga.h"

DEFINE_EVENT_TYPE( TOOLPANEL_CHANGED )
DEFINE_EVENT_TYPE( TOOLPANEL_CREATEDOC )

extern Config *config;


#define TB_STYLE wxTB_FLAT | wxTB_DOCKABLE | wxWANTS_CHARS | wxTAB_TRAVERSAL

/*****************************************************
**
**   MaitreyaToolbar   ---   Constructor
**
******************************************************/
MaitreyaToolbar::MaitreyaToolbar( wxWindow* parent )
		: wxToolBar( parent, -1, wxDefaultPosition, wxDefaultSize, TB_STYLE )
{
	//Connect( wxEVT_CHAR, wxKeyEventHandler( MaitreyaToolbar::OnChar ));
}

/*****************************************************
**
**   MaitreyaToolbar   ---   recreate
**
******************************************************/
void MaitreyaToolbar::recreate()
{
	ToolbarFactory *factory = ToolbarFactory::get();
	ClearTools();

	vector<ToolBarCommand> commands;
	if ( config->mainToolbarItems.IsEmpty())
	{
		commands = factory->getDefaultCommandList();
	}
	else
	{
		commands = factory->getCommandListFromString( config->mainToolbarItems );
	}

	// params
	SetToolBitmapSize( wxSize( 24, 24 ));
	for ( unsigned int i = 0; i < commands.size(); i++ )
	{
		if ( commands[i].commandId != 0 )
			AddTool( commands[i].commandId, commands[i].description, IconProvider::getBitmap( commands[i].bitmapId ), commands[i].description );
		else AddSeparator();
	}
}

/*****************************************************
**
**   MaitreyaToolbar   ---   OnChar
**
******************************************************/
void MaitreyaToolbar::OnChar( wxKeyEvent &event )
{
	printf( "Toolbar ONCHAR %d control %d\n", event.GetKeyCode(), event.m_controlDown );
}

/*****************************************************
**
**   MaitreyaToolbar   ---   updateTools
**
******************************************************/
void MaitreyaToolbar::updateTools( const bool &isDoc )
{
	for ( int i = CMD_CHILD_FIRST + 1; i < CMD_CHILD_LAST; i++ ) EnableTool( i, isDoc );
}


/*****************************************************
**
**   TOOLBAR DEFINITIONS
**
******************************************************/
const int tb_horawindow[] = { TBS_DATE, TBB_NOW, TBS_TZ, -1 };
const int tb_textwindow[] = { TBS_TEXTMODE, TBS_VARGA, TBS_DASA, -1 };
const int tb_ashtakavargawindow[] = { TBS_VARGA, -1 };
const int tb_eclipsewindow[] = { TBS_YEAR, TBS_NBYEARS, TBS_ECL_TYPE, TBS_TZ, -1 };
const int tb_transitwindow[] = { TBS_DATE, TBS_TIME, TBB_NOW, TBS_TZ, TBS_DATE_PROGRESS, TB_SEPARATOR, TBS_TRANSITMODE, TBS_YEARLENGTH,
                                 TBS_SORT, TBS_ORBIS, TBS_CIRCLETYPE, -1
                               };
const int tb_uranianwindow[] = { TBS_URANIANMODE, TBS_SORT, TBS_FILTER, TBS_ORBIS, TBS_CIRCLETYPE,
                                 TBS_YEAR, TBS_YEARLENGTH, -1
                               };
const int tb_gdasawindow[] = { TBB_GOTO, TBB_NOW, TBB_DASA, TB_SEPARATOR, TBS_ZOOM, TBS_ANTARDASALEVEL, TB_TEXT, -1 };
const int tb_ephemwindow[] = { TBS_YEAR, TBS_MONTH, TBB_NOW, TB_SEPARATOR, TBS_EPHEMMODE, TBS_TZ, TBS_DASA,
                               TBS_EPHEMDEG, -1
                             };
const int tb_solarwindow[] = { TBS_YEAR, TBB_NOW, TBB_NEW, TB_TEXT, -1 };
const int tb_partnerwindow[] = { TBS_PARTNER1 , TBS_PARTNER2, TBS_ORBIS, TBS_SORT, TBS_FILTER, TBS_CIRCLETYPE, -1 };

// Controls: Varga, Filter, 1st house
const int tb_yogawindow[] = { TBS_VARGA, TBS_YOGAGROUPFILTER, TBS_YOGASOURCEFILTER, TBS_YOGAFIRSTHOUSE, -1 };

const int tb_yogaeditor[] = { TBB_NEW, TBB_OPEN, TBB_SAVE, TBB_SAVEAS, TBB_CLOSE, -1 };

ToolbarFactory *ToolbarFactory::ego = 0;

/*****************************************************
**
**   ToolbarFactory   ---    Constructor
**
******************************************************/
ToolbarFactory::ToolbarFactory()
{
	init();
}

/*****************************************************
**
**   ToolbarFactory   ---    get
**
******************************************************/
ToolbarFactory *ToolbarFactory::get()
{
	if ( ! ego ) ego = new ToolbarFactory();
	return ego;
}

/*****************************************************
**
**   ToolbarFactory   ---    createToolbar
**
******************************************************/
wxToolBar *ToolbarFactory::createToolbar( wxWindow *parent, const int &type )
{
	toolbar = new wxToolBar( parent, -1, wxDefaultPosition, wxDefaultSize, TB_STYLE );
	toolbar->SetToolBitmapSize( wxSize( 24, 24 ));
	toolbar->SetMargins( 0, 0 );
	toolbar->SetToolSeparation( 0 );
	const int *a = 0;
	switch ( type )
	{
	case VIEW_URANIAN:
		a = tb_uranianwindow;
		break;
	case VIEW_TRANSIT:
		a = tb_transitwindow;
		break;
	case VIEW_HORA:
		a = tb_horawindow;
		break;
	case VIEW_TEXT:
		a = tb_textwindow;
		break;
	case VIEW_ASHTAKAVARGA:
		a = tb_ashtakavargawindow;
		break;
	case VIEW_SOLAR:
		a = tb_solarwindow;
		break;
	case VIEW_YOGA:
		a = tb_yogawindow;
		break;
	case VIEW_YOGA_EDITOR:
		a = tb_yogaeditor;
		break;
	case VIEW_GRAPHICALDASA:
		a = tb_gdasawindow;
		break;
	case VIEW_ECLIPSE:
		a = tb_eclipsewindow;
		break;
	case VIEW_EPHEM:
		a = tb_ephemwindow;
		break;
	case VIEW_PARTNER:
		a = tb_partnerwindow;
		break;
	}
	if ( a ) create( a );
	else printf( "Warning: no toolbar configuration for id#%d found\n", type );
	toolbar->Realize();
	return toolbar;
}

/*****************************************************
**
**   ToolbarFactory   ---    create
**
******************************************************/
void ToolbarFactory::create( const int *a )
{
	const int *i = a; // a is int array
	while ( *i != -1 )
	{
		if ( *i < TBS_LAST ) createChoiceItem( *i );
		else if ( *i == TB_SEPARATOR ) toolbar->AddSeparator();
		else if ( *i == TB_TEXT ) createTextField();
		else createButtonItem( *i );
		i++;
	}
}

/*****************************************************
**
**   ToolbarFactory   ---    createTextField
**
******************************************************/
void ToolbarFactory::createTextField()
{
	wxControl *control = 0;
	control = (wxControl*) new ToolbarLabel( toolbar, TB_TEXT, wxPoint( 300, 300 ), wxSize( 300, 24 ), wxEmptyString );
	toolbar->AddControl( control );
}

/*****************************************************
**
**   ToolbarFactory   ---    createChoiceItem
**
******************************************************/
void ToolbarFactory::createChoiceItem( const int &item )
{
	const static wxString text_viewmodes[] =
	{
		_("Base Data"),
		_("Arabic Parts"),
		_("Ashtaka Varga"),
		_("Astronomical"),
		_("Bhava"),
		_("Dasa (Short)"),
		_("Dasa (Compact)"),
		_("Dasa (Long)"),
		_("Jaimini"),
		_("Krishnamurti Paddhati"),
		_("Nakshatra"),
		_("Vargas"),
		_("Vedic Aspectarium"),
		_("Vedic Planets"),
		_("Western Aspectarium"),
		_("Western Planets")
	};
	const static wxString eclipse_types[] = { _("All"), _("Solar"), _("Lunar") };
	const static wxString uranian_viewmodes[] = { _("Midpoint Summary"), _("Yearly Preview") };
	const static wxString zoom_choices[] = { wxT("10%"), wxT("25%"), wxT("50%"), wxT("100%"),
		wxT("150%"), wxT("200%"), wxT("250%"), wxT("300%"), wxT("400%"), wxT("500%")
	                                       };
	const static wxString antardasa_choices[] = { wxT("Mahadasa"), wxT("Antardasa"), wxT("Pratyantardasa") };
	const static wxString ephem_mode_choices[] = { _("Default"), _("Graphical"), _("Sunrise/Tithi"),
		_("Ingress"), _( "Lunar" ), _("KP")
	                                             };
	const static wxString tz_choices[] = { wxT("UTC"), _("Local") };
	const static wxString ephemdeg_choices[] = { wxT("360"), wxT("180"), wxT("90"), wxT("45"), wxT("30"),
		wxT("22:30"), wxT("13:20"), wxT("11:15"), wxT("5:07:30")
	                                           };
	const static wxString first_house_choices[] = { _("Ascendant"), _("Moon"), _("Arudha Lagna"),
		_( "Aries" ), _( "Taurus" ), _( "Gemini" ), _( "Cancer" ), _( "Leo" ), _( "Virgo" ), _( "Libra" ),
		_( "Scorpio" ), _( "Sagittarius" ), _( "Capricorn" ), _( "Aquarius" ), _( "Pisces" ) };

	wxChoice *choice = 0;
	wxControl *control = 0;
	wxString s;
	switch ( item )
	{
	case TBS_TEXTMODE:
		choice = new wxChoice( toolbar, item, wxDefaultPosition, wxDefaultSize, 16, text_viewmodes, 0 );
		choice->SetToolTip( _( "Mode" ));
		break;
	case TBS_VARGA:
	{
		VargaExpert vexpert;
		choice = new wxChoice( toolbar, item, wxDefaultPosition, wxSize( 150, -1 ));
		for ( int i = 0; i < NB_VARGAS; i++ )
		{
			if ( vexpert.getVargaDivision(i) > 0 )
				s.Printf( wxT( "%s (D%d)" ), vexpert.getVargaName(i), vexpert.getVargaDivision(i) );
			else s = vexpert.getVargaName(i);
			choice->Append( s );
		}
		choice->SetToolTip( _( "Varga" ));
	}
	break;
	case TBS_DASA:
		choice = new wxChoice( toolbar, item, wxDefaultPosition, wxSize( 150, -1 ));
		for ( unsigned int i = 0; i < DasaExpertFactory::MAX_DASAEXPERTS; i++ )
			choice->Append( DasaExpertFactory::get()->getDasaExpert(i)->getName() );
		choice->SetToolTip( _( "Dasa" ));
		break;
	case TBS_YEAR:
		control = new wxSpinCtrl( toolbar, item, wxT(""), wxDefaultPosition, wxDefaultSize,
		                          wxSP_ARROW_KEYS | wxTAB_TRAVERSAL, 0, 3000);
		control->SetSize( 80, control->GetBestSize().GetY());
		control->SetToolTip( _( "Year" ));
		break;
	case TBS_NBYEARS:
		control = new wxSpinCtrl( toolbar, item, wxT(""), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 10);
		control->SetSize( 60, control->GetBestSize().GetY());
		control->SetToolTip( _( "Number of Years" ));
		break;
	case TBS_ECL_TYPE:
		control = new wxChoice( toolbar, item, wxDefaultPosition, wxDefaultSize, 3, eclipse_types, 0);
		control->SetToolTip( _( "Type" ));
		break;
	case TBS_DATE_PROGRESS:
		control = new wxSlider( toolbar, item, 0, -7, 7, wxDefaultPosition, wxSize( 100, -1 ), wxSL_HORIZONTAL );
		//| wxSL_AUTOTICKS ); | wxSL_LABELS );
		control->SetToolTip( _( "Progress Transit Date" ));
		break;
	case TBS_DATE:
		control = new DateSpin( toolbar, item );
		break;
	case TBS_TIME:
		control = new TimeSpin( toolbar, item );
		break;
	case TBS_TRANSITMODE:
		control = new TransitModeChoice( toolbar, item );
		break;
	case TBS_YEARLENGTH:
		control = new YearLengthChoice( toolbar, item, false );
		break;
	case TBS_ORBIS:
		//control = new wxSpinCtrl( toolbar, item, wxT(""), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS | wxTE_PROCESS_ENTER, 0, 150);
		control = new wxSpinCtrl( toolbar, item, wxT(""), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 150);
		control->SetSize( 60, control->GetBestSize().GetY());
		control->SetToolTip( _( "Orbis" ));
		break;
	case TBS_CIRCLETYPE:
		control = new CircleTypeChoice( toolbar, item );
		break;
	case TBS_SORT:
		control = new SortChoice( toolbar, item );
		break;
	case TBS_FILTER:
		control =  new FilterChoice( toolbar, item );
		break;
	case TBS_YOGAGROUPFILTER:
		choice = new wxChoice( toolbar, item, wxDefaultPosition, wxSize( 150, -1 ));
		choice->SetToolTip( wxT( "Filter for Yoga Types" ));
		break;
	case TBS_YOGASOURCEFILTER:
		choice = new wxChoice( toolbar, item, wxDefaultPosition, wxSize( 150, -1 ));
		choice->SetToolTip( wxT( "Filter for Yoga Sources" ));
		break;
	case TBS_YOGAFIRSTHOUSE:
		choice = new wxChoice( toolbar, item, wxDefaultPosition, wxSize( 120, -1 ), 15,
		                       first_house_choices, wxADJUST_MINSIZE );
		choice->SetToolTip( wxT( "Define Ascendant" ));
		break;
	case TBS_URANIANMODE:
		choice = new wxChoice( toolbar, item, wxDefaultPosition, wxDefaultSize, 2, uranian_viewmodes, 0);
		choice->SetSelection( 0 );
		choice->SetToolTip( _( "Mode" ));
		break;
	case TBS_ZOOM:
		choice = new wxChoice( toolbar, item, wxDefaultPosition, wxDefaultSize, 10, zoom_choices, 0);
		choice->SetToolTip( _( "Zoom" ));
		break;
	case TBS_ANTARDASALEVEL:
		choice = new wxChoice( toolbar, item, wxDefaultPosition, wxDefaultSize, 3, antardasa_choices, 0);
		choice->SetSelection( 1 );
		choice->SetToolTip( _( "Dasa Level" ));
		break;
	case TBS_MONTH:
		control =  new wxSpinCtrl( toolbar, item, wxT(""), wxDefaultPosition, wxDefaultSize,
		                           wxSP_ARROW_KEYS|wxSP_WRAP|wxTE_PROCESS_ENTER, 1, 12 );
		control->SetSize( 50, control->GetBestSize().GetY());
		control->SetToolTip( _( "Month" ));
		break;
	case TBS_EPHEMMODE:
		choice = new wxChoice( toolbar, item, wxDefaultPosition, wxSize( 120, -1 ), 6, ephem_mode_choices, 0);
		choice->SetToolTip( _( "Mode" ));
		break;
	case TBS_TZ:
		choice = new wxChoice( toolbar, item, wxDefaultPosition, wxDefaultSize, 2, tz_choices, 0);
		choice->SetToolTip( _( "Time Zone" ));
		break;
	case TBS_EPHEMDEG:
		choice = new wxChoice( toolbar, item, wxDefaultPosition, wxSize( 120, -1 ), 9, ephemdeg_choices, 0);
		choice->SetToolTip( _( "Circle Type" ));
		break;
	case TBS_PARTNER1:
		choice = new wxChoice( toolbar, item, wxDefaultPosition, wxSize( 150, -1 ));
		choice->SetToolTip( _( "Partner 1" ));
		break;
	case TBS_PARTNER2:
		choice = new wxChoice( toolbar, item, wxDefaultPosition, wxSize( 150, -1 ));
		choice->SetToolTip( _( "Partner 2" ));
		break;
	default:
		assert( 0 );
		break;
	}
	assert( choice || control );
	if ( choice )
	{
		choice->SetSelection( 0 );
		toolbar->AddControl( choice );
	}
	else
	{
		toolbar->AddControl( control );
	}
}

/*****************************************************
**
**   ToolbarFactory   ---    createButtonItem
**
******************************************************/
void ToolbarFactory::createButtonItem( const int &item )
{
	IconProvider p;
	switch ( item )
	{
	case TBB_NOW:
		toolbar->AddTool( item, _("Now"), p.getBitmap( BITMAP_NOW ));
		break;
	case TBB_DASA:
		toolbar->AddTool( item, _("Select Dasas"), p.getBitmap( BITMAP_DASA ));
		break;
	case TBB_GOTO:
		toolbar->AddTool( item, _("Goto Year"), p.getBitmap( BITMAP_GOTO ));
		break;
	case TBB_NEW:
		toolbar->AddTool( item, _("New File"), p.getBitmap( BITMAP_NEW ));
		break;
	case TBB_OPEN:
		toolbar->AddTool( item, _("Open File"), p.getBitmap( BITMAP_OPEN ));
		break;
	case TBB_SAVE:
		toolbar->AddTool( item, _("Save File"), p.getBitmap( BITMAP_SAVE ));
		break;
	case TBB_SAVEAS:
		toolbar->AddTool( item, _("Save File As"), p.getBitmap( BITMAP_SAVEAS ));
		break;
	case TBB_CLOSE:
		toolbar->AddTool( item, _("Close Child"), p.getBitmap( BITMAP_CLOSE ));
		break;
	default:
		assert( 0 );
		break;
	}
}

/*****************************************************
**
**   ToolbarFactory   ---    getCommandListFromString
**
******************************************************/
vector<ToolBarCommand> ToolbarFactory::getCommandListFromString( wxString toolbarConfig )
{
	wxString s;
	long l;
	vector<ToolBarCommand> ret;

  wxStringTokenizer tk( toolbarConfig, wxT( ";" ));
  while ( tk.HasMoreTokens() )
  {
    s = tk.GetNextToken();
    s.ToLong( &l );
    ret.push_back( getItemById( (int)l ));
  }
	return ret;
}

/*****************************************************
**
**   ToolbarFactory   ---    getItemById
**
******************************************************/
ToolBarCommand ToolbarFactory::getItemById( const int &index )
{
	for ( unsigned int i = 0; i < cmdlist.size(); i++ )
	{
		if ( cmdlist[i].commandId == index )
		{
			return cmdlist[i];
		}
	}
	//assert( false );
	printf( "Error in ToolbarFactory::getItemById: cannot find item with index %d\n", index );
	return ToolBarCommand();
}

/*****************************************************
**
**   ToolbarFactory   ---    init
**
******************************************************/
void ToolbarFactory::init()
{
	static const struct CmdIdList
	{
		int cmdId;
		bool isDefault;
	}
	idList[] =
	{
		{ APP_FILENEW, true },
		{ APP_FILEOPEN, true },
		{ CMD_CHILD_SAVE, true },
		{ CMD_CHILD_SAVEAS, false },
		{ CMD_CHILD_PRINT, true },
		{ CMD_CHILD_QUICKPRINT, false },
		{ CMD_CHILD_EDITDATA, true },
		{ 0, true },

		{ CMD_CHILD_NEW_RASI, true },
		{ CMD_CHILD_NEW_RASI+1, true },
		{ CMD_CHILD_NEW_RASI+2, false },
		{ CMD_CHILD_NEW_RASI+3, false },
		{ CMD_CHILD_NEW_RASI+4, false },
		{ CMD_CHILD_NEW_RASI+5, false },
		{ CMD_CHILD_NEW_RASI+6, false },
		{ CMD_CHILD_NEW_RASI+7, false },
		{ CMD_CHILD_NEW_RASI+8, false },
		{ CMD_CHILD_NEW_RASI+9, false },
		{ CMD_CHILD_NEW_RASI+10, false },
		{ CMD_CHILD_NEW_RASI+11, false },
		{ CMD_CHILD_NEW_RASI+12, false },
		{ CMD_CHILD_NEW_RASI+13, false },
		{ CMD_CHILD_NEW_RASI+14, false },
		{ CMD_CHILD_NEW_RASI+15, false },
		{ CMD_CHILD_NEW_RASI+16, false },
		{ CMD_CHILD_NEW_RASI+17, false },
		{ CMD_CHILD_NEW_RASI+18, false },
		{ CMD_CHILD_NEW_RASI+19, false },
		{ CMD_CHILD_NEW_RASI+20, false },
		{ CMD_CHILD_NEW_WCHART, true },
		{ CMD_CHILD_NEW_SBC, true },

		{ CMD_CHILD_NEW_VARGA, true },
		{ CMD_CHILD_NEW_TEXT, true },
		{ CMD_CHILD_NEW_DASA, true },
		{ CMD_CHILD_NEW_GRAPHICALDASA, true },
		{ CMD_CHILD_NEW_ASHTAKAVARGA, false },
		{ CMD_CHILD_NEW_TRANSIT, true },
		{ CMD_CHILD_NEW_YOGA, true },
		{ CMD_CHILD_NEW_SOLAR, false },

		{ CMD_CHILD_NEW_URANIAN, false },
		{ 0, true },

		{ APP_SHOWHORA, false },
		{ APP_NEW_PARTNER, true },
		{ APP_SHOWEPHEM, true },
		{ APP_SHOWECLIPSE, false },
		{ APP_LOCATION, true },
		{ APP_CONFIGURE, true },
		{ APP_HELP, false },
		{ APP_ABOUT, false }
	};

	int imageId = 1;
	CommandHandler *handler = CommandHandler::get();
	MaitreyaCommand *command;
	for ( size_t i = 0; i < WXSIZEOF(idList); i++ )
	{
		command = handler->getCommand( idList[i].cmdId );
		cmdlist.push_back(
			ToolBarCommand( idList[i].cmdId, command ? command->getBitmapId() : 0,
			command ? command->getText() : wxEmptyString,
			idList[i].isDefault, idList[i].cmdId? imageId : 0 ));
		if ( idList[i].cmdId != 0 ) imageId++;
	}
}

/*****************************************************
**
**   ToolbarFactory   ---    getDefaultCommandList
**
******************************************************/
vector<ToolBarCommand> ToolbarFactory::getDefaultCommandList()
{
	vector<ToolBarCommand> l;
	for ( unsigned int i = 0; i < cmdlist.size(); i++ )
		if ( cmdlist[i].isDefault ) l.push_back( cmdlist[i] );
	return l;
}

/*****************************************************
**
**   ToolbarFactory   ---    createMainToolbar
**
******************************************************/
MaitreyaToolbar *ToolbarFactory::createMainToolbar( wxWindow *parent )
{
	MaitreyaToolbar *toolbar = new MaitreyaToolbar( parent );
	toolbar->recreate();
	return toolbar;
}

