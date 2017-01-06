# Unix Makefile for GlkLoader library

# GlkLoader 0.3.0
MAJOR=0
MINOR=3
MICRO=0

# This generates two files. One, of course, is libglkloader.a -- the library
# itself. The other is Make.glkloader; this is a snippet of Makefile code
# which locates the cheapglk library.
#
# When you install glkloader, you must put libglkloader.a in the lib directory,
# and glk.h, glkstart.h, and Make.glkloader in the include directory.

GLKNAME = glkloader
GLKINCLUDEDIR = /usr/local/include/glk/
GLKLIBDIR = /usr/local/lib/

# Pick a C compiler.
#CC = cc
CC = gcc
LINK = gcc
INSTALL = install -g 0 -o 0

OPTIONS = -g -Wall -fPIC -DLINUX
INCLUDEDIRS = -I$(GLKINCLUDEDIR)

CFLAGS = $(OPTIONS) $(INCLUDEDIRS)
LDFLAGS = -rdynamic
LIBS = -ldl

GLKLIBSTATIC = lib$(GLKNAME).a
GLKLIBSHARED = lib$(GLKNAME).so
GLKMAKEFILE = Make.$(GLKNAME)

OBJS =  \
	glkloader.o config.o glkfuncs.o blorbfuncs.o dispafuncs.o unixfuncs.o \
    linux_defines.o gi_blorb.o gi_dispa.o

HEADERS = \
	config.h defines.h linux_defines.h

all: $(GLKLIBSTATIC) $(GLKLIBSHARED) $(GLKMAKEFILE)

$(GLKLIBSTATIC): $(OBJS)
	ar r $(GLKLIBSTATIC) $(OBJS)
	ranlib $(GLKLIBSTATIC)

$(GLKLIBSHARED): $(OBJS)
	$(LINK) $(LDFLAGS) -shared -Wl,-soname,$(GLKLIBSHARED).$(MAJOR) -o $(GLKLIBSHARED).$(MAJOR).$(MINOR).$(MICRO) $(OBJS) $(LIBDIRS) $(LIBS)

$(GLKMAKEFILE):
	echo LINKLIBS = $(LIBDIRS) $(LIBS) > $(GLKMAKEFILE)
	echo GLKLIB = -l$(GLKNAME) >> $(GLKMAKEFILE)

glkfuncs.c: generate_glkfuncs.pl glk.h func.txt
	./generate_glkfuncs.pl glk.h func.txt

$(OBJS): $(HEADERS)

install:
	$(INSTALL) -m 2755 -d $(GLKINCLUDEDIR)
	$(INSTALL) -m 0644 $(GLKMAKEFILE) $(GLKINCLUDEDIR)
	$(INSTALL) -m 2755 -d $(GLKLIBDIR)
	$(INSTALL) -m 0644 $(GLKLIBSTATIC) $(GLKLIBDIR)
	$(INSTALL) -m 0755 $(GLKLIBSHARED).$(MAJOR).$(MINOR).$(MICRO) $(GLKLIBDIR)
	ln -s -f $(GLKLIBDIR)/$(GLKLIBSHARED).$(MAJOR).$(MINOR).$(MICRO) $(GLKLIBDIR)/$(GLKLIBSHARED).$(MAJOR).$(MINOR)
	ln -s -f $(GLKLIBDIR)/$(GLKLIBSHARED).$(MAJOR).$(MINOR).$(MICRO) $(GLKLIBDIR)/$(GLKLIBSHARED).$(MAJOR)
	ln -s -f $(GLKLIBDIR)/$(GLKLIBSHARED).$(MAJOR).$(MINOR).$(MICRO) $(GLKLIBDIR)/$(GLKLIBSHARED)
	ldconfig

uninstall:
	rm -f $(GLKINCLUDEDIR)/$(GLKMAKEFILE)
	rmdir -p --ignore-fail-on-non-empty $(GLKINCLUDEDIR)
	rm -f $(GLKLIBDIR)/$(GLKLIBSTATIC)
	rm -f $(GLKLIBDIR)/$(GLKLIBSHARED)
	rm -f $(GLKLIBDIR)/$(GLKLIBSHARED).$(MAJOR)
	rm -f $(GLKLIBDIR)/$(GLKLIBSHARED).$(MAJOR).$(MINOR)
	rm -f $(GLKLIBDIR)/$(GLKLIBSHARED).$(MAJOR).$(MINOR).$(MICRO)
	rmdir -p --ignore-fail-on-non-empty $(GLKLIBDIR)

clean:
	rm -f *~ *.o $(GLKLIBSTATIC) $(GLKLIBSHARED).$(MAJOR).$(MINOR).$(MICRO) $(GLKMAKEFILE) glkfuncs.c
