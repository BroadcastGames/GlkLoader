/**
 * xpconfig.c
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

#include <assert.h>
#include <string.h>

#include "xpconfig.h"

#include "platform.h"
#include "applist.h"
#include "sectionlist.h"
#include "keylist.h"
#include "tokenlist.h"

const char * SEPARATORS = " \t\n,;:";

AppList g_appList = NULL;

const char * findValue( const char * application, const char * section,
                        const char * key )
{
	SectionList sections;
	KeyList keys;
	const char * value;
	
	sections = findSectionsForApp( g_appList, application );
	if (NULL == sections) {
		/* application config file not initialized */
		return NULL;
	}

	keys = findKeysForSection( sections, section );
	if (NULL == keys) {
		/* section not present in config file */
		return NULL;
	}

	value = findValueForKey( keys, key );
	return value;
}

int tokenize( const char * str, TokenList * tokens )
{
	char * buf;
	char * ptr;
	char * token;
	int numTokens = 0;

	if ( NULL == str ) {
		return 0;
	}

	buf = strdup( str );

	token = strtok_r( buf, SEPARATORS, &ptr );
	while (NULL != token) {
		addTokenToList( tokens, token );
		numTokens++;
		token = strtok_r( NULL, SEPARATORS, &ptr );
	}

	free( buf );

	return numTokens;
}

BOOL convertToBool( const char * str )
{
	if (strcasecmp( str, "true" ) != 0 ||
		strcasecmp( str, "t" ) != 0 ||
		strcasecmp( str, "yes" ) != 0 ||
		strcasecmp( str, "y" ) != 0 ||
		strcasecmp( str, "1" ) != 0) 
	{
		return TRUE;
	}
	
	if (strcasecmp( str, "false" ) != 0 ||
		strcasecmp( str, "f" ) != 0 ||
		strcasecmp( str, "no" ) != 0 ||
		strcasecmp( str, "n" ) != 0 ||
		strcasecmp( str, "0" ) != 0) 
	{
		return FALSE;
	}

	/* FIXME: what to do in this case? */
	return FALSE;
}

int convertToInt( const char * str )
{
	/* FIXME: add error checking? */
	return atoi( str );
}

void xpcInitialize( const char * application )
{
	getApplicationConfig( application );
}

void xpcFinalize( const char * application )
{
	destroyAppList( &g_appList );
	cleanupApplicationData( application );
}

xpcString * xpcReadString( const char * application, const char * section,
                           const char * key )
{
	xpcString * obj;
	const char * value;
	
	value = findValue( application, section, key );
	if (value == NULL) {
		return NULL;
	}

	obj = (xpcString *) malloc( sizeof(xpcString) );
	obj->value = strdup( value );
	return obj;
}

xpcInt * xpcReadInt( const char * application, const char * section,
                     const char * key )
{
	xpcInt * obj;
	const char * value;
	int intValue;
	
	value = findValue( application, section, key );
	if (value == NULL) {
		return NULL;
	}

	intValue = convertToInt( value );

	obj = (xpcInt *) malloc( sizeof(xpcInt) );
	obj->value = intValue;
	return obj;
}

xpcBool * xpcReadBool( const char * application, const char * section,
                       const char * key )
{
	xpcBool * obj;
	const char * value;
	BOOL boolValue;
	
	value = findValue( application, section, key );
	if (value == NULL) {
		return NULL;
	}

	boolValue = convertToBool( value );

	obj = (xpcBool *) malloc( sizeof(xpcBool) );
	obj->value = boolValue;
	return obj;
}

xpcStringList * xpcReadStringList( const char * application,
                                   const char * section, const char * key )
{
	const char * value;
	TokenList tokens = NULL;
	TokenNode * tok;
	int numTokens;
	xpcStringList * list;
	int i;

	value = findValue( application, section, key );
	if (NULL == value) {
		return NULL;
	}

	numTokens = tokenize( value, &tokens );

	list = (xpcStringList *) malloc( sizeof(xpcStringList) );
	list->length = numTokens;
	list->list = (xpcString **) malloc( numTokens * sizeof(xpcString *) );

	for (i = 0, tok = tokens; i < numTokens; i++, tok = tok->next) {
		assert( NULL != tok );
		list->list[i] = (xpcString *) malloc( sizeof(xpcString) );
		list->list[i]->value = strdup( tok->token );
	}

	free( value );
	destroyTokenList( &tokens );

	return list;
}

