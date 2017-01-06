/**
 * xpconfig.h
 *
 * Copyright (c) 2001 Joe Mason <jcmason@uwaterloo.ca>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without pecific prior written permission.
 *
 *    THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 *    IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 *    OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 *    IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 *    INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 *    NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 *    THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _XPCONFIG_H_
#define _XPCONFIG_H_

#include "xpcdefines.h"

/**
 * Data types
 */

typedef struct _xpcString {
    char * value;
} xpcString;

typedef struct _xpcInt {
    int value;
} xpcInt;

typedef struct _xpcBool {
    BOOL value;
} xpcBool;

typedef struct _xpcStringList {
    int length;
    xpcString ** list;
} xpcStringList;

typedef struct _xpcIntList {
    int length;
    xpcInt ** list;
} xpcIntList;

typedef struct _xpcBoolList {
    int length;
    xpcBool ** list;
} xpcBoolList;

void xpcInitialize( const char * application );
void xpcFinalize( const char * application );

xpcString * xpcReadString( const char * application, const char * section,
                           const char * key );
xpcInt * xpcReadInt( const char * application, const char * section,
                     const char * key );
xpcBool * xpcReadBool( const char * application, const char * section,
                       const char * key );

xpcStringList * xpcReadStringList( const char * application,
                                   const char * section, const char * key );
xpcIntList * xpcReadIntList( const char * application, const char * section,
				             const char * key );
xpcBoolList * xpcReadBoolList( const char * application, const char * section,
                               const char * key );

void xpcFreeString( xpcString ** obj );
void xpcFreeInt( xpcInt ** obj );
void xpcFreeBool( xpcBool ** obj );

void xpcFreeStringList( xpcStringList ** obj );
void xpcFreeIntList( xpcIntList ** obj );
void xpcFreeBoolList( xpcBoolList ** obj );

/* Extension functions for various platforms */

void xpcSetExtensionData( const char * application, const char * filename,
                          BOOL isVerbose );

#endif /* _XPCONFIG_H_ */
