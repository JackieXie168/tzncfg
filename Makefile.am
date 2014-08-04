SUBDIRS=nvram nvram/include nvram/testcase nvram/conf nvram/host xutils

EXTRA_DIST = doc/* conf/*.conf

noinst_PROGRAMS = tznconfig tokenlist xobjconf trim

tokenlist_SOURCES = tokenlist.c
tokenlist_LDADD = $(top_srcdir)/nvram/libnvram-1.0.la $(top_srcdir)/xutils/libxutils-1.0.la

tznconfig_SOURCES = tznconfig.c
tznconfig_LDADD = $(top_srcdir)/nvram/libnvram-1.0.la $(top_srcdir)/xutils/libxutils-1.0.la

xobjconf_SOURCES = xobjconf.c
xobjconf_LDADD = $(top_srcdir)/nvram/libnvram-1.0.la $(top_srcdir)/xutils/libxutils-1.0.la

trim_SOURCES = trim.c
trim_LDADD = $(top_srcdir)/nvram/libnvram-1.0.la $(top_srcdir)/xutils/libxutils-1.0.la

AM_CFLAGS = -I$(top_srcdir)/nvram/include
AM_LDFLAGS = -lm

CLEANFILES = *~ *.log
