/**
 * dispafuncs.c
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
#include <glk.h>
#include <gi_dispa.h>

#include "defines.h"

extern LIB_HANDLE gGlkLib;

/**
 *  Dispatch functions:
 *  gidispatch_set_object_registry
 *  gidispatch_get_objrock
 *  gidispatch_set_retained_registry
 *  
 *  :FIXME: should these be hard-coded here?
 */
void gidispatch_set_object_registry(
    gidispatch_rock_t (*regi)(void *obj, glui32 objclass), 
    void (*unregi)(void *obj, glui32 objclass, gidispatch_rock_t objrock))
{
    void (*funcPtr)(
        gidispatch_rock_t (*regi)(void *obj, glui32 objclass),
        void (*unregi)(void *obj, glui32 objclass, gidispatch_rock_t objrock)); 
    char * errBuf = NULL;

    funcPtr = loadSymbol(gGlkLib, "gidispatch_set_object_registry", &errBuf);
    if (NULL != errBuf) {
        fprintf(stderr, "%s\n", errBuf);
        exit(1);
    }

    (*funcPtr)(regi, unregi);
}

gidispatch_rock_t gidispatch_get_objrock(void *obj, glui32 objclass)
{
    gidispatch_rock_t (*funcPtr)(void *obj, glui32 objclass);
    char * errBuf = NULL;

    funcPtr = loadSymbol(gGlkLib, "gidispatch_get_objrock", &errBuf);
    if (NULL != errBuf) {
        fprintf(stderr, "%s\n", errBuf);
        exit(1);
    }

    return (*funcPtr)(obj, objclass);
}

void gidispatch_set_retained_registry(
    gidispatch_rock_t (*regi)(void *array, glui32 len, char *typecode), 
    void (*unregi)(void *array, glui32 len, char *typecode, 
        gidispatch_rock_t objrock))
{
    void (*funcPtr)(
        gidispatch_rock_t (*regi)(void *array, glui32 len, char *typecode),
        void (*unregi)(void *array, glui32 len, char *typecode,
            gidispatch_rock_t objrock)); 
    char * errBuf = NULL;

    funcPtr = loadSymbol(gGlkLib, "gidispatch_set_retained_registry", &errBuf);
    if (NULL != errBuf) {
        fprintf(stderr, "%s\n", errBuf);
        exit(1);
    }

    return (*funcPtr)(regi, unregi);
}
