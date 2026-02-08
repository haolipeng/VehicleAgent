#ifndef FLB_URI_H
#define FLB_URI_H

#include <monkey/mk_core.h>
#include <fluent-bit/flb_info.h>
#include <fluent-bit/flb_sds.h>

/* By default we allow a maximum of 8 URI patterns in our map */
#define FLB_URI_MAX       8

struct flb_uri_field {
    size_t length;
    char *value;
    struct mk_list _head;
};

struct flb_uri {
    char *full;                    /* Original full URI */
    uint8_t count;                 /* Number of entries in the list */
    struct mk_list list;           /* List head for uri patterns    */
    struct flb_uri_field *map;     /* Map / O(1) lookup by position */
};

static inline int flb_uri_to_encode(char c)
{
    if ((c >= 48 && c <= 57)  ||  /* 0-9 */
        (c >= 65 && c <= 90)  ||  /* A-Z */
        (c >= 97 && c <= 122) ||  /* a-z */
        (c == '?' || c == '&' || c == '-' || c == '_' || c == '.' ||
         c == '~' || c == '/' || c == '=')) {
        return FLB_FALSE;
    }

    return FLB_TRUE;
}

struct flb_uri_field *flb_uri_get(struct flb_uri *uri, int pos);
struct flb_uri *flb_uri_create(const char *full_uri);
void flb_uri_destroy(struct flb_uri *uri);
void flb_uri_dump(struct flb_uri *uri);
flb_sds_t flb_uri_encode(const char *uri, size_t len);

#endif
