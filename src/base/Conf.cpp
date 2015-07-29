/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/base/Conf.cpp
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
#pragma implementation "Conf.h"
#endif

#include "Conf.h"

#include "ColorConfig.h"
#include "FileConfig.h"
#include "FontConfig.h"
#include "mathbase.h"

#include <wx/defs.h>
#include <wx/fileconf.h>
#include <wx/filename.h>
#include <wx/string.h>
#include <wx/tokenzr.h>
#include <wx/utils.h>

/*****************************************************
**
**   Config   ---   Constructor
**
******************************************************/
Config::Config()
{
	wxString s;
	s << FileConfig::get()->getConfigDir();
	if ( s.Last() != wxFileName::GetPathSeparator()) s << wxFileName::GetPathSeparator();
	s << wxT( "config" );

	cfg = new wxFileConfig( APP_NAME, VENDOR_NAME, s, wxEmptyString, wxCONFIG_USE_LOCAL_FILE );
	cfg->SetRecordDefaults();
	colors = new ColorConfig();
}

/*****************************************************
**
**   Config   ---   Destructor
**
******************************************************/
Config::~Config()
{
	delete colors;
	delete cfg;
}

/*****************************************************
**
**   Config   ---   writeDoubleEntry
**
******************************************************/
void Config::writeDoubleEntry( wxString path, const double value )
{
	wxString s;
	s.Printf( wxT( "%.12f" ), value );
	cfg->Write( path, s );
}

/*****************************************************
**
**   Config   ---   readDoubleEntry
**
******************************************************/
double Config::readDoubleEntry( wxString path, const double def )
{
	wxString s;
	wxString dd;
	dd.Printf( wxT( "%.12f" ), def );
	cfg->Read( path, &s, dd );
	return myatof( s );
}

// Debug only: find lost parameters that do not get saved that do not get saved
#define DO_READ( a, b, c ) if ( ! cfg->Read( a, b, c )) { printf( "Warn: config parameter not found: %s\n", str2char( a )); }
//#define DO_READ( a, b, c ) cfg->Read( a, b, c );

/*****************************************************
**
**   Config   ---   readIntArray
**
******************************************************/
void Config::readIntArray( wxArrayInt &array, wxString key, wxString defaultValue = wxEmptyString )
{
	wxString s, token;
	long l;

	DO_READ( key, &s, defaultValue );
	wxStringTokenizer tk( s, wxT( ";" ));
	array.Clear();
	while ( tk.HasMoreTokens())
	{
		token = tk.GetNextToken();
		token.ToLong( &l );
		array.Add( l );
	}
}

