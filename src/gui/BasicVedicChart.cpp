/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/BasicVedicChart.cpp
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
#pragma implementation "BasicVedicChart.h"
#endif

#include "BasicVedicChart.h"

#include "ChartProperties.h"
#include "ColorConfig.h"
#include "Conf.h"
#include "Horoscope.h"
#include "Lang.h"
#include "mathbase.h"
#include "Painter.h"
#include "Varga.h"
#include "VedicChartConfig.h"
#include "Writer.h"

#include <math.h>

extern Config *config;

/*****************************************************
**
**   ChartContents   ---   clear
**
******************************************************/
void ChartContents::clear()
{
	textitems.clear();
	graphicitems.clear();
	planets.Clear();
}

/*****************************************************
**
**   BasicVedicChart   ---   Constructor
**
******************************************************/
BasicVedicChart::BasicVedicChart( const ChartProperties *chartprops, const int &field_count, const int chart_count )
		: GraphicalChart( chartprops ),
		field_count( field_count )
{
	this->chart_count = chart_count;
	assert( chartprops );
	assert( field_count == 12 || field_count == 28 );
	assert( chart_count == 1 || chart_count == 2 );

	for ( int i = 0; i < field_count; i++ )
	{
		contents.push_back( ChartContents() );
		tcontents.push_back( ChartContents() );
	}
	vconf = (VedicChartConfig*)VedicChartConfigLoader::get()->getConfig( chartprops->getVedicSkin() );
}

/*****************************************************
**
**   BasicVedicChart   ---   OnDataChanged
**
******************************************************/
void BasicVedicChart::OnDataChanged()
{
	assert( vconf );
	writeChartContents( 0 );
	if ( chart_count == 2 ) writeChartContents( 1 );
}

/*****************************************************
**
**   BasicVedicChart   ---   writeChartContents
**
******************************************************/
void BasicVedicChart::writeChartContents( const int &chart_id )
{
	int planet, field;
	wxString lname, sname, symbol;
	bool retro;
	Lang lang;
	Writer *writer = WriterFactory().getWriter( WRITER_TEXT );

	if ( chartprops->isBlank()) return;

	vector<ChartContents> &c = getChartContents( chart_id );
	for ( unsigned int i = 0; i < c.size(); i++ ) c[i].clear();

	for ( unsigned int i = 0; i < chartprops->getVedicPlanetList().size(); i++ )
	{
		planet = chartprops->getVedicPlanetList()[i];

		// Skip Ascendant for north Indian chart
		// TODO Was ist mit SBC??
		if ( planet == OASCENDANT && chartprops->getVedicGraphicStyle().indianChartType == VGRAPHIC_NORTH_INDIAN ) continue;

		field = getPlanetField( planet, chart_id );
		c[field].planets.Add( planet );

		lname = writer->getObjectName( planet, TLARGE, true );
		sname = writer->getObjectName( planet, TMEDIUM, true );

		// retrogression
		retro = ( chartprops->getVedicGraphicStyle().showRetro ) && getPlanetRetro( planet, chart_id )
		        && planet != OMEANNODE && planet != OMEANDESCNODE;
		if ( retro )
		{
			lname += wxT( " " );
			lname += _( "(R)" );
			sname += _( "R" );
		}

		// symbol
		symbol.Clear();
		if ( config->usePlanetSymbols && vconf->useSymbols && planet <= MAX_EPHEM_OBJECTS ) symbol = lang.getPlanetSymbolCode( planet );
		if ( ! symbol.IsEmpty() ) c[field].graphicitems.push_back( ChartGraphicItem( symbol, planet, retro ));
		else
		{
			ChartTextItem item( lname, sname, retro );
			c[field].textitems.push_back( item );
		}
	}
	delete writer;
}

/*****************************************************
**
**   BasicVedicChart   ---   paintInternal
**
******************************************************/
void BasicVedicChart::paintInternal( ChartConfig *cfg)
{
	assert( cfg );
	assert( cfg->GetClassInfo()->IsKindOf( CLASSINFO( VedicChartConfig )));
	vconf = (VedicChartConfig*)cfg;

	xmax = rect.width;
	ymax = rect.height;

	// set total chart width and height to 95%, so radius is 47.5%
	const double rfactor =  0.00475 * vconf->outerRectangle.radius;

	xr = xmax * rfactor;
	yr = ymax * rfactor;

	if ( ! chartprops->isBlank() ) paintChart();
}

