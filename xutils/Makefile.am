## Source directory
SUBDIRS= .

#Distribute these directories:
DIST_SUBDIRS = .

#Build in these directories:

COMMON_FILES=xutils.h

AM_CFLAGS= -I$(top_srcdir) -I. -Ixutils

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = xutils-1.0.pc

EXTRA_DIST=autogen.sh

h_sources = xutils.h
c_sources = xutils.c

library_includedir=$(includedir)/$(X_LIBRARY_NAME)-$(GENERIC_API_VERSION)/$(X_LIBRARY_NAME)
library_include_HEADERS = $(h_sources)

lib_LTLIBRARIES= libxutils-1.0.la
libxutils_1_0_la_SOURCES= $(h_sources) $(c_sources)
libxutils_1_0_la_CFLAGS = -fPIC $(AM_CFLAGS)
libxutils_1_0_la_LDFLAGS= -version-info $(GENERIC_LIBRARY_VERSION) -release $(GENERIC_RELEASE)

METASOURCES = AUTO
CLEANFILES = *~ *.log
