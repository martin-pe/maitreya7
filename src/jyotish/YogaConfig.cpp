/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/YogaConfig.cpp
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
#pragma implementation "YogaConfig.h"
#endif

#include "YogaConfig.h"

#include "constants.h"
#include "Expression.h"
#include "FileConfig.h"

#include <wx/dir.h>
#include <wx/log.h>
#include <wx/textfile.h>
#include <wx/xml/xml.h>

IMPLEMENT_CLASS( YogaConfig, BaseXmlConfig )

/*****************************************************
**
**   YogaConfig   ---   Constructor
**
******************************************************/
YogaConfig::YogaConfig()
{
	rule = 0;
	description = wxT( "New Yoga" );
	allowHigherVargas = true;
}

/*****************************************************
**
**   YogaConfig   ---   Destructor
**
******************************************************/
YogaConfig::~YogaConfig()
{
	if ( rule ) delete rule;
}

/*****************************************************
**
**   YogaConfig   ---   dump
**
******************************************************/
wxString YogaConfig::dump()
{
	wxString s;
	s << wxT( " -----   Yoga -------" ) << Endl
	<< wxT( "Description: " ) << description << Endl
	<< wxT( "Group: " ) << group << Endl
	<< wxT( "Source: " ) << source << Endl
	<< wxT( "Effect: " ) << effect << Endl
	<< wxT( "AllowHigherVargas: " ) << allowHigherVargas << Endl
	<< wxT( "Rule: " ) << rulestr << Endl;
	return s;
}

/*****************************************************
**
**   YogaConfig   ---   dumpXml
**
******************************************************/
wxString YogaConfig::dumpXml()
{
	wxString s;
	//s << wxT( "	<YogaConfig>" ) << Endl
	s << wxT( "	<Config>" ) << Endl
	<< wxT( "		<Description>" ) << description << wxT( "</Description>" ) << Endl
	<< wxT( "		<Effect>" ) << effect << wxT( "</Effect>" ) << Endl
	<< wxT( "		<Group>" ) << group << wxT( "</Group>" ) << Endl
	<< wxT( "		<Source>" ) << source << wxT( "</Source>" ) << Endl
	<< wxT( "		<AllowHigherVargas>" ) << ( allowHigherVargas ? wxT( "true" ) : wxT( "false" )) << wxT( "</AllowHigherVargas>" ) << Endl
	<< wxT( "		<Rule>" ) << rulestr << wxT( "</Rule>" ) << Endl
	//<< wxT( "	</YogaConfig>" ) << Endl;
	<< wxT( "	</Config>" ) << Endl;
	return s;
}

YogaConfigLoader *YogaConfigLoader::ego = 0;

/*****************************************************
**
**   YogaConfigLoader   ---   Constructor
**
******************************************************/
YogaConfigLoader::YogaConfigLoader()
 : XmlConfigLoader( XML_TOPIC_YOGA )
{
}

/*****************************************************
**
**   YogaConfigLoader   ---   getConfig
**
******************************************************/
YogaConfig *YogaConfigLoader::getConfig( const int &i )
{
	return (YogaConfig*)getConfigInternal( i );
}

/*****************************************************
**
**   YogaConfigLoader   ---   createConfig
**
******************************************************/
BaseXmlConfig *YogaConfigLoader::createConfig( BaseXmlConfig* base )
{
	//printf( "YogaConfigLoader::createConfig base %ld\n", base );
	if ( base )
	{
		YogaConfig *cfg = (YogaConfig*) base;
		return new YogaConfig( *cfg );
	}
	else return new YogaConfig;
}

/*****************************************************
**
**   YogaConfigLoader   ---   readSpecificAttributes
**
******************************************************/
void YogaConfigLoader::readSpecificAttributes( BaseXmlConfig *cfg, wxXmlNode *cur )
{
	YogaConfig *yconf = (YogaConfig*)cfg;
	wxString key;

	while ( cur )
	{
		key = cur->GetName();
		if ( key == XML_NODE_GROUP ) yconf->group = cur->GetNodeContent();
		else if ( key == XML_NODE_DESCRIPTION ) yconf->description = cur->GetNodeContent();
		else if ( key == XML_NODE_EFFECT ) yconf->effect = cur->GetNodeContent();
		else if ( key == XML_NODE_SOURCE ) yconf->source = cur->GetNodeContent();
		else if ( key == XML_NODE_RULESTR ) yconf->rulestr = cur->GetNodeContent();
		else if ( key == XML_NODE_ALLOW_HIGHER_VARGAS ) XMLNODE2BOOLEAN( cur->GetNodeContent(), yconf->allowHigherVargas )
			else reportWrongKey( key );
		cur = cur->GetNext();
	}
}


