/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/base/ColorConfig.cpp
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
#pragma implementation "ColorConfig.h"
#endif

#include "ColorConfig.h"

#include "Conf.h"
#include "mathbase.h"

#include <wx/colour.h>
#include <wx/file.h>
#include <wx/fileconf.h>
#include <wx/gdicmn.h>
#include <wx/tokenzr.h>
#include <vector>

using namespace std;

extern Config *config;

#define SIGN_RED wxT( "#E39696" )
#define SIGN_YELLOW wxT( "#ece5a6" )
#define SIGN_GREEN wxT( "#b0eca6" )
#define SIGN_BLUE wxT( "#a6cdec" )

#define DEFAULT_FG_COLOR wxT( "black" )
#define DEFAULT_SBC_PLANET_COLOR wxT( "navy" )
#define DEFAULT_SBC_TRANSIT_COLOR wxT( "blue" )
#define DEFAULT_BG_COLOR wxT( "white" )
#define DEFAULT_TABLE_HEADER_BG_COLOR wxT( "#92BCD5" )
#define DEFAULT_SBC_MARKED_FIELDS_COLOR wxT( "#FDFE98" )
#define DEFAULT_SBC_BENEFIC_COLOR wxT( "green" )
#define DEFAULT_SBC_MALEFIC_COLOR wxT( "maroon" )
#define DEFAULT_ERROR_BG_COLOR wxT( "#c15757" )

/*****************************************************
**
**   ColorConfig   ---   Constructor
**
******************************************************/
ColorConfig::ColorConfig()
{
	// Base Colors
	fgColor = wxColour( DEFAULT_FG_COLOR );
	sbcPlanetFgColor = wxColour( DEFAULT_SBC_PLANET_COLOR );
	sbcTransitFgColor = wxColour( DEFAULT_SBC_TRANSIT_COLOR );
	bgColor = wxColour( DEFAULT_BG_COLOR );
	tableHeaderBgColor = wxColour( DEFAULT_TABLE_HEADER_BG_COLOR );
	sbcMarkedFieldsBgColor = wxColour( DEFAULT_SBC_MARKED_FIELDS_COLOR );
	sbcBeneficBgColor = wxColour( DEFAULT_SBC_BENEFIC_COLOR );
	sbcMaleficBgColor = wxColour( DEFAULT_SBC_MALEFIC_COLOR );
	errorBgColor = wxColour( DEFAULT_ERROR_BG_COLOR );

	static const struct DasaListDescription
	{
		wxString name;
		unsigned char fgcolor[3];
		unsigned char bgcolor[3];
		int bgstyle;
		wxString bmpname;
	}
	dasalist[] =
	{
		// Signs: Jataka Parijata I, 23; planets: II 19
		{  _( "Aries" ), { 255, 255, 255 }, { 255, 0, 0 }, wxSOLID, wxEmptyString },
		{  _( "Taurus" ), { 0, 0, 0 }, { 255, 255, 255 }, wxSOLID, wxEmptyString },
		{  _( "Gemini" ), { 0, 0, 0 }, { 0, 204, 51 }, wxSOLID, wxEmptyString },
		{  _( "Cancer" ), { 0, 0, 0 }, { 255, 160, 255 }, wxSOLID, wxEmptyString },
		{  _( "Leo" ), { 0, 0, 0 }, { 226, 255, 211 }, wxSOLID, wxEmptyString },
		{  _( "Virgo" ), { 0, 0, 0 }, { 255, 255, 255 }, wxSTIPPLE, _( "piebald.png" ) },
		{  _( "Libra" ), { 255, 255, 255 }, { 0, 0, 0 }, wxSOLID, wxEmptyString },
		{  _( "Scorpio" ), { 0, 0, 0 }, { 229, 169, 104 }, wxSOLID, wxEmptyString },
		{  _( "Sagittarius" ), { 255, 255, 255 }, { 167, 123, 76 }, wxSOLID, wxEmptyString },
		{  _( "Capricorn" ), { 0, 0, 0 }, { 255, 255, 255 }, wxSTIPPLE, _( "rainbow.png" ) },
		{  _( "Aquarius" ), { 255, 255, 255 }, { 104, 77, 47 }, wxSOLID, wxEmptyString },
		{  _( "Pisces" ), { 0, 0, 0 }, { 255, 255, 255 }, wxSOLID, wxEmptyString },
		{  _( "Sun" ), { 0, 0, 0 }, { 255, 94, 7 }, wxSOLID, wxEmptyString },
		{  _( "Moon" ), { 0, 0, 0 }, { 255, 255, 255 }, wxSOLID, wxEmptyString },
		{  _( "Mercury" ), { 0, 0, 0 }, { 102, 204, 102 }, wxSOLID, wxEmptyString },
		{  _( "Venus" ), { 0, 0, 0 }, { 255, 102, 255 }, wxSTIPPLE, _( "rainbow.png" ) },
		{  _( "Mars" ), { 255, 255, 255 }, { 204, 0, 0 }, wxSOLID, wxEmptyString },
		{  _( "Jupiter" ), { 0, 0, 0 }, { 255, 255, 102 }, wxSOLID, wxEmptyString },
		{  _( "Saturn" ), { 255, 255, 255 }, { 0, 0, 0 }, wxSOLID, wxEmptyString },
		{  _( "Rahu" ), { 255, 255, 255 }, { 82, 109, 198 }, wxSOLID, wxEmptyString },
		{  _( "Ketu" ), { 0, 0, 0 }, { 132, 132, 132 }, wxSOLID, wxEmptyString },
		{  _( "Ascendant" ), { 0, 0, 0 }, { 132, 132, 132 }, wxSOLID, wxEmptyString }
	};
	for ( int i = 0; i < MAX_DASA_COLORS; i++ )
	{
		dasa[i].name = dasalist[i].name;
		dasa[i].fgColor.Set( dasalist[i].fgcolor[0], dasalist[i].fgcolor[1], dasalist[i].fgcolor[2] );
		dasa[i].bgColor.Set( dasalist[i].bgcolor[0], dasalist[i].bgcolor[1], dasalist[i].bgcolor[2] );
		dasa[i].fgWidth = 1;
		dasa[i].fgStyle = wxSOLID;
		dasa[i].bgStyle = dasalist[i].bgstyle;
		dasa[i].bgImage = dasalist[i].bmpname;
	}
}

