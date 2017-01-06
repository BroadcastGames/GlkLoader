/**
 * applist.h
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

#ifndef _APPLIST_H_
#define _APPLIST_H_

#include "platform.h"
#include "sectionlist.h"
#include "keylist.h"

typedef struct _AppNode {
	const char * application;
	ExtensionData extensionData;
	SectionList sections;
	struct _AppNode * next;
} AppNode;

typedef AppNode * AppList;

void addAppToList( AppList * list, const char * application,
	SectionList sections, ExtensionData data );
AppList removeAppFromList( AppList * list, const char * application );
void setSectionsForApp( AppList * list, const char * application,
	SectionList sections );
SectionList findSectionsForApp( AppList list, const char * application );
void setExtensionDataForApp( AppList * list, const char * application,
	ExtensionData data );
ExtensionData findExtensionDataForApp( AppList list,
	const char * application );
void copyAppList( AppList from, AppList * to );
void destroyAppList( AppList * list );

#endif	/* _APPLIST_H_ */