/*****************************************************
**
**   BasicVedicChart   ---   getChartContents
**
******************************************************/
vector<ChartContents> &BasicVedicChart::getChartContents( const int &chart_id )
{
	assert( chart_id == 0 || chart_id == 1 );
	return ( chart_id ? tcontents : contents );
}

/*****************************************************
**
**   BasicVedicChart   ---   drawFieldText
**
******************************************************/
void BasicVedicChart::drawFieldText( const double &x, const double &y, const double &w, const double &h,
	const int &r, const int& align, const int &chart_id, const int border )
{
	drawFieldText( MyRect( x, y, w, h ), r, align, chart_id, border );
}

/*****************************************************
**
**   BasicVedicChart   ---   drawFieldText
**
******************************************************/
void BasicVedicChart::drawFieldText( MyRect rect, const int &r, const int& align, const int &chart_id, const int border )
{
	wxString s;

	/*
	painter->setPen( wxPen( *wxBLUE ));
	painter->drawRectangle( rect );
	painter->setDefaultPen();
	*/

	vector<ChartContents> &c = getChartContents( chart_id );

	if ( border > 0 )
	{
		if ( align & Align::Left )
		{
			rect.x += border;
			rect.width -= border;
		}
		else if ( align & Align::Right )
		{
			rect.width -= border;
		}
		if ( align & Align::Top )
		{
			rect.y += border;
			rect.height -= border;
		}
		else if ( align & Align::Bottom )
		{
			rect.height -= border;
		}
	}

	assert( (int)c.size() > r );

	// number of text items
	unsigned int titems = c[r].textitems.size();
	// number of graphic items
	unsigned int gitems = c[r].graphicitems.size();

	if ( titems == 0 && gitems == 0 ) return; // nothing to do

	// Maximum number of lines for complete contents
	int max_lines = (int)Max(rect.height/ text_height, 1 );
	//printf( "rect.height %f text_height %f max_lines %d\n", rect.height, text_height, max_lines );

	int total_lines = 0;

	// grahic items per line: put them into one line if more than 3, else 2 lines
	int gitems_pline = 0;
	if ( gitems > 3 )
	{
		total_lines = 2;
		gitems_pline = ( gitems + 1 ) / 2;
	}
	else if ( gitems > 0 )
	{
		total_lines = 1;
		gitems_pline = gitems;
	}

	// text items per line
	int titems_pline = 0;
	if ( titems > 0 )
	{
		const int left_lines = max_lines - total_lines;
		if ( left_lines <= 1 )
		{
			// put all items in one line, no place left
			titems_pline = titems;
			total_lines++;
		}
		else
		{
			// distribute the items over the left lines
			titems_pline = 1 + titems / left_lines;
			total_lines = total_lines + titems / titems_pline;
		}
	}

	int total_height = total_lines * text_height;
	int starty = rect.y;
	if ( align & Align::Bottom ) starty = rect.y + rect.height - total_height;
	else if ( align & Align::VCenter ) starty = rect.y + ( rect.height - total_height ) / 2;

	MyRect trect = rect;
	trect.y = starty;
	trect.height = text_height;

	// paint graphic items
	vector<ChartGraphicItem> g;
	unsigned int i = 0;
	int j = 0;
	painter->setSymbolFont( symbolzoom );
	while ( i < gitems )
	{
		g.push_back( c[r].graphicitems[i++] );
		if ( ++j >= gitems_pline )
		{
			drawGraphicItemLine( trect, g, align );
			j = 0;

			trect.y += text_height;
			g.clear();
		}
	}
	if ( g.size() > 0 )
	{
		drawGraphicItemLine( trect, g, align );
		trect.y += text_height;
	}

	// paint text items
	painter->setGraphicFont( textzoom );
	i = 0;
	j = 0;
	s.Clear();
	while ( i < titems )
	{
		if ( s.Len() > 0 ) s << wxT( " " );

		if ( titems_pline == 1
		        && c[r].textitems[i].longname.Len() * text_width < rect.width )
		{
			s << c[r].textitems[i++].longname;
		}
		else s << c[r].textitems[i++].shortname;

		if ( ++j >= titems_pline )
		{
			drawTextItemLine( trect, s, align );
			j = 0;
			trect.y += text_height;
			s.Clear();
		}
	}
	if ( s.Len() > 0 ) drawTextItemLine( trect, s, align );
}

