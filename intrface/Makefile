#
# Makefile for cflib intrface helper
#
TARGET = extobfix.prg

SHELL = /bin/sh
SUBDIRS = 

srcdir = .
top_srcdir = ..
subdir = intrface

default: all

include $(top_srcdir)/CONFIGVARS
include $(top_srcdir)/RULES
include $(top_srcdir)/PHONY

all-here: $(TARGET)

# default overwrites
INCLUDES += -I..
#GENERAL += -mshort

# default definitions
OBJS = $(COBJS:.c=.o) $(SOBJS:.s=.o)
LIBS += -L../.build/m68000 -lcflib -lgem

$(TARGET): $(OBJS)
	$(CC) -o $@ $(CFLAGS) $(LDFLAGS) $(OBJS) $(LIBS)
	$(STRIP) $@

clean::
	rm -f $(TARGET)

include $(top_srcdir)/DEPENDENCIES
