/**
 * blorbfuncs.c
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
#include <gi_blorb.h>

#include "defines.h"

extern LIB_HANDLE gGlkLib;

/**
 *  Blorb functions:
 *  giblorb_set_resource_map
 *  giblorb_get_resource_map
 *  
 *  :FIXME: should these be hard-coded here?
 */

giblorb_err_t giblorb_set_resource_map(strid_t file)
{
    giblorb_err_t (*funcPtr)(strid_t file);
    char * errBuf = NULL;

    funcPtr = loadSymbol(gGlkLib, "giblorb_set_resource_map", &errBuf);
    if (NULL != errBuf) {
        fprintf(stderr, "%s\n", errBuf);
        exit(1);
    }

    return (*funcPtr)(file);
}

giblorb_map_t *giblorb_get_resource_map(void)
{
    giblorb_map_t * (*funcPtr)(void);
    char * errBuf = NULL;

    funcPtr = loadSymbol(gGlkLib, "giblorb_get_resource_map", &errBuf);
    if (NULL != errBuf) {
        fprintf(stderr, "%s\n", errBuf);
        exit(1);
    }

    return (*funcPtr)();
}
