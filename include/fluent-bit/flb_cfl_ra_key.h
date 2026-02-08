#ifndef FLB_CFL_RA_KEY_H
#define FLB_CFL_RA_KEY_H

#include <fluent-bit/flb_info.h>
#include <fluent-bit/flb_sds.h>
#include <fluent-bit/flb_regex.h>
#include <fluent-bit/record_accessor/flb_ra_parser.h>
#include <monkey/mk_core.h>
#include <cfl/cfl.h>

#include <stdbool.h>

enum cfl_ra_types {
    FLB_CFL_RA_BOOL = 0,
    FLB_CFL_RA_INT,
    FLB_CFL_RA_FLOAT,
    FLB_CFL_RA_STRING,
    FLB_CFL_RA_NULL
};

/* condition value types */
typedef union {
    bool boolean;
    int64_t i64;
    double f64;
    flb_sds_t string;
} cfl_ra_val;

/* Represent any value object */
struct flb_cfl_ra_value {
    int type;
    struct cfl_variant v;
    cfl_ra_val val;
};

struct flb_cfl_ra_value *flb_cfl_ra_key_to_value(flb_sds_t ckey,
                                                   struct cfl_variant vobj,
                                                   struct mk_list *subkeys);
void flb_cfl_ra_key_value_destroy(struct flb_cfl_ra_value *v);

int flb_cfl_ra_key_value_get(flb_sds_t ckey, struct cfl_variant vobj,
                             struct mk_list *subkeys,
                             cfl_sds_t *start_key,
                             cfl_sds_t *out_key, struct cfl_variant **out_val);

int flb_cfl_ra_key_strcmp(flb_sds_t ckey, struct cfl_variant vobj,
                          struct mk_list *subkeys, char *str, int len);
int flb_cfl_ra_key_regex_match(flb_sds_t ckey, struct cfl_variant vobj,
                               struct mk_list *subkeys, struct flb_regex *regex,
                               struct flb_regex_search *result);
int flb_cfl_ra_key_value_append(struct flb_ra_parser *rp, struct cfl_variant *vobj,
                                struct cfl_variant *in_val);
int flb_cfl_ra_key_value_update(struct flb_ra_parser *rp,  struct cfl_variant *vobj,
                                cfl_sds_t in_key, struct cfl_variant *in_val);
#endif