#define DO_READ( a, b, c ) if ( ! cfg->Read( a, b, c )) { printf( "Warn: config parameter not found: %s\n", str2char( a )); }

/*****************************************************
**
**   ColorConfig   ---   readConfigParams
**
******************************************************/
void ColorConfig::readConfigParams( wxConfigBase *cfg )
{
	int i;
	wxString color;

	DO_READ( wxT( "/Colors/FgColor" ), &color,  DEFAULT_FG_COLOR );
	fgColor = wxColour( color );
	DO_READ( wxT( "/Colors/SbcPlanetFgColor" ), &color,  DEFAULT_SBC_PLANET_COLOR );
	sbcPlanetFgColor = wxColour( color );
	DO_READ( wxT( "/Colors/SbcTransitFgColor" ), &color,  DEFAULT_SBC_TRANSIT_COLOR );
	sbcTransitFgColor = wxColour( color );
	DO_READ( wxT( "/Colors/BgColor" ), &color,  DEFAULT_BG_COLOR );
	bgColor = wxColour( color );
	DO_READ( wxT( "/Colors/TableHeaderBgColor" ), &color,  DEFAULT_TABLE_HEADER_BG_COLOR );
	tableHeaderBgColor = wxColour( color );
	DO_READ( wxT( "/Colors/SbcMarkedFieldsBgColor" ), &color,  DEFAULT_SBC_MARKED_FIELDS_COLOR );
	sbcMarkedFieldsBgColor = wxColour( color );
	DO_READ( wxT( "/Colors/SbcBeneficBgColor" ), &color,  DEFAULT_SBC_BENEFIC_COLOR );
	sbcBeneficBgColor = wxColour( color );
	DO_READ( wxT( "/Colors/SbcMaleficBgColor" ), &color,  DEFAULT_SBC_MALEFIC_COLOR );
	sbcMaleficBgColor = wxColour( color );
	DO_READ( wxT( "/Colors/ErrorBgColor" ), &color,  DEFAULT_ERROR_BG_COLOR );
	errorBgColor = wxColour( color );

	for ( i = 0; i < 4; i++ )
	{
		DO_READ( wxString::Format( wxT( "/Colors/SignFgColor/%02d" ), i ), &color, wxEmptyString );
		if ( ! color.IsEmpty()) chart.signFgColor[i] = wxColour( color );

		DO_READ( wxString::Format( wxT( "/Colors/SignBgColor/%02d" ), i ), &color, wxEmptyString );
		if ( ! color.IsEmpty() ) chart.signBgColor[i] = wxColour( color );
		
		DO_READ( wxString::Format( wxT( "/Colors/HouseFgColor/%02d" ), i ), &color, wxEmptyString );
		if ( ! color.IsEmpty() ) chart.houseFgColor[i] = wxColour( color );

		DO_READ( wxString::Format( wxT( "/Colors/HouseBgColor/%02d" ), i ), &color, wxEmptyString );
		if ( ! color.IsEmpty() ) chart.houseBgColor[i] = wxColour( color );
	}
	for ( i = 0; i < MAX_EPHEM_OBJECTS; i++ )
	{
		DO_READ( wxString::Format( wxT( "/Colors/PlanetColor/%02d" ), i ), &color, wxEmptyString );
		if ( ! color.IsEmpty() ) chart.planetFgColor[i] = wxColour( color );
	}
	for ( i = 0; i < MAX_DASA_COLORS; i++ )
	{
		DO_READ( wxString::Format( wxT( "/Colors/Dasas/%02d" ), i ), &color, wxEmptyString );
		if ( color.IsEmpty() ) continue;
		wxStringTokenizer tk( color, wxT( ";" ));

		// Must have 6 or 7 tokens
		if ( tk.CountTokens() != 6 && tk.CountTokens() != 7 )
		{
			printf( "Error: wrong number of tokens (%d) in dasa config %d\n", (int)tk.CountTokens(), i );
		}
		else
		{
			if ( tk.HasMoreTokens() ) dasa[i].name = tk.GetNextToken();
			if ( tk.HasMoreTokens() ) dasa[i].fgColor = wxColour( tk.GetNextToken() );
			if ( tk.HasMoreTokens() ) dasa[i].bgColor = wxColour( tk.GetNextToken() );
			if ( tk.HasMoreTokens() ) dasa[i].fgWidth = (int)myatof( tk.GetNextToken() );
			if ( tk.HasMoreTokens() ) dasa[i].fgStyle = (int)myatof( tk.GetNextToken() );
			if ( tk.HasMoreTokens() ) dasa[i].bgStyle = (int)myatof( tk.GetNextToken() );
			if ( tk.HasMoreTokens() ) dasa[i].bgImage = tk.GetNextToken();
		}
	}
}
#undef DO_READ

