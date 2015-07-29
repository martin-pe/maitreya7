/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/SarvatobhadraChart.cpp
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
#pragma implementation "SarvatobhadraChart.h"
#endif

#include "SarvatobhadraChart.h"

#include "astrobase.h"
#include "ChartProperties.h"
#include "Conf.h"
#include "constants.h"
#include "ColorConfig.h"
#include "FontConfig.h"
#include "Horoscope.h"
#include "Lang.h"
#include "mathbase.h"
#include "Nakshatra.h"
#include "Painter.h"
#include "VedicChartConfig.h"
#include "Writer.h"

#include <vector>
#include <wx/brush.h>
#include <wx/font.h>

using namespace std;

extern Config *config;

#define RTOL_DOUBLE_FRAME .0125

// Upper line
const wxChar * sanskrit_ee[2] = { wxT( "ee" ), wxT( "\u0908" ) };
const wxChar * sanskrit_a[2] = { wxT( "a" ), wxT( "\u0905" ) };

// line 2
const wxChar * sanskrit_ri[2] = { wxT( "ri" ), wxT( "\u090B" ) };
const wxChar * sanskrit_g[2] = { wxT( "g" ), wxT( "\u0917" ) };
const wxChar * sanskrit_s[2] = { wxT( "s" ), wxT( "\u0938" ) };
const wxChar * sanskrit_d[2] = { wxT( "d" ), wxT( "\u0926" ) };
const wxChar * sanskrit_ch[2] = { wxT( "ch" ), wxT( "\u091A" ) };
const wxChar * sanskrit_l[2] = { wxT( "l" ), wxT( "\u0932" ) };
const wxChar * sanskrit_u[2] = { wxT( "u" ), wxT( "\u0909" ) };

// line 3
const wxChar * sanskrit_kh[2] = { wxT( "kh" ), wxT( "\u0916" ) };
const wxChar * sanskrit_ai[2] = { wxT( "ai" ), wxT( "\u0910" ) };
const wxChar * sanskrit_li[2] = { wxT( "li" ), wxT( "\u090C" ) };

// line 4
const wxChar * sanskrit_j[2] = { wxT( "j" ), wxT( "\u091C" ) };
const wxChar * sanskrit_ah[2] = { wxT( "ah" ), wxT( "\u0905\u0903" ) };
const wxChar * sanskrit_o[2] = { wxT( "o" ), wxT( "\u0913" ) };
const wxChar * sanskrit_v[2] = { wxT( "v" ), wxT( "\u0935" ) };

// line 5
const wxChar * sanskrit_bh[2] = { wxT( "bh" ), wxT( "\u092D" ) };
const wxChar * sanskrit_k[2] = { wxT( "k" ), wxT( "\u0915" ) };

// line 6
const wxChar * sanskrit_y[2] = { wxT( "y" ), wxT( "\u092F" ) };
const wxChar * sanskrit_am[2] = { wxT( "am" ), wxT( "\u0905\u0902" ) };
const wxChar * sanskrit_au[2] = { wxT( "au" ), wxT( "\u0914" ) };
const wxChar * sanskrit_h[2] = { wxT( "h" ), wxT( "\u0939" ) };

// line 7
const wxChar * sanskrit_n[2] = { wxT( "n" ), wxT( "\u0928" ) };
const wxChar * sanskrit_e[2] = { wxT( "e" ), wxT( "\u090F" ) };
const wxChar * sanskrit_dd[2] = { wxT( "d" ), wxT( "\u0921" ) };

// line 8
const wxChar * sanskrit_t[2] = { wxT( "t" ), wxT( "\u0924" ) };
const wxChar * sanskrit_r[2] = { wxT( "r" ), wxT( "\u0930" ) };
const wxChar * sanskrit_p[2] = { wxT( "p" ), wxT( "\u092A" ) };
const wxChar * sanskrit_th[2] = { wxT( "th" ), wxT( "\u091F" ) };
const wxChar * sanskrit_m[2] = { wxT( "m" ), wxT( "\u092E" ) };
const wxChar * sanskrit_uu[2] = { wxT( "uu" ), wxT( "\u090A" ) };

// Bottom line
const wxChar * sanskrit_i[2] = { wxT( "i" ), wxT( "\u0907" ) };
const wxChar * sanskrit_aa[2] = { wxT( "aa" ), wxT( "\u0906" ) };

