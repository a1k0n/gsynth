## Process this file with automake to produce Makefile.in

INCLUDES = @GTK_CFLAGS@ -I$(top_srcdir)

EXTRA_DIST = gsyn.xml

libguigtk_a_SOURCES = gladesrc.c gladesig.c \
	gladesrc.h gladesig.h \
	support.h support.c \
	machbuild.c machbuild.h \
	tracker.c tracker.h \
	splash.c splash.h

noinst_LIBRARIES = libguigtk.a

