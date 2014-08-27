# $(top_srcdir)/nvram/Makefile.am
#
# AUTOMAKE_OPTIONS= foreign

if IS_DARWIN
bin_PROGRAMS= xnvram
else
bin_PROGRAMS= nvram
endif

## Source directory
SUBDIRS= . testcase

#Distribute these directories:
DIST_SUBDIRS = .

#Build in these directories:

COMMON_FILES=\
	./include/bcmnvram.h \
	./include/typedefs.h

NVRAM_FILES= $(COMMON_FILES) \
	./src/nvram_cavium.c \
	./src/nvram_main.c
	
if IS_DARWIN
xnvram_SOURCES= $(NVRAM_FILES)
else
nvram_SOURCES= $(NVRAM_FILES)
endif

AM_CFLAGS= -I$(top_srcdir) -Iinclude -Invram
#LIBS= -L/usr/lib -lpthread

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = nvram-1.0.pc

EXTRA_DIST=autogen.sh

h_sources = ./include/bcmnvram.h ./include/typedefs.h
c_sources = ./src/nvram_cavium.c

library_includedir=$(includedir)/$(GENERIC_LIBRARY_NAME)-$(GENERIC_API_VERSION)/$(GENERIC_LIBRARY_NAME)
library_include_HEADERS = $(h_sources)

lib_LTLIBRARIES= libnvram-1.0.la
libnvram_1_0_la_SOURCES= $(h_sources) $(c_sources)
libnvram_1_0_la_CFLAGS = -fPIC $(AM_CFLAGS)
#libnvram_1_0_la_LIBADD= libnvram.la
libnvram_1_0_la_LDFLAGS= -version-info $(GENERIC_LIBRARY_VERSION) -release $(GENERIC_RELEASE)

METASOURCES = AUTO
#DISTCLEANFILES = Makefile.in
CLEANFILES = *~ *.log
