/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/dialogs/VedicChartPanel.h
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

#ifndef VEDICCHARTPANEL_H
#define VEDICCHARTPANEL_H

#ifdef __GNUG__
#pragma interface "VedicChartPanel.h"
#endif

#include "ConfigPanel.h"
#include "constants.h"

// begin wxGlade: ::dependencies
// end wxGlade

class ChartProperties;
class VedicChartPreviewPanel;

class wxButton;
class wxCheckBox;
class wxChoice;
class wxStaticBox;
class wxStaticLine;
class wxStaticText;
class wxTextCtrl;

/*************************************************//**
*
* \brief ConfigPanel for Vedic chart parameters
*
******************************************************/
class VedicChartPanel: public ConfigPanel
{
	DECLARE_CLASS( ConfVedicChartPanel )

public:
    // begin wxGlade: VedicChartPanel::ids
    // end wxGlade

	VedicChartPanel( wxWindow* parent );
	~VedicChartPanel();

	virtual void setData();
	virtual bool saveData();

private:
    // begin wxGlade: VedicChartPanel::methods
    void set_properties();
    void do_layout();
    // end wxGlade

	ChartProperties *chartprops;
	int mode;
	bool showTransits;
	VedicGraphicStyle style;

	void OnCommand( wxCommandEvent& );
	void OnBenefics( wxCommandEvent& );
	void updateUI();
	void calculateStyle();

protected:
    // begin wxGlade: VedicChartPanel::attributes
    wxStaticBox* sizer_preview_staticbox;
    wxStaticBox* sizer_sbc_staticbox;
    wxStaticBox* sizer_varga_chart_staticbox;
    wxStaticBox* sizer_general_staticbox;
    wxStaticBox* sizer_defaultview_staticbox;
    wxChoice* choice_defaultskin;
    wxCheckBox* check_retro;
    wxCheckBox* check_use_colors;
    wxStaticText* label_chart_type;
    wxChoice* choice_chart_type;
    wxStaticText* label_chartstyle;
    wxChoice* choice_chart;
    wxStaticText* label_north_style;
    wxChoice* choice_north_chart;
    wxStaticText* label_chartcenter;
    wxChoice* choice_chart_center;
    wxCheckBox* check_arudhas;
    wxCheckBox* check_sanskrit_symbols;
    wxCheckBox* check_afflictions;
    wxCheckBox* check_nakshatra_qualities;
    VedicChartPreviewPanel* window_preview;
    wxPanel* panel_preview;
    wxCheckBox* checktransit;
    // end wxGlade
}; // wxGlade: end class


#endif // VEDICCHARTPANEL_H
