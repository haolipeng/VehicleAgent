#ifndef FLB_STR_H
#define FLB_STR_H

#include <fluent-bit/flb_info.h>
#include <fluent-bit/flb_macros.h>
#include <fluent-bit/flb_mem.h>

#include <stdlib.h>
#include <string.h>

static inline char *flb_strndup(const char *s, size_t n)
{
    char *str;

    str = (char *) flb_malloc(n + 1);
    if (!str) {
        return NULL;
    }
    memcpy(str, s, n);
    str[n] = '\0';

    return str;
}

static inline char *flb_strdup(const char *s)
{
    return flb_strndup(s, strlen(s));
}

/* emptyval checks whether a string has a non-null value "". */
static inline int flb_str_emptyval(const char *s)
{
    if (s != NULL && strcmp(s, "") == 0) {
        return FLB_TRUE;
    }
    return FLB_FALSE;
}

/*
 Trim the `c` character sequence to the right of the `*str` string and return a copy.
 * @param *str Source string;
 * @param c Character to be trimmed.
 * @returns a new string, which is a trimmed copy of `*str`.
*/
static inline char *flb_rtrim(const char *str, char c) {
    ssize_t pos = strlen(str);

    while(c == str[--pos]);

    if (pos < 0){
        return NULL;
    }

    return flb_strndup(str, pos+1);
}

#endif
