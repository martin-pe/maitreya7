/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/base/PrintoutConfig.h
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
#ifndef _PRINTOUTCONFIG_H_
#define _PRINTOUTCONFIG_H_

#ifdef __GNUG__
#pragma interface "PrintoutConfig.h"
#endif

#include <vector>
#include <wx/string.h>

#include "constants.h"
#include "XmlConfigLoader.h"

using namespace std;

/**
*  Enumeration for PrintoutTypes
*/
enum { PRINTOUT_TYPE_TITLE = 0, PRINTOUT_TYPE_BASEDATA, PRINTOUT_TYPE_VEDIC_CHART_PAIR,
	PRINTOUT_TYPE_WESTERN_CHART, PRINTOUT_TYPE_DASA, PRINTOUT_TYPE_ASHTAKAVARGA, PRINTOUT_TYPE_SBC,
	PRINTOUT_TYPE_ASPECTARIUM, PRINTOUT_TYPE_URANIAN, PRINTOUT_TYPE_EPHEM
};

/*************************************************//**
*
* \brief single item in a printout config
*
******************************************************/
class PrintoutConfigItem
{
public:
	PrintoutConfigItem() {
		type = texttype = dasatype = varga1 = varga2 = 0;
	}

	/**
	 * \brief one of PRINTOUT_TYPE_*
	*/
	int type;

	/**
	 * \brief type of text output
	*/
	int texttype;

	/**
	 * \brief type of Dasa
	*/
	int dasatype;

	/**
	 * \brief first Varga of Vedic chart
	*/
	int varga1;

	/**
	 * \brief second Varga of Vedic chart
	*/
	int varga2;
};

/*************************************************//**
*
* \brief config for a single printout
*
******************************************************/
class PrintoutConfig : public BaseXmlConfig
{
	DECLARE_CLASS( PrintoutConfig )
public:

	PrintoutConfig() {
		pages = 0;
	}
	vector<PrintoutConfigItem> items;
	int pages;
	wxString toString();

	virtual wxString dump();
	virtual wxString dumpXml();
};

/*************************************************//**
*
* \brief reads and contains configs for printouts
*
******************************************************/
class PrintoutConfigLoader : public XmlConfigLoader
{
	DECLARE_SINGLETON_WITHOUT_CONSTRUCTOR( PrintoutConfigLoader )

public:
	PrintoutConfig *getConfig( const int& );

protected:
  virtual BaseXmlConfig *createConfig( BaseXmlConfig* = 0 );

  virtual void readSpecificAttributes( BaseXmlConfig*, wxXmlNode* );

private:

	PrintoutConfigLoader();

};

#endif

