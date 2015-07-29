/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/base/XmlConfigLoader.cpp
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
#pragma implementation "XmlConfigLoader.h"
#endif

#include "XmlConfigLoader.h"

#include "Conf.h"
#include "FileConfig.h"
#include "mathbase.h"

#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/log.h>
#include <wx/tokenzr.h>
#include <wx/xml/xml.h>

extern Config *config;

const wxChar *xml_core_name[MAX_XML_TOPICS] = { wxT( "MultipleViewConfigs" ), wxT( "WesternChartConfigs" ),
	wxT( "VedicChartConfigs" ), wxT( "PrintoutConfigs" ) };

IMPLEMENT_CLASS( BaseXmlConfig, wxObject )

/*****************************************************
**
**   BaseXmlConfig   ---   Constructor
**
******************************************************/
BaseXmlConfig::BaseXmlConfig()
{
	name = wxT( "unknown" );
}

/*****************************************************
**
**   XmlConfigLoader   ---   Constructor
**
******************************************************/
XmlConfigLoader::XmlConfigLoader( const int &topic )
		: topic ( topic )
{
	assert( topic >= -1 && topic < MAX_XML_TOPICS );
	doc = 0;
}

/*****************************************************
**
**   XmlConfigLoader   ---   getConfigInternal
**
******************************************************/
BaseXmlConfig *XmlConfigLoader::getConfigInternal( const int&i )
{
	if ( needsReload())
	{
		clearConfigs();
		initConfigs();
	}

	// maybe call for the first time
	if ( configs.size() == 0 ) initConfigs();

	if ( i >= 0 && i < (int)configs.size() )
	{
		return configs[i];
	}
	else
	{
		printf( "Error: cannot load XML config for index %d, size of configs is %d\n", i, (int)configs.size());
		assert( configs.size() > 0 );
		return configs[0];
	}
}

/*****************************************************
**
**   XmlConfigLoader   ---   getConfigs
**
******************************************************/
vector<BaseXmlConfig*> &XmlConfigLoader::getConfigs()
{
	if ( needsReload() ||  configs.size() == 0 ) initConfigs();
	return configs;
}

/*****************************************************
**
**   XmlConfigLoader   ---   initConfigs
**
******************************************************/
void XmlConfigLoader::initConfigs()
{
	clearConfigs();
	readConfig();
	if ( configs.size() == 0 )
	{
		printf( "Error: cannot load XML file, creating fallback\n" );
		BaseXmlConfig *cfg = createConfig();
		configs.push_back( cfg );
	}
}

/*****************************************************
**
**   XmlConfigLoader   ---   clearConfigs
**
******************************************************/
void XmlConfigLoader::clearConfigs()
{
	for( unsigned int i = 0; i < configs.size(); i++ )
	{
		delete configs[i];
	}
	configs.clear();
}

/*****************************************************
**
**   XmlConfigLoader   ---   readConfig
**
******************************************************/
void XmlConfigLoader::readConfig()
{
	wxString filename;

	assert( topic >= 0 && topic < MAX_XML_TOPICS );
	clearConfigs();

	if ( topic < 4 )
	{
		//XML_TOPIC_MULTIPLE_VIEWS = 0, XML_TOPIC_WESTERN_CHART, XML_TOPIC_VEDIC_CHART, XML_TOPIC_PRINTOUT,
		filename = FileConfig::get()->getXmlDir() + xml_core_name[topic] + wxT( ".xml" );
		parseFile( filename, configs );
	}
	else
	{
		//XML_TOPIC_YOGA
    wxString yogadir = FileConfig::get()->getYogaDir();
    modtime.clear();

    wxDir dir( yogadir );
    if ( ! dir.IsOpened() )
    {
      printf( "Error: cannot open Yoga config directory %s", str2char( yogadir ));
    }
    else
    {
      bool cont = dir.GetFirst( &filename, wxT( "*.xml" ), wxDIR_FILES );
      while ( cont )
      {
        // Windows implementation doesn't filter extension
        if ( filename.Right( 4 ) == wxT( ".xml" ))
        {
          parseFile( yogadir + filename, configs );
        }
        cont = dir.GetNext(&filename);
      }
    }
	}
}

