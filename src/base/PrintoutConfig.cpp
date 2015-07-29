/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/base/PrintoutConfig.cpp
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
#pragma implementation "PrintoutConfig.h"
#endif

#include "PrintoutConfig.h"

#include "constants.h"

#include <math.h>
#include <wx/xml/xml.h>

IMPLEMENT_CLASS( PrintoutConfig, BaseXmlConfig )

/*****************************************************
**
**   PrintoutConfig   ---   toString
**
******************************************************/
wxString PrintoutConfig::toString()
{
	wxString s;
	s << wxT( "Name: " ) << name << Endl
	<< wxT( "Description: " ) << description << Endl
	<< wxT( "Pages: " ) << pages << Endl;
	for ( unsigned int i = 0; i < items.size(); i++ )
	{
		s << wxT( " - Item:   " ) << Endl
		<< wxT( "      Type:  " ) << items[i].type << Endl
		<< wxT( "  Texttype: " ) << items[i].texttype << Endl
		<< wxT( "  Dasatype: " ) << items[i].dasatype << Endl
		<< wxT( "    Varga1: " ) << items[i].varga1 << Endl
		<< wxT( "    Varga2: " ) << items[i].varga2 << Endl << Endl;
	}
	return s;
}

/*****************************************************
**
**   PrintoutConfig   ---   dump
**
******************************************************/
wxString PrintoutConfig::dump()
{
	// TODO
	printf( "Not implemented: PrintoutConfig::dump\n" );
	wxString s;
	return s;
}

/*****************************************************
**
**   PrintoutConfig   ---   dumpXml
**
******************************************************/
wxString PrintoutConfig::dumpXml()
{
	// TODO
	printf( "Not implemented: PrintoutConfig::dumpXml\n" );
	wxString s;
	return s;
}

PrintoutConfigLoader *PrintoutConfigLoader::ego = 0;

/*****************************************************
**
**   PrintoutConfigLoader   ---   Constructor
**
******************************************************/
PrintoutConfigLoader::PrintoutConfigLoader()
 : XmlConfigLoader( XML_TOPIC_PRINTOUT )
{
}

/*****************************************************
**
**   PrintoutConfigLoader   ---   getConfig
**
******************************************************/
PrintoutConfig *PrintoutConfigLoader::getConfig( const int &i )
{
	return (PrintoutConfig*)getConfigInternal( i );
}

/*****************************************************
**
**   PrintoutConfigLoader   ---   createConfig
**
******************************************************/
BaseXmlConfig *PrintoutConfigLoader::createConfig( BaseXmlConfig* base )
{
	if ( base )
	{
		PrintoutConfig *cfg = (PrintoutConfig*) base;
		return new PrintoutConfig( *cfg );
	}
	else return new PrintoutConfig;
}

/*****************************************************
**
**   PrintoutConfigLoader   ---   readSpecificAttributes
**
******************************************************/
void PrintoutConfigLoader::readSpecificAttributes( BaseXmlConfig *cfg, wxXmlNode *cur )
{
	PrintoutConfig *pconf = (PrintoutConfig*)cfg;
	wxString key, childkey;
	wxXmlNode *child;

	while ( cur )
	{
		key = cur->GetName();
		if ( key == XML_NODE_PAGES ) XMLNODE2INT( cur->GetNodeContent(), pconf->pages )
		else if ( key == XML_PRINTOUT_TYPE_BASE_DATA )
		{
			PrintoutConfigItem item;
			item.type = PRINTOUT_TYPE_BASEDATA;
			child = cur->GetChildren();
			while ( child )
			{
				childkey = child->GetName();
				if ( childkey == XML_NODE_STYLE ) XMLNODE2INT( child->GetNodeContent(), item.texttype );
				child = child->GetNext();
			}
			pconf->items.push_back( item );
		}
		else if ( key == XML_PRINTOUT_TYPE_VEDIC_CHART_PAIR )
		{
			PrintoutConfigItem item;
			item.type = PRINTOUT_TYPE_VEDIC_CHART_PAIR;
			child = cur->GetChildren();
			while ( child )
			{
				childkey = child->GetName();
				if ( childkey == XML_NODE_VARGA1 ) XMLNODE2INT( child->GetNodeContent(), item.varga1 );
				if ( childkey == XML_NODE_VARGA2 ) XMLNODE2INT( child->GetNodeContent(), item.varga2 );
				child = child->GetNext();
			}
			pconf->items.push_back( item );
		}
		else if ( key == XML_PRINTOUT_TYPE_WESTERN_CHART )
		{
			PrintoutConfigItem item;
			item.type = PRINTOUT_TYPE_WESTERN_CHART;
			pconf->items.push_back( item );
		}
		else if ( key == XML_PRINTOUT_TYPE_DASA )
		{
			PrintoutConfigItem item;
			item.type = PRINTOUT_TYPE_DASA;
			child = cur->GetChildren();
			while ( child )
			{
				childkey = child->GetName();
				if ( childkey == XML_NODE_DASA_TYPE ) XMLNODE2INT( child->GetNodeContent(), item.dasatype );
				if ( childkey == XML_NODE_STYLE ) XMLNODE2INT( child->GetNodeContent(), item.texttype );
				child = child->GetNext();
			}
			pconf->items.push_back( item );
		}
		else if ( key == XML_PRINTOUT_TYPE_ASHTAKAVARGA )
		{
			PrintoutConfigItem item;
			item.type = PRINTOUT_TYPE_ASHTAKAVARGA;
			pconf->items.push_back( item );
		}
		else if ( key == XML_PRINTOUT_TYPE_SBC )
		{
			PrintoutConfigItem item;
			item.type = PRINTOUT_TYPE_SBC;
			pconf->items.push_back( item );
		}
		else if ( key == XML_PRINTOUT_TYPE_URANIAN )
		{
			PrintoutConfigItem item;
			item.type = PRINTOUT_TYPE_URANIAN;
			pconf->items.push_back( item );
		}
		else if ( key == XML_PRINTOUT_TYPE_ASPECTARIUM )
		{
			PrintoutConfigItem item;
			item.type = PRINTOUT_TYPE_ASPECTARIUM;
			pconf->items.push_back( item );
		}
		else reportWrongKey( key );
		cur = cur->GetNext();
	}
}

