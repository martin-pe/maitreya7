bin_PROGRAMS = maitreya7.bin maitreya7

maitreya7_SOURCES = maitreya7.template

AM_CPPFLAGS = $(WX_CPPFLAGS) -I. -I../base -I../jyotish -I../pdf -I../dialogs -Wall -DSTRICT

maitreya7_bin_SOURCES = ApplicationWindow.cpp ApplicationWindow.h AshtakaVargaView.cpp AshtakaVargaView.h \
	BasicVedicChart.cpp BasicVedicChart.h BasicView.cpp BasicView.h BasicWidget.cpp BasicWidget.h \
	ChildWindow.cpp ChildWindow.h DasaGraphicView.cpp DasaGraphicView.h DasaView.cpp DasaView.h \
	DiagramWidget.cpp DiagramWidget.h Document.cpp Document.h EphemView.cpp EphemView.h \
	GraphicalChart.cpp GraphicalChart.h GraphicGrid.cpp GraphicGrid.h GraphicView.cpp GraphicView.h \
	guibase.cpp guibase.h MainWindowControl.cpp MainWindowControl.h MenuProvider.cpp MenuProvider.h \
	MultipleChildWindow.cpp MultipleChildWindow.h NonDocTextView.cpp NonDocTextView.h \
	NotebookNodes.cpp NotebookNodes.h NotebookWidget.cpp NotebookWidget.h Painter.cpp Painter.h \
	PartnerView.cpp PartnerView.h PdfBase.cpp PdfBase.h PdfPainter.cpp PdfPainter.h \
	PdfPrinter.cpp PdfPrinter.h PdfWriter.cpp PdfWriter.h SarvatobhadraChart.cpp SarvatobhadraChart.h \
	SbcView.cpp SbcView.h SolarView.cpp SolarView.h SplitterWidget.cpp SplitterWidget.h \
	Statusbar.cpp Statusbar.h TextView.cpp TextView.h TextWidget.cpp TextWidget.h ToolPanel.cpp ToolPanel.h \
	TransitView.cpp TransitView.h TreeWidget.cpp TreeWidget.h UranianView.cpp UranianView.h \
	VargaView.cpp VargaView.h VedicChart.cpp VedicChart.h VedicRasiChart.cpp VedicRasiChart.h \
	WesternChart.cpp WesternChart.h YogaEditor.cpp YogaEditor.h YogaView.cpp YogaView.h


maitreya7_bin_LDADD =  ../jyotish/libjyotish.a ../dialogs/libmaitreya_dialogs.a ../base/libmaitreya_base.a \
	../swe/libswe.a ../db/libmaitreya_db.a  ../pdf/libmaitreya_pdf.a $(WX_LIBS) -lm -ldl $(LIBFONTCONFIG)

all-am: maitreya7

maitreya7: maitreya7.template
	sed "s|DATAROOTDIR|${datarootdir}|g; s|ABSTOPBUILDDIR|${abs_top_builddir}|g" maitreya7.template > maitreya7
	chmod 755 maitreya7

links: 
	for l in `cat ../../po/LINGUAS`; \
	do \
	echo Creating link for language $$l; \
	test -d $$l || mkdir $$l; \
	test -L $$l/maitreya7.mo || ln -fs ../../../po/$$l.gmo $$l/maitreya7.mo; \
	done

pkgdatadir = ${datarootdir}/maitreya7

SUBDIRS = 

EXTRA_DIST = Makefile.win maitreya.rc gui.vcproj maitreya7.template

clean:
	rm -f *.o *~ *.bak maitreya7.bin maitreya7
	rm -fr obj bin


