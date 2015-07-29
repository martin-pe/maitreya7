/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/base/FileConfig.h
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
#ifndef _FILECONFIG_H_
#define _FILECONFIG_H_

#ifdef __GNUG__
#pragma interface "FileConfig.h"
#endif

#include <wx/string.h>

/*************************************************//**
*
* \brief Provides paths and file names
*
******************************************************/
class FileConfig
{
public:

	static FileConfig *get();
	~FileConfig();

	void init( const wxChar *datadir );

	wxString getLicenseFile() { return licensefile; }
	wxString getPicDir() { return picdir; }
	wxString getXmlDir() { return xmldir; }
	wxString getYogaDir() { return yogadir; }
	wxString getFontDir() { return fontdir; }

	wxString getBinDir() { return bindir; }
	wxString getConfigDir() { return configdir; }
	wxString getWorkingDir() { return workingdir; }
	wxString getPathSeparator() { return separator; }

	wxString getUranianTextFile( const int& );

	bool backupFile( wxString filename, const bool doDelete = false );

	wxString getAtlasSqlFile();

private:

	FileConfig();
	static FileConfig *ego;

	void prepareConfigDir();

	wxString datadir, configdir, bindir, xmldir, yogadir, picdir, fontdir, locationdir, workingdir;
	wxString licensefile, separator;
};

#endif