/*****************************************************
**
**   XmlConfigLoader   ---   saveFile
**
******************************************************/
bool XmlConfigLoader::saveFile( wxString filename, vector<BaseXmlConfig*> &v )
{
	static wxChar *groupname[5] = { XML_ROOT_MULTIPLE_VIEWS, XML_ROOT_VEDIC_CHART, XML_ROOT_WESTERN_CHART, XML_ROOT_PRINTOUT, XML_ROOT_YOGA };
	wxString s;

	wxTextFile wfile( filename );

	if ( ! wfile.Exists() )
	{
		if ( ! wfile.Create( filename ) )
		{
			wxLogError( wxT( "Cannot create file %s" ), (const wxChar*)filename );
			return false;
		}
	}
	else
	{
		if ( ! wfile.Open( FILE_CONF_MODE ) )
		{
			wxLogError( wxT( "Cannot open file %s" ), (const wxChar*)filename );
			return false;
		}
		wfile.Clear();
	}

	s = wxT( "<?xml version=\"1.0\"?>" );
	wfile.AddLine( s );
	s.Clear();
	s << wxT( "<!DOCTYPE " ) << groupname[topic] << wxT( " SYSTEM \"" ) << groupname[topic] << wxT( ".dtd\">" );
	wfile.AddLine( s );
	s.Clear();
	s << wxT( "<" ) << groupname[topic] << wxT( ">" );
	wfile.AddLine( s );

	for ( unsigned int i = 0; i < v.size(); i++ )
	{
		wfile.AddLine( v[i]->dumpXml() );
	}

	s.Clear();
	s << wxT( "</" ) << groupname[topic] << wxT( ">" );
	wfile.AddLine( s );

	wfile.Write( wxTextFileType_None, FILE_CONF_MODE );
	wfile.Close();
	return true;
}

/*****************************************************
**
**   XmlConfigLoader   ---   parseFile
**
******************************************************/
bool XmlConfigLoader::parseFile( wxString filename, vector <BaseXmlConfig*>& c )
{
	if ( doc ) delete doc;

	if ( ! wxFileName::FileExists( filename ))
	{
		printf( "ERROR file %s does not exist\n", str2char( filename ));
		return false;
	}
	if ( ! wxFileName::IsFileReadable( filename ))
	{
		printf( "ERROR file %s is not readable\n", str2char( filename ));
		return false;
	}
	if ( wxFileName::GetSize( filename ) == 0 )
	{
		printf( "ERROR file %s has NULL size\n", str2char( filename ));
		return false;
	}

	doc = new wxXmlDocument( filename );
	if ( ! doc->IsOk() )
	{
		delete doc;
		doc = 0;
		printf( "PARSE ERROR in file %s\n", str2char( filename ));
		return false;
	}
	modtime[filename] = ::wxFileModificationTime( filename ) ;

	// Allesfresser: interessiert ihn nicht, wie der Root Node heisst
	wxXmlNode *cur = doc->GetRoot()->GetChildren();

	while ( cur != 0 )
	{
		BaseXmlConfig *cfg = 0;
		if ( cur->GetName() == XML_NODE_CONFIG)
		{
			wxString name;
			wxString description;
			wxXmlProperty *prop = cur->GetProperties();
			while( prop )
			{
				if ( ! prop->GetName().CmpNoCase( XML_NODE_EXTENDS ))
				{
					cfg = copyConfig( prop->GetValue());
				}
				else if ( ! prop->GetName().CmpNoCase( XML_NODE_NAME ))
				{
					name = prop->GetValue();
				}
				else if ( ! prop->GetName().CmpNoCase(  XML_NODE_DESCRIPTION )) description = prop->GetValue();
				else reportWrongKey( prop->GetName() );
				prop = prop->GetNext();
			}

			if ( cfg == 0 ) cfg = createConfig();
			cfg->name = name;
			cfg->description = description;

			readSpecificAttributes( cfg, cur->GetChildren());

			c.push_back( cfg );
		}
		else reportWrongRootNode( cur->GetName() );
		cur = cur->GetNext();
	}
	delete doc;
	doc = 0;

	return true;
}

/*****************************************************
**
**   XmlConfigLoader   ---   copyConfig
**
******************************************************/
BaseXmlConfig *XmlConfigLoader::copyConfig( wxString name )
{
	BaseXmlConfig *cfg = 0;
	for ( unsigned int i = 0; i < configs.size(); i++ )
	{
		if ( configs[i]->name == name )
		{
			cfg = createConfig( configs[i] );
			break;
		}
	}
	return cfg;
}

/*****************************************************
**
**   XmlConfigLoader   ---   needsReload
**
******************************************************/
bool XmlConfigLoader::needsReload()
{
	for ( FileModTimeHash::iterator it = modtime.begin(); it != modtime.end(); it++)
	{
		if ( modtime[it->first] != ::wxFileModificationTime( it->first ))
		{
			return true;
		}
	}
	return false;
}

/*****************************************************
**
**   XmlConfigLoader   ---   dumpFile
**
******************************************************/
bool XmlConfigLoader::dumpFile( wxString filename )
{
	// TODO
	return true;
}

/*****************************************************
**
**   XmlConfigLoader   ---   reportWrongKey
**
******************************************************/
void XmlConfigLoader::reportWrongKey( wxString k )
{
	if ( k == wxT( "comment" ))
	{
		// nothing
	}
	else
	{
		printf( "Error: wrong key %s in XML document\n", str2char( k ));
		//assert( false );
	}
}

/*****************************************************
**
**   XmlConfigLoader   ---   reportWrongRootNode
**
******************************************************/
void XmlConfigLoader::reportWrongRootNode( wxString k )
{
	if ( k == wxT( "comment" ))
	{
		// nothing
	}
	else
	{
		printf( "Error: wrong root node %s in XML document\n", str2char( k ));
	}
}

