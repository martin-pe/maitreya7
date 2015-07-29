INCLUDES = -I. -I../base

AM_CPPFLAGS = $(WX_CPPFLAGS) -Wall -DSTRICT

bin_PROGRAMS = maitreya_textclient
maitreya_textclient_SOURCES = textclient.cpp
maitreya_textclient_LDADD = libjyotish.a ../base/libmaitreya_base.a ../swe/libswe.a  $(WX_LIBS)

BUILT_SOURCES = MdlParser.hh 

MdlLexer.ll : MdlParser.yy 
MdlParser.hh : MdlParser.yy

MdlLexer.cc : MdlLexer.ll
	$(LEX) -t MdlLexer.ll > MdlLexer.cc

MdlParser.cc : MdlParser.yy
	$(YACC) -d MdlParser.yy
	mv y.tab.c MdlParser.cc
	mv y.tab.h MdlParser.hh

noinst_LIBRARIES = libjyotish.a

libjyotish_a_SOURCES = ArabicParts.cpp ArabicParts.h Ashtakavarga.cpp Ashtakavarga.h \
	AshtottariDasa.cpp AshtottariDasa.h Aspect.cpp Aspect.h astrobase.cpp astrobase.h \
	Dasa.cpp Dasa.h Eclipse.cpp Eclipse.h Ephemeris.cpp Ephemeris.h Expert.cpp Expert.h \
	Expression.cpp Expression.h GenericDasa.cpp GenericDasa.h Hora.cpp Hora.h Horoscope.cpp \
	Horoscope.h HtmlWriter.cpp HtmlWriter.h Jaimini.cpp Jaimini.h KalachakraDasa.cpp \
	KalachakraDasa.h MdlInterpreter.cpp MdlInterpreter.h Nakshatra.cpp Nakshatra.h \
	Partner.cpp Partner.h Planet.cpp Planet.h PrintoutHelper.cpp PrintoutHelper.h \
	SimpleHoroscope.cpp SimpleHoroscope.h SingletonLifeline.cpp SingletonLifeline.h textclient.cpp \
	TextHelper.cpp TextHelper.h TextWriter.cpp TextWriter.h Transit.cpp Transit.h UranianBase.cpp \
	UranianBase.h Uranian.cpp Uranian.h UranianHelper.cpp UranianHelper.h Varga.cpp Varga.h \
	VargaHoroscope.cpp VargaHoroscope.h VimsottariDasa.cpp VimsottariDasa.h Writer.cpp \
	WriterFactory.cpp Writer.h YogaConfig.cpp YogaConfig.h Yoga.cpp Yoga.h \
	MdlParser.cc MdlParser.hh MdlParser.yy MdlLexer.cc MdlLexer.ll


clean:
	rm -f lib*.a *.o y.tab.* core *~ *.bak
	rm -fr obj bin
	rm -f maitreya_textclient
	rm -f locations.dat

	
EXTRA_DIST = Makefile.win jyotish.vcproj textclient.vcproj MdlParser.h*



