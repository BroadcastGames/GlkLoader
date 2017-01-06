/**
 * defines.h
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

#ifndef _DEFINES_H_
#define _DEFINES_H_

#ifdef LINUX
#include "linux_defines.h"
#endif // LINUX

/**
 * loadLibrary()
 * loads the library found in file 'filename', and returns a handle to it
 * if an error occurs, puts a human-readable error message in 'error' and
 *   returns NULL
 * if no error occurs, 'error' is NULL
 * if called more than once on the same filename, the same handle is returned
 *   and the reference count is increased
 */
LIB_HANDLE loadLibrary( const char * filename, char ** error );

/**
 * unloadLibrary()
 * decreases the reference count on the library with handle 'library' and
 *   unloads it if the count reaches 0
 * if an error occurs, puts a human-readable error message in 'error' and
 *   returns a non-zero error code
 * if no error occurs, 'error' is NULL and 0 is returned
 */
int unloadLibrary( LIB_HANDLE library, char ** error );

/**
 * loadSymbol()
 * loads the symbol 'symbol' from the library with handle 'library', and
 *   returns a handle to it
 * if an error occurs, puts a human-readable error message in 'error' and
 *   returns NULL
 * if no error occurs, 'error' is NULL
 * if the symbol is not found or if 'library' or 'symbol' are NULL, sets
 *   'error' to NULL and returns NULL
 */
SYM_HANDLE loadSymbol( LIB_HANDLE library, const char * symbol, char ** 
  error );

/**
 * usage()
 * prints a usage message, including the name of the program, 'name'
 */
void usage( const char * name );

/**
 * error()
 * prints the given error message
 */
void error( const char * message );

/**
 * systemDefaultConfigFile()
 * returns the name of the default configuration file for this system
 */
char * systemDefaultConfigFile( void );

#endif // _DEFINES_H_
