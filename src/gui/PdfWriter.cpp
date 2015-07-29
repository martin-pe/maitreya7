/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/PdfWriter.cpp
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
#pragma implementation "PdfWriter.h"
#endif

#include "PdfWriter.h"

#include "ColorConfig.h"
#include "Conf.h"
#include "config.h"
#include "Lang.h"
#include "PdfBase.h"

#include <wx/utils.h>

extern Config *config;

/*****************************************************
**
**   PdfWriter   ---   Constructor
**
******************************************************/
PdfWriter::PdfWriter( BasePdfDocument *pdfdoc, Config *c )
		: Writer( c ),
	symbolzoomfactor( 20 )
{
	pdf = pdfdoc;
	type = WRITER_PDF;
}

/*****************************************************
**
**   PdfWriter   ---   beginWriting
**
******************************************************/
void PdfWriter::beginWriting()
{
	wxString s;
	pdf->AliasNbPages();

	pdf->SetAuthor( wxGetUserName() );
	s << GUI_APP_NAME << wxT( " " ) << wxConvertMB2WX( VERSION );
	pdf->SetCreator( s );
}

/*****************************************************
**
**   PdfWriter   ---   endWriting
**
******************************************************/
void PdfWriter::endWriting()
{
	pdf->SaveAsFile( filename );
}

/*****************************************************
**
**   PdfWriter   ---   getSignName
**
******************************************************/
const wxString PdfWriter::getSignName( const int &i, const int format )

{
	if ( ! cfg->useSignSymbols ) return Writer::getSignName( i, format );
	return lang->getSignSymbolCode( i );
}

/*****************************************************
**
**   PdfWriter   ---   getRetroSymbol
**
******************************************************/
const wxString PdfWriter::getRetroSymbol(  const int type )
{
	if ( ! config->useSignSymbols ) return Writer::getRetroSymbol();
	return( type ? wxT( "^" ) : wxT( "_" ) );
}

/*****************************************************
**
**   PdfWriter   ---   getObjectName
**
******************************************************/
const wxString PdfWriter::getObjectName( const int &num, const int &format, const bool vedic )
{
	if ( num >= OFORTUNE || ! cfg->usePlanetSymbols || ( num >= ODHUMA && num <= LAST_EXTENDED_OBJECT ))
		return Writer::getObjectName( num, format, vedic );
	else
		return lang->getPlanetSymbolCode( num );
}

/*****************************************************
**
**   PdfWriter   ---   writeHeader1
**
******************************************************/
void PdfWriter::writeHeader1( const wxString &s1 )
{
	pdf->setHeader1Font();
	pdf->MultiCell( 0, 7, s1, wxPDF_BORDER_NONE, wxPDF_ALIGN_CENTER );
	pdf->setDefaultFont();
}

/*****************************************************
**
**   PdfWriter   ---   writeHeader2
**
******************************************************/
void PdfWriter::writeHeader2( const wxString &s1 )
{
	pdf->setHeader2Font();
	pdf->MultiCell( 0, 5, s1, wxPDF_BORDER_NONE, wxPDF_ALIGN_CENTER);
	pdf->setDefaultFont();
}

/*****************************************************
**
**   PdfWriter   ---   writeHeader3
**
******************************************************/
void PdfWriter::writeHeader3( const wxString &s1 )
{
	pdf->setHeader3Font();
	pdf->MultiCell( 0, 4, s1, wxPDF_BORDER_NONE, wxPDF_ALIGN_LEFT);
	pdf->setDefaultFont();
}

/*****************************************************
**
**   PdfWriter   ---   writeParagraph
**
******************************************************/
void PdfWriter::writeParagraph( const wxString &s1 )
{
	pdf->MultiCell(0,4,s1);
}

/*****************************************************
**
**   PdfWriter   ---   writeLine
**
******************************************************/
void PdfWriter::writeLine( const wxString &s1 )
{
	pdf->MultiCell(0,4,s1);
}

/*****************************************************
**
**   PdfWriter   ---   getAvailablePageHeight
**
******************************************************/
double PdfWriter::getAvailablePageHeight()
{
	double a = pdf->GetPageHeight() - pdf->GetTopMargin() - pdf->GetBreakMargin() - pdf->GetY();
	printf ( "PdfWriter::getAvailablePageHeight Y %f %f\n", pdf->GetY(), a );
	return a;
	//return pdf->GetPageHeight() - pdf->GetTopMargin() - pdf->GetBreakMargin() - pdf->GetY();
}

