/*
 * platform_rcfile.c
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdio.h>

#include "xpconfig.h"
#include "platform.h"
#include "applist.h"

const int BUFSIZE = 128;

struct PlatformExtensionData {
	char * manualFilename;
    BOOL isVerbose;
};

extern AppList g_appList;


void xpcSetExtensionData( const char * application, const char * filename,
                          BOOL isVerbose )
{
	/* attach the filename to the app */
	ExtensionData data = (ExtensionData)
		malloc( sizeof(struct PlatformExtensionData) );
    if (NULL == filename) {
        data->manualFilename = NULL;
    } else {
	    data->manualFilename = strdup( filename );
    }
    data->isVerbose = isVerbose;
	setExtensionDataForApp( &g_appList, application, data );
}

void xpcSetVerbosity( const char * application, BOOL isVerbose )
{
}

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
/* FIXME:		error( "Invalid config file: section header does not begin
with '['" ); */
		return NULL;
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
/* FIXME:		error( "Invalid config file: section header does not end with ']'"
); */
		return NULL;
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
/* FIXME: 			error( "Invalid config file: key and value not separated by '='"
); */
			return NULL;
		}
	}

	/* set return values */
	*buf = end;
	return start;
}

char * eatValue( char ** buf )
{
	char * start = *buf;
	char * end;

	/* eat initial whitespace */
	eatWhitespace( &start );

	/* find end of token */
	end = start;
	while ('\0' != *end && '\n' != *end) {
		++end;
	}

	/* FIXME: eat trailing whitespace */
	*end = '\0';
/*
	++end;
	eatWhitespace( &end );
*/

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
/* FIXME:			error( "Invalid config file: extra characters after section
header" ); */
/* Could also be an error occured while parsing header */
		} else {
			*header = token1;
		}
		return;
	}

	/* read the key token */
	token1 = eatKey( &rover );
	/* check for premature end of line */
	if ('\0' == *rover) {
/* FIXME:		error( "Invalid config file: key with no value" ); */
/* could also be an error occured while parsing key */
	} else {
		/* read the value token */
		token2 = eatValue( &rover );
		/* check for end of line */
		if ('\0' != *rover) {
/* FIXME:			error( "Invalid config file: extra characters after key/value
pair" ); */
/* could also be an error occured while parsing value */
		} else {
			*key = token1;
			*value = token2;
		}
	}
	return;
}

void parseConfigFile( const char * filename, const char * application )
{
	char buf[BUFSIZE];  /* FIXME: buffer overrun */
	FILE * file;
	char * key;
	char * value;
	char * header;
	char * currentSection = NULL;
	const char * temp;
	KeyList keys = NULL;
	SectionList sections = NULL;
	BOOL sectionsInList = FALSE;
	ExtensionData ext;

    /* if we are in verbose mode, write the filename as it is processed */
	ext = findExtensionDataForApp( g_appList, application );
	if (NULL != ext && TRUE == ext->isVerbose) {
        fprintf( stderr, "Parsing %s\n", filename );
    }

	/* open the file for reading */
	file = fopen( filename, "r" );
	if (NULL == file || feof( file )) {
/* FIXME		error( "Cannot open config file: ignoring" ); */
		return;
	}

	/* get the list of sections to append to */
	sections = findSectionsForApp( g_appList, application );
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
			if (NULL != currentSection) {
				if (NULL != keys && !sectionsInList) {
					addSectionToList( &sections, currentSection, keys );
					keys = NULL;
				}
				free( currentSection );
    		}
			currentSection = strdup( header );
			keys = findKeysForSection( sections, currentSection );
			if (NULL == keys) {
				sectionsInList = FALSE;
			} else {
				sectionsInList = TRUE;
    		}
			continue;
		}

		/* deal with key/value pair */
		if (NULL == currentSection) {
/* FIXME			error( "Invalid config file: key/value pair found outside section"
); */
			return;
		} else {
			temp = findValueForKey( keys, key );
/* FIXME: should we merge or replace? */
			if (NULL == temp) {
				addKeyToList( &keys, key, value );
			} else {
#if 0
				mergeKeys( keys, key, value );
#else
				replaceKeys( keys, key, value );
#endif
			}
		}

	}

	/* add the last section to the list */
	if (NULL != currentSection) {
		if (NULL != keys) {
			if (!sectionsInList) {
				addSectionToList( &sections, currentSection, keys );
			}
			keys = NULL;
		}
		free( currentSection );
	}

	/* attach the section list to the app */
	setSectionsForApp( &g_appList, application, sections );

	return;
}

void getApplicationConfig( const char * application )
{
	char * filename;
	char * dirname;
	char buf[BUFSIZE];
	ExtensionData ext;
	ssize_t bytesRead;
	size_t bytesProcessed;
	int fd;
	off_t basep;
	struct dirent * entry;

	/* if there is a manual filename, use that */
	ext = findExtensionDataForApp( g_appList, application );
	if (NULL != ext && NULL != ext->manualFilename) {
		parseConfigFile( ext->manualFilename, application );
	} else {

		/* try the directory '/etc/application.d/' */
		dirname = malloc( (strlen(application) + 8) * sizeof(char) );
		sprintf( dirname, "/etc/%s.d", application );
		fd = open( dirname, O_RDONLY );
		if (-1 != fd) {
			basep = 0;
			do {
				bytesRead = getdirentries(fd, buf, BUFSIZE, &basep);
				bytesProcessed = 0;
				while (bytesProcessed < bytesRead) {
					entry = (struct dirent *) (buf + bytesProcessed);
                    /* filter out ., .., and swap files */
					if ('.' != entry->d_name[0])
					{
						filename = malloc( (strlen(dirname) +
							strlen(entry->d_name) + 2) * sizeof(char) );
						sprintf( filename, "%s/%s", dirname, entry->d_name );
						parseConfigFile( filename, application );
						free( filename );
					}
					bytesProcessed += entry->d_reclen;
                }
			} while (0 != bytesRead);
		}
		free( dirname );

		/* try '/etc/applicationrc' */
		filename = malloc( (strlen(application) + 8) * sizeof(char) );
		sprintf( filename, "/etc/%src", application );
		parseConfigFile( filename, application );
		free( filename );

		/* try '$HOME/.applicationrc' */
		dirname = getenv( "HOME" );
		if (NULL == dirname) {
			/* FIXME: is this an error?  In either case, skip this file if
			 * $HOME does not exist */
		} else {
			filename = malloc( (strlen(application) + strlen(dirname) + 5) *
				sizeof(char) );
			sprintf( filename, "%s/.%src", dirname, application );
			parseConfigFile( filename, application );
			free( filename );
			free( dirname );
		}
	}

}

void cleanupApplicationData( const char * application )
{
	/* nothing to do: extension data is destroyed along with the app list */
}

void copyExtensionData( ExtensionData from, ExtensionData * to )
{
	*to = (struct PlatformExtensionData *) malloc(
		sizeof(struct PlatformExtensionData) );
    memcpy( to, &from, sizeof(ExtensionData) );
}

void destroyExtensionData( ExtensionData * data )
{
	free( (*data)->manualFilename );
	free( *data );
	return;
}
