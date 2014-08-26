# $(top_srcdir)/Makefile.am

SUBDIRS=nvram nvram/include nvram/testcase nvram/conf nvram/host

noinst_PROGRAMS = tznconfig
tznconfig_SOURCES = tznconfig.c
tznconfig_LDADD = $(top_srcdir)/nvram/libnvram-1.0.la

INCLUDES = -I$(top_srcdir)/nvram/include

CLEANFILES = *~ *.log
DISTCLEANFILES = Makefile.in aclocal.m4 autoscan.log compile config.guess config.sub configure configure.scan ltmain.sh *~
