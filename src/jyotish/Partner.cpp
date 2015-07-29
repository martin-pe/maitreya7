/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/jyotish/Partner.cpp
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
#pragma implementation "Partner.h"
#endif

#include "Partner.h"

#include "astrobase.h"
#include "Calculator.h"
#include "ChartProperties.h"
#include "Conf.h"
#include "Horoscope.h"
#include "Planet.h"
#include "Lang.h"
#include "mathbase.h"
#include "Nakshatra.h"
#include "Varga.h"
#include "Writer.h"

extern Config *config;

IMPLEMENT_CLASS( CompositHoroscope, Horoscope )

/*****************************************************
**
**   VedicPartnerChart   ---   Constructor
**
******************************************************/
VedicPartnerChart::VedicPartnerChart()
{
	h1 = 0;
	h2 = 0;
	pvarna = pvashya = ptara = pyoni = pmaitri = pkoota = pgana = pnadi = ptotal = 0;
}

/*****************************************************
**
**   VedicPartnerChart   ---   Constructor
**
******************************************************/
VedicPartnerChart::VedicPartnerChart( Horoscope *h1, Horoscope *h2 )
{
	this->h1 = h1;
	this->h2 = h2;
	pvarna = pvashya = ptara = pyoni = pmaitri = pkoota = pgana = pnadi = ptotal = 0;
}

