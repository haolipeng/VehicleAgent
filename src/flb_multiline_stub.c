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

/*
 * Stub implementation for multiline functions.
 * These functions are disabled in this build configuration.
 */

#include <fluent-bit/flb_info.h>
#include <fluent-bit/flb_mem.h>
#include <fluent-bit/multiline/flb_ml.h>

int flb_ml_init(struct flb_config *config)
{
    (void) config;
    return 0;
}

int flb_ml_exit(struct flb_config *config)
{
    (void) config;
    return 0;
}

int flb_ml_type_lookup(char *str)
{
    (void) str;
    return -1;
}

struct flb_ml_parser *flb_ml_parser_create(struct flb_config *ctx,
                                           char *name,
                                           int type,
                                           char *match_str,
                                           int negate,
                                           int flush_ms,
                                           char *key_content,
                                           char *key_group,
                                           char *key_pattern,
                                           struct flb_parser *parser,
                                           char *parser_name)
{
    (void) ctx;
    (void) name;
    (void) type;
    (void) match_str;
    (void) negate;
    (void) flush_ms;
    (void) key_content;
    (void) key_group;
    (void) key_pattern;
    (void) parser;
    (void) parser_name;
    return NULL;
}

int flb_ml_parser_init(struct flb_ml_parser *ml_parser)
{
    (void) ml_parser;
    return -1;
}

int flb_ml_parser_destroy(struct flb_ml_parser *ml_parser)
{
    (void) ml_parser;
    return 0;
}

int flb_ml_rule_create(struct flb_ml_parser *ml_parser,
                       char *from_state,
                       char *regex,
                       char *to_state,
                       char *end_pattern)
{
    (void) ml_parser;
    (void) from_state;
    (void) regex;
    (void) to_state;
    (void) end_pattern;
    return -1;
}
