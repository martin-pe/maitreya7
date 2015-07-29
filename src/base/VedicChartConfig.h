/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/base/VedicChartConfig.h
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

#ifndef _VEDICCHARTCONFIG_H_
#define _VEDICCHARTCONFIG_H_

#ifdef __GNUG__
#pragma interface "VedicChartConfig.h"
#endif

#include "ChartConfig.h"
#include "constants.h"
#include "XmlConfigLoader.h"

/*************************************************//**
*
* 
*
******************************************************/
class VedicChartConfig : public ChartConfig
{
	DECLARE_CLASS( VedicChartConfig )
public:

	VedicChartConfig();

	wxString toString();

	virtual void setDefaultConfig();

	virtual wxString dump();
	virtual wxString dumpXml();

	GcRectangle outerRectangle;

	GcRegion markedFields;

	bool useSymbols;
};

/*************************************************//**
*
*
*
******************************************************/
class VedicChartConfigLoader : public ChartConfigLoader
{
	DECLARE_SINGLETON_WITHOUT_CONSTRUCTOR( VedicChartConfigLoader )

public:
	VedicChartConfig *getConfig( const int& );

protected:
  virtual BaseXmlConfig *createConfig( BaseXmlConfig* = 0 );

  virtual void readSpecificAttributes( BaseXmlConfig*, wxXmlNode* );

private:

	VedicChartConfigLoader();

};


#endif

