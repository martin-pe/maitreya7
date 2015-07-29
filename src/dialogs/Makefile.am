AM_CPPFLAGS = $(WX_CPPFLAGS) -I. -I../base -I../jyotish -I../gui -Wall -DSTRICT

noinst_LIBRARIES = libmaitreya_dialogs.a
libmaitreya_dialogs_a_SOURCES = AboutDialog.cpp AboutDialog.h AnimationPanel.cpp \
	AnimationPanel.h AspectColorPanel.cpp AspectColorPanel.h AspectPanel.cpp AspectPanel.h \
	AtlasAliasDialog.cpp AtlasAliasDialog.h AtlasDialog.cpp AtlasDialog.h \
	AtlasEditDialog.cpp AtlasEditDialog.h AtlasImportDialog.cpp AtlasImportDialog.h \
	AtlasPanel.cpp AtlasPanel.h ColorPanel.cpp ColorPanel.h CommonInputFields.cpp \
	CommonInputFields.h ConfigDialog.cpp ConfigDialog.h ConfigPanel.cpp ConfigPanel.h \
	DasaColorPanel.cpp DasaColorPanel.h DasaPanel.cpp DasaPanel.h DataDialog.cpp DataDialog.h \
	DateTimeCtrl.cpp DateTimeCtrl.h DefaultLocationPanel.cpp DefaultLocationPanel.h \
	DialogElements.cpp DialogElements.h DragChart.cpp DragChart.h EphemerisPanel.cpp \
	EphemerisPanel.h ExportDialog.cpp ExportDialog.h FontPanel.cpp FontPanel.h GeneralPanel.cpp \
	GeneralPanel.h guivalidators.cpp guivalidators.h MultipleViewPanel.cpp MultipleViewPanel.h \
	ObjectColorPanel.cpp ObjectColorPanel.h PrintConfigurationDialog.cpp PrintConfigurationDialog.h \
	PrintoutPanel.cpp PrintoutPanel.h ToolbarPanel.cpp ToolbarPanel.h VargaPanel.cpp VargaPanel.h \
	VedicCalculationPanel.cpp VedicCalculationPanel.h VedicChartPanel.cpp VedicChartPanel.h \
	VedicPanel.cpp VedicPanel.h ViewPanel.cpp ViewPanel.h WesternCalculationPanel.cpp \
	WesternCalculationPanel.h WesternChartPanel.cpp WesternChartPanel.h WesternPanel.cpp \
	WesternPanel.h YogaEditorPanel.cpp YogaEditorPanel.h

EXTRA_DIST = Makefile.win dialogs.vcproj

clean:
	rm -f lib*.a *.o *~ *.bak
	rm -fr obj bin

