#ifndef FLB_OUT_TCP_CONF_H
#define FLB_OUT_TCP_CONF_H

#include <fluent-bit/flb_info.h>
#include <fluent-bit/flb_output.h>

#include "tcp.h"

struct flb_out_tcp *flb_tcp_conf_create(struct flb_output_instance *ins,
                                        struct flb_config *config);
void flb_tcp_conf_destroy(struct flb_out_tcp *ctx);

#endif
