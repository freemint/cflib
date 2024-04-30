#
# Makefile for cflib, a enhanced GEM library
#

SHELL = /bin/sh
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

all-here: libdirs doc/cflib.hyp

dist: distdir
	-chmod -R a+r $(distdir) 
	GZIP=$(GZIP) $(TAR) chzf $(distdir).tar.gz $(distdir)
	-rm -rf $(distdir)

# default overwrites

# default definitions
BASE_FLAVOURS = m68000
ifeq ($(WITH_020_LIB), yes)
  BASE_FLAVOURS += m68020
endif
ifeq ($(WITH_V4E_LIB), yes)
  BASE_FLAVOURS += coldfire
endif

FASTCALL_FLAVOURS = $(BASE_FLAVOURS)
ifeq ($(WITH_FASTCALL), yes)
FASTCALL_FLAVOURS += $(addsuffix -fastcall,$(BASE_FLAVOURS))
endif
FLAVOURS = $(FASTCALL_FLAVOURS) $(addsuffix -short,$(FASTCALL_FLAVOURS))

BUILDDIR=.build

LIBDIRS = $(addprefix $(BUILDDIR)/,$(FLAVOURS))

SUBDIRS = $(LIBDIRS) demo intrface
CLEAN_SUBDIRS = demo intrface
ALL_LIBS = $(addsuffix /libcflib.a,$(LIBDIRS))

libdirs:
	@for flavour in $(FLAVOURS); do \
	   dir=$(BUILDDIR)/$$flavour; \
	   mkdir -p $$dir/.deps; \
	   cflags=""; \
	   for f in `echo $$flavour | sed -e 's/-/ /g'`; do \
	     case $$f in \
	     m68000) \
	        cflags="$$cflags $(CFLAGS_m68000)"; \
	        ;; \
	     m68020) \
	        cflags="$$cflags $(CFLAGS_m68020)"; \
	        ;; \
	     coldfire) \
	        cflags="$$cflags $(CFLAGS_coldfire)"; \
	        ;; \
	     short) \
	        cflags="$$cflags $(CFLAGS_short)"; \
	        ;; \
	     fastcall) \
	        cflags="$$cflags $(CFLAGS_fastcall)"; \
	        ;; \
	     esac; \
	   done; \
	   instdir=`$(CC) $$cflags -print-multi-directory`; \
	   ( echo "SHELL = /bin/sh"; \
	     echo 'srcdir = ../..'; \
	     echo 'top_srcdir = ../..'; \
	     echo "subdir = $$dir"; \
             echo 'include $$(top_srcdir)/CONFIGVARS'; \
             echo 'include $$(top_srcdir)/RULES'; \
             echo 'include $$(top_srcdir)/SRCFILES'; \
	     echo ""; \
	     echo "cflags = $$cflags"; \
	     echo "instdir = $$instdir"; \
	     echo "all: all-here"; \
	     echo ""; \
	     echo "all-here: libcflib.a"; \
	     echo ""; \
	     echo 'OBJS = $$(COBJS:.c=.o) $$(SOBJS:.S=.o)'; \
	     echo 'libcflib.a: $$(OBJS)'; \
	     echo '	$$(AM_V_at)$$(RM) $$@'; \
	     echo '	$$(AM_V_AR)$$(AR) rc $$@ $$(OBJS)'; \
	     echo '	$$(AM_V_RANLIB)$$(RANLIB) $$@'; \
	     echo ""; \
	     echo "install:"; \
	     echo '	install -m 755 -d $$(DESTDIR)$$(PREFIX)/lib/$$(instdir)'; \
	     echo '	install -m 644 libcflib.a $$(DESTDIR)$${PREFIX}/lib/$$(instdir)'; \
	     echo ""; \
	     echo "uninstall:"; \
	     echo '	rm -f $$(DESTDIR)$${PREFIX}/lib/$$(instdir)libcflib.a'; \
	     echo ""; \
	     echo 'include $$(top_srcdir)/DEPENDENCIES'; \
	   ) > $$dir/Makefile; \
	done

clean::
	rm -rf $(BUILDDIR)

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


install:
	install -m 755 -d $(DESTDIR)$(PREFIX)/include
	install -m 644 cflib.h        $(DESTDIR)${PREFIX}/include
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
	rm -f $(DESTDIR)${PREFIX}/lib/libcflib16.a
	rm -f $(DESTDIR)/stguide/cflib.stg
	rm -f $(DESTDIR)/stguide/cflib.hyp
	rm -f $(DESTDIR)/stguide/cflib.ref
