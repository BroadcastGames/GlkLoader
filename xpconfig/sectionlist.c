/**
 * sectionlist.c
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

#include "sectionlist.h"
#include "keylist.h"

void addSectionToList( SectionList * list, const char * section,
	KeyList keys )
{
	/* FIXME: check if section is already in list? */
	SectionNode * next = *list;
	*list = (SectionNode *) malloc( sizeof(SectionNode) );
	(*list)->section = strdup( section );
	copyKeyList( keys, &((*list)->keys) );
	(*list)->next = next;
}

SectionList removeSectionFromList( SectionList * list, const char * section )
{
	SectionNode * node;
	SectionNode * next;
	
	node = *list;
	if (NULL == node) {
		return NULL;
	}
	
	if (strcasecmp( node->section, section ) ==0) {
		*list = node->next;
		node->next = NULL;
		return node;
	}

	for (next = node->next; NULL != next; next = next->next, node = node->next)
	{
		if (strcasecmp( next->section, section ) == 0) {
			node->next = next->next;
			next->next = NULL;
			return next;
		}
	}

	return NULL;
}

KeyList findKeysForSection( SectionList list, const char * section )
{
	SectionNode * node;
	for (node = list; NULL != node; node = node->next) {
		if (strcasecmp( node->section, section ) == 0) {
			return node->keys;
		}
	}

	/* FIXME: section not in list.  What to do? */
	return NULL;
}

void copySectionList( SectionList from, SectionList * to )
{
	SectionNode * node;
	SectionNode * next;

	*to = NULL;
	for (node = from; NULL != node; node = node->next) {
		next = *to;
		*to = (SectionNode *) malloc( sizeof(SectionNode) );
		(*to)->section = strdup( node->section );
		copyKeyList( node->keys, &((*to)->keys) );
		(*to)->next = next;
	}
}

void destroySectionList( SectionList * list )
{
	SectionNode * node;
	SectionNode * next;
	
	node = *list;
	if (NULL == node) {
		return;
	}
	
	*list = node->next;
	free( node->section );
	destroyKeyList( &(node->keys) );

	destroySectionList( list );
	return;
}