/*****************************************************
**
**   VedicPartnerChart   ---   update
**
******************************************************/
void VedicPartnerChart::update()
{
	double len1, len2;
	int rasi1, nak1, rasi2, nak2, vashya1, vashya2;
	pvarna = pvashya = ptara = pyoni = pmaitri = pkoota = pgana = pnadi = ptotal = 0;

	NakshatraExpert expert;

	len1 = h1->getVedicLength( OMOON );
	len2 = h2->getVedicLength( OMOON );
	rasi1 = getRasi( len1 );
	nak1 = getNakshatra( len1, N27 );
	rasi2 = getRasi( len2 );
	nak2 = getNakshatra( len2, N27 );

	/*
	 * Varna
	 */
	if ( getVarna( len1 ) >= getVarna( len2 ) )
	{
		pvarna = 1;
	}

	/*
	 * Vashya
	 */
	vashya1 = getVashya( h1->getVedicLength( OMOON ));
	vashya2 = getVashya( h2->getVedicLength( OMOON ));
	assert( vashya1 >= 0 && vashya1 < 5 );
	assert( vashya2 >= 0 && vashya2 < 5 );

	const double vashya_map[5][5] = {
		{ 2, 1, .5, 0, 1 },
		{ 0, 2, 1, 0, 1 },
		{ 0, 1, 2, 0, 1 },
		{ .5, .5, 1, 2, 0 },
		{0, 1, 1, 0, 2 }
	};
	pvashya = vashya_map[vashya1][vashya2];

	/*
	 * Tara
	 */
	int dnak1 = nak2 - nak1;
	if ( dnak1 < 0 ) dnak1 += 27;
	dnak1++;
	int rest1 = dnak1 % 9;
	if (( rest1 == 3 ) || ( rest1 == 5 ) || ( rest1 == 7 ))
	{
	}
	else ptara += 1.5;

	int dnak2 = nak1 - nak2;
	if ( dnak2 < 0 ) dnak2 += 27;
	dnak2++;
	int rest2 = dnak2 % 9;
	if (( rest2 == 3 ) || ( rest2 == 5 ) || ( rest2 == 7 ))
	{
	}
	else ptara += 1.5;

	/*
	 * Yoni
	 */
	int yoni1 = expert.getYoni( len1 );
	int yoni2 = expert.getYoni( len2 );

	const int yoni_map[14][14] = {
		{ 4, 0, 1, 2, 3, 2, 2, 1, 3, 2, 3, 3, 1, 3 },
		{ 0, 4, 1, 2, 3, 2, 2, 2, 3, 2, 2, 2, 1, 3 },
		{ 1, 1, 4, 0, 1, 2, 2, 2, 1, 1, 2, 1, 2, 1 },
		{ 2, 3, 0, 4, 2, 2, 2, 2, 2, 2, 3, 2, 1, 2 },
		{ 3, 3, 1, 2, 4, 0, 3, 1, 3, 2, 3, 2, 1, 3 },
		{ 2, 2, 2, 2, 0, 4, 2, 1, 2, 2, 2, 2, 1, 2 },
		{ 2, 2, 2, 2, 3, 2, 4, 0, 2, 1, 2, 2, 2, 2 },
		{ 1, 2, 2, 2, 1, 1, 0, 4, 1, 1, 1, 1, 2, 1 },
		{ 3, 2, 1, 2, 3, 2, 2, 1, 4, 0, 2, 2, 1, 2 },
		{ 2, 2, 1, 2, 2, 2, 1, 1, 0, 4, 1, 1, 1, 1 },
		{ 3, 2, 2, 3, 3, 2, 2, 1, 2, 1, 4, 0, 2, 2 },
		{ 3, 2, 1, 2, 2, 2, 2, 1, 2, 1, 0, 4, 2, 2 },
		{ 1, 1, 2, 1, 1, 1, 2, 2, 1, 1, 2, 2, 4, 0 },
		{ 3, 3, 1, 2, 3, 2, 2, 1, 2, 1, 2, 2, 0, 4 }
	};
	pyoni = yoni_map[yoni1][yoni2];

	/*
	 * Graha Maitri
	 */
	int mitra1 = isFriend( getLord( rasi1 ), getLord( rasi2 ) );
	int mitra2 = isFriend( getLord( rasi2 ), getLord( rasi1 ) );
	//printf( "Graha maitri rasi1 %d lord1 %d rasi2 %d lord2 %d mitra1 %d mitra2 %d\n", rasi1, getLord(rasi1),rasi2,getLord(rasi2),mitra1,mitra2 );

	if ( mitra1 == FRIEND && mitra2 == FRIEND ) pmaitri = 5;
	else if (( mitra1 == FRIEND && mitra2 == NEUTRAL ) || ( mitra2 == FRIEND && mitra1 == NEUTRAL )) pmaitri = 4;
	else if (( mitra1 == NEUTRAL && mitra2 == NEUTRAL )
	         || ( mitra1 == FRIEND && mitra2 == ENEMY ) || ( mitra1 == ENEMY && mitra2 == FRIEND )) pmaitri = 2;
	else if (( mitra1 == ENEMY && mitra2 == NEUTRAL ) || ( mitra2 == ENEMY && mitra1 == NEUTRAL )) pmaitri = 1;
	else if ( mitra1 == ENEMY && mitra2 == ENEMY ) pmaitri = 0;
	else assert( 0 );

	/*
	 * Gana
	 */
	const int gana_map[3][3] = {
		{ 6, 5, 1 },
		{ 6, 6, 0 },
		{ 0, 0, 6 }
	};

	pgana = gana_map[expert.getGana( len1 )][expert.getGana( len2 )];

	/*
	 * Rasi Koota (or Bhakoota)
	 */
	int rasidiff = red12( rasi1 - rasi2 );
	if (( rasidiff == HOUSE1 )
	        || ( rasidiff == HOUSE3 )
	        || ( rasidiff == HOUSE4 )
	        || ( rasidiff == HOUSE7 )
	        || ( rasidiff == HOUSE10 )
	        || ( rasidiff == HOUSE11 ))
		pkoota = 7;

	/*
	 * Nadi
	 */
	if ( expert.getNadi( len1 ) != expert.getNadi( len2 ) ) pnadi = 8;

	/*
	 * Final
	 */
	ptotal = pvarna + pvashya + ptara + pyoni + pmaitri + pkoota + pgana + pnadi;
}

