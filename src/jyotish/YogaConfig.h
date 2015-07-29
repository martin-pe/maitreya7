/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/YogaConfig.h
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

#ifndef _YOGACONFIG_H_
#define _YOGACONFIG_H_

#ifdef __GNUG__
#pragma interface "YogaConfig.h"
#endif

#include <wx/string.h>
#include <vector>

#include "constants.h"
#include "XmlConfigLoader.h"

using namespace std;

/*************************************************//**
*
* \brief config and descrition for a single Yoga configuration
*
******************************************************/
class YogaConfig : public BaseXmlConfig
{
	DECLARE_CLASS( YogaConfig )
public:

	YogaConfig();
	~YogaConfig();

	virtual wxString dump();
	virtual wxString dumpXml();

	wxString group, effect, source, rulestr, sourcefile;
	bool allowHigherVargas;
	class Rule *rule;
};

/*************************************************//**
*
* \brief reads and contains the system wide Yoga configs
*
******************************************************/
class YogaConfigLoader : public XmlConfigLoader
{
	DECLARE_SINGLETON_WITHOUT_CONSTRUCTOR( YogaConfigLoader )

public:

	YogaConfig *getConfig( const int& );

protected:
  virtual BaseXmlConfig *createConfig( BaseXmlConfig* = 0 );

  virtual void readSpecificAttributes( BaseXmlConfig*, wxXmlNode* );

private:

	YogaConfigLoader();

};

#endif