/*****************************************************
**
**   Config   ---   readAll
**
******************************************************/
void Config::readAll()
{
	wxString s, locationName;
	double lon, lat;
	unsigned int i;

	DO_READ( wxT( "/General/LogMode" ), &logMode, 0l );
	DO_READ( wxT( "/General/PreferVedic" ), &preferVedic, true );
	DO_READ( wxT( "/General/TextMode" ), &textmode, 1 );
	DO_READ( wxT( "/General/MaxRecentFiles" ), &maxRecentFiles, 12 );
	DO_READ( wxT( "/General/AskOnQuit" ), &askOnQuit, false );
	DO_READ( wxT( "/General/OpenNewDocOnStart" ), &openNewDocOnStart, true );
	DO_READ( wxT( "/General/RecentFileList" ), &recentFileList, wxEmptyString );
	DO_READ( wxT( "/General/Lang" ), &lang, wxT( "en" ));
	DO_READ( wxT( "/General/LangList" ), &langList, wxT( "de ru" ));
	DO_READ( wxT( "/General/BackupFileMode" ), &backupFileMode, 1 );
	DO_READ( wxT( "/General/ToolbarItems" ), &mainToolbarItems, wxEmptyString );

	// Window Preferences
	DO_READ( wxT( "/WindowPreferences/UseMultipleViews" ), &useMultipleViews, false );
	DO_READ( wxT( "/WindowPreferences/DefaultMultipleView" ), &defaultMultipleView, 0 );
	DO_READ( wxT( "/WindowPreferences/ChildViewFrameWidth" ), &childViewFrameWidth, 0 );
	DO_READ( wxT( "/WindowPreferences/ShowTextViewHeaders" ), &showTextViewHeaders, 0 );

	DO_READ( wxT( "/WindowPreferences/ShowMainToolbar" ), &showMainToolbar, true );
	DO_READ( wxT( "/WindowPreferences/ListStyle" ), &listStyle, 0 );
	DO_READ( wxT( "/WindowPreferences/ShowStatusInfo" ), &showStatusInfo , true );
	DO_READ( wxT( "/WindowPreferences/ShowStatusBar" ), &showStatusBar, false );

	DO_READ( wxT( "/WindowPreferences/xPosMasterWindow" ), &xPosMasterWindow, 50 );
	DO_READ( wxT( "/WindowPreferences/yPosMasterWindow" ), &yPosMasterWindow, 50 );
	DO_READ( wxT( "/WindowPreferences/TextWindowPage" ), &textWindowPage, 0 );
	DO_READ( wxT( "/WindowPreferences/UranianWindowPage" ), &uranianWindowPage, 0 );
	DO_READ( wxT( "/WindowPreferences/PartnerStyle" ), &partnerStyle, 1 );
	DO_READ( wxT( "/WindowPreferences/EphemMode" ), &ephemMode, 0 );
	DO_READ( wxT( "/WindowPreferences/EphemCircleType" ), &ephemCircleType, 0 );
	DO_READ( wxT( "/WindowPreferences/EphemTimezone" ), &ephemTimezone, 0 );

	DO_READ( wxT( "/WindowPreferences/OrbisRadix" ), &orbisRadix, 90 );
	DO_READ( wxT( "/WindowPreferences/OrbisTransit" ), &orbisTransit, 30 );
	DO_READ( wxT( "/WindowPreferences/OrbisPartner" ), &orbisPartner, 30 );
	DO_READ( wxT( "/WindowPreferences/CircleType" ), &circleType, CIRCLE_TYPE_45 );
	DO_READ( wxT( "/WindowPreferences/PlanetSortOrder" ), &planetSortOrder, 0 );
	DO_READ( wxT( "/WindowPreferences/TransitModus" ), &transitModus, 0 );
	DO_READ( wxT( "/WindowPreferences/TransitTimezone" ), &transitTimezone, 0 );

	DO_READ( wxT( "/WindowPreferences/VedicGraphicStyle/ShowArudhas" ), &vGraphicStyle.showArudhas, false );
	DO_READ( wxT( "/WindowPreferences/VedicGraphicStyle/ShowRetro" ), &vGraphicStyle.showRetro, true );
	DO_READ( wxT( "/WindowPreferences/VedicGraphicStyle/ShowPlanetColors" ), &vGraphicStyle.showPlanetColors, true );
	DO_READ( wxT( "/WindowPreferences/VedicGraphicStyle/IndianChartType" ), &vGraphicStyle.indianChartType, VGRAPHIC_SOUTH_INDIAN );
	DO_READ( wxT( "/WindowPreferences/VedicGraphicStyle/CenterInfoType" ), &vGraphicStyle.centerInfoType, VGRAPHIC_CHART_CENTER_VARGA );
	DO_READ( wxT( "/WindowPreferences/VedicGraphicStyle/NorthIndianSignDisplayType" ), &vGraphicStyle.northIndianSignDisplayType, VGRAPHIC_NORTH_INDIAN_NUMBER );
	DO_READ( wxT( "/WindowPreferences/VedicGraphicStyle/ShowSbcSanskritSymbols" ), &vGraphicStyle.showSbcSanskritSymbols, false );
	DO_READ( wxT( "/WindowPreferences/VedicGraphicStyle/ShowSbcAfflictions" ), &vGraphicStyle.showSbcAfflictions, true );
	DO_READ( wxT( "/WindowPreferences/VedicGraphicStyle/ShowSbcNakshatraQuality" ), &vGraphicStyle.showSbcNakshatraQuality, true );

	DO_READ( wxT( "/WindowPreferences/VedicMainChildStyle" ), &vMainChildStyle, MAIN_CHILD_SHOW_NAVAMSA | MAIN_CHILD_SHOW_NAKSHATRA );
	DO_READ( wxT( "/WindowPreferences/VedicDefaultDasaList" ), &vDefaultDasaList, 7 );
	DO_READ( wxT( "/WindowPreferences/DasaCornerRadius" ), &dasaCornerRadius, 5 );
	DO_READ( wxT( "/WindowPreferences/AntarDasaLevelChoice;" ), &antarDasaLevelChoice, 1 );
	DO_READ( wxT( "/WindowPreferences/DasaZoomChoice" ), &dasaZoomChoice, 3 );

	// Yogas
	DO_READ( wxT( "/WindowPreferences/YogaSourceFilter" ), &vYogaSourceFilter, 0 );
	DO_READ( wxT( "/WindowPreferences/YogaGroupFilter" ), &vYogaGroupFilter, 0 );
	DO_READ( wxT( "/WindowPreferences/YogaCustomFilter" ), &vYogaCustomFilter, 0 );
	DO_READ( wxT( "/WindowPreferences/LastYogaConfigFile" ), &lastYogaConfigFile, wxEmptyString );

	DO_READ( wxT( "/WindowPreferences/WesternGraphicStyle/ShowHouses" ), &wGraphicStyle.showHouses, true );
	DO_READ( wxT( "/WindowPreferences/WesternGraphicStyle/ShowAspects"), &wGraphicStyle.showAspects, true );
	DO_READ( wxT( "/WindowPreferences/WesternGraphicStyle/ShowRetro" ), &wGraphicStyle.showRetro, true );
	DO_READ( wxT( "/WindowPreferences/WesternGraphicStyle/ShowPlanetColors" ), &wGraphicStyle.showPlanetColors, true );
	DO_READ( wxT( "/WindowPreferences/WesternGraphicStyle/ShowSignColors" ), &wGraphicStyle.showSignColors, true );
	DO_READ( wxT( "/WindowPreferences/WesternGraphicStyle/ShowHouseColors" ), &wGraphicStyle.showHouseColors, true );
	DO_READ( wxT( "/WindowPreferences/WesternGraphicStyle/ShowAspectColors" ), &wGraphicStyle.showAspectColors, true );
	DO_READ( wxT( "/WindowPreferences/WesternGraphicStyle/ShowAspectSymbols" ), &wGraphicStyle.showAspectSymbols, false );
	DO_READ( wxT( "/WindowPreferences/WesternGraphicStyle/TransitStyle" ), &wGraphicStyle.transitStyle, 0 );

	DO_READ( wxT( "/WindowPreferences/WesternChartOrientation" ), &wChartOrientation, 0 );
	DO_READ( wxT( "/WindowPreferences/WesternMainChildStyle" ), &wMainChildStyle, MAIN_CHILD_SHOW_HOUSEPOS );
	DO_READ( wxT( "/WindowPreferences/ConfigDialogWChartPreviewTransits" ), &configDialogWChartPreviewTransits, 0 );
	DO_READ( wxT( "/WindowPreferences/ConfigDialogVChartPreviewTransits" ), &configDialogVChartPreviewTransits, 0 );

	// Notebooks
	DO_READ( wxT( "Notebooks/AshtakaVarga" ), &activePageAshtakaVarga, 0 );
	DO_READ( wxT( "/Notebooks/Partner" ), &avtivePagePartner, 0 );
	DO_READ( wxT( "/Notebooks/Transit" ), &avtivePageTransit, 0 );
	DO_READ( wxT( "/Notebooks/VargaOverview" ), &activePageVargaOverview, 0 );
	DO_READ( wxT( "/Notebooks/ShadBala" ), &activePageShadbala, 0 );
#ifdef wxUSE_TOOLBOOK
	int def_nbstyle = NB_STYLE_TOOLBOOK;
#else
	int def_nbstyle = NB_STYLE_LISTBOOK;
#endif
	DO_READ( wxT( "/Notebooks/MultipleViewNotebookStyle" ), &mViewNotebookStyle, def_nbstyle );
	DO_READ( wxT( "/Notebooks/MultipleViewNotebookOrientation" ), &mViewNotebookOrientation, 0 );
	DO_READ( wxT( "/Notebooks/ConfigDialogActivePage" ), &configDialogActivePage, 0 );

	// Signs
	DO_READ( wxT( "/Signs/VedicSignNames" ), &useVedicSignNames, false );
	DO_READ( wxT( "/Signs/VedicPlanetNames" ), &useVedicPlanetNames, false );
	DO_READ( wxT( "/Signs/VedicPositions" ), &useVedicPositions, false );
	DO_READ( wxT( "/Signs/SignPrecision" ), &signPrecision, TLARGE );
	DO_READ( wxT( "/Signs/UseSignSymbols" ), &useSignSymbols, true );
	DO_READ( wxT( "/Signs/UsePlanetSymbols" ), &usePlanetSymbols, false );
	DO_READ( wxT( "/Signs/UranusSymbol" ), &uranusSymbol, 0 );
	DO_READ( wxT( "/Signs/PlutoSymbol" ), &plutoSymbol, 0 );
	DO_READ( wxT( "/Signs/CapricornSymbol" ), &capricornSymbol, 0 );

	// Paths
	DO_READ( wxT( "/Paths/DefaultOpenFiletype" ), &defOpenFiletype, 0 );
	DO_READ( wxT( "/Paths/DefaultTextExportType" ), &defTextExportType, 0 );
	DO_READ( wxT( "/Paths/DefaultGraphicExportType" ), &defGraphicExportType, 0 );
	DO_READ( wxT( "/Paths/ShowGraphicSizeDialog" ), &showGraphicSizeDialog, 1 );
	DO_READ( wxT( "/Paths/ExportAskOnOverwrite" ), &exportAskOnOverwrite, 1 );

	DO_READ( wxT( "/Paths/DefaultOpenPath" ), &defOpenPath, wxGetHomeDir() );
	DO_READ( wxT( "/Paths/DefaultSavePath" ), &defSavePath, wxGetHomeDir() );
	DO_READ( wxT( "/Paths/DefaultPdfSavePath" ), &defPdfSavePath, wxGetHomeDir() );
	DO_READ( wxT( "/Paths/DefaultExportPath" ), &defExportPath, wxGetHomeDir() );
	DO_READ( wxT( "/Paths/SwissEphemerisPath" ), &sePath, wxGetHomeDir() );

	// Default Location
	DO_READ( wxT( "/DefaultLocation/LocationName" ), &locationName, wxEmptyString );
	lon = readDoubleEntry( wxT( "/DefaultLocation/Longitude" ));
	lat = readDoubleEntry( wxT( "/DefaultLocation/Latitude" ));
	defaultLocation.setLocation( locationName, lon, lat, 0, 0 );

	// Animation
	DO_READ( wxT( "/Animation/AnimateInterval" ), &animateInterval, 5 );
	DO_READ( wxT( "/Animation/AnimateMode" ), &animateMode, 0 );
	DO_READ( wxT( "/Animation/AnimateStepLength" ), &animateStepLength, 5 );
	DO_READ( wxT( "/Animation/AnimateStepDimension" ), &animateStepDimension, 0 );

	// Ephemeris
	custom_t0 = readDoubleEntry( wxT( "/Ephemeris/Custom_t0" ), DEFAULT_T0 );
	custom_ayan_t0 = readDoubleEntry( wxT( "/Ephemeris/Custom_ayan_t0" ),  DEFAULT_AYAN_T0 );
	custom_aya_period = readDoubleEntry( wxT( "/Ephemeris/Custom_aya_period" ), DEFAULT_AYA_PERIOD );
	DO_READ( wxT( "/Ephemeris/Custom_aya_constant" ), &custom_aya_constant, false );
	DO_READ( wxT( "/Ephemeris/Sunrise_refrac" ), &sunrise_refrac, false );
	DO_READ( wxT( "/Ephemeris/Sunrise_def" ), &sunrise_def, 0 );
	DO_READ( wxT( "/Ephemeris/UseEphemerisTime" ), &useEphemerisTime, false );
	DO_READ( wxT( "/Ephemeris/EphemCoordinateSystem" ), &ephemCoordinateSystem, 0 ); // Geocentric
	DO_READ( wxT( "/Ephemeris/UseTruePositions" ), &useTruePositions, false );

	DO_READ( wxT( "/Vedic/Ayanamsa" ), &vAyanamsa, 1 );
	DO_READ( wxT( "/Vedic/VedicObjects" ), &vObjects,
	         OBJECTS_INCLUDE_DRAGONTAIL | OBJECTS_INCLUDE_DRAGONHEAD | OBJECTS_INCLUDE_ASCENDANT  );
	DO_READ( wxT( "/Vedic/OrderLagna" ), &vOrderLagna, 0 );
	DO_READ( wxT( "/Vedic/VGraphicSkin" ), &vGraphicSkin, 0 );
	DO_READ( wxT( "/Vedic/HouseSystem" ), &vHouseSystem, HOUSE_SYSTEM_PLACIDUS );
	DO_READ( wxT( "/Vedic/HouseWestern" ), &vHouseWestern, true );
	DO_READ( wxT( "/Vedic/LunarNodeMode" ), &vLunarNodeMode, LUNAR_NODE_MEAN );
	DO_READ( wxT( "/Vedic/ArudhaMode" ), &vArudhaMode, 0 );
	DO_READ( wxT( "/Vedic/VargaHoraMode" ), &vVargaHoraMode, 0 );
	DO_READ( wxT( "/Vedic/VargaDrekkanaMode" ), &vVargaDrekkanaMode, 0 );
	DO_READ( wxT( "/Vedic/VargaChaturtamsaMode" ), &vVargaChaturtamsaMode, 0 );
	DO_READ( wxT( "/Vedic/NakshatraMode28" ), &vNakshatraMode28, 0 );
	DO_READ( wxT( "/Vedic/NakshatraPortionMode" ), &vNakshatraPortionMode, 0 );
	DO_READ( wxT( "/Vedic/KalachakraMode" ), &vKalachakraMode, 0 );
	DO_READ( wxT( "/Vedic/AshtakavargaMode" ), &vAshtakavargaMode, 0 );
	DO_READ( wxT( "/Vedic/AshtakavargaSarva" ), &vAshtakavargaSarva, 0 );
	DO_READ( wxT( "/Vedic/AshtakavargaNumberMode" ), &vAshtakavargaNumberMode, 0 );
	DO_READ( wxT( "/Vedic/AshtakavargaMantreswarMode" ), &vAshtakavargaMantreswarMode, 0 );
	DO_READ( wxT( "/Vedic/YearLength" ), &vYearLength, 0 );
	DO_READ( wxT( "/Vedic/TempFriendBasedOnVarga" ), &tempFriendBasedOnVarga, false );
	double a = ICUSTOM_YEAR_LEN;
	vCustomYearLength = readDoubleEntry( wxT( "/Vedic/Calculation/CustomYearLength" ), a );

	DO_READ( wxT( "/Vedic/MoonBenefic" ), &vMoonBenefic, 1 ); // angle < 180
	DO_READ( wxT( "/Vedic/MercuryBenefic" ), &vMercuryBenefic, 1 ); // without malefic

	// Arabic Parts
	readIntArray( vArabicParts, wxT( "/Vedic/ArabicParts" ));

	// Kala Velas
	DO_READ( wxT( "/KalaVelas/KalaLord" ), &kalaVelaLord[0], KALAVELA_LORD1 );         // Sun
	DO_READ( wxT( "/KalaVelas/MrityuLord" ), &kalaVelaLord[1], KALAVELA_LORD3 );       // Mars
	DO_READ( wxT( "/KalaVelas/ArdhapraharaLord" ), &kalaVelaLord[2], KALAVELA_LORD4 ); // Mercury
	DO_READ( wxT( "/KalaVelas/YamaghantakaLord" ), &kalaVelaLord[3], KALAVELA_LORD5 ); // Jupiter
	DO_READ( wxT( "/KalaVelas/GulikaLord" ), &kalaVelaLord[4], KALAVELA_LORD7 );       // Saturn
	DO_READ( wxT( "/KalaVelas/MandiLord" ), &kalaVelaLord[5], KALAVELA_LORD_NONE );

	DO_READ( wxT( "/KalaVelas/KalaPortion" ), &kalaVelaPortion[0], KALAVELA_PORTION_START );
	DO_READ( wxT( "/KalaVelas/MrityuPortion" ), &kalaVelaPortion[1], KALAVELA_PORTION_START );
	DO_READ( wxT( "/KalaVelas/ArdhapraharaPortion" ), &kalaVelaPortion[2], KALAVELA_PORTION_START );
	DO_READ( wxT( "/KalaVelas/YamaghantakaPortion" ), &kalaVelaPortion[3], KALAVELA_PORTION_START );
	DO_READ( wxT( "/KalaVelas/GulikaPortion" ), &kalaVelaPortion[4], KALAVELA_PORTION_START);
	DO_READ( wxT( "/KalaVelas/MandiPortion" ), &kalaVelaPortion[5], KALAVELA_PORTION_START );

	// Western
	DO_READ( wxT( "/Western/Ayanamsa" ), &wAyanamsa, 0 );
	DO_READ( wxT( "/Western/WesternObjects" ), &wObjects,
	         OBJECTS_INCLUDE_OUTER | OBJECTS_INCLUDE_DRAGONTAIL | OBJECTS_INCLUDE_DRAGONHEAD | OBJECTS_INCLUDE_ASCENDANT );
	DO_READ( wxT( "/Western/PlanetOrder" ), &wPlanetOrder, 0 );
	DO_READ( wxT( "/Western/HouseSystem" ), &wHouseSystem, HOUSE_SYSTEM_PLACIDUS );
	DO_READ( wxT( "/Western/LunarNodeMode" ), &wLunarNodeMode, LUNAR_NODE_TRUE );
	DO_READ( wxT( "/Western/WGraphicSkin" ), &wGraphicSkin, 0 );
	DO_READ( wxT( "/Western/YearLength" ), &wYearLength, 0 );
	double b = WCUSTOM_YEAR_LEN;
	wCustomYearLength = readDoubleEntry( wxT( "/Western/CustomYearLength" ), b );

	// Arabic Parts
	readIntArray( wArabicParts, wxT( "/Western/ArabicParts" ));

	// Aspects
	for ( i = 0; i < MAX_ASPECT_TYPES; i++ )
	{
		s.Printf( wxT( "/Aspects/AspectTypes/%02d" ), i );
		DO_READ( s, &wAspectConfig[i], wxEmptyString );
	}

	// Window Sizes
	DO_READ( wxT( "/WindowSizes/xMasterWindow" ), &xMasterWindow, 720 );
	DO_READ( wxT( "/WindowSizes/yMasterWindow" ), &yMasterWindow, 200 );

	DO_READ( wxT( "/WindowSizes/xMultipleView" ), &xMultipleView, 800 );
	DO_READ( wxT( "/WindowSizes/yMultipleView" ), &yMultipleView, 600 );

	DO_READ( wxT( "/WindowSizes/xMainWindow" ), &xMainWindow, 500 );
	DO_READ( wxT( "/WindowSizes/yMainWindow" ), &yMainWindow, 400 );

	DO_READ( wxT( "/WindowSizes/xVedicGraphicWindow" ), &xVedicGraphicWindow, 350 );
	DO_READ( wxT( "/WindowSizes/yVedicGraphicWindow" ), &yVedicGraphicWindow, 350 );

	DO_READ( wxT( "/WindowSizes/xWesternGraphicWindow" ), &xWesternGraphicWindow, 450 );
	DO_READ( wxT( "/WindowSizes/yWesternGraphicWindow" ), &yWesternGraphicWindow, 450 );

	DO_READ( wxT( "/WindowSizes/xTextWindow" ), &xTextWindow, 700 );
	DO_READ( wxT( "/WindowSizes/yTextWindow" ), &yTextWindow, 500 );

	DO_READ( wxT( "/WindowSizes/xDasaWindow" ), &xDasaWindow, 400 );
	DO_READ( wxT( "/WindowSizes/yDasaWindow" ), &yDasaWindow, 500 );

	DO_READ( wxT( "/WindowSizes/xGraphicalDasaWindow" ), &xGraphicalDasaWindow, 800 );
	DO_READ( wxT( "/WindowSizes/yGraphicalDasaWindow" ), &yGraphicalDasaWindow, 600 );

	DO_READ( wxT( "/WindowSizes/xSbcWindow" ), &xSbcWindow, 800 );
	DO_READ( wxT( "/WindowSizes/ySbcWindow" ), &ySbcWindow, 600 );

	DO_READ( wxT( "/WindowSizes/xSolarWindow" ), &xSolarWindow, 600 );
	DO_READ( wxT( "/WindowSizes/ySolarWindow" ), &ySolarWindow, 500 );

	DO_READ( wxT( "/WindowSizes/xUranianWindow" ), &xUranianWindow, 800 );
	DO_READ( wxT( "/WindowSizes/yUranianWindow" ), &yUranianWindow, 600 );

	DO_READ( wxT( "/WindowSizes/xTransitWindow" ), &xTransitWindow, 800 );
	DO_READ( wxT( "/WindowSizes/yTransitWindow" ), &yTransitWindow, 600 );

	DO_READ( wxT( "/WindowSizes/xYogaWindow" ), &xYogaWindow, 800 );
	DO_READ( wxT( "/WindowSizes/yYogaWindow" ), &yYogaWindow, 600 );

	DO_READ( wxT( "/WindowSizes/xVargaSummaryWindow" ), &xVargaSummaryWindow, 800 );
	DO_READ( wxT( "/WindowSizes/yVargaSummaryWindow" ), &yVargaSummaryWindow, 600 );

	DO_READ( wxT( "/WindowSizes/xShadbalaWindow" ), &xShadbalaWindow, 800 );
	DO_READ( wxT( "/WindowSizes/yShadbalaWindow" ), &yShadbalaWindow, 600 );

	DO_READ( wxT( "/WindowSizes/xAshtakaVargaWindow" ), &xAshtakaVargaWindow, 800 );
	DO_READ( wxT( "/WindowSizes/yAshtakaVargaWindow" ), &yAshtakaVargaWindow, 600 );

	DO_READ( wxT( "/WindowSizes/xEphemWindow" ), &xEphemWindow, 800 );
	DO_READ( wxT( "/WindowSizes/yEphemWindow" ), &yEphemWindow, 600 );

	DO_READ( wxT( "/WindowSizes/xEclipseWindow" ), &xEclipseWindow, 600 );
	DO_READ( wxT( "/WindowSizes/yEclipseWindow" ), &yEclipseWindow, 500 );

	DO_READ( wxT( "/WindowSizes/xHoraWindow" ), &xHoraWindow, 600 );
	DO_READ( wxT( "/WindowSizes/yHoraWindow" ), &yHoraWindow, 500 );

	DO_READ( wxT( "/WindowSizes/xPartnerWindow" ), &xPartnerWindow, 800 );
	DO_READ( wxT( "/WindowSizes/yPartnerWindow" ), &yPartnerWindow, 600 );

	DO_READ( wxT( "/WindowSizes/xConfigDialog" ), &xConfigDialog, 800 );
	DO_READ( wxT( "/WindowSizes/yConfigDialog" ), &yConfigDialog, 600 );

	DO_READ( wxT( "/WindowSizes/xSimpleConfigDialog" ), &xSimpleConfigDialog, 600 );
	DO_READ( wxT( "/WindowSizes/ySimpleConfigDialog" ), &ySimpleConfigDialog, 500 );

	DO_READ( wxT( "/WindowSizes/xYogaEditor" ), &xYogaEditor, 600 );
	DO_READ( wxT( "/WindowSizes/yYogaEditor" ), &yYogaEditor, 400 );

	DO_READ( wxT( "/WindowSizes/SashPartnerDouble" ), &sashPartnerDouble, 250 );
	DO_READ( wxT( "/WindowSizes/SashPartnerVedic" ), &sashPartnerVedic, 250 );
	DO_READ( wxT( "/WindowSizes/SashPartnerComposite" ), &sashPartnerComposite, 250 );
	DO_READ( wxT( "/WindowSizes/SashTransit" ), &sashTransit, 250 );
	DO_READ( wxT( "/WindowSizes/SashSolar" ), &sashSolar, 250 );

	// Print
	DO_READ( wxT( "/Print/ShowPageFrame" ), &printShowPageFrame, true );
	DO_READ( wxT( "/Print/SaveFileOption" ), &printSaveFileOption, 0 );
	DO_READ( wxT( "/Print/ShowSelectDialog" ), &printShowSelectDialog, true );
	DO_READ( wxT( "/Print/AskOnOverwrite" ), &printAskOverwrite, true );
	DO_READ( wxT( "/Print/CompressPdf" ), &printCompressPdf, true );
	DO_READ( wxT( "/Print/LaunchPdfViewer" ), &printLaunchPdfViewer, true );

	wxString pdfStartCommand = wxEmptyString;
#ifdef __WXMAC__
	pdfStartCommand = MAC_PDF_START_COMMAND;
#endif
	DO_READ( wxT( "/Print/PdfViewerCommand" ), &printPdfViewerCommand, pdfStartCommand );

	DO_READ( wxT( "/Print/CustomFooter" ), &printCustomFooter, false );
	DO_READ( wxT( "/Print/CustomFooterText" ), &printCustomFooterText, DEFAULT_CUSTOM_FOOTER );
	DO_READ( wxT( "/Print/DefaultPrintout" ), &printDefaultPrintout, 0 );
	DO_READ( wxT( "/Print/VedicGraphicSkin" ), &printVGraphicSkin, 0 );
	DO_READ( wxT( "/Print/WesternGraphicSkin" ), &printWGraphicSkin, 0 );
	DO_READ( wxT( "/Print/PaperFormat" ), &printPaperFormat, wxPAPER_A4 );

	DO_READ( wxT( "/Fonts/Default" ), &fontDefault, wxEmptyString );
	DO_READ( wxT( "/Fonts/Header" ), &fontHeader, wxEmptyString );
	DO_READ( wxT( "/Fonts/Graphic" ), &fontGraphic, wxEmptyString );
	DO_READ( wxT( "/Fonts/Text" ), &fontText, wxEmptyString );
	DO_READ( wxT( "/Fonts/Symbol" ), &fontSymbol, wxEmptyString );
	DO_READ( wxT( "/Fonts/SymbolFontSize" ), &symbolFontSize, 1 );

	DO_READ( wxT( "/Fonts/PdfDefault" ), &fontPdfDefault, wxEmptyString );
	DO_READ( wxT( "/Fonts/PdfHeader" ), &fontPdfHeader, wxEmptyString );
	DO_READ( wxT( "/Fonts/PdfGraphic" ), &fontPdfGraphic, wxEmptyString );
	DO_READ( wxT( "/Fonts/PdfSymbol" ), &fontPdfSymbol, wxEmptyString );
	DO_READ( wxT( "/Fonts/PdfSymbolFontSize" ), &pdfSymbolFontSize, 1 );

	// Atlas
	DO_READ( wxT( "/Atlas/DatabaseFile" ), &databaseFile, wxEmptyString );
	DO_READ( wxT( "/Atlas/AtlasFavouriteCountries" ), &atlasFavouriteCountries, wxT( "IN;US;DE;RU;FR;IT;GB;CA" ));
	DO_READ( wxT( "/Atlas/AtlasFilterMode" ), &atlasFilterMode, 0 );
	DO_READ( wxT( "/Atlas/AtlasLastSelectedCountry" ), &atlasLastSelectedCountry, wxEmptyString );
	DO_READ( wxT( "/Atlas/AtlasGridColumnSizes" ), &atlasGridColumnSizes, wxEmptyString );
	DO_READ( wxT( "/Atlas/AtlasFilterCaseSensitive" ), &atlasFilterCaseSensitive, false );
	DO_READ( wxT( "/Atlas/AtlasCacheSize" ), &atlasCacheSize, 2000 );
	DO_READ( wxT( "/Atlas/xAtlasDialog" ), &xAtlasDialog, 800 );
	DO_READ( wxT( "/Atlas/yAtlasDialog" ), &yAtlasDialog, 750 );

	// show warning dialogs
	DO_READ( wxT( "/WarningDialogs/DoNotShowAgainWarningDFonts" ), &doNotShowAgainWarningDFonts, false );
	DO_READ( wxT( "/WarningDialogs/DoNotShowAgainWarningEphemFiles" ), &doNotShowAgainWarningEphemFiles, false );
	DO_READ( wxT( "/WarningDialogs/DoNotShowAgainAfterLanguageChange" ), &doNotShowAgainAfterLanguageChange, false );
	DO_READ( wxT( "/WarningDialogs/DoNotShowAgainEphemConfigChange" ), &doNotShowAgainEphemConfigChange, false );

	colors->readConfigParams( cfg );
}