/*****************************************************
**
**   SarvatobhadraChart   ---   Constructor
**
******************************************************/
SarvatobhadraChart::SarvatobhadraChart( const Horoscope* h1,  const Horoscope* h2, const ChartProperties *chartprops )
		: BasicVedicChart( chartprops, 28, 2 )
{
	nh1 = new NakshatraHoroscope( h1, N28 );
	nh2 = new NakshatraHoroscope( h2, N28 );
	init();
}

/*****************************************************
**
**   SarvatobhadraChart   ---   Constructor
**
******************************************************/
SarvatobhadraChart::SarvatobhadraChart( const Horoscope* h, const ChartProperties *chartprops )
		: BasicVedicChart( chartprops, 28, 1 ),
		nh2( 0 )
{
	nh1 = new NakshatraHoroscope( h, N28 );
	init();
}

/*****************************************************
**
**   SarvatobhadraChart   ---   Constructor
**
******************************************************/
SarvatobhadraChart::SarvatobhadraChart( const NakshatraHoroscope* h, const ChartProperties *chartprops )
		: BasicVedicChart( chartprops, 28, 1 ),
		nh1( h ),
		nh2( 0 )
{
	init();
}

/*****************************************************
**
**   SarvatobhadraChart   ---   Constructor
**
******************************************************/
SarvatobhadraChart::SarvatobhadraChart( const NakshatraHoroscope* h1, const NakshatraHoroscope *h2, const ChartProperties *chartprops )
		: BasicVedicChart( chartprops, 28, 2 ),
		nh1( h1 ),
		nh2( h2 )
{
	init();
}

/*****************************************************
**
**   SarvatobhadraChart   ---   Destructor
**
******************************************************/
SarvatobhadraChart::~SarvatobhadraChart()
{
	if ( nh1 ) delete nh1;
	if ( nh2 ) delete nh2;
}

/*****************************************************
**
**   SarvatobhadraChart   ---   init
**
******************************************************/
void SarvatobhadraChart::init()
{
	int x, y;
	for ( int i = 0; i < 9; i++ )
		for ( int j = 0; j < 9; j++ )
			mark[i][j] = 0;

	if ( chartprops->isBlank() ) return;
	assert( nh1->getHoroscope() );
	chart_count = nh2 ? 2 : 1;

	getNakshatraIndex( nh1->getNakshatra( OMOON ), x, y ) ;
	mark[x][y] = 1;
	getRasiIndex( getRasi( nh1->getHoroscope()->getVedicLength( OASCENDANT )), x, y );
	mark[x][y] = 1;
	getWeekdayIndex( nh1->getHoroscope()->getDataSet()->getWeekDay(), x, y );
	mark[x][y] = 1;

	NakshatraExpert expert;
	expert.getSbcNakshatraProperties( getNakshatra( nh1->getHoroscope()->getVedicLength( OMOON ), N27 ), nakshatraProps );
}