/*****************************************************
**
**   VedicPartnerChart   ---   write
**
******************************************************/
void VedicPartnerChart::write( Writer *writer )
{
	wxString s;
	Lang lang;
	NakshatraExpert nexpert;

	int perc = (int)(100 * (double)ptotal / 36);

	//writer->writeHeader1( _( "Vedic Partner Chart" ));
	s.Printf( wxT( "%s: %s" ), _( "Groom" ), (const wxChar*)h1->getHName());
	writer->writeLine( s );
	s.Printf( wxT( "%s: %s" ), _( "Bride" ), (const wxChar*)h2->getHName());
	writer->writeParagraph( s );
	s.Clear();

	Table table( 2, 10 );
	table.setHeader( 0, _( "Quality" ));
	table.setHeader( 1, _( "Points" ));

	table.setEntry( 0, 1, _( "Varna" ));
	s << pvarna << wxT( " / 1" );
	table.setEntry( 1, 1, s );
	s.Clear();

	table.setEntry( 0, 2, _( "Vashya" ));
	s << pvashya << wxT( " / 2" );
	table.setEntry( 1, 2, s );
	s.Clear();

	table.setEntry( 0, 3, _( "Tara" ));
	s << ptara << wxT( " / 3" );
	table.setEntry( 1, 3, s );
	s.Clear();

	table.setEntry( 0, 4, _( "Yoni" ));
	s << pyoni << wxT( " / 4" );
	table.setEntry( 1, 4, s );
	s.Clear();

	table.setEntry( 0, 5, _( "Graha Maitri" ));
	s << pmaitri << wxT( " / 5" );
	table.setEntry( 1, 5, s );
	s.Clear();

	table.setEntry( 0, 6, _( "Gana" ));
	s << pgana << wxT( " / 6" );
	table.setEntry( 1, 6, s );
	s.Clear();

	table.setEntry( 0, 7, _( "Bhakoota" ));
	s << pkoota << wxT( " / 7" );
	table.setEntry( 1, 7, s );
	s.Clear();

	table.setEntry( 0, 8, _( "Nadi" ));
	s << pnadi << wxT( " / 8" );
	table.setEntry( 1, 8, s );
	s.Clear();

	table.setHeaderEntry( 0, 9, _( "Total" ));
	s << ptotal << wxT( " / 36 (" ) << perc << wxT( "%)" );
	//s << ptotal << wxT( " / 36" );
	table.setHeaderEntry( 1, 9, s );
	s.Clear();

	writer->writeTable( table );

	// Details about Moon #1
	writer->writeHeader1( _( "Groom" ));

	double mlen1 = h1->getObjectPosition( OMOON, true ).length;
	Table t1( 2, 6 );
	t1.setHeader( 0, _( "Quality" ));
	t1.setHeader( 1, _( "Value" ));

	t1.setEntry( 0, 1, _( "Nakshatra" ));
	t1.setEntry( 1, 1, lang.getNakshatraName(getNakshatra( mlen1, N27), N27, TLARGE ));


	t1.setEntry( 0, 2, _( "Varna" ));
	t1.setEntry( 1, 2, lang.getVarnaName( getVarna( mlen1 )));

	t1.setEntry( 0, 3, _( "Yoni" ));
	t1.setEntry( 1, 3, lang.getYoniName( nexpert.getYoni( mlen1 )));

	t1.setEntry( 0, 4, _( "Gana" ));
	t1.setEntry( 1, 4, lang.getGanaName( nexpert.getGana( mlen1 )));

	t1.setEntry( 0, 5, _( "Nadi" ));
	t1.setEntry( 1, 5, lang.getNadiName( nexpert.getNadi( mlen1 )));
	writer->writeTable( t1 );

	// Details about Moon #2
	writer->writeHeader1( _( "Bride" ));

	double mlen2 = h2->getObjectPosition( OMOON, true ).length;
	Table t2( 2, 6 );
	t2.setHeader( 0, _( "Quality" ));
	t2.setHeader( 1, _( "Value" ));

	t2.setEntry( 0, 1, _( "Nakshatra" ));
	t2.setEntry( 1, 1, lang.getNakshatraName(getNakshatra( mlen2, N27), N27, TLARGE ));

	t2.setEntry( 0, 2, _( "Varna" ));
	t2.setEntry( 1, 2, lang.getVarnaName( getVarna( mlen2 )));

	t2.setEntry( 0, 3, _( "Yoni" ));
	t2.setEntry( 1, 3, lang.getYoniName( nexpert.getYoni( mlen2 )));

	t2.setEntry( 0, 4, _( "Gana" ));
	t2.setEntry( 1, 4, lang.getGanaName( nexpert.getGana( mlen2 )));

	t2.setEntry( 0, 5, _( "Nadi" ));
	t2.setEntry( 1, 5, lang.getNadiName( nexpert.getNadi( mlen2 )));
	writer->writeTable( t2 );

}