/*****************************************************
**
**   BasicVedicChart   ---   drawSingleGraphicItem
**
******************************************************/
void BasicVedicChart::drawSingleGraphicItem( const MyRect &rect, const ChartGraphicItem &g )
{
	if ( vconf->useTextColors && chartprops->getVedicGraphicStyle().showPlanetColors )
	{
		painter->setTextColor( vconf->getPlanetColor( g.pindex ));
	}
	else
	{
		setDefaultTextColor();
	}
	painter->drawSimpleText( rect.x, rect.y, rect.width, rect.height, g.name );
	if ( g.retro ) painter->drawSimpleText( rect.x + text_width, rect.y+text_height/3, rect.width, rect.height, wxT( "_" ));
}

/*****************************************************
**
**   BasicVedicChart   ---   drawTextItemLine
**
******************************************************/
void BasicVedicChart::drawTextItemLine( MyRect rect, const wxString &s, const int &align )
{
	double h, w;
	setDefaultTextColor();
	painter->getTextExtent( s, &w, &h );

	if ( align & Align::Right ) rect.x = rect.x + rect.width - w;
	else if ( align & Align::HCenter ) rect.x = rect.x + .5 * ( rect.width - w );

	//painter->drawRectangle( rect );

	// TODO

	//painter->drawTextFormatted( rect, s, Align::Left + Align::Top );
	painter->drawTextFormatted( rect.x, rect.y, rect.width, rect.height, s, Align::Left + Align::Top );
}

/*****************************************************
**
**   BasicVedicChart   ---   drawGraphicItemLine
**
******************************************************/
void BasicVedicChart::drawGraphicItemLine( MyRect rect, const vector<ChartGraphicItem> g, const int &align )
{
	double leftx = rect.x;
	double xstep = 2 * text_width;
	double total_width = g.size() * xstep;

	if ( align &  Align::Right ) leftx = rect.x + rect.width - total_width;
	else if ( align & Align::HCenter ) leftx = rect.x + ( rect.width - total_width ) / 2;

	MyRect trect = rect;
	trect.x = leftx;
	trect.width = xstep;
	for ( unsigned int i = 0; i < g.size(); i++ )
	{
		drawSingleGraphicItem( trect, g[i] );
		trect.x += xstep;
	}
}

/*****************************************************
**
**   BasicVedicChart   ---   drawTextFormatted
**
******************************************************/
void BasicVedicChart::drawTextFormatted( const MyRect &rect, const wxString t, const int& align )
{
	painter->drawTextFormatted( rect.x, rect.y, rect.width, rect.height, t, align );
}

#define RTOL_DOUBLE_FRAME .012

/*****************************************************
**
**   BasicVedicChart   ---   paintOuterRectangle
**
******************************************************/
void BasicVedicChart::paintOuterRectangle()
{
  if ( ! vconf->outerRectangle.show ) return;

  painter->setPen( vconf->outerRectangle.pen.IsOk() ? vconf->outerRectangle.pen : defaultPen  );

  if ( vconf->outerRectangle.brush.IsOk() ) painter->setBrush( vconf->outerRectangle.brush );
  else painter->setTransparentBrush();

	MyRect coord( xcenter - xr, ycenter - yr, 2 * xr, 2 * yr );
  painter->drawRectangle( coord );

	if ( vconf->outerRectangle.doubleOuterLine )
	{
		// distance of second frame: derive from rmax -> prevent scaling errors in pdf output
		const double delta = rmax / 100.0;
		painter->drawRectangle( MyRect( coord.x - delta, coord.y - delta, coord.width + 2 * delta, coord.height + 2 * delta ));
	}
	painter->setPen( defaultPen );
}