/*****************************************************
**
**   SarvatobhadraChart   ---   paintChart
**
******************************************************/
void SarvatobhadraChart::paintChart()
{
	Lang lang;
	int i, j;
	double w, h;
	MyRect rect, fieldrect;
	wxString s;


	// SBC has smaller fonts
	const double sbcsclaingfactor = .6;
	textzoom *= sbcsclaingfactor;
	symbolzoom *= sbcsclaingfactor;
	symbol_height *= sbcsclaingfactor;
	text_width *= sbcsclaingfactor;
	text_height *= sbcsclaingfactor;

	xstep = xr / 4.5;
	ystep = yr / 4.5;

	const int text_border = xr / 100;

	paintOuterRectangle();

	painter->setPen( defaultPen );
	painter->setTransparentBrush();

	for ( i = 1; i <= 8; i++ )
	{
		painter->drawLine( xcenter - xr + i * xstep, ycenter - yr, xcenter - xr + i * xstep, ycenter + yr );
		painter->drawLine( xcenter - xr , ycenter - yr + i * ystep , xcenter + xr, ycenter - yr + i * ystep );
	}

  GcZodiacalSigns *signs = &vconf->signs;
	if ( signs->needsBgPaint() )
	{
		for ( i = 0; i < 28; i++ )
		{
			painter->setDefaultPen();
			painter->setBrush(  signs->getBrush( i ));
			painter->drawRectangle( getNakshatraCoord( i ));
		}
		painter->setTransparentBrush();
	}

	painter->setPen( defaultPen );
	if ( vconf->markedFields.brush.IsOk() )
	{
		painter->setBrush( vconf->markedFields.brush );
	}
	for ( i = 0; i < 9; i++ )
	{
		for ( j = 0; j < 9; j++ ) if ( mark[i][j] ) painter->drawRectangle( getFieldCoord( i, j ));
	}
	painter->setPen( defaultPen );
	painter->setTransparentBrush();

	if ( chartprops->getVedicGraphicStyle().showSbcAfflictions ) drawAspects();

	// Draw Nakshatra Names
	painter->setGraphicFont( textzoom );
	setDefaultTextColor();
	for ( i = 0; i < 28; i++ )
	{
		int quality = nakshatraProps[i].taraQuality;
		wxString qq;

		if ( chartprops->getVedicGraphicStyle().showSbcNakshatraQuality )
		{
			if ( quality == TARA_PROP_BENEFIC ) qq = wxT( " B" );
			else if ( quality == TARA_PROP_MALEFIC ) qq = wxT( " M" );
		}
		s = lang.getNakshatraName( i, N28, TMEDIUM ) + qq;
		painter->getTextExtent( s, &w, &h );

		if ( w > .9 * xstep ) s = lang.getNakshatraName( i, N28, TSHORT ) + qq;
		painter->drawTextFormatted( getNakshatraCoord( i ), s, Align::HCenter + Align::Bottom );
	}

	// Static text portions
	const int symbolindex = chartprops->getVedicGraphicStyle().showSbcSanskritSymbols ? 1 : 0;

#define DRAW_STATIC_FIELD( a, b, t ) \
	fieldrect = getFieldCoord( a, b ); \
	if ( ! refreshRect || ( refreshRect && refreshRect->Intersects( fieldrect.toWxRect() ))) \
	{ \
		painter->drawTextFormatted( fieldrect, t, Align::HCenter + Align::Bottom ); \
	}

#define DRAW_STATIC_FIELD_SANSKRIT( a, b, t ) \
	fieldrect = getFieldCoord( a, b ); \
	if ( ! refreshRect || ( refreshRect && refreshRect->Intersects( fieldrect.toWxRect() ))) \
	{ \
		painter->drawTextFormatted( fieldrect, t[symbolindex], Align::HCenter + Align::Bottom ); \
	}

	DRAW_STATIC_FIELD_SANSKRIT( 0, 0, sanskrit_ee )
	DRAW_STATIC_FIELD_SANSKRIT( 8, 0, sanskrit_a )

	// Line 2
	DRAW_STATIC_FIELD_SANSKRIT( 1, 1, sanskrit_ri )
	DRAW_STATIC_FIELD_SANSKRIT( 2, 1, sanskrit_g )
	DRAW_STATIC_FIELD_SANSKRIT( 3, 1, sanskrit_s )
	DRAW_STATIC_FIELD_SANSKRIT( 4, 1, sanskrit_d )
	DRAW_STATIC_FIELD_SANSKRIT( 5, 1, sanskrit_ch )
	DRAW_STATIC_FIELD_SANSKRIT( 6, 1, sanskrit_l )
	DRAW_STATIC_FIELD_SANSKRIT( 7, 1, sanskrit_u )

	// Line 3
	DRAW_STATIC_FIELD_SANSKRIT( 1, 2, sanskrit_kh )
	DRAW_STATIC_FIELD_SANSKRIT( 2, 2, sanskrit_ai )
	DRAW_STATIC_FIELD_SANSKRIT( 6, 2, sanskrit_li )
	DRAW_STATIC_FIELD_SANSKRIT( 7, 2, sanskrit_a )

	// Line 4
	DRAW_STATIC_FIELD_SANSKRIT( 1, 3, sanskrit_j )
	DRAW_STATIC_FIELD_SANSKRIT( 3, 3, sanskrit_ah )
	DRAW_STATIC_FIELD( 4, 3, lang.getWeekdayName( 5 ).Left(3) )
	DRAW_STATIC_FIELD_SANSKRIT( 5, 3, sanskrit_o )
	DRAW_STATIC_FIELD_SANSKRIT( 7, 3, sanskrit_v )

	// Line 5
	DRAW_STATIC_FIELD_SANSKRIT( 1, 4, sanskrit_bh )
	DRAW_STATIC_FIELD( 3, 4, lang.getWeekdayName( 4 ).Left(3) )
	DRAW_STATIC_FIELD( 4, 4, lang.getWeekdayName( 6 ).Left(3) )
	DRAW_STATIC_FIELD( 5, 4, lang.getWeekdayName( 0 ).Left(3) + wxT( " " ) + lang.getWeekdayName( 2 ).Left(3) )
	DRAW_STATIC_FIELD_SANSKRIT( 7, 4, sanskrit_k )

	// Line 6
	DRAW_STATIC_FIELD_SANSKRIT( 1, 5, sanskrit_y )
	DRAW_STATIC_FIELD_SANSKRIT( 3, 5, sanskrit_am )
	DRAW_STATIC_FIELD( 4, 5, lang.getWeekdayName( 1 ).Left(3) + wxT( " " ) + lang.getWeekdayName( 3 ).Left(3))
	DRAW_STATIC_FIELD_SANSKRIT( 5, 5, sanskrit_au )
	DRAW_STATIC_FIELD_SANSKRIT( 7, 5, sanskrit_h )

	// Line 7
	DRAW_STATIC_FIELD_SANSKRIT( 1, 6, sanskrit_n )
	DRAW_STATIC_FIELD_SANSKRIT( 2, 6, sanskrit_e )
	DRAW_STATIC_FIELD_SANSKRIT( 6, 6, sanskrit_li )
	DRAW_STATIC_FIELD_SANSKRIT( 7, 6, sanskrit_dd )

	// Line 8
	DRAW_STATIC_FIELD_SANSKRIT( 1, 7, sanskrit_kh )
	DRAW_STATIC_FIELD_SANSKRIT( 2, 7, sanskrit_t )
	DRAW_STATIC_FIELD_SANSKRIT( 3, 7, sanskrit_r )
	DRAW_STATIC_FIELD_SANSKRIT( 4, 7, sanskrit_p )
	DRAW_STATIC_FIELD_SANSKRIT( 5, 7, sanskrit_th )
	DRAW_STATIC_FIELD_SANSKRIT( 6, 7, sanskrit_m )
	DRAW_STATIC_FIELD_SANSKRIT( 7, 7, sanskrit_uu )

	// Line 9
	DRAW_STATIC_FIELD_SANSKRIT( 0, 8, sanskrit_i )
	DRAW_STATIC_FIELD_SANSKRIT( 8, 8, sanskrit_aa )
#undef DRAW_STATIC_FIELD
#undef DRAW_STATIC_FIELD_SANSKRIT

	// Signs
	if ( config->useSignSymbols ) painter->setSymbolFont( symbolzoom );
#define DRAW_SIGN_SYMBOL( x, y, index ) \
	painter->drawTextFormatted( getFieldCoord( x, y ), \
	config->useSignSymbols ? (wxString)lang.getSignSymbolCode( index ) : lang.getSignName( index, TMEDIUM ), \
		Align::HCenter + Align::Bottom );

	DRAW_SIGN_SYMBOL( 5, 2, ARIES )
	DRAW_SIGN_SYMBOL( 6, 3, TAURUS )
	DRAW_SIGN_SYMBOL( 6, 4, GEMINI )
	DRAW_SIGN_SYMBOL( 6, 5, CANCER )
	DRAW_SIGN_SYMBOL( 5, 6, LEO )
	DRAW_SIGN_SYMBOL( 4, 6, VIRGO )
	DRAW_SIGN_SYMBOL( 3, 6, LIBRA )
	DRAW_SIGN_SYMBOL( 2, 5, SCORPIO )
	DRAW_SIGN_SYMBOL( 2, 4, SAGITTARIUS )
	DRAW_SIGN_SYMBOL( 2, 3, CAPRICORN )
	DRAW_SIGN_SYMBOL( 3, 2, AQUARIUS )
	DRAW_SIGN_SYMBOL( 4, 2, PISCES )
#undef DRAW_SIGN_SYMBOL
	painter->setGraphicFont( textzoom );

	// Draw planets
	setDefaultTextColor();

	for ( i = 0; i < 28; i++ )
	{
		rect = getNakshatraCoord( i );

		// radix planet in upper area
		rect.height /= 3.0;
		if ( ! refreshRect || (refreshRect && refreshRect->Intersects( rect.toWxRect() )))
		{
			//printf( "SBC drawFieldText rect.x %f, rect.y %f rect.height %f rect.width %f\n", rect.x, rect.y, rect.height, rect.width );
			drawFieldText( rect, i, Align::Center, 0 );
		}
		if ( chart_count > 1 )
		{
			// transit planet vertically centered
			rect.y += rect.height;
			if ( ! refreshRect || ( refreshRect && refreshRect->Intersects( rect.toWxRect() )))
			{
				drawFieldText(  rect, i, Align::Center, 1, text_border );
			}
		}
	}
}

