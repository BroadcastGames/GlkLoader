#!/usr/bin/perl

# generate_header.pl

# Copyright (c) 2001 Joe Mason <jcmason@uwaterloo.ca>
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 3. The name of the author may not be used to endorse or promote products
#    derived from this software without specific prior written permission.
#
#    THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
#    IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
#    OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
#    IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
#    INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
#    NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
#    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
#    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
#    THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

open (OUT_FILE, ">xpconfig.h") ||
   die "Can't open xpconfig.h";

print OUT_FILE "/**\n";
print OUT_FILE " * xpconfig.h\n";
print OUT_FILE " *\n";
print OUT_FILE " * Copyright (c) 2001 Joe Mason <jcmason\@uwaterloo.ca>\n";
print OUT_FILE " * All rights reserved.\n";
print OUT_FILE " *\n";
print OUT_FILE " * Redistribution and use in source and binary forms, with ";
	print OUT_FILE "or without\n";
print OUT_FILE " * modification, are permitted provided that the following ";
	print OUT_FILE "conditions\n";
print OUT_FILE " * are met:\n";
print OUT_FILE " * 1. Redistributions of source code must retain the above ";
	print OUT_FILE "copyright\n";
print OUT_FILE " *    notice, this list of conditions and the following ";
	print OUT_FILE "disclaimer.\n";
print OUT_FILE " * 2. Redistributions in binary form must reproduce the ";
	print OUT_FILE "above copyright\n";
print OUT_FILE " *    notice, this list of conditions and the following ";
	print OUT_FILE "disclaimer in the\n";
print OUT_FILE " *    documentation and/or other materials provided with ";
	print OUT_FILE "the distribution.\n";
print OUT_FILE " * 3. The name of the author may not be used to endorse or ";
	print OUT_FILE "promote products\n";
print OUT_FILE " *    derived from this software without pecific prior ";
	print OUT_FILE "written permission.\n";
print OUT_FILE " *\n";
print OUT_FILE " *    THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' ";
	print OUT_FILE "AND ANY EXPRESS OR\n";
print OUT_FILE " *    IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, ";
	print OUT_FILE "THE IMPLIED WARRANTIES\n";
print OUT_FILE " *    OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR ";
	print OUT_FILE "PURPOSE ARE DISCLAIMED.\n";
print OUT_FILE " *    IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY ";
	print OUT_FILE "DIRECT, INDIRECT,\n";
print OUT_FILE " *    INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL ";
	print OUT_FILE "DAMAGES (INCLUDING, BUT\n";
print OUT_FILE " *    NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR ";
	print OUT_FILE "SERVICES; LOSS OF USE,\n";
print OUT_FILE " *    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER ";
	print OUT_FILE "CAUSED AND ON ANY\n";
print OUT_FILE " *    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT ";
	print OUT_FILE "LIABILITY, OR TORT\n";
print OUT_FILE " *    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY ";
	print OUT_FILE "WAY OUT OF THE USE OF\n";
print OUT_FILE " *    THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF ";
	print OUT_FILE "SUCH DAMAGE.\n";
print OUT_FILE " */\n";
print OUT_FILE "\n";
print OUT_FILE "#ifndef _XPCONFIG_H_\n";
print OUT_FILE "#define _XPCONFIG_H_\n";
print OUT_FILE "\n";
print OUT_FILE "#include \"xpcdefines.h\"\n";
print OUT_FILE "\n";
print OUT_FILE "/**\n";
print OUT_FILE " * Data types\n";
print OUT_FILE " */\n";
print OUT_FILE "\n";
print OUT_FILE "typedef struct _xpcString {\n";
print OUT_FILE "    char * value;\n";
print OUT_FILE "} xpcString;\n";
print OUT_FILE "\n";
print OUT_FILE "typedef struct _xpcInt {\n";
print OUT_FILE "    int value;\n";
print OUT_FILE "} xpcInt;\n";
print OUT_FILE "\n";
print OUT_FILE "typedef struct _xpcBool {\n";
print OUT_FILE "    BOOL value;\n";
print OUT_FILE "} xpcBool;\n";
print OUT_FILE "\n";
print OUT_FILE "typedef struct _xpcStringList {\n";
print OUT_FILE "    int length;\n";
print OUT_FILE "    xpcString ** list;\n";
print OUT_FILE "} xpcStringList;\n";
print OUT_FILE "\n";
print OUT_FILE "typedef struct _xpcIntList {\n";
print OUT_FILE "    int length;\n";
print OUT_FILE "    xpcInt ** list;\n";
print OUT_FILE "} xpcIntList;\n";
print OUT_FILE "\n";
print OUT_FILE "typedef struct _xpcBoolList {\n";
print OUT_FILE "    int length;\n";
print OUT_FILE "    xpcBool ** list;\n";
print OUT_FILE "} xpcBoolList;\n";
print OUT_FILE "\n";
print OUT_FILE "void xpcInitialize( const char * application );\n";
print OUT_FILE "void xpcFinalize( const char * application );\n";
print OUT_FILE "\n";
print OUT_FILE "xpcString * xpcReadString( const char * application, const ";
	print OUT_FILE "char * section,\n";
print OUT_FILE "                           const char * key );\n";
print OUT_FILE "xpcInt * xpcReadInt( const char * application, const char ";
	print OUT_FILE "* section,\n";
print OUT_FILE "                     const char * key );\n";
print OUT_FILE "xpcBool * xpcReadBool( const char * application, const ";
	print OUT_FILE "char * section,\n";
print OUT_FILE "                       const char * key );\n";
print OUT_FILE "\n";
print OUT_FILE "xpcStringList * xpcReadStringList( const char * ";
print OUT_FILE "application,\n";
print OUT_FILE "                                   const char * section, ";
	print OUT_FILE "const char * key );\n";
print OUT_FILE "xpcIntList * xpcReadIntList( const char * application, ";
	print OUT_FILE "const char * section,\n";
print OUT_FILE "				             const char * key );\n";
print OUT_FILE "xpcBoolList * xpcReadBoolList( const char * application, ";
	print OUT_FILE "const char * section,\n";
print OUT_FILE "                               const char * key ";
	print OUT_FILE ");\n";
print OUT_FILE "\n";
print OUT_FILE "void xpcFreeString( xpcString ** obj );\n";
print OUT_FILE "void xpcFreeInt( xpcInt ** obj );\n";
print OUT_FILE "void xpcFreeBool( xpcBool ** obj );\n";
print OUT_FILE "\n";
print OUT_FILE "void xpcFreeStringList( xpcStringList ** obj );\n";
print OUT_FILE "void xpcFreeIntList( xpcIntList ** obj );\n";
print OUT_FILE "void xpcFreeBoolList( xpcBoolList ** obj );\n";
print OUT_FILE "\n";
print OUT_FILE "/* Extension functions for various platforms */\n";
print OUT_FILE "\n";

if ($ARGV[0] eq "platform_rcfile.c" ) {  
	print OUT_FILE "void xpcSetExtensionData( const char * application, const ";
		print OUT_FILE "char * filename,\n";
    print OUT_FILE "                          BOOL isVerbose );\n";
	print OUT_FILE "\n";
}
print OUT_FILE "#endif /* _XPCONFIG_H_ */\n";
