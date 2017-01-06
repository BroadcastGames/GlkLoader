/**
 * keylist.c
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

#include <string.h>

#include "keylist.h"

void addKeyToList( KeyList * list, const char * key,
	const char * value )
{
	/* FIXME: check if key is already in list? */
	KeyNode * next = *list;

	*list = (KeyNode *) malloc( sizeof(KeyNode) );
	(*list)->key = strdup( key );
	(*list)->value = strdup( value );
	(*list)->next = next;
}

KeyList removeKeyFromList( KeyList * list, const char * key )
{
	KeyNode * node;
	KeyNode * next;
	
	node = *list;
	if (NULL == node) {
		return NULL;
	}
	
	if (strcasecmp( node->key, key ) ==0) {
		*list = node->next;
		node->next = NULL;
		return node;
	}

	for (next = node->next; NULL != next; next = next->next, node = node->next)
	{
		if (strcasecmp( next->key, key ) == 0) {
			node->next = next->next;
			next->next = NULL;
			return next;
		}
	}

	return NULL;
}

const char * findValueForKey( KeyList list, const char * key )
{
	KeyNode * node;
	for (node = list; NULL != node; node = node->next) {
		if (strcasecmp( node->key, key ) == 0) {
			return node->value;
		}
	}

	/* FIXME: key not in list.  What to do? */
	return NULL;
}

void copyKeyList( KeyList from, KeyList * to )
{
	KeyNode * node;
	KeyNode * next;

	*to = NULL;
	for (node = from; NULL != node; node = node->next) {
		next = *to;

		*to = (KeyNode *) malloc( sizeof(KeyNode) );
		(*to)->key = strdup( node->key );
		(*to)->value = strdup( node->value );
		(*to)->next = next;
	}
}

void destroyKeyList( KeyList * list )
{
	KeyNode * node;
	KeyNode * next;

	node = *list;
	if (NULL == node) {
		return;
	}
	
	*list = node->next;
	free( node->key );
	free( node->value );
	destroyKeyList( list );
	return;
}

void mergeKeys( KeyList list, const char * key, const char * value )
{
	char * oldValue;
	char * newValue;
	KeyNode * node;
	for (node = list; NULL != node; node = node->next) {
		if (strcasecmp( node->key, key ) == 0) {
			oldValue = node->value;
			newValue = malloc( strlen(oldValue) + strlen(value) + 2 *
				sizeof(char) );
			sprintf( newValue, "%s %s", value, oldValue );
			node->value = newValue;
			free( oldValue );
			return;
		}
	}
	return;
}

void replaceKeys( KeyList list, const char * key, const char * value )
{
	char * oldValue;
	KeyNode * node;
	for (node = list; NULL != node; node = node->next) {
		if (strcasecmp( node->key, key ) == 0) {
			oldValue = node->value;
			node->value = strdup( value );
			free( oldValue );
			return;
		}
	}
	return;
}