/*****************************************************
**
**   ColorConfig   ---   writeConfigParams
**
******************************************************/
void ColorConfig::writeConfigParams( wxConfigBase *cfg )
{
	int i;
	wxString s;

#define COLOR2STRING( c ) c.GetAsString( wxC2S_HTML_SYNTAX )

	cfg->Write( wxT( "/Colors/FgColor" ), COLOR2STRING( fgColor ));
	cfg->Write( wxT( "/Colors/SbcPlanetFgColor" ), COLOR2STRING( sbcPlanetFgColor ));
	cfg->Write( wxT( "/Colors/SbcTransitFgColor" ), COLOR2STRING( sbcTransitFgColor ));
	cfg->Write( wxT( "/Colors/BgColor" ), COLOR2STRING( bgColor ));
	cfg->Write( wxT( "/Colors/TableHeaderBgColor" ), COLOR2STRING( tableHeaderBgColor ));
	cfg->Write( wxT( "/Colors/SbcMarkedFieldsBgColor" ), COLOR2STRING( sbcMarkedFieldsBgColor ));
	cfg->Write( wxT( "/Colors/SbcBeneficBgColor" ), COLOR2STRING( sbcBeneficBgColor ));
	cfg->Write( wxT( "/Colors/SbcMaleficBgColor" ), COLOR2STRING( sbcMaleficBgColor ));
	cfg->Write( wxT( "/Colors/ErrorBgColor" ), COLOR2STRING( errorBgColor ));
	for ( i = 0; i < 4; i++ )
	{
		cfg->Write( wxString::Format( wxT( "/Colors/SignFgColor/%02d" ), i ), COLOR2STRING( chart.signFgColor[i] ));
		cfg->Write( wxString::Format( wxT( "/Colors/SignBgColor/%02d" ), i ), COLOR2STRING( chart.signBgColor[i] ));
		cfg->Write( wxString::Format( wxT( "/Colors/HouseFgColor/%02d" ), i ), COLOR2STRING( chart.houseFgColor[i] ));
		cfg->Write( wxString::Format( wxT( "/Colors/HouseBgColor/%02d" ), i ), COLOR2STRING( chart.houseBgColor[i] ));
	}

	for ( i = 0; i < MAX_EPHEM_OBJECTS; i++ )
	{
		cfg->Write( wxString::Format( wxT( "/Colors/PlanetColor/%02d" ), i ), COLOR2STRING( chart.planetFgColor[i] ));
	}
	for ( i = 0; i < MAX_DASA_COLORS; i++ )
	{
		cfg->Write( wxString::Format( wxT( "/Colors/Dasas/%02d" ), i ), dasa[i].toString());
	}
#undef COLOR2STRING
}

