#
# Makefile for cflib, a enhanced GEM library
#

SHELL = /bin/sh
SUBDIRS = demo intrface
BUILDLIB=cflib

srcdir = .
top_srcdir = .
subdir = 
top_builddir = .

default: all

include $(top_srcdir)/CONFIGVARS
include $(top_srcdir)/RULES
include $(top_srcdir)/PHONY
include $(srcdir)/VERSION

all-here: libs doc/cflib.hyp

dist: distdir
	-chmod -R a+r $(distdir) 
	GZIP=$(GZIP) $(TAR) chzf $(distdir).tar.gz $(distdir)
	-rm -rf $(distdir)

# default overwrites

# default definitions
GENFILES = .lib* libcflib*.a

ALL_LIBS = normal mshort

ifeq ($(WITH_020_LIB),yes)
ALL_LIBS += normal020
endif

ifeq ($(WITH_V4E_LIB),yes)
ALL_LIBS += normalv4e
endif

libs: $(ALL_LIBS)

normal:
	$(MAKE) libcflib.a TARGET=""

mshort:
	$(MAKE) libcflib16.a TARGET="16"
	
normal020:
	$(MAKE) libcflib020.a TARGET="020"

normalv4e:
	$(MAKE) libcflibv4e.a TARGET="v4e"

clean::
	rm -rf .lib*

libcflib$(TARGET).a: .lib$(TARGET)/objs
	$(AM_V_at)$(RM) $@
	$(AM_V_AR)$(AR) rc $@ \
		$(shell for file in `cat .lib$(TARGET)/objs` ; \
			do echo .lib$(TARGET)/$$file ; done)
	$(AM_V_RANLIB)$(RANLIB) $@

.lib$(TARGET)/objs: .lib$(TARGET) $(COBJS) $(SOBJS) $(HEADER)
	cd .lib$(TARGET); $(MAKE)

.lib$(TARGET):
	$(AM_V_at)$(MKDIR) $@
	$(AM_V_at)$(CP) Makefile.objs $@/Makefile

cflib.spec: cflib.spec.in VERSION
	$(AM_V_at)rm -f $@
	$(AM_V_GEN)sed 's,@VERSION@,$(VERSION),g' $@.in >$@

docu: doc/cflib.hyp

doc/cflib.hyp: doc/cflib.stg
	@if test "$(HCP)" != ""; then hcp="$(HCP)"; else hcp=hcp; fi; \
	if test "$$hcp" != "" && $$hcp --version >/dev/null 2>&1; then \
		$$hcp -o $@ $<; \
	else \
		echo "HCP not found, help file not compiled" >&2; \
	fi


all-recursive:: normal

install:
	install -m 755 -d $(DESTDIR)$(PREFIX)/include
	install -m 644 cflib.h        $(DESTDIR)${PREFIX}/include
	install -m 755 -d $(DESTDIR)$(PREFIX)/lib
	install -m 644 libcflib.a     $(DESTDIR)${PREFIX}/lib
	install -m 755 -d $(DESTDIR)$(PREFIX)/lib/mshort
	install -m 644 libcflib16.a   $(DESTDIR)${PREFIX}/lib/mshort/libcflib.a
	ln -sf mshort/libcflib.a	      $(DESTDIR)${PREFIX}/lib/libcflib16.a
ifeq ($(WITH_020_LIB),yes)
	install -m 755 -d $(DESTDIR)$(PREFIX)/lib/m68020-60
	install -m 644 libcflib020.a  $(DESTDIR)${PREFIX}/lib/m68020-60/libcflib.a
endif
ifeq ($(WITH_V4E_LIB),yes)
	install -m 755 -d $(DESTDIR)$(PREFIX)/lib/m5475
	install -m 644 libcflibv4e.a  $(DESTDIR)${PREFIX}/lib/m5475/libcflib.a
endif
	install -m 755 -d $(DESTDIR)/stguide
	if test -f doc/cflib.hyp; then \
		install -m 644 doc/cflib.hyp      $(DESTDIR)/stguide; \
		install -m 644 doc/cflib.ref      $(DESTDIR)/stguide; \
	else \
		echo "HCP not found, installing documentation source" >&2; \
		install -m 644 doc/cflib.stg      $(DESTDIR)/stguide; \
	fi

uninstall:
	rm -f $(DESTDIR)${PREFIX}/include/cflib.h
	rm -f $(DESTDIR)${PREFIX}/lib/libcflib.a
	rm -f $(DESTDIR)${PREFIX}/lib/libcflib16.a
	rm -f $(DESTDIR)${PREFIX}/lib/mshort/libcflib.a
ifeq ($(WITH_020_LIB),yes)
	rm -f $(DESTDIR)${PREFIX}/lib/m68020-60/libcflib.a
endif
ifeq ($(WITH_V4E_LIB),yes)
	rm -f $(DESTDIR)${PREFIX}/lib/m5475/libcflib.a
endif
	rm -f $(DESTDIR)/stguide/cflib.stg
	rm -f $(DESTDIR)/stguide/cflib.hyp
	rm -f $(DESTDIR)/stguide/cflib.ref
