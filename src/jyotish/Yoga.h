/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/Yoga.h
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

#ifndef _YOGA_H_
#define _YOGA_H_

#ifdef __GNUG__
#pragma interface "Yoga.h"
#endif

#include <wx/string.h>
#include <vector>

#include "constants.h"
#include "Expression.h"

class AbstractVargaHoroscope;
class MdlInterpreter;
class Rule;

using namespace std;

/*************************************************//**
*
* \brief singleton class encapsulates calculation and text report of Yogas
*
******************************************************/
class YogaExpert : public ExpressionErrorHandler
{
	DECLARE_SINGLETON_WITHOUT_CONSTRUCTOR( YogaExpert )

public:

	/**
	 * setup sources, groups and parse rules
	 */
	void init();

	/**
	 * update yogas according to VargaHoroscope, filter, group and Lagna
	 */
	vector<unsigned int> update( const class VargaHoroscope*, const int artificialLagna = -1,
	                             const wxString groupFilter = wxEmptyString, const wxString sourceFilter = wxEmptyString );

	/**
	 * write output to YogaView, text console or file
	 */
	void write( class Writer*, vector<unsigned int> );

	/**
	 * parse a rule with a given string representation
	 */
	Rule *parseRule( wxString s, wxString &msg );

	/**
	 * evaluate a Rule for a given chart 
	 */
	bool evaluateRule( AbstractVargaHoroscope *evaluationChart, Rule*, wxString &msg );

	/**
	 * get list of possible sources (for choice in YogaView)
	 */
	vector<wxString> getSources() const {
		return sources;
	}

	/**
	 * get list of possible groups (for choice in YogaView)
	 */
	vector<wxString> getGroups() const {
		return groups;
	}

	/**
	 *  cleanup groups, soources and Yoga definitions
	 */
	void clear();

private:

	YogaExpert();

	vector<wxString> sources;
	vector<wxString> groups;
	wxString groupFilter;
	int artificialLagna;
	MdlInterpreter *interpreter;
	bool initialized;

};

#endif


