INCLUDES = -I$(top_srcdir)
SUBDIRS = libgsyn modules gui-common gui-gtk gui-mfc images
CPPFLAGS = -D_POSIX -D_MODULEDIR=\"$(moduledir)\"

EXTRA_DIST = test.gsn

bin_PROGRAMS = gsynth

gsynth_SOURCES = gsynth.cpp gsynth.h stlstuff.h \
	gsynfile.h gsynfile.cpp

gsynth_LDADD = libgsyn/libgsyn.a \
	gui-gtk/libguigtk.a \
	gui-common/libguicommon.a \
	@GTK_LIBS@

gsynth_DEPENDENCIES = libgsyn/libgsyn.a \
	gui-gtk/libguigtk.a \
	gui-common/libguicommon.a \
	Makefile