/*****************************************************
**
**   SarvatobhadraChart   ---   getPlanetField
**
******************************************************/
int SarvatobhadraChart::getPlanetField( const int &planet, const int &chart_id )
{
	const NakshatraHoroscope *nh = ( chart_id == 0 ? nh1 : nh2 );
	if ( ! nh )
	{
		printf( "WARN: horoscope in SarvatobhadraChart::getPlanetIndex is NULL, returning Aries\n" );
		return false;
	}

	return nh->getNakshatra( planet );
}

/*****************************************************
**
**   SarvatobhadraChart   ---   getPlanetRetro
**
******************************************************/
bool SarvatobhadraChart::getPlanetRetro( const int &planet, const int &chart_id )
{
	const NakshatraHoroscope *nh = ( chart_id == 0 ? nh1 : nh2 );
	if ( ! nh )
	{
		printf( "WARN: horoscope in SarvatobhadraChart::getPlanetRetro is NULL, returning false\n" );
		return false;
	}
	return nh->isRetrograde( planet );
}

/*****************************************************
**
**   SarvatobhadraChart   ---   setCharts
**
******************************************************/
void SarvatobhadraChart::setCharts( const NakshatraHoroscope* h1, const NakshatraHoroscope* h2 )
{
	if ( nh1 ) delete nh1;
	nh1 = h1;
	if ( nh2 ) delete nh2;
	nh2 = h2;
	init();
}

