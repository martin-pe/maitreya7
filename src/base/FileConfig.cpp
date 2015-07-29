/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/base/FileConfig.cpp
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
#pragma implementation "FileConfig.h"
#endif

#include "FileConfig.h"

#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/utils.h>

#include "Conf.h"
#include "Lang.h"
#include "LanguageConfig.h"
#include "mathbase.h"

extern Config *config;

FileConfig *FileConfig::ego = 0;

/**************************************************************
***
**   FileConfig   ---   get
***
***************************************************************/
FileConfig *FileConfig::get()
{
	if ( ! ego ) ego = new FileConfig();
	return ego;
}

/*****************************************************
**
**   FileConfig   ---   Constructor
**
******************************************************/
FileConfig::FileConfig()
{
	separator = wxFileName::GetPathSeparator();
}

/*****************************************************
**
**   FileConfig   ---   Destructor
**
******************************************************/
FileConfig::~FileConfig()
{
	if ( ego ) delete ego;
	delete config;
}

/*****************************************************
**
**   FileConfig   ---   init
**
******************************************************/
void FileConfig::init( const wxChar *s )
{
	datadir = s;

	if ( ! datadir.EndsWith( separator )) datadir += separator;

	if ( ! wxDir::Exists( datadir ))
	{
		printf( "WARN: datadir does not exist\n" );
	}
	workingdir = wxGetCwd();
	picdir = datadir + PIC_DIR_NAME;
	if ( ! wxDir::Exists( picdir )) picdir = datadir + wxT( ".." ) + separator + PIC_DIR_NAME;

	xmldir = datadir + XML_DIR_NAME;
	if ( ! wxDir::Exists( xmldir )) xmldir = datadir + wxT( ".." ) + separator + XML_DIR_NAME;

	fontdir = datadir + FONT_DIR_NAME;
	if ( ! wxDir::Exists( fontdir )) fontdir = datadir + wxT( ".." ) + separator + FONT_DIR_NAME;

	locationdir = datadir + ATLAS_DIR_NAME;
	if ( ! wxDir::Exists( locationdir )) locationdir = datadir;

	if ( ! picdir.EndsWith( separator )) picdir += separator;
	if ( ! xmldir.EndsWith( separator )) xmldir += separator;
	if ( ! fontdir.EndsWith( separator )) fontdir += separator;
	if ( ! locationdir.EndsWith( separator )) locationdir += separator;

	prepareConfigDir(); // make dir ~/.maitreya7 and populate it with files (if not present)
	licensefile = datadir + LISENSE_FILE_NAME;

	// Try and error
	if ( ! wxFile::Access( licensefile, wxFile::read )) licensefile = datadir + wxT( "../" ) + LISENSE_FILE_NAME;
	if ( ! wxFile::Access( licensefile, wxFile::read )) licensefile = datadir + wxT( "../../" ) + LISENSE_FILE_NAME;
	if ( ! wxFile::Access( licensefile, wxFile::read )) licensefile = datadir + WIN_LISENSE_FILE_NAME;
	if ( ! wxFile::Access( licensefile, wxFile::read )) licensefile = datadir + wxT( "../" ) + WIN_LISENSE_FILE_NAME;
	if ( ! wxFile::Access( licensefile, wxFile::read )) licensefile = datadir + wxT( "../../" ) + WIN_LISENSE_FILE_NAME;

	config = new Config();
	config->readAll();  // depends on config dir/Linux

	LanguageConfig::get(); // depends on path variables
}

