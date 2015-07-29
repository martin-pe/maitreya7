/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/gui/PdfBase.h
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

#ifndef _PDFBASE_H_
#define _PDFBASE_H_

#ifdef __GNUG__
#pragma interface "PdfBase.h"
#endif

#include <wx/font.h>
#include <wx/gdicmn.h>
#include <wx/pdfdoc.h>

/*************************************************//**
*
* \brief adds Footer and font methods to wxPdfDocument
*
******************************************************/
class BasePdfDocument : public wxPdfDocument
{
public:

	BasePdfDocument();

	virtual void Footer();

	void setDefaultFont();
	void setDefaultBoldFont();
	void setTinyFont();
	void setGraphicFont();

	void setHeader1Font();
	void setHeader2Font();
	void setHeader3Font();

	void setSymbolFont( const int = 100 );

private:

};

#endif


