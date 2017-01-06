/**
 * glkloader.c
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
 *    derived from this software without specific prior written permission.
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

#include <stdlib.h>
#include <stdio.h>
#include <glk.h>

#include "defines.h"
#include "config.h"

/**
 *  A global handle to the Glk library
 */

LIB_HANDLE gGlkLib;

/**
 *  main() - load the Glk library and call its main routine
 */

int main( int argc, char **argv )
{
    int newArgC;
    char ** newArgV;
    char * rcFileName = NULL;
    char * libName = NULL;
    char * libFullName = NULL;
    char * errBuf = NULL;
    int (*mainPtr)(char, char **);
    int retVal;
    int i, j;
    int glkParamIndex = -1;
    int rcParamIndex = -1;
    int numDroppedParams = 0;
    int numNewParams = 0;
    char ** newParams = NULL;

    /* first pass : find the -glk and -rc parameters */
    for (i = 0; i < argc; i++) {
        if (strcmp( argv[i], "-glk" ) == 0) {
            glkParamIndex = i;
            numDroppedParams += 2;
            i++;
            if (i >= argc) {
                usage( argv[0] );
                exit( 1 );
            } else {
                if (NULL != libName) {
                    usage( argv[0] );
                    exit( 1 );
                }
                libName = argv[i];
            }
        } else if (strcmp( argv[i], "-rc" ) == 0) {
            rcParamIndex = i;
            numDroppedParams += 2;
            i++;
            if (i >= argc) {
                usage( argv[0] );
                exit( 1 );
            } else {
                if (NULL != rcFileName) {
                    usage( argv[0] );
                    exit( 1 );
                }
                rcFileName = argv[i];
            }
        }
    }

    /* parse the config file */
    readConfig( libName, rcFileName, &libFullName, &numNewParams, &newParams );
    if (NULL == libFullName) {
        usage( argv[0] );
        exit( 1 );
    }

    /* second pass : create the new argv array */
    newArgC = argc - numDroppedParams + numNewParams;
    newArgV = (char **) malloc( newArgC * sizeof(char *) );
    newArgV[0] = argv[0];  
    for (i = 1, j = 0; j < numNewParams; i++, j++) {
        newArgV[i] = newParams[j];
    }
    for (j = 1; j < argc; j++, i++) {
        if (j == glkParamIndex || j == rcParamIndex) {
            j += 2;
        }
        if (j < argc) {
            newArgV[i] = argv[j];
        }
    }

    /* load the Glk library */
    gGlkLib = loadLibrary( libFullName, &errBuf );
    if (NULL != errBuf) {
        error( errBuf );
        exit( 1 );
    }

    /* load the library's main function */ 
    mainPtr = loadSymbol( gGlkLib, "main", &errBuf );
    if (NULL != errBuf) {
        error( errBuf );
        exit( 1 );
    }

    /* call the library's main function */
    retVal = (*mainPtr)( newArgC, newArgV );

    /* free the configuration data  */
    freeConfigData( &libFullName, &numNewParams, &newParams );

    /* unload the Glk library */
    free( newArgV );
    unloadLibrary( gGlkLib, &errBuf );
    if (NULL != errBuf) {
        error( errBuf );
        exit( 1 );
    }

    return retVal;
}
