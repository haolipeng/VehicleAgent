#ifndef FLB_RA_PARSER_H
#define FLB_RA_PARSER_H

#include <fluent-bit/flb_info.h>
#include <fluent-bit/flb_sds.h>
#include <monkey/mk_core.h>

/* Stub structure for flb_ra_parser */
struct flb_ra_parser {
    int type;
    flb_sds_t key;
    struct mk_list subkeys;
    struct mk_list _head;
};

#endif /* FLB_RA_PARSER_H */