xpcIntList * xpcReadIntList( const char * application, const char * section,
                             const char * key )
{
	const char * value;
	TokenList tokens = NULL;
	TokenNode * tok;
	int numTokens;
	xpcIntList * list;
	int i;

	value = findValue( application, section, key );
	if (NULL == value) {
		return NULL;
	}

	numTokens = tokenize( value, &tokens );

	list = (xpcIntList *) malloc( sizeof(xpcIntList) );
	list->length = numTokens;
	list->list = (xpcInt **) malloc( numTokens * sizeof(xpcInt *) );

	for (i = 0, tok = tokens; i < numTokens; i++, tok = tok->next) {
		assert( NULL != tok );
		list->list[i] = (xpcInt *) malloc( sizeof(xpcInt) );
		list->list[i]->value = convertToInt( tok->token );
	}

	free( value );
	destroyTokenList( &tokens );

	return list;
}

xpcBoolList * xpcReadBoolList( const char * application, const char * section, 
	                           const char * key )
{
	const char * value;
	TokenList tokens = NULL;
	TokenNode * tok;
	int numTokens;
	xpcBoolList * list;
	int i;

	value = findValue( application, section, key );
	if (NULL == value) {
		return NULL;
	}

	numTokens = tokenize( value, &tokens );

	list = (xpcBoolList *) malloc( sizeof(xpcBoolList) );
	list->length = numTokens;
	list->list = (xpcBool **) malloc( numTokens * sizeof(xpcBool *) );

	for (i = 0, tok = tokens; i < numTokens; i++, tok = tok->next) {
		assert( NULL != tok );
		list->list[i] = (xpcBool *) malloc( sizeof(xpcBool) );
		list->list[i]->value = convertToBool( tok->token );
	}

	free( value );
	destroyTokenList( &tokens );

	return list;
}

void xpcFreeString( xpcString ** obj )
{
	if (*obj && (*obj)->value) {
		free( (*obj)->value );
		(*obj)->value = 0;
	}
	if (*obj) {
		free( *obj );
		*obj = 0;
	}
}

void xpcFreeInt( xpcInt ** obj )
{
	if (*obj) {
		free( *obj );
		*obj = 0;
	}
}

void xpcFreeBool( xpcBool ** obj )
{
	if (*obj) {
		free( *obj );
		*obj = 0;
	}
}

void xpcFreeStringList( xpcStringList ** obj )
{
	int i;
	xpcString * elem;

	if (*obj) {
		for (i = 0; i < (*obj)->length; i++) {
			elem = (*obj)->list[i];
			if (elem && elem->value) {
				free( elem->value );
				elem->value = 0;
			}
			if (elem) {
				free( elem );
				elem = (*obj)->list[i] = 0;
			}
		}

		free( *obj );
		*obj = 0;
	}
}

void xpcFreeIntList( xpcIntList ** obj )
{
	int i;
	xpcInt * elem;

	if (*obj) {
		for (i = 0; i < (*obj)->length; i++) {
			elem = (*obj)->list[i];
			if (elem) {
				free( elem );
				elem = (*obj)->list[i] = 0;
			}
		}

		free( *obj );
		*obj = 0;
	}
}

void xpcFreeBoolList( xpcBoolList ** obj )
{
	int i;
	xpcBool * elem;

	if (*obj) {
		for (i = 0; i < (*obj)->length; i++) {
			elem = (*obj)->list[i];
			if (elem) {
				free( elem );
				elem = (*obj)->list[i] = 0;
			}
		}

		free( *obj );
		*obj = 0;
	}
}
