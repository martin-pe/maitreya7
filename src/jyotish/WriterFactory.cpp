/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/WriterFactory.cpp
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
#pragma implementation "WriterFactory.h"
#endif

#include "Writer.h"

#include "HtmlWriter.h"
#include "constants.h"
#include "TextWriter.h"

extern Config *config;

/*****************************************************
**
**   WriterFactory   ---   getWriter
**
******************************************************/
Writer *WriterFactory::getWriter( const int& type, Config *cfg )
{
	Writer *writer = 0;
	Config *c = cfg ? cfg : config;
	switch ( type )
	{
	case WRITER_TEXT:
		writer = new TextWriter( c );
		break;
	case WRITER_HTML:
		writer = new HtmlWriter( c );
		break;
	case WRITER_CSV:
		writer = new CsvWriter( c );
		break;
	default:
		assert( false );
		break;
	}
	return writer;
}