/*****************************************************
**
**   FileConfig   ---   prepareConfigDir
**
******************************************************/
void FileConfig::prepareConfigDir()
{
	wxString yogabasedir, fn, sourcefile, targetfile;

	wxGetHomeDir( &configdir );
	if ( configdir.Last() != separator ) configdir << separator;
	configdir << wxT( "." ) << CONFIG_DIR_NAME;

	if ( wxFileName::DirExists( configdir ))
	{
		if ( ! wxFileName::IsDirWritable( configdir ))
		{
			printf( "Fatal error: config directory \"%s\" not writable\n", str2char( configdir ) );
			exit( 1 );
		}
	}
	else
	{
		printf( "Creating config directory \"%s\" for the first time ...\n", str2char( configdir ));
		bool error = wxFileName::Mkdir( configdir, 0775 );
		if ( error != true )
		{
			printf( "Fatal error: cannot create config directory %s\n", str2char( configdir ));
			exit( 1 );
		}
	}

	// Create Yoga directory
	yogadir = configdir + separator + YOGA_TARGET_DIR_NAME + separator;
	yogabasedir = xmldir + YOGA_DIR_NAME;
	if ( wxFileName::DirExists( yogadir ))
	{
		if ( ! wxFileName::IsDirWritable( yogadir ))
		{
			printf( "Fatal error: yoga config directory \"%s\" not writable\n", str2char( yogadir ));
			exit( 1 );
		}
	}
	else
	{
		printf( "Creating yoga config directory \"%s\" for the first time ...\n", str2char( yogadir ));
		bool error = wxFileName::Mkdir( yogadir, 0775 );
		if ( error != true )
		{
			printf( "Fatal error: cannot create Yoga config directory %s\n", str2char( yogadir ));
			exit( 1 );
		}
	}

	// Copy Yoga definition files
	wxDir dir( yogabasedir );
	if ( !dir.IsOpened() )
	{
		printf( "Fatal error: cannot open Yoga config directory %s\n", str2char( yogabasedir ));
		exit( 1 );
	}

	bool cont = dir.GetFirst( &fn, wxT( "*.xml" ), wxDIR_FILES );
	while ( cont )
	{
		// Windows implementation doesn't filter extension
		if ( fn.Right( 4 ) == wxT( ".xml" ))
		{
			targetfile = yogadir + fn;
			if ( ! wxFile::Exists( targetfile ))
			{
				sourcefile = yogabasedir + separator + fn;
				printf( "Copy sourcefile %s to %s\n", str2char( sourcefile ), str2char( targetfile ));
				wxCopyFile( sourcefile, targetfile, false );
			}
		}
		cont = dir.GetNext(&fn);
	}
}

/*****************************************************
**
**   FileConfig   ---   getUranianTextFile
**
******************************************************/
wxString FileConfig::getUranianTextFile( const int &mode )
{
	wxString s = datadir;
	if ( s.Last() != separator ) s << separator;
	if ( mode ) s << wxT( "witte.txt" );
	else s << wxT( "ebertin.txt" );
	return s;
}

/*****************************************************
**
**   FileConfig   ---   backupFile
**
******************************************************/
bool FileConfig::backupFile( wxString filename, const bool doDelete )
{
	if ( config->backupFileMode == 0 ) return true;

	wxFileName fn( filename );
	fn.SetExt( wxT( "bak" ) );

	if ( config->backupFileMode == 2 && wxFileName::FileExists( fn.GetFullPath() ))
	{
		for ( int i = 1; i < 1000; i++ )
		{
			fn.SetExt( wxString::Format( wxT( "bak%d" ), i ));
			if ( ! wxFileName::FileExists( fn.GetFullPath() )) break;
		}
	}
	if ( doDelete ) return wxRenameFile( filename, fn.GetFullPath(), true );
	else return wxCopyFile( filename, fn.GetFullPath(), true );
}

/*****************************************************
**
**   FileConfig   ---   getAtlasSqlFile
**
******************************************************/
wxString FileConfig::getAtlasSqlFile()
{
	wxString filename = locationdir;
	if ( ! filename.EndsWith( separator )) filename << separator;

	filename << ATLAS_DEFAULT_SQLFILENAME;
	return filename;
}

/*****************************************************
**
**   FileConfig   ---   getAtlasDbFile
**
******************************************************/
/*
wxString FileConfig::getAtlasDbFile()
{
	wxString dbName; // = config->atlasDbFile;
	if ( dbName.IsEmpty())
	{
		dbName << getConfigDir() + ATLAS_DEFAULT_DBFILENAME;
		config->atlasDbFile = dbName;
	}
	return dbName;
}
*/

