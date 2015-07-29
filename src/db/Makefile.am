AM_CPPFLAGS = $(WX_CPPFLAGS) -w -I/usr/local/include -DSTRICT 

noinst_LIBRARIES = libmaitreya_db.a
libmaitreya_db_a_SOURCES = wxsqlite3.cpp sqlite3.c sqlite3ext.h sqlite3.h \
	wx/wxsqlite3.h wx/wxsqlite3def.h wx/wxsqlite3dyn.h wx/wxsqlite3opt.h

EXTRA_DIST = Makefile.win db.vcproj 

clean:
	rm -f lib*.a *.o *~ *.bak
	rm -fr obj bin