/*****************************************************
**
**   SarvatobhadraChart   ---   handleMouseLeftEvent
**
******************************************************/
bool SarvatobhadraChart::handleMouseLeftEvent( int x, int y, wxRect &rect )
{
	if ( getFieldIndex( &x, &y ))
	{
		mark[x][y] = ! mark[x][y];
		rect = getFieldCoord( x, y ).toWxRect();
		return true;
	}
	return false;
}

/*****************************************************
**
**   SarvatobhadraChart   ---   markField
**
******************************************************/
void SarvatobhadraChart::markField( int x, int y )
{
	if ( getFieldIndex( &x, &y ))
	{
		mark[x][y] = ! mark[x][y];
	}
}

/*****************************************************
**
**   SarvatobhadraChart   ---   getFieldIndex
**
******************************************************/
bool SarvatobhadraChart::getFieldIndex( int *x, int *y )
{
	if ( *x < xcenter - xr ) return false;
	if ( *x > xcenter + xr ) return false;
	if ( *y < ycenter -yr ) return false;
	if ( *y > ycenter + yr ) return false;

	*x = (int)(( *x - xcenter + xr ) / xstep);
	*y = (int)(( *y - ycenter + yr ) / ystep);
	return true;
}

/*****************************************************
**
**   SarvatobhadraChart   ---   getFieldCoord
**
******************************************************/
MyRect SarvatobhadraChart::getFieldCoord( const int &x, const int &y )
{
	return MyRect( xcenter - xr + x * xstep, ycenter - yr + y * ystep, xstep, ystep );
}

/*****************************************************
**
**   SarvatobhadraChart   ---   getWeekdayIndex
**
******************************************************/
void SarvatobhadraChart::getWeekdayIndex( const int &w, int &x, int &y )
{
	assert( w >= 0 && w < 7 );
	switch ( w )
	{
	case 0:
	case 2:
		x = 5;
		y = 4;
		break;
	case 1:
	case 3:
		x = 4;
		y = 5;
		break;
	case 4:
		x = 3;
		y = 4;
		break;
	case 5:
		x = 4;
		y = 3;
		break;
	default:
		x = 4;
		y = 4;
		break;
	}
}

/*****************************************************
**
**   SarvatobhadraChart   ---   getWeekdayCoord
**
******************************************************/
MyRect SarvatobhadraChart::getWeekdayCoord( const int &w )
{
	int x, y;
	getWeekdayIndex( w, x, y );
	return getFieldCoord( x, y );
}