/*****************************************************
**
**   PdfWriter   ---   writeTable
**
******************************************************/
void PdfWriter::writeTable( Table &t, const bool repeatHeader )
{
	int c, c1, r;
	TableEntry *e;
	double total_height = 0;
	double next_row_height = 0;
	double current_height;

	const double cellx_tol = 2.2;
	const double rowscalingfactor = .5;
	const double min_width = .1;

	// prepare background color for table header elements
	pdf->SetFillColour( config->colors->tableHeaderBgColor );

	// mark skipped cells
	for ( r = 0; r < (int)t.nb_rows; r++ )
	{
		for ( c = 0; c < (int)t.nb_cols; c++ )
		{
			e = &t.contents[r].value[c];
			for ( c1 = 0; c1 < e->rightJoinLines && c + c1 < (int)t.nb_cols; c1++ )
			{
				t.contents[r].value[c+c1+1].isSkipped = true;
			}
		}
	}

	// calculate individual cell sizes, calculate col width according to cells without join
	for ( c = 0; c < (int)t.nb_cols; c++ )
	{
		for ( r = 0; r < (int)t.nb_rows; r++ )
		{
			e = &t.contents[r].value[c];

			//if ( e->isSkipped | e->value.IsEmpty()) continue;
			if ( e->isSkipped ) continue;

			if ( e->isHeader ) pdf->setDefaultBoldFont();
			else if ( e->isSymbol ) pdf->setSymbolFont( symbolzoomfactor );
			else pdf->setDefaultFont();

			e->width = pdf->GetStringWidth( e->value ) + cellx_tol;
			if ( e->rightJoinLines == 0 )
			{
				t.col_width[c] = Max( e->width, t.col_width[c] );
			}
			t.row_height[r] = Max( rowscalingfactor * pdf->GetFontSize(), t.row_height[r] );
		}
	}

	for ( r = 0; r < (int)t.nb_rows; r++ )
	{
		total_height += t.row_height[r];
	}

	double joinedcolwidth = 0;

	// correct cell width due to skipped entries
	for ( r = 0; r < (int)t.nb_rows; r++ )
	{
		for ( c = 0; c < (int)t.nb_cols; c++ )
		{
			e = &t.contents[r].value[c];

			if ( e->isSkipped ) continue;

			if ( e->rightJoinLines == 0 ) continue;

			joinedcolwidth = 0;
			for ( c1 = 0; c1 <= e->rightJoinLines; c1++ )
			{
				assert( c + c1 < (int)t.nb_cols );
				joinedcolwidth += t.col_width[c + c1];
			}

			if ( e->width < joinedcolwidth )
			{
				// entry too small, increase
				e->width = joinedcolwidth;
			}
			else
			{
				// sum off cols too small, enlarge last joined col to fit the size
				t.col_width[c + e->rightJoinLines] += e->width - joinedcolwidth;
			}
		}
	}

	double total_width = 0;
	for ( c = 0; c < (int)t.nb_cols; c++ )
	{
		total_width += Max( min_width, t.col_width[c] );
	}

	// available page width
	const double pw = pdf->GetPageWidth() - pdf->GetLeftMargin() - pdf->GetRightMargin();

	// center table on page
	t.left_p = pdf->GetLeftMargin() + ( pw - total_width ) / 2;

	int start_row = 1;

	const double y_break = pdf->GetPageHeight() - pdf->GetTopMargin() - pdf->GetBreakMargin();
	current_height = pdf->GetY();

	for ( r = 1; r < (int)t.nb_rows; r++ )
	{
		next_row_height = 0;
		if ( r + 1 < (int)t.nb_rows ) next_row_height = t.row_height[r + 1];

		//printf( "current_height %f next_row_height %f break bei %fi repeat %d\n", current_height, next_row_height, y_break, repeatHeader );

		if ( r + 1 == (int)t.nb_rows || ( next_row_height + current_height > y_break && repeatHeader ))
		{
			writeTablePart( t, start_row, r );

			start_row = r + 1;

			if ( pdf->GetY() > y_break && r + 1 < (int)t.nb_rows ) pdf->AddPage();

			current_height = pdf->GetY();
		}
		else
		{
			current_height += next_row_height;
		}

	}

	pdf->SetX( pdf->GetLeftMargin() );
	pdf->setDefaultFont();
}

