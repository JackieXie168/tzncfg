# $Id: Makefile,v 2.0 2008/10/08 18:55:40 fusz Exp fusz $

prog_name = tznconfig

######################################
#
# Generic makefile
#
# by Jackie Xie
# email: jackie.CPlusPlus@gmail.com
#
# Copyright (c) 2008 Jackie Xie
# All rights reserved.
#
# No warranty, no liability;
# you use this at your own risk.
#
# You are free to modify and
# distribute this without giving
# credit to the original author.
#
######################################
### Customising
#
# Adjust the following if necessary; EXECUTABLE is the target
# executable's filename, and LIBS is a list of libraries to link in
# (e.g. alleg, stdcx, iostr, etc). You can override these on make's
# command line of course, if you prefer to do it that way.
#
OS := $(shell uname -s)
EXECUTABLE := $(prog_name)
LIBS := -L/usr/local/lib -lnvram-1.0

# Now alter any implicit rules' variables if you like, e.g.:
#
CFLAGS := -g -Wall -O3 -fno-stack-protector -I/usr/local/include
CXXFLAGS := $(CFLAGS)
#CC := g++
CC := gcc

# The next bit checks to see whether rm is in your djgpp bin
# directory; if not it uses del instead, but this can cause (harmless)
# `File not found' error messages. If you are not using DOS at all,
# set the variable to something which will unquestioningly remove
# files.
#
ifneq ($(wildcard $(DJDIR)/bin/rm),)
RM-F := rm -f
else
RM-F := del
endif

# You shouldn't need to change anything below this point.
#
# 從這裡開始，你應該不需要改動任何東西。
SOURCE := $(wildcard *.c) #$(wildcard *.cc) $(wildcard *.cpp) $(wildcard *.cxx)
#OBJS := $(patsubst %.c,%.o,$(patsubst %.cc,%.o,$(SOURCE)))
OBJS := $(patsubst %.c,%.o,$(patsubst %.cc,%.o,$(patsubst %.cpp,%.o,$(patsubst %.cpp,%.o,$(SOURCE)))))
DEPS := $(patsubst %.o,%.d,$(OBJS))
MISSING_DEPS := $(filter-out $(wildcard $(DEPS)),$(DEPS))
MISSING_DEPS_SOURCES := $(wildcard $(patsubst %.d,%.c,$(MISSING_DEPS)) $(patsubst %.d,%.cc,$(MISSING_DEPS)))
CPPFLAGS += -MD

.PHONY : everything deps objs clean veryclean rebuild

everything : $(EXECUTABLE)

deps : $(DEPS)

objs : $(OBJS)

clean:
	${RM-F} *.d *.o *~
ifeq ($(strip $(OS)),Darwin)
	${RM-F} $$(file `find .` | grep "bit executable" | awk '{print $$1}' | sed 's/\.*\://')
else ifeq ($(strip $(OS)),Linux)
	${RM-F} $$(file `find .` | grep "bit ...* executable" | awk '{print $$1}' | sed 's/\.*\://')
endif
	for dir in ${SUBDIR}; do \
		${MAKE} -C $$dir clean; \
	done

veryclean: clean
	@$(RM-F) $(EXECUTABLE)
	@$(RM-F) *.*~ *~

distclean: veryclean

rebuild: veryclean everything
ifneq ($(MISSING_DEPS),)
$(MISSING_DEPS) :
	@$(RM-F) $(patsubst %.d,%.o,$@)
endif
-include $(DEPS)

$(EXECUTABLE) : $(OBJS)
	$(CC) -o $(EXECUTABLE) $(OBJS) $(LIBS)
#	$(CC) -o $(EXECUTABLE) $(OBJS) $(addprefix -l,$(LIBS))