/*****************************************************
**
**   SarvatobhadraChart   ---   getRasiIndex
**
******************************************************/
void SarvatobhadraChart::getRasiIndex( const int &r, int &x, int &y )
{
	assert( r >= 0 && r < 12 );
	switch ( r )
	{
	case ARIES:
		x = 5;
		y = 2;
		break;
	case TAURUS:
		x = 6;
		y = 3;
		break;
	case GEMINI:
		x = 6;
		y = 4;
		break;
	case CANCER:
		x = 6;
		y = 5;
		break;
	case LEO:
		x = 5;
		y = 6;
		break;
	case VIRGO:
		x = 4;
		y = 6;
		break;
	case LIBRA:
		x = 3;
		y = 6;
		break;
	case SCORPIO:
		x = 2;
		y = 5;
		break;
	case SAGITTARIUS:
		x = 2;
		y = 4;
		break;
	case CAPRICORN:
		x = 2;
		y = 3;
		break;
	case AQUARIUS:
		x = 3;
		y = 2;
		break;
	case PISCES:
	default:
		x = 4;
		y = 2;
		break;
	}
}

/*****************************************************
**
**   SarvatobhadraChart   ---   getRasiCoord
**
******************************************************/
MyRect SarvatobhadraChart::getRasiCoord( const int &r )
{
	int x, y;
	getRasiIndex( r, x, y );
	return getFieldCoord( x, y );
}

/*****************************************************
**
**   SarvatobhadraChart   ---   getNakshatraIndex
**
******************************************************/
void SarvatobhadraChart::getNakshatraIndex( const int &i, int &x, int &y )
{
	assert( i >= 0 && i <= 27 );
	if ( i < 2 )
	{
		x = i + 6;
		y = 0;
	}
	else if ( i >= 2 && i <= 8 )
	{
		x = 8;
		y = i-1;
	}
	else if ( i >= 9 && i <= 15 )
	{
		x = 16 - i;
		y = 8;
	}
	else if ( i >= 16 && i <= 22 )
	{
		x = 0;
		y =  8 - ( i - 15 );
	}
	else
	{
		x = i - 22;
		y = 0;
	}
}

/*****************************************************
**
**   SarvatobhadraChart   ---   getNakshatraCoord
**
******************************************************/
MyRect SarvatobhadraChart::getNakshatraCoord( const int &i )
{
	int x, y;
	assert( i >= 0 && i <= 27 );
	getNakshatraIndex( i, x, y );
	return MyRect( xcenter - xr + ( x * xstep ), ycenter - yr + ( y * ystep ), xstep, ystep );
}

/*****************************************************
**
**   SarvatobhadraChart   ---   drawAspects
**
******************************************************/
void SarvatobhadraChart::drawAspects()
{
	int i, j, xr1, yr1, xr2, yr2, sw, p;
	unsigned int k;
	MyRect r1, r2;
	const Horoscope *h1 = nh1->getHoroscope();

	int nb_lines[28];
	for ( i = 0; i < 28; i++ ) nb_lines[i] = 0;

	painter->setDefaultPen();

	vector<ChartContents> &c = getChartContents( chart_count > 1 ? 1 : 0 );
	for ( i = 0; i < 28; i++ )
	{
		for ( k = 0; k < c[i].planets.size(); k++ )
		{
			r1 = getNakshatraCoord( i );
			xr1 = r1.x + r1.width/2;
			yr1 = r1.y + r1.height/2;
			p = c[i].planets[k];

			painter->setPenColor( ( p < 10 && h1->isBenefic( p ) ? config->colors->sbcBeneficBgColor : config->colors->sbcMaleficBgColor ));

			for ( j = 1; j <= 3; j++ )
			{
				r2 = getNakshatraCoord( nak_aspects[i][j] );
				xr2 = r2.x + r2.width/2;
				yr2 = r2.y + r2.height/2;

				sw = 2 * ( nb_lines[i] + nb_lines[nak_aspects[i][j]] );
				painter->drawLine( xr1 + sw, yr1 + sw, xr2 + sw, yr2 + sw );
				nb_lines[i]++;
				nb_lines[nak_aspects[i][j]]++;
			}
		}
	}
	painter->setPen( defaultPen );
}



