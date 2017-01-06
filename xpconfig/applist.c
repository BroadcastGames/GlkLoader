/**
 * applist.c
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

#include "applist.h"
#include "sectionlist.h"

void addAppToList( AppList * list, const char * application,
	SectionList sections, ExtensionData data )
{
	/* FIXME: check if application is already in list? */
	AppNode * next = *list;
	
	*list = (AppNode *) malloc( sizeof(AppNode) );
	(*list)->application = strdup( application );
	if (NULL == data) {
		(*list)->extensionData = NULL;
	} else {
		copyExtensionData( data, &((*list)->extensionData) );
	}
	if (NULL == sections) {
		(*list)->sections = NULL;
	} else {
		copySectionList( sections, &((*list)->sections) );
	}
	(*list)->next = next;
}

AppList removeAppFromList( AppList * list, const char * application )
{
	AppNode * node;
	AppNode * next;
	
	node = *list;
	if (NULL == node) {
		return NULL;
	}
	
	if (strcasecmp( node->application, application ) ==0) {
		*list = node->next;
		node->next = NULL;
		return node;
	}

	for (next = node->next; NULL != next; next = next->next, node = node->next)
	{
		if (strcasecmp( next->application, application ) == 0) {
			node->next = next->next;
			next->next = NULL;
			return next;
		}
	}

	return NULL;
}

void setSectionsForApp( AppList * list, const char * application,
	SectionList sections )
{
	AppNode * node;
	SectionList temp;
	for (node = *list; NULL != node; node = node->next) {
		if (strcasecmp( node->application, application ) == 0) {
			temp = node->sections;
			copySectionList( sections, &(node->sections) );
			if (NULL != temp) {
				destroySectionList( &temp );
			}
			return;
		}
	}

	/* app not in list, so add it */
	addAppToList( list, application, sections, NULL );

	return;
}

SectionList findSectionsForApp( AppList list, const char * application )
{
	AppNode * node;
	for (node = list; NULL != node; node = node->next) {
		if (strcasecmp( node->application, application ) == 0) {
			return node->sections;
		}
	}
	return NULL;
}

void setExtensionDataForApp( AppList * list, const char * application,
	ExtensionData data )
{
	AppNode * node;
	ExtensionData temp;
	for (node = *list; NULL != node; node = node->next) {
		if (strcasecmp( node->application, application ) == 0) {
			temp = node->extensionData;
			copyExtensionData( data, &(node->extensionData) );
			if (NULL != temp) {
				destroyExtensionData( &temp );
			}
			return;
		}
	}

	/* app not in list, so add it */
	addAppToList( list, application, NULL, data );

	return;
}

ExtensionData findExtensionDataForApp( AppList list,
	const char * application )
{
	AppNode * node;
	for (node = list; NULL != node; node = node->next) {
		if (strcasecmp( node->application, application ) == 0) {
			return node->extensionData;
		}
	}
	return NULL;
}

void copyAppList( AppList from, AppList * to )
{
	AppNode * node;
	AppNode * next;

	*to = NULL;
	for (node = from; NULL != node; node = node->next) {
		next = *to;

		*to = (AppNode *) malloc( sizeof(AppNode) );
		(*to)->application = strdup( node->application );
		copyExtensionData( node->extensionData, &((*to)->extensionData) );
		copySectionList( node->sections, &((*to)->sections) );
		(*to)->next = next;
	}
}

void destroyAppList( AppList * list )
{
	AppNode * node;
	AppNode * next;
	
	node = *list;
	if (NULL == node) {
		return;
	}
	
	*list = node->next;
	free( node->application );
	if (NULL != node->extensionData) {
		destroyExtensionData( &(node->extensionData) );
	}
	destroySectionList( &(node->sections) );

	destroyAppList( list );
	return;
}