#undef DO_READ

/*****************************************************
**
**   Config   ---   writeIntArray
**
******************************************************/
void Config::writeIntArray( wxString key, const wxArrayInt &array )
{
	wxString s;
	for ( unsigned int i = 0; i < array.size(); i++ )
	{
		if ( ! s.IsEmpty() ) s << wxT( ";" );
		s << array[i];
	}
	cfg->Write( key, s );
}

/*****************************************************
**
**   Config   ---   writeAll
**
******************************************************/
void Config::writeAll()
{
	unsigned int i;
	wxString s;

	colors->writeConfigParams( cfg );

	cfg->Write( wxT( "/General/LogMode" ), logMode );
	cfg->Write( wxT( "/General/PreferVedic" ), preferVedic );
	cfg->Write( wxT( "/General/TextMode" ), textmode );
	cfg->Write( wxT( "/General/MaxRecentFiles" ), maxRecentFiles );
	cfg->Write( wxT( "/General/AskOnQuit" ), askOnQuit );
	cfg->Write( wxT( "/General/OpenNewDocOnStart" ), openNewDocOnStart );
	cfg->Write( wxT( "/General/RecentFileList" ), recentFileList );
	cfg->Write( wxT( "/General/Lang" ), lang );
	cfg->Write( wxT( "/General/LangList" ), langList );
	cfg->Write( wxT( "/General/BackupFileMode" ), backupFileMode );
	cfg->Write( wxT( "/General/ToolbarItems" ), mainToolbarItems );

	// Window Preferences
	cfg->Write( wxT( "/WindowPreferences/UseMultipleViews" ), useMultipleViews );
	cfg->Write( wxT( "/WindowPreferences/DefaultMultipleView" ), defaultMultipleView );
	cfg->Write( wxT( "/WindowPreferences/ChildViewFrameWidth" ), childViewFrameWidth );
	cfg->Write( wxT( "/WindowPreferences/ShowTextViewHeaders" ), showTextViewHeaders );

	cfg->Write( wxT( "/WindowPreferences/ShowMainToolbar" ), showMainToolbar );
	cfg->Write( wxT( "/WindowPreferences/ListStyle" ), listStyle );
	cfg->Write( wxT( "/WindowPreferences/ShowStatusInfo" ), showStatusInfo );
	cfg->Write( wxT( "/WindowPreferences/ShowStatusBar" ), showStatusBar );

	cfg->Write( wxT( "/WindowPreferences/xPosMasterWindow" ), xPosMasterWindow );
	cfg->Write( wxT( "/WindowPreferences/yPosMasterWindow" ), yPosMasterWindow );
	cfg->Write( wxT( "/WindowPreferences/TextWindowPage" ), textWindowPage );
	cfg->Write( wxT( "/WindowPreferences/UranianWindowPage" ), uranianWindowPage );
	cfg->Write( wxT( "/WindowPreferences/PartnerStyle" ), partnerStyle );
	cfg->Write( wxT( "/WindowPreferences/EphemMode" ), ephemMode );
	cfg->Write( wxT( "/WindowPreferences/EphemCircleType" ), ephemCircleType );
	cfg->Write( wxT( "/WindowPreferences/EphemTimezone" ), ephemTimezone );

	cfg->Write( wxT( "/WindowPreferences/VedicGraphicStyle/ShowArudhas" ), vGraphicStyle.showArudhas );
	cfg->Write( wxT( "/WindowPreferences/VedicGraphicStyle/ShowRetro" ), vGraphicStyle.showRetro );
	cfg->Write( wxT( "/WindowPreferences/VedicGraphicStyle/ShowPlanetColors" ), vGraphicStyle.showPlanetColors );
	cfg->Write( wxT( "/WindowPreferences/VedicGraphicStyle/IndianChartType" ), vGraphicStyle.indianChartType );
	cfg->Write( wxT( "/WindowPreferences/VedicGraphicStyle/CenterInfoType" ), vGraphicStyle.centerInfoType );
	cfg->Write( wxT( "/WindowPreferences/VedicGraphicStyle/NorthIndianSignDisplayType" ), vGraphicStyle.northIndianSignDisplayType );
	cfg->Write( wxT( "/WindowPreferences/VedicGraphicStyle/ShowSbcSanskritSymbols" ), vGraphicStyle.showSbcSanskritSymbols );
	cfg->Write( wxT( "/WindowPreferences/VedicGraphicStyle/ShowSbcAfflictions" ), vGraphicStyle.showSbcAfflictions );
	cfg->Write( wxT( "/WindowPreferences/VedicGraphicStyle/ShowSbcNakshatraQuality" ), vGraphicStyle.showSbcNakshatraQuality );

	cfg->Write( wxT( "/WindowPreferences/VedicMainChildStyle" ), vMainChildStyle );
	cfg->Write( wxT( "/WindowPreferences/VedicDefaultDasaList" ), vDefaultDasaList );
	cfg->Write( wxT( "/WindowPreferences/DasaCornerRadius" ), dasaCornerRadius );
	cfg->Write( wxT( "/WindowPreferences/AntarDasaLevelChoice;" ), antarDasaLevelChoice );
	cfg->Write( wxT( "/WindowPreferences/DasaZoomChoice" ), dasaZoomChoice );

	// Yogas
	cfg->Write( wxT( "/WindowPreferences/YogaSourceFilter" ), vYogaSourceFilter );
	cfg->Write( wxT( "/WindowPreferences/YogaGroupFilter" ), vYogaGroupFilter );
	cfg->Write( wxT( "/WindowPreferences/YogaCustomFilter" ), vYogaCustomFilter );
	cfg->Write( wxT( "/WindowPreferences/LastYogaConfigFile" ), lastYogaConfigFile );

	cfg->Write( wxT( "/WindowPreferences/WesternGraphicStyle/ShowHouses" ), wGraphicStyle.showHouses );
	cfg->Write( wxT( "/WindowPreferences/WesternGraphicStyle/ShowAspects"), wGraphicStyle.showAspects );
	cfg->Write( wxT( "/WindowPreferences/WesternGraphicStyle/ShowRetro" ), wGraphicStyle.showRetro );
	cfg->Write( wxT( "/WindowPreferences/WesternGraphicStyle/ShowPlanetColors" ), wGraphicStyle.showPlanetColors );
	cfg->Write( wxT( "/WindowPreferences/WesternGraphicStyle/ShowSignColors" ), wGraphicStyle.showSignColors );
	cfg->Write( wxT( "/WindowPreferences/WesternGraphicStyle/ShowHouseColors" ), wGraphicStyle.showHouseColors );
	cfg->Write( wxT( "/WindowPreferences/WesternGraphicStyle/ShowAspectColors" ), wGraphicStyle.showAspectColors );
	cfg->Write( wxT( "/WindowPreferences/WesternGraphicStyle/ShowAspectSymbols" ), wGraphicStyle.showAspectSymbols );
	cfg->Write( wxT( "/WindowPreferences/WesternGraphicStyle/TransitStyle" ), wGraphicStyle.transitStyle );
	cfg->Write( wxT( "/WindowPreferences/WesternChartOrientation" ), wChartOrientation );
	cfg->Write( wxT( "/WindowPreferences/WesternMainChildStyle" ), wMainChildStyle );

	cfg->Write( wxT( "/WindowPreferences/OrbisRadix" ), orbisRadix );
	cfg->Write( wxT( "/WindowPreferences/OrbisTransit" ), orbisTransit );
	cfg->Write( wxT( "/WindowPreferences/OrbisPartner" ), orbisPartner );
	cfg->Write( wxT( "/WindowPreferences/CircleType" ), circleType );
	cfg->Write( wxT( "/WindowPreferences/PlanetSortOrder" ), planetSortOrder );
	cfg->Write( wxT( "/WindowPreferences/TransitModus" ), transitModus );
	cfg->Write( wxT( "/WindowPreferences/TransitTimezone" ), transitTimezone );
	cfg->Write( wxT( "/WindowPreferences/ConfigDialogWChartPreviewTransits" ), configDialogWChartPreviewTransits );
	cfg->Write( wxT( "/WindowPreferences/ConfigDialogVChartPreviewTransits" ), configDialogVChartPreviewTransits );

	// Notebooks
	cfg->Write( wxT( "Notebooks/AshtakaVarga" ), activePageAshtakaVarga );
	cfg->Write( wxT( "/Notebooks/Partner" ), avtivePagePartner );
	cfg->Write( wxT( "/Notebooks/Transit" ), avtivePageTransit );
	cfg->Write( wxT( "/Notebooks/VargaOverview" ), activePageVargaOverview );
	cfg->Write( wxT( "/Notebooks/ShadBala" ), activePageShadbala );
	cfg->Write( wxT( "/Notebooks/MultipleViewNotebookStyle" ), mViewNotebookStyle );
	cfg->Write( wxT( "/Notebooks/MultipleViewNotebookOrientation" ), mViewNotebookOrientation );
	cfg->Write( wxT( "/Notebooks/ConfigDialogActivePage" ), configDialogActivePage );

	// Signs
	cfg->Write( wxT( "/Signs/VedicSignNames" ), useVedicSignNames );
	cfg->Write( wxT( "/Signs/VedicPlanetNames" ), useVedicPlanetNames );
	cfg->Write( wxT( "/Signs/VedicPositions" ), useVedicPositions );
	cfg->Write( wxT( "/Signs/SignPrecision" ), signPrecision );
	cfg->Write( wxT( "/Signs/UseSignSymbols" ), useSignSymbols );
	cfg->Write( wxT( "/Signs/UsePlanetSymbols" ), usePlanetSymbols );
	cfg->Write( wxT( "/Signs/UranusSymbol" ), uranusSymbol );
	cfg->Write( wxT( "/Signs/PlutoSymbol" ), plutoSymbol );
	cfg->Write( wxT( "/Signs/CapricornSymbol" ), capricornSymbol );

	// Paths
	cfg->Write( wxT( "/Paths/DefaultOpenFiletype" ), defOpenFiletype );
	cfg->Write( wxT( "/Paths/DefaultTextExportType" ), defTextExportType );
	cfg->Write( wxT( "/Paths/ShowGraphicSizeDialog" ), showGraphicSizeDialog );
	cfg->Write( wxT( "/Paths/DefaultGraphicExportFiletype" ), defGraphicExportType );
	cfg->Write( wxT( "/Paths/ExportAskOnOverwrite" ), exportAskOnOverwrite );

	cfg->Write( wxT( "/Paths/DefaultOpenPath" ), defOpenPath );
	cfg->Write( wxT( "/Paths/DefaultSavePath" ), defSavePath );
	cfg->Write( wxT( "/Paths/DefaultPdfSavePath" ), defPdfSavePath );
	cfg->Write( wxT( "/Paths/DefaultExportPath" ), defExportPath );
	cfg->Write( wxT( "/Paths/SwissEphemerisPath" ), sePath );

	// Default Location
	cfg->Write( wxT( "/DefaultLocation/LocationName" ), defaultLocation.getLocName() );
	writeDoubleEntry( wxT( "/DefaultLocation/Longitude" ), defaultLocation.getLongitude() );
	writeDoubleEntry( wxT( "/DefaultLocation/Latitude" ), defaultLocation.getLatitude() );

	// Animation
	cfg->Write( wxT( "/Animation/AnimateInterval" ), animateInterval );
	cfg->Write( wxT( "/Animation/AnimateMode" ), animateMode );
	cfg->Write( wxT( "/Animation/AnimateStepLength" ), animateStepLength );
	cfg->Write( wxT( "/Animation/AnimateStepDimension" ), animateStepDimension );

	// Ephemeris
	writeDoubleEntry( wxT( "/Ephemeris/Custom_t0" ), custom_t0 );
	writeDoubleEntry( wxT( "/Ephemeris/Custom_ayan_t0" ),  custom_ayan_t0 );
	writeDoubleEntry( wxT( "/Ephemeris/Custom_aya_period" ), custom_aya_period );
	cfg->Write( wxT( "/Ephemeris/Custom_aya_constant" ), custom_aya_constant );
	cfg->Write( wxT( "/Ephemeris/Sunrise_refrac" ), sunrise_refrac );
	cfg->Write( wxT( "/Ephemeris/Sunrise_def" ), sunrise_def );
	cfg->Write( wxT( "/Ephemeris/UseEphemerisTime" ), useEphemerisTime );
	cfg->Write( wxT( "/Ephemeris/EphemCoordinateSystem" ), ephemCoordinateSystem );
	cfg->Write( wxT( "/Ephemeris/UseTruePositions" ), useTruePositions );

	cfg->Write( wxT( "/Vedic/Ayanamsa" ), vAyanamsa );
	cfg->Write( wxT( "/Vedic/VedicObjects" ), vObjects );
	cfg->Write( wxT( "/Vedic/OrderLagna" ), vOrderLagna );
	cfg->Write( wxT( "/Vedic/VGraphicSkin" ), vGraphicSkin );
	cfg->Write( wxT( "/Vedic/HouseSystem" ), vHouseSystem );
	cfg->Write( wxT( "/Vedic/HouseWestern" ), vHouseWestern );
	cfg->Write( wxT( "/Vedic/LunarNodeMode" ), vLunarNodeMode );
	cfg->Write( wxT( "/Vedic/ArudhaMode" ), vArudhaMode );
	cfg->Write( wxT( "/Vedic/VargaHoraMode" ), vVargaHoraMode );
	cfg->Write( wxT( "/Vedic/VargaDrekkanaMode" ), vVargaDrekkanaMode );
	cfg->Write( wxT( "/Vedic/VargaChaturtamsaMode" ), vVargaChaturtamsaMode );
	cfg->Write( wxT( "/Vedic/NakshatraMode28" ), vNakshatraMode28 );
	cfg->Write( wxT( "/Vedic/NakshatraPortionMode" ), vNakshatraPortionMode );
	cfg->Write( wxT( "/Vedic/KalachakraMode" ), vKalachakraMode );
	cfg->Write( wxT( "/Vedic/AshtakavargaMode" ), vAshtakavargaMode );
	cfg->Write( wxT( "/Vedic/AshtakavargaSarva" ), vAshtakavargaSarva );
	cfg->Write( wxT( "/Vedic/AshtakavargaNumberMode" ), vAshtakavargaNumberMode );
	cfg->Write( wxT( "/Vedic/AshtakavargaMantreswarMode" ), vAshtakavargaMantreswarMode );
	cfg->Write( wxT( "/Vedic/YearLength" ), vYearLength );
	cfg->Write( wxT( "/Vedic/TempFriendBasedOnVarga" ), tempFriendBasedOnVarga );
	writeDoubleEntry( wxT( "/Vedic/Calculation/CustomYearLength" ), vCustomYearLength );

	cfg->Write( wxT( "/Vedic/MoonBenefic" ), vMoonBenefic );
	cfg->Write( wxT( "/Vedic/MercuryBenefic" ), vMercuryBenefic );

	// Arabic Parts
	writeIntArray( wxT( "/Vedic/ArabicParts" ), vArabicParts );

	// Kala Velas
	cfg->Write( wxT( "/KalaVelas/KalaLord" ), kalaVelaLord[0] );         // Sun
	cfg->Write( wxT( "/KalaVelas/MrityuLord" ), kalaVelaLord[1] );       // Mars
	cfg->Write( wxT( "/KalaVelas/ArdhapraharaLord" ), kalaVelaLord[2] ); // Mercury
	cfg->Write( wxT( "/KalaVelas/YamaghantakaLord" ), kalaVelaLord[3] ); // Jupiter
	cfg->Write( wxT( "/KalaVelas/GulikaLord" ), kalaVelaLord[4] );       // Saturn
	cfg->Write( wxT( "/KalaVelas/MandiLord" ), kalaVelaLord[5] );

	cfg->Write( wxT( "/KalaVelas/KalaPortion" ), kalaVelaPortion[0] );
	cfg->Write( wxT( "/KalaVelas/MrityuPortion" ), kalaVelaPortion[1] );
	cfg->Write( wxT( "/KalaVelas/ArdhapraharaPortion" ), kalaVelaPortion[2] );
	cfg->Write( wxT( "/KalaVelas/YamaghantakaPortion" ), kalaVelaPortion[3] );
	cfg->Write( wxT( "/KalaVelas/GulikaPortion" ), kalaVelaPortion[4]);
	cfg->Write( wxT( "/KalaVelas/MandiPortion" ), kalaVelaPortion[5] );

	// Western
	cfg->Write( wxT( "/Western/Ayanamsa" ), wAyanamsa );
	cfg->Write( wxT( "/Western/WesternObjects" ), wObjects );
	cfg->Write( wxT( "/Western/PlanetOrder" ), wPlanetOrder );
	cfg->Write( wxT( "/Western/HouseSystem" ), wHouseSystem );
	cfg->Write( wxT( "/Western/LunarNodeMode" ), wLunarNodeMode );
	cfg->Write( wxT( "/Western/WGraphicSkin" ), wGraphicSkin );
	cfg->Write( wxT( "/Western/YearLength" ), wYearLength );
	cfg->Write( wxT( "/Western/CustomYearLength" ), wCustomYearLength );

	// Arabic Parts
	writeIntArray( wxT( "/Western/ArabicParts" ), wArabicParts );

  // Aspects
  for( i = 0; i < MAX_ASPECT_TYPES; i++ )
  {
    s.Printf( wxT( "/Aspects/AspectTypes/%02d" ), i );
    cfg->Write( s, wAspectConfig[i] );
  }

	// Window Sizes
	cfg->Write( wxT( "/WindowSizes/xMasterWindow" ), xMasterWindow );
	cfg->Write( wxT( "/WindowSizes/yMasterWindow" ), yMasterWindow );

	cfg->Write( wxT( "/WindowSizes/xMultipleView" ), xMultipleView );
	cfg->Write( wxT( "/WindowSizes/yMultipleView" ), yMultipleView );

	cfg->Write( wxT( "/WindowSizes/xMainWindow" ), xMainWindow );
	cfg->Write( wxT( "/WindowSizes/yMainWindow" ), yMainWindow );

	cfg->Write( wxT( "/WindowSizes/xVedicGraphicWindow" ), xVedicGraphicWindow );
	cfg->Write( wxT( "/WindowSizes/yVedicGraphicWindow" ), yVedicGraphicWindow );

	cfg->Write( wxT( "/WindowSizes/xWesternGraphicWindow" ), xWesternGraphicWindow );
	cfg->Write( wxT( "/WindowSizes/yWesternGraphicWindow" ), yWesternGraphicWindow );

	cfg->Write( wxT( "/WindowSizes/xTextWindow" ), xTextWindow );
	cfg->Write( wxT( "/WindowSizes/yTextWindow" ), yTextWindow );

	cfg->Write( wxT( "/WindowSizes/xDasaWindow" ), xDasaWindow );
	cfg->Write( wxT( "/WindowSizes/yDasaWindow" ), yDasaWindow );

	cfg->Write( wxT( "/WindowSizes/xGraphicalDasaWindow" ), xGraphicalDasaWindow );
	cfg->Write( wxT( "/WindowSizes/yGraphicalDasaWindow" ), yGraphicalDasaWindow );

	cfg->Write( wxT( "/WindowSizes/xSbcWindow" ), xSbcWindow );
	cfg->Write( wxT( "/WindowSizes/ySbcWindow" ), ySbcWindow );

	cfg->Write( wxT( "/WindowSizes/xSolarWindow" ), xSolarWindow );
	cfg->Write( wxT( "/WindowSizes/ySolarWindow" ), ySolarWindow );

	cfg->Write( wxT( "/WindowSizes/xUranianWindow" ), xUranianWindow );
	cfg->Write( wxT( "/WindowSizes/yUranianWindow" ), yUranianWindow );

	cfg->Write( wxT( "/WindowSizes/xTransitWindow" ), xTransitWindow );
	cfg->Write( wxT( "/WindowSizes/yTransitWindow" ), yTransitWindow );

	cfg->Write( wxT( "/WindowSizes/xYogaWindow" ), xYogaWindow );
	cfg->Write( wxT( "/WindowSizes/yYogaWindow" ), yYogaWindow );

	cfg->Write( wxT( "/WindowSizes/xVargaSummaryWindow" ), xVargaSummaryWindow );
	cfg->Write( wxT( "/WindowSizes/yVargaSummaryWindow" ), yVargaSummaryWindow );

	cfg->Write( wxT( "/WindowSizes/xShadbalaWindow" ), xShadbalaWindow );
	cfg->Write( wxT( "/WindowSizes/yShadbalaWindow" ), yShadbalaWindow );

	cfg->Write( wxT( "/WindowSizes/xAshtakaVargaWindow" ), xAshtakaVargaWindow );
	cfg->Write( wxT( "/WindowSizes/yAshtakaVargaWindow" ), yAshtakaVargaWindow );

	cfg->Write( wxT( "/WindowSizes/xEphemWindow" ), xEphemWindow );
	cfg->Write( wxT( "/WindowSizes/yEphemWindow" ), yEphemWindow );

	cfg->Write( wxT( "/WindowSizes/xEclipseWindow" ), xEclipseWindow );
	cfg->Write( wxT( "/WindowSizes/yEclipseWindow" ), yEclipseWindow );

	cfg->Write( wxT( "/WindowSizes/xHoraWindow" ), xHoraWindow );
	cfg->Write( wxT( "/WindowSizes/yHoraWindow" ), yHoraWindow );

	cfg->Write( wxT( "/WindowSizes/xPartnerWindow" ), xPartnerWindow );
	cfg->Write( wxT( "/WindowSizes/yPartnerWindow" ), yPartnerWindow );

	cfg->Write( wxT( "/WindowSizes/xConfigDialog" ), xConfigDialog );
	cfg->Write( wxT( "/WindowSizes/yConfigDialog" ), yConfigDialog );

	cfg->Write( wxT( "/WindowSizes/xSimpleConfigDialog" ), xSimpleConfigDialog );
	cfg->Write( wxT( "/WindowSizes/ySimpleConfigDialog" ), ySimpleConfigDialog );

	cfg->Write( wxT( "/WindowSizes/xYogaEditor" ), xYogaEditor );
	cfg->Write( wxT( "/WindowSizes/yYogaEditor" ), yYogaEditor );

	cfg->Write( wxT( "/WindowSizes/SashPartnerDouble" ), sashPartnerDouble );
	cfg->Write( wxT( "/WindowSizes/SashPartnerVedic" ), sashPartnerVedic );
	cfg->Write( wxT( "/WindowSizes/SashPartnerComposite" ), sashPartnerComposite );
	cfg->Write( wxT( "/WindowSizes/SashTransit" ), sashTransit );
	cfg->Write( wxT( "/WindowSizes/SashSolar" ), sashSolar );

	// Print
	cfg->Write( wxT( "/Print/ShowPageFrame" ), printShowPageFrame );
	cfg->Write( wxT( "/Print/SaveFileOption" ), printSaveFileOption );
	cfg->Write( wxT( "/Print/ShowSelectDialog" ), printShowSelectDialog );
	cfg->Write( wxT( "/Print/AskOnOverwrite" ), printAskOverwrite );
	cfg->Write( wxT( "/Print/CompressPdf" ), printCompressPdf );
	cfg->Write( wxT( "/Print/LaunchPdfViewer" ), printLaunchPdfViewer );
	cfg->Write( wxT( "/Print/PdfViewerCommand" ), printPdfViewerCommand );
	cfg->Write( wxT( "/Print/CustomFooter" ), printCustomFooter );
	cfg->Write( wxT( "/Print/CustomFooterText" ), printCustomFooterText );
	cfg->Write( wxT( "/Print/DefaultPrintout" ), printDefaultPrintout );
	cfg->Write( wxT( "/Print/VedicGraphicSkin" ), printVGraphicSkin );
	cfg->Write( wxT( "/Print/WesternGraphicSkin" ), printWGraphicSkin );
	cfg->Write( wxT( "/Print/PaperFormat" ), printPaperFormat );

	FontConfig::get()->writeConfigEntries();
	cfg->Write( wxT( "/Fonts/Default" ), fontDefault );
	cfg->Write( wxT( "/Fonts/Header" ), fontHeader );
	cfg->Write( wxT( "/Fonts/Graphic" ), fontGraphic );
	cfg->Write( wxT( "/Fonts/Text" ), fontText );
	cfg->Write( wxT( "/Fonts/Symbol" ), fontSymbol );
	cfg->Write( wxT( "/Fonts/SymbolFontSize" ), symbolFontSize );

	cfg->Write( wxT( "/Fonts/PdfDefault" ), fontPdfDefault );
	cfg->Write( wxT( "/Fonts/PdfHeader" ), fontPdfHeader );
	cfg->Write( wxT( "/Fonts/PdfGraphic" ), fontPdfGraphic );
	cfg->Write( wxT( "/Fonts/PdfSymbol" ), fontPdfSymbol );
	cfg->Write( wxT( "/Fonts/PdfSymbolFontSize" ), pdfSymbolFontSize );

	// Atlas
	cfg->Write( wxT( "/Atlas/DatabaseFile" ), databaseFile );
	cfg->Write( wxT( "/Atlas/AtlasFavouriteCountries" ), atlasFavouriteCountries );
	cfg->Write( wxT( "/Atlas/AtlasFilterMode" ), atlasFilterMode );
	cfg->Write( wxT( "/Atlas/AtlasLastSelectedCountry" ), atlasLastSelectedCountry );
	cfg->Write( wxT( "/Atlas/AtlasGridColumnSizes" ), atlasGridColumnSizes );
	cfg->Write( wxT( "/Atlas/AtlasFilterCaseSensitive" ), atlasFilterCaseSensitive );
	cfg->Write( wxT( "/Atlas/AtlasCacheSize" ), atlasCacheSize );
	cfg->Write( wxT( "/Atlas/xAtlasDialog" ), xAtlasDialog );
	cfg->Write( wxT( "/Atlas/yAtlasDialog" ), yAtlasDialog );

	// show warning dialogs
	cfg->Write( wxT( "/WarningDialogs/DoNotShowAgainWarningDFonts" ), doNotShowAgainWarningDFonts );
	cfg->Write( wxT( "/WarningDialogs/DoNotShowAgainWarningEphemFiles" ), doNotShowAgainWarningEphemFiles );
	cfg->Write( wxT( "/WarningDialogs/DoNotShowAgainAfterLanguageChange" ), doNotShowAgainAfterLanguageChange );
	cfg->Write( wxT( "/WarningDialogs/DoNotShowAgainEphemConfigChange" ), doNotShowAgainEphemConfigChange );

	cfg->Flush();
}