/*****************************************************
**
**   CompositHoroscope   ---   Constructor
**
******************************************************/
CompositHoroscope::CompositHoroscope()
		: Horoscope()
{
}

/*****************************************************
**
**   CompositHoroscope   ---   update
**
******************************************************/
void CompositHoroscope::update( Horoscope *h1, Horoscope *h2 )
{
	int i, p;
	double eps, r[13], houselen;
	Calculator *calculator = CalculatorFactory().getCalculator();
	VargaExpert expert( this );

	if ( ! h1 || ! h2 ) return;

	iayanamsa = .5 * ( h1->getAyanamsa( true ) + h1->getAyanamsa( true ));
	wayanamsa = .5 * ( h1->getAyanamsa( false ) + h1->getAyanamsa( false ));
	siderealTime = .5 * ( h1->getSiderealTime() + h2->getSiderealTime());
	for ( i = OSUN; i < MAX_EPHEM_OBJECTS; i++ ) object_len[i] = calcGeometricalMidpoint( h1->object_len[ i ], h2->object_len[ i ] );
	//for( i = 0; i < NUM_LAGNA; i++ ) lagna_len[i] = calcGeometricalMidpoint( h1->lagna_len[ i ], h2->lagna_len[ i ] );
	//for( i = 0; i < NUM_UPA; i++ ) upagraha_len[i] = calcGeometricalMidpoint( h1->upagraha_len[ i ], h2->upagraha_len[ i ] );

	double arm1 = h1->getSiderealTime() * 15;
	double arm2 = h2->getSiderealTime() * 15;
	double arm = calcGeometricalMidpoint( arm1, arm2 );
	calculator->calcEps( eps, .5 * ( h1->getJD() + h2->getJD() ));
	calculator->calcAscendantByArmc( arm, eps, .5 * ( h1->getLatitude() + h2->getLatitude()), r, config->wHouseSystem );

	updateObjectPositions();

	for ( i = 0; i < 12; i++ ) whousecusp[i] = red_deg( r[i] - wayanamsa );
	if ( config->vHouseSystem != config->wHouseSystem )
		calculator->calcAscendantByArmc( arm, eps, .5 * ( h1->getLatitude() + h2->getLatitude()), r, config->vHouseSystem );
	for ( i = 0; i < 12; i++ ) ihousecusp[i] = red_deg( r[i] - iayanamsa );
	for ( i = 0; i < 12; i++ )
	{
		houselen = red_deg( ihousecusp[i<11 ? i+1 : 0] - ihousecusp[i] );
		ihousesandhi[i] = red_deg( ihousecusp[i] + .5 * houselen );
	}

	updatePositionArrays();
	for ( p = 0; p < MAX_EPHEM_OBJECTS; p++ ) ephem_planets[p]->update( getObjectPosition( p, true ).length );
	for ( p = 0; p < MAX_EXTENDED_OBJECTS; p++ ) extended_planets[p]->update( getObjectPosition( p + FIRST_EXTENDED_OBJECT, true ).length );

	// Attention: must be separated from updateVargaData because Vargas reference each other
	for ( p = 0; p <= OSATURN; p++ ) ephem_planets[p]->calculateDignities();
}

/*****************************************************
**
**   CompositHoroscope   ---   dump
**
******************************************************/
void CompositHoroscope::dump( Writer *writer, const ChartProperties* chartprops, bool show_header )
{
	vector<int> obs;
	ObjectPosition pos;
	int line = 0;

	if ( show_header ) writer->writeHeader1( _( "Composite Chart" ));

	Table table( 2, chartprops->getPlanetList().size() + 1 );
	table.setHeader( 0,  _( "Planet" ));
	table.setHeader( 1,  _( "Longitude" ));

	for ( unsigned i = 0; i < chartprops->getPlanetList().size(); i++ )
	{
		line++;
		pos = getObjectPosition( chartprops->getPlanetList()[i], chartprops->isVedic() );
		table.setEntry( 0, line, writer->getObjectName( chartprops->getPlanetList()[i], TLARGE ));
		table.setEntry( 1, line, writer->getPosFormatted( pos.length, pos.retro ));
	}
	writer->writeTable( table );
}


