/**
 * linux_defines.c
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
#include <string.h>

#include "defines.h"

LIB_HANDLE loadLibrary( const char * filename, char ** error )
{
  LIB_HANDLE library;
  if (NULL == filename) {
    library = NULL;
    *error = "Empty file name passed to loadLibrary";
  } else {
    library = dlopen( filename, RTLD_LAZY | RTLD_GLOBAL ); 
    *error = dlerror();
  }
  return library;
}

int unloadLibrary( LIB_HANDLE library, char ** error )
{
  int result;
  if (NULL == library) {
    result = 0;
    *error = "Null handle passed to unloadLibrary";
  } else {
    result = dlclose( library );
    *error = dlerror();
  }
  return result;
}

SYM_HANDLE loadSymbol( LIB_HANDLE library, const char * symbol, char ** 
  error )
{
  SYM_HANDLE handle;
  if (NULL == library) {
    handle = NULL;
    *error = "Null handle passed to loadSymbol";
  } else if (NULL == symbol) {
    handle = NULL;
    *error = "Empty symbol name passed to loadSymbol";
  } else {
    handle = dlsym( library, symbol );
    *error = dlerror();
  }
  return handle;
}

void usage( const char * name )
{
    char * template = "Usage: %s [-glk <library>] [-rc <rcfile>] [library-specific options]";
    char * buf = (char *) malloc( (strlen(template) + strlen(name)) *
                 sizeof(char) );
    
    sprintf( buf, template, name );
    error( buf );
    free( buf );
}

void error( const char * message )
{
    fprintf( stderr, message );
    fprintf( stderr, "\n" );
}

char * systemDefaultConfigFile( void )
{
    char * homeDir = getenv( "HOME" );
    char * fileName = "/.glkloaderrc";
    char * buf = (char *) malloc( (strlen(homeDir) + strlen(fileName)) *
                 sizeof(char) );
    strcpy( buf, homeDir );
    strcat( buf, fileName );
    return buf;
    /* FIXME: memory leak */
}