/*****************************************************
**
**   PdfWriter   ---   writeTablePart
**
******************************************************/
void PdfWriter::writeTablePart( const Table &t, const int &start_row, const int &last_row )
{
	int c, r;
	//TableEntry *e;
	int align, frame;
	double swidth;

	for ( r = 0; r < (int)t.nb_rows; r++ )
	{
		// skip row if it is not the first (header) row and out of range
		if ( r > 0 && ( r < start_row || r >= last_row + 1 )) continue;

		pdf->SetX( t.left_p );
		for ( c = 0; c < (int)t.nb_cols; c++ )
		{
			const TableEntry *e = &t.contents[r].value[c];
			if ( e->isSkipped ) continue;

			// cell frame
			if ( t.show_grid ) frame = wxPDF_BORDER_FRAME;
			else
			{
				// right border from cell param or from cell on the left if lines are skipped
				bool show_rightborder = t.col_line[c];

				if ( e->rightJoinLines > 0 )
				{
					assert( c + e->rightJoinLines < (int)t.nb_cols );
					show_rightborder = t.col_line[c + e->rightJoinLines];
				}

				// Fix 7.0.2: last column has always right border
				if ( c + e->rightJoinLines == (int)t.nb_cols -1 )
				{
					show_rightborder = true;
				}

				// default frame: outer frame and frame around 1st row (header)
				frame = wxPDF_BORDER_NONE;
				if ( c == 0 ) frame = wxPDF_BORDER_LEFT;
				if ( c == (int)t.nb_cols-1 ) frame = wxPDF_BORDER_RIGHT;
				if ( r == 0 || r == 1 ) frame |= wxPDF_BORDER_TOP;

				if ( r == last_row || e->isHeader ) frame |= wxPDF_BORDER_BOTTOM;

				if ( show_rightborder ) frame |= wxPDF_BORDER_RIGHT;
			}

			// font
			if ( e->isSymbol ) pdf->setSymbolFont( symbolzoomfactor );
			else if ( e->isHeader ) pdf->setDefaultBoldFont();
			else pdf->setDefaultFont();

			// alignment
			if ( t.col_alignment[c] & Align::Left ) align = wxPDF_ALIGN_LEFT;
			else if ( t.col_alignment[c] & Align::Right ) align = wxPDF_ALIGN_RIGHT;
			else align = wxPDF_ALIGN_CENTER;

			if ( e->isHeader ) align = wxPDF_ALIGN_CENTER;

			if ( e->rightJoinLines > 0 ) swidth = e->width;
			else swidth = t.col_width[c];

			// write cell
			//printf( "CELL row %d col %d height %f width %f contents %s\n", r, c, row_height[r], swidth, str2char( e->value) );
			pdf->Cell( swidth, t.row_height[r], e->value, frame, 0, align, e->isHeader ? 1 : 0 );
		}
		pdf->Ln();
	}
}

/*****************************************************
**
**   PdfWriter   ---  writeListInColumns
**
******************************************************/
void PdfWriter::writeListInColumns( const vector<wxString> v, const int &colnr )
{
	unsigned int i, j, index;
	unsigned int ePerCol = (int)v.size() / colnr;
	if ( (int)v.size() % colnr ) ePerCol++;

	Table table( colnr, ePerCol );
	for ( i = 0; i < (unsigned int)colnr; i++ )
	{
		table.col_alignment[i] = Align::Left;
		for ( j = 0; j < ePerCol; j++ )
		{
			index = i*ePerCol + j;
			if ( index < v.size() ) table.setEntry( i, j, v[index] );
		}
		s << Endl;
	}
	writeTable( table );
}

/*****************************************************
**
**   PdfWriter   ---  writeUranianDiagram
**
******************************************************/
void PdfWriter::writeUranianDiagram( vector<UranianDiagram> mp )
{
	// not yet implemented
}

