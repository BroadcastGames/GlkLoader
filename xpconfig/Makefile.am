if XPC_RCFILE
XPC_PLATFORM_FILE = platform_rcfile.c
else
if XPC_REGISTRY
XPC_PLATFORM_FILE = platform_registry.c
else
if XPC_XML
XPC_PLATFORM_FILE = platform_xml.c
else
XPC_PLATFORM_FILE =
endif	# XPC_XML
endif	# XPC_REGISTRY
endif	# XPC_RCFILE

xpconfig.h:	generate_header.pl
	./generate_header.pl $(XPC_PLATFORM_FILE)

BUILT_SOURCES = xpconfig.h

include_HEADERS = xpconfig.h xpcdefines.h

lib_LTLIBRARIES = libxpconfig.la
libxpconfig_la_SOURCES = \
	xpcdefines.h \
	xpconfig.c xpconfig.h \
	$(XPC_PLATFORM_FILE) platform.h \
	applist.c applist.h \
	sectionlist.c sectionlist.h \
	keylist.c keylist.h \
	tokenlist.c tokenlist.h \
    generate_header.pl
