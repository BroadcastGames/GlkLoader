/**
 * config.c
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

#include "config.h"
#include "defines.h"

const int BUFSIZE = 128;

typedef int Section;

#define SECTION_NONE    0
#define SECTION_LIBRARY 1
#define SECTION_DEFAULT 2

typedef struct _Node {
    char * name;
    char * path;
    int numParams;
    char ** params;
    struct _Node * next;
} Node;

void eatWhitespace( char ** buf )
{
        /* FIXME: check for '\0' or fall off end */
    char * s = *buf;
    while (' ' == *s || '\t' == *s || '\n' == *s) {
        ++s;
    }
    *buf = s;
}

char * eatHeader( char ** buf )
{
        /* FIXME: check for '\0' or fall off end */
    char * start = *buf;
    char * end;

    /* eat initial whitespace */
    eatWhitespace( &start );

    /* eat '[' */
    if ('[' != *start ) {
        error( "Invalid config file: section header does not begin with '['" );
        exit( 1 );
    } else {
        ++start;
    }

    /* find end of token */
    end = start;
    while (']' != *end && '\n' != *end) {
        ++end;
    }

    /* eat ']' and trailing whitespace */
    if ( ']' == *end ) {
        *end = '\0';
        ++end;
        eatWhitespace( &end );
    } else {
        error( "Invalid config file: section header does not end with ']'" );
        exit( 1 );
    }

    /* set return values */
    *buf = end;
    return start;
}

char * eatKey( char ** buf )
{
        /* FIXME: check for '\0' or fall off end */
    char * start = *buf;
    char * end;

    /* eat initial whitespace */
    eatWhitespace( &start );

    /* find end of token */
    end = start;
    while (' ' != *end && '\t' != *end && '\n' != *end && '=' != *end) {
        ++end;
    }

    /* eat '=' and trailing whitespace */
    if ( '=' == *end ) {
        *end = '\0';
        ++end;
        eatWhitespace( &end );
    } else {
        *end = '\0';
        ++end;
        eatWhitespace( &end );
        if ( '=' == *end ) {
            *end = '\0';
            ++end;
            eatWhitespace( &end );
        } else {
            error( "Invalid config file: key and value not separated by '='" );
            exit( 1 );
        }
    }

    /* set return values */
    *buf = end;
    return start;
}

char * eatValue( char ** buf )
{
        /* FIXME: check for '\0' or fall off end */
    char * start = *buf;
    char * end;

    /* eat initial whitespace */
    eatWhitespace( &start );

    /* find end of token */
    end = start;
    while (' ' != *end && '\t' != *end && '\n' != *end) {
        ++end;
    }

    /* eat trailing whitespace */
    *end = '\0';
    ++end;
    eatWhitespace( &end );

    /* set return values */
    *buf = end;
    return start;
}

void parseLine( char * line, char ** key, char ** value, char ** header )
{
    char * rover;
    char * token1;
    char * token2;

    /* initialize return values to NULL */
    *key = NULL;
    *value = NULL;
    *header = NULL;

    /* eat initial whitespace */
    rover = line;
    eatWhitespace( &rover );

    /* check for comment or empty line */
    if ('#' == *rover || '\0' == *rover) {
        return;
    }

    /* check for header */
    if ('[' == *rover) {
        /* read the header token */
        token1 = eatHeader( &rover );
        /* check for end of line */
        if ('\0' != *rover) {
            error( "Invalid config file: extra characters after section header" );
            exit( 1 );
        } else {
            *header = token1; 
        }
        return;
    }

    /* read the key token */
    token1 = eatKey( &rover );
    /* check for premature end of line */
    if ('\0' == *rover) {
        error( "Invalid config file: key with no value" );
        exit( 1 );
    } else {
        /* read the value token */
        token2 = eatValue( &rover );
        /* check for end of line */
        if ('\0' != *rover) {
            error( "Invalid config file: extra characters after key/value pair" );
            exit( 1 );
        } else {
            *key = token1;
            *value = token2;
        }
    }
    return;
}

