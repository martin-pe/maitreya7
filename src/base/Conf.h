/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/base/Conf.h
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
#ifndef _CONFIG_H_
#define _CONFIG_H_

#ifdef __GNUG__
#pragma interface "Conf.h"
#endif

#include <wx/colour.h>
#include <wx/string.h>

#include "constants.h"
#include "DataSet.h"

class wxConfigBase;
class ColorConfig;

/*************************************************//**
*
* \brief contains all configurations of the application
*
******************************************************/
class Config
{
public:
	Config();
	~Config();

	void readAll();
	void writeAll();

	ColorConfig *colors;

	// General
	bool preferVedic;
	Location defaultLocation;
	int backupFileMode;

	// Main window options
	bool showStatusInfo;
	bool showStatusBar;
	int maxRecentFiles;
	int listStyle;
	bool showMainToolbar;
	wxString mainToolbarItems;
	bool openNewDocOnStart;
	bool askOnQuit;
	int childViewFrameWidth;
	bool showTextViewHeaders;

	// Multiple views
	bool useMultipleViews;
	int defaultMultipleView;
	int mViewNotebookStyle;
	int mViewNotebookOrientation;

	// astrological display options
	bool useVedicSignNames;
	bool useVedicPlanetNames;
	bool useVedicPositions;
	int signPrecision;
	bool useSignSymbols;
	bool usePlanetSymbols;
	int uranusSymbol;
	int plutoSymbol;
	int capricornSymbol;

	// graphical dasa
	int dasaCornerRadius;
	int antarDasaLevelChoice;
	int dasaZoomChoice;

	// Fonts
	wxString fontDefault, fontHeader, fontGraphic, fontText, fontSymbol;
	wxString fontPdfDefault, fontPdfHeader, fontPdfGraphic, fontPdfSymbol;
	int symbolFontSize;
	int pdfSymbolFontSize;

	// Atlas
	wxString databaseFile;
	wxString atlasFavouriteCountries;
	int atlasFilterMode;
	wxString atlasLastSelectedCountry;
	wxString atlasGridColumnSizes;
	bool atlasFilterCaseSensitive;
	int atlasCacheSize;
	int xAtlasDialog, yAtlasDialog;

	// files, paths etc
	wxString recentFileList;
	wxString defOpenPath, defSavePath, defExportPath, sePath, defPdfSavePath;
	int defOpenFiletype;
	int defTextExportType;
	int defGraphicExportType;
	bool showGraphicSizeDialog;
	bool exportAskOnOverwrite;

	// Animation
	int animateInterval;
	int animateMode;
	int animateStepLength;
	int animateStepDimension;

	// Ephemeris
	bool useEphemerisTime;
	int ephemCoordinateSystem;
	bool useTruePositions;
	double custom_t0;
	double custom_ayan_t0;
	double custom_aya_period;
	bool custom_aya_constant;
	bool sunrise_refrac;
	int sunrise_def;

	int ephemTimezone;
	int ephemMode;
	int ephemCircleType;

	// Vedic
	int vAyanamsa;
	int vObjects;
	int vOrderLagna;
	VedicGraphicStyle vGraphicStyle;
	int vHouseSystem;
	bool vHouseWestern;
	int vLunarNodeMode;
	int vArudhaMode;
	int vVargaHoraMode;
	int vVargaDrekkanaMode;
	int vVargaChaturtamsaMode;

	int vNakshatraMode28;
	int vNakshatraPortionMode;
	int vKalachakraMode;
	int vAshtakavargaMode;
	int vAshtakavargaSarva;
	int vAshtakavargaNumberMode;
	bool vAshtakavargaMantreswarMode;
	int vMainChildStyle;
	bool tempFriendBasedOnVarga;
	int vDefaultDasaList;
	int vYearLength;
	double vCustomYearLength;
	int kalaVelaLord[6];
	int kalaVelaPortion[6];
	wxArrayInt vArabicParts;
	int vMoonBenefic;
	int vMercuryBenefic;
	int vGraphicSkin;

	// Yogas
	int vYogaSourceFilter;
	int vYogaGroupFilter;
	int vYogaCustomFilter;
	wxString lastYogaConfigFile;

	// Western
	int wAyanamsa;
	int wObjects;
	int wPlanetOrder;
	int wHouseSystem;
	WesternGraphicStyle wGraphicStyle;
	int wChartOrientation;
	int wLunarNodeMode;
	int wMainChildStyle;
	int wGraphicSkin;
	int wYearLength;
	double wCustomYearLength;
	wxArrayInt wArabicParts;

	int transitModus;
	int orbisRadix;
	int orbisTransit;
	int orbisPartner;
	int circleType;
	int planetSortOrder;
	int transitTimezone;

	// Aspects
	wxString wAspectConfig[MAX_ASPECT_TYPES];

	// Print
	bool printShowPageFrame;
	bool printSaveFileOption;
	bool printShowSelectDialog;
	int printDefaultPrintout;
	int printPaperFormat;
	bool printLaunchPdfViewer;
	bool printAskOverwrite;
	bool printCompressPdf;
	int printVGraphicSkin;
	int printWGraphicSkin;
	wxString printPdfViewerCommand;
	bool printCustomFooter;
	wxString printCustomFooterText;

	// Window sizes and positions
	int xMasterWindow, yMasterWindow, xPosMasterWindow, yPosMasterWindow;
	int xMultipleView, yMultipleView;
	int xMainWindow, yMainWindow;
	int xVedicGraphicWindow, yVedicGraphicWindow;
	int xWesternGraphicWindow, yWesternGraphicWindow;
	int xTextWindow, yTextWindow;
	int xDasaWindow, yDasaWindow;
	int xGraphicalDasaWindow, yGraphicalDasaWindow;
	int xSbcWindow, ySbcWindow;
	int xSolarWindow, ySolarWindow;
	int xUranianWindow, yUranianWindow;
	int xTransitWindow, yTransitWindow;
	int xYogaWindow, yYogaWindow;
	int xVargaSummaryWindow, yVargaSummaryWindow;
	int xShadbalaWindow, yShadbalaWindow;
	int xAshtakaVargaWindow, yAshtakaVargaWindow;
	int sashPartnerDouble, sashPartnerComposite, sashPartnerVedic, sashSolar, sashTransit;
	int xEphemWindow, yEphemWindow;
	int xEclipseWindow, yEclipseWindow;
	int xHoraWindow, yHoraWindow;
	int xPartnerWindow, yPartnerWindow;
	int xConfigDialog, yConfigDialog;
	int xSimpleConfigDialog, ySimpleConfigDialog;
	int xYogaEditor, yYogaEditor;

	// convenience params for active notebook etc. pages
	bool configDialogWChartPreviewTransits;
	bool configDialogVChartPreviewTransits;
	int avtivePagePartner, avtivePageTransit, activePageAshtakaVarga, activePageVargaOverview, activePageShadbala;
	int textWindowPage;
	int uranianWindowPage;
	int partnerStyle;
	int configDialogActivePage;

	// show warning dialogs
	bool doNotShowAgainWarningDFonts;
	bool doNotShowAgainWarningEphemFiles;
	bool doNotShowAgainAfterLanguageChange;
	bool doNotShowAgainEphemConfigChange;

	// Misc options
	int textmode;
	wxString lang;
	wxString langList;
	int logMode;

private:

	wxConfigBase *cfg;
	double readDoubleEntry( wxString path, const double def = 0 );
	void writeDoubleEntry( wxString path, const double value );
	void readIntArray( wxArrayInt&, wxString key, wxString defaultValue );
	void writeIntArray( wxString key, const wxArrayInt& );
};



#endif

