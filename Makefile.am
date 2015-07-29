
SUBDIRS = po src debian 

EXTRA_DIST = config.rpath config.sub config.guess stamp-h COPYING Makefile.win config.h \
	maitreya.sln setup_vcpp_resources.bat maitreya7.desktop \
	Doxyfile maitreya.spec create_mac_release.sh MacOS/*

pkgdatadir = ${datarootdir}/maitreya7

dist_pkgdata_DATA = COPYING debian/changelog po/maitreya6.pot


all-am: echo-font-message

echo-font-message:
	@echo ""
	@echo ""
	@echo "HOW TO START"
	@echo "------------"
	@echo "You can start the program with the following commands"
	@echo "1. local installation: ./src/gui/maitreya7"
	@echo "2. system wide installation: type 'sudo make install' followed by 'maitreya7'"
	@echo ""
	@echo ""
	@echo "FONT INFO"
	@echo "---------"
	@echo "The symbol font will be installed on most systems by 'make install'."
	@echo "You can also manually install it."
	@echo "It is located at src/fonts/MaitreyaSymbols.ttf"
	@echo ""

AUTOMAKE_OPTIONS = dist-bzip2 dist-zip

ACLOCAL_AMFLAGS = -I m4

desktopdir = ${datarootdir}/applications
desktop_DATA = maitreya7.desktop

clean: clean-recursive
	rm -f *~ *.bak fonts
	rm -fr autom4te.cache bin obj doc/*

