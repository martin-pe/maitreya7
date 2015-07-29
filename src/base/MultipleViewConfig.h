/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/base/MultipleViewConfig.h
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
#ifndef _MULTIPLEVIEWCONFIG_H_
#define _MULTIPLEVIEWCONFIG_H_

#ifdef __GNUG__
#pragma interface "MultipleViewConfig.h"
#endif

#include "ChartConfig.h"
#include "constants.h"
#include "ViewNode.h"
#include "XmlConfigLoader.h"

using namespace std;

/*************************************************//**
*
* \brief configuration for a single mulitple view
*
******************************************************/
class MultipleViewConfig : public ViewNode, public BaseXmlConfig
{
public:

	MultipleViewConfig() : ViewNode( 0 ) {
		sizex = sizey = 500;
		window = 0;
	}
	virtual ~MultipleViewConfig();

	virtual void dump( wxString& );
	virtual wxString dump();
	virtual wxString dumpXml();

	//wxString name, description;
	ViewNode *window;
	int sizex, sizey;

private:
	DECLARE_CLASS( MultipleViewConfig )
};

/*************************************************//**
*
* \brief contains all multiple view definitions
*
******************************************************/
class MultipleViewConfigLoader : public XmlConfigLoader
{
	DECLARE_SINGLETON_WITHOUT_CONSTRUCTOR( MultipleViewConfigLoader )

public:
	MultipleViewConfig *getConfig( const int& );

protected:
  virtual BaseXmlConfig *createConfig( BaseXmlConfig* = 0 );

  virtual void readSpecificAttributes( BaseXmlConfig*, wxXmlNode* );

private:

	MultipleViewConfigLoader();

	int assignViewId( wxString );
	void parseWidgetVargas( WidgetNode *node, wxXmlNode *cur );
	ViewNode *parseWidgetNode( ViewNode*, wxXmlNode* );
	ViewNode *parseSplitterNode( ViewNode*, wxXmlNode* );
	ViewNode *parseWindowNode( ViewNode*, wxXmlNode* );
	MultipleViewConfig *parseMultipleViewConfig( wxXmlNode* );
	ViewNode *parseNotebookNode( ViewNode*, wxXmlNode* );
	PageNode *parsePageNode( ViewNode*, wxXmlNode* );

};

#endif


