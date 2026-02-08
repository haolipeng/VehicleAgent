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
