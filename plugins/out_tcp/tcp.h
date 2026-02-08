#ifndef FLB_OUT_TCP_H
#define FLB_OUT_TCP_H

#include <fluent-bit/flb_info.h>

struct flb_out_tcp {
    /* Output format */
    int out_format;
    flb_sds_t raw_message_key;

    char *host;
    int port;

    /* Timestamp format */
    int       json_date_format;
    flb_sds_t json_date_key;
    flb_sds_t date_key;

    /* Upstream connection to the backend server */
    struct flb_upstream *u;

    struct flb_output_instance *ins;
};

#endif
