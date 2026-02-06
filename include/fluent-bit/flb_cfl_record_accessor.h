/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*  Fluent Bit
 *  ==========
 *  Copyright (C) 2015-2024 The Fluent Bit Authors
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef FLB_CFL_RECORD_ACCESSOR_H
#define FLB_CFL_RECORD_ACCESSOR_H

#include <fluent-bit/flb_info.h>
#include <fluent-bit/flb_sds.h>
#include <cfl/cfl.h>

/* Stub structure for flb_cfl_record_accessor */
struct flb_cfl_record_accessor {
    flb_sds_t pattern;
};

/* Stub function declarations */
static inline struct flb_cfl_record_accessor *flb_cfl_ra_create(char *str, int translate_env)
{
    (void) str;
    (void) translate_env;
    return NULL;
}

static inline void flb_cfl_ra_destroy(struct flb_cfl_record_accessor *ra)
{
    (void) ra;
}

static inline flb_sds_t flb_cfl_ra_translate(struct flb_cfl_record_accessor *ra,
                                              char *tag, int tag_len,
                                              struct cfl_variant var,
                                              struct cfl_list *subkeys)
{
    (void) ra;
    (void) tag;
    (void) tag_len;
    (void) var;
    (void) subkeys;
    return NULL;
}

#endif /* FLB_CFL_RECORD_ACCESSOR_H */
