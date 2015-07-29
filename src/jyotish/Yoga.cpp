/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/Yoga.cpp
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
#pragma implementation "Yoga.h"
#endif

#include "Yoga.h"

#include "Horoscope.h"
#include "MdlInterpreter.h"
#include "Expression.h"
#include "Varga.h"
#include "VargaHoroscope.h"
#include "Writer.h"
#include "YogaConfig.h"

#include <wx/string.h>
#include <math.h>
#include <assert.h>
#include <stdio.h>

extern struct yy_buffer_state * yy_scan_string (const char *yy_str  );
extern int yyparse( void );
extern Rule *rule;
extern wxString parseErrorMessage;

extern int yylineno;

#define YOGA_PRINT_STACK_TRACE_CONSOLE
#define YOGA_PRINT_STACK_TRACE_GUI

IMPLEMENT_SINGLETON( YogaExpert )

/*****************************************************
**
**   YogaExpert   ---   Constructor
**
******************************************************/
YogaExpert::YogaExpert()
{
	interpreter = new MdlInterpreter();
	initialized = false;
}

/*****************************************************
**
**   YogaExpert   ---   init
**
******************************************************/
void YogaExpert::init()
{
	bool found;
	//vector<YogaConfig*> defs = YogaConfigLoader::get()->getConfigs();
	vector<BaseXmlConfig*> defs = YogaConfigLoader::get()->getConfigs();

	for ( unsigned int i = 0; i < defs.size(); i++ )
	{
		YogaConfig *cfg = (YogaConfig*)defs[i];

		// Setup groups
		found = false;
		for ( unsigned int j = 0; j < groups.size(); j++ )
		{
			if ( groups[j] == cfg->group )
			{
				found = true;
				break;
			}
		}
		if ( ! found ) groups.push_back( cfg->group );

		// Setup sources
		found = false;
		for ( unsigned int j = 0; j < sources.size(); j++ )
		{
			if ( sources[j] == cfg->source )
			{
				found = true;
				break;
			}
		}
		if ( ! found ) sources.push_back( cfg->source );

		// parse rule string if not already done
		if ( cfg->rule == 0 )
		{
			yy_scan_string( wxConvCurrent->cWX2MB( cfg->rulestr ) );
			int errorcode = yyparse();

			if ( errorcode != 0 )
			{
				printf( "Parse error, rule for Yoga #%d will be NULL, rule was \"%s\"", (int)i, str2char( cfg->rulestr ));
				cfg->rule = 0;
			}
			else cfg->rule = rule;
			rule = 0;
		}
	}
	initialized = true;
}

/*****************************************************
**
**   YogaExpert   ---   clear
**
******************************************************/
void YogaExpert::clear()
{
	sources.clear();
	groups.clear();

	clearErrors();

	delete YogaConfigLoader::get();
	initialized = false;
}

/*****************************************************
**
**   YogaExpert   ---   parseRule
**
******************************************************/
Rule *YogaExpert::parseRule( wxString s, wxString &msg )
{
	yylineno = 0;
	yy_scan_string( wxConvCurrent->cWX2MB( s ));
	int ret = yyparse();
	if ( ret != 0 )
	{
		msg = parseErrorMessage;
		return 0;
	}
	else assert( rule );
	if ( rule->hasErrors )
	{
		if ( rule->errorStack.size() > 0 ) msg = rule->errorStack[0];
		else msg = wxT( "Unknown error in Rule class: error stack empty" );
#ifdef YOGA_PRINT_STACK_TRACE_CONSOLE
		rule->printStackTrace();
#endif
		return 0;
	}
	msg = _( "Rule successfully parsed" );
	return rule;
}