void parseConfigFile( char * fileName, char ** defaultLib, Node ** libList )
{
    
    char buf[BUFSIZE];  /* FIXME: buffer overrun */
    FILE * file = NULL;
    Section section = SECTION_NONE;
    Node * node = NULL;
    char * key;
    char * value;
    char * header;

    /* initialize return values to NULL */
    *defaultLib = NULL;
    *libList = NULL;

    /* open the file for reading */
    file = fopen( fileName, "r" );
    if (NULL == file || feof( file )) {
        error( "Cannot open config file: ignoring" );
        return;
    }

    while (!feof( file )) {

        /* read the next line */
        if (NULL == fgets( buf, BUFSIZE, file )) {
            break;
        }

        /* parse the line */
        parseLine( buf, &key, &value, &header );

        /* check for blank line */
        if (NULL == key && NULL == value && NULL == header) {
            continue;
        }

        /* check for section header */
        if (NULL != header) {
            if (strcasecmp( header, "Default" ) == 0 ||
                strcasecmp( header, "Defaults" ) == 0)
            {
                section = SECTION_DEFAULT;
                /* FIXME: check for duplicate [Default] sections */
            } else if (strcasecmp( header, "Library" ) == 0 ||
                       strcasecmp( header, "Lib" ) == 0)
            {
                Node * newNode = (Node *) malloc( sizeof(Node) );
                memset( newNode, 0, sizeof(Node) );
                newNode->next = node;
                node = newNode;
                section = SECTION_LIBRARY;
            } else {
                error( "Invalid config file: unrecognized section" );
                exit( 1 );
            }
            continue;
        }

        /* deal with key/value pair */
        switch (section) {
        case SECTION_NONE:
            error( "Invalid config file: key/value pair found outside section" );
            exit( 1 );
            return;
        case SECTION_LIBRARY:
            if (strcasecmp( key, "name" ) == 0) {
                node->name = (char *) malloc( strlen(value) * sizeof(char) );
                strcpy( node->name, value );
            } else if (strcasecmp( key, "path" ) == 0) {
                node->path = (char *) malloc( strlen(value) * sizeof(char) );
                strcpy( node->path, value );
/*            } else if (strcasecmp( key, "params" ) == 0) {   */
                /* FIXME: deal with params */
            } else {
                error( "Invalid config file: unrecognized key in [Library] section" );
                exit( 1 );
            }
            break;
        case SECTION_DEFAULT:
            if (strcasecmp( key, "library" ) == 0 ||
                strcasecmp( key, "lib" ) == 0)
            {
                *defaultLib = (char *) malloc( strlen(value) * sizeof(char) );
                strcpy( *defaultLib, value );
            } else {
                error( "Invalid config file: unrecognized key in [Default] section" );
                exit( 1 );
            }
            break;
        default:
            error( "Internal error: bad section number" );
            exit( 1 );
            break;
        }

    }

    *libList = node;
}

Node * searchList( Node * list, char * key )
{
    Node * n;
    if (NULL == key) {
        return NULL;
    }
    for (n = list; NULL != n; n = n->next) {
        if (strcmp( n->name, key ) == 0) {
            return n;
        }
    }
    return NULL;
}

void freeList( Node * list )
{
    Node * n;
    Node * next;
    int i;
    for (n = list; NULL != n; n = next) {
        free( n->name );
        free( n->path );
        for (i = 0; i < n->numParams; i++) {
            free( n->params[i] );
        }
        next = n->next;
        free( n );
    }
}

void readConfig( char * libName, char * rcFileName, char ** libFullName,
                 int * numNewParams, char *** newParams )
{
    char * defaultLib;
    Node * libList;
    Node * lib;
    int i;

    /* parse the config file */
    if (NULL == rcFileName) {
            /*FIXME: look in home directory */
        rcFileName = systemDefaultConfigFile();
    }
    parseConfigFile( rcFileName, &defaultLib, &libList );

    /* find the default library if necessary */
    if (NULL == libName) {
        libName = defaultLib;
    }
    
    /* find the settings of the named library */
    lib = searchList( libList, libName );
    if (NULL != lib) {
        *libFullName = (char *) malloc( strlen(lib->path) * sizeof(char) );
        strcpy( *libFullName, lib->path );
        *numNewParams = lib->numParams;
        *newParams = (char **) malloc( *numNewParams * sizeof(char *) );
        for (i = 0; i < *numNewParams; i++) {
            (*newParams)[i] = (char *) malloc( strlen(lib->params[i]) *
                sizeof(char) );
            strcpy( (*newParams)[i], lib->params[i] );
        }
    } else if (NULL != libName) {
        *libFullName = (char *) malloc( strlen(libName) * sizeof(char) );
        strcpy( *libFullName, libName );
        *numNewParams = 0;
        *newParams = NULL;
    } else {
        *libFullName = NULL;
        *numNewParams = 0;
        *newParams = NULL;
    }

    /* free the data allocated in parseConfigFile */
    free( defaultLib );
    freeList( libList ); 
}

void freeConfigData( char ** libFullName, int * numNewParams,
                     char *** newParams )
{
    int i;
    free( *libFullName );
    for (i = 0; i < *numNewParams; i++) {
        free( (*newParams)[i] );
    }
    free( *newParams );
}
