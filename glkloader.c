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
#include "xpconfig/xpconfig.h"

/**
 *  The name of the app, for xpconfig
 */

const char * APP_NAME = "glkloader";

/**
 *  A global handle to the Glk library
 */

LIB_HANDLE gGlkLib;

/**
 *  loadGlkLibrary() - load the Glk library and call its main routine
 */

int loadGlkLibrary( char * libName, int argc, char ** argv, char ** errBuf )
{
    int i, j;
    int newArgC;
    char ** newArgV;
    int (*mainPtr)(char, char **);
    int retVal;
    xpcString * libFullName = NULL;
    xpcStringList * newParams = NULL;

    /* load given library */
    libFullName = xpcReadString( APP_NAME, libName, "path" );
    if (NULL == libFullName) {
        /* no [libName] section found, so assume libName is the full name */
        gGlkLib = loadLibrary( libName, errBuf );
    } else {
        gGlkLib = loadLibrary( libFullName->value, errBuf );
    }
    if (NULL != *errBuf) {
        xpcFreeString( &libFullName );
        return 0;
    }

    /* create the new argv array */
    newParams = xpcReadStringList( APP_NAME, libName, "params" );
    if (NULL == newParams) {
        newArgC = argc;
        newArgV = argv;
    } else {
        newArgC = argc + newParams->length;
        newArgV = (char **) malloc( newArgC * sizeof(char *) );
        newArgV[0] = argv[0];  
        for (i = 1, j = 0; j < newParams->length; i++, j++) {
            newArgV[i] = newParams->list[j]->value;
        }
        for (j = 1; j < argc; j++, i++) {
            newArgV[i] = argv[j];
        }
    }

    /* load the library's main function */ 
    mainPtr = loadSymbol( gGlkLib, "main", errBuf );
    if (NULL != *errBuf) {
        xpcFreeString( &libFullName );
        xpcFreeStringList( &newParams );
        if (newArgV != argv) {
            free( newArgV );
        }
        unloadLibrary( gGlkLib, errBuf );
        return 0;
    }

    /* call the library's main function */
    retVal = (*mainPtr)( newArgC, newArgV );

    if (0 != retVal) {
        unloadLibrary( gGlkLib, errBuf );
    }

    return retVal;
}

/**
 *   main() - parse the command line and config file, and load the Glk lib
 */

int main( int argc, char **argv )
{
    char * rcFilename = NULL;
    char * libName = NULL;
    BOOL isVerbose = FALSE;
    char * errBuf = NULL;
    xpcStringList * libList = NULL;
    int i, j;
    int newArgC;
    char ** newArgV;
    int glkParamIndex = -1;
    int rcParamIndex = -1;
    int vParamIndex = -1;
    int numDroppedParams = 0;
    int retVal;

    /* find the -glk and -rc parameters */
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
                if (NULL != rcFilename) {
                    usage( argv[0] );
                    exit( 1 );
                }
                rcFilename = argv[i];
            }
        } else if (strcmp( argv[i], "-v" ) == 0) {
            vParamIndex = i;
            numDroppedParams += 1;
            i++;
            isVerbose = TRUE;
        }
    }
    xpcSetExtensionData( APP_NAME, rcFilename, isVerbose );

    /* create a copy of argv without the -glk and -rc params */
    newArgC = argc - numDroppedParams;
    newArgV = (char **) malloc( newArgC * sizeof(char *) );
    for (i = 0, j = 0; j < argc; j++, i++) {
        if (j == glkParamIndex || j == rcParamIndex) {
            j += 2;
        } else if (j == vParamIndex) {
            j += 1;
        }
        if (j < argc) {
            newArgV[i] = argv[j];
        }
    }

    /* read the config file */
    xpcInitialize( APP_NAME );

    /* load the Glk library */
    if (NULL == libName) {
        libList = xpcReadStringList( APP_NAME, "Default", "libs" );
        if (NULL == libList) {
            error( "No Glk library was specified on the command line or config file." );
            xpcFinalize( APP_NAME );
            free( newArgV );
            exit( 1 );
        }
        for (i = 0; i < libList->length; i++) {
            libName = libList->list[i]->value;
            retVal = loadGlkLibrary( libName, newArgC, newArgV, &errBuf );
            if (NULL != errBuf) {
                error( errBuf );
            } else if (0 == retVal) {
                break;
            }
        }
        if (NULL != errBuf || 0 != retVal) {
            error( "Could not find a working Glk library." );
            xpcFreeStringList( &libList );
            xpcFinalize( APP_NAME );
               free( newArgV );
            exit( 1 );
        }
    } else {
        retVal = loadGlkLibrary( libName, newArgC, newArgV, &errBuf );
        if (NULL != errBuf) {
            error( errBuf );
        }
        if (NULL != errBuf || 0 != retVal) {
            error( "Could not load the specified Glk library." );
            xpcFinalize( APP_NAME );
            free( newArgV );
            exit( 1 );
        }
    }

    /* free the configuration data  */
    xpcFreeStringList( &libList );
    xpcFinalize( APP_NAME );

    /* unload the Glk library */
    free( newArgV );
    unloadLibrary( gGlkLib, &errBuf );
    if (NULL != errBuf) {
        error( errBuf );
        exit( 1 );
    }

    return retVal;
}