/*****************************************************
**
**   ColorConfig   ---   getDasa
**
******************************************************/
DasaColorConfig *ColorConfig::getDasa( const int &i )
{
	assert( i >= 0 && i < MAX_DASA_COLORS );
	return &dasa[i];
}

/*****************************************************
**
**   ColorConfig   ---   getDasaColorConfigId
**
******************************************************/
int ColorConfig::getDasaColorConfigId( const int &i, const bool &rasitype )
{
	int p;
	if ( rasitype )
	{
		assert( i >= 0 && i < 12 );
		return i;
	}
	else
	{
		if ( i >= OSUN && i <= OSATURN ) p = i;
		else if ( i == OMEANNODE || i == OTRUENODE || i == 8 ) p = 7;
		else if ( i == OMEANDESCNODE || i == OTRUEDESCNODE ) p = 8;
		else if ( i == OASCENDANT || i == 7 ) p = 9; // caution: Ascendant on 7 (AV) TODO
		else assert( false );
		return p + 12;
	}
}

/*****************************************************
**
**   ColorConfig   ---   getPlanetColor
**
******************************************************/
wxColour &ColorConfig::getPlanetColor( const int &i )
{
	assert( i >= 0 && i < MAX_EPHEM_OBJECTS );
	return chart.planetFgColor[i];
}

/*****************************************************
**
**   ColorConfig   ---   getSignFgColor
**
******************************************************/
wxColour &ColorConfig::getSignFgColor( const int &i )
{
	return chart.signFgColor[i%4];
}

/*****************************************************
**
**   ColorConfig   ---   getSignBgColor
**
******************************************************/
wxColour &ColorConfig::getSignBgColor( const int &i )
{
	return chart.signBgColor[i%4];
}