/*****************************************************
**
**   YogaExpert   ---   evaluateRule
**
******************************************************/
bool YogaExpert::evaluateRule( AbstractVargaHoroscope *evaluationChart, Rule *rule, wxString &msg )
{
	yylineno = 0;
	interpreter->setChart( evaluationChart );
	int ret = interpreter->evaluateYogaRule( rule );

	if ( interpreter->hasErrors )
	{
		if ( interpreter->errorStack.size() > 0 ) msg = interpreter->errorStack[0];
		else msg = wxT( "Unknown interpreter error" );
		return false;
	}
	return (bool)ret;
}

/*****************************************************
**
**   YogaExpert   ---   update
**
******************************************************/
vector<unsigned int> YogaExpert::update( const VargaHoroscope *chart, const int artificialLagna, const wxString groupFilter,
        const wxString sourceFilter )
{
	vector<unsigned int> ret;
	unsigned int i;
	YogaConfigLoader *loader = YogaConfigLoader::get();

	if ( ! initialized || loader->needsReload() ) init();

	clearErrors();

	interpreter->setChart( chart );
	if ( artificialLagna != -1 ) interpreter->setLagna( artificialLagna );

	vector<BaseXmlConfig*> defs = loader->getConfigs();

	for ( i = 0; i < defs.size(); i++ )
	{
		YogaConfig *cfg = (YogaConfig*)defs[i];

		if ( chart->getDivision() > 0 && cfg->allowHigherVargas == false ) continue;
		if (  groupFilter != wxEmptyString && cfg->group != groupFilter )  continue;
		if (  sourceFilter != wxEmptyString && cfg->source != sourceFilter )  continue;

		if ( cfg->rule == 0 )
		{
			printf( "Error: Rule NULL, continue\n" );
			continue;
		}
		int a = interpreter->evaluateYogaRule( cfg->rule );

#ifdef YOGA_PRINT_STACK_TRACE_CONSOLE
		if ( interpreter->hasErrors )
		{
			printf( "Error in Yoga\n" );
			printf( "Group: %s / Source: %s\n", str2char( cfg->group ), str2char( cfg->source ));
			printf( "Description: %s\n", str2char( cfg->description ));
			printf( "Rule: %s\n", str2char( cfg->rulestr ));
			interpreter->printStackTrace();
			printf( "\n" );
		}
#endif

		if ( a ) ret.push_back( i );
		if ( interpreter->hasErrors )
		{
			addError( wxT( "Rule: " + cfg->rulestr ));
			appendErrors( interpreter );
		}
	}
	return ret;
}

/*****************************************************
**
**   YogaExpert   ---   write
**
******************************************************/
void YogaExpert::write( Writer *writer, vector<unsigned int> list )
{
	unsigned int i, j;

	if ( ! initialized ) init();

	//vector<YogaConfig*> defs = YogaConfigLoader::get()->getConfigs();
	vector<BaseXmlConfig*> defs = YogaConfigLoader::get()->getConfigs();

	if ( defs.size() == 0 || list.size() == 0 )
	{
		writer->writeParagraph( _( "No matching Yogas found." ));
		return;
	}
	Table table( 4, list.size() + 1 );
	table.setHeader( 0, _( "Group" ));
	table.setHeader( 1, _( "Description" ));
	table.setHeader( 2, _( "Effect" ));
	table.setHeader( 3, _( "Source" ));
	table.col_break[1] = true;
	table.col_break[2] = true;

	for ( i = 0; i < list.size(); i++ )
	{
		j = list[i];
		assert( j >= 0 && j < defs.size() );

		YogaConfig *cfg = (YogaConfig*)defs[j];

		table.setEntry( 0, i+1, cfg->group );
		table.setEntry( 1, i+1, cfg->description );
		table.setEntry( 2, i+1, cfg->effect );
		table.setEntry( 3, i+1, cfg->source );
	}
	writer->writeTable( table );

#ifdef YOGA_PRINT_STACK_TRACE_GUI
	if ( errorStack.size() > 0 )
	{
		writer->writeHeader2( wxT( "Error Stack" ));
		for ( unsigned int i = 0; i < errorStack.size(); i++ )
		{
			writer->writeLine( errorStack[i] );
		}
	}
#endif
}

