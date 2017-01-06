/**
 * tokenlist.c
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

#include "tokenlist.h"

void addTokenToList( TokenList * list, const char * token )
{
	TokenNode * node;
	
	node = *list;
	if (NULL == node) {
		*list = (TokenNode *) malloc( sizeof(TokenNode) );
		(*list)->token = strdup( token );
		(*list)->next = NULL;
		return;
	}

	for (; NULL != node->next; node = node->next) {
		/* do nothing */
	}
		
	node->next = (TokenNode *) malloc( sizeof(TokenNode) );
	node->next->token = strdup( token );
	node->next->next = NULL;
	return;
}

TokenList removeTokenFromList( TokenList * list, const char * token )
{
	TokenNode * node;
	TokenNode * next;
	
	node = *list;
	if (NULL == node) {
		return NULL;
	}
	
	if (strcasecmp( node->token, token ) ==0) {
		*list = node->next;
		node->next = NULL;
		return node; 
	}

	for (next = node->next; NULL != next; next = next->next, node = node->next)
	{
		if (strcasecmp( next->token, token ) == 0) {
			node->next = next->next;
			next->next = NULL;
			return next;
		}
	}

	return NULL;
}

void copyTokenList( TokenList from, TokenList * to )
{
	TokenNode * node;
	TokenNode * next;

	*to = NULL;
	for (node = from; NULL != node; node = node->next) {
		next = *to;
		*to = (TokenNode *) malloc( sizeof(TokenNode) );
		(*to)->token = strdup( node->token );
		(*to)->next = next;
	}
}

void destroyTokenList( TokenList * list )
{
	TokenNode * node;
	TokenNode * next;
	
	node = *list;
	if (NULL == node) {
		return;
	}
	
	*list = node->next;
	free( node->token );

	destroyTokenList( list );
	return;
}