/*****************************************************
**
**   ColorConfig   ---   getHouseFgColor
**
******************************************************/
wxColour &ColorConfig::getHouseFgColor( const int &i )
{
	return chart.houseFgColor[i%4];
}

/*****************************************************
**
**   ColorConfig   ---   getHouseBgColor
**
******************************************************/
wxColour &ColorConfig::getHouseBgColor( const int &i )
{
	return chart.houseBgColor[i%4];
}

/*****************************************************
**
**   DasaColorConfig   ---   Constructor
**
******************************************************/
DasaColorConfig::DasaColorConfig()
{
	bgColor = *wxWHITE;
	fgColor = *wxBLACK;
	fgWidth = 1;
	fgStyle = bgStyle = wxSOLID;
}

/*****************************************************
**
**   DasaColorConfig   ---   toString
**
******************************************************/
wxString DasaColorConfig::toString()
{
	wxString s;
	s << name
	<< wxT( ";" )
	<< fgColor.GetAsString( wxC2S_HTML_SYNTAX )
	<< wxT( ";" )
	<< bgColor.GetAsString( wxC2S_HTML_SYNTAX )
	<< wxT( ";" )
	<< fgWidth
	<< wxT( ";" )
	<< fgStyle
	<< wxT( ";" )
	<< bgStyle
	<< wxT( ";" )
	<< bgImage;
	return s;
}

/*****************************************************
**
**   AspectColorConfig   ---   Constructor
**
******************************************************/
AspectColorConfig::AspectColorConfig()
{
	fgWidth = 1;
	fgStyle = wxSOLID;
}

/*****************************************************
**
**   ChartColorConfig   ---   Constructor
**
******************************************************/
ChartColorConfig::ChartColorConfig()
{
	int i;
	const static wxChar *k_sign_house_bg_color[4]  = {
		SIGN_RED, SIGN_YELLOW, SIGN_GREEN, SIGN_BLUE
	};
	const static wxChar *k_sign_house_fg_color[4]  = {
		wxT( "red" ), wxT( "yellow" ), wxT( "green" ), wxT( "blue" )
	};
	const static wxChar *k_planet_color[MAX_EPHEM_OBJECTS]  = {
		wxT( "red" ),    // Sun
		wxT( "blue" ),   // Moon
		wxT( "green" ),  // Merc
		wxT( "yellow" ), // Ven
		wxT( "red" ),    // Mars
		wxT( "red" ),    // Jup
		wxT( "yellow" ), // Sat
		wxT( "green" ),  // Uran
		wxT( "blue" ),   // Nep
		wxT( "blue" ),   // Plu
		wxT( "gray" ),   // Mean Node
		wxT( "gray" ),   // True Node
		wxT( "gray" ),   // Desc Mean Node
		wxT( "gray" ),   // Desc True Node
		wxT( "red" ),    // Ascendant
		wxT( "yellow" ), // MC
		wxT( "yellow" ), // Cupido
		wxT( "navy" ),   // Hades
		wxT( "green" ),  // Zeus
		wxT( "navy" ),   // Kronos
		wxT( "red" ),    // Apollon
		wxT( "yellow" ), // Admetos
		wxT( "red" ),    // Vulkanus
		wxT( "blue" ),   // Poseidon
		wxT( "red" ),   // Chiron
		wxT( "red" ),   // Pholus
		wxT( "purple" ),   // Plan1
		wxT( "purple" ),   //
		wxT( "purple" ),   //
		wxT( "purple" ),   // Plan4
		wxT( "gray" )   // Lilith
	};
	for ( i = 0; i < 4; i++ )
	{
		signFgColor[i] = k_sign_house_fg_color[i];
		houseFgColor[i] = k_sign_house_fg_color[i];
		signBgColor[i] = k_sign_house_bg_color[i];
		houseBgColor[i] = k_sign_house_bg_color[i];
	}
	for ( i = 0; i < MAX_EPHEM_OBJECTS; i++ ) planetFgColor[i] = k_planet_color[i];
}

